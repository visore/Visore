#include <dirent.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#ifndef MAX_PATH
	#define MAX_PATH 8192
#endif

using namespace std;

void getdir(string directory, vector<string> &files)
{
	DIR *dp;
	struct dirent *dirp;

	if((dp = opendir(directory.c_str())) == NULL)
	{
		return;
	}

	while((dirp = readdir(dp)) != NULL)
	{
		string path(directory+"/"+string(dirp->d_name));
		int lastindex = path.find_last_of("/");
		string file = path.substr(lastindex + 1, strlen(dirp->d_name));
		if(file.find(".") == -1)
		{
			#ifdef WINDOWS
				files.push_back(path);
			#else
				char absolutePath[MAX_PATH];
				realpath(path.c_str(), absolutePath);
				files.push_back(absolutePath);
			#endif
			getdir(path, files);
		}
	}

	closedir(dp);
}

int main(int argc, char** argv)
{
	#if !(defined WINDOWS || defined MACOSX || defined LINUX)
        cout << "Visore is currently only supported on Windows, Linux and Mac OS X." << endl;
		return -1;
	#endif

	vector<string> files = vector<string>();
	string currentDirectory = "";
	#ifdef WINDOWS
		char buffer[MAX_PATH];
		getcwd(buffer, MAX_PATH);
		currentDirectory = string(buffer);
	#else
		currentDirectory = string(argv[0]);
		int lastindex = currentDirectory.find_last_of("/");
		currentDirectory = currentDirectory.substr(0, lastindex);
	#endif
	
	getdir(currentDirectory, files);
	
	string command = "";
	char separator;
	#ifdef WINDOWS
		separator = ';';
		command += "@echo off && cd " + currentDirectory + " && set PATH=%PATH%" + separator;
	#elif defined MACOSX
		separator = ':';
		command += "cd " + currentDirectory + " && export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH" + separator;
	#elif defined LINUX
		separator = ':';
		command += "cd " + currentDirectory + " && export LD_LIBRARY_PATH=$LD_LIBRARY_PATH" + separator;
	#endif

	for(int i = 0; i < files.size(); ++i)
	{
		command += files[i] + separator;
	}

	command += string(QTPATH) + separator;	

	#ifdef WINDOWS
		command += " && start visore.exe";
	#else
		command += " && ./visore";
	#endif

	if(argc == 1)
	{
		system(command.c_str());
	}
	else
	{
		string newCommand = "gnome-terminal --maximize ";
		string tabStart = "--tab -e \"bash -c \\\"";
		string tabEnd = "; exec bash\\\"\" ";

		char **values = argv + 1;
		int count = argc - 1;

		char* genres[] = {"-classical", "-country", "-electro", "-jazz", "-metal", "-pop", "-reggae", "-rock"};
		if(strcmp(argv[1], "all") == 0 || strcmp(argv[1], "-all") == 0)
		{
			values = genres;
			count = 8;
		}

		for(int i = 0; i < count; ++i)
		{
			string subCommand = command + " " + values[i];
			newCommand += tabStart + subCommand + tabEnd;
		}
		newCommand += " &";
		system(newCommand.c_str());
	}

	return 0;
}
