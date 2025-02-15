﻿/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *		  http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_select_dispatch_h_
#define css_select_dispatch_h_

#include <stdint.h>

#include <thirdparty/libcss/libcss/errors.h>
#include <thirdparty/libcss/libcss/computed.h>

#include "thirdparty/libcss/libcss/stylesheetImpl.h"
#include "thirdparty/libcss/libcss/bytecode/bytecode.h"
#include "thirdparty/libcss/libcss/select/select.h"

extern struct prop_table {
	css_error (*cascade)(uint32_t opv, css_style *style,
			css_select_state *state);
	css_error (*set_from_hint)(const css_hint *hint,
			css_computed_style *style);
	css_error (*initial)(css_select_state *state);
	css_error (*compose)(const css_computed_style *parent,
			const css_computed_style *child,
			css_computed_style *result);
	unsigned int inherited;
} prop_dispatch[CSS_N_PROPERTIES];

#endif
