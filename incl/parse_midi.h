//Midi events according to midi 1.0 spec. Only the first 4 bits are relevant. The last 4bits contain information about the channel
enum EventName {
	VoiceNoteOff = 0x80,
	VoiceNoteOn = 0x90,
	VoiceAftertouch = 0xA0,
	VoiceControlChange = 0xB0,
	VoiceProgramChange = 0xC0,
	VoiceChannelPressure = 0xD0,
	VoicePitchBend = 0xE0,
	SystemExclusive = 0xF0,
};

//Meta events according to midi 1.0 spec (custom ones are marked)
enum MetaEventName{
	MetaSequence = 0x00,
	MetaText = 0x01,
	MetaCopyright = 0x02,
	MetaTrackName = 0x03,
	MetaInstrumentName = 0x04,
	MetaLyrics = 0x05,
	MetaMarker = 0x06,
	MetaCuePoint = 0x07,
	MetaProgramName = 0x08,
	MetaDevicePort = 0x09,
	MetaChannelPrefix = 0x20,
	MetaMidiPort = 0x21,
	MetaEndOfTrack = 0x2F,
	MetaSetTempo = 0x51,
	MetaSMPTEOffset = 0x54,
	MetaTimeSignature = 0x58,
	MetaKeySignature = 0x59,
	MetaSequencerSpecific = 0x7F,
};

void parse_midi(FILE *f){

	unsigned int m_nTempo = 0;
	unsigned int m_nBPM = 0;

	// Buffers for different data types
	unsigned int *puf32;
	puf32 = malloc(sizeof(unsigned int));
	*puf32 = 0;
	unsigned short *puf16;
	puf16 = malloc(sizeof(unsigned short));
	*puf16 = 0;
	unsigned char *puf8;
	puf8 = malloc(sizeof(unsigned char));
	*puf8 = 0x00;

	//headerchunk
	printf("%.8x: ",ftell(f));
	printf("---------- Header Chunk [MThd] ----------\n");
	readString(f, 4);
	
	//chunk lenght
	printf("%.8x: ",ftell(f));
	fread(puf32, sizeof(unsigned int), 1, f);
	unsigned int chunk_lenght_header = swap32(*puf32);
	printf("Chunk Lenght:\t%2u\n", chunk_lenght_header);
	
	//format
	printf("%.8x: ",ftell(f));
	fread(puf16, sizeof(unsigned short), 1, f);
	unsigned short format = swap16(*puf16);
	printf("Format:\t%2u\n", chunk_lenght_header);
	
	//nTracks
	printf("%.8x: ",ftell(f));
	fread(puf16, sizeof(unsigned short), 1, f);
	unsigned short ntrks = swap16(*puf16);
	printf("nTracks:\t%2u\n", ntrks);
	
	//division
	printf("%.8x: ",ftell(f));
	fread(puf16, sizeof(unsigned short), 1, f);
	unsigned short division = swap16(*puf16);
	printf("Division:\t%2u\n",division);



	//trackchunks
	for(short ntrackc = 0; ntrackc < ntrks; ntrackc++){
		
		//MTrk
		printf("%.8x: ",ftell(f));
		printf("---------- Track Chunk %i [MTrk]---------\n", ntrackc+1); 
		readString(f, 4);

		//ChunkLenght
		printf("%.8x: ",ftell(f));
		fread(puf32, sizeof(unsigned int), 1, f);
		unsigned int chunk_lenght_track = swap32(*puf32);
		printf("chunk_lenght: %i\n", chunk_lenght_track);

		bool endofTrack = false;
		unsigned char previousStatus = 0;

		while (!feof(f) && !endofTrack)
		{
			printf("%.8x: ",ftell(f));
			// All Midi Events contain a time and a status
			unsigned int statustimedelta = 0;
			unsigned char status = 0;

			statustimedelta = readValue(f);
			fread(puf8, sizeof(unsigned char), 1, f);
			status = *puf8;

			// All MIDI status have the most significant byte set 
			// There's a concept in MIDI called running status which omits the status after it was once set. 
			// Not all MIDI Files do this but this code accounts for it, if done.

			if (status < 0x80)
			{
				status = previousStatus; //previously cached status overwrites the status that was wrongly read because of Midi running status
				fseek(f, -1L, SEEK_CUR); // We jump back one byte in our Stream
				//printf("MidiRunningStatus is used\n");
			}

			if((status & 0xF0) == VoiceNoteOff)
			{
				//channel
				previousStatus = status;
				unsigned char channel = status & 0x0F;
				//noteID
				fread(puf8, sizeof(unsigned char), 1, f);
				unsigned char noteID = *puf8;
				//velocity
				fread(puf8, sizeof(unsigned char), 1, f);
				unsigned char noteVelocity = *puf8;
				printf("Channel %3u: Note OFF\t@ dTime: %4u: [NoteID: %3u NoteVel: %3u]\n",channel,statustimedelta,noteID,noteVelocity);
			}
			else if((status & 0xF0) == VoiceNoteOn)
			{
				//channel
				previousStatus = status;
				unsigned char channel = status & 0x0F;
				//noteID
				fread(puf8, sizeof(unsigned char), 1, f);
				unsigned char noteID = *puf8;
				//velocity
				fread(puf8, sizeof(unsigned char), 1, f);
				unsigned char noteVelocity = *puf8;
				if (noteVelocity==0){//MIDI declares a Note ON Event with velocity 0 to a NoteOFF event
					printf("Channel %3u: Note OFF\t@ dTime: %4u: [NoteID: %3u NoteVel: %3u]\n",channel,statustimedelta,noteID,noteVelocity);
				}
				else{
					printf("Channel %3u: Note ON\t@ dTime: %4u: [NoteID: %3u NoteVel: %3u]\n",channel,statustimedelta,noteID,noteVelocity);
				}
			}
			else if((status & 0xF0) == VoiceAftertouch)
			{
				//channel
				previousStatus = status;
				unsigned char channel = status & 0x0F;
				//noteID
				fread(puf8, sizeof(unsigned char), 1, f);
				unsigned char noteID = *puf8;
				//velocity
				fread(puf8, sizeof(unsigned char), 1, f);
				unsigned char noteVelocity = *puf8;	
				printf("Channel %3u: Aftertouch\t@ dTime: %4u: [NoteID: %3u NoteVel: %3u]\n",channel,statustimedelta,noteID,noteVelocity);
			}
			else if((status & 0xF0) == VoiceControlChange)
			{
				//channel
				previousStatus = status;
				unsigned char channel = status & 0x0F;
				//controlID
				fread(puf8, sizeof(unsigned char), 1, f);
				unsigned char controlID = *puf8;
				//controlValue
				fread(puf8, sizeof(unsigned char), 1, f);
				unsigned char controlValue = *puf8;	
				printf("Channel %3u: ControlChange\t@ dTime: %4u: [CtrlID: %3u CtrlVal: %3u]\n",channel,statustimedelta,controlID,controlValue);
			}
			else if((status & 0xF0) == VoiceProgramChange){
				//channel
				previousStatus = status;
				unsigned char channel = status & 0x0F;
				//programID
				fread(puf8, sizeof(unsigned char), 1, f);
				unsigned char programID = *puf8;	
				printf("Channel %3u: ProgramChange\t@ dTime: %4u: [ProgID: %3u]\n",channel,statustimedelta,programID);
			}
			else if((status & 0xF0) == VoiceChannelPressure){
				//channel
				previousStatus = status;
				unsigned char channel = status & 0x0F;
				//channelPressure
				fread(puf8, sizeof(unsigned char), 1, f);
				unsigned char channelPressure = *puf8;
				printf("Channel %3u: ChannelPressure\t@ dTime: %4u: [ChannelPressure: %3u]\n",channel,statustimedelta,channelPressure);
			}
			else if((status & 0xF0) == VoicePitchBend){
				//channel
				previousStatus = status;
				unsigned char channel = status & 0x0F;
				//LSB
				fread(puf8, sizeof(unsigned char), 1, f);
				unsigned char LS7B = *puf8;
				//MSB
				fread(puf8, sizeof(unsigned char), 1, f);
				unsigned char MS7B = *puf8;
				printf("Channel %3u: PitchBend\t@ dTime: %4u: [LS7B: %3u MS7B: %3u]\n",channel,statustimedelta,LS7B,MS7B);
				
			}
			else if((status & 0xF0) == SystemExclusive){
				previousStatus = 0;

				if (status == 0xFF){
					//Meta Messagees
					fread(puf8, sizeof(unsigned char), 1, f);
					unsigned char type = *puf8;
					unsigned char lenght = readValue(f);

					switch (type)
					{
					case MetaSequence:
						fread(puf16, sizeof(unsigned short), 1, f);
						unsigned short sequenceNumber = swap16(*puf16);
						printf("Sequence Number: %i\n", sequenceNumber);
						break;
					case MetaText:
						printf("Text: %s\n", readString(f, lenght));
						break;
					case MetaCopyright:
						printf("Copyright: %s\n", readString(f, lenght));
						break;
					case MetaTrackName:
						printf("Track Name: %s\n", readString(f, lenght));
						break;
					case MetaInstrumentName:
						printf("Instrument Name: %s\n", readString(f, lenght));
						break;
					case MetaLyrics:
						printf("Lyrics: %s\n", readString(f, lenght));
						break;
					case MetaMarker:
						printf("Marker: %s\n", readString(f, lenght));
						break;
					case MetaCuePoint:
						printf("Cue: %s\n", readString(f, lenght));
						break;
					case MetaChannelPrefix:
						fread(puf8, sizeof(unsigned char), 1, f);
						printf("Channel Prefix: %i\n", *puf8);
						break;
					case MetaEndOfTrack:
						printf("End of Track\n");
						endofTrack = true;
						break;
					case MetaSetTempo:
						// Tempo in microseconds per quarternote
						if (m_nTempo == 0){
							fread(puf8, sizeof(unsigned char), 1, f);
							(m_nTempo |= (*puf8 << 16));
							fread(puf8, sizeof(unsigned char), 1, f);
							(m_nTempo |= (*puf8 << 8));
							fread(puf8, sizeof(unsigned char), 1, f);
							(m_nTempo |= (*puf8 << 0));
							m_nBPM = (60000000 / m_nTempo);
							printf("Tempo: %u; BPM: %u\n", m_nTempo, m_nBPM);

						}
						break;
					case MetaSMPTEOffset:
						printf("SPTPE: ");
						fread(puf8, sizeof(unsigned char), 1, f);
						printf("hr: %u; ", *puf8);
						fread(puf8, sizeof(unsigned char), 1, f);
						printf("mn: %u; ", *puf8);
						fread(puf8, sizeof(unsigned char), 1, f);
						printf("se: %u; ", *puf8);
						fread(puf8, sizeof(unsigned char), 1, f);
						printf("fr: %u; ", *puf8);
						fread(puf8, sizeof(unsigned char), 1, f);
						printf("ff: %u; ", *puf8);
						break;
					
					case MetaTimeSignature:
						printf("Time signature: ");
						fread(puf8, sizeof(unsigned char), 1, f);
						printf("%u/", *puf8);
						fread(puf8, sizeof(unsigned char), 1, f);
						printf("%u; ", (2<<*puf8));
						fread(puf8, sizeof(unsigned char), 1, f);
						printf("Clocks per tick: %u; ", *puf8);
						//A MIDI "BEAT" is 24 ticks, so specify how many 32nd notes constitute a beat
						fread(puf8, sizeof(unsigned char), 1, f);
						printf("32per24clocks: %u\n", *puf8);
						break;
					
					case MetaKeySignature:
						fread(puf8, sizeof(char), 1, f);
						printf("Key Signature: %u", *puf8);
						fread(puf8, sizeof(char), 1, f);
						printf("Minor Key: %u", *puf8);
						break;
					
					case MetaSequencerSpecific:
						printf("Sequencer Specific: %s\n", readString(f, lenght));
						break;
	
					
					default:
						printf("Unrecognized Metaevent %x\n", type);
						break;
					}
				}


				
			}
			else
			{
				printf("Unrecognized Status Byte: %x\n", status);
			}

			
		}
		

	}

	
}