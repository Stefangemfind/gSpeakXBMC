#include "speak.h"
#include "config.h"

using namespace std;

void speak::initSpeak(){

    festival_initialize(atoi(c.getValue("[General]", "LoadInitFiles").c_str()), atoi(c.getValue("[General]", "HeapSize").c_str()));

    const char *c_str = c.getValue("[General]", "Voice").c_str();
    const EST_String Voice(c_str);
    festival_eval_command(c_str);

}

void speak::speakThis(string phrase){
    if(phrase == ""){
        return;
    }else{
        if(isAlive(c.getValue("[General]", "SpeechUrl")) == 1){
            speakGoogle(phrase);
        }else{
            speakFestival(phrase);
        }
    }
}

void speak::speakFestival(std::string phrase){
    const char *c_str = phrase.c_str();
    const EST_String Phrase(c_str);

    festival_say_text(Phrase);
}

void speak::speakGoogle(std::string phrase){
    std::string command = c.getValue("[General]", "SpeechScript") + " \"" + phrase + "\";";
    system(command.c_str());
}

int speak::isAlive(std::string site){
    
    std::string command = c.getValue("[General]", "Ping") + " -c 1 " + site;
    FILE* pipe = popen(command.c_str(), "r");
    std::string bufferStr;
    char buffer[128];
    std::size_t found;
    int alive = 0;
    
    while(!feof(pipe)) {
    	fgets(buffer, 128, pipe);
        bufferStr = buffer;
        
        found = bufferStr.find("1 packets transmitted, 1 received");
        if(found!=std::string::npos){
             alive = 1;
        }
        
    }
    pclose(pipe);
    
    return alive;
}
