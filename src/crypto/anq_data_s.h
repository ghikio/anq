/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_CRYPTO_ANQCRYPTODATA_H
#define ANQ_CRYPTO_ANQCRYPTODATA_H

/* Represent the way the program will act based
 * on user input. */
enum anq_operation_t {
	ANQ_OP_ENCRYPT = 2,
	ANQ_OP_DECRYPT = 4,
};

struct anq_data_s {
	enum anq_operation_t op;
	/* keyquery CAN be null, in which case all the system keys
	 * will be searched. */
	char *keyquery;
};

enum anq_operation_t anq_get_operation(struct anq_data_s *ops);
void		     anq_set_operation(struct anq_data_s *ops, 
				       enum anq_operation_t op);

char *anq_get_keyquery(struct anq_data_s *ops);
void  anq_set_keyquery(struct anq_data_s *ops, char *query);

#endif
