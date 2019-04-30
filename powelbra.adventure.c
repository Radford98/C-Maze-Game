/* Brad Powell
 * CS344_400 Operating Systems
 * Program 2
 * 04/29/2019
 * This program finds the most recent rooms directory and loads in those rooms. It then
 * creates a game that allows the player to move from room to room until they find the
 * end room.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

struct room {
	int id;
	char* name;
	int numOutConn;
	struct room* outConn[6];
	char* connList;	// String list of connected rooms for easy printing
	int roomType;	// 0 for start, 1 for mid, 2 for end

};

void NewestDir(struct room[]);
void CreateRooms(struct room[]);


int main() {
	// Create an array of structs for holding the rooms
	struct room rooms[7];
	// Find the newest directory and create the rooms
	NewestDir(rooms);

	return 0;
}

// Function which accepts an array of 7 room structs. It finds the newest directory with a given prefix
// and pulls the files out of the directory to create room structs for the array.
// Uses the code from 2.4 Manipulating Directories as a model.
void NewestDir(struct room rooms[]) {
	int newestDirTime = -1;				// Variable to hold and compare timestamps of directories
	char targetDirPrefix[20] = "powelbra.rooms.";	// Prefix of directories to search
	char newestDirName[256];			// Holds the name of the newest directory found
	memset(newestDirName, '\0', sizeof(newestDirName));

	DIR* dirToCheck;	// Holds the directory to search, namely, the current directory
	struct dirent *fileInDir;	// Holds the files/folders in the current directory for checkings
	struct stat dirAtt;	// Holds information about the subdir

	dirToCheck = opendir(".");	// Open the current directory

	if (dirToCheck > 0) {	// If the directory opened successfully
		while ((fileInDir = readdir(dirToCheck)) != NULL) {	// Check each entry in dirToCheck
			if (strstr(fileInDir->d_name, targetDirPrefix) != NULL) { // If entry has the prefix
				stat(fileInDir->d_name, &dirAtt);	// Get attributes of the entry

				if ((int)dirAtt.st_mtime > newestDirTime) {	// If this time is more recent
					newestDirTime = (int)dirAtt.st_mtime;
					memset(newestDirName, '\0', sizeof(newestDirName));	// Reset string
					strcpy(newestDirName, fileInDir->d_name);	// Set to newest dir
				}
			}
		}
	}
	closedir(dirToCheck);	// After searching, close the current directory
	dirToCheck = opendir(newestDirName);	// Open the newest directory

	if (dirToCheck > 0) {
		while ((fileInDir = readdir(dirToCheck)) != NULL) {
			if (strlen(fileInDir->d_name) > 2){	// Ignore . and .. directories
				char fileName[50];
				memset(fileName, '\0', sizeof(fileName));
				sprintf(fileName, "%s/%s", newestDirName, fileInDir->d_name);
				FILE* roomFile = fopen(fileName, "r");

				// Print room name
				char fileLine[50];
				memset(fileLine, '\0', sizeof(fileLine));
				fscanf(roomFile, "%*s %*s %s", fileLine);
				printf("%s\n", fileLine);

				// Check connections, set fileLine to connection to start
				fscanf(roomFile, "%s", fileLine);
				while (strcmp("CONNECTION", fileLine) == 0) {	// If the next line is CONNECTION
					char line[10];
					fscanf(roomFile, "%s %s", line, fileLine);
					printf("Connection %s %s\n", line, fileLine);
					fscanf(roomFile, "%s", fileLine);	// Set fileLine to next line
				}
				fscanf(roomFile, "%*s %s", fileLine);	// skip "type" and save name
				printf("Room type: %s\n", fileLine);
			}
		}
	}
}











