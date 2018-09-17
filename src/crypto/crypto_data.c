/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "err_codes.h"
#include "crypto_data.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

enum op crypto_get_operation(struct crypto_data *dt)
{
	assert(dt->op != 0);
	return dt->op;
}

void crypto_set_operation(struct crypto_data *dt, enum op op)
{
	/* Based on the current anq_op, change if new
	 * options are added. */
	assert(op % 2 == 0 && op >= 2);
	dt->op = op;
}

char *crypto_get_keyquery(struct crypto_data *dt)
{
	return dt->key;
}

void crypto_set_keyquery(struct crypto_data *dt, char *query)
{
	assert(query != NULL);
	strncpy(dt->key, query, ARGV_READ_SIZE);
}

char *crypto_get_service(struct crypto_data *dt)
{
	return dt->svc;
}

void crypto_set_service(struct crypto_data *dt, char *svc)
{
	assert(svc != NULL);
	strncpy(dt->svc, svc, ARGV_READ_SIZE);
}

char *crypto_get_passdir(struct crypto_data *dt)
{
	return dt->passd;
}

void crypto_set_passdir(struct crypto_data *dt, char *passd)
{
	strncpy(dt->passd, passd, ARGV_READ_SIZE);
}

char *crypto_get_plain(struct crypto_data *dt)
{
	return dt->plain;
}

void crypto_set_plain(struct crypto_data *dt, char *plain)
{
	strncpy(dt->plain, plain, ARGV_READ_SIZE);
}

// TODO [criw mp] comment this up a little
void ask_plain_password(struct crypto_data *dt)
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

int crypto_validate_data(struct crypto_data *dt)
{
	if(dt->svc[0] == '\0')
		return ANQ_ERR_NO_SERVICE;

	char *kq = crypto_get_keyquery(dt);
	if(kq == NULL)
		return ANQ_ERR_NO_KEYQUERY;

	char *pd = crypto_get_passdir(dt);
	if(pd == NULL)
		return ANQ_ERR_NO_PASSDIR;

	if(dt->op == ANQ_OP_ENCRYPT) {
		ask_plain_password(dt);

		if(dt->plain[0] == '\0')
			return ANQ_ERR_NO_PASSWORD;
	}

	return 0;
}

