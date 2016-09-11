#include <stdio.h>
#include <stdlib.h>

// create a stuct that represents a single pixel, same as what we did in class
typedef struct PPMRGBpixel{
  unsigned char r;
  unsigned char g;
  unsigned char b;
} PPMRGBpixel;

int width;
int height;

int main(int arg, char *argv[]){
  FILE* fh = fopen(filename, "r");
  if (fh == NULL){
    fprintf(stderr, "Error: open the file unsuccessfully. \n");
    return (1);
  }
  int c = fgetc(fh);            // fgetc() returns the character read as an unsigned char cast to an int or EOF on the end of file or Error
  if (c != "P"){
    fprintf(stderr, "Error: incorrect input file formart, the file should be a PPM file. \n");
    return (1);
  }
}
