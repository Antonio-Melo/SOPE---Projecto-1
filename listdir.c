#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
int main(int argc, char *argv[])
{
 DIR *dirp;
struct dirent *direntp;
struct stat stat_buf;
char *str;
char name[200];
if (argc != 2)
 {
 fprintf( stderr, "Usage: %s dir_name\n", argv[0]);
 exit(1);
 }
if ((dirp = opendir( argv[1])) == NULL)
 {
 perror(argv[1]);
 exit(2);
 }
char og_dir[128];
realpath(argv[1],og_dir);
strcat(og_dir,"/files.txt");
FILE *files = fopen(og_dir,"w+");


while ((direntp = readdir( dirp)) != NULL)
 {
 sprintf(name,"%s/%s",argv[1],direntp->d_name);
 if (lstat(name, &stat_buf)==-1)
 {
 perror("lstat ERROR");
 exit(3);
 }
 printf("%10d - ",(int) stat_buf.st_ino);
 printf("%10d - ",(int) stat_buf.st_size);
 //printf("%10d - ",(int) stat_buf.st_gid);
 if (S_ISREG(stat_buf.st_mode)) str = "regular";
 else if (S_ISDIR(stat_buf.st_mode)) str = "directory";
 else str = "other";
 printf("%-25s - %s\n", direntp->d_name, str);
 }
 closedir(dirp);
 exit(0);
}
