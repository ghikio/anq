/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "io_utils.h"
#include "err_codes.h"
#include "argv_parser.h"
#include "argv_handler.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Grab the argument sliced (e.g. arga = "-d", argb = "~/home")
 * and check if it coincide with one of our record. */
int check_argv(char *arga, char *argb);

/* Check each argument for a defined one in argv_handler and if found
 * call it's callback. */
int parse_argv(int argc, char *argv[])
{
	int  err;
	char *arga = calloc(ARGV_READ_SIZE + 1, sizeof(char));

	if(!arga) {
		err = ANQ_ERR_UNALLOCATED_MEMORY;
		goto arga_err;
	}

	char *argb = calloc(ARGV_READ_SIZE + 1, sizeof(char));

	if(!argb) {
		err = ANQ_ERR_UNALLOCATED_MEMORY;
		goto argb_err;
	}

	/* Start at 1 so we don't parse the program name. */
	for(int i = 1; i < argc; i++) {
		slice_argv(argv[i], '=', arga, argb);
		err = check_argv(arga, argb);

		if(err)
			goto del_err;
	}

del_err:
	free(argb);
argb_err:
	free(arga);
arga_err:
	exit_args();
	return err;
}

int check_argv(char *arga, char *argb)
{
	int err;

	short ahc = args_get_argc();
	for(short x = 0; x < ahc; x++) {
		if(strncmp(args_get_arg(x), arga, ARGV_READ_SIZE) == 0) {
			/* Get the callback pointer and calls it. */
			argv_fp fop = args_get_fop(x);
			err = (*fop)(arga, argb);

			if(err)
				return err;
		}
	}

	return ANQ_OK;
}
