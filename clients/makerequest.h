#ifndef MAKEREQUEST_H
#define MAKEREQUEST_H
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <regex>
using namespace std;
static string output;
void curlCallback(char *dataPointer, size_t size, size_t count, void *userdata)
{  
    size_t calcSize = size * count;
    output.append(dataPointer, calcSize);
   // cerr << userdata;
}
string makeRequest(string url)
{
    output.clear();
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *request = curl_easy_init();
    curl_easy_setopt(request, CURLOPT_URL, url.c_str());
    curl_easy_setopt(request, CURLOPT_WRITEFUNCTION, curlCallback);
    curl_easy_perform(request);
    //cerr<<code;
    curl_easy_cleanup(request);
    request=NULL;
    regex validChar("[a-zA-Z0-9\\]\\[\\}\\{\\.\\?\\+\\-\\/\\:\\\"\\,\\ \\\\\\)\\(]");
    string filteredOutput;
    for(int i = 0; i<output.size();i++){
        if(regex_match(string(1,output[i]),validChar)){
            filteredOutput += output[i];
        }
        else{
            //replaces unknown characters with a "?", but this breaks yahoo for some reason.
            filteredOutput += "?";
        }
    }
    return filteredOutput;
}
#endif