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
	case ANQ_ERR_UNALLOCATED_MEMORY:
		strncpy(str, "ERR: Memory for internal component couldn't be allocated.\n",
			ANQ_ERR_SIZE);
		break;
	case ANQ_ERR_NO_OPERATION:
		strncpy(str, "ERR: Operation parameter required.\n",
			ANQ_ERR_SIZE);
		break;
	case ANQ_ERR_NO_SERVICE:
		strncpy(str, "ERR: Service parameter required.\n", 
			ANQ_ERR_SIZE);
		break;
	case ANQ_ERR_NO_SERVICE_VALUE:
		strncpy(str, "ERR: Service parameter needs a value and didn't found one.\n",
			ANQ_ERR_SIZE);
		break;
	case ANQ_ERR_NO_KEYQUERY:
		strncpy(str, "ERR: ANQ_KEY_NAME config variable not found, take a look at the configuration sample file.\n",
			ANQ_ERR_SIZE);
		break;
	case ANQ_ERR_NO_PASSWORD:
		strncpy(str, "ERR: A zero-length password was inserted.\n",
			ANQ_ERR_SIZE);
		break;
	case ANQ_ERR_NO_PASSDIR:
		strncpy(str, "ERR: ANQ_PASSPATH config variable is not set, take a look at the configuration sample file.\n",
			ANQ_ERR_SIZE);
		break;
	case ANQ_ERR_INVALID_CONF:
		strncpy(str, "ERR: Couldn't parse the configuration file correctly. Ensure the directory and the config file exist and the configuration inside is correct.\n",
			ANQ_ERR_SIZE);
	default:
		str[0] = '\0';
		break;
	}

	return str;
}
