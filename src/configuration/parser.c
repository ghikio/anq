/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "parser.h"
#include "err_codes.h"
#include "crypto_ops.h"
#include "crypto_data.h"
#include "argv_parser.h"
#include "config_parser.h"

#include <stdio.h>
#include <stdlib.h>

extern struct crypto_data dt;
extern struct crypto_ops  op;

void parse(int argc, char *argv[])
{
	int err;

	err = parse_argv(argc, argv);
	if(err)
		goto parse_err;

	parse_config();

	return;

parse_err:
	print_err_str(err);
	exit(err);
}
