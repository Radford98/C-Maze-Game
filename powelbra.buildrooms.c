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

struct room {
	int id;
	char* name;
	int numOutConn;	// Counter to keep track of connections to other rooms
	struct room* outConn[6];	// Array of pointers to other rooms.
	int roomType;	// 0 for start, 1 for mid, 2 for end
	int ready;	// Set to 1 after the room has 3 connections
};

// Function prototypes
int IsGraphFull();
void AddRandomConnection();
struct room GetRandomRoom();
int CanAddConnectionFrom(struct room);
int ConnectionAlreadyExists(struct room, struct room);
void ConnectRoom(struct room, struct room);
int IsSameRoom(struct room, struct room);

int main() {
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

	printf("Hello!\n");




	return 0;
}

// Create all connections in the graph. The loop checks if the graph is full, and
// if it isn't, add a new connection between rooms.
/*
while (IsGraphFull() == 0) {
	AddRandomConnection();
}
*/
// Returns 1 (true) if all rooms have 3-6 outbound connections, 0 (false) otherwise
int IsGraphFull()
{

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