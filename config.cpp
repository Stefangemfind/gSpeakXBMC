#include "config.h"

using namespace std;

config::config(){
    iniFile = "/root/settings.ini";
}

std::string config::getValue(std::string parent, std::string value){
    bool foundParent = false;
    bool foundValue = false;
    
    std::string returnString;
    std::string loop;
    std::string compare;
    
    ifstream in(iniFile.c_str());
    
    while(std::getline(in, loop)) {
        if(loop == parent){
            foundParent = true;
        }
        if(foundParent == true && foundValue != true){
            compare = loop.substr(0, value.length());
            if(compare == value){
                foundValue = true;
                returnString = loop.substr(value.length() + 1);
            }
        }
    }
    
    in.close();
    
    return returnString;
}