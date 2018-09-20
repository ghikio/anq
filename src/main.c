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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct crypto_data dt;
struct crypto_ops  op;

int main(int argc, char *argv[])
{
	int err;

	err = parse(argc, argv);
	if(err)
		goto err;
	err = crypto_validate_data(&dt);
	if(err)
		goto err;

	err = start_ops(&op, &dt);

	return 0;

err:
	print_err_str(err);
	return err;
}
