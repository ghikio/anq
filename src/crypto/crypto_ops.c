/**
 * @file
 * Provides the list of required functionalities from the backend crypto library.
 *
 * @authors
 * Copyright (c) 2018-2019, Vitor Fernandes
 *
 * All rights reserved.
 *
 * @copyright
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "io_utils.h"
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
	case ANQ_OP_LIST:
		err = list_dir(dt->passd);
		if(err)
			goto err;
		break;
	case ANQ_OP_ENCRYPT:
		err = op->encrypt(dt);
		if(err)
			goto err;
		break;
	case ANQ_OP_DECRYPT:
		err = op->decrypt(dt);
		if(err)
			goto err;
		break;
	}

	err = op->exit(dt);

	return 0;

err:
	return err;
}
