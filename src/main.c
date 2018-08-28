/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "help_menu.h"
#include "err_codes.h"
#include "argv_parser.h"
#include "argv_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void main_init(int argc, char *argv[]);
void main_exit();

int decrypt_function(char *arga, char *argb)
{
	if(argb[0] == '\0') 
		return ANQ_ERR_NO_DELIMITER;

	printf("testing function %s=%s\n", arga, argb);
	return 0;
}

int encrypt_function(char *arga, char *argb)
{
	if(argb[0] == '\0')
		return ANQ_ERR_NO_DELIMITER;

	printf("testing function %s=%s\n", arga, argb);
	return 0;
}

void main_init(int argc, char *argv[])
{
	int err;

	err = anq_argv_add_parameter("-h", anq_help); 
	if(err)
		goto no_param;
	err = anq_argv_add_parameter("-d", decrypt_function);
	if(err)
		goto no_param;
	err = anq_argv_add_parameter("-e", encrypt_function);
	if(err)
		goto no_param;

	/* Since the argv handler is the first thing allocated
	 * by the program, anq_argv_parse will automatically
	 * exit if a error happens. In the future if something
	 * needs to be allocated before the argv handler we'll
	 * need to change that. */
	anq_argv_parse(argc, argv);
	return;

no_param:
	exit(err);
}

void main_exit()
{
	return;
}

int main(int argc, char *argv[])
{
	main_init(argc, argv);

	// not yet <3

	main_exit();
}
