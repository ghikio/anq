/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_CRYPTO_ANQDATA_H
#define ANQ_CRYPTO_ANQDATA_H

#include "argv_handler.h"

/* Represent the way the program will act based
 * on user input. */
enum anq_op {
	ANQ_OP_ENCRYPT = 2,
	ANQ_OP_DECRYPT = 4,
};

struct anq_data {
	enum anq_op op;
	char service[ARGV_READ_SIZE];
	char keyquery[ARGV_READ_SIZE];
	char plain[ARGV_READ_SIZE];
};

enum anq_op anq_get_operation(struct anq_data *dt);
void	    anq_set_operation(struct anq_data *dt, 
			      enum anq_op op);

char *anq_get_keyquery(struct anq_data *dt);
void  anq_set_keyquery(struct anq_data *dt, char *query);

char *anq_get_service(struct anq_data *dt);
void  anq_set_service(struct anq_data *dt, char *service);

void ask_plain_password(struct anq_data *dt);
int  validate_data(struct anq_data *dt);

#endif
