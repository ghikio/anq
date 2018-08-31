/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_CRYPTO_ANQDATA_H
#define ANQ_CRYPTO_ANQDATA_H

#include "argv_handler.h"

#define CYPHER_SIZE 1024

/* Represent the way the program will act based
 * on user input. */
enum anq_op {
	ANQ_OP_ENCRYPT = 2,
	ANQ_OP_DECRYPT = 4,
};

/* Stores user input by arguments and environment vars plus
 * data manipulated by the program that needs to be shared. 
 *
 * op	  - Specifies the action that needs to be executed.
 * svc    - Specifies what's the password from.
 *		When encrypting, it's used as the file name. 
 *		When decrypting, it's used to find the file.
 * passd  - Specifies the directory where password files are
 *	    saved. This is defined by the ANQ_PASSDIR env var.
 * key    - Specifies the key name that will encrypt & decrypt.
 *	    This is defined by the ANQ_KEY env var.
 * plain  - Stores the plain password.
 * cypher - Stores the chyper password. */
struct anq_data {
	enum anq_op op;
	char svc[ARGV_READ_SIZE];
	char passd[ARGV_READ_SIZE];
	char key[ARGV_READ_SIZE];

	char plain[ARGV_READ_SIZE];
	char cypher[CYPHER_SIZE];
};

enum anq_op anq_get_operation(struct anq_data *dt);
void	    anq_set_operation(struct anq_data *dt, 
			      enum anq_op op);

char *anq_get_keyquery(struct anq_data *dt);
void  anq_set_keyquery(struct anq_data *dt, char *query);

char *anq_get_service(struct anq_data *dt);
void  anq_set_service(struct anq_data *dt, char *svc);

char *anq_get_passdir(struct anq_data *dt);
void  anq_set_passdir(struct anq_data *dt, char *passd);

char *anq_get_plain(struct anq_data *dt);
void  anq_set_plain(struct anq_data *dt, char *plain);

char *anq_get_cypher(struct anq_data *dt);
void  anq_set_cypher(struct anq_data *dt, char *chyper);

void ask_plain_password(struct anq_data *dt);
int  validate_data(struct anq_data *dt);

#endif
