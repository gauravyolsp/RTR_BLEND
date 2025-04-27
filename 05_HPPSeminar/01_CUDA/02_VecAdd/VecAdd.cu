// header files
// standard header
#include<stdio.h>
#include<stdlib.h>

// CUDA headers
#include<cuda.h>
#include "helper_timer.h"

//global variables
 // const int iNumberOfArrayElements = 5;
 const int iNumberOfArrayElements = 11444777;

float* hostInput1 = NULL;
float* hostInput2 = NULL;
float* hostOutput = NULL;
float* gold = NULL;

float* deviceInput1 = NULL;
float* deviceInput2 = NULL;
float* deviceOutput = NULL;

float timeOnCPU = 0.0f;
float timeOnGPU = 0.0f;

// CUDA kernel
__global__ void vecAddGPU(float* in1,float* in2, float* out,int len)
{
    // code
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if(i < len)
    {
        out[i] = in1[i] + in2[i];
    }
}
int main(void)
{
    // function declerations
    void fillFloatArrayWithRandomNumbers(float*, int);
    void vecAddCPU(const float*, const float*, float*, int);
    void cleanup(void);

    int size = iNumberOfArrayElements * sizeof(float);
    cudaError_t result = cudaSuccess;

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
    gold = (float*)malloc(size);
    if(gold == NULL)
    {
        printf("Host Memory is failed for gold. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // fill values into host Arrays
    fillFloatArrayWithRandomNumbers(hostInput1,iNumberOfArrayElements);
    fillFloatArrayWithRandomNumbers(hostInput2,iNumberOfArrayElements);

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
    
    // copy data host array into device array
    result = cudaMemcpy(deviceInput1,hostInput1,size,cudaMemcpyHostToDevice);
    if(result != cudaSuccess)
    {
        printf("Host Memory is failed for deviceInput1. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    result = cudaMemcpy(deviceInput2,hostInput2,size,cudaMemcpyHostToDevice);
    if(result != cudaSuccess)
    {
        printf("Host Memory is failed for deviceInput2. \n");
        cleanup();
        exit(EXIT_FAILURE);
    }

    // give kernel configuration
    dim3 dimGrid = dim3((int)ceil((float)iNumberOfArrayElements / 256.0f),1,1);
    dim3 dimBlock = dim3(256,1,1);

    // CUDA kernel for vector Addition
    StopWatchInterface* timer = NULL;
    sdkCreateTimer(&timer);
    sdkStartTimer(&timer);

    vecAddGPU <<<dimGrid,dimBlock>>> (deviceInput1,deviceInput2,deviceOutput,iNumberOfArrayElements);

    sdkStopTimer(&timer);
    timeOnGPU = sdkGetTimerValue(&timer);
    sdkDeleteTimer(&timer);
    timer = NULL;

    // vector addition on Host
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
   	vecAddCPU(hostInput1, hostInput2, gold, iNumberOfArrayElements);

    // compaison
    const float epsilon = .000001f;
    int breakValue = -1;
    bool bAccuracy = true;

    for(int i = 0; i < iNumberOfArrayElements; i++)
    {
        float val1 = gold[i];
        float val2 = hostOutput[i];
        if(fabs(val1 - val2) > epsilon)
        {
            bAccuracy = false;
            breakValue = i;
            break;
        }
    }
    char str[128];
    if(bAccuracy == false)
    {
       sprintf(str,"Comparison of CPU and GPU vector Addition is NOT within accuricy of 0.000001 at Array index %d",breakValue);
    }
    else
    {
       sprintf(str,"Comparison of CPU and GPU vector Addition is within accuricy of 0.000001");
    }

    printf("Array1 is begin from 0th index %.6f to %dth index %.6f \n", hostInput1[0], iNumberOfArrayElements - 1, hostInput1[iNumberOfArrayElements - 1]);
    printf("Array2 is begin from 0th index %.6f to %dth index %.6f \n", hostInput2[0], iNumberOfArrayElements - 1, hostInput2[iNumberOfArrayElements - 1]);

    printf("CUDA kernel Grid dimentions = %d,%d,%d and Block dimentions = %d,%d.%d \n", dimGrid.x, dimGrid.y, dimGrid.z, dimBlock.x, dimBlock.y, dimBlock.z);

    printf(" Output Array is begin from 0th index %.6f to %dth index %.6f \n", hostOutput[0], iNumberOfArrayElements - 1, hostOutput[iNumberOfArrayElements - 1]);

    printf("Time taken for vector addition on CPU = %.6f \n", timeOnCPU);
    printf("Time taken for vector addition on GPU = %.6f \n", timeOnGPU);


    // cleanup
    cleanup();

    return 0;
}

void fillFloatArrayWithRandomNumbers(float* arr, int len)
{ 
   // code
   const float fscale = 1.0f/(float)RAND_MAX;
   for(int i = 0; i < len; i++)
   {
      arr[i] = fscale * rand();
   }

}

void vecAddCPU(const float* arr1,const float* arr2,float *out, int len)
{
   StopWatchInterface* timer = NULL;
   sdkCreateTimer(&timer);
   sdkStartTimer(&timer);

   for(int i = 0;i< len; i++)
   {
      out[i] = arr1[i] + arr2[i];
   }
   sdkStopTimer(&timer);
   timeOnCPU = sdkGetTimerValue(&timer);
   sdkDeleteTimer(&timer);
   timer = NULL;
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

    if(gold)
    {
       cudaFree(gold);
       gold = NULL;
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





