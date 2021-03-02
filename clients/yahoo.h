#ifndef yahoo_CLIENT_H
#define yahoo_CLIENT_H
#include "makerequest.h"
#include "json.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using nlohmann::json;
vector<string> yahoo_CLIENT(string search, string lang)
{
     vector<string> output;
    string yahoo_URL = "https://search.yahoo.com/sugg/gossip/gossip-us-ura/?command=${SEARCH}&output=sd1";
    yahoo_URL = yahoo_URL.replace(yahoo_URL.find("${SEARCH}"),sizeof("${SEARCH}")-1,search);
    string rqoutput = makeRequest(yahoo_URL);
    rqoutput = rqoutput.substr(0,rqoutput.size()-2);
    json jsonOutput = json::parse(rqoutput);
    json results = jsonOutput["r"];
    for(int i=0;i<results.size();i++){
        string result = results[i]["k"];
        output.push_back(result);
    }
    return output;
}
#endif