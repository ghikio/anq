/**
 * @file
 * Implements parsing for the configuration file.
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

extern struct crypto_data anq_crypto_data;

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

	while(fgets(buf, sizeof(buf), fp)) {
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
		crypto_set_keyquery(&anq_crypto_data, val);
	else if((err = strncmp(opt, "ANQ_PASSPATH", INPUT_SIZE))
			== 0)
		crypto_set_passdir(&anq_crypto_data, val);
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

	if(!check_file_access(cffile, R_OK)) {
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
