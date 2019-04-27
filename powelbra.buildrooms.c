/* Brad Powell
 * CS344_400 Operating Systems
 * Program 2
 * 04/24/2019
 * This program creates 7 rooms and writes them to files in a created directory for
 * later use by "adventure." One room is the start, another is the end. There are
 * random connections between the rooms.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


struct room {
	int id;
	char* name;
	int numOutConn;	// Counter to keep track of connections to other rooms
	struct room* outConn[6];	// Array of pointers to other rooms.
	int roomType;	// 0 for start, 1 for mid, 2 for end
};

// Function prototypes
int IsGraphFull(struct room[]);
void AddRandomConnection();
struct room GetRandomRoom();
int CanAddConnectionFrom(struct room);
int ConnectionAlreadyExists(struct room, struct room);
void ConnectRoom(struct room, struct room);
int IsSameRoom(struct room, struct room);
void RandomizeRooms(char**);

int main() {
	// Create a random seed based on the time
	srand(time(0));

	// Create names for rooms
	char* roomNames[10];
	roomNames[0] = "Foyer";
	roomNames[1] = "Bathroom";
	roomNames[2] = "Master";
	roomNames[3] = "Kitchen";
	roomNames[4] = "Dungeon";
	roomNames[5] = "Study";
	roomNames[6] = "Conservatory";
	roomNames[7] = "Attic";
	roomNames[8] = "Armory";
	roomNames[9] = "Chapel";

	// Randomize the room names for assignment.
	RandomizeRooms(roomNames);

	// Create room struct array and initialize them.
	struct room rooms[7];
	int i;
	for (i = 0; i < 7; i++) {
		rooms[i].id = i;
		rooms[i].name = roomNames[i+3];	// i+3 because selected names are at end of arrray
		rooms[i].numOutConn = 0;
		int j;
		for (j = 0; j < 6; j++) {
			rooms[i].outConn[j] = NULL;
		}
		rooms[i].roomType = 1;	// Set initial room type to mid
	}
	// Set start and end rooms.
	rooms[0].roomType = 0;
	rooms[6].roomType = 2;

	// Create all connections in the graph. The loop checks if the graph is full, and
	// if it isn't, add a new connection between rooms.
	while (IsGraphFull(rooms) == 0) {
		//AddRandomConnection();
	}


	return 0;
}

// Function which takes the array of room names and randomizes them. I remember learning this
// technique in a previous class, but not the name of the technique. It is similar to a heap
// sort in that a randomly chosen index is swapped with the last index, then the max number
// for rand is decreased by one so previously selected names aren't selected again.
// After the whole thing runs, the 7 chosen names will be in indexes [3]-[9].
void RandomizeRooms(char** array) {
	int select;	// variable to hold the randomly selected index
	char* temp;	// variable to temporary hold names for swapping
	int i;

	// Loop 7 times, moving the selected name to the back of the array
	// where it won't be touched by a decreasing rand 'max'.
	for (i = 0; i < 7; i++) {
		// rand() % 10 gives number 0-9. By subtracting i each iteration,
		// the remaining names is shortened.
		select = rand() % (10-i);
		temp = array[select];
		array[select] = array[9-i];	// Last index (9, 8, 7....)
		array[9-i] = temp;
	}	
}


// Returns 1 (true) if all rooms have 3-6 outbound connections, 0 (false) otherwise
// Loops through each room. Once one room is found to have less than 3 connections, return 
// false. If the loop completes, each room has at least 3 connections (other functions
// safeguard the max) and the graph is complete (return true).
int IsGraphFull(struct room rooms[]) {
	int i;
	for (i = 0; i < 7; i++) {
		if (rooms[i].numOutConn < 3) {
			return 0;
		}
	}
	return 1;	
}

// Adds a random, valid outbound connection from a Room to another Room
void AddRandomConnection() {
	struct room A;	// Maybe a struct, maybe global array of ints
	struct room B;

	while(1) {	// Keep this loop running until a valid room is selected
		A = GetRandomRoom();

		if (CanAddConnectionFrom(A) == 1)
			break;
	}

	// Set B to a random room and check if it's valid (not full, different from A, and it's a new connection)
	do {
		B = GetRandomRoom();
	} while(CanAddConnectionFrom(B) == 0 || IsSameRoom(A, B) == 1
		|| ConnectionAlreadyExists(A, B) == 1);

	ConnectRoom(A, B);	// TODO: Add this connection to the real variables,
	ConnectRoom(B, A);	// because this A and B will be destroyed when the function terminates
}

// Returns a random Room, but does NOT validate if connection can be added
struct room GetRandomRoom() {

}

// Returns 1 (true) if a connection can be added from Room x (<6 outbound connections), 0 (false) otherwise
int CanAddConnectionFrom(struct room x) {

}

// Returns 1 (true) if a connection from Room x to Room y already exists, 0 (false) otherwise
int ConnectionAlreadyExists(struct room x, struct room y) {

}

// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRoom(struct room x, struct room y) {

}

// Returns 1 (true) if Rooms x and y are the same room, 0 (false) otherwise
int IsSameRoom(struct room x, struct room y) {

}
