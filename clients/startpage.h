#ifndef SP_CLIENT_H
#define SP_CLIENT_H
#include "makerequest.h"
#include "json.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using nlohmann::json;
vector<string> SP_CLIENT(string search, string lang)
{
    //startpage has a language parameter, but it does not influence the result
    string SP_URL = "https://startpage.com/suggestions?q=${SEARCH}";
    SP_URL = SP_URL.replace(SP_URL.find("${SEARCH}"), sizeof("${SEARCH}") - 1, search);
    string rqoutput = makeRequest(SP_URL);
    json jsonOutput = json::parse(rqoutput);
    json autocompletes = jsonOutput["suggestions"];
    vector<string> output;
    for (int i = 0; i < autocompletes.size(); i++)
    {
        string item = autocompletes[i]["text"];
        output.push_back(item);
    }
    return output;
}
#endif