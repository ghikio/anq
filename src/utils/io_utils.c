/**
 * @file
 * Provide an interface to handle IO manipulation.
 *
 * @authors
 * Copyright (c) 2018-2019, Vitor Fernandes
 *
 * All rights reserved.
 *
 * @copyright
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "io_utils.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

bool check_dir_exists(char *d)
{
	assert(d != NULL);

	DIR *dp;
	if(access(d, F_OK) != 0 || (dp = opendir(d)) == NULL) {
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

int list_dir(char *d)
{
	assert(d != NULL);

	if(!check_dir_exists(d))
		return -1;

	DIR *dp;
	struct dirent *ent;
	if((dp = opendir(d)) == NULL) {
		return -1;
	}

	printf("== %s ==\n", d);
	while((ent = readdir(dp)) != NULL) {
		if(strncmp(ent->d_name, ".", 255) != 0 
				&& strncmp(ent->d_name, "..", 255) != 0)
			printf (">> %s\n", ent->d_name);
	}

	closedir(dp);
	return 0;
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
