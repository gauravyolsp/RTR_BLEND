#include<stdio.h>

// OpenCL header
#include<CL/opencl.h>

// entry-point function
int main(void)
{
	// function declaration
	void printOpenCLDeviceProperties(void);

	// code
	printOpenCLDeviceProperties();
}

void printOpenCLDeviceProperties(void)
{
	// code
	printf("OpenCL INFORMATION : \n");
	printf("================================================= \n");

	cl_int result;
	cl_platform_id ocl_platform_id;
	cl_uint dev_count;
	cl_device_id* ocl_device_ids;
	char oclPlatformInfo[512] = { 0 };

	// get first platform ID
	result = clGetPlatformIDs(1, &ocl_platform_id, NULL);
	if (result != CL_SUCCESS)
	{
		printf("clGetPlatformIDs() Failed \n");
		exit(EXIT_FAILURE);
	}

	// get GPU device count
	result = clGetDeviceIDs(ocl_platform_id, &CL_DEVICE_TYPE_GPU,0 ,NULL,&dev_count);
	if (result != CL_SUCCESS)
	{
		printf("clGetDeviceIDs() Failed \n");
		exit(EXIT_FAILURE);
	}
	else if (dev_count == 0)
	{
		printf("There is no OpenCL Supportedn Device on this system. \n");
		exit(EXIT_FAILURE);
	}
	else
	{
		// get platform name
		clGetPlatformInfo(ocl_platform_id, CL_PLATFORM_NAME, 500, &oclPlatformInfo, NULL);
		printf("OpenCL Supporting GPU Platform Name : %s \n", oclPlatform);

		// get platform version
		clGetPlatformInfo(ocl_platform_id, CL_PLATFORM_VERSION, 500, &oclPlatformInfo, NULL);
		printf("OpenCL Supporting GPU platform version : %s \n", oclPlatformInfo);

		// printing supporting device number
		printf("Total number of OpenCL supporting GPU Device/Devices on this System : %d \n", dev_count);

		// allocate memory to holdthose device ids
		ocl_device_ids = (cl_device_id*)malloc(sizeof(cl_device_id) * dev_count);

		// get ids into allocated buffer
		clGetDeviceIDs(ocl_platform_id, CL_DEVICE_TYPE_GPU, dev_count, ocl_device_ids, NULL);

		char ocl_dev_prop[1024] = { 0 };
		int i;
		for (i = 0;i < (int)dev_count;i++)
		{
			printf("\n");
			printf("***************** GPU DEVICE GENERAL INFORMATION ******************* \n");
			printf("GPU Device number        : %d \n", i);

			clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_NAME, sizeof(ocl_dev_prop), &ocl_dev_prop, NULL);
			printf("GPU Device Name     : %s \n", ocl_dev_prop);

			clGetDeviceInfo((ocl_device_ids[i], CL_DEVICE_VENDER, sizeof(ocl_dev_prop), &ocl_dev_prop, NULL);)
			printf("GPU Device Vender     : %s \n", ocl_dev_prop);

			clGetDeviceInfo((ocl_device_ids[i], CL_DRIVER_VERSION, sizeof(ocl_dev_prop), &ocl_dev_prop, NULL);)
			printf("GPU Device Dtriver version     : %s \n", ocl_dev_prop);

			clGetDeviceInfo((ocl_device_ids[i], CL_DEVICE_VERSION, sizeof(ocl_dev_prop), &ocl_dev_prop, NULL);)
			printf("GPU Device OpenCL version     : %s \n", ocl_dev_prop);

			cl_uint clock_frequency;
			clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(clock_frequency), &clock_frequency, NULL);
			printf("GPU Device clock rate    : %u \n", clock_frequency);

			printf("\n");
			printf("***************** GPU DEVICE MEMORY INFORMATION ************* \n");
			printf("================================================================ \n");
			cl_ulong mem_size;
			clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(mem_size), &mem_size, NULL);
			printf("GPU Device global memory    : %llu Bytes \n", (unsigned long long)mem_size);

			clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(mem_size), &mem_size, NULL);
			printf("GPU Device local memory    : %llu Bytes \n", (unsigned long long)mem_size);

			clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(mem_size), &mem_size, NULL);
			printf("GPU Device constant Buffer    : %llu Bytes \n", (unsigned long long)mem_size);

			printf("\n");
			printf("*************** GPU DEVICE COMPUTE INFORMATION ***************** \n");
			printf("================================================================ \n");

			cl_uint compute_units;
			clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(compute_units), &compute_units, NULL);
			printf("GPU Device number of parallel Processor cores : %u \n", compute_units);

			size_t woritem_dims;
			clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_MAX_WORK_GROUP, sizeof(workgroup_size), &workgroup_size, NULL);
			printf("GPU Device work group size : %u \n", (unsigned int)workgroup_size);

			size_t woritem_dims;
			clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_MAX_WORK_ITEM_DIMENTIONS, sizeof(woritem_dims), &woritem_dims, NULL);
			printf("GPU Device work Item Dimentions : %u \n", (unsigned int)woritem_dims);

			size_t woritem_size[3];
			clGetDeviceInfo(ocl_device_ids[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(woritem_size), &woritem_size, NULL);
			printf("GPU Device work item sizes : %u %u %u \n", (unsigned int)woritem_size[0], (unsigned int)woritem_size[1], (unsigned int)woritem_size[2]);

		}
		free(ocl_device_ids);
	}
}
