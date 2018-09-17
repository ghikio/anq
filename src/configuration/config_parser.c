/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

// TODO [criw hp] comment this file.

#include "io_utils.h"
#include "crypto_data.h"
#include "config_parser.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>

#define CONFIG_DIR_NAME  "anq"
#define CONFIG_FILE_NAME "config"

extern struct crypto_data dt;

char *get_config_file();
void  set_option(char *opt, char *val);

int parse_config()
{
	char *file = get_config_file();
	if(!file)
		goto nname_err;

	FILE *fp;
	if((fp = fopen(file, "rt")) == NULL)
		goto nfile_err;

	char buf[ANQ_ENVVAR_SIZE];
	char opt[ANQ_ENVVAR_SIZE];
	char val[ANQ_ENVVAR_SIZE];

	while(fgets(buf, sizeof(buf) + 1, fp)) {
		slice_argv(buf, '=', opt, val);
		if(opt[0] == '\0' || val[0] == '\0')
			goto conf_err;
		printf("opt: %s\nval: %s\n", opt, val);
		
		set_option(opt, val);
	}

	free(file);

	return 0;
conf_err:
	// TODO [criw hp] add err code for this
	exit(32);
nfile_err:
	free(file);
nname_err:
	// TODO [criw hp] add err code for this
	return 99;
}

void set_option(char *opt, char *val)
{
	assert(opt[0] != '\0');
	assert(val[0] != '\0');

	int err;

	if((err = strncmp(opt, "ANQ_KEY_NAME", ANQ_ENVVAR_SIZE))
			== 0)
		crypto_set_keyquery(&dt, val);
	else if((err = strncmp(opt, "ANQ_PASSPATH", ANQ_ENVVAR_SIZE))
			== 0)
		crypto_set_passdir(&dt, val);
}

char *get_config_file()
{
	char *cftdir = getenv("XDG_CONFIG_HOME");
	if(!cftdir)
		goto exit_err;

	char cfdir[ANQ_ENVVAR_SIZE];
	strncpy(cfdir, cftdir, ANQ_ENVVAR_SIZE);

	char *cfpath = make_filename(cfdir,  CONFIG_DIR_NAME);
	if(!cfpath)
		goto exit_err;
	char *cffile = make_filename(cfpath, CONFIG_FILE_NAME);
	if(!cffile)
		goto ncffile_err;

	if(!check_file_access(cffile, X_OK)) {
		goto xacc_err;
	}

	free(cfpath);

	return cffile;

xacc_err:
	free(cffile);
ncffile_err:
	free(cfpath);
exit_err:
	return NULL;
}
