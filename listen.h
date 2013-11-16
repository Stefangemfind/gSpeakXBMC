#ifndef LISTEN_H
#define	LISTEN_H

#if !defined(_WIN32_WCE)
#include <signal.h>
#include <setjmp.h>
#endif

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <sphinxbase/cont_ad.h>
#include <pocketsphinx/pocketsphinx.h>
#include <string>
#include <ctime>

#include "interpret.h"
#include "config.h"
#include "speak.h"

class listen{
    
public:
    void run(char *);
    
private:
    void recognize_from_microphone();
    void updateTimestamp();
    int checkState(std::string);
    
    enum STATE {ACTIVE, SLEEPING};
    STATE state;
    
    interpret i;
    config c;
    speak s;
    
};

#endif	/* LISTEN_H */

