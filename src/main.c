/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "parser.h"
#include "anq_ops.h"
#include "anq_data.h"
#include "help_menu.h"
#include "err_codes.h"
#include "argv_parser.h"
#include "argv_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct anq_data dt;
struct anq_ops  op;

void main_init(int argc, char *argv[]);
void main_exit();

int argv_decrypt(char *arga, char *argb)
{
	anq_set_operation(&dt, ANQ_OP_DECRYPT);
	return 0;
}

int argv_encrypt(char *arga, char *argb)
{
	anq_set_operation(&dt, ANQ_OP_ENCRYPT);
	return 0;
}

int argv_service(char *arga, char *argb)
{
	if(argb[0] == '\0')
		return ANQ_ERR_NO_SERVICE_VALUE;

	anq_set_service(&dt, argb);
	return 0;
}

int main(int argc, char *argv[])
{
	int err;

	err = anq_argv_add_parameter("-h", anq_help); 
	if(err)
		goto no_param;
	err = anq_argv_add_parameter("-d", argv_decrypt);
	if(err)
		goto no_param;
	err = anq_argv_add_parameter("-e", argv_encrypt);
	if(err)
		goto no_param;
	err = anq_argv_add_parameter("-s", argv_service);
	if(err)
		goto no_param;

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
