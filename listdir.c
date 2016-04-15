#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char **argv){
	DIR *dirp;
	struct dirent *direntp;
	struct stat stat_buf;
	char name[256];
  char og_dir[256];

	if(argc !=2){
		fprintf(stderr, "Usage: %s dir_name\n",argv[0]);
		exit(1);
	}
	// Can open directory
	if ((dirp = opendir( argv[1])) == NULL) {
		perror(argv[1]);
		exit(2);
	}



  // Create files.txt on given directory
	realpath(argv[1], og_dir);
	strcat(og_dir, "/files.txt");
	FILE *files = fopen(og_dir, "w+");

  //Reset
	realpath(argv[1], og_dir);

	while ((direntp = readdir(dirp)) != NULL) {
		sprintf(name,"%s/%s", argv[1], direntp->d_name);

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
					exit(4);
				}

				pid = fork();

				if(pid <0){
					perror("Fork Failed");
					exit(5);
				}
				else if(pid == 0){
					execl("lsdir","lsdir",new_dirp,NULL);
					closedir(dirp);
					exit(6);
				}
			}
		}
	}
	closedir(dirp);
	return 0;
}
