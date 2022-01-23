/* main.c of the midi-c program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../incl/helperfunctions.h"
//#include "../incl/ReadVarLen.h"
#include "../incl/parse_midi.h"

// In the next version the method parse midi should return a linked list of midi sequences for further processing.
// These are the prototypes and not relevant to the code since parse_midi is void for now.
/*struct MidiEvent {
	enum type{
		NoteOff,
		NoteOn,
		Other,
	} event;

	unsigned char e_key;
	unsigned char e_velocity;
	unsigned int e_WallTick;
	unsigned int e_DeltaTick;
	//struct MidiEvent *next;
};

struct MidiNote {
	unsigned int n_starttime;
	unsigned int n_lenght;
	unsigned char n_key;
	unsigned char n_velocity;
	//struct MidiNote *next;
};

struct MidiSequence {
	char *s_name;
	char *s_instrument;
	struct MidiNote *s_noten;
	struct MidiEvent *s_events;
	unsigned char s_minNote;
	unsigned char s_maxNote;
	//struct MidiSequence *next;
};

//struct MidiSequence *next = NULL;
//struct MidiSequence *start = NULL;
*/

int main(int argc, char **argv) {

	FILE *quelle;
	if (argc<2)
	{
		printf("No argument, specify the path your MIDI file is located.\n");
		return EXIT_FAILURE;
	}
	if (argc>2)
	{
		printf("To many arguments, this program only works with one MIDI file.\n");
		return EXIT_FAILURE;
	}
	
	quelle = fopen(argv[1], "r+b");
	parse_midi(quelle);

	return EXIT_SUCCESS;
}