
void gray_2_binary(uint height, uint width, byte image[height][width], byte g2b[8][height][width], byte g2byte[8][height][width],  
float floatmixedimg[8][height][width], byte mixedimg[8][height][width], float floatimg[height][width], byte finalimg[height][width],
blocktype *block, unsigned long *blockno,unsigned long *total_blockno, blocktype *half_block, unsigned long *half_blockno,
unsigned long *total_half_blockno);

void convert_bimg_2_byteimg(uint height, uint width, byte g2b[height][width], byte g2byte[height][width]);

void float_rebuild(uint height, uint width, blocktype *block, unsigned long *blockno,
blocktype *half_block, unsigned long *half_blockno, float floatmixedimg[height][width]);

void convert_floatimg_2_byteimg(uint height, uint width, float floatmixedimg[height][width], byte mixedimg[height][width]);