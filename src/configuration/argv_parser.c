/**
 * @file
 * Implements parsing for command-line arguments.
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
#include "argv_parser.h"
#include "crypto_data.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern void   die(int err);
extern struct crypto_data anq_crypto_data;

int set_arg(char *arg, char *val);
/* Check each argument for a defined one in argv_handler and if found
 * call it's callback. */
int parse_argv(int argc, char *argv[])
{
	int  err;
	char *arga = calloc(INPUT_SIZE + 1, sizeof(char));

	if(!arga)
		die(ANQ_ERR_UNALLOCATED_MEMORY);

	char *argb = calloc(INPUT_SIZE + 1, sizeof(char));

	if(!argb)
		die(ANQ_ERR_UNALLOCATED_MEMORY);

	if(argc == 1)
		set_arg("-h", NULL);

	/* Start at 1 so we don't parse the program name. */
	for(int i = 1; i < argc; i++) {
		slice_argv(argv[i], '=', arga, argb);

		err = set_arg(arga, argb);
		if(err)
			goto del_err;
	}

	free(argb);
	free(arga);

	return 0;

del_err:
	free(argb);
	free(arga);
	return err;
}


/* Maybe not the best place for these constants, but since they're
 * not yet needed anywhere else in the program, we'll secretly
 * keep them here ;) */
#define NAME	"ANQ"
#define AUTHOR  "Vitor Fernandes <me@ghikio.dev>"
#define VERSION "v0.0.1"
#define LICENSE "Released under BSD 3-Clause \"New\" or \"Revised\" License" \
		"\nCopyright (c) 2018-2019, Vitor Fernandes" \
		"\nAll rights reserved." \
		"\n\nhttps://spdx.org/licenses/BSD-3-Clause.html\n"

int set_arg(char *arg, char *val)
{
	if(strncmp(arg, "-l", INPUT_SIZE) == 0) {
		crypto_set_operation(&anq_crypto_data, ANQ_OP_LIST);
	} else if(strncmp(arg, "-d", INPUT_SIZE) == 0) {
		crypto_set_operation(&anq_crypto_data, ANQ_OP_DECRYPT);
	} else if(strncmp(arg, "-e", INPUT_SIZE) == 0) {
		crypto_set_operation(&anq_crypto_data, ANQ_OP_ENCRYPT);
	} else if(strncmp(arg, "-s", INPUT_SIZE) == 0) {
		if(val[0] == '\0')
			return ANQ_ERR_NO_SERVICE_VALUE;

		crypto_set_service(&anq_crypto_data, val);
	} else if(strncmp(arg, "-v", INPUT_SIZE) == 0) {
		printf("version: %s\n", VERSION);
		return ANQ_ERR_HELP_MENU;
	} else if(strncmp(arg, "-h", INPUT_SIZE) == 0) {
		printf("%s - %s\n%s\n\n%s\n", NAME, VERSION, 
		       AUTHOR, LICENSE);
		puts("-v - print version");
		puts("-h - print help menu");
		puts("-l - list passwords");
		puts("-d - decrypt password");
		puts("-e - encrypt password");
		puts("-s - password's service name");

		return ANQ_ERR_HELP_MENU;
	}

	return 0;
}
