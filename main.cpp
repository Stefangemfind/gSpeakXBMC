#include "listen.h"

int main(int argc, char* argv[]){

    if(argc != 1){
        cout << "This application does not accept arguments." << endl;
        exit(0);
    }
    
    class listen l;
    l.run(argv[0]);
    
    return 0;
}

