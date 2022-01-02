#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstring>

#include <time.h>
#include <sys/time.h>


/* gpu */
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif


#define 	MEM_SIZE                (128)
#define 	MAX_SOURCE_SIZE 	(0x100000)
#define   LO_THRES                0.88
#define   HI_THRES                0.92

std::vector<std::vector<std::vector<int>>> do_gpu(char *a,char *b,
            int *as,int *bs,
            int *al,int *bl,
            const int len1,
            const int len2,
            float *res,
            int MBS,int lws){
  std::vector<std::vector<std::vector<int>>> out;
  std::vector<std::vector<int>> out1;
  std::vector<std::vector<int>> out2;
  //defining opencl memory objects
  cl_mem cl_a,cl_as,cl_al,cl_b,cl_bs,cl_bl,cl_res;
  cl_a,cl_as,cl_al,cl_b,cl_bs,cl_bl,cl_res = NULL;

  //opencl prep
  cl_platform_id platform_id = NULL;
	cl_uint ret_num_platforms;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_context context = NULL;
	cl_kernel kernel = NULL;
	cl_program program = NULL;
	cl_command_queue command_queue = NULL;
	cl_int ret;

  //Load the source code containing the kernel
	char string[MEM_SIZE];
	FILE *fp;
	char fileName[] = "./src/kernel.cl";
	char *source_str;
	size_t source_size;
	fp = fopen(fileName, "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char*) malloc (MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

  //Opencl Setup
  // Platform
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	if (ret != CL_SUCCESS) {
	    printf("Failed to get platform ID.\n");
		return out;
	}
	// Device
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	if (ret != CL_SUCCESS) {
	    printf("Failed to get device ID.\n");
		return out;
	}
	// Context
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, NULL);//&ret);
	if (ret != CL_SUCCESS) {
		printf("Failed to create OpenCL context.\n");
	    return out;
	}
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if (ret != CL_SUCCESS) {
	   printf("Failed to create command queue %d\n", (int) ret);
	   return out;
	}



  //Creating the memory buffers
  int size_a_long = 0;
  for(int i = 0; i<len1 ; i++) size_a_long += as[i];
  int size_b_long = 0;
  for(int i = 0; i<len2 ; i++) size_b_long += bs[i];

  cl_a = clCreateBuffer (context, CL_MEM_READ_ONLY, size_a_long * sizeof(char), NULL, &ret);
  cl_as = clCreateBuffer (context, CL_MEM_READ_ONLY, len1 * sizeof(int), NULL, &ret);
  cl_al = clCreateBuffer (context, CL_MEM_READ_ONLY, len1 * sizeof(int), NULL, &ret);
  cl_b = clCreateBuffer (context, CL_MEM_READ_ONLY, size_b_long * sizeof(char), NULL, &ret);
  cl_bs = clCreateBuffer (context, CL_MEM_READ_ONLY, len2 * sizeof(int), NULL, &ret);
  cl_bl = clCreateBuffer (context, CL_MEM_READ_ONLY, len2 * sizeof(int), NULL, &ret);
  cl_res = clCreateBuffer (context, CL_MEM_WRITE_ONLY,  len1*len2 * sizeof(float), NULL, &ret);

  // Create Kernel Program from source
	 program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
			(const size_t *)&source_size, &ret);
	if (ret != CL_SUCCESS) {
		printf("Failed to create OpenCL program from source %d\n", (int) ret);
		return out;
	}
	// Build Kernel Program
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if (ret != CL_SUCCESS) {
		printf("Failed to build program %d\n", (int) ret);
		char build_log[16348];
		clGetProgramBuildInfo (program, device_id, CL_PROGRAM_BUILD_LOG, sizeof (build_log), build_log, NULL);
		printf ("Error in kernel: %s\n", build_log);
		return out;
	}
	// Create OpenCL Kernel
	kernel = clCreateKernel(program, "jarowinkler", &ret);
	if (ret != CL_SUCCESS) {
		printf("Failed to create kernel %d\n", (int) ret);
		return out;
	}

  ret &= clEnqueueWriteBuffer (command_queue, cl_a, CL_TRUE, 0, (size_t) size_a_long * sizeof(char), (void *)a, 0, NULL, NULL);
  ret &= clEnqueueWriteBuffer (command_queue, cl_al, CL_TRUE, 0, len1 * sizeof(int), (void *)al, 0, NULL, NULL);
  ret &= clEnqueueWriteBuffer (command_queue, cl_as, CL_TRUE, 0, len1 * sizeof(int), (void *)as, 0, NULL, NULL);

  ret &= clEnqueueWriteBuffer (command_queue, cl_b, CL_TRUE, 0, (size_t) size_b_long * sizeof(char), (void *)b, 0, NULL, NULL);
  ret &= clEnqueueWriteBuffer (command_queue, cl_bl, CL_TRUE, 0, len2 * sizeof(int), (void *)bl, 0, NULL, NULL);
  ret &= clEnqueueWriteBuffer (command_queue, cl_bs, CL_TRUE, 0, len2 * sizeof(int), (void *)bs, 0, NULL, NULL);




  ret  = clSetKernelArg(kernel, 0, sizeof (cl_mem), (void *) &cl_a);
  ret &= clSetKernelArg(kernel, 1, sizeof (cl_mem), (void *) &cl_b);
  ret &= clSetKernelArg(kernel, 2, sizeof (cl_mem), (void *) &cl_as);
  ret &= clSetKernelArg(kernel, 3, sizeof (cl_mem), (void *) &cl_bs);
  ret &= clSetKernelArg(kernel, 4, sizeof (cl_mem), (void *) &cl_al);
  ret &= clSetKernelArg(kernel, 5, sizeof (cl_mem), (void *) &cl_bl);
  ret &= clSetKernelArg(kernel, 6, sizeof (cl_mem), (void *) &cl_res);
  ret &= clSetKernelArg(kernel, 7, sizeof (cl_int), (void *) &len1);
  ret &= clSetKernelArg(kernel, 8, sizeof (cl_int), (void *) &len2);
  ret &= clSetKernelArg(kernel,11, sizeof (cl_int), (void *) &MBS);
  if (ret != CL_SUCCESS) {
    printf("Failed to set kernel arguments %d\n", (int) ret);
    return out;
  }

  //Setting Work size
  std::vector<size_t> local_work_size = {lws,lws};
  std::vector<size_t> global_work_size = {MBS,MBS};

  int timesa = ceil((float)(len1)/(float)(MBS));
  int timesb = ceil((float)(len2)/(float)(MBS));
  std::vector<int> varo = {0,0,0};
  for(int i = 0; i < timesa ; i++){
    for(int j = 0; j < timesb; j++){
      ret &= clSetKernelArg(kernel, 9, sizeof (cl_int), (void *) &i);
      ret &= clSetKernelArg(kernel,10, sizeof (cl_int), (void *) &j);

      ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_work_size.data(), local_work_size.data(), 0, NULL, NULL);
      if (ret != CL_SUCCESS) {
        printf("Failed to execute kernel for execution %d\n", (int) ret);
        return out;
      }

      //Copying data back
      ret = clEnqueueReadBuffer(command_queue, cl_res, CL_TRUE, 0, MBS*MBS * sizeof(float), (void *)res, 0, NULL, NULL);
      if (ret != CL_SUCCESS) {
        printf("Failed to copy data from device to host %d\n", (int) ret);
        return out;
      }
      for(int k = 0; k < MBS*MBS; k++){
        if(res[k] >= HI_THRES){
          varo[0] = i;
          varo[1] = j;
          varo[2] = k;
          out2.push_back(varo);
        }else if(res[k]>= LO_THRES){
          varo[0] = i;
          varo[1] = j;
          varo[2] = k;
          out1.push_back(varo);
        }
      }
    }
  }
  /* free device resources */
  clFlush(command_queue);
  clFinish(command_queue);
  clReleaseKernel(kernel);
  clReleaseProgram(program);

  clReleaseMemObject(cl_a);
  clReleaseMemObject(cl_b);
  clReleaseMemObject(cl_as);
  clReleaseMemObject(cl_bs);
  clReleaseMemObject(cl_al);
  clReleaseMemObject(cl_bl);
  clReleaseMemObject(cl_res);

  clReleaseCommandQueue(command_queue);
  clReleaseContext(context);
    /* free host resources */
  free(source_str);

  out.push_back(out1);
  out.push_back(out2);

  return out;
}
