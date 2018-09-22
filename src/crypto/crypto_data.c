/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "io_utils.h"
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
	strncpy(dt->key, query, INPUT_SIZE);
}

char *crypto_get_service(struct crypto_data *dt)
{
	return dt->svc;
}

void crypto_set_service(struct crypto_data *dt, char *svc)
{
	assert(svc != NULL);
	strncpy(dt->svc, svc, INPUT_SIZE);
}

char *crypto_get_passdir(struct crypto_data *dt)
{
	return dt->passd;
}

void crypto_set_passdir(struct crypto_data *dt, char *passd)
{
	strncpy(dt->passd, passd, INPUT_SIZE);
}

char *crypto_get_plain(struct crypto_data *dt)
{
	return dt->plain;
}

void crypto_set_plain(struct crypto_data *dt, char *plain)
{
	strncpy(dt->plain, plain, INPUT_SIZE);
}

void ask_plain_password(struct crypto_data *dt)
{
	static struct termios oterm;
	static struct termios nterm;

	tcgetattr(STDIN_FILENO, &oterm);

	nterm = oterm;
	/* Disable ECHO (displaying input) for the new term
	 * and set nterm as the actual terminal. */
	nterm.c_lflag &= ~(ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &nterm);

	printf("Introduce password: ");
	if(fgets(dt->plain, INPUT_SIZE, stdin) == NULL)
		dt->plain[0] = '\0';

	// Goes back to the old terminal.
	tcsetattr(STDIN_FILENO, TCSANOW, &oterm);
}

int crypto_validate_data(struct crypto_data *dt)
{
	int err;

	if(dt->svc[0] == '\0')
		return ANQ_ERR_NO_SERVICE;

	char *kq = crypto_get_keyquery(dt);
	if(kq == NULL)
		return ANQ_ERR_NO_KEYQUERY;

	char *pd = crypto_get_passdir(dt);
	if(pd == NULL || !check_dir_exists(pd))
		return ANQ_ERR_NO_PASSDIR;

	char *file = make_filename(pd, dt->svc);

	switch(dt->op) {
	case ANQ_OP_ENCRYPT:
		ask_plain_password(dt);

		if(dt->plain[0] == '\0') {
			err = ANQ_ERR_NO_PASSWORD;
			goto err;
		}
		break;
	case ANQ_OP_DECRYPT:
		if(!check_file_access(file, F_OK)) {
			err = ANQ_ERR_DECRYPT_NO_SERVICE;
			goto err;
		}
		break;
	}

	free(file);
	return 0;

err:
	free(file);
	return err;
}

