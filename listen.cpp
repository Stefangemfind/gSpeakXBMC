#include "listen.h"

jmp_buf jbuf;
ps_decoder_t *ps;
cmd_ln_t *config;

static const arg_t cont_args_def[] = {
    POCKETSPHINX_OPTIONS,
    /* Argument file. */
    { "-argfile",
      ARG_STRING,
      NULL,
      "Argument file giving extra arguments." },
    { "-adcdev",
      ARG_STRING,
      NULL,
      "Name of audio device to use for input." },
    { "-infile",
      ARG_STRING,
      NULL,
      "Audio file to transcribe." },
    { "-time",
      ARG_BOOLEAN,
      "no",
      "Print word times in file transcription." },
    CMDLN_EMPTY_OPTION
};

static void sleep_msec(int32 ms) {
#if (defined(WIN32) && !defined(GNUWINCE)) || defined(_WIN32_WCE)
    Sleep(ms);
#else
    /* ------------------- Unix ------------------ */
    struct timeval tmo;

    tmo.tv_sec = 0;
    tmo.tv_usec = ms * 1000;

    select(0, NULL, NULL, NULL, &tmo);
#endif
}

void listen::recognize_from_microphone(){

    ad_rec_t *ad;
    int16 adbuf[4096];
    int32 k, ts, rem;
    char buffer[128];
    char const *hyp;
    char const *uttid;
    cont_ad_t *cont;
    state = SLEEPING;
    FILE* pipe = popen(c.getValue("[General]", "Hcidump").c_str(), "r");
    std::string bufferStr;
    std::size_t found;
    
    if((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"), (int)cmd_ln_float32_r(config, "-samprate"))) == NULL)
        E_FATAL("Failed to open audio device\n");

    /* Initialize continuous listening module */
    if((cont = cont_ad_init(ad, ad_read)) == NULL)
        E_FATAL("Failed to initialize voice activity detection\n");
    if(ad_start_rec(ad) < 0)
        E_FATAL("Failed to start recording\n");
    if(cont_ad_calib(cont) < 0)
        E_FATAL("Failed to calibrate voice activity detection\n");

    while(!feof(pipe) || state == SLEEPING){
        fgets(buffer, 128, pipe);
        bufferStr = buffer;

        found = bufferStr.find(c.getValue("[General]", "KeyPress"));
        if(found!=std::string::npos){
            i.pauseIfPlaying();
            s.speakThis(c.getValue("[General]", "WakeUpPhrase"));
            state = ACTIVE;
            while(state != SLEEPING){
            
                /* Indicate listening for next utterance */
                printf("READY....\n");
                fflush(stdout);
                fflush(stderr);

                /* Wait data for next utterance */
                while ((k = cont_ad_read(cont, adbuf, 4096)) == 0){
                    sleep_msec(100);
                }

                if (k < 0)
                    E_FATAL("Failed to read audio\n");

                /*
                 * Non-zero amount of data received; start recognition of new utterance.
                 * NULL argument to uttproc_begin_utt => automatic generation of utterance-id.
                 */
                if (ps_start_utt(ps, NULL) < 0)
                    E_FATAL("Failed to start utterance\n");
                ps_process_raw(ps, adbuf, k, FALSE, FALSE);
                printf("Listening...\n");
                fflush(stdout);

                /* Note timestamp for this first block of data */
                ts = cont->read_ts;

                /* Decode utterance until end (marked by a "long" silence, >1sec) */
                for(;;){
                //while(sleep(2)){
                    /* Read non-silence audio data, if any, from continuous listening module */
                    if ((k = cont_ad_read(cont, adbuf, 4096)) < 0)
                        E_FATAL("Failed to read audio\n");
                    if (k == 0){
                        /*
                         * No speech data available; check current timestamp with most recent
                         * speech to see if more than 1 sec elapsed.  If so, end of utterance.
                         */
                        if ((cont->read_ts - ts) > DEFAULT_SAMPLES_PER_SEC)
                            break;
                    }
                    else {
                        /* New speech data received; note current timestamp */
                        ts = cont->read_ts;
                    }

                    /*
                     * Decode whatever data was read above.
                     */
                    rem = ps_process_raw(ps, adbuf, k, FALSE, FALSE);

                    /* If no work to be done, sleep a bit */
                    if ((rem == 0) && (k == 0))
                        sleep_msec(20);
                }

                /*
                 * Utterance ended; flush any accumulated, unprocessed A/D data and stop
                 * listening until current utterance completely decoded
                 */
                ad_stop_rec(ad);
                while (ad_read(ad, adbuf, 4096) >= 0);
                cont_ad_reset(cont);

                printf("Stopped listening, please wait...\n");
                fflush(stdout);

                /* Finish decoding, obtain and print result */
                ps_end_utt(ps);
                hyp = ps_get_hyp(ps, NULL, &uttid);

                fflush(stdout);

                /* Resume A/D recording for next utterance */
                if (ad_start_rec(ad) < 0)
                    E_FATAL("Failed to start recording\n");

                if(hyp != NULL){
                    
                    if(hyp == c.getValue("[General]", "Sleep")){
                        state = SLEEPING;
                        s.speakThis(c.getValue("[General]", "SleepPhrase"));
                    }else{
                        if(state != SLEEPING){
                            i.parse(hyp);
                        }
                        
                        //Hack for play/pause/select
                        std::string hypStr = hyp;
                        if(hypStr == "PLAY ITEM" || hypStr == "PAUSE ITEM" || hypStr == "SELECT ITEM"){
                            state = SLEEPING;
                        }
                    }
                }
            }
        }
    }
    pclose(pipe);
    cont_ad_close(cont);
    ad_close(ad);
}

static void sighandler(int signo){
    longjmp(jbuf, 1);
    signo = 0;
}

int listen::checkState(std::string phrase){
    int valid = 0;
    
    //If valid
    if(state == ACTIVE){
        valid = 1;
    }
    
    //Sleep
    if(state == ACTIVE && phrase == c.getValue("[General]", "Sleep")){
        state = SLEEPING;
        valid = 0;
        s.speakThis(c.getValue("[General]", "SleepPhrase"));
    }
    
    return valid;
}

void listen::run(char *application){

    //Init manually for now
    int argcnt = 5;
    char *arguments[argcnt];
    
    arguments[0] = new char[strlen(application)];
    strcpy(arguments[0], application);
    
    arguments[1] = new char[c.getValue("[General]", "Argument1").size()];
    strcpy(arguments[1], c.getValue("[General]", "Argument1").c_str());
    
    arguments[2] = new char[c.getValue("[General]", "Argument2").size()];
    strcpy(arguments[2], c.getValue("[General]", "Argument2").c_str());
    
    arguments[3] = new char[c.getValue("[General]", "Argument3").size()];
    strcpy(arguments[3], c.getValue("[General]", "Argument3").c_str());
    
    arguments[4] = new char[c.getValue("[General]", "Argument4").size()];
    strcpy(arguments[4], c.getValue("[General]", "Argument4").c_str());
    
    if (argcnt == 2) {
        config = cmd_ln_parse_file_r(NULL, cont_args_def, arguments[1], TRUE);
    }
    else {
        config = cmd_ln_parse_r(NULL, cont_args_def, argcnt, arguments, FALSE);
    }

    ps = ps_init(config);

    //E_INFO("%s COMPILED ON: %s, AT: %s\n\n", argv[0], __DATE__, __TIME__);

    /* Make sure we exit cleanly (needed for profiling among other things) */
    /* Signals seem to be broken in arm-wince-pe. */
    #if !defined(GNUWINCE) && !defined(_WIN32_WCE) && !defined(__SYMBIAN32__)
    signal(SIGINT, &sighandler);
    #endif

    if(setjmp(jbuf) == 0){
        recognize_from_microphone();
    }

    ps_free(ps);

}
