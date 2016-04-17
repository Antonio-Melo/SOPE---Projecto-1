#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>

#define NAME_LENGTH 256


int main(int argc, char *argv[]) {

	// Wrong number of arguments
	if (argc != 2) {
		fprintf( stderr, "Usage: %s dir_name\n", argv[0]);
		exit(1);
	}

	// Create files.txt on given directory
	char og_dir[NAME_LENGTH];
	realpath(argv[1], og_dir);
	strcat(og_dir, "/files.txt");
	FILE *files_file = fopen(og_dir, "w+");
	fclose(files_file);

	realpath(argv[1], og_dir);

	// List files
	pid_t pid;

	pid = fork();

	if (pid == 0) {
		execl("listdir", "listdir", og_dir, og_dir, NULL);
		exit(0);
	}
	else if (pid > 0) {
		wait(NULL);
	}

	exit(0);
}
