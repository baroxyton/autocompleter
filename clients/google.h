#ifndef google_CLIENT_H
#define google_CLIENT_H
#include "makerequest.h"
#include "json.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using nlohmann::json;
vector<string> google_CLIENT(string search, string lang)
{
    vector<string> output;
    string google_URL = "https://www.google.com/complete/search?q=${SEARCH}&client=gws-wiz&hl=${LANG}&authuser=0";
    google_URL = google_URL.replace(google_URL.find("${SEARCH}"), sizeof("${SEARCH}") - 1, search);
    google_URL = google_URL.replace(google_URL.find("${LANG}"), sizeof("${LANG}") - 1, lang);
    string raw_reqout = makeRequest(google_URL);
    raw_reqout = raw_reqout.substr(19, raw_reqout.size()-20);
    while(raw_reqout.find("\\u003cb\\u003e")!=string::npos){
    raw_reqout = raw_reqout.replace(raw_reqout.find("\\u003cb\\u003e"),sizeof("\\u003cb\\u003e")-1,"");
    }
    while(raw_reqout.find("\\u003c\\/b\\u003e")!=string::npos){
    raw_reqout = raw_reqout.replace(raw_reqout.find("\\u003c\\/b\\u003e"),sizeof("\\u003c\\/b\\u003e")-1,"");
    }
    //output.push_back(raw_reqout);
    //return output;
    json reqout = json::parse(raw_reqout);
    json autocompletes = reqout[0];
    for (int i = 0; i < autocompletes.size(); i++)
    {
        string autocomplete = autocompletes[i][0];
        output.push_back(autocomplete);
    }
    return output;
}
#endif