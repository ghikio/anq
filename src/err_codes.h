/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_ERR_CODES_HH
#define ANQ_ERR_CODES_HH

#define ANQ_ERR_SIZE 124

typedef int ANQ_ERR;

enum anq_err_codes {
	ANQ_OK = 0,

	/* Given parameter needs a value and it hasn't. */
	ANQ_ERR_NO_DELIMITER = -2,
	
	/* Returned if a malloc() or similar hasn't been able to
	 * allocate memory */
	ANQ_ERR_UNALLOCATED_MEMORY = -100,
};

void  print_err(ANQ_ERR err);
char *err_to_mess(ANQ_ERR err);

#endif
