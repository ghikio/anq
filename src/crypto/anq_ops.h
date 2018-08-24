/*  Released under BSD 3-Clause "New" or "Revised" License
 *  Copyright (c) 2018, Vitor Flavio Fernandes Ferreira
 *  All rights reserved.
 *
 *  https://spdx.org/licenses/BSD-3-Clause.html
 */

#ifndef ANQ_CRYPTO_ANQOPS_H
#define ANQ_CRYPTO_ANQOPS_H

#include "anq_data.h"

struct anq_ops {
	void (*encrypt) (struct anq_data *);
	void (*decrypt) (struct anq_data *);
};

#endif
