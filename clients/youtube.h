#ifndef YT_CLIENT_H
#define YT_CLIENT_H
#include "makerequest.h"
#include "json.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using nlohmann::json;
vector<string> YT_CLIENT(string search, string lang)
{
    vector<string> output;
    string YT_URL = "https://suggestqueries-clients6.youtube.com/complete/search?client=youtube&hl=${LANG}&gs_rn=64&gs_ri=youtube&ds=yt&q=${SEARCH}";
    YT_URL = YT_URL.replace(YT_URL.find("${SEARCH}"), sizeof("${SEARCH}") - 1, search);
    YT_URL = YT_URL.replace(YT_URL.find("${LANG}"), sizeof("${LANG}") - 1, lang);
    string raw_reqout = makeRequest(YT_URL);
    raw_reqout = raw_reqout.substr(19, raw_reqout.size()-20);
    json reqout = json::parse(raw_reqout);
    json autocompletes = reqout[1];
    for (int i = 0; i < autocompletes.size(); i++)
    {
        string autocomplete = autocompletes[i][0];
        output.push_back(autocomplete);
    }
    return output;
}
#endif