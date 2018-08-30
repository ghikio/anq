/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "help_menu.h"

#include <stdio.h>

int anq_help(char *arg, char *value)
{
	printf("%s - %s\n%s\n\n%s\n", NAME, VERSION, AUTHOR, LICENSE);
	printf("-h - print help menu\n");
	printf("-d - decrypt password\n");
	printf("-e - encrypt password\n");
	printf("-s - password's service name\n");

	return 0;
}
