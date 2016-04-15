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
  pid_t pid;

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

  //Fork to read files data
  pid = fork();

  if (pid <0){
    perror("Fork failed");
    exit(3);
  }
  //Child
  else if(pid == 0){
    execl("lsdir", "lsdir", argv[1], NULL);
    perror(argv[1]);
    exit(4);
  }

	// List files
	//listFiles(dirp, files, og_dir);
	exit(0);
}
