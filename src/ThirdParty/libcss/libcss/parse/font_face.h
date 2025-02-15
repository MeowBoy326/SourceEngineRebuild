﻿/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2011 Things Made Out Of Other Things Ltd.
 * Written by James Montgomerie <jamie@th.ingsmadeoutofotherthin.gs>
 */

#ifndef css_parse_font_face_h_
#define css_parse_font_face_h_

#include <thirdparty/libcss/libparserutils/utils/vector.h>

#include "thirdparty/libcss/libcss/stylesheetImpl.h"
#include "thirdparty/libcss/libcss/lex/lex.h"
#include "thirdparty/libcss/libcss/parse/language.h"

css_error css__parse_font_descriptor(css_language *c,
		const css_token *descriptor, const parserutils_vector *vector,
		int *ctx, struct css_rule_font_face *rule);

#endif
