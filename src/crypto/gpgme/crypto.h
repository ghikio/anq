/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_CRYPTO_GPGME_CRYPTO_H
#define ANQ_CRYPTO_GPGME_CRYPTO_H

#include "anq_data.h"

int anq_gpgme_init(struct crypto_data *dt);
int anq_gpgme_exit(struct crypto_data *dt);

int anq_gpgme_encrypt(struct crypto_data *dt);
int anq_gpgme_decrypt(struct crypto_data *dt);

#endif
