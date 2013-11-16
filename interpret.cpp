#include "interpret.h"

using namespace std;

//Global string stream to store the cURL data
std::ostringstream stream;

//Process the signal
void interpret::parse(std::string signal){

    valid = false;
    
    //Check is screen is sleeping, is it is, wake it up
    if(screenSaverActive() == 1){
        commandUp(1);
    }
    
    //Play/Pause
    if(signal == "PLAY ITEM" || signal == "PAUSE ITEM"){
        commandPlayPause();
    }
    //Stop
    if(signal == "STOP ITEM"){
        commandStop();
    }
    //Select
    if(signal == "SELECT ITEM"){
        commandSelect();
    }
    //Back
    if(signal == "GO BACK"){
        commandBack();
    }
    //Home
    if(signal == "GO TO HOME"){
        commandHome();
    }
    //Up 1
    if(signal == "NAVIGATE UP ONE"){
        commandUp(1);
    }
    //Up 2
    if(signal == "NAVIGATE UP TWO"){
        commandUp(2);
    }
    //Up 3
    if(signal == "NAVIGATE UP THREE"){
        commandUp(3);
    }
    //Up 4
    if(signal == "NAVIGATE UP FOUR"){
        commandUp(4);
    }
    //Up 5
    if(signal == "NAVIGATE UP FIVE"){
        commandUp(5);
    }
    //Up 6
    if(signal == "NAVIGATE UP SIX"){
        commandUp(6);
    }
    //Up 7
    if(signal == "NAVIGATE UP SEVEN"){
        commandUp(7);
    }
    //Up 8
    if(signal == "NAVIGATE UP EIGHT"){
        commandUp(8);
    }
    //Up 9
    if(signal == "NAVIGATE UP NINE"){
        commandUp(9);
    }
    //Up 10
    if(signal == "NAVIGATE UP TEN"){
        commandUp(10);
    }
    //Down 1
    if(signal == "NAVIGATE DOWN ONE"){
        commandDown(1);
    }
    //Down 2
    if(signal == "NAVIGATE DOWN TWO"){
        commandDown(2);
    }
    //Down 3
    if(signal == "NAVIGATE DOWN THREE"){
        commandDown(3);
    }
    //Down 4
    if(signal == "NAVIGATE DOWN FOUR"){
        commandDown(4);
    }
    //Down 5
    if(signal == "NAVIGATE DOWN FIVE"){
        commandDown(5);
    }
    //Down 6
    if(signal == "NAVIGATE DOWN SIX"){
        commandDown(6);
    }
    //Down 7
    if(signal == "NAVIGATE DOWN SEVEN"){
        commandDown(7);
    }
    //Down 8
    if(signal == "NAVIGATE DOWN EIGHT"){
        commandDown(8);
    }
    //Down 9
    if(signal == "NAVIGATE DOWN NINE"){
        commandDown(9);
    }
    //Down 10
    if(signal == "NAVIGATE DOWN TEN"){
        commandDown(10);
    }
    //Left
    if(signal == "GO LEFT"){
        commandLeft();
    }
    //Right
    if(signal == "GO RIGHT"){
        commandRight();
    }
    //Movies
    if(signal == "GO TO MOVIES"){
        commandMovies();
    }
    //Tv Shows
    if(signal == "GO TO TV SHOWS"){
        commandTvShows();
    }
    //Volume up
    if(signal == "VOLUME UP"){
        commandVolumeUp();
    }
    //Volume down
    if(signal == "VOLUME DOWN"){
        commandVolumeDown();
    }
    //Mute/Unmute
    if(signal == "MUTE ITEM" || signal == "UNMUTE ITEM"){
        commandMuteUnmute();
    }
    //Youtube
    if(signal == "GO TO YOUTUBE"){
        commandYoutube();
    }
    //Seek forward
    if(signal == "SEEK FORWARD"){
        commandSeekForward();
    }
    //Seek backward
    if(signal == "SEEK BACKWARD"){
        commandSeekBackward();
    }
    //If method isn't found
    if(valid == false){
        cout << "No matching method found for: " << signal << endl;
    }
    
}

//Check is screensaver is running
int interpret::screenSaverActive(){
    int running = 0;
    std::string active;
    
    sendSignal("{\"jsonrpc\": \"2.0\", \"method\": \"XBMC.GetInfoBooleans\", \"params\": { \"booleans\": [\"System.ScreenSaverActive \"] }, \"id\": 1}", 1);
    active = curlResponse.substr(62, curlResponse.length() - 62);
    active = active.substr(0, active.length() - 2);
    
    if(active == "true"){
        running = 1;
    }
    
    return running;
}

//Pause the playback if it is playing
void interpret::pauseIfPlaying(){
    std::string active;
    
    //Check is screen is sleeping, is it is, wake it up
    if(screenSaverActive() == 1){
        commandUp(1);
    }
    
    sendSignal("{\"jsonrpc\": \"2.0\", \"method\": \"XBMC.GetInfoBooleans\", \"params\": { \"booleans\": [\"Player.Playing \"] }, \"id\": 1}", 1);
    active = curlResponse.substr(52, curlResponse.length() - 52);
    active = active.substr(0, active.length() - 2);
    
    cout << active << endl;
        
    if(active == "true"){
        this->commandPlayPause();
    }
    
}

//Get the base URL for the XBMC API
std::string interpret::getURLBase(){
    std::string base = "http://" +
        c.getValue("[General]", "XBMCUsername") + 
        ":" +
        c.getValue("[General]", "XBMCPassword") +
        "@" +
        c.getValue("[General]", "XBMCHostname") +
        ":" +
        c.getValue("[General]", "XBMCPort") +
        c.getValue("[General]", "XBMCAPIBase");
    
    return base;
}

//Catch the cURL output
size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
    
    std::ostringstream *stream = (std::ostringstream*)userdata;
    size_t count = size * nmemb;
    stream->write(ptr, count);
    
    return count;
}

//Send the cURL signal
void interpret::sendSignal(std::string query, int times){
    
    CURL *curl;
    CURLcode res;
 
    curl = curl_easy_init();
    if(curl){
 
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, query.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, getURLBase().c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream);

        for(int x = 1; x <= times; x++){
            res = curl_easy_perform(curl);
            if(res != CURLE_OK){
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
            }
            curlResponse = stream.str();
            stream.str("");
            stream.clear();
        }
        
        /* always cleanup */ 
        curl_easy_cleanup(curl);
    }
}

//Play/Pause command
void interpret::commandPlayPause(){
    sendSignal("{\"jsonrpc\": \"2.0\", \"method\": \"Player.PlayPause\", \"params\": { \"playerid\": 1}, \"id\": 0}", 1);
    valid = true;
}

//Stop command
void interpret::commandStop(){
    sendSignal("{\"jsonrpc\": \"2.0\", \"method\": \"Player.Stop\", \"params\": { \"playerid\": 1}, \"id\": 0}", 1);
    valid = true;
}

//Select command
void interpret::commandSelect(){
    sendSignal("{\"jsonrpc\": \"2.0\", \"method\": \"Input.Select\"}", 1);
    valid = true;
}

//Back command
void interpret::commandBack(){
    sendSignal("{\"jsonrpc\": \"2.0\", \"method\": \"Input.Back\"}", 1);
    valid = true;
}

//Home command
void interpret::commandHome(){
    sendSignal("{\"jsonrpc\": \"2.0\", \"method\": \"Input.Home\"}", 1);
    valid = true;
}

//Up command
void interpret::commandUp(int times){
    sendSignal("{\"jsonrpc\": \"2.0\", \"method\": \"Input.Up\"}", times);
    valid = true;
}

//Down command
void interpret::commandDown(int times){
    sendSignal("{\"jsonrpc\": \"2.0\", \"method\": \"Input.Down\"}", times);
    valid = true;
}

//Left command
void interpret::commandLeft(){
    sendSignal("{\"jsonrpc\": \"2.0\", \"method\": \"Input.Left\"}", 1);
    valid = true;
}

//Right command
void interpret::commandRight(){
    sendSignal("{\"jsonrpc\": \"2.0\", \"method\": \"Input.Right\"}", 1);
    valid = true;
}

//Movies command
void interpret::commandMovies(){
    sendSignal("{\"jsonrpc\": \"2.0\", \"method\": \"GUI.ActivateWindow\", \"params\": {\"window\": \"video\", \"parameters\": [ \"MovieTitles\" ]}}", 1);
    valid = true;
}

//Tv Shows command
void interpret::commandTvShows(){
    sendSignal("{\"jsonrpc\": \"2.0\", \"method\": \"GUI.ActivateWindow\", \"params\": {\"window\": \"video\", \"parameters\": [ \"TvShowTitles\" ]}}", 1);
    valid = true;
}

//Volume up command
void interpret::commandVolumeUp(){
    int volume;
    int newVolume;
    std::string volumeStr;
    
    sendSignal("{\"jsonrpc\":\"2.0\",\"method\":\"Application.GetProperties\",\"params\":{\"properties\":[\"volume\"]}, \"id\": 1}", 1);
    
    //Get the value
    volumeStr = curlResponse.substr(43, curlResponse.length() - 43);
    volumeStr = volumeStr.substr(0, volumeStr.length() - 2);
    
    //Convert volume string to int
    std::istringstream ssi(volumeStr);
    ssi >> volume;
    
    //Check volume for max value
    if(volume < 97){
        newVolume = volume + 3;
    }else{
        newVolume = 100;
    }
    
    //Convert back to string
    std::stringstream sss;
    sss << newVolume;
    
    //Send signal to change volume
    sendSignal("{\"jsonrpc\":\"2.0\",\"method\":\"Application.SetVolume\",\"params\": {\"volume\": " + sss.str() + "}, \"id\": 1}", 1);

    valid = 1;
}

//Volume down command
void interpret::commandVolumeDown(){
    int volume;
    int newVolume;
    std::string volumeStr;
    
    sendSignal("{\"jsonrpc\":\"2.0\",\"method\":\"Application.GetProperties\",\"params\":{\"properties\":[\"volume\"]}, \"id\": 1}", 1);
    
    //Get the value
    volumeStr = curlResponse.substr(43, curlResponse.length() - 43);
    volumeStr = volumeStr.substr(0, volumeStr.length() - 2);
    
    //Convert volume string to int
    std::istringstream ssi(volumeStr);
    ssi >> volume;
    
    //Check volume for min value
    if(volume > 3){
        newVolume = volume - 3;
    }else{
        newVolume = 0;
    }
    
    //Convert back to string
    std::stringstream sss;
    sss << newVolume;
    
    //Send signal to change volume
    sendSignal("{\"jsonrpc\":\"2.0\",\"method\":\"Application.SetVolume\",\"params\": {\"volume\": " + sss.str() + "}, \"id\": 1}", 1);

    valid = 1;
}

//Mute/Unmute command
void interpret::commandMuteUnmute(){
    sendSignal("{\"jsonrpc\": \"2.0\", \"method\": \"Application.SetMute\", \"params\": {\"mute\": \"toggle\"}, \"id\": \"1\"}", 1);
    valid = 1;
}

//Youtube command
void interpret::commandYoutube(){
    sendSignal("{\"jsonrpc\": \"2.0\",\"method\": \"Addons.ExecuteAddon\",\"params\": {\"wait\": false,\"addonid\": \"plugin.video.youtube\"},\"id\": 1}", 1);
    valid = 1;
}

//Seek forward command
void interpret::commandSeekForward(){
    sendSignal("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"Player.Seek\",\"params\":{\"playerid\":1,\"value\":\"smallforward\"}}", 1);
    valid = 1;
}

//Seek backward command
void interpret::commandSeekBackward(){
    sendSignal("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"Player.Seek\",\"params\":{\"playerid\":1,\"value\":\"smallbackward\"}}", 1);
    valid = 1;
}
