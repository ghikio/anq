/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "io_utils.h"
#include "err_codes.h"
#include "argv_parser.h"
#include "crypto_data.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern struct crypto_data dt;

int set_arg(char *arg, char *val);
/* Check each argument for a defined one in argv_handler and if found
 * call it's callback. */
int parse_argv(int argc, char *argv[])
{
	int  err;
	char *arga = calloc(INPUT_SIZE + 1, sizeof(char));

	if(!arga) {
		err = ANQ_ERR_UNALLOCATED_MEMORY;
		goto arga_err;
	}

	char *argb = calloc(INPUT_SIZE + 1, sizeof(char));

	if(!argb) {
		err = ANQ_ERR_UNALLOCATED_MEMORY;
		goto argb_err;
	}

	/* Start at 1 so we don't parse the program name. */
	for(int i = 1; i < argc; i++) {
		slice_argv(argv[i], '=', arga, argb);

		err = set_arg(arga, argb);
		if(err)
			goto del_err;
	}

del_err:
	free(argb);
argb_err:
	free(arga);
arga_err:
	return err;
}


/* Maybe not the best place for these constants, but since they're
 * not yet needed anywhere else in the program, we'll secretly
 * keep them here ;) */
#define NAME	"ANQ"
#define AUTHOR  "Vitor Fernandes <contact@thecriw.com>"
#define VERSION "v0.0.1"
#define LICENSE "Released under BSD 3-Clause \"New\" or \"Revised\" License" \
		"\nCopyright (c) 2018, Vitor Flavio Fernandes Ferreira" \
		"\nAll rights reserved." \
		"\n\nhttps://spdx.org/licenses/BSD-3-Clause.html\n"

int set_arg(char *arg, char *val)
{
	if(strncmp(arg, "-d", INPUT_SIZE) == 0) {
		crypto_set_operation(&dt, ANQ_OP_DECRYPT);
	} else if(strncmp(arg, "-e", INPUT_SIZE) == 0) {
		crypto_set_operation(&dt, ANQ_OP_ENCRYPT);
	} else if(strncmp(arg, "-s", INPUT_SIZE) == 0) {
		if(val[0] == '\0')
			return ANQ_ERR_NO_SERVICE_VALUE;

		crypto_set_service(&dt, val);
	} else if(strncmp(arg, "-h", INPUT_SIZE) == 0) {
		printf("%s - %s\n%s\n\n%s\n", NAME, VERSION, 
		       AUTHOR, LICENSE);
		puts("-h - print help menu");
		puts("-d - decrypt password");
		puts("-e - encrypt password");
		puts("-s - password's service name");

		return ANQ_ERR_HELP_MENU;
	}

	return 0;
}
