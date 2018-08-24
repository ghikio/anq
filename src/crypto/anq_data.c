/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "anq_data.h"

#include <assert.h>

enum anq_op anq_get_operation(struct anq_data *dt)
{
	assert(dt->op != 0);
	return dt->op;
}

void anq_set_operation(struct anq_data *dt, enum anq_op op)
{
	/* Based on the current anq_op, change if new
	 * options are added. */
	assert(op % 2 == 0 && op <= 2);
	dt->op = op;
}

char *anq_get_keyquery(struct anq_data *dt)
{
	return dt->keyquery;
}

void anq_set_keyquery(struct anq_data *dt, char *query)
{
	dt->keyquery = query;
}
