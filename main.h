#define uint unsigned int
#define byte unsigned char

typedef struct {
    uint x1,x2,y1,y2;
} blocktype;

void read_image(char* fname, uint height, uint width, byte image[height][width]);
void write_image(char* fname, uint height, uint width, byte image[height][width]);