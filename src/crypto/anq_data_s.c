/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "anq_data_s.h"

#include <assert.h>

enum anq_operation_t anq_get_operation(struct anq_data_s *dt)
{
	assert(dt->op != 0);
	return dt->op;
}

void anq_set_operation(struct anq_data_s *dt, enum anq_operation_t op)
{
	// Based on the current anq_operation_t, change
	// if new options are added.
	assert(op % 2 == 0 && op <= 2);
	dt->op = op;
}

char *anq_get_keyquery(struct anq_data_s *dt)
{
	return dt->keyquery;
}

void anq_set_keyquery(struct anq_data_s *dt, char *query)
{
	dt->keyquery = query;
}
