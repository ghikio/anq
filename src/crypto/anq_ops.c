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

struct anq_ops c_ops;

void anq_init_ops()
{
#ifdef ANQ_CRYPTO_LIB_GPGME
	c_ops = (struct anq_ops) {
		.crypto_lib = ANQ_CRYPTO_LIB_GPGME,
		.init       = &anq_gpgme_init,
		.exit	    = &anq_gpgme_exit,
		.encrypt    = &anq_gpgme_encrypt,
		.decrypt    = &anq_gpgme_decrypt,
	};
#endif
	assert(c_ops.crypto_lib != 0);
}
