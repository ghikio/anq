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
	return dt->key;
}

void anq_set_keyquery(struct anq_data *dt, char *query)
{
	assert(query != NULL);
	strncpy(dt->key, query, ARGV_READ_SIZE);
}

char *anq_get_service(struct anq_data *dt)
{
	return dt->svc;
}

void anq_set_service(struct anq_data *dt, char *svc)
{
	assert(svc != NULL);
	strncpy(dt->svc, svc, ARGV_READ_SIZE);
}

char *anq_get_passdir(struct anq_data *dt)
{
	return dt->passd;
}

void anq_set_passdir(struct anq_data *dt, char *passd)
{
	strncpy(dt->passd, passd, ARGV_READ_SIZE);
}

char *anq_get_plain(struct anq_data *dt)
{
	return dt->plain;
}

void anq_set_plain(struct anq_data *dt, char *plain)
{
	strncpy(dt->plain, plain, ARGV_READ_SIZE);
}

char *anq_get_cypher(struct anq_data *dt)
{
	return dt->cypher;
}

void anq_set_cypher(struct anq_data *dt, char *cypher)
{
	strncpy(dt->cypher, cypher, CYPHER_SIZE);
}

// TODO [criw mp] comment this up a little
void ask_plain_password(struct anq_data *dt)
{
	static struct termios oterm;
	static struct termios nterm;

	tcgetattr(STDIN_FILENO, &oterm);

	nterm = oterm;
	nterm.c_lflag &= ~(ECHO);

	tcsetattr(STDIN_FILENO, TCSANOW, &nterm);

	//printf("Introduce password: ");
	if(fgets(dt->plain, ARGV_READ_SIZE, stdin) == NULL)
		dt->plain[0] = '\0';

	tcsetattr(STDIN_FILENO, TCSANOW, &oterm);
}

int validate_data(struct anq_data *dt)
{
	if(dt->svc[0] == '\0')
		return ANQ_ERR_NO_SERVICE;

	char *key = getenv("ANQ_KEYQUERY");
	if(key == NULL)
		return ANQ_ERR_NO_KEYQUERY;
	anq_set_keyquery(dt, key);

	char *passd = getenv("ANQ_PASSD");
	anq_set_passdir(dt, passd);

	if(dt->op == ANQ_OP_ENCRYPT) {
		ask_plain_password(dt);

		if(dt->plain[0] == '\0')
			return ANQ_ERR_NO_PASSWORD;
	}

	return 0;
}

