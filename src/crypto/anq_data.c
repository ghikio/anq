/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "anq_data.h"
#include "err_codes.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

enum anq_op anq_get_operation(struct anq_data *dt)
{
	assert(dt->op != 0);
	return dt->op;
}

void anq_set_operation(struct anq_data *dt, enum anq_op op)
{
	/* Based on the current anq_op, change if new
	 * options are added. */
	assert(op % 2 == 0 && op >= 2);
	dt->op = op;
}

char *anq_get_keyquery(struct anq_data *dt)
{
	return dt->keyquery;
}

void anq_set_keyquery(struct anq_data *dt, char *query)
{
	strncpy(dt->keyquery, query, ARGV_READ_SIZE);
}

char *anq_get_service(struct anq_data *dt)
{
	return dt->service;
}

void anq_set_service(struct anq_data *dt, char *service)
{
	strncpy(dt->service, service, ARGV_READ_SIZE);
}

void ask_plain_password(struct anq_data *dt)
{
	static struct termios oterm;
	static struct termios nterm;

	tcgetattr(STDIN_FILENO, &oterm);

	nterm = oterm;
	nterm.c_lflag &= ~(ECHO);

	tcsetattr(STDIN_FILENO, TCSANOW, &nterm);

	if(fgets(dt->plain, ARGV_READ_SIZE, stdin) == NULL)
		dt->plain[0] = '\0';
	else
		dt->plain[strlen(dt->plain) - 1] = '\0';

	// go back to the old settings
	tcsetattr(STDIN_FILENO, TCSANOW, &oterm);
}

int validate_data(struct anq_data *dt)
{
	if(dt->service[0] == '\0')
		return ANQ_ERR_NO_SERVICE;

	char *keyquery = getenv("ANQ_KEYQUERY");
	if(keyquery == NULL)
		return ANQ_ERR_NO_KEYQUERY;
	anq_set_keyquery(dt, keyquery);

	ask_plain_password(dt);

	return 0;
}

