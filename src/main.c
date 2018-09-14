/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "parser.h"
#include "anq_ops.h"
#include "anq_data.h"
#include "err_codes.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct anq_data dt;
struct anq_ops  op;

int main(int argc, char *argv[])
{
	int err;

	/* Since the argv handler is the first thing allocated
	 * by the program, anq_argv_parse will automatically
	 * exit if a error happens. In the future if something
	 * needs to be allocated before the argv handler we'll
	 * need to change that. */
	parse(argc, argv);

	err = validate_data(&dt);
	if(err)
		goto no_param;

	err = start_ops(&op, &dt);

	return 0;

no_param:
	print_err_str(err);
	exit(err);
}
