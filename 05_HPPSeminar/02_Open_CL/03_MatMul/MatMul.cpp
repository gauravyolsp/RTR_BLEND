#include<stdio.h>
#include<math.h>

// CUDA header
// OpenCL header
#include<cuda.h>

#include"helper_timer.h"

// macros
#define BLOCK_WIDTH 64

// global variable 
cl_platform_id oclPlatformID;
cl_command_id oclDeviceID;

cl_program oclProgram;
cl_kernel oclKernel;

int* hostA = NULL;
int* hostB = NULL;
int* hostC = NULL;
int* gold = NULL;

int* deviceA = NULL;
int* deviceB = NULL;
int* deviceC = NULL;

float timeOnCPU = 0.0f;
float timeOnGPU = 0.0f;

// openCL kernel 
const char* oclSourceCode =
"__kernel void matMulGPU(__global int* A, __global int* B, __global int* C, int numRows, int numAColumns, int numBColumns, int numCColumns)"\
"{"\
"int row = get_global_id(0);"\
"int column = get_global_id(1);"\

	"if ((row < numRows) && (column < numBColumns))"\
	"{"\
		"int value = 0.0;"\
		"for (int k = 0;k < numAColumns;k++)"\
		"{"\
			"int a = A[row * numAColumns + k];"\
			"int b = B[k * numBColumns + column];"\
			"value += a * b;"\
		"}"\
		"C[row * numCColumns + column] = value;"\
	"}"\
"}"\

int main(int args, char* argv[])
{
	// function declarations
	void InitA(int* data, int, int);
	void InitB(int* data, int, int);
	void matMulCPU(int*, int*, int*, int, int, int, int);
	void cleanup();

	// variable declarations
	int numARows = BLOCK_WIDTH;
	int numAColumns = BLOCK_WIDTH;
	int numBRows = BLOCK_WIDTH;
	int numBColumns = BLOCK_WIDTH;

	int numCRows = numARows;
	int numCColumns = numBColumns;

	int numGoldRows = numARows;
	int numGoldColumns = numBColumns;

	int sizeA = numARows * numAColumns * sizeof(int);
	int sizeB = numBRows * numBColumns * sizeof(int);
	int sizeC = numCRows * numCColumns * sizeof(int);
	int sizeGold = numGoldRows * numGoldColumns * sizeof(int);

	cl_result result = cudaSuccess;

	// code
	// host memory Allocation

	hostA = (int*)malloc(sizeA);
	if (hostA == NULL)
	{
		printf("host memory allocation is failed for hostA matrix. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}
	hostB = (int*)malloc(sizeB);
	if (hostB == NULL)
	{
		printf("host memory allocation is failed for hostB matrix. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}
	hostC = (int*)malloc(sizeC);
	if (hostA == NULL)
	{
		printf("host memory allocation is failed for hostC matrix. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}
	
	gold = (int*)malloc(sizeGold);
	if (gold == NULL)
	{
		printf("host memory allocation is failed for gold matrix. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	// printing matrix dimention and size

	printf("The Dimentions of the matrix 'hodtA' are : %d x %d \n", numARows, numAColumns);
	printf("The Dimentions of the matrix 'hodtB' are : %d x %d \n", numBRows, numBColumns);
	printf("The Dimentions of the matrix 'hodtC' are : %d x %d \n", numCRows, numCColumns);
	printf("The Dimentions of the matrix 'gold'  are : %d x %d \n", numGoldRows, numGoldColumns);

	printf("Size of matrix hostA = %d \n", sizeA);
	printf("Size of matrix hostB = %d \n", sizeB);
	printf("Size of matrix hostC = %d \n", sizeC);
	printf("Size of matrix gold  = %d \n", sizeGold);

	// fill source matrix
	InitA(hostA, numARows, numAColumns);
	InitB(hostB, numBRows, numBColumns);

	// get OpenCL supporting platform's ID
	result = clGetPlatformIDs(1,&oclPlatformID,NULL);
	if (result != cudaSuccess)
	{
		printf("clGetPlatformIDs failed. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}
	
	// get openCL supporting CPU device's ID
	result = clGetDeviceIDs(oclPlatformID, CL_DEVICE_TYPE_GPU, 1 & oclDeviceID, NULL);
	if (result != cudaSuccess)
	{
		printf("clGetDeviceIDs failed. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	// create OPenCL compute context
	oclContext = clCreateContext(NULL, 1, &oclDeviceID, NULL, NULL, &result);
	if (result != cudaSuccess)
	{
		printf("clCreateContext failed. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	// create command queue
	oclCommandQueue = clCreateCommandQueue(oclContext, oclDeviceID, 0, &result);
	if (result != cudaSuccess)
	{
		printf("clCreateCommandQueue failed. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	// oclProgram OpenCL program from .c1
	oclProgram = clCreateProgramWithSource(oclContext, 1, (const char**) &oclSourceCode, NULL,&result);
	if (result != cudaSuccess)
	{
		printf("clCreateProgramWithSource failed. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	// build openCL program
	result = clBuildProgram(oclProgram, 0, NULL, NULL, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		size_t len;
		char buffer[248] = { 0 };

		clGetProgramBuildInfo(oclProgram, oclDeviceID, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		printf("program Build log : %s \n", buffer);
		printf("clBuildProgram() Failed : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// create openCL kernel by passing kernel function name that we used in .cl file
	oclKernel = clCreateKernel(oclProgram, "matMulGPU", &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateKernel() Failed : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// device memory allocation
	deviceA = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, size, NULL, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateBuffer() Failed for deviceA : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	deviceB = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, size, NULL, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateBuffer() Failed for deviceB : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	deviceC = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, size, NULL, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateBuffer() Failed for deviceC : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set 0 based 0th argument i.e deviceA
	result = clSetKernelArg(oclKernel, 0, sizeof(cl_mem), (void*)&deviceA);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg() Failed for 1 st Argument : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set 0 based 1st argument i.e deviceB
	result = clSetKernelArg(oclKernel, 1, sizeof(cl_mem), (void*)&deviceB);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg() Failed for 2nd Argument : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set 0 based 2nd argument i.e deviceC
	result = clSetKernelArg(oclKernel, 2, sizeof(cl_mem), (void*)&deviceC);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg() Failed for 3rd Argument : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set based 4th argument i.e numColumns
	result = clSetKernelArg(oclKernel, 3, sizeof(cl_int), (void*)&numARows);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg() Failed for 4th Argument : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set based 5th argument i.e numColumns
	result = clSetKernelArg(oclKernel, 4, sizeof(cl_int), (void*)&numAColumns);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg() Failed for 5th Argument : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set based 5th argument i.e numColumns
	result = clSetKernelArg(oclKernel, 5, sizeof(cl_int), (void*)&numBColumns);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg() Failed for 6th Argument : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set based 6th argument i.e numColumns
	result = clSetKernelArg(oclKernel, 6, sizeof(cl_int), (void*)&numCColumns);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg() Failed for 7th Argument : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}
	
	// write above input device buffer to devioce memory
	result = clEnqueueWriterBuffer(oclCommandQueue, deviceA, CL_FALSE, 0,sizeA,0,NULL,NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueWriterBuffer() Failed for 1st input device buffer : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	result = clEnqueueWriterBuffer(oclCommandQueue, deviceB, CL_FALSE, 0, sizeB, 0, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueWriterBuffer() Failed for 2nd input device buffer : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// kernel configuration
	size_t globalWorkSize[2];
	globalWorkSize[0] = BLOCK_WIDTH;
	globalWorkSize[1] = BLOCK_WIDTH;

	// start timer
	StopWatchInterface* timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	result = clEnqueueNDRangeKernel(oclCommandQueue, oclKernel, 2, NULL, globalWorkSize, NULL, 0, NULL,NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueNDRangeKernel() Failed : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// finish OpenCL command queue
	clFinish(oclCommandQueue);

	// stop timer
	stdStopTimer(&timer);
	timeOnGPU = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);

	// read back result from the device into cpu variable 
	result = clEnqueueReadBuffer(oclCommandQueue, deviceC, CL_TRUE, 0, sizeC, hostC, 0, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueReadBuffer() Failed : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// matrix multiplication on CPU
	matMulCPU(hostA, hostB, gold, numARows, numBColumns, numBColumns, numCColumns);

	// comparison
	int breakValue = -1;
	bool bAccuracy = true;
	for (int i = 0;i < numCRows * numCColumns;i++)
	{
		int val1 = gold[i];
		int val2 = hostC[i];
		if (val1 != val2)
		{
			bAccuricy = false;
			breakValue = i;
			break;
		}
	}
	char str[128] = { 0 };
	if (bAccuracy == false)
	{
		sprintf(str, "comparision for  CPU and GPU matrix multiplication is not accurite at array index %d", breakValue);
	}
	else
	{
		sprintf(str, "comparision for  CPU and GPU matrix multiplication is accurite at array index");
	}

	printf("Time taken for matrix multiplication on CPU = %.6f \n", timeOnCPU);
	printf("Time taken for matrix multiplication on GPU = %.6f \n", timeOnGPU);

	printf("%s \n", str);

	// cleanup
	cleanup();

	return 0;
}

void InitA(int* data, int row, int col)
{
	int num = 1;

	// code
	for (int i = 0;i < row;i++)
	{
		for (int j = 0;j < col;j++)
		{
			*(data + i * col + j) = num;
			num++;
		}
	}
}

void InitB(int* data, int row, int col)
{
	int num = BLOCK_WIDTH;

	// code
	for (int i = 0;i < row;i++)
	{
		for (int j = 0;j < col;j++)
		{
			*(data + i * col + j) = num;
			num--;
		}
	}
}

void matMulCPU(int* A, int* B, int* C, int numARows, int numAColumns, int numBColumns, int numCColumns)
{
	// code
	StopWatchInterface* timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	for (int i = 0;i < numARows;++i)
	{
		for (int j = 0;j < numBColumns;++j)
		{
			int value = 0.0f;
			for (int k = 0;k < numAColumns;++k)
			{
				int a = A[i * numAColumns + k];
				int b = B[k * numBColumns + j];
				value += a * b;
			}
			C[i * numCColumns + j] = value;
		}
	}
	sdkStopTimer(&timer);
	timeOnCPU = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);
	timer = NULL;
}

void cleanup(void)
{
	// code
	printf("cleanup called");
	if (deviceC)
	{
		clReleaseMemObject(deviceC);
		deviceC = NULL;
	}
	if (deviceB)
	{
		clReleaseMemObject(deviceB);
		deviceB = NULL;
	}
	if (deviceA)
	{
		clReleaseMemObject(deviceA);
		deviceA = NULL;
	}
	if (colKernel)
	{
		clReleaseMemObject(colKernel);
		colKernel = NULL;
	}
	if (oclProgram)
	{
		clReleaseMemObject(oclProgram);
		oclProgram = NULL;
	}
	if (gold)
	{
		clReleaseMemObject(gold);
		gold = NULL;
	}
	if (hostC)
	{
		clReleaseMemObject(hostC);
		hostC = NULL;
	}

	if (hostB)
	{
		clReleaseMemObject(hostB);
		hostB = NULL;
	}

	if (hostA)
	{
		clReleaseMemObject(hostA);
		hostA = NULL;
	}
}