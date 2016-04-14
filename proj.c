#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void listFiles(DIR *dirp, FILE *files, char og_dir[]);

int main(int argc, char *argv[])
{
	DIR *dirp;
	char *str;

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
	char og_dir[128];
	realpath(argv[1], og_dir);
	strcat(og_dir, "/files.txt");
	FILE *files = fopen(og_dir, "w+");

	realpath(argv[1], og_dir);

	// List files
	listFiles(dirp, files, og_dir);	

	closedir(dirp);
	exit(0);
}


void listFiles(DIR *dirp, FILE *files, char og_dir[]) {
	struct dirent *direntp;
	struct stat stat_buf;	
	char name[128];

	while ((direntp = readdir(dirp)) != NULL) {
		
		//strcpy(name, og_dir);
		//strcat(name, "/");
		//strcat(name, direntp->d_name);
		sprintf(name,"%s/%s", og_dir, direntp->d_name); // <----- NOTAR
		// alternativa a chdir(); ex: anterior
		
		if (lstat(name, &stat_buf)==-1) {// testar com stat() 
			perror("lstat ERROR");
			exit(3);
		}


		// printf("%10d - ",(int) stat_buf.st_ino);
		if (S_ISREG(stat_buf.st_mode)) {
			//sprintf(name2, "%s", direntp->d_name);

			printf(direntp->d_name);
			printf("\n");
			fwrite(direntp->d_name, sizeof(char), strlen(direntp->d_name), files);
			fwrite("\n", sizeof(char), sizeof(char), files);
		}
			
		else if (S_ISDIR(stat_buf.st_mode)) {
			pid_t pid;
			DIR *new_dirp;
						
			if ((pid = fork()) == 0) {
				if ((new_dirp = opendir(name)) == NULL) {
					perror(name);
					exit(2);
				}

				listFiles(new_dirp, files, name);
			}
		}
	}
}
