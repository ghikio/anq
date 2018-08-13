/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "err_codes.h"
#include "argv_parser.h"
#include "argv_handler.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Slice argv in two parts denoted by the (del)imiter. 
 * First part go into sa and second part into sb.
 *
 * If no delimiter is found, returns ANQ_ERR_NO_DELIMITER,
 * otherwise 0, even if sb ends being NULL. */
ANQ_ERR argv_slice(char *argv, char del, char *sa, char *sb);
/* Grab the argument sliced (e.g. arga = "-d", argb = "~/home")
 * and check if it coincide with one of our record. */
ANQ_ERR argv_check_argument(char *arga, char *argb, int err);

/* Check each argument for a defined one in argv_handler and if found
 * call it's callback. */
ANQ_ERR argv_parse(int argc, char *argv[])
{
	int  err = 0;
	char *arga = malloc(sizeof(char) * ARGV_READ_SIZE + 1);

	if(!arga) {
		err = ANQ_ERR_UNALLOCATED_MEMORY;
		goto arga_err;
	}

	char *argb = malloc(sizeof(char) * ARGV_READ_SIZE + 1);

	if(!argb) {
		err = ANQ_ERR_UNALLOCATED_MEMORY;
		goto argb_err;
	}

	/* Start at 1 so we don't parse the program name. */
	for(int i = 1; i < argc; i++) {
		err = argv_slice(argv[i], '=', arga, argb);
		err = argv_check_argument(arga, argb, err);

		if(err == ANQ_ERR_NO_DELIMITER)
			goto del_err;
	}

del_err:
	free(argb);
argb_err:
	free(arga);
arga_err:

	return err;
}

// TODO [criw lp] I feel like we should optimize this function arguments.
ANQ_ERR argv_check_argument(char *arga, char *argb, int err)
{
	short acnt = argv_get_argc();
	for(short x = 0; x < acnt; x++) {
		if(strncmp(argv_get_arg(x), arga, ARGV_READ_SIZE) == 0) {
			/* Check if the argument must have a delimiter. */
			if(err == ANQ_ERR_NO_DELIMITER && argv_get_del(x))
				return ANQ_ERR_NO_DELIMITER;

			/* Get the callback pointer and calls it. */
			argv_fptr fop = argv_get_fop(x);
			(*fop)(arga, argb);
		}
	}

	return ANQ_OK;
}

ANQ_ERR argv_slice(char *argv, char del, char *sa, char *sb)
{
	int  i = 0;
	bool found = false;

	/* Keeps searching for the delimiter, if it finds
	 * the string null character first, return an error
	 * saying that there is no delimiter. */
	while(argv[i] != '\0' && !found) {
		if(argv[i] == del) 
			found = true;
		else
			sa[i] = argv[i];

		i++;
	}
	sa[i] = '\0';

	if(found == false)
		return ANQ_ERR_NO_DELIMITER;

	int n = 0;
	while(argv[i] != '\0') {
		sb[n] = argv[i];
		n++;
		i++;
	}
	sb[n + 1] = '\0';

	return ANQ_OK;
}
