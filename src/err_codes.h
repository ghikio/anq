/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_ERR_CODES_HH
#define ANQ_ERR_CODES_HH

#define ANQ_OK 0

typedef int ANQ_ERR;

enum s_exit_err_codes {
	/* command line argument -e or -d has been parsed
	 * without giving the subsequent service filename. */
	ANQ_ERR_UNKNOWN_SERVICE = -1,
	/* argument handler hasn't been able to start */
	ANQ_ERR_ARGV_HANDLER	= -2,
	ANQ_ERR_NEED_DELIMITER  = -3, 
};

enum s_return_err_codes {
	/* Returned if a malloc() or similar hasn't been able to
	 * allocate memory */
	ANQ_ERR_UNALLOCATED_STRUCT = -100,
	/* Returned when s_argv_handler.argc "argv_handler.h" has
	 * exceeded ARGV_SIZE "argv_handler.h" */
	ANQ_ERR_MAX_ARGS_EXCEEDED = -101,
	ANQ_ERR_NO_DELIMITER = -102,
};

#endif
