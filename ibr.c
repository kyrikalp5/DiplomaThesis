#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "ibr.h"

void ibr(uint height, uint width, blocktype *block, unsigned long *blockno, unsigned long *total_blockno, 
blocktype *half_block, unsigned long *half_blockno, unsigned long *total_half_blockno, byte g2b[height][width], 
int delta_x) {
    
    int i,j,k,y,x,x1,x2,y1,y2,kc,kp,j_last,j_curr,intervalfound,intervalmatched,try2match;
    int flagx, xrun, xstart, xend, cnt1, cnt0;
    int flagy, yrun, ystart, yend, xfirst, xlast, state1, state2;
    int c[width/2], p[width/2];

    *blockno = 0;
    *half_blockno = 0;
    kp=0;
    for (y=0; y<height; y++) {
        intervalfound=0;
        kc=0;
        j_curr=0;
        // Ψάχνω να βρώ διάστημα με συνεχόμενους άσσους
        for (x=0; x<width; x++) {
            try2match=0;
            if (g2b[y][x] && !intervalfound)  {
                x1 = x;
                intervalfound = 1;
            }
            if (!g2b[y][x] && intervalfound) {
                x2 = x-1;
                intervalfound = 0;
                try2match = 1;
            }
            if (x==width-1 && g2b[y][x] && intervalfound) {
                x2 = x;
                try2match = 1;
            }
            // Αφού βρώ το διάστημα ψάχνω για γκρί μπλόκ
            if (try2match && x2-x1+1<=delta_x) {
                xstart = x1, xrun = x2 + 1;
                flagx = 1;
                cnt1=0, cnt0=0;
                // Δημιουργώ γκρί μπλόκ στον άξονα x έως ότου να συναντήσω delta_x+1 
                // συνεχόμενους άσσους η μηδενικά 
                do {
                    if (xrun > width - 1) {
                        xend = width - 1;
                        flagx = 0;
                    }
                    if (flagx && g2b[y][xrun]) {
                        cnt1++;
                        cnt0 = 0;
                    }
                    if (flagx && !g2b[y][xrun]) {
                        cnt0++;
                        cnt1 = 0;
                    }
                    if ((flagx && cnt1==delta_x+1) || (flagx && cnt0==delta_x+1)) {
                        xend = xrun - delta_x - 1;
                        flagx = 0;
                    }
                    xrun++;
                } while (flagx);
                ystart = y, yrun = y + 1;
                flagy = 1, state1 = 0, state2 = 0; 
                // Διαχωρίζω σε δύο περιπτώσεις state1 και state2 
                // επεκτείνω το διάστημα ελένχου ανάλογα με τις περιπτώσεις
                if (xend-xstart+1>delta_x) {
                    state1 = 1;
                    if (xend+delta_x>=width-1) {
                        xlast = width-1;
                    }
                    else {
                        xlast = xend + delta_x;
                    }
                    if (xstart-delta_x<=0) {
                        xfirst = 0;
                    }
                    else {
                        xfirst = xstart - delta_x;
                    }  
                }
                if (xend-xstart+1<=delta_x) {
                    state2 = 1;
                    if (xend+(xend-xstart)>=width-1) {
                        xlast = width-1;
                    }
                    else {
                        xlast = xend + (xend - xstart);
                    }
                    if (xstart-(xend-xstart)<=0) {
                        xfirst = 0;
                    }
                    else {
                        xfirst = xstart - (xend-xstart);
                    }
                }
                // Ξεκινάω τους ελένχους για γκρί μπλόκ στο διάστημα y
                do {
                    if (yrun > height - 1) {
                        yend = height - 1;
                        flagy = 0;
                    }
                    // Αν state 1
                    if (flagy && state1) {
                        cnt1=0, cnt0=0;
                        if (xstart-1 >= 0) {
                            if (!g2b[yrun][xstart-1]) {
                                flagy=0;
                                yend = yrun - 1;
                            }
                        }
                        if (xend+1 <= width-1) {
                            if (!g2b[yrun][xend+1]) {
                                flagy=0;
                                yend = yrun - 1;
                            } 
                        }
                        for (k=xfirst; k<=xlast && flagy; k++) {
							if (g2b[yrun][k]) {
								cnt1++;
								cnt0 = 0;
							}
							if (!g2b[yrun][k]) {
								cnt0++;
								cnt1 = 0;
							}
							if (cnt1==delta_x+1 || cnt0==delta_x+1) {
								yend = yrun - 1;
								flagy = 0;
							}
                        }    
                    }
                    // Αν state2
                    if (flagy && state2) {
                        cnt1=0, cnt0=0;
                        if (xstart-1 >= 0) {
                            if (!g2b[yrun][xstart-1]) {
                                flagy=0;
                                yend = yrun - 1;
                            }
                        }
                        if (xend+1 <= width-1) {
                            if (!g2b[yrun][xend+1]) {
                                flagy=0;
                                yend = yrun - 1;
                            } 
                        }
                        for (k=xfirst; k<=xlast && flagy; k++) {
							if (g2b[yrun][k]) {
								cnt1++;
								cnt0 = 0;
							}
							if (!g2b[yrun][k]) {
								cnt0++;
								cnt1 = 0;
							}
							if (cnt1==xend-xstart+1 || cnt0==xend-xstart+1) {
								yend = yrun - 1;
								flagy = 0;
							}
						}
                    }        
                    yrun++;
                } while (flagy);
                half_block[*half_blockno].x1 = xstart;
                half_block[*half_blockno].x2 = xend;
                half_block[*half_blockno].y1 = ystart;
                half_block[*half_blockno].y2 = yend;
                (*half_blockno)++; 
                (*total_half_blockno)++;
                for (i=xstart; i<=xend; i++) {
                    for (j=ystart; j<=yend; j++) {
                        g2b[j][i] = 0;
                    }
                } 
            }
            // Find white blocks depending on delta_x
            if (try2match && x2-x1+1>delta_x) {
                intervalmatched = 0;
                j_last = j_curr;
                for (j=j_last; j<kp && x1>=block[p[j]].x1; j++) {
                    j_curr = j;
                    if (x1==block[p[j]].x1 && x2==block[p[j]].x2) {
                        c[kc]= p[j];
                        block[p[j]].y2 = y;
                        intervalmatched = 1;
                    }
                }
                if (!intervalmatched) {
                    block[*blockno].x1 = x1;
                    block[*blockno].x2 = x2;
                    block[*blockno].y1 = y;
                    block[*blockno].y2 = y;
                    c[kc] = (*blockno)++;
                    (*total_blockno)++;
                }
                kc++;
            }
        }
        for (i=0; i<kc; i++) {
            p[i] = c[i]; 
        }
        kp = kc;
    }

    printf("white blocks: %ld // gray blocks: %ld\n", *blockno, *half_blockno);
    //for (i=0; i<block_no; i++) {
    //printf("block[%d] = %d, %d, %d, %d\n", i, block[i].x1, block[i].x2, block[i].y1, block[i].y2);
    //}
}