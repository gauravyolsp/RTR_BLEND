#include<stdio.h>

// OpenCL header
#include<CL/opencl.h>

// global variable
const int iNumberOfArrayElements = 5;

// openCL variable
cl_platform_id oclPlatformId;              // internaly STRUCT
cl_device_id oclDeviceId;                  // internaly STRUCT

cl_context oclContext = NULL;             // give current context
cl_command_queue hostOutput = NULL; //  

cl_program oclProgram = NULL;                  // to send to kernel
cl_kernel oclKernel = NULL;

float* hostInput1 = NULL;
float* hostInput2 = NULL;
float* hostOutput = NULL;

cl_mem deviceInput1 = NULL;
cl_mem deviceInput2 = NULL;
cl_mem deviceOutput = NULL;

// OpenCL kernel
const char* oclSourceCode =
"__kernel void vecAddGPU(__global float *in1,__global float *in2,__global float*out,int len)"\
"{"\
"int i = get_global_id(0);"\
"if(i < len)"\
"{"\
"out[i] in1[i] + in2[i];"\
"}"\
"}";

// entry-point function
int main(void)
{
	// function declaration
	void cleanup(void);

	// variable declarations
	int size = iNumberOfArrayElements * sizeof(float);
	cl_int result;

	// code
	// host memory allocation
	hostInput1 = (float*)malloc(size);
	if (hostInput1 == NULL)
	{
		printf("Host memory allocation is failed for input1 array. \n");
		cleanup();
		exit(EXIT_FAILLURE);
	}

	hostInput2 = (float*)malloc(size);
	if (hostInput2 == NULL)
	{
		printf("Host memory allocation is failed for hostInput2 array. \n");
		cleanup();
		exit(EXIT_FAILLURE);
	}

	hostOutput = (float*)malloc(size);
	if (hostOutput == NULL)
	{
		printf("Host memory allocation is failed for hostOutput array. \n");
		cleanup();
		exit(EXIT_FAILLURE);
	}

	// filling values into host arrays
	hostInput1[0] = 101.0;
	hostInput1[1] = 102.0;
	hostInput1[2] = 103.0;
	hostInput1[3] = 104.0;
	hostInput1[4] = 105.0;

	hostInput2[0] = 201.0;
	hostInput2[1] = 202.0;
	hostInput2[2] = 203.0;
	hostInput2[3] = 204.0;
	hostInput2[4] = 204.0;

	// get OpenCL supporting platform's ID
	result = clGetPlatformIDs(1, &ocPlatformID, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clGetPlatformIDs() Failed : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// get OpenCL supporting CPU device ID
	result = clGetDeviceIDs(oclPlatformjID, CL_DEVICE_TYPE_GPU,1,&oclDeviceID,NULL);
	if (result != CL_SUCCESS)
	{
		printf("clGetDeviceIDs() Failed : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// get OpenCL compute context
	oclContext = clCreateContext(NULL, &oclDeviceID, NULL, NULL, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateContext() Failed : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// create command queue
	oclCommandQueue = clCreateCommandQueue(oclContext, oclDeviceID, 0, &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateCommandQueue() Failed : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// create openCL program from .cl
	oclProgram = clCreateProgramWithSource(oclContext, 1, (const char**),&oclSourceCode,NULL &result);
	if (result != CL_SUCCESS)
	{
		printf("clCreateProgramWithSource() Failed : %d \n", result);
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
	result = clSetKernelArg(oclKernel, 0, sizeof(cl_mem), (void *)&deviceInput1);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg() Failed for 1st Argument : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set 0 based 0th argument i.e deviceInput2
	result = clSetKernelArg(oclKernel, 0, sizeof(cl_mem), (void*)&deviceInput2);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg() Failed for 2nd Argument : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set 0 based 0th argument i.e deviceOutput
	result = clSetKernelArg(oclKernel, 0, sizeof(cl_mem), (void*)&deviceOutput);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg() Failed for 3rd Argument : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// set 0 based 3rd argument i,e len
	result = clSetKernelArg(oclKernel, 3, sizeof(cl_int), (void*)&iNumberOfArrayElements);
	if (result != CL_SUCCESS)
	{
		printf("clSetKernelArg() Failed for 4th Argument : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// write above input device buffer to device memory
	result = clEnqueueWriterBuffer(oclCommandQueue, deviceInput1, CL_FALSE, 0,size,hostInput1,,NULL,NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueWriterBuffer() Failed for 1st input device buffer : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	result = clEnqueueWriterBuffer(oclCommandQueue, deviceInput2, CL_FALSE, 0, size, hostInput1, , NULL, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueWriterBuffer() Failed for 1st input device buffer : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// kernel configuration
	size_t global_size = 5;      // 1-D 5 element array operation
	result = clEnqueNDRangeKernel(oclCommandQueue, oclKernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueNDRangeKernel() Failed for 1st input device buffer : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// finish OpenCL command queue
	clFinish(oclCommandQueue);

	// read back result from the device i.e from deviceOutput into cpu varible i.e hostOutput
	result = clEnqueueReadBuffer(oclCommandQueue, deviceOutput, CL_TRUE, 0, size, hostOutput, 0, NULL, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clEnqueueReadBuffer() Failed for 1st input device buffer : %d \n", result);
		cleanup();
		exit(EXIT_FAILURE);
	}

	// display result
	int i;
	for (i = 0;i < iNumberOfArrayElements;i++)
	{
		printf("%f + %f \n", hostInput1[i], hostInput2[i], hostOutput[i]);
	}

	// cleanup
	cleanup();

	return 0;
}

void cleanup(void)
{
	// code
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

	if (oclContext)
	{
		clReleaseMemObject(oclContext);
		oclContext = NULL;
	}

	if (hostOutput)
	{
		clReleaseMemObject(hostOutput);
		hostOutput = NULL;
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
