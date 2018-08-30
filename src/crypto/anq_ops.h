/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_CRYPTO_ANQOPS_H
#define ANQ_CRYPTO_ANQOPS_H

#include "anq_data.h"

/* By now the only implemented crypto lib will be gpgme, but
 * if in the future a new one is implemented, it would be nice
 * to switch between implementations with this. Details in 
 * anq_init_ops(). */
#define ANQ_CRYPTO_LIB_GPGME 1

struct anq_ops {
	int crypto_lib;

	int (*init)    (struct anq_data *);
	int (*exit)    (struct anq_data *);
	int (*encrypt) (struct anq_data *);
	int (*decrypt) (struct anq_data *);
};

void init_ops(struct anq_ops *op);

#endif
