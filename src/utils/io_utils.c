/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "io_utils.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

bool check_dir_exists(char *f)
{
	assert(f != NULL);

	DIR *dp;
	if(access(f, F_OK) != 0 || (dp = opendir(f)) == NULL) {
		return false;
	}

	closedir(dp);
	return true;
}

bool check_file_access(char* f, int mode)
{
	assert(f != NULL);
	assert(mode == R_OK || mode == W_OK || mode == X_OK || mode == F_OK);

	return (access(f, mode) == 0);
}

char *make_filename(char *path, char *file)
{
	assert(path != NULL);
	assert(file != NULL);

	char *filename = calloc(strlen(path) + strlen(file) + 2,
				sizeof(char));
	if(!filename)
		return NULL;

	strcpy(filename, path);
	strcat(filename, "/");
	strcat(filename, file);

	return filename;
}

void slice_argv(char *argv, char del, char *sa, char *sb)
{
	int  i = 0;
	bool found = false;

	/* Keeps searching for the delimiter, if it finds
	 * the string null character first, return saying 
	 * that there is no delimiter. */
	while(argv[i] != '\0' && !found) {
		if(argv[i] == del) {
			found = true;
			sa[i] = '\0';
		} else {
			sa[i] = argv[i];
		}

		i++;
	}

	if(found == false)
		return;

	int n = 0;
	while(argv[i] != '\0') {
		sb[n] = argv[i];
		n++;
		i++;
	}
	if(sb[n-1] == '\n')
		sb[n-1] = '\0';
	else
		sb[n] = '\0';
}
