#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char **argv[]){
	struct dirent *direntp;
	struct stat stat_buf;
	char name[256];
  char og_dir[256];

  // Create files.txt on given directory
	realpath(argv[1], og_dir);
	strcat(og_dir, "/files.txt");
	FILE *files = fopen(og_dir, "w+");

  //Reset
	realpath(argv[1], og_dir);


	while ((direntp = readdir(argv[1])) != NULL) {
		sprintf(name,"%s/%s", og_dir, direntp->d_name);

		if (lstat(name, &stat_buf)==-1) {
			perror("lstat ERROR");
			exit(3);
		}

		// Write to "files.txt" the file's data
		// Ignore files with name "files.txt"
		if (S_ISREG(stat_buf.st_mode)) {
			if (strcmp(direntp->d_name, "files.txt") != 0) {
				char size[50];
				sprintf(size, " %d", (int) stat_buf.st_size);

				fwrite(direntp->d_name, sizeof(char), strlen(direntp->d_name), files);
				fwrite(size,sizeof(char),strlen(size),files);
				fwrite("\n", sizeof(char), sizeof(char), files);
			}
		}

		// Call listFiles for every directory
		// Creates a process for each directory
		// Ignores the parent directory ("..") and itself (".")
		else if (S_ISDIR(stat_buf.st_mode)) {
			pid_t pid;
			DIR *new_dirp;
			struct dirent *new_direntp;


			if (strcmp(direntp->d_name, "..") != 0 && strcmp(direntp->d_name, ".") != 0) {
				if ((new_dirp = opendir(name)) == NULL) {
					perror(name);
					exit(2);
				}

				if ((pid = fork()) == 0) {
					listFiles(new_dirp, files, name);
					break;
				}

			}
		}
	}

	closedir(argv[1]);
}
