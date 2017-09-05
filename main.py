import numpy as np
import cv2

img = cv2.imread('image1.jpg',-1)
#img = cv2.imread('image2.jpg',-1)

# Part A
from conv import Conv2D
conv2d = Conv2D(3,1,3,1,'')
#conv2d = Conv2D(3,2,5,1,'')
#conv2d = Conv2D(3,3,3,2,'')
count, output_image = conv2d.forward(img)


# Part B
for i in range(10):
    import math
    from conv import Conv2D
    conv2d = Conv2D(3, int(math.pow(2, i)), 3, 1, 'rand')
    import timeit
    start = timeit.default_timer()
    count, output_image = conv2d.forward(img)
    stop = timeit.default_timer()
    run_time = stop - start 
    print (i, run_time)


# Part C
for i in range(1,6):
    import math
    from conv import Conv2D
    conv2d = Conv2D(3, 2, 2*i+1, 1, 'rand')
    count, output_image = conv2d.forward(img)
    print (2*i+1, count)
