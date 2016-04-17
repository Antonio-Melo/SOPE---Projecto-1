#include "rmdup.h"

int cmpFileTime (const void* elem1, const void* elem2);
int cmpFiles (const file_info* elem1, const file_info* elem2);

int main(int argc, char *argv[]) {

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

	closedir(dirp);

	// List regular files
	pid_t pid;
	pid = fork();

	if (pid == 0) {
		execl("lsdir", "lsdir", argv[1], NULL);
		exit(0);
	}
	else if (pid > 0) {
		wait(NULL);
	}

	// Put files read on an array
	char og_dir[NAME_LENGTH];
	realpath(argv[1], og_dir);
	strcat(og_dir, "/files.txt");
	FILE *files_file = fopen(og_dir, "r");

	file_info** files_array = malloc(0);
	int num_files = 0;


	while (!feof(files_file)) {
		// File name
		file_info *reg = malloc(sizeof(file_info));

		if (fscanf(files_file, "%s", reg->name) == -1) {
			break;
		}

		// Path name
		if (fscanf(files_file, "%s", reg->path) == -1) {
			break;
		}

		// Time last changed
		if (fscanf(files_file, "%lu", &(reg->time_cg)) == -1) {
			break;
		}
		// Permissions
		if (fscanf(files_file, "%lu", &(reg->protection)) == -1) {
			break;
		}

		num_files++;
		files_array = realloc (files_array, num_files * sizeof(file_info *));
		files_array[num_files - 1] = reg;
	}

	fclose(files_file);

	// Sort the array by the time of last change of the file
	qsort(files_array, num_files, sizeof(file_info *), cmpFileTime);


	// Create hlinks.txt
	realpath(argv[1], og_dir);
	strcat(og_dir, "/hlinks.txt");
	files_file = fopen(og_dir, "w+");

	// Remove duplicates
	int i, j;
	for (i = 0; i < num_files - 1; i++) {
		if (files_array[i] == NULL) {
			continue;
		}

		for (j = i + 1; j < num_files; j++) {
			if (files_array[j] == NULL) {
				continue;
			}

			if (cmpFiles(files_array[i], files_array[j]) == TRUE) {
				unlink(files_array[j]->path);
				link(files_array[i]->path, files_array[j]->path);
				fprintf(files_file, "%s\n", files_array[j]->path);
				files_array[j] = NULL;
			}
		}
	}


	return 0;
}
