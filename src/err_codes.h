/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_ERR_CODES_HH
#define ANQ_ERR_CODES_HH

#define ANQ_ERR_SIZE 124

enum anq_err_codes {
	ANQ_OK = 0,

	/* Returned if a malloc() or similar hasn't been able to
	 * allocate memory */
	ANQ_ERR_UNALLOCATED_MEMORY	= 1,
	/* No operation parameter has been found. */
	ANQ_ERR_NO_OPERATION		= 100,
	/* Parameter -s required and not found. */
	ANQ_ERR_NO_SERVICE		= 101,
	/* Parameter -s requires a value and hasn't one. */
	ANQ_ERR_NO_SERVICE_VALUE	= 102,
	/* env variable ANQ_KEYQUERY required and not found. */
	ANQ_ERR_NO_KEYQUERY		= 103,
	/* A null password was inserted when asking. */
	ANQ_ERR_NO_PASSWORD		= 104,
	/* env variable ANQ_PASSDIR required and not found. */
	ANQ_ERR_NO_PASSDIR		= 105,
	// Returned for not implemented features.
	ANQ_ERR_NOT_IMPLEMENTED		= 127,
};

char *err_to_str(int err);
void  print_err_str(int err);

#endif
