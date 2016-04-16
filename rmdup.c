#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>


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

   	execl("lsdir", "lsdir", argv[1], NULL);

	return 0;
}
