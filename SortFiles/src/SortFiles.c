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
static int uk_symb;
static int input_handle;
static char is_availible = 1;
static int read_count;

inline u_char get_next_symb(u_char buffer[], int size) {
	is_availible = 1;
	if (uk_symb % size == 0) {
		read_count = read(input_handle, buffer, size);
		if (read_count == 0) {
			is_availible = 0;
			if (uk_symb != file_size) {
				fprintf(stderr, "can't read file till the end");
			}
			return 0;
		}
	}
	if (uk_symb % size >= read_count) {
		is_availible = 0;
		return 0;
	}
	uk_symb++;
	return buffer[(uk_symb - 1) % size];

}

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

void populate()
/*
 * Чтение делаю ручками потому что нужно обрабатывать слишком большие числа а также мусор в файлах
 */
 
 {
	long long now = 0;
	uk_symb=0;
	is_availible=1;

	int state = 0;
	char min = 1;
	/*
	 0: deafult
	 1: - on start
	 2: число уже идёт
	 3: игнорить число
	 */
	while (is_availible) {
		u_char symb = get_next_symb(buffer, BF_SIZE);
		if(!is_availible)
			break;
		switch (state) {
		case 0:
			now = 0;
			if (symb == '-') {
				min = -1;
				state = 1;
			} else if (symb >= '0' && symb <= '9') {
				min = 1;
				now = symb - '0';
				state = 2;
			} else {

				//fprintf(stderr, "bad symbol %c at %d\n", symb, (int) uk_symb);
			}
			break;
		case 1:

		case 2:
			if (symb >= '0' && symb <= '9') {
				state = 2;
				now = now * 10 + symb - '0';
				if (now & 0xFFFF80000000) {
				/*	fprintf(stderr,
							"value in bad range at %d\n, ignoring next digits",
							(int) uk_symb);*/
					now = now / 10;
					int_buffer_add(now * min);
					state = 3;
				}
			} else {
				if (state != 1)
				{
					int_buffer_add(now * min);
					state = 0;
				}
				//if (!isblank(symb));
					/*fprintf(stderr, "bad symbol %c at %d\n", symb,
							(int) uk_symb);*/
			}

			break;
		case 3:
			if (!(symb >= '0' && symb <= '9')) {
				state = 0;

			}
			break;
		}

	}
}



	int addfile(char *filename) {
		struct stat st;
		uk_symb=0;
		if (stat(filename, &st) == -1) {
			fprintf(stderr, "can't stat file %s\n", filename);
			return EXIT_FAILURE;
		}
		file_size = st.st_size;
		input_handle = open(filename, O_RDONLY);		
		populate();
		close(input_handle);
		return 0;
	}
	
	static FILE * out;	
	void printall()
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
	
		if(out_file_name == 0 || (out=fopen(out_file_name,"wt"))==0)
		{
			
			out = stdout;
			printall();
		}
		else
		{				
			printall();		
			fclose(out);
		}
	
		free(int_arr.arr);		
				
		return 0;
	};
