#include <stdio.h>
#include <stdlib.h>

// create a stuct that represents a single pixel, same as what we did in class
typedef struct PPMRGBpixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} PPMRGBpixel;

typedef struct PPMimage {
	int width;
	int height;
	int maxColorValue;
	unsigned char *data;
} PPMimage;

PPMimage *buffer;

PPMimage PPMRead(char *inputFilename);
int PPMWrite(char *outPPMVersion, char *outputFilename);
int PPMDataWrite(char ppmVersionNum, FILE *outputFile);

PPMimage PPMRead(char *inputFilename) {
	buffer = (PPMimage*)malloc(sizeof(PPMimage));
	FILE* fh = fopen(inputFilename, "rb");
	if (fh == NULL) {
		fprintf(stderr, "Error: open the file unsuccessfully. \n");
		exit(1);
	}
	int c = fgetc(fh);            // get the first character of the file
	if (c != 'P') {
		fprintf(stderr, "Error: incorrect input file formart, the file should be a PPM file. \n");
		exit(1);
	}
	c = fgetc(fh);              // get the second character of the file, which should be a number with char type
	char ppmVersionNum = c;
	if (ppmVersionNum != '3' && ppmVersionNum != '6') {
		fprintf(stderr, "Error: invalid magic number, the ppm version should be either P3 or P6. \n");
		exit(1);
	}

	// since we want to go to the next line, so we check the new line character.
	while (c != '\n') {
		c = fgetc(fh);
	}
	c = fgetc(fh);
	//skip comments
	while (c == '#') {
		while (c != '\n') {
			c = fgetc(fh);
		}
		c = fgetc(fh);
	}
	ungetc(c, fh);

	int wh = fscanf(fh, "%d %d", &buffer->width, &buffer->height);
	if (wh != 2) {
		fprintf(stderr, "Error: the size of image has to include width and height, invalid size. \n");
		exit(1);
	}
	int mcv = fscanf(fh, "%d", &buffer->maxColorValue);
	if (mcv != 1) {
		fprintf(stderr, "Error: the max color value has to be one single value. \n");
		exit(1);
	}
	if (buffer->maxColorValue != 255) {
		fprintf(stderr, "Error: the image has to be 8-bit per channel. \n");
		exit(1);
	}

	buffer->data = (unsigned char*)malloc(buffer->width*buffer->height*sizeof(PPMRGBpixel));
	if (buffer == NULL) {
		fprintf(stderr, "Error: allocate the memory unsuccessfully. \n");
		exit(1);
	}
	if (ppmVersionNum == '3') {
		int i, j;
		for (i = 0; i<buffer->height; i++) {
			for (j = 0; j<buffer->width; j++) {
				PPMRGBpixel *pixel = (PPMRGBpixel*)malloc(sizeof(PPMRGBpixel));
				fscanf(fh, "%d %d %d", &pixel->r, &pixel->g, &pixel->b);
				buffer->data[i*buffer->width * 3 + j * 3] = pixel->r;
				buffer->data[i*buffer->width * 3 + j * 3 + 1] = pixel->g;
				buffer->data[i*buffer->width * 3 + j * 3 + 2] = pixel->b;
			}
		}
	}
	else if (ppmVersionNum == '6') {
		// read the pixel data, the type size_t might be used
		size_t s = fread(buffer->data, sizeof(PPMRGBpixel), buffer->width*buffer->height, fh);
		if (s < buffer->width*buffer->height) {
			fprintf(stderr, "Error: ");
			exit(1);
		}
	}
	else {
		fprintf(stderr, "Error: the ppm version cannot be read. \n");
		exit(1);
	}
	fclose(fh);
	return *buffer;
}

int PPMWrite(char *outPPMVersion, char *outputFilename) {
	int width = buffer->width;
	int height = buffer->height;
	int maxColorValue = buffer->maxColorValue;
	char ppmVersionNum = outPPMVersion[1];
	FILE *fh = fopen(outputFilename, "wb");
	if (fh == NULL) {
		fprintf(stderr, "Error: open the file unscuccessfully. \n");
		return (1);
	}
	char *comment = "# output.ppm";
	fprintf(fh, "P%c\n%s\n%d %d\n%d\n", ppmVersionNum, comment, width, height, 255);
	PPMDataWrite(ppmVersionNum, fh);
	fclose(fh);
}

int PPMDataWrite(char ppmVersionNum, FILE *outputFile) {
	// write image data bytes to the file if the ppm version is P6
	if (ppmVersionNum == '6') {
		fwrite(buffer->data, sizeof(PPMRGBpixel), buffer->width*buffer->height, outputFile);
		printf("The file saved successfully! \n");
		return (0);
	}
	else if (ppmVersionNum == '3') {
		int i, j;
		for (i = 0; i < buffer->height; i++) {
			for (j = 0; j < buffer->width; j++) {
				fprintf(outputFile, "%d %d %d ", buffer->data[i*buffer->width*3+j*3], buffer->data[i*buffer->width+j*3+1], buffer->data[i*buffer->width*3+2]);
			}
			fprintf(outputFile, "\n");
		}

		printf("The file saved successfully! \n");
		return (0);
	}
	else {
		fprintf(stderr, "Error: incorrect ppm version. \n");
		return (1);
	}
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		fprintf(stderr, "Error: the magic number, input file name and output file name are required. \n");
		return (1);
	}
	char *ppmVersion = argv[1];
	char *inputFilename = argv[2];
	char *outputFilename = argv[3];

	PPMRead(inputFilename);
	if (*ppmVersion == '6') {
		PPMWrite("P6", outputFilename);
	}
	else if (*ppmVersion == '3') {
		PPMWrite("P3", outputFilename);
	}
	else {
		fprintf(stderr, "Error: The file has to be read before the conversion \n");
		return (1);
	}
	return (0);
}


