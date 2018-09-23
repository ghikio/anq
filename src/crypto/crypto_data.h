/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_CRYPTO_ANQDATA_H
#define ANQ_CRYPTO_ANQDATA_H

#define INPUT_SIZE  124
#define CYPHER_SIZE 1024

/* Represent the way the program will act based
 * on user input. */
enum op {
	ANQ_OP_LIST    = 2,
	ANQ_OP_ENCRYPT = 4,
	ANQ_OP_DECRYPT = 8,
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
 * plain  - Stores the plain password. */
struct crypto_data {
	enum op op;
	char svc[INPUT_SIZE];
	char passd[INPUT_SIZE];
	char key[INPUT_SIZE];
	char plain[INPUT_SIZE];
};

enum op crypto_get_operation(struct crypto_data *dt);
void    crypto_set_operation(struct crypto_data *dt, 
			     enum op op);

char *crypto_get_keyquery(struct crypto_data *dt);
void  crypto_set_keyquery(struct crypto_data *dt, char *query);

char *crypto_get_service(struct crypto_data *dt);
void  crypto_set_service(struct crypto_data *dt, char *svc);

char *crypto_get_passdir(struct crypto_data *dt);
void  crypto_set_passdir(struct crypto_data *dt, char *passd);

char *crypto_get_plain(struct crypto_data *dt);
void  crypto_set_plain(struct crypto_data *dt, char *plain);

void ask_plain_password(struct crypto_data *dt);
int  crypto_validate_data(struct crypto_data *dt);

#endif
