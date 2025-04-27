#include<stdio.h>
#include<math.h>

// CUDA header
// OpenCL header
#include<cuda.h>

#include"helper_timer.h"

//global variables
// const int iNumberOfArrayElements = 5;
const int iNumberOfArrayElements = 11444777;

cl_platform_id oclPlatformID;
cl_command_id oclDeviceID;

cl_program oclProgram;
cl_kernel oclKernel;

float* hostInput1 = NULL;
float* hostInput2 = NULL;
float* hostOutput = NULL;
float* gold = NULL;

float* deviceInput1 = NULL;
float* deviceInput2 = NULL;
float* deviceOutput = NULL;

float timeOnCPU = 0.0f;
float timeOnGPU = 0.0f;

// openCL kernel 
const char* oclSourceCode =
"__kernel void vecAddGPU(float* in1, float* in2, float* out, int len)"\
"{"\
	"int i = blockIdx.x * blockDim.x + threadIdx.x;"\
	"if (i < len)"\
	"{"\
		"out[i] = in1[i] + in2[i];"\
	"}"\
"}"\

int main(int args, char* argv[])
{
	// function declerations
	void fillFloatArrayWithRandomNumbers(float*, int);
	void vecAddCPU(const float*, const float*, float*, int);
	void cleanup(void);

	int size = iNumberOfArrayElements * sizeof(float);
	cudaError_t result = cudaSuccess;

	hostInput1 = (float*)malloc(size);
	if (hostInput1 == NULL)
	{
		printf("Host Memory is failed for hostInput1. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	hostInput2 = (float*)malloc(size);
	if (hostInput2 == NULL)
	{
		printf("Host Memory is failed for hostInput2. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	hostOutput = (float*)malloc(size);
	if (hostOutput == NULL)
	{
		printf("Host Memory is failed for hostOutput. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}
	gold = (float*)malloc(size);
	if (gold == NULL)
	{
		printf("Host Memory is failed for gold. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	// fill values into host Arrays
	fillFloatArrayWithRandomNumbers(hostInput1, iNumberOfArrayElements);
	fillFloatArrayWithRandomNumbers(hostInput2, iNumberOfArrayElements);

	// get OpenCL supporting platform's ID
	result = clGetPlatformIDs(1, &oclPlatformID, NULL);
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
	oclProgram = clCreateProgramWithSource(oclContext, 1, (const char**)&oclSourceCode, NULL, &result);
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
	oclKernel = clCreateKernel(oclProgram, "vecAddGPU", &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateKernel() Failed : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// device memory allocation
	deviceInput1 = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, size, NULL, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateBuffer() Failed for deviceInput1 : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	deviceInput2 = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, size, NULL, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateBuffer() Failed for deviceInput2 : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	deviceOutput = clCreateBuffer(oclContext, CL_MEM_READ_ONLY, size, NULL, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateBuffer() Failed for deviceOutput : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set 0 based 0th argument i.e deviceInput1
	result = clSetKernelArg(oclKernel, 0, sizeof(cl_mem), (void*)&deviceInput1);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg() Failed for 1 st Argument : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set 0 based 1st argument i.e deviceInput2
	result = clSetKernelArg(oclKernel, 1, sizeof(cl_mem), (void*)&deviceInput2);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg() Failed for 2nd Argument : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set 0 based 2nd argument i.e deviceOutput
	result = clSetKernelArg(oclKernel, 2, sizeof(cl_mem), (void*)&deviceOutput);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg() Failed for 3rd Argument : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set based 4th argument i.e numColumns
	result = clSetKernelArg(oclKernel, 3, sizeof(cl_int), (void*)&iNumberOfArrayElements);
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

	// write above input device buffer to devioce memory
	result = clEnqueueWriterBuffer(oclCommandQueue, deviceInput1, CL_FALSE, 0, sizeA, 0, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueWriterBuffer() Failed for 1st input device buffer : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	result = clEnqueueWriterBuffer(oclCommandQueue, deviceInput2, CL_FALSE, 0, sizeB, 0, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueWriterBuffer() Failed for 2nd input device buffer : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// kernel configuration
	size_t globalWorkSize = 256;
	size_t globalWorkSize;

	globalWorkSize = RoundGlobalSizeToNearestMultipleOfLocalSize(localWorkSize, iNumberOfArrayElements);

	// start timer
	StopWatchInterface* timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	result = clEnqueueNDRangeKernel(oclCommandQueue, oclKernel, 1, NULL, &globalWorkSize, &localWorkSize, 0, NULL, NULL);
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
	result = clEnqueueReadBuffer(oclCommandQueue, deviceOutput, CL_TRUE, 0, size, hostOutput, 0, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueReadBuffer() Failed : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// vector addition onhost
	vecAddCPU(hostInput1, hostInput2, gold, iNumberofArrayElements);

	// comparison
	const float epsilon = 0.000001f;
	int breakValue = -1;
	bool bAccuracy = true;
	for (int i = 0; i < iNumberOfArrayElements; i++)
	{
		int val1 = gold[i];
		int val2 = hostOutput[i];
		if (fabs(val1 - val2)>epsilon)
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
	printf("Array 1 begin from 0th index %.6f to %d th index %.6f \n", hostInput1[0], iNumberOfArrayElement - 1, hostInput1[iNumberOfArrayElement - 1]);
	printf("Array 2 begin from 0th index %.6f to %d th index %.6f \n", hostInput2[0], iNumberOfArrayElement - 1, hostInput2[iNumberOfArrayElement - 1]);

	printf("OpenCL kernel global size = %lu and local work size = %lu \n", globalWorkSize, localWorkSize);

	printf("Output Array begin from 0th index %.6f to %d th index %.6f \n", hostOutput[0], iNumberOfArrayElement - 1, hostOutput[iNumberOfArrayElement - 1]);

	printf("Time taken for matrix multiplication on CPU = %.6f \n", timeOnCPU);
	printf("Time taken for matrix multiplication on GPU = %.6f \n", timeOnGPU);

	printf("%s \n", str);
	// cleanup
	cleanup();

	return 0;
}

void fillFloatArrayWithRandomNumbers(float* arr, int len)
{
	// code
	const float fscale = 1.0f / (float)RAND_MAX;
	for (int i = 0; i < len; i++)
	{
		arr[i] = fscale * rand();
	}

}

void vecAddCPU(const float* arr1, const float* arr2, float* out, int len)
{
	StopWatchInterface* timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	for (int i = 0;i < len; i++)
	{
		out[i] = arr1[i] + arr2[i];
	}
	sdkStopTimer(&timer);
	timeOnCPU = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);
	timer = NULL;
}

size_t RoundGlobalSizeToNearestMultipleOfLocalSize(int local_size, unsigned int global_size)
{
	// code
	unsigned int r = global_size & % local_size;
	if (r == o)
	{
		return(global_size);
	}
	else
	{
		return(global_size + local_size);
	}
}
void cleanup(void)
{
	if (deviceOutput)
	{
		clReleaseMemObject(deviceOutput);
		deviceOutput = NULL;
	}

	if (deviceInput2)
	{
		clReleaseMemObject(deviceInput2);
		deviceInput2 = NULL;
	}

	if (deviceInput1)
	{
		clReleaseMemObject(deviceInput1);
		deviceInput1 = NULL;
	}

	if (oclKernel)
	{
		clReleaseMemObject(oclKernel);
		oclKernel = NULL;
	}

	if (oclProgram)
	{
		clReleaseMemObject(oclProgram);
		oclProgram = NULL;
	}

	if (oclCommandQueue)
	{
		clReleaseMemObject(oclCommandQueue);
		oclCommandQueue = NULL;
	}

	if (gold)
	{
		clReleaseMemObject(gold);
		gold = NULL;
	}

	if (hostOutput)
	{
		clReleaseMemObject(deviceOutput);
		deviceOutput = NULL;
	}

	if (hostInput2)
	{
		clReleaseMemObject(hostInput2);
		hostInput2 = NULL;
	}

	if (hostInput1)
	{
		clReleaseMemObject(hostInput1);
		hostInput1 = NULL;
	}

}