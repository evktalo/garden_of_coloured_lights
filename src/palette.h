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


void define_palette (void);
int build_new_palette (int base, int rshift, int gshift, int bshift);
void run_palette_shift (void);
void set_base_palette (void);

enum
{
  BASECOL_L1_SPRING,
  BASECOL_L1_SUMMER,
  BASECOL_L1_AUTUMN,
  BASECOL_L1_WINTER,
  BASECOL_L1_SPRING2,
  BASECOL_L2_1,
  BASECOL_L2_2,
  BASECOL_L2_3,
  BASECOL_L2_4,
  BASECOL_L2_5,
  BASECOL_L3_1,
  BASECOL_L3_2,
  BASECOL_L3_3,
  BASECOL_L3_4,
  BASECOL_L3_5,
  BASECOL_L4_1,
  BASECOL_L4_2,
  BASECOL_L4_3,
  BASECOL_L4_4,
  BASECOL_L4_5,
  BASECOL_CFLOWER,
  BASECOL_END
};


enum
{
  COL_OUTLINE = 1,
  COL_WHITE,
  COL_LGREY,
  COL_DGREY,
  COL_COL1,
  COL_COL2,
  COL_COL3,
  COL_COL4,
  COL_BACK1,
  COL_BACK2,
  COL_BACK3,
  COL_BACK4,
  COL_BACK5
};





/*
See the ebullet drawing function in display.c for how to use transparent colours.
Only these colours may be used (there is more flexibility if you decide to use 16 or higher colour depth,
but I like 8).
*/
enum
{
  TRANS_SH1_OUT = 15,
  TRANS_SH1_IN = 28,
  TRANS_SH2_OUT = 41,
  TRANS_SH2_IN = 54,
  TRANS_ORANGE_OUT = 67,
  TRANS_ORANGE_IN = 80,
  TRANS_YELLOW_OUT = 93,
  TRANS_YELLOW_IN = 106,
  TRANS_B1_OUT = 119,
  TRANS_B1_IN = 132,
  TRANS_B2_OUT = 145,
  TRANS_B2_IN = 158,
  TRANS_B3_OUT = 171,
  TRANS_B3_IN = 184,
  TRANS_B4_OUT = 197,
  TRANS_B4_IN = 210,
  TRANS_B5_OUT = 223,
  TRANS_B5_IN = 236
};




enum
{
  TCOL_SH1,
  TCOL_SH2,
  TCOL_ORANGE,
  TCOL_YELLOW,
  TCOL_B1,
  TCOL_B2,
  TCOL_B3,
  TCOL_B4,
  TCOL_B5
};

enum
{
  EBCOL_B1,
  EBCOL_B2,
  EBCOL_B3,
  EBCOL_B4,
  EBCOL_B5
};

#define TRANS_BACK2_OUTLINE 237
#define TRANS_BACK3_OUTLINE 238
enum
{
  TRANS_CONVERT_1 = 239,
  TRANS_CONVERT_2,
  TRANS_CONVERT_3,
  TRANS_CONVERT_4,
  TRANS_CONVERT_5,
  TRANS_CONVERT_6,
  TRANS_CONVERT_7,
  TRANS_CONVERT_8,
  TRANS_CONVERT_9,
  CONVERT_WHITE_TO_GREY
};

// don't think these do anything at the moment:
#define TRANS_REVERSE 225
#define TRANS_DARKEN 226

#define FIX_BITMAP 244

int colour_to_trans (int y);
