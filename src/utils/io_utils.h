/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_UTILS_IOUTILS_H
#define ANQ_UTILS_IOUTILS_H

#include <stdbool.h>

bool  check_file_access(char* f, int mode);
char *make_filename(char *path, char *file);
/* Slice argv in two parts denoted by the (del)imiter. 
 * First part go into sa and second part into sb.
 *
 * If no delimiter is found, returns ANQ_ERR_NO_DELIMITER,
 * otherwise 0, even if sb ends being NULL. */
void slice_argv(char *argv, char del, char *sa, char *sb);

#endif
