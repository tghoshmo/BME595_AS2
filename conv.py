import numpy as np
from PIL import Image

class Conv2D:

 def  __init__(self,in_channel, o_channel, kernel_size, stride, mode):
     self.in_channel = in_channel
     self.o_channel = o_channel
     self.kernel_size = kernel_size
     self.stride = stride
     self.mode = mode

 def forward(self,input_image):

     self.input_image = input_image[:,:,0] + input_image[:,:,1] + input_image[:,:,2]

     if self.mode == 'rand':
         kernel = np.random.rand(self.o_channel, self.kernel_size, self.kernel_size)

     else:
         if self.o_channel == 1:
             K1 = np.array([[-1, -1, -1], [0, 0, 0], [1, 1, 1]])
             kernel = np.array([K1])
         elif self.o_channel == 2:
             K4 = np.array([[-1, -1, -1, -1, -1], [-1, -1, -1, -1, -1], [0, 0, 0, 0, 0], [1, 1, 1, 1, 1], [1, 1, 1, 1, 1]])
             K5 = np.array([[-1, -1, 0, 1, 1], [-1, -1, 0, 1, 1], [-1, -1, 0, 1, 1], [-1, -1, 0, 1, 1], [-1, -1, 0, 1, 1]])
             kernel = np.array([K4, K5])
         elif self.o_channel == 3:
             K1 = np.array([[-1, -1, -1], [0, 0, 0], [1, 1, 1]])
             K2 = np.array([[-1, 0, 1], [-1, 0, 1], [-1, 0, 1]])
             K3 = np.array([[1/9, 1/9, 1/9], [1/9, 1/9, 1/9], [1/9, 1/9, 1/9]])
             kernel = np.array([K1, K2, K3])
             
     output_height = int((self.input_image.shape[0]-self.kernel_size)/self.stride + 1)
     output_width = int((self.input_image.shape[1]-self.kernel_size)/self.stride + 1)
     output_image = np.zeros([output_height, output_width, self.o_channel]) 
     
     for channel in range(self.o_channel):
         for x in range(0,output_image.shape[0]):     
             for y in range(0,output_image.shape[1]):
                 output_image[x,y,channel] = np.sum([self.input_image[x*self.stride:x*self.stride+self.kernel_size,y*self.stride:y*self.stride+self.kernel_size]*kernel[channel]])  
     
     # Part A
     for i in range(self.o_channel):
         image_norm = (((output_image[:,:,i]-output_image[:,:,i].min())/(output_image[:,:,i].max()-output_image[:,:,i].min()))*255.0).astype(np.uint8)
         image_gray = Image.fromarray(image_norm)
         name = str(i+1)+".jpg"
         image_gray.save(name)

     count = self.input_image.shape[0]*self.input_image.shape[1] + output_height*output_width * (self.kernel_size*self.kernel_size + self.kernel_size*self.kernel_size - 1) * self.o_channel

     return (count, output_image)

