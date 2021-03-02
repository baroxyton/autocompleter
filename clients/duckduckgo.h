#ifndef DDG_CLIENT_H
#define DDG_CLIENT_H
#include "makerequest.h"
#include "json.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using nlohmann::json;
vector<string> DDG_CLIENT(string search, string lang)
{
    string DDG_URL = "https://duckduckgo.com/ac/?q=${SEARCH}&kl=${LANG}";
    vector<string> output;
    DDG_URL = DDG_URL.replace(DDG_URL.find("${SEARCH}"), sizeof("${SEARCH}") - 1, search);
    DDG_URL = DDG_URL.replace(DDG_URL.find("${LANG}"), sizeof("${LANG}") - 1, lang);
    string rqoutput = makeRequest(DDG_URL);
    json jsonOutput = json::parse(rqoutput);
    for (int i = 0; i < jsonOutput.size(); i++)
    {
        json item = jsonOutput[i];
        string phrase = item["phrase"];
        output.push_back(phrase);
    }
    return output;
}
#endif