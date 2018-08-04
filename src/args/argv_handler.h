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
 * in s_argv_handler. */
#define ARGV_SIZE 10
/* Size used to delimit how much a parameter may take up. */
#define ARGV_READ_SIZE 124

/* Type of the pointer which saves the callback function. */
typedef void (*argv_fptr)(char *, char *);

/* Defines a interface where each argument information is saved in different
 * arrays with the same index.
 *
 * argv    - stores each defined argument.
 * has_del - stores the possibility of the argument having value syntax. 
 * fops    - stores the pointer to the argument's callback function. 
 *
 * argc    - stores how much arguments have been inserted. */
struct s_argv_handler {
	char argv[ARGV_SIZE][ARGV_READ_SIZE];
	bool has_del[ARGV_SIZE];
	argv_fptr fops[ARGV_SIZE];

	short argc;
};

int  argv_init(void);
void argv_exit(void);

char *argv_get_arg(int i);
bool  argv_get_del(int i);
argv_fptr argv_get_fop(int i);
short argv_get_argc();

/* Push elements to s_argv_handler.
 *
 * parameter:	The new argument. (e.g. -d)
 * del:		True if the argument has a value, false if not.
 *		(.e.g. false -> -d, true -> -d=something).
 * fop:		The argument callback function. */
int
argv_add_parameter(char *arg, bool del, argv_fptr fop);

#endif
