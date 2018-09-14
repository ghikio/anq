/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "parser.h"
#include "err_codes.h"
#include "argv_parser.h"
#include "configuration_parser.h"

#include <stdlib.h>

void parse(int argc, char *argv[])
{
	int err;

	err = parse_argv(argc, argv);
	if(err)
		goto print_err;

	//parse_configuration();

	return;

print_err:
	print_err_str(err);
	exit(err);
}
