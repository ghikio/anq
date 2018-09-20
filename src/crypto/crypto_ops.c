/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "crypto_ops.h"
#include "crypto_data.h"

#ifdef ANQ_CRYPTO_LIB_GPGME
	#include "gpgme/crypto.h"
#endif

#include <assert.h>

void init_ops(struct crypto_ops *op)
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

int start_ops(struct crypto_ops *op, struct crypto_data *dt)
{
	int err;
	if(op->crypto_lib == 0)
		init_ops(op);
	err = op->init(dt);

	switch(dt->op) {
	case ANQ_OP_ENCRYPT:
		err = op->encrypt(dt);
		if(err)
			goto enc_err;
		break;
	case ANQ_OP_DECRYPT:
		err = op->decrypt(dt);
		break;
	default:
		break;
	}

	err = op->exit(dt);

	return 0;

enc_err:
	return err;
}
