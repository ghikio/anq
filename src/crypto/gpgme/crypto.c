/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "crypto.h"
#include "anq_data.h"
#include "err_codes.h"

#include <gpgme.h>
#include <locale.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define GPGME_REQUIRED_VERSION "1.11.1"

gpgme_ctx_t	    ctx;
gpgme_engine_info_t eng;
gpgme_key_t	    keys[2] = { NULL, NULL };

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

	if((err = gpgme_engine_check_version(GPGME_PROTOCOL_OpenPGP)) != GPG_ERR_NO_ERROR)
		// TODO [criw hp] implement engine check error
		exit(ANQ_ERR_NOT_IMPLEMENTED);

	if((err = gpgme_get_engine_info(&eng)) 
			!= GPG_ERR_NO_ERROR)
		// TODO [criw hp] implement get engine info err
		exit(ANQ_ERR_NOT_IMPLEMENTED);

	if((err = gpgme_new(&ctx)) != GPG_ERR_NO_ERROR)
		// TODO [criw hp] implement new context err
		exit(ANQ_ERR_NOT_IMPLEMENTED);

	if((err = gpgme_set_protocol(ctx, GPGME_PROTOCOL_OpenPGP))
			!= GPG_ERR_NO_ERROR)
		// TODO [criw hp] implement set protocol err
		exit(ANQ_ERR_NOT_IMPLEMENTED);

	if((err = gpgme_ctx_set_engine_info(ctx, GPGME_PROTOCOL_OpenPGP,
					    eng->file_name, eng->home_dir))
			!= GPG_ERR_NO_ERROR)
		// TODO [criw hp] implement set engine info err
		exit(ANQ_ERR_NOT_IMPLEMENTED);

	anq_gpgme_setkey(dt);
	return 0;
}

int anq_gpgme_exit()
{
	gpgme_release(ctx);
	return 0;
}

int anq_gpgme_setkey(struct crypto_data *dt)
{
	char *query = crypto_get_keyquery(dt);

	assert(ctx   != NULL);
	assert(query != NULL);

	gpgme_error_t err = 0;

	err = gpgme_op_keylist_start(ctx, query, 0);

	if((err = gpgme_op_keylist_next(ctx, &keys[0]))
			!= GPG_ERR_NO_ERROR) {
		if(!keys[0]->uids || !keys[0]->uids->name)
			goto not_found;
	}

	return 0;

not_found:
	// TODO [criw hp] implement key not found
	exit(ANQ_ERR_NOT_IMPLEMENTED);
}

int anq_gpgme_write(struct crypto_data *dt, gpgme_data_t str)
{
	gpgme_error_t err = 0;
	char *passd = crypto_get_passdir(dt);
	char *src   = crypto_get_service(dt);
	char *file  = calloc(strlen(passd) + strlen(src) + 1,
			     sizeof(char));
	if(!file)
		goto no_file;

	strcpy(file, passd);
	strcat(file, "/");
	strcat(file, src);

	FILE *fp;
	fp = fopen(file, "w");
	if(!fp)
		goto no_file;

	char buffer[CYPHER_SIZE];

	err = gpgme_data_seek(str, 0, SEEK_SET);
	if(err)
		// TODO [criw hp] implement
		goto no_file;

	while((err = gpgme_data_read(str, buffer, CYPHER_SIZE))
			> 0)
		fwrite(buffer, err, 1, fp);

	fclose(fp);
	free(file);

	return 0;

no_file:
	// TODO [criw hp] implement
	exit(ANQ_ERR_NOT_IMPLEMENTED);
}

int anq_gpgme_encrypt(struct crypto_data *dt)
{
	char *plain = crypto_get_plain(dt);

	assert(ctx != NULL);
	assert(plain != NULL);
	assert(keys[0] != NULL && keys[1] == NULL);

	gpgme_error_t err = 0;
	gpgme_data_t  src;
	gpgme_data_t  dst;

	/* gpgme_data_t stores and delivers the kind of data
	 * that must be exchanged between the user and the
	 * crypto engine. */
	if((err = gpgme_data_new_from_mem(&src, plain,
					  strlen(plain), 1))
			!= GPG_ERR_NO_ERROR)
		// TODO [criw hp] implement set data error
		exit(ANQ_ERR_NOT_IMPLEMENTED);

	if((err = gpgme_data_new(&dst)) != GPG_ERR_NO_ERROR)
		// TODO [criw hp] implement set data error
		exit(ANQ_ERR_NOT_IMPLEMENTED);

	if((err = gpgme_op_encrypt(ctx, keys,
				   GPGME_ENCRYPT_NO_ENCRYPT_TO,
				   src, dst))
			!= GPG_ERR_NO_ERROR)
		// TODO [criw hp] implement encrypt err
		exit(ANQ_ERR_NOT_IMPLEMENTED);

	gpgme_data_set_encoding(dst, GPGME_DATA_ENCODING_BINARY);

	anq_gpgme_write(dt, dst);

	gpgme_data_release(src);
	gpgme_data_release(dst);

	return 0;
}

int anq_gpgme_decrypt(struct crypto_data *dt)
{
	assert(ctx != NULL);

	gpgme_error_t err = 0;
	char *passd = crypto_get_passdir(dt);
	char *srv   = crypto_get_service(dt);
	char *file  = calloc(strlen(passd) + strlen(srv) + 1,
			     sizeof(char));
	if(!file)
		goto no_file;

	strcpy(file, passd);
	strcat(file, "/");
	strcat(file, srv);

	gpgme_data_t src;
	gpgme_data_t dst;

	if((err = gpgme_data_new_from_file(&src, file, 1))
			!= GPG_ERR_NO_ERROR)
		// TODO [criw hp] implement set data error 
		exit(ANQ_ERR_NOT_IMPLEMENTED);

	if((err = gpgme_data_new(&dst)) != GPG_ERR_NO_ERROR)
		// TODO [criw hp] implement set data error 
		exit(ANQ_ERR_NOT_IMPLEMENTED);

	if((err = gpgme_op_decrypt(ctx, src, dst)) 
			!= GPG_ERR_NO_ERROR)
		// TODO [criw hp] implement decrypt error
		exit(ANQ_ERR_NOT_IMPLEMENTED);


	char buffer[CYPHER_SIZE];

	err = gpgme_data_seek(dst, 0, SEEK_SET);
	if(err)
		// TODO [criw hp] Implement seek error
		exit(1);

	while((err = gpgme_data_read(dst, buffer, CYPHER_SIZE)) 
			> 0)
		fwrite(buffer, err, 1, stdout);

	gpgme_data_release(src);
	gpgme_data_release(dst);

	free(file);

	return 0;

no_file:
	// TODO [criw hp] implement
	exit(ANQ_ERR_NOT_IMPLEMENTED);
}
