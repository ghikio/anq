/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "err_codes.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_err_str(int err)
{
	char *str = err_to_str(err);
	fputs(str, stderr);

	free(str);
}

char *err_to_str(int err)
{
	char *str = calloc(ANQ_ERR_SIZE, sizeof(char));
	if(!str)
		return NULL;

	switch(err) {
	case ANQ_ERR_NO_DELIMITER:
		strncpy(str, "ERR: One of the parameters need a value.\n",
			ANQ_ERR_SIZE);
		break;
	case ANQ_ERR_UNALLOCATED_MEMORY:
		strncpy(str, "ERR: Memory for internal component couldn't be allocated.\n",
			ANQ_ERR_SIZE);
		break;
	case ANQ_ERR_NO_OPERATION:
		strncpy(str, "ERR: Operation parameter required.\n", ANQ_ERR_SIZE);
		break;
	case ANQ_ERR_NO_SERVICE:
		strncpy(str, "ERR: Service parameter required.\n", ANQ_ERR_SIZE);
		break;
	case ANQ_ERR_NO_KEYQUERY:
		strncpy(str, "ERR: ANQ_KEYQUERY environment variable not found.\n",
			ANQ_ERR_SIZE);
		break;
	default:
		str[0] = '\0';
		break;
	}

	return str;
}
