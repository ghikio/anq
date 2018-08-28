/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_ARGV_PARSER_H
#define ANQ_ARGV_PARSER_H

/* The trigger, must be called after argv_init and after all the args have
 * been setted. */
void anq_argv_parse(int argc, char *argv[]);

#endif
