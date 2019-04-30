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


	return 0;
}
