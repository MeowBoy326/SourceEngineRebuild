﻿/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2017 The NetSurf Project
 */


struct css_computed_style_i {
/*
 * Property                       Size (bits)     Size (bytes)
 * ---                            ---             ---
 * align_content                    3             
 * align_items                      3             
 * align_self                       3             
 * background_attachment            2             
 * background_color                 2               4
 * background_image                 1             sizeof(ptr)
 * background_position              1 + 10          8
 * background_repeat                3             
 * border_bottom_color              2               4
 * border_bottom_style              4             
 * border_bottom_width              3 + 5           4
 * border_collapse                  2             
 * border_left_color                2               4
 * border_left_style                4             
 * border_left_width                3 + 5           4
 * border_radius_bottom_left        2 + 5           4
 * border_radius_bottom_right       2 + 5           4
 * border_radius_top_left           2 + 5           4
 * border_radius_top_right          2 + 5           4
 * border_right_color               2               4
 * border_right_style               4             
 * border_right_width               3 + 5           4
 * border_spacing                   1 + 10          8
 * border_top_color                 2               4
 * border_top_style                 4             
 * border_top_width                 3 + 5           4
 * bottom                           2 + 5           4
 * box_sizing                       2             
 * break_after                      4             
 * break_before                     4             
 * break_inside                     4             
 * caption_side                     2             
 * clear                            3             
 * clip                             6 + 20         16
 * color                            1               4
 * column_count                     2               4
 * column_fill                      2             
 * column_gap                       2 + 5           4
 * column_rule_color                2               4
 * column_rule_style                4             
 * column_rule_width                3 + 5           4
 * column_span                      2             
 * column_width                     2 + 5           4
 * direction                        2             
 * display                          5             
 * empty_cells                      2             
 * flex_basis                       2 + 5           4
 * flex_direction                   3             
 * flex_grow                        1               4
 * flex_shrink                      1               4
 * flex_wrap                        2             
 * float                            2             
 * font_size                        4 + 5           4
 * font_style                       2             
 * font_variant                     2             
 * font_weight                      4             
 * height                           2 + 5           4
 * justify_content                  3             
 * left                             2 + 5           4
 * letter_spacing                   2 + 5           4
 * line_height                      2 + 5           4
 * list_style_image                 1             sizeof(ptr)
 * list_style_position              2             
 * list_style_type                  4             
 * margin_bottom                    2 + 5           4
 * margin_left                      2 + 5           4
 * margin_right                     2 + 5           4
 * margin_top                       2 + 5           4
 * max_height                       2 + 5           4
 * max_width                        2 + 5           4
 * min_height                       2 + 5           4
 * min_width                        2 + 5           4
 * opacity                          1               4
 * order                            1               4
 * orphans                          1               4
 * outline_color                    2               4
 * outline_style                    4             
 * outline_width                    3 + 5           4
 * overflow_x                       3             
 * overflow_y                       3             
 * padding_bottom                   1 + 5           4
 * padding_left                     1 + 5           4
 * padding_right                    1 + 5           4
 * padding_top                      1 + 5           4
 * page_break_after                 3             
 * page_break_before                3             
 * page_break_inside                2             
 * position                         3             
 * right                            2 + 5           4
 * table_layout                     2             
 * text_align                       4             
 * text_decoration                  5             
 * text_indent                      1 + 5           4
 * text_transform                   3             
 * top                              2 + 5           4
 * unicode_bidi                     2             
 * vertical_align                   4 + 5           4
 * visibility                       2             
 * white_space                      3             
 * widows                           1               4
 * width                            2 + 5           4
 * word_spacing                     2 + 5           4
 * writing_mode                     2             
 * z_index                          2               4
 * 
 * Encode content as an array of content items, terminated with a blank entry.
 * 
 * content                          2             sizeof(ptr)
 * 
 * Encode counter_increment as an array of name, value pairs, terminated with a
 * blank entry.
 * 
 * counter_increment                1             sizeof(ptr)
 * 
 * Encode counter_reset as an array of name, value pairs, terminated with a
 * blank entry.
 * 
 * counter_reset                    1             sizeof(ptr)
 * 
 * Encode cursor uri(s) as an array of string objects, terminated with a blank
 * entry
 * 
 * cursor                           5             sizeof(ptr)
 * 
 * Encode font family as an array of string objects, terminated with a blank
 * entry.
 * 
 * font_family                      3             sizeof(ptr)
 * 
 * Encode quotes as an array of string objects, terminated with a blank entry.
 * 
 * quotes                           1             sizeof(ptr)
 * 
 * ---                            ---             ---
 *                                488 bits        244 + 8sizeof(ptr) bytes
 *                                ===================
 *                                305 + 8sizeof(ptr) bytes
 * 
 * Bit allocations:
 * 
 * 0  bbbbbbbboooooooouuuuuuuurrrrrrrr
 * border_top_width; border_bottom_width; outline_width; border_right_width
 * 
 * 1  vvvvvvvvvbbbbbbbbccccccccooooooo
 * vertical_align; border_left_width; column_rule_width; column_gap
 * 
 * 2  cccccccccccccccccccccccccctttttt
 * clip; text_indent
 * 
 * 3  bbbbbbbwwwwwwwmmmmmmmppppppttttt
 * border_radius_top_left; word_spacing; margin_right; padding_top;
 * text_decoration
 * 
 * 4  lllllllmmmmmmmaaaaaaaeeeeeeeffff
 * letter_spacing; max_width; margin_top; left; font_weight
 * 
 * 5  bbbbbbbooooooommmmmmmrrrrrrruuuu
 * bottom; border_radius_bottom_left; min_width; border_radius_top_right;
 * outline_style
 * 
 * 6  tttttttccccccclllllllmmmmmmmbbbb
 * top; column_width; line_height; margin_left; break_inside
 * 
 * 7  mmmmmmmfffffffrrrrrrrhhhhhhhbbbb
 * margin_bottom; flex_basis; right; height; border_right_style
 * 
 * 8  mmmmmmmaaaaaaabbbbbbbwwwwwwwoooo
 * min_height; max_height; border_radius_bottom_right; width; border_left_style
 * 
 * 9  ppppppaaaaaaddddddiiiiicccccbbbb
 * padding_right; padding_left; padding_bottom; display; cursor; break_after
 * 
 * 10 bbbbrrrrttttccccoooollllaaakkkpp
 * border_bottom_style; break_before; text_align; column_rule_style;
 * border_top_style; list_style_type; align_items; background_repeat;
 * page_break_inside
 * 
 * 11 tttwwwfffpppjjjcccaaaooolllgggvv
 * text_transform; white_space; flex_direction; page_break_after;
 * justify_content; clear; align_self; overflow_x; align_content;
 * page_break_before; visibility
 * 
 * 12 bbbbbbbbbbbaaaaaaaaaaafffffffffc
 * border_spacing; background_position; font_size; counter_increment
 * 
 * 13 pppfffooouuccbbllxxaattzzrrnneeq
 * position; font_family; overflow_y; unicode_bidi; caption_side;
 * border_left_color; column_rule_color; box_sizing; float; table_layout;
 * z_index; border_collapse; font_style; flex_wrap; quotes
 * 
 * 14 bbccooaawwllnnkkiirruuddeettffsx
 * border_bottom_color; column_count; border_top_color; background_attachment;
 * writing_mode; column_span; content; background_color; list_style_position;
 * border_right_color; column_fill; direction; empty_cells; outline_color;
 * font_variant; counter_reset; flex_grow
 * 
 * 15 opwrcfbl........................
 * order; opacity; widows; orphans; color; flex_shrink; background_image;
 * list_style_image
 */
	uint32_t bits[16];
	
	css_color background_color;
	lwc_string *background_image;
	css_fixed background_position_a;
	css_fixed background_position_b;
	css_color border_bottom_color;
	css_fixed border_bottom_width;
	css_color border_left_color;
	css_fixed border_left_width;
	css_fixed border_radius_bottom_left;
	css_fixed border_radius_bottom_right;
	css_fixed border_radius_top_left;
	css_fixed border_radius_top_right;
	css_color border_right_color;
	css_fixed border_right_width;
	css_fixed border_spacing_a;
	css_fixed border_spacing_b;
	css_color border_top_color;
	css_fixed border_top_width;
	css_fixed bottom;
	css_fixed clip_a;
	css_fixed clip_b;
	css_fixed clip_c;
	css_fixed clip_d;
	css_color color;
	int32_t column_count;
	css_fixed column_gap;
	css_color column_rule_color;
	css_fixed column_rule_width;
	css_fixed column_width;
	css_fixed flex_basis;
	css_fixed flex_grow;
	css_fixed flex_shrink;
	css_fixed font_size;
	css_fixed height;
	css_fixed left;
	css_fixed letter_spacing;
	css_fixed line_height;
	lwc_string *list_style_image;
	css_fixed margin_bottom;
	css_fixed margin_left;
	css_fixed margin_right;
	css_fixed margin_top;
	css_fixed max_height;
	css_fixed max_width;
	css_fixed min_height;
	css_fixed min_width;
	css_fixed opacity;
	int32_t order;
	int32_t orphans;
	css_color outline_color;
	css_fixed outline_width;
	css_fixed padding_bottom;
	css_fixed padding_left;
	css_fixed padding_right;
	css_fixed padding_top;
	css_fixed right;
	css_fixed text_indent;
	css_fixed top;
	css_fixed vertical_align;
	int32_t widows;
	css_fixed width;
	css_fixed word_spacing;
	int32_t z_index;
	
};

struct css_computed_style {
	struct css_computed_style_i i;
	
	css_computed_content_item *content;
	css_computed_counter *counter_increment;
	css_computed_counter *counter_reset;
	lwc_string **cursor;
	lwc_string **font_family;
	lwc_string **quotes;
	
	struct css_computed_style *next;
	uint32_t count;
	uint32_t bin;
};
