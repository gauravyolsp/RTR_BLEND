// header files
// standard header
#include<stdio.h>
#include<stdlib.h>

// CUDA headers
#include<cuda.h>

//global variables
const int iNumberOfArrayElements = 5;

float* hostInput1 = NULL;
float* hostInput2 = NULL;
float* hostOutput = NULL;

float* deviceInput1 = NULL;
float* deviceInput2 = NULL;
float* deviceOutput = NULL;

// CUDA kernel
__global__ void vecAddGPU(float* in1,float* in2, float* out,int len)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if(i < len)
    {
        out[i] = in1[i] + in2[i];
    }
}
int main(void)
{
    // function Declaration
    void cleanup(void);

    int size = iNumberOfArrayElements * sizeof(float);
    
    cudaError_t result = cudaSuccess;

    // code
    // Host  memory Allocation
    hostInput1 = (float*)malloc(size);
    if(hostInput1 == NULL)
    {
        printf("Host Memory is failed for hostInput1. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    hostInput2 = (float*)malloc(size);
    if(hostInput2 == NULL)
    {
        printf("Host Memory is failed for hostInput2. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    hostOutput = (float*)malloc(size);
    if(hostOutput == NULL)
    {
        printf("Host Memory is failed for hostOutput. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    hostInput1[0] = 101.0f;
    hostInput1[1] = 102.0f;
    hostInput1[2] = 103.0f;
    hostInput1[3] = 104.0f;
    hostInput1[4] = 105.0f;

    hostInput2[0] = 201.0f;
    hostInput2[1] = 202.0f;
    hostInput2[2] = 203.0f;
    hostInput2[3] = 204.0f;
    hostInput2[4] = 205.0f;

    // device memory allocation
    result = cudaMalloc((void**)&deviceInput1,size);
    if(result != cudaSuccess)
    {
        printf("Host Memory is failed for deviceInput1. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    result = cudaMalloc((void**)&deviceInput2,size);
    if(result != cudaSuccess)
    {
        printf("Host Memory is failed for deviceInput2. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    result = cudaMalloc((void**)&deviceOutput,size);
    if(result != cudaSuccess)
    {
        printf("Host Memory is failed for deviceOutput. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // send data 
    result = cudaMemcpy(deviceInput1, hostInput1,size,cudaMemcpyHostToDevice);
    if(result != cudaSuccess)
    {
        printf("Host Memory is failed for cudaMemcpy. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    result = cudaMemcpy(deviceInput2,hostInput2,size,cudaMemcpyHostToDevice);
    if(result != cudaSuccess)
    {
        printf("Host Memory is failed for cudaMemcpy. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // give kernel 
    dim3 dimGrid = dim3(iNumberOfArrayElements,1,1);    // x,y,z
    dim3 dimBlock = dim3(1,1,1);   // x,y,z

    // CUDA kernel for vector Addition
    vecAddGPU <<<dimGrid,dimBlock>>> (deviceInput1,deviceInput2,deviceOutput,iNumberOfArrayElements);

    // copy data from device array into host array
    result = cudaMemcpy(hostOutput,deviceOutput,size,cudaMemcpyDeviceToHost);
    if(result != cudaSuccess)
    {
        printf("Host Memory is failed for cudaMemcpy. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // vector addition on Host
    for(int i = 0; i < iNumberOfArrayElements; i++)
    {
        printf("%f + %f = %f \n",hostInput1[i],hostInput2[i],hostOutput[i]);
    }

    // cleanup
    cleanup();

    return 0;
}

void cleanup(void)
{
    if(deviceOutput)
    {
        cudaFree(deviceOutput);
        deviceOutput = NULL;
    }

    if(deviceInput2)
    {
        cudaFree(deviceInput2);
        deviceInput2 = NULL;
    }

    if(deviceInput1)
    {
        cudaFree(deviceInput1);
        deviceInput1 = NULL;
    }
    if(hostOutput)
    {
        cudaFree(deviceOutput);
        deviceOutput = NULL;
    }

    if(hostInput2)
    {
        cudaFree(hostInput2);
        hostInput2 = NULL;
    }

    if(hostInput1)
    {
        cudaFree(hostInput1);
        hostInput1 = NULL;
    }

}





