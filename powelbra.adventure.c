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

void TestRooms(struct room[]);
void CreateRooms(struct room[]);
void RunAdventure(struct room[], struct room*);

int main() {
	int i;	// Index variable for loops.

	// Create an array of structs for holding the rooms
	struct room rooms[7];
	// Find the newest directory and create the rooms
	CreateRooms(rooms);
	
	// Room pointer to keep track of which room the player is in.
	struct room* curRoom;
	// Set the player's position to the start room
	for (i = 0; i < 7; i++) {
		if (strcmp(rooms[i].roomType, "START_ROOM") == 0) {
			curRoom = &rooms[i];
			break;	// No need to continue searching once the room is found
		}
	}

	RunAdventure(rooms, curRoom);
	
	return 0;
}

// This function accepts an array of 7 rooms and prints out their variables.
void TestRooms(struct room rooms[]) {
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
				fclose(roomFile);
			}
		}
	}
	
	closedir(dirToCheck);	

}

// Function to run the game. After creating some variables to help track things, runs a loop until
// the game is won. It prints some navigation for the user, waits for a new room to move to,
// checks if that room is valid, and moves if it is. Once the END_ROOM has been found, the game
// will end and the player's stats will be reported to them.
void RunAdventure(struct room rooms[], struct room* curRoom) {
	int i;		// Index variable
	int invalid;	// Variable for error checking
	int steps = 0;	// Count the nuumber of steps needed.
	char path[256];	// Path to victory
	memset(path, '\0', sizeof(path));
	char* input = NULL;	// Variables for getline
	size_t inputSize = 0;
	int gameEnd = 1; // Used by the loop to keep going until the game is won

	while (gameEnd) {
		// Report location
		printf("CURRENT LOCATION: %s\nPOSSIBLE CONNECTIONS: %s\nWHERE TO? >",
			curRoom->name, curRoom->connList);

		// Get the next room from the user
		getline(&input, &inputSize, stdin);
		printf("\n");

		// Check if the line entered maps to a valid room connection
		invalid = 1;
		for (i = 0; i < curRoom->numOutConn; i++) {
			if (strcmp(input, curRoom->outConn[i]) == 0) {	// If the names match
				int j;
				for (j = 0; j < 7; j++) {
					if (strcmp(input, rooms[j].name) == 0) { // Find the right room
						curRoom = &rooms[j];	// Switch to the new room
						break;
					}
				}
				invalid = 0;			// Let the program know the room was valid
				steps++;			// Increment step counter
				strcat(path, curRoom->name);	// Add the name to the path
				strcat(path, "\n");		// Add a newline
				break;	// Stop the loop from checking more than necessary
			}
		}

		// Print an error message if the provided name didn't match a room exactly.
		if (invalid == 1) {
			printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
		}

		// Check if the game is over





	}
	// Game won stuff goes here

}
