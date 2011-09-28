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

#define ARR_SIZE 0x1000000
#define BF_SIZE ARR_SIZE*sizeof(int)

u_char buffer[BF_SIZE];
int *int_buffer;
int int_buffer_count;
off_t file_size;
int uk_symb;
int input_handle;
char is_availible = 1;
int read_count;

inline u_char get_next_symb(u_char buffer[], int size) {
	is_availible = 1;
	if (uk_symb % size == 0) {
		read_count = read(input_handle, buffer, size);
		if (read_count == 0) {
			is_availible = 0;
			if (uk_symb != file_size) {
				fprintf(stderr, "can't read file o end");
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
	int_buffer[int_buffer_count++] = val;

}

void populate() {
	long long now = 0;
	int_buffer_count=0;
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

				fprintf(stderr, "bad symbol %c at %d\n", symb, (int) uk_symb);
			}
			break;
		case 1:

		case 2:
			if (symb >= '0' && symb <= '9') {
				state = 2;
				now = now * 10 + symb - '0';
				if (now & 0xFFFF80000000) {
					fprintf(stderr,
							"value in bad range at %d\n, ignoring next digits",
							(int) uk_symb);
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
				if (!isblank(symb))
					fprintf(stderr, "bad symbol %c at %d\n", symb,
							(int) uk_symb);
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



	int sortfile(char *filename) {
		struct stat st;
		uk_symb=0;
		if (stat(filename, &st) == -1) {
			fprintf(stderr, "can't stat file %s", filename);
			return EXIT_FAILURE;
		}
		file_size = st.st_size;
		input_handle = open(filename, O_RDONLY);
		int_buffer = malloc(st.st_size / 2 + 1);
		int_buffer_count=0;
		populate();
		close(input_handle);
		array_sort(int_buffer,int_buffer_count);
		int i;
		char *new_name=malloc(strlen(filename)+10);
		sprintf(new_name,"%s.sorted",filename);
		FILE* out;
		if(out=fopen(new_name,"wt"))
		{
			for(i=0;i<int_buffer_count;i++)
				fprintf(out,"%d\n",int_buffer[i]);
			fclose(out);
		}
		free(int_buffer);
		free(new_name);

		/*u_char *barray=malloc(st.st_size);
		 int *iarray = malloc();*/

		return 0;
	}

int main(int argc, char **argv) {

		int i;
		for (i = 1; i < argc; i++) {
			sortfile(argv[i]);
		}
		return 0;
	};
