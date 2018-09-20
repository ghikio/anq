/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_ERR_CODES_HH
#define ANQ_ERR_CODES_HH

#define ANQ_ERR_SIZE 255

enum anq_err_codes {
	/* Returned if a malloc() or similar hasn't been able to
	 * allocate memory */
	ANQ_ERR_UNALLOCATED_MEMORY	= 1,
	/* No operation parameter has been found. */
	ANQ_ERR_NO_OPERATION		= 100,
	/* Parameter -s required and not found. */
	ANQ_ERR_NO_SERVICE		= 101,
	/* Parameter -s requires a value and hasn't one. */
	ANQ_ERR_NO_SERVICE_VALUE	= 102,
	/* conf variable ANQ_KEY_NAME required and not found. */
	ANQ_ERR_NO_KEYQUERY		= 103,
	/* A null password was inserted when asking. */
	ANQ_ERR_NO_PASSWORD		= 104,
	/* conf variable ANQ_PASSPATH required and not found. */
	ANQ_ERR_NO_PASSDIR		= 105,
	/* configuration file couldn't be correctly parsed. */
	ANQ_ERR_INVALID_CONF		= 106,
	/* Denote the execution of the help menu. */
	ANQ_ERR_HELP_MENU		= 107,
	/* Returned when the service file isn't in
	 * the ANQ_PASSPATH directory. */
	ANQ_ERR_DECRYPT_NO_SERVICE	= 108,
	// Returned for not implemented features.
	ANQ_ERR_NOT_IMPLEMENTED		= 127,

	// GPGME ERRORS

	// Returned when gpgme can't find the opengpg backend.
	ANQ_ERR_NO_OPENGPG_PROTOCOL	= 200,
	// Returned when the supplied name for the key has not been found.
	ANQ_ERR_CRYPTO_KEY_NOT_FOUND	= 201,
	// Returned if the gpg key (ANQ_KEY_NAME) is invalid.
	ANQ_ERR_CRYPTO_KEY_UNUSABLE	= 202,
	/* Returned when the encrypt function couldn't 
	 * write to a file correctly. */
	ANQ_ERR_CRYPTO_ENCRYPT_WRITE	= 203,
	// Returned if the crypto encoding fails.
	ANQ_ERR_CRYPTO_ENCRYPT_ERR	= 204,
	// Returned if the crypto fails fails.
	ANQ_ERR_CRYPTO_DECRYPT_ERR	= 205,
};

char *err_to_str(int err);
void  print_err_str(int err);

#endif
