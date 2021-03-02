#ifndef READARGS_H
#define READARGS_H

//include libraries and stuff
#include <iostream>
#include <regex>
#include <vector>
#include <string>
//so I don't have to put "std::" in front of everything
using namespace std;
//class/template for option
class Option
{
public:
	//example:	--key=value
	string key, value;
};
//class/template for final argument list
//is returned by the parseArgs function
class Arguments
{
public:
	//example for flags: ls -la
	vector<string> flags;
	//example for options: chromium --app="https://google.com"
	vector<Option> opts;
	//example for args: ls myDir
	vector<string> args;
};
//function that parses arguments
//pass argc and argv from main function
Arguments parseArgs(int argc, char *argv[])
{
	//the result
	Arguments result;
	//regex that is used to parse options
	regex optWithValue("^--(([a-zA-Z0-9]){1,})=((.*){1,})$");
	regex optNoValue("^--(([a-zA-Z0-9]){1,})$");
	regex flagRegex("^-(([a-zA-Z0-9]){1,})$");
	//vector of all raw arguments
	vector<string> rawArguments(argv + 1, argv + argc);
	//loop through arguments
	for (int i = 0; i < rawArguments.size(); i++)
	{
		//check if argument is option
		smatch owvMatch;
		string rawArg = rawArguments[i];
		if (regex_search(rawArg.cbegin(), rawArg.cend(), owvMatch, optWithValue))
		{
			//create Option instane,
			Option output;
			//extract key and value,
			output.key = owvMatch[1];
			output.value = owvMatch[3];
			//and add it to the output
			result.opts.push_back(output);
			continue;
		}
		//check if its option without value
		smatch onvMatch;
		if (regex_search(rawArg.cbegin(), rawArg.cend(), onvMatch, optNoValue))
		{
			Option output;
			output.key = onvMatch[1];
			output.value = "";
			result.opts.push_back(output);
			continue;
		}
		smatch flagmatch;
		if (regex_search(rawArg.cbegin(), rawArg.cend(), flagmatch, flagRegex))
		{
			string flags = flagmatch[1];
			int length = flags.length();
			for (int j = 0; j < length; j++)
			{
				string flag;
				flag = flags[j];
				result.flags.push_back(flag);
			}
			continue;
		}
		//not a flag or option, assume its a normal argument
		result.args.push_back(rawArg);
	}
	return result;
}
//
#endif