struct wav_t{
  char                RIFF[4];        
  int                 ChunkSize;     
  char                WAVE[4];       
  char                fmt[4];        
  int                 Subchunk1Size;                              
  short int           AudioFormat;  
  short int           NumChan;      
  int                 SamplesPerSec;  
  int                 bytesPerSec;    
  short int           blockAlign;    
  short int           bitsPerSample;  
  char               *extra;
  char                Subchunk2ID[4];
  int                 Subchunk2Size; 
  short int          *data;
};

typedef struct wav_t WAV;

int little_endian_2(FILE *fptr);
int little_endian_4(FILE *fptr);
WAV *read_file(char *wavfile);
void sound_bite(WAV *inwav, WAV *bitewav, char *outfile, double time);

// Extra credit
void lo_pass(WAV *inwav, char *outfile, int freq);

