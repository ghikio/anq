/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "parser.h"
#include "err_codes.h"
#include "crypto_ops.h"
#include "crypto_data.h"
#include "argv_parser.h"
#include "argv_handler.h"
#include "config_parser.h"

#include <stdio.h>
#include <stdlib.h>

extern struct crypto_data dt;
extern struct crypto_ops  op;

/* Maybe not the best place for these constants, but since they're
 * not yet needed anywhere else in the program, we'll secretly
 * keep them here ;) */
#define NAME	"ANQ"
#define AUTHOR  "Vitor Fernandes <contact@thecriw.com>"
#define VERSION "v0.0.1"
#define LICENSE "Released under BSD 3-Clause \"New\" or \"Revised\" License" \
		"\nCopyright (c) 2018, Vitor Flavio Fernandes Ferreira" \
		"\nAll rights reserved." \
		"\n\nhttps://spdx.org/licenses/BSD-3-Clause.html\n"

int args_help(char *arg, char *value)
{
	printf("%s - %s\n%s\n\n%s\n", NAME, VERSION, AUTHOR, LICENSE);
	puts("-h - print help menu");
	puts("-d - decrypt password");
	puts("-e - encrypt password");
	puts("-s - password's service name");

	return ANQ_ERR_HELP_MENU;
}

int args_decrypt(char *arg, char *value)
{
	crypto_set_operation(&dt, ANQ_OP_DECRYPT);
	return 0;
}

int args_encrypt(char *arg, char *value)
{
	crypto_set_operation(&dt, ANQ_OP_ENCRYPT);
	return 0;
}

int args_service(char *arg, char *value)
{
	if(value[0] == '\0')
		return ANQ_ERR_NO_SERVICE_VALUE;

	crypto_set_service(&dt, value);
	return 0;
}

void parse(int argc, char *argv[])
{
	int err;

	err = init_args();
	if(err)
		goto init_err;

	args_new_arg("-h", args_help); 
	args_new_arg("-d", args_decrypt);
	args_new_arg("-e", args_encrypt);
	args_new_arg("-s", args_service);

	err = parse_argv(argc, argv);
	if(err)
		goto parse_err;

	parse_config();

	return;

parse_err:
	exit_args();
init_err:
	print_err_str(err);
	exit(err);
}
