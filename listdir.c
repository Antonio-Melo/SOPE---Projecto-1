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

// Checks if there are directories in the given one
// Launnch list dir for each one
// argv[1] - Original directory (where files.txt is)
// argv[2] - Current directory
int main(int argc, char *argv[]) {
	struct dirent *direntp;
	struct stat stat_buf;	
	char path[NAME_LENGTH];
	DIR *dirp;
	pid_t pid;

	// Cant open directory
	if ((dirp = opendir( argv[2])) == NULL) {
		perror(argv[2]);
		exit(2);
	}

	pid = fork();
				
	if (pid > 0) {
		wait(NULL);
	}
	else {
		execl("listfiles", "listfiles", argv[1], argv[2], NULL);
		exit(0);
	}


	while ((direntp = readdir(dirp)) != NULL) {
		sprintf(path,"%s/%s", argv[2], direntp->d_name);
		
		if (lstat(path, &stat_buf)==-1) {
			perror("lstat ERROR");
			exit(3);
		}

		if (S_ISDIR(stat_buf.st_mode)) {
			DIR *new_dirp;


			if (strcmp(direntp->d_name, "..") != 0 && strcmp(direntp->d_name, ".") != 0) {
				if ((new_dirp = opendir(path)) == NULL) {
					perror(path);
					exit(2);
				}
								
				pid = fork();
				
				if (pid > 0) {
					wait(NULL);
				}
				else {
					execl("listdir", "listdir", argv[1], path, NULL);
					exit(0);
				}				
			}
		}
	}

	closedir(dirp);

	exit(0);
}
