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

struct anq_argv_handler *ahdl = NULL;

int anq_argv_init(void)
{
	/* argv_init shouldn't be called more than once. */
	assert(!ahdl);

	ahdl = malloc(sizeof(struct anq_argv_handler));

	if(!ahdl)
		return ANQ_ERR_UNALLOCATED_MEMORY;

	ahdl->argc = 0;
	return ANQ_OK;
}

void anq_argv_exit(void)
{
	free(ahdl);
	ahdl = NULL;
}

char *anq_argv_get_arg(int i)
{
	assert(i < ARGV_SIZE);
	return ahdl->argv[i];
}

short anq_argv_get_argc()
{
	return ahdl->argc;
}

argv_fp anq_argv_get_fop(int i)
{
	assert(i < ARGV_SIZE);
	return ahdl->fops[i];
}

int anq_argv_add_parameter(char *arg, argv_fp fp)
{
	int err;
	if(!ahdl) {
		err = anq_argv_init();
		if(err)
			return err;
	}

	assert(ahdl->argc < ARGV_SIZE);

	strncpy(ahdl->argv[ahdl->argc], arg, ARGV_READ_SIZE);
	/* set the callback function for said parameter */
	ahdl->fops[ahdl->argc] = fp;
	ahdl->argc++;
	return ANQ_OK;
}
