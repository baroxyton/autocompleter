#include "readargs.h"
#include <string>
#include <iostream>
#include <vector>
#include <termios.h>
#include <fcntl.h>
#include <regex>
//import all avaliable clients
#include "clients/duckduckgo.h"
#include "clients/startpage.h"
#include "clients/yahoo.h"
#include "clients/google.h"
#include "clients/youtube.h"
static struct termios oldChars, newChars;
using namespace std;
//this function clears the terminal
int clear(void)
{
	cerr << "\x1B[2J\x1B[H";
	return 0;
}
//this function renders the menu when given
//all items and the currently selected item
int render(vector<string> items, int selected, string input)
{
	cerr << input + "_\n\n";
	//get number of items
	int totalItems = items.size();
	//literate through all item
	for (int i = 0; i < totalItems; i++)
	{
		//this ansi escape resets the styles
		string ansi = "\033[0m";
		//if the current item is the selected item, change the ansi escape to a bold font instead
		if (i == selected)
		{
			ansi = "\033[1m> ";
		}
		//ansi escpae + current item + newline
		string output = ansi + items[i] + "\n";
		//print the current item
		cerr << output;
	}
	return 0;
}
//this function can be used to asynchronously detect a keypress
int get_input()
{
	int character;
	//make the function nonblocking
	fcntl(0, F_SETFL, O_NONBLOCK);
	tcgetattr(0, &oldChars);
	newChars = oldChars;
	newChars.c_lflag &= ~ICANON;
	newChars.c_lflag &= 0 ? ECHO : ~ECHO;
	newChars.c_lflag &= ~ECHOE;
	tcsetattr(0, TCSANOW, &newChars);
	tcsetattr(0, TCSANOW, &oldChars);
	character = getchar();
	return character;
}
//returns all autocompletes in a vector of strings
vector<string> autocomplete(string search, string lang, string client)
{
	//decide which client to use based on the --client=x option
	if (client == "duckduckgo")
	{
		return DDG_CLIENT(search, lang);
	}
	if (client == "startpage")
	{
		return SP_CLIENT(search, lang);
	}
	if (client == "yahoo")
	{
		return yahoo_CLIENT(search, lang);
	}
	if (client == "google")
	{
		return google_CLIENT(search, lang);
	}
	if (client == "youtube")
	{
		return YT_CLIENT(search, lang);
	}
	//if invalid client was entered, default to duckduckgo
	return DDG_CLIENT(search, lang);
}
//this function is started when we run the program
int main(int argc, char *argv[])
{
	//the user input string that is used for searching. Escapes certain characters
	string searchInput = "";
	//input that is displayed to the user
	string displayInput = "";
	//the langage that will be used to autocomplete. Change with --lang="etc-ETC"
	string lang = "en-US";
	//the client that will be used to autocomplete. Change with --client=someClient.
	//valid options: youtube, yahoo, google, duckduckgo, startpage
	string client = "duckduckgo";
	//executed after autocompleting. change with --exec=command
	string exec = "#";
	bool showOutput = false;
	//process commandline arguments
	Arguments args = parseArgs(argc, argv);
	vector<Option> options = args.opts;
	int optionSize = options.size();
	for (int i = 0; i < optionSize; i++)
	{
		Option option = options[i];
		string key = option.key;
		if (key == "client")
		{
			client = option.value;
			continue;
		}
		if (key == "lang")
		{
			lang = option.value;
			continue;
		}
		if (key == "exec")
		{
			exec = option.value;
			continue;
		}
		if (key == "output")
		{
			showOutput = true;
			continue;
		}
		if (key == "help")
		{
			cerr << "\033[1mUtility for autocompletion\033[0m\n";
			cerr << "Options: \n";
			cerr << "--lang=language		choose the langauge to autocomplete in\n";
			cerr << "--client=client		choose the client to use for autocompletion. Valid options: duckduckgo, startpage, youtube, yahoo, google\n";
			cerr << "--exec=command			the command that will be executed when the user is done autocompleting. Example: --exec=\"echo {{{auto}}}\"\n";
			cerr << "--output		print the autocompleted string into stound for further processing. Exaple: touch \"file_$(autocompleter --output)\"\n";
			return 0;
		}
		cerr << "Invalid option: " + key;
		cerr << "\nValid options: lang, client\n";
		return -1;
	}
	//stores all autocompletes
	vector<string> items{};
	//currently selected item/autocomplete. Can be changed by navigating with the up- and downkey
	int selected = 0;
	//teporary store for the input id
	int input;
	//render screen first time
	clear();
	render(items, selected, displayInput);
	while (true)
	{
		input = get_input();
		string finalUserInput;
		switch (input)
		{
			//-1 means that no character was pressed
		case -1:
			break;
			//65 and 66 are arrow keys
		case 65:
			selected--;
			if (selected == -1)
			{
				selected = 0;
			}
			clear();
			render(items, selected, displayInput);
			break;
		case 66:
			selected++;
			if (selected == items.size())
			{
				selected = items.size() - 1;
			}
			clear();
			render(items, selected, displayInput);
			break;
			//enter key
		case 10:
			clear();
			if (items.size() == 0)
			{
				finalUserInput = displayInput;
			}
			else
			{
				finalUserInput = items[selected];
			}
			if (showOutput)
			{
				cout << finalUserInput;
			}
			if (exec.find("{{{auto}}}") != string::npos)
			{
				exec = exec.replace(exec.find("{{{auto}}}"), sizeof("{{{auto}}}") - 1, finalUserInput);
			}
			exec += "&";
			system(exec.c_str());
			return 0;
			break;
		case 263:
			//backspace. Broken for some reason
			displayInput.pop_back();
			clear();
			render(items, selected, displayInput);
			break;
		default:
			//none of the characters above. Assume it's letters
			char rawChar = input;
			string charString = string(1, rawChar);
			//sometimes there's random garbage, prevent crash by only allowing letters and some special characters
			regex isSearchable("[a-zA-Z0-9\\.\\?\\+\\-\\/\\:\\\"]");
			if (regex_match(charString, isSearchable))
			{
				displayInput = displayInput + rawChar;
				searchInput = searchInput + rawChar;
				clear();
				render(items, selected, displayInput);
				items = autocomplete(searchInput, lang, client);
				clear();
				render(items, selected, displayInput);
			}
			//space needs to be replaced by %20 for libcurl to work
			if (charString == " ")
			{
				searchInput += "%20";
				displayInput += " ";
			}
			break;
		}
	}
	return 0;
}
