from PIL import Image
import cv2
import numpy as np  
from skimage.metrics import structural_similarity

fd1 = open('boat512.raw', 'rb')
fd2 = open('byte_finalimg.raw', 'rb')
rows = 512
cols = 512
f1 = np.fromfile(fd1, dtype=np.uint8, count=rows*cols)
f2 = np.fromfile(fd2, dtype=np.uint8, count=rows*cols)
img1 = f1.reshape((rows, cols)) #notice row, column format
img2 = f2.reshape((rows, cols)) #notice row, column format
fd1.close()
fd2.close()


ssim_score = structural_similarity(img1, img2, multichannel=False)
print(ssim_score)






