/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "constants.h"
#include "default_arguments.h"

#include <stdio.h>

void anq_help(char *arg, char *value)
{
	printf("%s - %s\n%s\n\n%s\n\n", NAME, VERSION, AUTHOR, LICENSE);
	printf("-h\tprint help menu\n");
	printf("-d\tdecrypt password\n");
	printf("-e\tencrypt password\n");
}
