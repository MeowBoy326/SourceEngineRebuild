﻿

/*********************************************************************************
properties
*
*********************************************************************************/
/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_select_properties_h_
#define css_select_properties_h_

#include <thirdparty/libcss/libcss/errors.h>
#include <thirdparty/libcss/libcss/computed.h>

#include "thirdparty/libcss/libcss/stylesheetImpl.h"
#include "thirdparty/libcss/libcss/select/select.h"

#define PROPERTY_FUNCS(pname)                                           \
  css_error css__cascade_##pname (uint32_t opv, css_style *style, css_select_state *state); \
  css_error css__set_##pname##_from_hint(const css_hint *hint, css_computed_style *style); \
  css_error css__initial_##pname (css_select_state *state);                  \
  css_error css__compose_##pname (const css_computed_style *parent, const css_computed_style *child, css_computed_style *result); \
  uint32_t destroy_##pname (void *bytecode)

PROPERTY_FUNCS(align_content);
PROPERTY_FUNCS(align_items);
PROPERTY_FUNCS(align_self);
PROPERTY_FUNCS(azimuth);
PROPERTY_FUNCS(background_attachment);
PROPERTY_FUNCS(background_color);
PROPERTY_FUNCS(background_image);
PROPERTY_FUNCS(background_position);
PROPERTY_FUNCS(background_repeat);
PROPERTY_FUNCS(border_collapse);
PROPERTY_FUNCS(border_spacing);
PROPERTY_FUNCS(border_top_color);
PROPERTY_FUNCS(border_right_color);
PROPERTY_FUNCS(border_bottom_color);
PROPERTY_FUNCS(border_left_color);
PROPERTY_FUNCS(border_top_style);
PROPERTY_FUNCS(border_right_style);
PROPERTY_FUNCS(border_bottom_style);
PROPERTY_FUNCS(border_left_style);
PROPERTY_FUNCS(border_top_width);
PROPERTY_FUNCS(border_right_width);
PROPERTY_FUNCS(border_bottom_width);
PROPERTY_FUNCS(border_left_width);
PROPERTY_FUNCS(border_radius_top_left);
PROPERTY_FUNCS(border_radius_top_right);
PROPERTY_FUNCS(border_radius_bottom_right);
PROPERTY_FUNCS(border_radius_bottom_left);
PROPERTY_FUNCS(bottom);
PROPERTY_FUNCS(box_sizing);
PROPERTY_FUNCS(break_after);
PROPERTY_FUNCS(break_before);
PROPERTY_FUNCS(break_inside);
PROPERTY_FUNCS(caption_side);
PROPERTY_FUNCS(clear);
PROPERTY_FUNCS(clip);
PROPERTY_FUNCS(color);
PROPERTY_FUNCS(column_count);
PROPERTY_FUNCS(column_fill);
PROPERTY_FUNCS(column_gap);
PROPERTY_FUNCS(column_rule_color);
PROPERTY_FUNCS(column_rule_style);
PROPERTY_FUNCS(column_rule_width);
PROPERTY_FUNCS(column_span);
PROPERTY_FUNCS(column_width);
PROPERTY_FUNCS(content);
PROPERTY_FUNCS(counter_increment);
PROPERTY_FUNCS(counter_reset);
PROPERTY_FUNCS(cue_after);
PROPERTY_FUNCS(cue_before);
PROPERTY_FUNCS(cursor);
PROPERTY_FUNCS(direction);
PROPERTY_FUNCS(display);
PROPERTY_FUNCS(elevation);
PROPERTY_FUNCS(empty_cells);
PROPERTY_FUNCS(flex_basis);
PROPERTY_FUNCS(flex_direction);
PROPERTY_FUNCS(flex_grow);
PROPERTY_FUNCS(flex_shrink);
PROPERTY_FUNCS(flex_wrap);
PROPERTY_FUNCS(float);
PROPERTY_FUNCS(font_family);
PROPERTY_FUNCS(font_size);
PROPERTY_FUNCS(font_style);
PROPERTY_FUNCS(font_variant);
PROPERTY_FUNCS(font_weight);
PROPERTY_FUNCS(height);
PROPERTY_FUNCS(justify_content);
PROPERTY_FUNCS(left);
PROPERTY_FUNCS(letter_spacing);
PROPERTY_FUNCS(line_height);
PROPERTY_FUNCS(list_style_image);
PROPERTY_FUNCS(list_style_position);
PROPERTY_FUNCS(list_style_type);
PROPERTY_FUNCS(margin_top);
PROPERTY_FUNCS(margin_right);
PROPERTY_FUNCS(margin_bottom);
PROPERTY_FUNCS(margin_left);
PROPERTY_FUNCS(max_height);
PROPERTY_FUNCS(max_width);
PROPERTY_FUNCS(min_height);
PROPERTY_FUNCS(min_width);
PROPERTY_FUNCS(opacity);
PROPERTY_FUNCS(order);
PROPERTY_FUNCS(orphans);
PROPERTY_FUNCS(outline_color);
PROPERTY_FUNCS(outline_style);
PROPERTY_FUNCS(outline_width);
PROPERTY_FUNCS(overflow_x);
PROPERTY_FUNCS(overflow_y);
PROPERTY_FUNCS(padding_top);
PROPERTY_FUNCS(padding_right);
PROPERTY_FUNCS(padding_bottom);
PROPERTY_FUNCS(padding_left);
PROPERTY_FUNCS(page_break_after);
PROPERTY_FUNCS(page_break_before);
PROPERTY_FUNCS(page_break_inside);
PROPERTY_FUNCS(pause_after);
PROPERTY_FUNCS(pause_before);
PROPERTY_FUNCS(pitch_range);
PROPERTY_FUNCS(pitch);
PROPERTY_FUNCS(play_during);
PROPERTY_FUNCS(position);
PROPERTY_FUNCS(quotes);
PROPERTY_FUNCS(richness);
PROPERTY_FUNCS(right);
PROPERTY_FUNCS(speak_header);
PROPERTY_FUNCS(speak_numeral);
PROPERTY_FUNCS(speak_punctuation);
PROPERTY_FUNCS(speak);
PROPERTY_FUNCS(speech_rate);
PROPERTY_FUNCS(stress);
PROPERTY_FUNCS(table_layout);
PROPERTY_FUNCS(text_align);
PROPERTY_FUNCS(text_decoration);
PROPERTY_FUNCS(text_indent);
PROPERTY_FUNCS(text_transform);
PROPERTY_FUNCS(top);
PROPERTY_FUNCS(unicode_bidi);
PROPERTY_FUNCS(vertical_align);
PROPERTY_FUNCS(visibility);
PROPERTY_FUNCS(voice_family);
PROPERTY_FUNCS(volume);
PROPERTY_FUNCS(white_space);
PROPERTY_FUNCS(widows);
PROPERTY_FUNCS(width);
PROPERTY_FUNCS(word_spacing);
PROPERTY_FUNCS(writing_mode);
PROPERTY_FUNCS(z_index);

#undef PROPERTY_FUNCS

#endif



/*********************************************************************************
helpers
*
*********************************************************************************/
/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_select_properties_helpers_h_
#define css_select_properties_helpers_h_

uint32_t generic_destroy_color(void* bytecode);
uint32_t generic_destroy_uri(void* bytecode);
uint32_t generic_destroy_length(void* bytecode);
uint32_t generic_destroy_number(void* bytecode);

css_unit css__to_css_unit(uint32_t u);

css_error css__cascade_bg_border_color(uint32_t opv, css_style* style,
    css_select_state* state,
    css_error(*fun)(css_computed_style*, uint8_t, css_color));
css_error css__cascade_uri_none(uint32_t opv, css_style* style,
    css_select_state* state,
    css_error(*fun)(css_computed_style*, uint8_t,
        lwc_string*));
css_error css__cascade_border_style(uint32_t opv, css_style* style,
    css_select_state* state,
    css_error(*fun)(css_computed_style*, uint8_t));
css_error css__cascade_border_width(uint32_t opv, css_style* style,
    css_select_state* state,
    css_error(*fun)(css_computed_style*, uint8_t, css_fixed,
        css_unit));
css_error css__cascade_length_auto(uint32_t opv, css_style* style,
    css_select_state* state,
    css_error(*fun)(css_computed_style*, uint8_t, css_fixed,
        css_unit));
css_error css__cascade_length_normal(uint32_t opv, css_style* style,
    css_select_state* state,
    css_error(*fun)(css_computed_style*, uint8_t, css_fixed,
        css_unit));
css_error css__cascade_length_none(uint32_t opv, css_style* style,
    css_select_state* state,
    css_error(*fun)(css_computed_style*, uint8_t, css_fixed,
        css_unit));
css_error css__cascade_length(uint32_t opv, css_style* style,
    css_select_state* state,
    css_error(*fun)(css_computed_style*, uint8_t, css_fixed,
        css_unit));
css_error css__cascade_number(uint32_t opv, css_style* style,
    css_select_state* state,
    css_error(*fun)(css_computed_style*, uint8_t, css_fixed));
css_error css__cascade_page_break_after_before_inside(uint32_t opv,
    css_style* style, css_select_state* state,
    css_error(*fun)(css_computed_style*, uint8_t));
css_error css__cascade_break_after_before_inside(uint32_t opv,
    css_style* style, css_select_state* state,
    css_error(*fun)(css_computed_style*, uint8_t));
css_error css__cascade_counter_increment_reset(uint32_t opv, css_style* style,
    css_select_state* state,
    css_error(*fun)(css_computed_style*, uint8_t,
        css_computed_counter*));

#endif
