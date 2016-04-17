#include "auxfunc.h"




// Compare file's last modification time
// Return 1 if time1 < time2
// Return 0 if time1 == time2
// Return -1 if time1 > time2
int cmpFileTime (const void* elem1, const void* elem2) {
	file_info *time1 = *(file_info**) elem1;
	file_info *time2 = *(file_info**) elem2;

	if (time1->time_cg < time2->time_cg) {
		return -1;
	}
	else if (time1->time_cg > time2->time_cg) {
		return 1;
	}
	else {
		return 0;
	}
}

// Check if the file elem2 is a duplicate of elem1
int cmpFiles (const file_info* elem1, const file_info* elem2) {

	// Compare name
	if (strcmp(elem1->name, elem2->name) != 0) {
		return FALSE;
	}

	// Compare permissions
	if (elem1->protection != elem2->protection) {
		return FALSE;
	}

	// Compare content
	FILE *file1 = fopen(elem1->path, "r");
	FILE *file2 = fopen(elem2->path, "r");

	char char1;
	char char2;

	do {
		char1 = getc(file1);
		char2 = getc(file2);
	} while ((char1 != EOF) && (char2 != EOF) && (char1 == char2));

	fclose(file1);
	fclose(file2);

	if (char1 == char2) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}
