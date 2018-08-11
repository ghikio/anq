/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "err_codes.h"
#include "argv_parser.h"
#include "argv_handler.h"
#include "default_arguments.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void main_init(int argc, char *argv[]);
void main_exit();

void decrypt_function(char *arga, char *argb)
{
	printf("testing function %s=%s\n", arga, argb);
}

void encrypt_function(char *arga, char *argb)
{
	printf("testing function %s=%s\n", arga, argb);
}

void main_init(int argc, char *argv[])
{
	int  err;

	err = argv_init();
	if(err)
		goto init_err;

	argv_add_parameter("-h", false, anq_help); 
	argv_add_parameter("-d", true, decrypt_function);
	argv_add_parameter("-e", true, encrypt_function);

	err = argv_parse(argc, argv);

	if(err == ANQ_ERR_UNALLOCATED_MEMORY ||
	   err == ANQ_ERR_NO_DELIMITER)
		goto pars_err;

	return;

pars_err:
	main_exit();
init_err:
	print_err(err);
	exit(err);
}

void main_exit()
{
	argv_exit();
}

int main(int argc, char *argv[])
{
	main_init(argc, argv);

	// not yet <3

	main_exit();
}
