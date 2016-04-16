#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define NAME_LENGTH 256
#define TRUE 1
#define FALSE 0


typedef struct file_info {
	char name[NAME_LENGTH]; // Name of file
	char path[NAME_LENGTH]; // Path to file
	unsigned long time_cg; 	// Time last modified
	unsigned long protection; 	// Permissions
} file_info;


int main(int argc, char *argv[]) {

	DIR *dirp;

	// Wrong number of arguments
	if (argc != 2) {
		fprintf( stderr, "Usage: %s dir_name\n", argv[0]);
		exit(1);
	}

	

	// Cant open directory
	if ((dirp = opendir( argv[1])) == NULL) {
		perror(argv[1]);
		exit(2);
	}
	
	closedir(dirp);

	// List regular files
	pid_t pid;
	pid = fork();

	if (pid == 0) {
		execl("lsdir", "lsdir", argv[1], NULL);
		exit(0);
	}
	else if (pid > 0) {
		wait(NULL);
	}

	// Put files read on an array
	char og_dir[NAME_LENGTH];
	realpath(argv[1], og_dir);
	strcat(og_dir, "/files.txt");
	FILE *files_file = fopen(og_dir, "r");

	file_info** files_array = malloc(0);
	int num_files = 0;

	
	while (!feof(files_file)) {
		num_files++;

		// File name
		file_info *reg = malloc(sizeof(file_info));

		fscanf(files_file, "%s", reg->name);

		// Path name
		fscanf(files_file, "%s", reg->path);

		// Time last changed
		fscanf(files_file, "%lu", &(reg->time_cg));	

		// Permissions
		fscanf(files_file, "%lu", &(reg->protection));

		files_array = realloc (files_array, num_files * sizeof(file_info *));
		files_array[num_files - 1] = reg;
	}
	
	printf("%i", num_files);
	int i;
	for (i = 0; i < num_files; i++) {
		printf("%s\n", ((file_info*) files_array[i])->name);

	}

	fclose(files_file);

	return 0;
}
