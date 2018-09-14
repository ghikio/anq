/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_ARGV_HANDLER_H
#define ANQ_ARGV_HANDLER_H

#include <stdbool.h>

/* Size used to limit how much arguments may be defined 
 * in argv_handler. */
#define ARGV_SIZE 10
/* Size used to delimit how much a parameter may take up. */
#define ARGV_READ_SIZE 124

/* Type of the pointer which saves the callback function. 
 * Note that the program itself doesn't check if the arguments
 * are null, is the handler function responsibility to do so.*/
typedef int (*argv_fp)(char *, char *);

void exit_args(void);

/* Defines a interface where each argument information is saved in different
 * arrays with the same index.
 *
 * argv    - stores each defined argument.
 * fops    - stores the pointer to the argument's callback function. 
 *
 * argc    - stores how much arguments have been inserted. */
struct args_handler {
	char argv[ARGV_SIZE][ARGV_READ_SIZE];
	argv_fp fops[ARGV_SIZE];

	short argc;
};

char   *args_get_arg(int i);
argv_fp args_get_fop(int i);
short   args_get_argc();

/* Push elements to argv_handler.
 *
 * parameter:	The new argument. (e.g. -d)
 * fop:		The argument callback function. */
int args_add_arg(char *arg, argv_fp fop);

#endif
