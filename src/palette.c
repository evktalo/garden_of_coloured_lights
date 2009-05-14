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

/*

   This function is straight out of Excellent Bifurcation.
   I haven't fixed it up for this example at all.
   Use the colour enum in palette.h.

 */


#include "data.h"

#include "allegro.h"


#include "palette.h"
#include "globvars.h"

COLOR_MAP trans_table;
//COLOR_MAP trans_table2;

unsigned char trans_converts[NUM_TRANS_CONVERTS] = {TRANS_CONVERT_0, TRANS_CONVERT_1, TRANS_CONVERT_2, TRANS_CONVERT_3, TRANS_CONVERT_4, TRANS_CONVERT_5, TRANS_CONVERT_6, TRANS_CONVERT_7, TRANS_CONVERT_8};
//int blend_function(int base, int trans, RGB *rgbl);
int blend_function (int trans, int base, RGB * rgbl);
int blend_function2 (int trans, int base, RGB * rgbl);
int limit_colour (int colour_input);

void create_inner_bullet_colour (int pa, int index, int r, int g, int b);
void create_outer_bullet_colour (int pa, int index, int r, int g, int b);

int build_new_palette (int base, int rshift, int gshift, int bshift);

void halfway_colour (int base, int target, int r, int g, int b);


RGB other_palet[1324];
RGB palet[2][256];
RGB palet2[256];
//RGB light_palet [256];
//RGB dark_palet [256];
RGB other_palet2[2048];
// what seems to be a weird bug in Allegro forces me to put padding around
//  the palet array, or it becomes corrupted. The same thing happened in
//  World of Violence and I have no idea why.

/*


IMPORTANT TO REMEMBER:

    I created the palette for the GIMP by saving a 16x16 image with all colours
    as a gif then importing the palette from that. This let me get the colours
    in the right order!


 In the GIMP, must go to image|mode|indexed colour and change the settings there.






*/


void special_create_color_table (COLOR_MAP * table);	//, AL_CONST PALETTE pal);
void special_create_color_table2 (COLOR_MAP * table, AL_CONST PALETTE pal);
int base_colour (int y);

void colour_table (const char *which_call);


/*
Colour scheme:

Trans colours:
0 - base
1 - trans1 inner
2 - trans1 outer
3 - trans2 inner
4 - trans2 outer
5 - trans3 inner
6 - trans3 outer
7 - white inner
8 - white outer

base colours: to 28
Remember, 0 is black so base 0 is actually indexed as 1

outline 0
white 1
lgrey 2
dgrey 3
col1 4
col2 5
col3 6
col4 7
back1 9
back2 9
back3 10
back4 11
back5 12


*/


int base_colours[BASECOL_END][10][3] = {

	{
// Spring
		{45, 45, 25},		// col1
		{35, 35, 18},		// col2
		{45, 25, 35},		// col3
		{35, 18, 25},		// col4
//{8, 10, 11}, // back1
		{11, 16, 12},		// back1
		{15, 19, 16},		// back2
		{16, 23, 18},		// back3
		{19, 26, 19},		// back4}
		{43, 33, 43}			// back5}

	},

	{
// Summer
		{46, 35, 21},		// col1
		{35, 28, 17},		// col2
		{50, 25, 21},		// col3
		{38, 21, 16},		// col4
		{16, 16, 22},		// back1
		{19, 19, 31},		// back2
		{18, 21, 10},		// back3
		{21, 24, 13},		// back4
		{37, 34, 19}			// back5}

	},
	/*
	{
	// Summer
	{46, 35, 21}, // col1
	{35, 28, 17}, // col2
	{50, 25, 21}, // col3
	{38, 21, 16}, // col4
	{14, 14, 22}, // back1
	{15, 15, 31}, // back2
	{18, 21, 10}, // back3
	{21, 24, 13}, // back4
	{45, 42, 15} // back5}

	},*/

	{
// Autumn
		{46, 21, 21},		// col1
		{35, 27, 17},		// col2
		{40, 35, 21},		// col3
		{35, 26, 16},		// col4
		{18, 18, 16},		// back1
		{22, 22, 19},		// back2
		{22, 20, 6},			// back3
		{24, 22, 9},			// back4
		{45, 12, 12}			// back5}

	},

	{
// Winter
		{27, 27, 53},		// col1
		{24, 24, 49},		// col2
		{47, 47, 52},		// col3
		{34, 34, 41},		// col4
		{16, 16, 16},		// back1
		{18, 18, 18},		// back2
		{20, 20, 20},		// back3
		{22, 22, 22},		// back4
		{32, 27, 11}			// back5}

	},

	{
// Second spring
		{45, 45, 25},		// col1
		{35, 35, 18},		// col2
		{45, 25, 35},		// col3
		{35, 18, 25},		// col4
//{8, 10, 11}, // back1
		{10, 15, 12},		// back1
		{14, 18, 16},		// back2
		{15, 21, 17},		// back3
		{18, 24, 18},		// back4}
		{43, 33, 43}			// back5}

		/*{10, 15, 12}, // back1
		{13, 19, 15}, // back2
		{14, 22, 16}, // back3
		{17, 25, 17}, // back4}
		{45, 30, 45} // back5}*/

	},
	/*
	{
	{33, 33, 42}, // col1
	{20, 20, 28}, // col2
	{33, 42, 33}, // col3
	{20, 28, 20}, // col4
	{40, 40, 50}, // back1
	{30, 30, 40}, // back2
	{22, 22, 30}, // back3
	{12, 15, 25}, // back4
	{16, 18, 30} // back5}

	},*/


	{
// Stage 2 - cold
		{33, 33, 42},		// col1
		{20, 20, 28},		// col2
		{33, 42, 33},		// col3
		{20, 28, 20},		// col4
		{40, 40, 50},		// back1
		{30, 30, 40},		// back2
		{22, 22, 30},		// back3
		{12, 17, 25},		// back4
		{16, 20, 30}			// back5}

	},


	{
// Stage 2 - warmer
		{36, 33, 39},		// col1
		{22, 20, 26},		// col2
		{35, 40, 33},		// col3
		{22, 26, 20},		// col4
		{45, 40, 45},		// back1
		{35, 30, 35},		// back2
		{26, 22, 26},		// back3
		{15, 15, 23},		// back4
		{20, 18, 27}			// back5}

	},

	{
// Stage 2 - hot
		{39, 33, 33},		// col1
		{26, 20, 20},		// col2
		{39, 40, 33},		// col3
		{26, 26, 20},		// col4
		{45, 35, 42},		// back1
		{35, 26, 32},		// back2
		{28, 20, 24},		// back3
		{17, 13, 21},		// back4
		{23, 16, 25}			// back5}

	},

	{
// Stage 2 - hotter
		{42, 28, 28},		// col1
		{28, 16, 18},		// col2
		{45, 42, 28},		// col3
		{35, 29, 16},		// col4
		{47, 25, 26},		// back1
		{37, 12, 15},		// back2
		{31, 10, 12},		// back3
		{25, 15, 15},		// back4
		{32, 19, 18}			// back5}

	},

	{
// Stage 2 - hottest
		{54, 45, 45},		// col1
		{45, 32, 28},		// col2
		{54, 49, 45},		// col3
		{45, 35, 28},		// col4
		{47, 35, 25},		// back1
		{38, 22, 17},		// back2
		{37, 15, 15},		// back3
		{29, 22, 19},		// back4
		{34, 27, 22}			// back5}

	},




	{
// stage 3 - red
		{35, 35, 35},		// col1
		{40, 10, 10},		// col2
		{10, 40, 10},		// col3
		{35, 35, 10},		// col4
		{12, 12, 12},		// back1
		{20, 20, 20},		// back2
		{30, 20, 20},		// back3
		{40, 20, 20},		// back4
		{50, 20, 20}			// back6}
	},

	{
// stage 3 - orange/yellow
		{40, 30, 30},		// col1
		{40, 10, 10},		// col2
		{10, 40, 10},		// col3
		{35, 35, 10},		// col4
		{11, 11, 11},		// back1
		{20, 20, 20},		// back2
		{30, 25, 20},		// back3
		{40, 32, 20},		// back4
		{50, 40, 20}			// back6}
	},

	{
// stage 3 - green
		{30, 40, 30},		// col1
		{45, 10, 10},		// col2
		{10, 45, 10},		// col3
		{39, 39, 10},		// col4
		{9, 9, 9},			// back1
		{18, 18, 18},		// back2
		{22, 30, 20},		// back3
		{26, 40, 20},		// back4
		{30, 50, 20}			// back6}
	},

	{
// stage 3 - blue
		{30, 30, 40},		// col1
		{50, 10, 10},		// col2
		{10, 50, 10},		// col3
		{45, 45, 10},		// col4
		{8, 8, 8},			// back1
		{10, 15, 20},		// back2
		{11, 20, 30},		// back3
		{12, 20, 40},		// back4
		{13, 20, 50}			// back6}
	},

	{
// stage 3 - violet
		{45, 40, 45},		// col1
		{50, 20, 20},		// col2
		{20, 50, 20},		// col3
		{45, 45, 20},		// col4
		{23, 22, 26},		// back1
		{28, 20, 32},		// back2
		{32, 14, 37},		// back3
		{37, 11, 43},		// back4
		{41, 10, 50}			// back6}
	},

	/*{
	// stage 3 - violet
	{45, 40, 45}, // col1
	{50, 20, 20}, // col2
	{20, 50, 20}, // col3
	{45, 45, 20}, // col4
	{6, 6, 6}, // back1
	{15, 10, 20}, // back2
	{23, 10, 30}, // back3
	{30, 10, 40}, // back4
	{35, 10, 50} // back6}
	},*/

	/*
	{
	// Stage 4
	{45, 25, 25}, // col1
	{25, 25, 45}, // col2
	{45, 25, 25}, // col3
	{25, 25, 45}, // col4
	{25, 25, 25}, // back1
	{13, 19, 15}, // back2
	{14, 22, 16}, // back3
	{17, 17, 15}, // back4}
	{26, 26, 25} // back5}

	},
	*/
	/*
	{
	// Stage 4
	{45, 40, 45}, // col1
	{36, 36, 40}, // col2
	{42, 42, 38}, // col3
	{45, 40, 45}, // col4


	{30, 30, 30}, // back1
	{34, 34, 34}, // back2
	{32, 32, 32}, // back3
	{32, 32, 32}, // back4}
	{34, 34, 34} // back5}


	},*/


	{
// Stage 4 - 1
		{47, 35, 35},		// col1
		{38, 30, 30},		// col2
		{40, 30, 40},		// col3
		{33, 25, 33},		// col4
		{33, 33, 33},		// back1
		{34, 34, 44},		// back2
		{38, 30, 30},		// back3
		{35, 35, 35},		// back4}
		{37, 37, 37}			// back5}
	},


	{
// Stage 4 - 2
		{37, 45, 37},		// col1
		{32, 36, 32},		// col2
		{37, 37, 32},		// col3
		{34, 31, 30},		// col4
		{34, 30, 30},		// back1
		{44, 34, 34},		// back2
		{30, 30, 38},		// back3
		{37, 32, 32},		// back4}
		{39, 33, 33}			// back5}
	},

	{
// Stage 4 - 3
		{47, 37, 37},		// col1
		{38, 32, 32},		// col2
		{43, 32, 32},		// col3
		{38, 30, 30},		// col4
		{29, 26, 26},		// back1
		{46, 34, 34},		// back2
		{40, 30, 30},		// back3
		{32, 28, 28},		// back4}
		{35, 30, 30}			// back5}
	},


	{
// Stage 4 - 4
		{47, 47, 35},		// col1
		{38, 38, 30},		// col2
		{44, 44, 38},		// col3
		{47, 47, 40},		// col4
		{31, 31, 29},		// back1
		{44, 44, 34},		// back2
		{37, 37, 32},		// back3
		{33, 33, 30},		// back4}
		{35, 35, 32}			// back5}

	},

	{
// Stage 4 - 5
		{46, 46, 46},		// col1
		{36, 36, 36},		// col2
		{42, 42, 42},		// col3
		{47, 47, 47},		// col4
		{25, 25, 25},		// back1
		{44, 44, 44},		// back2
		{32, 32, 32},		// back3
		{28, 28, 28},		// back4}
		{31, 31, 31}			// back5}

	},

	{
// CFLOWER (for winning screen)
		{45, 45, 35},		// col1
		{45, 35, 45},		// col2
		{45, 45, 45},		// col3
		{40, 36, 43},		// col4
//{8, 10, 11}, // back1
		{35, 35, 42},		// back1
		{42, 32, 32},		// back2
		{42, 39, 30},		// back3
		{45, 30, 40},		// back4}
		{40, 40, 40}			// back5}

	},


};

int current_base[10][3];

int swap;

void set_base_palette (void)
{

	int i, j;

	for (i = 5; i < 14; i++)
	{
		for (j = 0; j < 3; j++)
		{
			current_base[i - 5][j] = base_colours[arena.target_palette][i - 5][j];
		}
	}
// vsync();
// set_palette(palet [swap]);
}

void run_palette_shift (void)
{

	int i, j;
	char changed = 0;

	for (i = 5; i < 14; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (current_base[i - 5][j] <
			        base_colours[arena.target_palette][i - 5][j])
			{
				current_base[i - 5][j]++;
				changed = 1;
			}
			if (current_base[i - 5][j] >
			        base_colours[arena.target_palette][i - 5][j])
			{
				current_base[i - 5][j]--;
				changed = 1;
			}
		}
	}

	if (changed == 0)
		arena.shift_palette = 0;

}

void define_palette (void)
{

	int i;

	/* palet [0].r = 0;
	 palet [0].g = 0;
	 palet [0].b = 0;*/

	swap = 0;

	do
	{

		for (i = 0; i < 256; i++)
		{
			palet[swap][i].r = 0;
			palet[swap][i].g = 0;
			palet[swap][i].b = 0;
		}

		palet[swap][1].r = 10;	// outline
		palet[swap][1].g = 10;
		palet[swap][1].b = 10;

		palet[swap][2].r = 50;	// white
		palet[swap][2].g = 50;
		palet[swap][2].b = 50;

		palet[swap][3].r = 35;	// lgrey - was 40
		palet[swap][3].g = 35;
		palet[swap][3].b = 35;

		palet[swap][4].r = 22;	// dgrey - was 30
		palet[swap][4].g = 22;
		palet[swap][4].b = 22;

		palet[swap][255].r = 63;
		palet[swap][255].g = 63;
		palet[swap][255].b = 63;

		swap++;
		if (swap == 2)
		{
			swap = 0;
			break;
		}

	}
	while (TRUE);

	for (i = 5; i < 14; i++)
	{
		current_base[i - 5][0] = base_colours[0][i - 5][0];
		current_base[i - 5][1] = base_colours[0][i - 5][1];
		current_base[i - 5][2] = base_colours[0][i - 5][2];
	}


	build_new_palette (0, 0, 0, 0);

	vsync ();
	set_palette (palet[0]);

	swap = 1;

	/*
	 BITMAP *palt = new_bitmap(16, 16, "Palette");

	 for (i = 0; i < 16; i ++)
	 {
	  for (j = 0; j < 16; j ++)
	  {
	   putpixel(palt, i, j, i + j * 16);
	  }
	 }

	 save_bitmap("palt.bmp", palt, palet [0]);
	*/
	special_create_color_table (&trans_table);	//, palet);

//   pork_create_color_table2(&trans_table2, palet);

	color_map = &trans_table;

//   set_palette(palet2);

}

void create_outer_bullet_colour (int pa, int index, int r, int g, int b)
{
	int i;

	for (i = 0; i < 13; i++)
	{
		palet[pa][13 + 26 * index + i + 1].r =
		    limit_colour (palet[pa][i + 1].r / 4 + r * 4);
		palet[pa][13 + 26 * index + i + 1].g =
		    limit_colour (palet[pa][i + 1].g / 4 + g * 4);
		palet[pa][13 + 26 * index + i + 1].b =
		    limit_colour (palet[pa][i + 1].b / 4 + b * 4);
		/*  palet [pa] [13 + 26 * index + i + 1].r = limit_colour(palet [pa] [i + 1].r / 4 + r * 4);
		  palet [pa] [13 + 26 * index + i + 1].g = limit_colour(palet [pa] [i + 1].g / 4 + g * 4);
		  palet [pa] [13 + 26 * index + i + 1].b = limit_colour(palet [pa] [i + 1].b / 4 + b * 4);*/
	}

}

void create_inner_bullet_colour (int pa, int index, int r, int g, int b)
{
	int i;

	for (i = 0; i < 13; i++)
	{
		palet[pa][26 + 26 * index + i + 1].r =
		    limit_colour (palet[pa][i + 1].r / 1.5 + r * 1.5);
		palet[pa][26 + 26 * index + i + 1].g =
		    limit_colour (palet[pa][i + 1].g / 1.5 + g * 1.5);
		palet[pa][26 + 26 * index + i + 1].b =
		    limit_colour (palet[pa][i + 1].b / 1.5 + b * 1.5);
		/*  palet [pa] [26 + 26 * index + i + 1].r = limit_colour(palet [pa] [i + 1].r + r);
		  palet [pa] [26 + 26 * index + i + 1].g = limit_colour(palet [pa] [i + 1].g + g);
		  palet [pa] [26 + 26 * index + i + 1].b = limit_colour(palet [pa] [i + 1].b + b);*/
	}


}

// base currently just used for transparencies
int build_new_palette (int base, int rshift, int gshift, int bshift)
{

	int k;

	for (k = 5; k < 14; k++)
	{
		/*    palet [swap] [k].r = base_colours [base] [k - 5] [0];
		    palet [swap] [k].g = base_colours [base] [k - 5] [1];
		    palet [swap] [k].b = base_colours [base] [k - 5] [2];*/
		palet[swap][k].r = current_base[k - 5][0];
		palet[swap][k].g = current_base[k - 5][1];
		palet[swap][k].b = current_base[k - 5][2];
	}

	switch (base)
	{
	default:
	case 1:

// shield1
		create_inner_bullet_colour (swap, 0, 0, 0, 10);
		create_outer_bullet_colour (swap, 0, 10, 20, 30);

// shield2
		create_inner_bullet_colour (swap, 1, 10, 10, 20);
		create_outer_bullet_colour (swap, 1, 15, 25, 35);

// orange
		create_inner_bullet_colour (swap, 2, 10, 5, 0);
		create_outer_bullet_colour (swap, 2, 30, 13, 5);

// yellow
		create_inner_bullet_colour (swap, 3, 6, 6, 0);
		create_outer_bullet_colour (swap, 3, 25, 25, 5);

// red
		create_inner_bullet_colour (swap, 4, 12, 0, 0);
		create_outer_bullet_colour (swap, 4, 30, 20, 10);

// purple
		create_inner_bullet_colour (swap, 5, 7, 0, 7);
		create_outer_bullet_colour (swap, 5, 20, 13, 20);

// blue
		create_inner_bullet_colour (swap, 6, 0, 0, 12);
		create_outer_bullet_colour (swap, 6, 10, 20, 30);

// turquoise
		create_inner_bullet_colour (swap, 7, 0, 3, 3);
		create_outer_bullet_colour (swap, 7, 2, 12, 12);

// green
		create_inner_bullet_colour (swap, 8, 0, 12, 0);
		create_outer_bullet_colour (swap, 8, 11, 20, 11);

		break;
	case 2:

// shield1
		create_inner_bullet_colour (swap, 0, 5, 0, 0);
		create_outer_bullet_colour (swap, 0, 20, 10, 5);

// shield2
		create_inner_bullet_colour (swap, 1, 7, 3, 0);
		create_outer_bullet_colour (swap, 1, 22, 8, 3);

// orange
		create_inner_bullet_colour (swap, 2, 10, 5, 0);
		create_outer_bullet_colour (swap, 2, 30, 13, 5);

// yellow
		create_inner_bullet_colour (swap, 3, 6, 6, 0);
		create_outer_bullet_colour (swap, 3, 25, 25, 5);

// red
		create_inner_bullet_colour (swap, 4, 9, 0, 5);
		create_outer_bullet_colour (swap, 4, 18, 11, 18);

// purple
		create_inner_bullet_colour (swap, 5, 5, 0, 10);
		create_outer_bullet_colour (swap, 5, 20, 13, 20);

// blue
		create_inner_bullet_colour (swap, 6, 0, 0, 15);
		create_outer_bullet_colour (swap, 6, 10, 20, 30);

// turquoise
		create_inner_bullet_colour (swap, 7, 0, 7, 0);
		create_outer_bullet_colour (swap, 7, 8, 17, 8);

// green
		create_inner_bullet_colour (swap, 8, 2, 7, 2);
		create_outer_bullet_colour (swap, 8, 11, 20, 11);

		/*
		// yellow
		    create_inner_bullet_colour(swap, 3, 6, 6, 0);
		    create_outer_bullet_colour(swap, 3, 25, 25, 5);

		// red
		    create_inner_bullet_colour(swap, 4, 12, 0, 0);
		    create_outer_bullet_colour(swap, 4, 30, 20, 10);

		// purple
		    create_inner_bullet_colour(swap, 5, 7, 0, 7);
		    create_outer_bullet_colour(swap, 5, 20, 13, 20);

		// blue
		    create_inner_bullet_colour(swap, 6, 0, 0, 12);
		    create_outer_bullet_colour(swap, 6, 10, 20, 30);

		// turquoise
		    create_inner_bullet_colour(swap, 7, 0, 3, 3);
		    create_outer_bullet_colour(swap, 7, 4, 17, 17);

		// green
		    create_inner_bullet_colour(swap, 8, 0, 12, 0);
		    create_outer_bullet_colour(swap, 8, 11, 20, 11);
		*/
		break;
	case 3:
// shield1
		create_inner_bullet_colour (swap, 0, 0, 0, 10);
		create_outer_bullet_colour (swap, 0, 5, 8, 10);

// shield2
		create_inner_bullet_colour (swap, 1, 5, 0, 5);
		create_outer_bullet_colour (swap, 1, 10, 6, 10);
		/*
		// shield1
		    create_inner_bullet_colour(swap, 0, 0, 0, 10);
		    create_outer_bullet_colour(swap, 0, 7, 15, 25);

		// shield2
		    create_inner_bullet_colour(swap, 1, 5, 0, 5);
		    create_outer_bullet_colour(swap, 1, 18, 9, 18);
		*/

// orange
		create_inner_bullet_colour (swap, 2, 10, 5, 0);
		create_outer_bullet_colour (swap, 2, 30, 13, 5);

// yellow
		create_inner_bullet_colour (swap, 3, 6, 6, 0);
		create_outer_bullet_colour (swap, 3, 25, 25, 5);

// red
		create_inner_bullet_colour (swap, 4, 4, -5, -6);
		create_outer_bullet_colour (swap, 4, 28, 20, 15);

// purple
		create_inner_bullet_colour (swap, 5, 5, -5, 5);
		create_outer_bullet_colour (swap, 5, 16, 8, 16);

// blue
		create_inner_bullet_colour (swap, 6, 4, 4, 12);
		create_outer_bullet_colour (swap, 6, 10, 20, 30);

// turquoise
		create_inner_bullet_colour (swap, 7, 3, 3, 6);
		create_outer_bullet_colour (swap, 7, 9, 17, 17);

// green
		create_inner_bullet_colour (swap, 8, 7, 12, 7);
		create_outer_bullet_colour (swap, 8, 19, 20, 19);

		break;
	}


	if (swap == 0)
		swap = 1;
	else
	{
		swap = 0;
		return 1;
	}

	return 0;

}



/*
void halfway_colour(int base, int target, int r, int g, int b)
{

    int diff;

    diff = palet[base].r - r;
    diff /= 2;
    palet[target].r += diff;

    diff = palet[base].g - g;
    diff /= 2;
    palet[target].g += diff;

    diff = palet[base].b - b;
    diff /= 2;
    palet[target].b += diff;

}
*/

int limit_colour (int colour_input)
{

	if (colour_input < 0)
		return 0;
	if (colour_input > 63)
		return 63;
	return colour_input;

}

/*
int blend_function(int trans, int base, RGB *rgbl)
{

 if (base <= COLOUR_11)
  return trans + base;

// int low_colour = base % 12;
// int low_trans = trans / 12;
 int trans_strength = (trans / 12 - 1) % 6;

 if (trans == TRANS_PURPLE1)
  trans_strength = 10;
 if (trans == TRANS_PURPLE2)
  trans_strength = 11;

 int base_trans_strength = (base / 12 - 1) % 6;
 if (trans == TRANS_PURPLE1)
  base_trans_strength = 10;
 if (trans == TRANS_PURPLE2)
  base_trans_strength = 11;

 if (trans_strength >= base_trans_strength)
  return trans + (base % 12);
   else return base;

 return trans;

}
*/
/*

THIS ONE!!!

int blend_function(int trans, int base, RGB *rgbl)
{

 if (trans >= TRANS_CONVERT_1 && trans <= TRANS_CONVERT_9)
 {
  if (base == 0)
   return 0;

  if (((base - 2) / 13) % 2 == 1)
   return (trans - TRANS_CONVERT_1) * 26 + 15;
    else
     return (trans - TRANS_CONVERT_1) * 26 + 28;

 }

 if (trans == CONVERT_WHITE_TO_GREY)
 {
  if (base == COL_WHITE)
   return COL_LGREY;
  return base;
 }

 if (trans == TRANS_BACK2_OUTLINE)
 {
      return base;
 }

 if (trans == TRANS_BACK3_OUTLINE)
 {
  if (base == COL_BACK1)//2 || base == COL_BACK1)
    return COL_OUTLINE;
     else
      return base;
 }

 if (base < TRANS_SH1_OUT - 1)
  return trans + base - 2;

 int trans_strength = (trans) / 13;


// int base_trans_strength = (base - 1) / 13;
 int base_trans_strength = (base - 1) / 13;

 if (trans_strength > base_trans_strength)
  return trans + ((base - 1) % 13) - 1;
   else return base;

 return trans;

}

*/
int blend_function (int trans, int base, RGB * rgbl)
{

	if (trans >= TRANS_CONVERT_0 && trans <= TRANS_CONVERT_8)
	{
		if (base == 0)
			return 0;

		if (((base - 1) / 13) % 2 == 1)
			return (trans - TRANS_CONVERT_0) * 26 + 15;
		else
			return (trans - TRANS_CONVERT_0) * 26 + 28;

	}

	if (trans == CONVERT_WHITE_TO_GREY)
	{
		if (base == COL_WHITE)
			return COL_LGREY;
		return base;
	}

	if (trans == TRANS_BACK2_OUTLINE)
	{
		return base;
	}

	if (trans == TRANS_BACK3_OUTLINE)
	{
		if (base == COL_BACK1)	//2 || base == COL_BACK1)
			return COL_OUTLINE;
		else
			return base;
	}

// if (base == COL_OUTLINE)
//  base = COL_DGREY;

	if (base < TRANS_SH1_OUT - 1)	// corrected recently
		return trans + base - 2;

	int trans_strength = (trans - 1) / 13;


	int base_trans_strength = (base - 1) / 13;
// int base_trans_strength = (base - 1) / 13;

	if (trans_strength > base_trans_strength)
		return trans + ((base - 1) % 13) - 1;
	else
		return base;

	return trans;

}



/*int blend_function(int trans, int base, RGB *rgbl)
{

 if (base <= COLOUR_12)
  return trans + base;

 trans --;

// int low_colour = base % 12;
// int low_trans = trans / 12;
 int trans_strength = (trans / 12 - 1) % 6;

 if (trans == TRANS_PURPLE1)
  trans_strength = 10;
 if (trans == TRANS_PURPLE2)
  trans_strength = 11;

 int base_trans_strength = (base / 12 - 1) % 6;
 if (trans == TRANS_PURPLE1)
  base_trans_strength = 10;
 if (trans == TRANS_PURPLE2)
  base_trans_strength = 11;

 if (trans_strength >= base_trans_strength)
  return trans + (base % 12);
   else return base;

 return trans;

}
*/
/*
This function had to be modified from the allegro create_color_table
because the allegro version used bestfit_color, whereas we need
specific color values
*/
void special_create_color_table (COLOR_MAP * table)
{
	int x, y, z;
	RGB c;

	for (x = 0; x < PAL_SIZE; x++)
	{
		for (y = 0; y < PAL_SIZE; y++)
		{
			z = blend_function (x, y, &c);

			table->data[x][y] = z;
		}

	}
}
