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

// Lists the files in a directory
// argv[1] - Original directory (where files.txt is)
// argv[2] - Current directory

int main(int argc, char *argv[]) {
	struct dirent *direntp;
	struct stat stat_buf;	
	char path[NAME_LENGTH];
	DIR *dirp;

	// Open files.txt
	char og_dir[NAME_LENGTH];
	realpath(argv[1], og_dir);
	strcat(og_dir, "/files.txt");
	FILE *files_file = fopen(og_dir, "a");

	// Cant open directory
	if ((dirp = opendir( argv[2])) == NULL) {
		perror(argv[2]);
		exit(2);
	}


	while ((direntp = readdir(dirp)) != NULL) {
		sprintf(path,"%s/%s", argv[2], direntp->d_name);
		
		if (lstat(path, &stat_buf)==-1) {
			perror("lstat ERROR");
			exit(3);
		}

		// Write to "files.txt" the files' data
		// Ignore files with name "files.txt"
		if (S_ISREG(stat_buf.st_mode)) {

			if (strcmp(direntp->d_name, "files.txt") != 0) {				
				// Write name
				fprintf(files_file, "%s\n", direntp->d_name);

				// Write path
				fprintf(files_file, "%s\n", path);

				// Write date (nr seconds since epoch)
				unsigned long long time_changed = (unsigned long long) stat_buf.st_mtime;
				
				fprintf(files_file, "%llu\n", time_changed);

				// Write permissions
				fprintf(files_file, "%d\n", stat_buf.st_mode);		
			}
		}
	}

	fclose(files_file);
	closedir(dirp);

	exit(0);
}
