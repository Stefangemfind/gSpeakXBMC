#ifndef CONFIG_H
#define	CONFIG_H

#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>

class config{
public:
    config();
    std::string getValue(std::string, std::string);
private:
    std::string iniFile;
};



#endif	/* CONFIG_H */
