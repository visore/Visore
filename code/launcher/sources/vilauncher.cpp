#include <dirent.h>
#include <vector>
#include <string>
#include <stdlib.h>
#include <string.h>

#ifndef MAX_PATH
	#define MAX_PATH 8192
#endif

using namespace std;

void getdir (string directory, vector<string> &files)
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
			files.push_back(path);
			getdir(path, files);
		}
	}

	closedir(dp);
}

int main()
{
	char buffer[MAX_PATH];
	getcwd(buffer, MAX_PATH);
	string currentDirectory = string(buffer);
	vector<string> files = vector<string>();
	getdir(currentDirectory, files);

	string command = "";
	char separator;
	#ifdef WIN32
		separator = ';';
		command += "@echo off && cd " + currentDirectory + " && set PATH=%PATH%" + separator;
	#else
		separator = ':';
		command += "cd " + currentDirectory + " && export LD_LIBRARY_PATH=$LD_LIBRARY_PATH" + separator;
	#endif

	for(int i = 0; i < files.size(); ++i)
	{
		command += files[i] + separator;
	}

	#ifdef WIN32
		command += " && start visore.exe";
	#else
		command += " && ./visore";
	#endif

	system(command.c_str());
    return 0;
}
