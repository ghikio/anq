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

struct s_argv_handler *ahdl;

int argv_init(void)
{
	ahdl = malloc(sizeof(struct s_argv_handler));

	if(!ahdl)
		return ANQ_ERR_UNALLOCATED_STRUCT;

	ahdl->argc = 0;
	return 0;
}

void argv_exit(void)
{
	free(ahdl);
}

char *argv_get_arg(int i)
{
	if(i >= ARGV_SIZE)
		return NULL;
	return ahdl->argv[i];
}

bool argv_get_del(int i)
{
	if(i >= ARGV_SIZE)
		return NULL;
	return ahdl->has_del[i];
}

short argv_get_argc()
{
	return ahdl->argc;
}

argv_fptr argv_get_fop(int i)
{
	if(i >= ARGV_SIZE)
		return NULL;
	return ahdl->fops[i];
}

int argv_add_parameter(char *arg, bool del, argv_fptr fop)
{
	if(ahdl->argc >= ARGV_SIZE) {
		return ANQ_ERR_MAX_ARGS_EXCEEDED;
	}

	strncpy(ahdl->argv[ahdl->argc], arg, ARGV_READ_SIZE);
	ahdl->has_del[ahdl->argc] = del;
	/* set the callback function for said parameter */
	ahdl->fops[ahdl->argc] = fop;
	ahdl->argc++;

	return 0;
}
