/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "err_codes.h"
#include "argv_handler.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

struct args_handler *ah = NULL;

int init_args(void)
{
	/* argv_init shouldn't be called more than once. */
	assert(!ah);

	ah = malloc(sizeof(struct args_handler));

	if(!ah)
		return ANQ_ERR_UNALLOCATED_MEMORY;

	ah->argc = 0;
	return ANQ_OK;
}

void exit_args(void)
{
	free(ah);
	ah = NULL;
}

char *args_get_arg(int i)
{
	assert(i < ARGV_SIZE);
	return ah->argv[i];
}

short args_get_argc()
{
	return ah->argc;
}

argv_fp args_get_fop(int i)
{
	assert(i < ARGV_SIZE);
	return ah->fops[i];
}

int args_add_arg(char *arg, argv_fp fp)
{
	int err;
	if(!ah) {
		err = init_args();
		if(err)
			return err;
	}

	assert(ah->argc < ARGV_SIZE);

	strncpy(ah->argv[ah->argc], arg, ARGV_READ_SIZE);
	/* set the callback function for said parameter */
	ah->fops[ah->argc] = fp;
	ah->argc++;
	return ANQ_OK;
}
