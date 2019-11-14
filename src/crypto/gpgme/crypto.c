/**
 * @file
 * Provides the gpgme implementation for crypto_ops 
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

#include "crypto.h"
#include "io_utils.h"
#include "err_codes.h"
#include "crypto_data.h"

#include <gpgme.h>
#include <locale.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define GPGME_REQUIRED_VERSION "1.11.1"

extern void die(int err);

gpgme_ctx_t anq_crypto_gpgme_ctx;
gpgme_engine_info_t anq_crypto_gpgme_eng;
gpgme_key_t keys[2] = { NULL, NULL };

int anq_gpgme_setkey(struct crypto_data *dt);

int anq_gpgme_init(struct crypto_data *dt)
{
	gpgme_error_t err = 0;

	setlocale(LC_ALL, "");
	const char *vrs = 
		gpgme_check_version(GPGME_REQUIRED_VERSION);
	/* Fail either if the gpgme library version isn't equal
	 * or bigger than the one specified, or it's a invalid
	 * version. */
	assert(vrs != NULL);

	gpgme_set_locale(NULL, LC_CTYPE, setlocale(LC_CTYPE, NULL));
	/* Set LC_MESSAGES only if HAVE_W32_SYSTEM, the check is
	 * done internally by gpgme. */
#ifdef LC_MESSAGES
	gpgme_set_locale(NULL, LC_MESSAGES, 
			 setlocale(LC_MESSAGES, NULL));
#endif

	/* To have access to crypto operations, we need to define a
	 * cryptographic protocol. GPGME implements the backend for us
	 * so we'll just check if it's installed and accessible. */
	if((err = gpgme_engine_check_version(GPGME_PROTOCOL_OpenPGP)) 
			!= GPG_ERR_NO_ERROR)
		return ANQ_ERR_NO_OPENGPG_PROTOCOL;

	if((err = gpgme_get_engine_info(&anq_crypto_gpgme_eng)) 
			!= GPG_ERR_NO_ERROR)
		die(ANQ_ERR_UNALLOCATED_MEMORY);

	if((err = gpgme_new(&anq_crypto_gpgme_ctx)) != GPG_ERR_NO_ERROR) {
		assert(err != GPG_ERR_INV_VALUE);
		assert(err != GPG_ERR_NOT_OPERATIONAL);
		assert(err != GPG_ERR_SELFTEST_FAILED);

		die(ANQ_ERR_UNALLOCATED_MEMORY);
	}

	if((err = gpgme_set_protocol(anq_crypto_gpgme_ctx, GPGME_PROTOCOL_OpenPGP))
			!= GPG_ERR_NO_ERROR) {
		assert(err != GPG_ERR_INV_VALUE);
		die(ANQ_ERR_UNALLOCATED_MEMORY);
	}

	if((err = gpgme_ctx_set_engine_info(anq_crypto_gpgme_ctx, GPGME_PROTOCOL_OpenPGP,
					    anq_crypto_gpgme_eng->file_name, anq_crypto_gpgme_eng->home_dir))
			!= GPG_ERR_NO_ERROR)
		die(ANQ_ERR_UNALLOCATED_MEMORY);

	if((err = anq_gpgme_setkey(dt)) != 0)
		return err;

	return 0;
}

int anq_gpgme_exit(struct crypto_data *dt)
{
	gpgme_release(anq_crypto_gpgme_ctx);
	return 0;
}

int anq_gpgme_setkey(struct crypto_data *dt)
{
	char *query = crypto_get_keyquery(dt);

	assert(anq_crypto_gpgme_ctx != NULL);
	assert(query != NULL);

	gpgme_error_t err = 0;

	err = gpgme_op_keylist_start(anq_crypto_gpgme_ctx, query, 0);

	if((err = gpgme_op_keylist_next(anq_crypto_gpgme_ctx, &keys[0]))
			!= GPG_ERR_NO_ERROR) {
		if(!keys[0]->uids || !keys[0]->uids->name)
			goto not_found;
	}

	return 0;

not_found:
	return ANQ_ERR_CRYPTO_KEY_NOT_FOUND;
}

int anq_gpgme_write(struct crypto_data *dt, gpgme_data_t str)
{
	gpgme_error_t err = 0;

	char *passd = crypto_get_passdir(dt);
	char *src   = crypto_get_service(dt);
	char *file  = make_filename(passd, src);
	if(!file)
		goto mem_err;

	FILE *fp;
	fp = fopen(file, "wb");
	if(!fp)
		goto nfile_err;

	char buffer[CYPHER_SIZE];

	err = gpgme_data_seek(str, 0, SEEK_SET);
	if(err)
		goto seek_err;

	while((err = gpgme_data_read(str, buffer, CYPHER_SIZE))
			> 0)
		fwrite(buffer, err, 1, fp);

	fclose(fp);
	free(file);

	return 0;

seek_err:
	perror("crypto");
	fclose(fp);
nfile_err:
	free(file);
mem_err:
	return err;
}

/*
 * TODO [criw mp] Evaluate if program can die() safely in here
 *
 * Instead of doing err = ANQ_ERR_UNALLOCATED_MEMORY and goto,
 * change to die(ANQ_ERR_UNALLOCATED_MEMORY) if no security
 * issues are found.
 */
int anq_gpgme_encrypt(struct crypto_data *dt)
{
	char *plain = crypto_get_plain(dt);

	assert(anq_crypto_gpgme_ctx != NULL);
	assert(plain != NULL);
	assert(keys[0] != NULL && keys[1] == NULL);

	gpgme_error_t err = 0;
	gpgme_data_t  src;
	gpgme_data_t  dst;

	/* gpgme_data_t stores and delivers the kind of data
	 * that must be exchanged between the user and the
	 * crypto anq_crypto_gpgme_engine. */
	if((err = gpgme_data_new_from_mem(&src, plain,
					  strlen(plain), 1))
			!= GPG_ERR_NO_ERROR) {
		assert(err != GPG_ERR_INV_VALUE);

		err = ANQ_ERR_UNALLOCATED_MEMORY;
		goto src_mem_err;
	}

	if((err = gpgme_data_new(&dst)) != GPG_ERR_NO_ERROR) {
		assert(err != GPG_ERR_INV_VALUE);

		err = ANQ_ERR_UNALLOCATED_MEMORY;
		goto dst_mem_err;
	}

	if((err = gpgme_op_encrypt(anq_crypto_gpgme_ctx, keys,
				   GPGME_ENCRYPT_NO_ENCRYPT_TO,
				   src, dst))
			!= GPG_ERR_NO_ERROR) {
		assert(err != GPG_ERR_INV_VALUE);
		assert(err != GPG_ERR_BAD_PASSPHRASE);

		if(err == GPG_ERR_UNUSABLE_PUBKEY)
			err = ANQ_ERR_CRYPTO_KEY_UNUSABLE;
		else
			err = ANQ_ERR_CRYPTO_ENCRYPT_ERR;

		goto encrypt_err;
	}

	gpgme_data_set_encoding(dst, GPGME_DATA_ENCODING_BINARY);

	err = anq_gpgme_write(dt, dst);
	if(err)
		goto encrypt_err;

	gpgme_data_release(src);
	gpgme_data_release(dst);

	return 0;
encrypt_err:
	gpgme_data_release(dst);
dst_mem_err:
	gpgme_data_release(src);
src_mem_err:
	return err;
}

int anq_gpgme_decrypt(struct crypto_data *dt)
{
	assert(anq_crypto_gpgme_ctx != NULL);

	gpgme_error_t err = 0;
	char *passd = crypto_get_passdir(dt);
	char *srv   = crypto_get_service(dt);
	char *file  = make_filename(passd, srv);
	if(!file)
		goto nfile_err;

	gpgme_data_t src;
	gpgme_data_t dst;

	if((err = gpgme_data_new_from_file(&src, file, 1))
			!= GPG_ERR_NO_ERROR) {
		assert(err != GPG_ERR_INV_VALUE); 
		assert(err != GPG_ERR_NOT_IMPLEMENTED); 

		err = ANQ_ERR_UNALLOCATED_MEMORY;
		goto src_mem_err;
	}

	if((err = gpgme_data_new(&dst)) != GPG_ERR_NO_ERROR) {
		err = ANQ_ERR_UNALLOCATED_MEMORY;
		goto dst_mem_err;
	}

	if((err = gpgme_op_decrypt(anq_crypto_gpgme_ctx, src, dst)) 
			!= GPG_ERR_NO_ERROR) {
		assert(err != GPG_ERR_INV_VALUE);

		err = ANQ_ERR_CRYPTO_DECRYPT_ERR;
		goto decrypt_err;
	}

	char buffer[CYPHER_SIZE];

	err = gpgme_data_seek(dst, 0, SEEK_SET);
	if(err)
		goto seek_err;

	while((err = gpgme_data_read(dst, buffer, CYPHER_SIZE)) 
			> 0)
		fwrite(buffer, err, 1, stdout);

	gpgme_data_release(src);
	gpgme_data_release(dst);

	free(file);

	return 0;

seek_err:
	perror("crypto");
decrypt_err:
	gpgme_data_release(dst);
dst_mem_err:
	gpgme_data_release(src);
src_mem_err:
	free(file);
nfile_err:
	return err;
}
