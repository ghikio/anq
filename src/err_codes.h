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

	/* Given parameter needs a value and it hasn't. */
	ANQ_ERR_NO_DELIMITER = -2,
	
	/* Returned if a malloc() or similar hasn't been able to
	 * allocate memory */
	ANQ_ERR_UNALLOCATED_MEMORY = -100,

	// Returned for not implemented features.
	ANQ_ERR_NOT_IMPLEMENTED = 127,
};

void  print_err(int err);
char *err_to_mess(int err);

#endif
