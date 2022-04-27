#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

int main( int argc, char* argv[]) {

	char filename[50];
	uint image_width, image_height; 
    unsigned long block_no, half_block_no, total_block_no, total_half_block_no;
	byte **img, ***g2b, ***g2byte, ***mixedimg, **finalimg;
	float **floatimg, ***floatmixedimg;
    blocktype *block, *half_block;
	int k, i, j;

	strcpy(filename, argv[1]);
	image_height = atoi(argv[2]);
	image_width = atoi(argv[3]);

	printf("%s ,%d ,%d \n\n", filename, image_width, image_height);

    // Dynamically allocate memory 
	img = malloc( image_height * image_width * sizeof(byte));
	if (!img) {
		printf("Memory not allocated\n");
		exit(0);
	}
	g2b = malloc( 8 * image_height * image_width * sizeof(byte));
	if (!g2b) {
		printf("Memory not allocated\n");
		exit(0);
	}
	g2byte = malloc( 8 * image_height * image_width * sizeof(byte));
	if (!g2byte) {
		printf("Memory not allocated\n");
		exit(0);
	}
	mixedimg = malloc( 8 * image_height * image_width * sizeof(byte));
	if (!mixedimg) {
		printf("Memory not allocated\n");
		exit(0);
	}	
	floatmixedimg = malloc( 8 * image_height * image_width * sizeof(float));
	if (!floatmixedimg) {
		printf("Memory not allocated\n");
		exit(0);
	}
	floatimg = malloc( image_height * image_width * sizeof(float));
	if (!floatimg) {
		printf("Memory not allocated\n");
		exit(0);
	}
	finalimg = malloc( image_height * image_width * sizeof(byte));
	if (!finalimg) {
		printf("Memory not allocated\n");
		exit(0);
	}
    block = malloc(image_height * (image_width/2) * sizeof(blocktype));
    if (!block) {
        printf("Memory not allocated\n");
        exit(0);
    }
	half_block = malloc(image_height * (image_width/2) * sizeof(blocktype));
    if (!half_block) {
        printf("Memory not allocated\n");
        exit(0);
    } // End

	read_image(filename, image_height, image_width, img);
	gray_2_binary(image_height, image_width, img, g2b, g2byte, floatmixedimg, mixedimg, floatimg, finalimg, block, &block_no, &total_block_no, half_block, &half_block_no, &total_half_block_no); 

    free(img);
	free(g2b);
	free(g2byte);
	free(floatmixedimg);
	free(mixedimg);
	free(floatimg);
	free(finalimg);
    free(block);
	free(half_block);
}

void read_image(char* fname, uint height, uint width, byte image[height][width]) {

	FILE* fp;
	int i;

	fp = fopen(fname,"r");
	if (!fp) {
		printf("Can not open the file\n");
		exit(0);
	}
	for (i=0; i<height; i++) {
		fread(image[i], 1, width, fp);
	}
	fclose(fp);
}