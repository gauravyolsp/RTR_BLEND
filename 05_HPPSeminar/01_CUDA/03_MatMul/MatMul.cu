#include<stdio.h>

// CUDA files
#include<cuda.h>
#include"helper_timer.h"

// macros
#define BLOCK_WIDTH 32

// global variable 
int* hostA = NULL;
int* hostB = NULL;
int* hostC = NULL;
int* gold = NULL;

int* deviceA = NULL;
int* deviceB = NULL;
int* deviceC = NULL;

float timeOnCPU = 0.0f;
float timeOnGPU = 0.0f;

// cuda kernel function
__global__ void matMulGPU(int* A, int* B, int* C, int numRows, int numAColumns, int numBColumns, int numCColumns)
{
	// variable declarations
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int column = blockIdx.x * blockDim.x + threadIdx.x;

	// code
	if ((row < numRows) && (column < numBColumns))
	{
		int value = 0.0;
		for (int k = 0;k < numAColumns;k++)
		{
			int a = A[row * numAColumns + k];
			int b = B[k * numBColumns + column];
			value += a * b;
		}
		C[row * numCColumns + column] = value;
	}
}

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

	cudaError_t result = cudaSuccess;

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
	hostC = (int*)malloc(sizeC);
	if (hostC == NULL)
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

	// device mem0ory alloacation
	result = cudaMalloc((void**)&deviceA, sizeA);
	if (result != cudaSuccess)
	{
		printf("Device memory allocation is failed for then deviceA matrix. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}
	result = cudaMalloc((void**)&deviceB, sizeB);
	if (result != cudaSuccess)
	{
		printf("Device memory allocation is failed for then deviceB matrix. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	result = cudaMalloc((void**)&deviceC, sizeC);
	if (result != cudaSuccess)
	{
		printf("Device memory allocation is failed for then deviceC matrix. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	// copy data from host matrix to device matrix
	result = cudaMemcpy(deviceA, hostA, sizeA, cudaMemcpyHostToDevice);
	if (result != cudaSuccess)
	{
		printf("Host to device data copy is failed for the deviceA matrix. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	result = cudaMemcpy(deviceB, hostB, sizeB, cudaMemcpyHostToDevice);
	if (result != cudaSuccess)
	{
		printf("Host to device data copy is failed for the deviceB matrix. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	dim3 dimGrid = dim3(ceil((int)numBColumns / (int)BLOCK_WIDTH), ceil((int)numAColumns / (int)BLOCK_WIDTH), 1);
	dim3 dimBlock = dim3(BLOCK_WIDTH, BLOCK_WIDTH, 1);
	printf("dim3 dimGrid \n");
	// cuda kernel for matrix multiplication
	StopWatchInterface* timer = NULL;
	sdkCreateTimer(&timer);
	sdkStartTimer(&timer);

	matMulGPU <<< dimGrid, dimBlock >>> (deviceA, deviceB, deviceC, numARows, numAColumns, numBColumns, numCColumns);

	printf("matMulGPU \n"); 
	sdkStopTimer(&timer);
	timeOnGPU = sdkGetTimerValue(&timer);
	sdkDeleteTimer(&timer);
	timer = NULL;

	// copy data from device matrix into host matrix
	result = cudaMemcpy(hostC, deviceC, sizeC, cudaMemcpyDeviceToHost);
	if (result != cudaSuccess)
	{
		printf("Device to host copy is failed for hostC matrix. \n");
		cleanup();
		exit(EXIT_FAILURE);
	}

	// matrix multiplication on host
	matMulCPU(hostA, hostB, gold, numARows, numAColumns, numBColumns, numCColumns);
	printf("matMulCPU \n");
	// compairison
	int breakValue = -1;
	bool bAccuracy = true;

	for (int i = 0;i < numCRows * numCColumns; i++)
	{
		int val1 = gold[i];
		int val2 = hostC[i];

		if (val1 != val2)
		{
			bAccuracy = false;
			breakValue = i;
			break;
		}
	}
	printf("after for \n");
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
		cudaFree(deviceC);
		deviceC = NULL;
	}
	if (deviceB)
	{
		cudaFree(deviceB);
		deviceB = NULL;
	}
	if (deviceA)
	{
		cudaFree(deviceA);
		deviceA = NULL;
	}
	if (gold)
	{
		cudaFree(gold);
		gold = NULL;
	}
	if (hostC)
	{
		cudaFree(hostC);
		hostC = NULL;
	}

	if (hostB)
	{
		cudaFree(hostB);
		hostB = NULL;
	}

	if (hostA)
	{
		cudaFree(hostA);
		hostA = NULL;
	}
}