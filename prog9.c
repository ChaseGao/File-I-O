/* For this MP, I implemented the little_endian_2(),little_endian_4(),read_file(), sound_bite(), and lo_pass() functions.*/



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "prog9.h"

//These two funtions are used to reverse the read in process. These functions will output data to the WAV file in little endian format
void rev_little_endian_2(int i,FILE *output);
void rev_little_endian_4(int i,FILE *output);

int little_endian_2(FILE *fptr){
   
  //each char represents 8 bits, we read in 16 bits of data 
  unsigned char c0,c1;
  fscanf(fptr,"%c%c",&c0,&c1);

  //reverse the endian format of the data
  int i = 0;
  i += c1;
  i <<= 8;
  i += c0;
 
  


  return i;

}

int little_endian_4(FILE *fptr){

  //each char represents 8 bits, we read in 32 bits of data 
  unsigned char c0,c1,c2,c3;
  fscanf(fptr,"%c%c%c%c",&c0,&c1,&c2,&c3);

  //reverse the endian formate of the data
  int i = 0;
  i += c3;
  i <<= 8;
  i += c2;
  i <<= 8;
  i += c1;
  i <<= 8;
  i += c0;
  

  return i;

}

WAV *read_file(char *wavfile){

  //read in the data from the input WAV file
  FILE * fptr = fopen(wavfile,"r");
  WAV* wav_ptr = (WAV *)malloc(sizeof(WAV));
  wav_ptr -> extra = (char *)malloc(sizeof(char));
  unsigned char c0,c1,c2,c3;
  fscanf(fptr,"%c%c%c%c",&c0,&c1,&c2,&c3);
  wav_ptr -> RIFF[0] = c0;
  wav_ptr -> RIFF[1] = c1;
  wav_ptr -> RIFF[2] = c2;
  wav_ptr -> RIFF[3] = c3;
  wav_ptr -> ChunkSize = little_endian_4(fptr);
  fscanf(fptr,"%c%c%c%c",&c0,&c1,&c2,&c3);
  wav_ptr -> WAVE[0] = c0;
  wav_ptr -> WAVE[1] = c1;
  wav_ptr -> WAVE[2] = c2;
  wav_ptr -> WAVE[3] = c3;
  fscanf(fptr,"%c%c%c%c",&c0,&c1,&c2,&c3); 
  wav_ptr -> fmt[0] = c0;
  wav_ptr -> fmt[1] = c1;
  wav_ptr -> fmt[2] = c2;
  wav_ptr -> fmt[3] = c3;
  wav_ptr -> Subchunk1Size = little_endian_4(fptr);
  wav_ptr -> AudioFormat = little_endian_2(fptr);  
  wav_ptr -> NumChan = little_endian_2(fptr);  
  wav_ptr -> SamplesPerSec = little_endian_4(fptr);
  wav_ptr -> bytesPerSec = little_endian_4(fptr);
  wav_ptr -> blockAlign = little_endian_2(fptr);  
  wav_ptr -> bitsPerSample = little_endian_2(fptr);
  fscanf(fptr,"%c%c",&c0,&c1);
  wav_ptr -> extra[0] = c0;
  wav_ptr -> extra[1] = c1;
  fscanf(fptr,"%c%c%c%c",&c0,&c1,&c2,&c3); 
  wav_ptr -> Subchunk2ID[0] = c0;
  wav_ptr -> Subchunk2ID[1] = c1;
  wav_ptr -> Subchunk2ID[2] = c2;
  wav_ptr -> Subchunk2ID[3] = c3;
  wav_ptr -> Subchunk2Size = little_endian_4(fptr);
  wav_ptr->data = (short int*)malloc(((wav_ptr -> Subchunk2Size)/((wav_ptr -> bitsPerSample)/8))*sizeof(short int));
  
   //put the sound data into the data array
   int n;
  
   for(n=0;n<((wav_ptr -> Subchunk2Size)/((wav_ptr -> bitsPerSample)/8));n++) {
  
             wav_ptr -> data[n] = little_endian_2(fptr);
           
        
             
             }
  
       
  // print information reagrding the WAV file to the console
  printf("ChunkID: %c%c%c%c\n",wav_ptr -> RIFF[0],wav_ptr -> RIFF[1],wav_ptr -> RIFF[2],wav_ptr -> RIFF[3]);
  printf("ChunkSize: %d\n",wav_ptr -> ChunkSize);
  printf("Format: %c%c%c%c\n",wav_ptr -> WAVE[0],wav_ptr -> WAVE[1],wav_ptr -> WAVE[2],wav_ptr -> WAVE[3]);  
  printf("Subchunck1ID: %c%c%c%c\n",wav_ptr -> fmt[0],wav_ptr -> fmt[1],wav_ptr -> fmt[2],wav_ptr -> fmt[3]);  
  printf("Subchunk1Size: %d\n",wav_ptr -> Subchunk1Size);
  printf("AudioFormat: %d\n",wav_ptr -> AudioFormat);
  printf("NumChannels: %d\n",wav_ptr -> NumChan);  
  printf("SampleRate: %d\n",wav_ptr -> SamplesPerSec);    
  printf("ByteRate: %d\n",wav_ptr -> bytesPerSec);      
  printf("BlockAlign: %d\n",wav_ptr -> blockAlign);
  printf("BitsPerSample: %d\n",wav_ptr -> bitsPerSample);              
  printf("Subchunck2ID: %c%c%c%c\n",wav_ptr -> Subchunk2ID[0],wav_ptr -> Subchunk2ID[1],wav_ptr -> Subchunk2ID[2],wav_ptr -> Subchunk2ID[3]);   
  printf("Subchunk2Size: %d\n",wav_ptr -> Subchunk2Size);                  
  
  return wav_ptr;

}

void sound_bite(WAV *inwav, WAV *bitewav, char *outfile, double time){

      int i;  //used to store the data to be processed
      FILE * output = fopen(outfile,"w");  //creat a file pointer to the output WAV
      
      //output data to the WAV file
      fprintf(output,"%c%c%c%c",inwav -> RIFF[0],inwav -> RIFF[1],inwav -> RIFF[2],inwav -> RIFF[3]);

      i= inwav -> ChunkSize;
      rev_little_endian_4(i,output);

      fprintf(output,"%c%c%c%c",inwav -> WAVE[0],inwav -> WAVE[1],inwav -> WAVE[2],inwav -> WAVE[3]);  
      fprintf(output,"%c%c%c%c",inwav -> fmt[0],inwav -> fmt[1],inwav -> fmt[2],inwav -> fmt[3]);  

      i= inwav -> Subchunk1Size;
      rev_little_endian_4(i,output);

      i= inwav -> AudioFormat;
      rev_little_endian_2(i,output);

      i= inwav -> NumChan;
      rev_little_endian_2(i,output);  

      i= inwav -> SamplesPerSec;
      rev_little_endian_4(i,output);    

      i= inwav -> bytesPerSec;
      rev_little_endian_4(i,output); 

      i= inwav -> blockAlign;
      rev_little_endian_2(i,output);

      i= inwav -> bitsPerSample;
      rev_little_endian_2(i,output);              

      fprintf(output,"%c%c",inwav -> extra[0],inwav -> extra[1]); 
      fprintf(output,"%c%c%c%c",inwav -> Subchunk2ID[0],inwav -> Subchunk2ID[1],inwav -> Subchunk2ID[2],inwav -> Subchunk2ID[3]); 

      i= inwav -> Subchunk2Size;
      rev_little_endian_4(i,output);   
      
      //output sound data to the output WAV file
      int n,b,c;
  
  
      //copy the unprocessed sound data from input1 until the program reaches the the time that mix happens
       for(n=0;n<(((inwav -> bytesPerSec)*time)/2);n++) {
  
              i = inwav -> data[n];
              rev_little_endian_2(i,output);              
           
        
             
             }
       //mix the input1 with input2 from the time specified by the input parameter of the function      
       for(n=0;n<((bitewav -> Subchunk2Size)/2);n++) {
             
             c = (((inwav -> bytesPerSec)*time*1)/2);
             i = inwav -> data[n+c];
             b = bitewav -> data[n];
             i = i + b;
             rev_little_endian_2(i,output);
             
       
       
             }
         
        // c is the starting point of the rest of the WAV file after mixing input1 and input2     
        c = (((inwav -> bytesPerSec)*time*1)/2); 
        c = c + ((bitewav -> Subchunk2Size)/2);
        
        for (n=c;n<((inwav -> Subchunk2Size)/2);n++) {  
        
        
             i = inwav -> data[n];
             rev_little_endian_2(i,output);   
             
             
             }

                 fclose(output);
                    
      
      
}

//Extra credit
void lo_pass(WAV *inwav, char *outfile, int freq){

      int i;  //used to store the data to be processed
      FILE * output = fopen(outfile,"w");  //creat a file pointer to the output WAV
      
      //output data to the WAV file
      fprintf(output,"%c%c%c%c",inwav -> RIFF[0],inwav -> RIFF[1],inwav -> RIFF[2],inwav -> RIFF[3]);

      i= inwav -> ChunkSize;
      rev_little_endian_4(i,output);

      fprintf(output,"%c%c%c%c",inwav -> WAVE[0],inwav -> WAVE[1],inwav -> WAVE[2],inwav -> WAVE[3]);  
      fprintf(output,"%c%c%c%c",inwav -> fmt[0],inwav -> fmt[1],inwav -> fmt[2],inwav -> fmt[3]);  

      i= inwav -> Subchunk1Size;
      rev_little_endian_4(i,output);

      i= inwav -> AudioFormat;
      rev_little_endian_2(i,output);

      i= inwav -> NumChan;
      rev_little_endian_2(i,output);  

      i= inwav -> SamplesPerSec;
      rev_little_endian_4(i,output);    

      i= inwav -> bytesPerSec;
      rev_little_endian_4(i,output); 

      i= inwav -> blockAlign;
      rev_little_endian_2(i,output);

      i= inwav -> bitsPerSample;
      rev_little_endian_2(i,output);              

      fprintf(output,"%c%c",inwav -> extra[0],inwav -> extra[1]); 
      fprintf(output,"%c%c%c%c",inwav -> Subchunk2ID[0],inwav -> Subchunk2ID[1],inwav -> Subchunk2ID[2],inwav -> Subchunk2ID[3]); 

      i= inwav -> Subchunk2Size;
      rev_little_endian_4(i,output);   
      
      double r;
      
      r = 0.5; //resolution
      
      int f;
      
      f = freq;
      
      double c,a1,a2,a3,b1,b2;
      
      
      //these are coeffcients for the low pass filter
      c = 1.0 / tan( M_PI * f / (inwav -> SamplesPerSec));
 
      a1 = 1.0 / ( 1.0 + r * c + c * c);
      a2 = 2.0 * a1;
      a3 = a1;
      b1 = 2.0 * ( 1.0 - c*c) * a1;
      b2 = ( 1.0 - r * c + c * c) * a1;
      
      
      // initialze variables for calculating the filtered output
      short int in,i1,i2,o1,o2;
      short int * o = malloc(((inwav -> Subchunk2Size)/2) * sizeof(short int));
      int n;
      
      
      // calculate the filtered output and put them in the output array
      for (n=0;n<((inwav -> Subchunk2Size)/2);n++) {  
        
             if ((n==0)||(n==1)) {
             
             in = inwav -> data[n];
             i1 = 0;
             i2 = 0;
             o1 = 0;
             o2 = 0;
             
             }
             
             else if ((n==2)||(n==3)) {
             
             
             in = inwav -> data[n];
             i1 = inwav -> data[n-2];
             i2 = 0;
             o1 = o[n-2];
             o2 = 0;
             
             
             }
             
             else if (n>=4) {
             
            in = inwav -> data[n];
            i1 = inwav -> data[n-2];
            i2 = inwav -> data[n-4]; 
            o1 = o[n-2];
            o2 = o[n-4];
             
             }
             
                         o[n] = (a1 * in) + (a2 * i1) + (a3 * i2) - (b1 * o1) - (b2 * o2);
             
             }
             
            // output the array to the WAV file 
      for (n=0;n<((inwav -> Subchunk2Size)/2);n++) {
         
                   
              in = o[n];
              rev_little_endian_2(in,output);        

            }
             

          fclose(output);
             

        

}

void rev_little_endian_2(int i,FILE *output) { 

           //this function takes in a 16-bit integer and output its last 8 bits first to the output file and then the fisrt 8 bits.
           char c1,c0; 
           c0 = i;
           i >>= 8;
           c1 = i;
           fprintf(output,"%c%c",c0,c1);
           
           }
           
void rev_little_endian_4(int i,FILE *output) { 

    //this function takes in a 32-bit integer and output its last 8 bits first to the output file and then the fisrt 8 bits.
           char c3,c2,c1,c0; 
           c0 = i;
           i >>= 8;
           c1 = i;
           i >>= 8;
           c2 = i;
           i >>= 8;
           c3 = i;
           fprintf(output,"%c%c%c%c",c0,c1,c2,c3); 
           
           }           
           
           
           

