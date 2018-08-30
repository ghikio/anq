/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "anq_ops.h"

#ifdef ANQ_CRYPTO_LIB_GPGME
	#include "gpgme/crypto.h"
#endif

#include <assert.h>

void init_ops(struct anq_ops *op)
{
#ifdef ANQ_CRYPTO_LIB_GPGME
	op->crypto_lib = ANQ_CRYPTO_LIB_GPGME;
	op->init       = &anq_gpgme_init;
	op->exit       = &anq_gpgme_exit;
	op->encrypt    = &anq_gpgme_encrypt;
	op->decrypt    = &anq_gpgme_decrypt;
#endif
	assert(op->crypto_lib != 0);
}
