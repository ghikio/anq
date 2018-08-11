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

void print_err(ANQ_ERR err)
{
	char *mess = malloc(ANQ_ERR_SIZE);
	if(!mess)
		exit(ANQ_ERR_UNALLOCATED_MEMORY);

	mess = err_to_mess(err);
	fputs(mess, stderr);

	free(mess);
}

char *err_to_mess(ANQ_ERR err)
{
	char *mess = malloc(ANQ_ERR_SIZE);

	switch(err) {
	case ANQ_ERR_NO_DELIMITER:
		strncpy(mess, "ERR: One of the parameters need a value.\n",
			ANQ_ERR_SIZE);
		break;
	case ANQ_ERR_UNALLOCATED_MEMORY:
		strncpy(mess, "ERR: Memory for internal component couldn't be allocated.\n",
			ANQ_ERR_SIZE);
		break;
	default:
		mess[0] = '\0';
		break;
	}

	return mess;
}
