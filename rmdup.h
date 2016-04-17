#ifndef RMDUP_H
#define RMFUP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define TRUE 1
#define FALSE 0
#define NAME_LENGTH 256

typedef struct file_info {
	char name[NAME_LENGTH];		// Name of file
	char path[NAME_LENGTH]; 	// Path to file
	unsigned long time_cg; 		// Time last modified
	unsigned long protection; 	// Permissions
} file_info;

#endif
