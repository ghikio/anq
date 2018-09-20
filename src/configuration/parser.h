/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_CONFIGURATION_PARSER_H
#define ANQ_CONFIGURATION_PARSER_H

/* Built as a abstraction for the whole layer of the
 * configuration. Internally handles the arguments parsing
 * and the configuration file parsing.
 *
 * If an error occurs it stops the program, since if the
 * parsing fails, the data collected is not ensured to be the
 * one wanted. */
int parse(int argc, char *argv[]);

#endif
