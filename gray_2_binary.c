#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <tgmath.h>

#include "main.h"
#include "gray_2_binary.h"


void gray_2_binary(uint height, uint width, byte image[height][width], byte g2b[8][height][width], byte g2byte[8][height][width],  
float floatmixedimg[8][height][width], byte mixedimg[8][height][width], float floatimg[height][width], byte finalimg[height][width],
blocktype *block, unsigned long *blockno,unsigned long *total_blockno, blocktype *half_block, unsigned long *half_blockno,
unsigned long *total_half_blockno) {

    char filename[50];
	int k,i,j,x,y,att=1,error, max=-1;
	unsigned long initial_total_block;
	float Error, tmp, MSEtemp = 0.0, MSE = 0.0, PSNR = 0.0, MAX = 255 * 255;
	int delta_x;
	int cr=width/50;
	//int cr=10;

 	struct imgbyte {
		unsigned b7 : 1; // Least significant bit
		unsigned b6 : 1;
		unsigned b5 : 1;
		unsigned b4 : 1;
		unsigned b3 : 1;
		unsigned b2 : 1;
		unsigned b1 : 1;
		unsigned b0 : 1; // Most significant bit
	} *imgb;

	/* Extraction of bits for every gray pixel */
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			imgb = &image[i][j];
			g2b[7][i][j] = imgb->b0; // Most significant bitplane
			g2b[6][i][j] = imgb->b1;
			g2b[5][i][j] = imgb->b2;
			g2b[4][i][j] = imgb->b3;
			g2b[3][i][j] = imgb->b4;
			g2b[2][i][j] = imgb->b5;
			g2b[1][i][j] = imgb->b6;
			g2b[0][i][j] = imgb->b7; // Least significant bitplane
        }
    }

	*total_blockno = 0;
	*total_half_blockno = 0;
	for (k=0; k<8; k++) {
		delta_x = 0;
		printf("delta_x: %d // ",delta_x);
		ibr(height, width, block, blockno, total_blockno, half_block, half_blockno, total_half_blockno, g2b[k], delta_x);
	}
	initial_total_block = *total_blockno;
	printf("\ninitial_total_block:%ld\n", initial_total_block);

	do {
		/* Extraction of bits for every gray pixel */
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				imgb = &image[i][j];
				g2b[7][i][j] = imgb->b0; // Most significant bitplane
				g2b[6][i][j] = imgb->b1;
				g2b[5][i][j] = imgb->b2;
				g2b[4][i][j] = imgb->b3;
				g2b[3][i][j] = imgb->b4;
				g2b[2][i][j] = imgb->b5;
				g2b[1][i][j] = imgb->b6;
				g2b[0][i][j] = imgb->b7; // Least significant bitplane
				floatimg[i][j] = 0.0;
				finalimg[i][j] = 0;
        	}
    	}
		for (k=0; k<8; k++) {
			for (i=0; i<height; i++) {
				for (j=0; j<width; j++){
					g2byte[k][i][j] = 0;
					mixedimg[k][i][j] = 0;
					floatmixedimg[k][i][j] = 0.0;
				}
			}
		}

		printf("\nattempt No:%d\n", att);
		*total_blockno = 0;
		*total_half_blockno = 0;
    	for (k=0; k<8; k++) {
			if (k<4) {
				delta_x = cr - k*((cr/5)+1);
				//delta_x = 1000;
				//delta_x = 10;
			}
			else {
				delta_x = 0;
			}

			// convert g2b images from range [0,1] to range [0,255]
			//convert_bimg_2_byteimg(height, width, g2b[k], g2byte[k]);
			// print g2byte images
			//sprintf(filename, "bout%d.raw", k);
			//write_image(filename, height, width, g2byte[k]);

			// call ibr to find gray and white blocks
			printf("delta_x: %d // ",delta_x);
       		ibr(height, width, block, blockno, total_blockno, half_block, half_blockno, total_half_blockno, g2b[k], delta_x);

			// rebuild float images from gray and white blocks
			float_rebuild(height, width, block, blockno, half_block, half_blockno, floatmixedimg[k]);
			// convert float images from range [0,1] to range [0,255]
			convert_floatimg_2_byteimg(height, width, floatmixedimg[k], mixedimg[k]);
			// print mixedimg images
			sprintf(filename, "mixedout%d.raw", k);
			write_image(filename, height, width, mixedimg[k]);
		}
		printf("\ntotal white blocks: %ld // total gray blocks: %ld\n", *total_blockno, *total_half_blockno);
		printf("total blocks: %ld // %ld\n", (*total_blockno)+(*total_half_blockno), initial_total_block);
		
		//ΑΝΑΚΑΤΑΣΚΕΥΗ ΤΗΣ GRAY ΕΙΚΟΝΑΣ ΑΠΌ ΤΑ BITPLANES
		for (x = 0; x < height; x++)
			for (y = 0; y < width; y++) {
				for (k = 0; k < 8; k++)
					floatimg[y][x] += (float)powf(2, k) * floatmixedimg[k][y][x];
				finalimg[y][x] = (byte)roundf(floatimg[y][x]);
			}
		write_image("byte_finalimg.raw", height, width, finalimg);

		// ΥΠΟΛΟΓΙΣΜΟΣ ΤΟΥ ΣΦΑΛΜΑΤΟΣ ΑΝΑΜΕΣΑ ΣΤΗΝ ΑΡΧΙΚΗ ΕΙΚΟΝΑ IMG ΚΑΙ ΣΤΗΝ ΑΝΑΚΑΤΑΣΚΕΥΑΣΜΕΝΗ BYTE_FINALIMG
		Error = 0; tmp = 0;
		for (i = 0; i < height; i++)
			for (j = 0; j < width; j++) {
				Error += pow((image[i][j] - finalimg[i][j]), 2);
				tmp += pow(image[i][j], 2);
			}
		Error = sqrt(Error / tmp) * 100;
		error = (int)(Error*100);
		printf("Error=%f\n", Error);


		// ΥΠΟΛΟΓΙΣΜΟΣ ΤΟΥ PSNR ΑΝΑΜΕΣΑ ΣΤΗΝ ΑΡΧΙΚΗ ΕΙΚΟΝΑ IMG ΚΑΙ ΣΤΗΝ ΑΝΑΚΑΤΑΣΚΕΥΑΣΜΕΝΗ BYTE_FINALIMG
		for (x = 0; x < height; x++) {
			for (y = 0; y < width; y++) {
				MSEtemp = abs(finalimg[y][x] - image[y][x]);
				MSE += MSEtemp * MSEtemp;
			}
		}
		MSE /= height * width;

		// ΑΠΟΦΥΓΗ ΔΙΑΙΡΕΣΗΣ ΜΕ ΤΟ 0
		if (MSE == 0) PSNR = 99.0;
		else PSNR = 10 * log10(MAX / MSE);
		printf("MSE:%f \n",MSE);
		printf("PSNR=%f \n", PSNR); 

		if (att==5) {
			break;
		}
		if (error > max) {
			max = error;
		}
		else {
			break;
		}
		cr += 10;
		att += 1;
	} while (((*total_blockno)+(*total_half_blockno)>=(initial_total_block*0.1)) && (Error<=4));
}

void write_image(char fname[50], uint height, uint width, byte image[height][width]) {
	FILE* fp;
	int i;

	fp = fopen(fname,"w");
	if (!fp) {
		printf("Can not open the file\n");
		exit(0);
	}
	for (i=0; i<height; i++) {
		fwrite(image[i], 1, width, fp);
	}
	fclose(fp);
}

void convert_bimg_2_byteimg(uint height, uint width, byte g2b[height][width], byte g2byte[height][width]) {
	int i,j;
	
	for (i=0; i<height; i++) {
		for (j=0; j<width; j++) {
			if (g2b[i][j] == 1) {
				g2byte[i][j] = 255;
			}
			else {
				g2byte[i][j] = 0;
			}	
		}
	}
}

void float_rebuild(uint height, uint width, blocktype *block, unsigned long *blockno,
blocktype *half_block, unsigned long *half_blockno, float floatmixedimg[height][width]) {
	int i,j,x,y;

	for (i=0; i<height; i++) {
		for (j=0; j<width; j++) {
			floatmixedimg[i][j] = 0;
		}
	}
	
	for (i=0; i<(*blockno); i++) {
        for (x=block[i].x1; x<=block[i].x2; x++) {
            for (y=block[i].y1; y<=block[i].y2; y++) {
                floatmixedimg[y][x] = 1;
            }
        }
    }
	
    for (i=0; i<(*half_blockno); i++) {
        for (x=half_block[i].x1; x<=half_block[i].x2; x++) {
            for (y=half_block[i].y1; y<=half_block[i].y2; y++) {
                floatmixedimg[y][x] = 0.5;
            }
        }
    }
}

void convert_floatimg_2_byteimg(uint height, uint width, float floatmixedimg[height][width], byte mixedimg[height][width]) {
	int i, j;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			mixedimg[i][j] = (byte)(255.0 * floatmixedimg[i][j]);
		}
	}
}