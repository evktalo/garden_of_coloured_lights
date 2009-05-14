/*
 * This file is part of 'Garden of coloured lights'.
 *
 * 'Garden of coloured lights' is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 'Garden of coloured lights' is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 'Garden of coloured lights'.  If not, see <http://www.gnu.org/licenses/>.
 */

/** @file */

#ifndef DISPLAY_INIT_H
#define DISPLAY_INIT_H

#include "display.h"

/** Safe wrapper for the Allegro create_bitmap function. */
BITMAP *new_bitmap(int x, int y, const char errtxt []);

#ifdef OLD_MODE
/** Draws a 4-sided polygon with a border */
void bordered_poly4(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int col, int border_col);
void bordered_poly6(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int x6, int y6, int col, int border_col);
void fast_bordered_poly4(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int col, int border_col, int fillx, int filly);
#endif /* OLD_MODE */
void bordered_triangle(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int col, int border_col);
void fast_bordered_triangle(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int col, int border_col, int fillx, int filly);
/** Makes a space-efficient RLE from a bitmap */
void new_rle_struct(BITMAP *source, const char errtxt [], struct RLE_STRUCT *str, int bl);
void prepare_enemy_bullets(void);
void prepare_platform_rles(void);
void prepare_s_enemy_rles(void);
void prepare_l_enemy_rles(void);
void prepare_trans_rles(void);
void prepare_blades(void);
void prepare_multi_rles(void);
void prepare_rings(void);
void prepare_petals(void);
//void prepare_icon_rles(void);

void prepare_lwbeamwaves(void);
void prepare_mshockwaves(void);
void prepare_lshockwaves(void);
void prepare_hshockwaves(void);
void prepare_rshockwaves(void);

//void prepare_various_effects(void);

void fix_outline(BITMAP *source);
void fix_trans(BITMAP *source);
/** Draw a border between black and non-black colours */
void coat_sprite(BITMAP *target, int col);
/** The same as coat_sprite(), but on a sub-bitmap */
void coat_file_sprite_xy(BITMAP *target, int col, int x1, int y1, int x2, int y2);
/** Make the flower RLEs */
void prepare_flowers(void);
void extract_rle_struct(BITMAP *source, int x1, int y1, int x2, int y2, const char errtxt [], struct RLE_STRUCT *str, int bl);
void extract_rle_struct2(BITMAP *source, struct RLE_STRUCT *str, int x_source, int y_source, int x, int y, int b1);
//void prepare_underlay(void);

/**  */
void new_rle_struct_colours(BITMAP *source, const char errtxt [], struct RLE_STRUCT str [SMALL_ROTATIONS] [9], int bl, int index2, int col_0, int col_n);

BITMAP *load_up_bitmap(const char fname []);
RLE_SPRITE *extract_rle_sprite(BITMAP *source, int x_source, int y_source, int x, int y);
RLE_SPRITE *extract_flip_rle_sprite(BITMAP *source, int x_source, int y_source, int x, int y, int flip_type);
RLE_SPRITE *sized_rle_sprite(BITMAP *source, int x, int y, const char errtxt []);
RLE_SPRITE *new_rle_sprite(BITMAP *source, const char errtxt []);


void prepare_display (void);

#endif /* DISPLAY_INIT_H */
