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
	char name[15];
	int numOutConn;
	char outConn[6][15];	// Array of strings for room names
	char connList[80];		// String list of connected rooms for easy printing
	char roomType[15];

};

void CreateRooms(struct room[]);


int main() {
	// Create an array of structs for holding the rooms
	struct room rooms[7];
	// Find the newest directory and create the rooms
	CreateRooms(rooms);

	int i;
	for (i = 0; i<7; i++) {
		printf("Room %d: %s\n", rooms[i].id+1, rooms[i].name);
		printf("All connections: %s\n", rooms[i].connList);
		int j;
		for (j = 0; j < rooms[i].numOutConn; j++) {
			printf("Connection %d: %s\n", j+1, rooms[i].outConn[j]);
		}
		printf("Room type: %s\n", rooms[i].roomType);
	}

	return 0;
}

// Function which accepts an array of 7 room structs. It finds the newest directory with a given prefix
// and pulls the files out of the directory to create room structs for the array.
// Uses the code from 2.4 Manipulating Directories as a model.
void CreateRooms(struct room rooms[]) {
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


	// Begin creating rooms
	dirToCheck = opendir(newestDirName);	// Open the newest directory
	int roomID = 0;		// Set room ID for use with the array

	if (dirToCheck > 0) {
		while ((fileInDir = readdir(dirToCheck)) != NULL) {	// Check each file in subdir
			if (strlen(fileInDir->d_name) > 2){	// Ignore . and .. directories
				rooms[roomID].id = roomID;	// set the ID

				// Since files are being opened in another directory, the file location/name
				// must be constructed
				char fileName[50];
				memset(fileName, '\0', sizeof(fileName));
				sprintf(fileName, "%s/%s", newestDirName, fileInDir->d_name);
				FILE* roomFile = fopen(fileName, "r");

				// Get room name, ignore the first two words in the file (ROOM NAME:)
				// and assign the actual name to the struct
				char fileLine[20];
				memset(fileLine, '\0', sizeof(fileLine));
				fscanf(roomFile, "%*s %*s %s", fileLine);
				strcpy(rooms[roomID].name, fileLine);

				// Read connections connections, set fileLine to first connection to start
				fscanf(roomFile, "%s", fileLine);
				int count = 0;
				memset(rooms[roomID].connList, '\0', sizeof(rooms[roomID].connList));
				while (strcmp("CONNECTION", fileLine) == 0) {	// If the next line is CONNECTION
					fscanf(roomFile, "%*s %s", fileLine);	// Skip "#:" and get room
					strcpy(rooms[roomID].outConn[count], fileLine); // Set room to latest outConn
					strcat(rooms[roomID].connList, fileLine);	// Add connection to list
					strcat(rooms[roomID].connList, ", ");
					fscanf(roomFile, "%s", fileLine);	// Set fileLine to next line
					count++;				// Increment counter
				}
				rooms[roomID].numOutConn = count;	// Set number of outbound connections
				int listLen = strlen(rooms[roomID].connList);	// Format end of conn list
				rooms[roomID].connList[listLen - 2] = 46;	// set to .
				rooms[roomID].connList[listLen - 1] = 0;	// replace space with \0
				fscanf(roomFile, "%*s %s", fileLine);	// skip "type" and save name
				strcpy(rooms[roomID].roomType, fileLine);
				roomID++;	// Increment room counter
			}
		}
	}
	


}











