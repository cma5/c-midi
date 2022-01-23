unsigned int swap32 (unsigned int ic){
return (((ic >> 24) & 0xff) | ((ic << 8) & 0xff0000) | ((ic >> 8) & 0xff00) | ((ic << 24) & 0xff000000));
}

unsigned short swap16 (unsigned short ic){
return ((ic << 8) | (ic >> 8));
}

void showbits( unsigned char x )
{
	int i=0;
	for (i = (sizeof(unsigned char) * 8) - 1; i >= 0; i--)
	{
		putchar(x & (1u << i) ? '1' : '0');
	}
	printf("\n");
}

char* readString (FILE *f, int tlenght){
    unsigned char *puffer;
    puffer = malloc(sizeof(unsigned char)*(tlenght+1));
    int i =0;
    if(f != NULL){
        /*for (i; i>tlenght; i++){
            puffer[i]=fgetc(f);
        }
        puffer[i+1]=='\n';*/
        fread(puffer, sizeof(unsigned char), tlenght, f);
        puffer[tlenght]='\0';
        return(puffer);
        //printf("%s", puffer);
    }
    else{
        printf("Fileopen non worko\n");
        return(puffer);
    }
 }

 unsigned int readValue (FILE* f){
     unsigned int* nValue;
     unsigned char* nByte;
     nValue = malloc(sizeof(unsigned int));
     nByte = malloc(sizeof(unsigned char));
     *nByte = 0;
     *nValue = 0;

     // read byte
     fread(nValue, sizeof(unsigned char), 1, f);
     if (*nValue & 0x80){
         *nValue &= 0x7F;

         do{
             fread(nByte, sizeof(unsigned char), 1, f);
             *nValue = (*nValue << 7) | (*nByte & 0x7f);

         }
         while (*nByte & 0x80);
         
     }
     return *nValue;

 }