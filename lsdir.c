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
#define TRUE 1
#define FALSE 0


typedef struct file_info {
	char name[NAME_LENGTH]; // Name of file
	char path[NAME_LENGTH]; // Path to file
	mode_t protection; 	// Permissions 	
	time_t time_cg; 	// Time of last modification
} file_info;


void listFiles(DIR *dirp, FILE *files_file, char og_dir[]);
void fwriteDate(int tm, FILE *files_file);

int main(int argc, char *argv[])
{
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

	// Create files.txt on given directory
	char og_dir[NAME_LENGTH];
	realpath(argv[1], og_dir);
	strcat(og_dir, "/files.txt");
	FILE *files_file = fopen(og_dir, "w+");

	realpath(argv[1], og_dir);

	// List files
	listFiles(dirp, files_file, og_dir);

	// Get files and put on array

	exit(0);
}

// List files in a directory and subdirectories
void listFiles(DIR *dirp, FILE *files_file, char og_dir[]) {
	struct dirent *direntp;
	struct stat stat_buf;	
	char path[NAME_LENGTH];

	while ((direntp = readdir(dirp)) != NULL) {
		sprintf(path,"%s/%s", og_dir, direntp->d_name);
		
		if (lstat(path, &stat_buf)==-1) {
			perror("lstat ERROR");
			exit(3);
		}

		// Write to "files.txt" the files' data
		// Add to array of files
		// Ignore files with name "files.txt"
		if (S_ISREG(stat_buf.st_mode)) {
			if (strcmp(direntp->d_name, "files.txt") != 0) {				
				// Write name
				fwrite(direntp->d_name, sizeof(char), strlen(direntp->d_name), files_file);
				fwrite("\n", sizeof(char), sizeof(char), files_file);

				// Write path
				fwrite(path, sizeof(char), strlen(path), files_file);
				fwrite("\n", sizeof(char), sizeof(char), files_file);

				// Write date
				struct tm* time_changed = localtime(&(stat_buf.st_mtime));
				
				fwriteDate(time_changed->tm_year, files_file);
				fwriteDate(time_changed->tm_yday, files_file);
				fwriteDate(time_changed->tm_hour, files_file);
				fwriteDate(time_changed->tm_min, files_file);
				fwriteDate(time_changed->tm_sec, files_file);

				fwrite("\n", sizeof(char), sizeof(char), files_file);

				// Write permissions
				fprintf(files_file, "%d", stat_buf.st_mode);
				fwrite("\n", sizeof(char), sizeof(char), files_file);
			}
		}
		
		// Call listFiles for every directory
		// Creates a process for each directory
		// Ignores the parent directory ("..") and itself (".")
		else if (S_ISDIR(stat_buf.st_mode)) {
			pid_t pid;
			DIR *new_dirp;


			if (strcmp(direntp->d_name, "..") != 0 && strcmp(direntp->d_name, ".") != 0) {
				if ((new_dirp = opendir(path)) == NULL) {
					perror(path);
					exit(2);
				}
				
				pid = fork();

				if (pid == 0) {
					listFiles(new_dirp, files_file, path);
					break;
				}
			}
		}
	}

	closedir(dirp);
}


void fwriteDate(int tm, FILE *files_file) {
	if (tm < 10) {
		fwrite("000", sizeof(char), strlen("000"), files_file);
	}
	else if (tm < 100) {
		fwrite("00", sizeof(char), strlen("00"), files_file);
	}	
	else if (tm < 1000) {
		fwrite("0", sizeof(char), strlen("0"), files_file);
	}

	fprintf(files_file, "%d", tm);
}
