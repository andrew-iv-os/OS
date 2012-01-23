/*
 Сделал я большую часть на системных вызовах. таким образом получается лучшая взаимосвязь с системой.
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define ARR_SIZE 0x1000
#define BF_SIZE ARR_SIZE*sizeof(int)

u_char buffer[BF_SIZE];

static struct vector{
	int *arr;
	int count;
	size_t capacity;
} int_arr;

static off_t file_size;


int compare(const void * a, const void * b) {
	return (*(int*) a - *(int*) b);
}

void array_sort(int arr[], int count) {
	qsort(arr, count, sizeof(int), compare);
	return;
}

void int_buffer_add(int val) {
	
	if(int_arr.count >= int_arr.capacity){
				int_arr.capacity = int_arr.capacity<<1;
				int_arr.arr = (int*)realloc(int_arr.arr, int_arr.capacity*sizeof(int));
				if( int_arr.arr == 0 )
				{
					fprintf(stderr,"malloc failed\n");
					exit(1);
				}
			}
	int_arr.arr[int_arr.count++] = val;

}


	int addfile(char *filename) {
		struct stat st;
		
		
		
		
		FILE *input_handle = fopen(filename, "rt");		
		if (input_handle == 0) {
			fprintf(stderr, "can't open file %s\n", filename);
			return EXIT_FAILURE;
		}
		int now;
		int add_now=0;
		
		while (fscanf(input_handle, "%d",&now) !=0&& feof(input_handle) == 0)
		{
			int_buffer_add(now);						add_now++;
		}		
		if(feof(input_handle) == 0)
		{
			int_arr.count-=add_now;
		}
		
		fclose(input_handle);
		return 0;
	}
	
	void printall(FILE * out)
	{
		int i;
		for(i=0;i<int_arr.count;i++)
				if(fprintf(out,"%d\n",int_arr.arr[i])<0)
					{
						fprintf(stderr, "can't write to file\n");
						exit(1);
					}
			
	}
	

int main(int argc, char **argv) {
		if(argc == 1)
		{
			puts("usage: ./a.out [file1] ... [fileN] [--output filename]\n");
			exit(0);
		}
		int_arr.arr = malloc(sizeof(int));
		int_arr.count=0;
		int_arr.capacity = 1;
		char * out_file_name = 0;
		int i;
		int read_output = 0;
		int argn;
		for (argn = 1; argn < argc; argn++) {
			if (strcmp(argv[argn], "--output") == 0) {
                read_output = 1;
            } else {
                if (read_output == 1) {
					out_file_name = argv[argn];
					
                    read_output = 0;
                } else {					
                    addfile(argv[argn]);
                    
                }			
			
		}		
	}
	
	array_sort(int_arr.arr,int_arr.count);
 FILE * out;	
	
		if(out_file_name == 0 || (out=fopen(out_file_name,"wt"))==0)
		{
			
			out = stdout;
			printall(out);
		}
		else
		{				
			printall(out);		
			fclose(out);
		}
	
		free(int_arr.arr);		
				
		return 0;
	};
