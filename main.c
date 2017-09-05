#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *pFile;

int IMG_ROW = 1280;
int IMG_COL = 720;
int IMG_DEPTH = 3;


int *input_image;
int *input_image_flattened;
int *output_image;

int kernel_power_max = 10;
int size_of_kernel = 3;
int no_of_kernel = 1;
int *kernel;
int step = 1;

int initialize_image()
{
	int i, j, k;
	for(i=0; i<IMG_ROW; i=i+1)
	{
		for(j=0; j<IMG_COL; j=j+1)
		{
			for(k=0; k<IMG_DEPTH; k=k+1)
			{
				input_image[i*IMG_DEPTH*IMG_COL + j*IMG_DEPTH + k] = rand()%256;
			}
		}
	}
	return 0;
}



int flatten_input(int in_channel)
{
	int i, j, k;
	for(i=0; i<IMG_ROW; i=i+1)
	{
		for(j=0; j<IMG_COL; j=j+1)
		{
			input_image_flattened[i*IMG_COL + j] = 0;

			for(k=0; k<in_channel; k=k+1)
			{
				input_image_flattened[i*IMG_COL + j] +=  input_image[i*in_channel*IMG_COL + j*in_channel + k];
			}
		}
	}
	return 0;
}


int modify_kernel(int o_channel, int kernel_size)
{
	int i, j ,k;
	free(kernel);
	kernel = (int *)malloc(kernel_size*kernel_size*o_channel*sizeof(int));

	for(i=0; i<kernel_size; i=i+1)
	{
		for(j=0; j<kernel_size; j=j+1)
		{
			for(k=0; k<o_channel; k=k+1)
			{
				kernel[i*kernel_size*o_channel + j*o_channel + k] = rand() % 8;
			}
		}
	}
	return 0;
}



int c_conv(int in_channel, int o_channel, int kernel_size, int stride)
{
	int i, j, k;
	int ii, jj;


	double flattening_time_1 = clock();

	flatten_input(in_channel);

	double flattening_time_2 = clock();
	double flattening_time = (flattening_time_2 - flattening_time_1)/CLOCKS_PER_SEC;
	printf("\n%dx%d size image flattening time = %g\n", IMG_ROW, IMG_COL, flattening_time);
	fprintf(pFile, "\n%dx%d size image flattening time = %g\n", IMG_ROW, IMG_COL, flattening_time);


	printf("For output channels = %d:\n", o_channel);
	fprintf(pFile, "For output channels = %d:\n", o_channel);
	double kernel_modification_time_1 = clock();

	modify_kernel(o_channel, kernel_size);

	double kernel_modification_time_2 = clock();
	double kernel_modification_time = (kernel_modification_time_2 - kernel_modification_time_2)/CLOCKS_PER_SEC;
	printf("Kernel modification time = %g\n", kernel_modification_time);
	fprintf(pFile, "Kernel modification time = %g\n", kernel_modification_time);

	int output_ROW = (int)((IMG_ROW-kernel_size)/stride) + 1;
	int output_COL = (int)((IMG_COL-kernel_size)/stride) + 1;


	free(output_image);
	output_image = (int *)malloc((output_COL)*(output_ROW)*o_channel*sizeof(int));

	for(i=0; i<output_ROW; i=i+1)
	{
		for(j=0; j<output_COL; j=j+1)
		{
			for(k=0; k<o_channel; k=k+1)
			{
				output_image[i*(output_COL)*o_channel + j*o_channel + k] = 0;

				for(ii=0; ii<kernel_size; ii=ii+1)
				{
					for(jj=0; jj<kernel_size; jj=jj+1)
					{
						output_image[i*(output_COL)*o_channel + j*o_channel + k] += input_image_flattened[(i*stride+ii)*(IMG_COL-kernel_size+1) + (j*stride+jj)] * kernel[ii*kernel_size*o_channel + jj*o_channel + k] ;
					}
				}
			}
		}
	}

	return 0;

}

int main()
{
	char output_file[35];
    sprintf(output_file, "Image_size_%dx%d_data.txt", IMG_ROW, IMG_COL);
	pFile = fopen(output_file, "w");
	srand(time(NULL));
	int i;
	kernel = (int *)malloc(size_of_kernel*size_of_kernel*no_of_kernel*sizeof(int));


	input_image = (int *)malloc(IMG_COL*IMG_ROW*IMG_DEPTH*sizeof(int));
	input_image_flattened = (int *)malloc(IMG_COL*IMG_ROW* sizeof(int));

	output_image = (int *)malloc((IMG_COL-size_of_kernel+1)*(IMG_ROW-size_of_kernel+1)*no_of_kernel*sizeof(int));


	initialize_image();

	for(i=0; i<=kernel_power_max; i=i+1)
	{

		double output_generation_time_1 = clock();
		c_conv(IMG_DEPTH, no_of_kernel, size_of_kernel, step);
		double output_generation_time_2 = clock();
		double output_generation_time = (output_generation_time_2 - output_generation_time_1)/CLOCKS_PER_SEC;

		printf("\nOutput generation time = %g\n", output_generation_time);
		fprintf(pFile, "\nOutput generation time = %g\n", output_generation_time);

		no_of_kernel = no_of_kernel*2;

	}

	free(input_image);
	free(input_image_flattened);
	free(output_image);
	free(kernel);

	fclose(pFile);
	return 0;
}
