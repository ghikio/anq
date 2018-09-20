/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "io_utils.h"
#include "err_codes.h"
#include "crypto_data.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/wait.h>

/* Directory created at XDG_CONFIG_HOME for anq. */
#define CONFIG_DIR_NAME  "anq"
/* Readable configuration file at CONFIG_DIR_NAME. */
#define CONFIG_FILE_NAME "config"

extern struct crypto_data dt;

/* If opt is valid, set val in the associated crypto_data
 * field. Otherwise does nothing. */
void set_option(char *opt, char *val);
/* Dinamically build the path for the configuration file based
 * on the env var XDG_CONFIG_HOME, which points to the user's
 * configuration personal directory. */
char *get_config_file(void);

int parse_config()
{
	char *file = get_config_file();
	if(!file)
		goto nname_err;

	FILE *fp;
	if((fp = fopen(file, "rt")) == NULL)
		goto nfile_err;

	char buf[INPUT_SIZE];
	char opt[INPUT_SIZE];
	char val[INPUT_SIZE];

	while(fgets(buf, sizeof(buf) + 1, fp)) {
		slice_argv(buf, '=', opt, val);
		if(opt[0] == '\0' || val[0] == '\0')
			goto conf_err;
		
		set_option(opt, val);
	}

	fclose(fp);
	free(file);

	return 0;
conf_err:
	fclose(fp);
nfile_err:
	free(file);
nname_err:
	return ANQ_ERR_INVALID_CONF;
}

void set_option(char *opt, char *val)
{
	assert(opt[0] != '\0');
	assert(val[0] != '\0');

	int err;

	if((err = strncmp(opt, "ANQ_KEY_NAME", INPUT_SIZE))
			== 0)
		crypto_set_keyquery(&dt, val);
	else if((err = strncmp(opt, "ANQ_PASSPATH", INPUT_SIZE))
			== 0)
		crypto_set_passdir(&dt, val);
}

char *get_config_file(void)
{
	char *cftdir = getenv("XDG_CONFIG_HOME");
	if(!cftdir)
		goto exit_err;

	char cfdir[INPUT_SIZE];
	strncpy(cfdir, cftdir, INPUT_SIZE);

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
