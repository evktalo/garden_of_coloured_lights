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


#include "data.h"

#include "allegro.h"

#include <math.h>

#include "globvars.h"
#include "palette.h"
#include "stuff.h"
#include "display_init.h"

enum
{
	FLIP_NONE,
	FLIP_H,
	FLIP_V,
	FLIP_VH
};

extern volatile unsigned char ticked;

extern BITMAP *display;
//extern RGB palet [256];
extern RGB palet [1] [256];

extern RLE_SPRITE *icon_RLEs [30]; // in menu.c

extern RLE_SPRITE *player_icon_RLE;

extern RLE_SPRITE *platform_RLE [PLATFORM_RLES];
extern RLE_SPRITE *island1_RLE [ISLAND1_RLES];
extern RLE_SPRITE *bone_RLE [BONE_RLES];
extern RLE_SPRITE *eRLE_small1 [S_ENEMY_RLES];
extern RLE_SPRITE *eRLE_large1 [L_ENEMY_RLES];

extern struct RLE_STRUCT eRLE_flower [5] [50];

extern struct RLE_STRUCT dart_bullet [SMALL_ROTATIONS] [9];
extern struct RLE_STRUCT diamond_bullet [SMALL_ROTATIONS] [9];
//extern struct RLE_STRUCT diamond_bullet [SMALL_ROTATIONS] [9];
extern struct RLE_STRUCT sdart_bullet [SMALL_ROTATIONS] [9];

extern RLE_SPRITE *trans_RLE [TRANS_RLES];

extern RLE_SPRITE *RLE_circle [RLE_CIRCLES] [9];
extern struct RLE_STRUCT RLE_mg_shock [10];

extern struct RLE_STRUCT RLE_blade [SMALL_ROTATIONS];

extern struct RLE_STRUCT RLE_multi [WPN_TYPES] [8];

extern struct RLE_STRUCT petal1 [SMALL_ROTATIONS];
extern struct RLE_STRUCT petal2 [SMALL_ROTATIONS];
extern struct RLE_STRUCT petal3 [SMALL_ROTATIONS];

extern struct RLE_STRUCT RLE_beam [SMALL_ROTATIONS];

extern BITMAP *underlay;

extern struct RLE_STRUCT RLE_mshock [4] [20];
extern struct RLE_STRUCT RLE_lshock [4] [50];
extern struct RLE_STRUCT RLE_hshock [4] [50];
extern struct RLE_STRUCT RLE_lwbeamshock [15];
extern struct RLE_STRUCT RLE_ring [20];

extern struct RLE_STRUCT RLE_rshock [5] [20];

char filename_buffer [DATADIR_SIZE];

/*

Basic display init functions.

*/

/** Call during start-up.
 */
void prepare_display(void)
{

	/* int col [3] [3];

	 col [0] [0] = 5;
	 col [0] [1] = 0;
	 col [0] [2] = 0;

	 col [1] [0] = 0;
	 col [1] [1] = 5;
	 col [1] [2] = 5;

	 col [2] [0] = 0;
	 col [2] [1] = 0;
	 col [2] [2] = 5;*/

	define_palette();

	clear_to_color(screen, COL_OUTLINE);

	/* Preparing to load GFX datafile  */
	strncpy(filename_buffer, data_directory, sizeof(char) * DATADIR_SIZE);
	strncat(filename_buffer, "gfx/garden.dat", sizeof(char) * DATADIR_SIZE);
	DATAFILE *datf = load_datafile(filename_buffer);
	if (datf == NULL)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Error: Couldn't find data.dat (expected it at %s)!", filename_buffer );
		allegro_message("");
		exit(1);
	}
// int i, j;
	/* we want fonts from it */
	font = (FONT *)datf[0].dat;

	/*

	splash bitmap

	*/

	/* Preparing to display the splash screen... */
	RGB splash_palette [256];
	memset(& splash_palette, 0, sizeof(splash_palette));
	/* notice the hardcoded path to splash bitmap */
	strncpy(filename_buffer, data_directory, sizeof(char) * DATADIR_SIZE);
	strncat(filename_buffer, "gfx/splash.bmp", sizeof(char) * DATADIR_SIZE);
	BITMAP *splash_bitmap = load_bitmap(filename_buffer, splash_palette);

	if (splash_bitmap == NULL)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Failed to load in bitmap! (File not found?) %s", filename_buffer);
		exit(1);
	}

	/* display the splash stuff! */
	vsync();
	set_palette(splash_palette);

	blit(splash_bitmap, screen, 0, 0, 0, 0, 640, 480);

	/* We are going to leave the screen alone for a while, so it is needed no longer */
	destroy_bitmap(splash_bitmap);

	/*
	 * SOME COMMENTED CODE USED ANY MORE WAS HERE AND WERE REMOVED...
	 * CHECK THE SVN REPOSITORY IF YOU MISS THEM
	 */
	/*
	 * SOME IFDEFS THAT DIDN'T SEEM TO BE USED WERE HERE AND WERE REMOVED...
	 * CHECK THE SVN REPOSITORY IF YOU MISS THEM
	 */
	/*
	 * SOME COMMENTED CODE USED ANY MORE WAS HERE AND WERE REMOVED...
	 * CHECK THE SVN REPOSITORY IF YOU MISS THEM
	 */

	drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

	/* Looks like sophisticated stuff is going on here...  */
	//prepare_various_effects();
	prepare_trans_rles();
	prepare_s_enemy_rles();
	prepare_l_enemy_rles();
	prepare_enemy_bullets();
	prepare_platform_rles();
	prepare_flowers();
	prepare_lwbeamwaves();
	prepare_mshockwaves();
	prepare_rshockwaves();
	prepare_lshockwaves();
	prepare_hshockwaves();
	prepare_blades();
	prepare_multi_rles();
	prepare_rings();
	prepare_petals();
//prepare_icon_rles();


	display = new_bitmap(640, 480, "Display");
// clear_to_color(display, COL_OUTLINE);

// clear_bitmap(screen);
	/*
	  int i;

	 for (i = 0; i < 50; i ++)
	 {
	     rectfill(screen, i*22, 10, i* 22 + 8, 20, i);
	     rect(screen, i*22, 10, i* 22 + 8, 20, COL_WHITE);
	     textprintf_centre_ex(screen, font, i * 22 + 4, 25, -1, -1, "%i", i);

	 }

	 do { i ++; } while (key [KEY_SPACE] == 0);
	*/

}

void prepare_s_enemy_rles(void)
{
	strncpy(filename_buffer, data_directory, sizeof(char) * DATADIR_SIZE);
	strncat(filename_buffer, "gfx/small.bmp", sizeof(char) * DATADIR_SIZE);
	BITMAP *file_bitmap = load_up_bitmap(filename_buffer);

	fix_outline(file_bitmap);

	eRLE_small1 [S_ENEMY_SWOOPER1_1] = extract_flip_rle_sprite(file_bitmap, 1, 1, 31, 32, FLIP_NONE);
	eRLE_small1 [S_ENEMY_SWOOPER1_2] = extract_flip_rle_sprite(file_bitmap, 33, 1, 33, 32, FLIP_NONE);
	eRLE_small1 [S_ENEMY_SWOOPER1_3] = extract_flip_rle_sprite(file_bitmap, 67, 1, 35, 32, FLIP_NONE);
	eRLE_small1 [S_ENEMY_SWOOPER1_4] = extract_flip_rle_sprite(file_bitmap, 103, 1, 37, 34, FLIP_NONE);

	eRLE_small1 [S_ENEMY_DARTER1_MAIN] = extract_flip_rle_sprite(file_bitmap, 141, 1, 49, 51, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DARTER1_CENTRE] = extract_flip_rle_sprite(file_bitmap, 219, 1, 27, 28, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DARTER1_TOP] = extract_flip_rle_sprite(file_bitmap, 191, 1, 27, 14, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DARTER1_INNER_L] = extract_flip_rle_sprite(file_bitmap, 192, 16, 14, 41, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DARTER1_OUTER_L] = extract_flip_rle_sprite(file_bitmap, 207, 16, 9, 31, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DARTER1_INNER_R] = extract_flip_rle_sprite(file_bitmap, 192, 16, 14, 41, FLIP_H);
	eRLE_small1 [S_ENEMY_DARTER1_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 207, 16, 9, 31, FLIP_H);

	player_icon_RLE = extract_flip_rle_sprite(file_bitmap, 277, 38, 23, 31, FLIP_NONE);

	eRLE_small1 [S_ENEMY_PLAYER] = extract_flip_rle_sprite(file_bitmap, 219, 30, 21, 18, FLIP_NONE);
	eRLE_small1 [S_ENEMY_PLAYER_UP_L] = extract_flip_rle_sprite(file_bitmap, 242, 32, 10, 8, FLIP_NONE);
	eRLE_small1 [S_ENEMY_PLAYER_UP_R] = extract_flip_rle_sprite(file_bitmap, 242, 32, 10, 8, FLIP_H);
	eRLE_small1 [S_ENEMY_PLAYER_DOWN_L] = extract_flip_rle_sprite(file_bitmap, 242, 41, 9, 20, FLIP_NONE);
	eRLE_small1 [S_ENEMY_PLAYER_DOWN_R] = extract_flip_rle_sprite(file_bitmap, 242, 41, 9, 20, FLIP_H);
	eRLE_small1 [S_ENEMY_PLAYER1] = extract_flip_rle_sprite(file_bitmap, 249, 1, 15, 10, FLIP_NONE);
	eRLE_small1 [S_ENEMY_PLAYER2] = extract_flip_rle_sprite(file_bitmap, 265, 1, 17, 11, FLIP_NONE);
	eRLE_small1 [S_ENEMY_PLAYER3] = extract_flip_rle_sprite(file_bitmap, 283, 1, 19, 12, FLIP_NONE);

	eRLE_small1 [S_ENEMY_DIVER1_CENTRE] = extract_flip_rle_sprite(file_bitmap, 325, 35, 35, 43, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DIVER1_INNER_L] = extract_flip_rle_sprite(file_bitmap, 314, 35, 10, 42, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DIVER1_OUTER_L] = extract_flip_rle_sprite(file_bitmap, 304, 37, 9, 13, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DIVER1_INNER_R] = extract_flip_rle_sprite(file_bitmap, 314, 35, 10, 42, FLIP_H);
	eRLE_small1 [S_ENEMY_DIVER1_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 304, 37, 9, 13, FLIP_H);

	eRLE_small1 [S_ENEMY_C1P1_CORE] = extract_flip_rle_sprite(file_bitmap, 368, 38, 25, 25, FLIP_NONE);
	eRLE_small1 [S_ENEMY_C1P1_TOP] = extract_flip_rle_sprite(file_bitmap, 395, 38, 17, 5, FLIP_NONE);
	eRLE_small1 [S_ENEMY_C1P1_BOTTOM1] = extract_flip_rle_sprite(file_bitmap, 395, 57, 23, 6, FLIP_NONE);
	eRLE_small1 [S_ENEMY_C1P1_BOTTOM2] = extract_flip_rle_sprite(file_bitmap, 395, 64, 17, 5, FLIP_NONE);
	eRLE_small1 [S_ENEMY_C1P1_MIDL] = extract_flip_rle_sprite(file_bitmap, 395, 44, 13, 12, FLIP_NONE);
	eRLE_small1 [S_ENEMY_C1P1_MIDR] = extract_flip_rle_sprite(file_bitmap, 395, 44, 13, 12, FLIP_H);

	eRLE_small1 [S_ENEMY_SWOOPER2_1] = extract_flip_rle_sprite(file_bitmap, 458, 47, 41, 43, FLIP_NONE);
	eRLE_small1 [S_ENEMY_SWOOPER2_2] = extract_flip_rle_sprite(file_bitmap, 500, 47, 43, 42, FLIP_NONE);
	eRLE_small1 [S_ENEMY_SWOOPER2_3] = extract_flip_rle_sprite(file_bitmap, 544, 47, 45, 41, FLIP_NONE);
	eRLE_small1 [S_ENEMY_SWOOPER2_4] = extract_flip_rle_sprite(file_bitmap, 590, 47, 47, 41, FLIP_NONE);

	eRLE_small1 [S_ENEMY_DARTER2_CORE] = extract_flip_rle_sprite(file_bitmap, 689, 9, 25, 25, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DARTER2_TOP] = extract_flip_rle_sprite(file_bitmap, 675, 5, 13, 21, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DARTER2_BOTTOM] = extract_flip_rle_sprite(file_bitmap, 675, 31, 13, 16, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DARTER2_INNER_L] = extract_flip_rle_sprite(file_bitmap, 660, 2, 13, 48, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DARTER2_OUTER_L] = extract_flip_rle_sprite(file_bitmap, 638, 2, 16, 48, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DARTER2_INNER_R] = extract_flip_rle_sprite(file_bitmap, 660, 2, 13, 48, FLIP_H);
	eRLE_small1 [S_ENEMY_DARTER2_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 638, 2, 16, 48, FLIP_H);

	eRLE_small1 [S_ENEMY_SMALL_OUTER_L] = extract_flip_rle_sprite(file_bitmap, 544, 3, 15, 29, FLIP_NONE);
	eRLE_small1 [S_ENEMY_SMALL_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 544, 3, 15, 29, FLIP_H);
	eRLE_small1 [S_ENEMY_SMALL_CORE1] = extract_flip_rle_sprite(file_bitmap, 561, 2, 15, 19, FLIP_NONE);
	eRLE_small1 [S_ENEMY_SMALL_CORE2] = extract_flip_rle_sprite(file_bitmap, 561, 21, 15, 19, FLIP_NONE);

	eRLE_small1 [S_ENEMY_LARGER_OUTER_L] = extract_flip_rle_sprite(file_bitmap, 578, 4, 18, 35, FLIP_NONE);
	eRLE_small1 [S_ENEMY_LARGER_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 578, 4, 18, 35, FLIP_H);
	eRLE_small1 [S_ENEMY_LARGER_CORE1] = extract_flip_rle_sprite(file_bitmap, 597, 2, 21, 18, FLIP_NONE);
	eRLE_small1 [S_ENEMY_LARGER_CORE2] = extract_flip_rle_sprite(file_bitmap, 597, 20, 21, 18, FLIP_NONE);

	eRLE_small1 [S_ENEMY_WINGS_OUTER_L] = extract_flip_rle_sprite(file_bitmap, 716, 4, 22, 43, FLIP_NONE);
	eRLE_small1 [S_ENEMY_WINGS_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 716, 4, 22, 43, FLIP_H);
	eRLE_small1 [S_ENEMY_WINGS_CORE1] = extract_flip_rle_sprite(file_bitmap, 739, 3, 23, 32, FLIP_NONE);
	eRLE_small1 [S_ENEMY_WINGS_CORE2] = extract_flip_rle_sprite(file_bitmap, 739, 36, 23, 32, FLIP_NONE);

	eRLE_small1 [S_ENEMY_MEGA_OUTER_L] = extract_flip_rle_sprite(file_bitmap, 776, 4, 39, 76, FLIP_NONE);
	eRLE_small1 [S_ENEMY_MEGA_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 776, 4, 39, 76, FLIP_H);
	eRLE_small1 [S_ENEMY_MEGA_CORE] = extract_flip_rle_sprite(file_bitmap, 816, 4, 43, 62, FLIP_NONE);

	eRLE_small1 [S_ENEMY_BOSS3_CORE] = extract_flip_rle_sprite(file_bitmap, 1030, 1, 43, 62, FLIP_NONE);

	eRLE_small1 [S_ENEMY_DRIFTER1_CORE] = extract_flip_rle_sprite(file_bitmap, 638, 53, 39, 31, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DRIFTER1_L] = extract_flip_rle_sprite(file_bitmap, 678, 64, 20, 19, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DRIFTER1_R] = extract_flip_rle_sprite(file_bitmap, 678, 64, 20, 19, FLIP_H);

	eRLE_small1 [S_ENEMY_BEAMER_CORE] = extract_flip_rle_sprite(file_bitmap, 862, 12, 39, 42, FLIP_NONE);
	eRLE_small1 [S_ENEMY_BEAMER_L] = extract_flip_rle_sprite(file_bitmap, 901, 19, 35, 42, FLIP_NONE);
	eRLE_small1 [S_ENEMY_BEAMER_R] = extract_flip_rle_sprite(file_bitmap, 901, 19, 35, 42, FLIP_H);
	eRLE_small1 [S_ENEMY_BEAMER_U] = extract_flip_rle_sprite(file_bitmap, 904, 3, 23, 15, FLIP_NONE);
	eRLE_small1 [S_ENEMY_BEAMER_D] = extract_flip_rle_sprite(file_bitmap, 873, 56, 19, 12, FLIP_NONE);

	/*    eRLE_small1 [S_ENEMY_RISER1_CORE] = extract_flip_rle_sprite(file_bitmap, 911, 69, 11, 15, FLIP_NONE);
	    eRLE_small1 [S_ENEMY_RISER1_L] = extract_flip_rle_sprite(file_bitmap, 894, 63, 16, 29, FLIP_NONE);
	    eRLE_small1 [S_ENEMY_RISER1_R] = extract_flip_rle_sprite(file_bitmap, 894, 63, 16, 29, FLIP_H);*/

	eRLE_small1 [S_ENEMY_RISER1_CORE] = extract_flip_rle_sprite(file_bitmap, 905, 62, 27, 30, FLIP_NONE);
	eRLE_small1 [S_ENEMY_RISER1_L] = extract_flip_rle_sprite(file_bitmap, 894, 62, 11, 23, FLIP_NONE);
	eRLE_small1 [S_ENEMY_RISER1_R] = extract_flip_rle_sprite(file_bitmap, 894, 62, 11, 23, FLIP_H);

	eRLE_small1 [S_ENEMY_DRIFTER2_CORE] = extract_flip_rle_sprite(file_bitmap, 936, 2, 39, 31, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DRIFTER2_L] = extract_flip_rle_sprite(file_bitmap, 976, 3, 16, 39, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DRIFTER2_R] = extract_flip_rle_sprite(file_bitmap, 976, 3, 16, 39, FLIP_H);

	eRLE_small1 [S_ENEMY_ESCORT_CORE] = extract_flip_rle_sprite(file_bitmap, 934, 57, 41, 25, FLIP_NONE);
	eRLE_small1 [S_ENEMY_ESCORT_L] = extract_flip_rle_sprite(file_bitmap, 977, 58, 16, 25, FLIP_NONE);
	eRLE_small1 [S_ENEMY_ESCORT_R] = extract_flip_rle_sprite(file_bitmap, 977, 58, 16, 25, FLIP_H);

	eRLE_small1 [S_ENEMY_SHOOTER_L] = extract_flip_rle_sprite(file_bitmap, 995, 17, 33, 42, FLIP_NONE);
	eRLE_small1 [S_ENEMY_SHOOTER_R] = extract_flip_rle_sprite(file_bitmap, 995, 17, 33, 42, FLIP_H);
	eRLE_small1 [S_ENEMY_SHOOTER_U] = extract_flip_rle_sprite(file_bitmap, 998, 1, 23, 14, FLIP_NONE);

	eRLE_small1 [S_ENEMY_HOOK3] = extract_flip_rle_sprite(file_bitmap, 1075, 1, 33, 32, FLIP_NONE);
	eRLE_small1 [S_ENEMY_HOOK2] = extract_flip_rle_sprite(file_bitmap, 1109, 1, 33, 32, FLIP_NONE);
	eRLE_small1 [S_ENEMY_HOOK1] = extract_flip_rle_sprite(file_bitmap, 1143, 1, 33, 33, FLIP_NONE);

	eRLE_small1 [S_ENEMY_DROPPER1] = extract_flip_rle_sprite(file_bitmap, 1080, 35, 29, 43, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DROPPER2] = extract_flip_rle_sprite(file_bitmap, 1110, 34, 29, 44, FLIP_NONE);
	eRLE_small1 [S_ENEMY_DROPPER3] = extract_flip_rle_sprite(file_bitmap, 1140, 34, 29, 45, FLIP_NONE);

	eRLE_small1 [S_ENEMY_RING1] = extract_flip_rle_sprite(file_bitmap, 1177, 1, 49, 49, FLIP_NONE);
	eRLE_small1 [S_ENEMY_RING2] = extract_flip_rle_sprite(file_bitmap, 1227, 0, 51, 51, FLIP_NONE);
	eRLE_small1 [S_ENEMY_RING3] = extract_flip_rle_sprite(file_bitmap, 1279, 0, 53, 53, FLIP_NONE);

	eRLE_small1 [S_ENEMY_HUNTER3] = extract_flip_rle_sprite(file_bitmap, 1333, 1, 57, 51, FLIP_NONE);
	eRLE_small1 [S_ENEMY_HUNTER2] = extract_flip_rle_sprite(file_bitmap, 1391, 0, 57, 52, FLIP_NONE);
	eRLE_small1 [S_ENEMY_HUNTER1] = extract_flip_rle_sprite(file_bitmap, 1449, 0, 57, 53, FLIP_NONE);

	eRLE_small1 [S_ENEMY_HOOK2_3] = extract_flip_rle_sprite(file_bitmap, 1170, 56, 35, 35, FLIP_NONE);
	eRLE_small1 [S_ENEMY_HOOK2_2] = extract_flip_rle_sprite(file_bitmap, 1206, 56, 35, 35, FLIP_NONE);
	eRLE_small1 [S_ENEMY_HOOK2_1] = extract_flip_rle_sprite(file_bitmap, 1242, 56, 35, 35, FLIP_NONE);


	destroy_bitmap(file_bitmap);
	file_bitmap = NULL;
}
/*
void prepare_icon_rles(void)
{

 strncpy(filename_buffer, data_directory, sizeof(char) * DATADIR_SIZE);
 strncat(filename_buffer, "gfx/icons.bmp", sizeof(char) * DATADIR_SIZE);
 BITMAP *file_bitmap = load_up_bitmap(filename_buffer);

 fix_outline(file_bitmap);
  int i;

  for (i = 0; i < 11; i ++)
  {

    icon_RLEs [i] = extract_flip_rle_sprite(file_bitmap, 1 + i * 41, 1, 40, 40, FLIP_NONE);

  }

    destroy_bitmap(file_bitmap);

}
*/
//#define FIXING

void prepare_l_enemy_rles(void)
{
	/*
	 * SOME IFDEFS THAT DIDN'T SEEM TO BE USED WERE HERE AND WERE REMOVED...
	 */
	strncpy(filename_buffer, data_directory, sizeof(char) * DATADIR_SIZE);
	strncat(filename_buffer, "gfx/large.bmp", sizeof(char) * DATADIR_SIZE);
	BITMAP *file_bitmap = load_up_bitmap(filename_buffer);

	fix_outline(file_bitmap);


	eRLE_large1 [L_ENEMY_STOPPER1] = extract_flip_rle_sprite(file_bitmap, 1, 1, 93, 146, FLIP_NONE);

	eRLE_large1 [L_ENEMY_STOPPER1_FRONT] = extract_flip_rle_sprite(file_bitmap, 100, 1, 43, 20, FLIP_NONE);
	eRLE_large1 [L_ENEMY_STOPPER1_CENTRE] = extract_flip_rle_sprite(file_bitmap, 130, 100, 91, 95, FLIP_NONE);
	eRLE_large1 [L_ENEMY_STOPPER1_F_L] = extract_flip_rle_sprite(file_bitmap, 100, 23, 27, 29, FLIP_NONE);
	eRLE_large1 [L_ENEMY_STOPPER1_F_R] = extract_flip_rle_sprite(file_bitmap, 100, 23, 27, 29, FLIP_H);
	eRLE_large1 [L_ENEMY_STOPPER1_M_L] = extract_flip_rle_sprite(file_bitmap, 97, 53, 33, 86, FLIP_NONE);
	eRLE_large1 [L_ENEMY_STOPPER1_M_R] = extract_flip_rle_sprite(file_bitmap, 97, 53, 33, 86, FLIP_H);
	eRLE_large1 [L_ENEMY_STOPPER1_B_L] = extract_flip_rle_sprite(file_bitmap, 132, 21, 22, 77, FLIP_NONE);
	eRLE_large1 [L_ENEMY_STOPPER1_B_R] = extract_flip_rle_sprite(file_bitmap, 132, 21, 22, 77, FLIP_H);

	eRLE_large1 [L_ENEMY_CARRIER1_CORE] = extract_flip_rle_sprite(file_bitmap, 224, 6, 137, 154, FLIP_NONE);
	eRLE_large1 [L_ENEMY_CARRIER1_TOP1] = extract_flip_rle_sprite(file_bitmap, 366, 1, 65, 12, FLIP_NONE);
	eRLE_large1 [L_ENEMY_CARRIER1_TOP2] = extract_flip_rle_sprite(file_bitmap, 366, 14, 105, 40, FLIP_NONE);
	eRLE_large1 [L_ENEMY_CARRIER1_TOP3] = extract_flip_rle_sprite(file_bitmap, 365, 55, 129, 44, FLIP_NONE);
	eRLE_large1 [L_ENEMY_CARRIER1_MIDL1] = extract_flip_rle_sprite(file_bitmap, 364, 100, 51, 31, FLIP_NONE);
	eRLE_large1 [L_ENEMY_CARRIER1_MIDL2] = extract_flip_rle_sprite(file_bitmap, 423, 99, 49, 31, FLIP_NONE);
	eRLE_large1 [L_ENEMY_CARRIER1_MIDR1] = extract_flip_rle_sprite(file_bitmap, 364, 100, 51, 31, FLIP_H);
	eRLE_large1 [L_ENEMY_CARRIER1_MIDR2] = extract_flip_rle_sprite(file_bitmap, 423, 99, 49, 31, FLIP_H);
	eRLE_large1 [L_ENEMY_CARRIER1_BOTTOM1] = extract_flip_rle_sprite(file_bitmap, 367, 131, 125, 37, FLIP_NONE);
	eRLE_large1 [L_ENEMY_CARRIER1_BOTTOM2] = extract_flip_rle_sprite(file_bitmap, 378, 168, 93, 31, FLIP_NONE);

	eRLE_large1 [L_ENEMY_POD1_CORE] = extract_flip_rle_sprite(file_bitmap, 555, 101, 65, 94, FLIP_NONE);
	eRLE_large1 [L_ENEMY_POD1_BODYL] = extract_flip_rle_sprite(file_bitmap, 500, 48, 53, 90, FLIP_NONE);
	eRLE_large1 [L_ENEMY_POD1_BODYR] = extract_flip_rle_sprite(file_bitmap, 500, 48, 53, 90, FLIP_H);
	eRLE_large1 [L_ENEMY_POD1_TOPC] = extract_flip_rle_sprite(file_bitmap, 528, 139, 19, 36, FLIP_NONE);
	eRLE_large1 [L_ENEMY_POD1_TOPL] = extract_flip_rle_sprite(file_bitmap, 500, 139, 22, 44, FLIP_NONE);
	eRLE_large1 [L_ENEMY_POD1_TOPR] = extract_flip_rle_sprite(file_bitmap, 500, 139, 22, 44, FLIP_H);

	eRLE_large1 [L_ENEMY_CRUISER1] = extract_flip_rle_sprite(file_bitmap, 633, 17, 99, 125, FLIP_NONE);

	eRLE_large1 [L_ENEMY_CRUISER2] = extract_flip_rle_sprite(file_bitmap, 744, 4, 111, 60, FLIP_NONE);

	eRLE_large1 [L_ENEMY_CRUISER3] = extract_flip_rle_sprite(file_bitmap, 748, 68, 119, 120, FLIP_NONE);

	eRLE_large1 [L_ENEMY_CRUISER4] = extract_flip_rle_sprite(file_bitmap, 869, 3, 95, 95, FLIP_NONE);

	eRLE_large1 [L_ENEMY_BOSS1_CORE] = extract_flip_rle_sprite(file_bitmap, 976, 12, 155, 95, FLIP_NONE);
	eRLE_large1 [L_ENEMY_BOSS1_UL] = extract_flip_rle_sprite(file_bitmap, 957, 119, 102, 63, FLIP_NONE);
	eRLE_large1 [L_ENEMY_BOSS1_UR] = extract_flip_rle_sprite(file_bitmap, 957, 119, 102, 63, FLIP_H);
	eRLE_large1 [L_ENEMY_BOSS1_DR] = extract_flip_rle_sprite(file_bitmap, 957, 119, 102, 63, FLIP_VH);
	eRLE_large1 [L_ENEMY_BOSS1_DL] = extract_flip_rle_sprite(file_bitmap, 957, 119, 102, 63, FLIP_V);

	eRLE_large1 [L_ENEMY_BOSS1_OUT_L] = extract_flip_rle_sprite(file_bitmap, 888, 122, 58, 65, FLIP_NONE);
	eRLE_large1 [L_ENEMY_BOSS1_OUT_R] = extract_flip_rle_sprite(file_bitmap, 888, 122, 58, 65, FLIP_H);

	eRLE_large1 [L_ENEMY_BOSS1_U] = extract_flip_rle_sprite(file_bitmap, 1069, 118, 69, 31, FLIP_NONE);
	eRLE_large1 [L_ENEMY_BOSS1_D] = extract_flip_rle_sprite(file_bitmap, 1069, 118, 69, 31, FLIP_V);

	eRLE_large1 [L_ENEMY_BOSS1_L] = extract_flip_rle_sprite(file_bitmap, 1146, 127, 31, 67, FLIP_NONE);
	eRLE_large1 [L_ENEMY_BOSS1_R] = extract_flip_rle_sprite(file_bitmap, 1146, 127, 31, 67, FLIP_H);

	eRLE_large1 [L_ENEMY_BOSS2_CORE] = extract_flip_rle_sprite(file_bitmap, 1255, 36, 55, 73, FLIP_NONE);

	eRLE_large1 [L_ENEMY_BOSS2_IN_L] = extract_flip_rle_sprite(file_bitmap, 1182, 20, 70, 105, FLIP_NONE);
	eRLE_large1 [L_ENEMY_BOSS2_IN_R] = extract_flip_rle_sprite(file_bitmap, 1182, 20, 70, 105, FLIP_H);

	eRLE_large1 [L_ENEMY_BOSS2_OUT_L] = extract_flip_rle_sprite(file_bitmap, 1143, 29, 32, 58, FLIP_NONE);
	eRLE_large1 [L_ENEMY_BOSS2_OUT_R] = extract_flip_rle_sprite(file_bitmap, 1143, 29, 32, 58, FLIP_H);

	eRLE_large1 [L_ENEMY_WALKER2] = extract_flip_rle_sprite(file_bitmap, 1312, 2, 153, 127, FLIP_NONE);
	eRLE_large1 [L_ENEMY_WALKER1] = extract_flip_rle_sprite(file_bitmap, 1466, 2, 153, 127, FLIP_NONE);
	eRLE_large1 [L_ENEMY_WALKER3] = extract_flip_rle_sprite(file_bitmap, 1620, 2, 153, 127, FLIP_NONE);

	eRLE_large1 [L_ENEMY_SPINNER3] = extract_flip_rle_sprite(file_bitmap, 1774, 3, 105, 166, FLIP_NONE);
	eRLE_large1 [L_ENEMY_SPINNER2] = extract_flip_rle_sprite(file_bitmap, 1882, 2, 105, 168, FLIP_NONE);
	eRLE_large1 [L_ENEMY_SPINNER1] = extract_flip_rle_sprite(file_bitmap, 1988, 1, 105, 170, FLIP_NONE);

	eRLE_large1 [L_ENEMY_BOSS4] = extract_flip_rle_sprite(file_bitmap, 2098, 0, 229, 202, FLIP_NONE);


	destroy_bitmap(file_bitmap);
	file_bitmap = NULL;
}

void prepare_trans_rles(void)
{
	strncpy(filename_buffer, data_directory, sizeof(char) * DATADIR_SIZE);
	strncat(filename_buffer, "gfx/trans.bmp", sizeof(char) * DATADIR_SIZE);
	BITMAP *file_bitmap = load_up_bitmap(filename_buffer);

	fix_trans(file_bitmap);

	extract_rle_struct2(file_bitmap, RLE_mg_shock, 1, 1, 1, 1, 0);
	extract_rle_struct2(file_bitmap, RLE_mg_shock, 3, 1, 3, 3, 1);
	extract_rle_struct2(file_bitmap, RLE_mg_shock, 7, 1, 7, 7, 2);
	extract_rle_struct2(file_bitmap, RLE_mg_shock, 15, 1, 9, 9, 3);
	extract_rle_struct2(file_bitmap, RLE_mg_shock, 25, 1, 11, 11, 4);
	extract_rle_struct2(file_bitmap, RLE_mg_shock, 37, 1, 13, 13, 5);
	extract_rle_struct2(file_bitmap, RLE_mg_shock, 51, 1, 17, 17, 6);
	extract_rle_struct2(file_bitmap, RLE_mg_shock, 69, 1, 19, 19, 7);
	extract_rle_struct2(file_bitmap, RLE_mg_shock, 89, 1, 21, 21, 8);

// trans_RLE [RLE_TRANS_MGBULLET1] = extract_flip_rle_sprite(file_bitmap, 1, 1, 5, 23, FLIP_NONE);

	/*    trans_RLE [RLE_TRANS_MGBULLET1] = extract_flip_rle_sprite(file_bitmap, 1, 1, 5, 23, FLIP_NONE);
	    trans_RLE [RLE_TRANS_MGBULLET2] = extract_flip_rle_sprite(file_bitmap, 7, 1, 5, 23, FLIP_NONE);
	    trans_RLE [RLE_TRANS_MGBULLET3] = extract_flip_rle_sprite(file_bitmap, 13, 1, 5, 23, FLIP_NONE);
	    trans_RLE [RLE_TRANS_MGBULLET4] = extract_flip_rle_sprite(file_bitmap, 19, 1, 5, 23, FLIP_NONE);*/

	destroy_bitmap(file_bitmap);
	file_bitmap = NULL;
}

/** Prepares RLEs for enemy bullets */
void prepare_enemy_bullets(void)
{
	int i;//, o;

	BITMAP *temp_bitmap;

	const int char_dim = 50;

	temp_bitmap = new_bitmap(char_dim, char_dim, "prepare_glass_bullets");

	int inc = ANGLE_1 / SMALL_ROTATIONS;
	int angle;
	int col = TRANS_B1_IN, border = TRANS_B1_OUT;

	/* proportions of various types of bullets (used in the following loop) */
	int std_darts[8];
	int std_darts_long = char_dim / 50 * 23;
	int std_darts_short = char_dim / 50 * 11;

	int small_darts[8];
	int small_darts_long = char_dim / 50 * 17;
	int small_darts_short = char_dim / 50 * 7;

	int diamonds[8];
	int diamonds_long = char_dim / 10 * 3;
	int diamonds_short = char_dim / 50 * 9;
	for (i = 0; i < SMALL_ROTATIONS; i ++)
	{
		clear_bitmap(temp_bitmap);
		angle = i * inc;
		/* The following code is going to repeat many times:
		 * 1. We create the characteristic dimensions (before the loop body, upwards)
		 * 2. We set the polygon points
		 * 3. We make a bordered polygon
		 * 4. We put it in a RLE struct */

		/* std darts */
		std_darts[0] = char_dim / 2 + xpart(angle, std_darts_long);
		std_darts[1] = char_dim / 2 + ypart(angle, std_darts_long);
		std_darts[2] = char_dim / 2 + xpart(angle + ANGLE_8, std_darts_short);
		std_darts[3] = char_dim / 2 + ypart(angle + ANGLE_8, std_darts_short);
		std_darts[4] = std_darts[5] = char_dim / 2;
		std_darts[6] = char_dim / 2 + xpart(angle - ANGLE_8, std_darts_short);
		std_darts[7] = char_dim / 2 + ypart(angle - ANGLE_8, std_darts_short);
		fast_bordered_poly(temp_bitmap, 4, std_darts, col, border);
		new_rle_struct_colours(temp_bitmap, "prepare dart bullets", dart_bullet, -1, i, TCOL_B1, TCOL_B5);

		/* small darts */
		clear_bitmap(temp_bitmap);
		angle = i * inc;

		small_darts[0] = char_dim / 2 + xpart(angle, small_darts_long);
		small_darts[1] = char_dim / 2 + ypart(angle, small_darts_long);
		small_darts[2] = char_dim / 2 + xpart(angle + ANGLE_8, small_darts_short);
		small_darts[3] = char_dim / 2 + ypart(angle + ANGLE_8, small_darts_short);
		small_darts[4] = small_darts[5] = char_dim / 2;
		small_darts[6] = char_dim / 2 + xpart(angle - ANGLE_8, small_darts_short);
		small_darts[7] = char_dim / 2 + ypart(angle - ANGLE_8, small_darts_short);
		fast_bordered_poly(temp_bitmap, 4, small_darts, col, border);
		new_rle_struct_colours(temp_bitmap, "prepare sdart bullets", sdart_bullet, -1, i, TCOL_B1, TCOL_B5);

		/* diamond bullets */
		clear_bitmap(temp_bitmap);
		angle = i * inc;
		diamonds[0] = char_dim / 2 + xpart(angle, diamonds_long);
		diamonds[1] = char_dim / 2 + ypart(angle, diamonds_long);
		diamonds[2] = char_dim / 2 + xpart(angle + ANGLE_4, diamonds_short);
		diamonds[3] = char_dim / 2 + ypart(angle + ANGLE_4, diamonds_short);
		diamonds[4] = char_dim / 2 - xpart(angle, diamonds_long);
		diamonds[5] = char_dim / 2 - ypart(angle, diamonds_long);
		diamonds[6] = char_dim / 2 + xpart(angle - ANGLE_4, diamonds_short);
		diamonds[7] = char_dim / 2 + ypart(angle - ANGLE_4, diamonds_short);
		fast_bordered_poly(temp_bitmap, 4, diamonds, col, border);
		new_rle_struct_colours(temp_bitmap, "prepare diamond bullets", diamond_bullet, -1, i, TCOL_B1, TCOL_B5);
	}/* endfor (i = 0; i < SMALL_ROTATIONS; i ++) */

	destroy_bitmap(temp_bitmap);

	/* circles */
	int j;

	for (i = 0; i < RLE_CIRCLES; i ++)
	{
		temp_bitmap = new_bitmap((i + 1) * 2, (i + 1) * 2, "prepare circles");
		clear_bitmap(temp_bitmap);

		for (j = 0; j < 9; j ++)
		{
			circlefill(temp_bitmap, i + 1, i + 1, i, 28 + j * 26);
			circle(temp_bitmap, i + 1, i + 1, i, 15 + j * 26);
			RLE_circle [i] [j] = get_rle_sprite(temp_bitmap);
		}
		destroy_bitmap(temp_bitmap);
	}/* endfor (i = 0; i < RLE_CIRCLES; i ++) */

	/* make beams */
	const int beams_char_dim = (char_dim * 6) / 5;
	temp_bitmap = new_bitmap(beams_char_dim, beams_char_dim, "prepare beams");

	int beams[8];
	const int beams_long = (beams_char_dim * 5) / 12;
	const int beams_short = beams_char_dim / 15;
	for (i = 0; i < SMALL_ROTATIONS; i ++)
	{
		clear_bitmap(temp_bitmap);
		angle = i * inc;
		beams[0] = beams_char_dim / 2 + xpart(angle, beams_long) + xpart(angle + ANGLE_4, beams_short);
		beams[1] = beams_char_dim / 2 + ypart(angle, beams_long) + ypart(angle + ANGLE_4, beams_short);
		beams[2] = beams_char_dim / 2 + xpart(angle, beams_long) + xpart(angle - ANGLE_4, beams_short);
		beams[3] = beams_char_dim / 2 + ypart(angle, beams_long) + ypart(angle - ANGLE_4, beams_short);
		beams[4] = beams_char_dim / 2 - xpart(angle, beams_long) + xpart(angle - ANGLE_4, beams_short);
		beams[5] = beams_char_dim / 2 - ypart(angle, beams_long) + ypart(angle - ANGLE_4, beams_short);
		beams[6] = beams_char_dim / 2 - xpart(angle, beams_long) + xpart(angle + ANGLE_4, beams_short);
		beams[7] = beams_char_dim / 2 - ypart(angle, beams_long) + ypart(angle + ANGLE_4, beams_short);
		fast_bordered_poly(temp_bitmap, 4, beams, TRANS_B1_IN, TRANS_B1_OUT);
		new_rle_struct(temp_bitmap, "beam RLE", &RLE_beam [i], -1);

	}/* endfor (i = 0; i < SMALL_ROTATIONS; i ++) */

	destroy_bitmap(temp_bitmap);
}

void prepare_blades(void)
{
	int i;

	BITMAP *temp_bitmap;

	const int blade_char_dim = 80;
	temp_bitmap = new_bitmap(blade_char_dim, blade_char_dim, "prepare_blades");

	int blade[8];
	const int blade_long = (blade_char_dim * 23) / 80;
	const int blade_short = blade_char_dim / 10;

	int inc = ANGLE_1 / SMALL_ROTATIONS;
	int angle;

	for (i = 0; i < SMALL_ROTATIONS; i ++)
	{

		clear_bitmap(temp_bitmap);
		angle = i * inc;

		blade[0] = blade_char_dim / 2 + xpart(angle, blade_long);
		blade[1] = blade_char_dim / 2 + ypart(angle, blade_long);
		blade[2] = blade_char_dim / 2 - xpart(angle + ANGLE_4 + ANGLE_8, blade_short);
		blade[3] = blade_char_dim / 2 - ypart(angle + ANGLE_4 + ANGLE_8, blade_short);
		blade[4] = blade[5] = blade_char_dim / 2;
		blade[6] = blade_char_dim / 2 - xpart(angle - ANGLE_4 - ANGLE_8, blade_short);
		blade[7] = blade_char_dim / 2 - ypart(angle - ANGLE_4 - ANGLE_8, blade_short);
		fast_bordered_poly(temp_bitmap, 4, blade, TRANS_YELLOW_IN, TRANS_YELLOW_OUT);
		new_rle_struct(temp_bitmap, "blade RLE", &RLE_blade [i], -1);

	}/* endfor (i = 0; i < SMALL_ROTATIONS; i ++) */

	destroy_bitmap(temp_bitmap);
}

void prepare_petals(void)
{
	int small_petal_char_dim = 80;

	BITMAP *temp_bitmap;
	temp_bitmap = new_bitmap(small_petal_char_dim, small_petal_char_dim, "prepare_petals");

	int inc = ANGLE_1 / SMALL_ROTATIONS;
	int angle;

	int petal_1a[8]; 
	const int petal_1a_short = small_petal_char_dim / 10; 
	const int petal_1a_long = (small_petal_char_dim * 23) / 80;

	int petal_1b[8];
	const int petal_1b_nano = small_petal_char_dim / 16; 
	const int petal_1b_short = (small_petal_char_dim * 3) / 20; 
	const int petal_1b_long = (small_petal_char_dim * 33) / 80;

	int i;
	for (i = 0; i < SMALL_ROTATIONS; i ++)
	{
		clear_bitmap(temp_bitmap);
		angle = i * inc;
		petal_1a[0] = small_petal_char_dim / 2 + xpart(angle, petal_1a_long);
		petal_1a[1] = small_petal_char_dim / 2 + ypart(angle, petal_1a_long);
		petal_1a[2] = small_petal_char_dim / 2 - xpart(angle + ANGLE_4 + ANGLE_8, petal_1a_short);
		petal_1a[3] = small_petal_char_dim / 2 - ypart(angle + ANGLE_4 + ANGLE_8, petal_1a_short);
		petal_1a[4] = petal_1a[5] = small_petal_char_dim / 2;
		petal_1a[6] = small_petal_char_dim / 2 - ypart(angle - ANGLE_4 - ANGLE_8, petal_1a_short);
		petal_1a[7] = small_petal_char_dim / 2 - xpart(angle - ANGLE_4 - ANGLE_8, petal_1a_short);
		fast_bordered_poly(temp_bitmap, 4, petal_1a, TRANS_SH1_IN, TRANS_SH1_OUT);

		petal_1b[0] = small_petal_char_dim / 2 + xpart(angle, petal_1b_long);
		petal_1b[1] = small_petal_char_dim / 2 + ypart(angle, petal_1b_long);
		petal_1b[2] = small_petal_char_dim / 2 - xpart(angle + ANGLE_4 + ANGLE_8, petal_1b_short);
		petal_1b[3] = small_petal_char_dim / 2 - ypart(angle + ANGLE_4 + ANGLE_8, petal_1b_short);
		petal_1b[4] = small_petal_char_dim / 2 + xpart(angle, petal_1b_nano);
		petal_1b[5] = small_petal_char_dim / 2 + ypart(angle, petal_1b_nano);
		petal_1b[6] = small_petal_char_dim / 2 - xpart(angle - ANGLE_4 - ANGLE_8, petal_1b_short);
		petal_1b[7] = small_petal_char_dim / 2 - ypart(angle - ANGLE_4 - ANGLE_8, petal_1b_short);
		fast_bordered_poly(temp_bitmap, 4, petal_1b, TRANS_SH2_IN, TRANS_SH2_OUT);

		circlefill(temp_bitmap, 40 + xpart(angle, 12), 40 + ypart(angle, 12), 3, TRANS_SH1_IN);
		circle(temp_bitmap, 40 + xpart(angle, 12), 40 + ypart(angle, 12), 3, TRANS_SH2_OUT);

		new_rle_struct(temp_bitmap, "petal1 RLE", &petal1 [i], -1);

	}/* endfor (i = 0; i < SMALL_ROTATIONS; i ++) */

	int petal_2[8]; 
	const int petal_2_short = (small_petal_char_dim * 7) / 80; 
	const int petal_2_long = (small_petal_char_dim * 11) / 40;
	for (i = 0; i < SMALL_ROTATIONS; i ++)
	{

		clear_bitmap(temp_bitmap);
		angle = i * inc;
		petal_2[0] = small_petal_char_dim / 2 + xpart(angle, petal_2_long);
		petal_2[1] = small_petal_char_dim / 2 + ypart(angle, petal_2_long);
		petal_2[2] = small_petal_char_dim / 2 - xpart(angle + ANGLE_4 + ANGLE_8, petal_2_short);
		petal_2[3] = small_petal_char_dim / 2 - ypart(angle + ANGLE_4 + ANGLE_8, petal_2_short);
		petal_2[4] = petal_2[5] = small_petal_char_dim / 2;
		petal_2[6] = small_petal_char_dim / 2 - ypart(angle - ANGLE_4 - ANGLE_8, petal_2_short);
		petal_2[7] = small_petal_char_dim / 2 - xpart(angle - ANGLE_4 - ANGLE_8, petal_2_short);
		fast_bordered_poly(temp_bitmap, 4, petal_2, TRANS_SH1_IN, TRANS_SH1_OUT);
		new_rle_struct(temp_bitmap, "petal2 RLE", &petal2 [i], -1);

	}/* endfor (i = 0; i < SMALL_ROTATIONS; i ++) */

	destroy_bitmap(temp_bitmap);

	const int big_petal_char_dim = (small_petal_char_dim * 3) / 2;

	temp_bitmap = new_bitmap(big_petal_char_dim, big_petal_char_dim, "prepare petal3");

	int petal_3a[8]; 
	const int petal_3a_short = (big_petal_char_dim * 1) / 10; 
	const int petal_3a_long = (big_petal_char_dim * 3) / 8;

	int petal_3b[12]; 
	const int petal_3b_nano = (big_petal_char_dim * 3) / 40; 
	const int petal_3b_short = (big_petal_char_dim * 3) / 20; 
	const int petal_3b_medium = (big_petal_char_dim * 3) / 8; 
	const int petal_3b_long = (big_petal_char_dim * 11) / 24;

	for (i = 0; i < SMALL_ROTATIONS; i ++)
	{

		clear_bitmap(temp_bitmap);
		angle = i * inc;

		petal_3a[0] = big_petal_char_dim / 2 + xpart(angle, petal_3a_long);
		petal_3a[1] = big_petal_char_dim / 2 + ypart(angle, petal_3a_long);
		petal_3a[2] = big_petal_char_dim / 2 - xpart(angle + ANGLE_4 + ANGLE_8, petal_3a_short);
		petal_3a[3] = big_petal_char_dim / 2 - ypart(angle + ANGLE_4 + ANGLE_8, petal_3a_short);
		petal_3a[4] = petal_3a[5] = big_petal_char_dim / 2;
		petal_3a[6] = big_petal_char_dim / 2 - ypart(angle - ANGLE_4 - ANGLE_8, petal_3a_short);
		petal_3a[7] = big_petal_char_dim / 2 - xpart(angle - ANGLE_4 - ANGLE_8, petal_3a_short);
		fast_bordered_poly(temp_bitmap, 4, petal_3a, TRANS_SH1_IN, TRANS_SH1_OUT);

		petal_3b[0] = big_petal_char_dim / 2 + xpart(angle, petal_3b_long);
		petal_3b[1] = big_petal_char_dim / 2 + ypart(angle, petal_3b_long);
		petal_3b[2] = big_petal_char_dim / 2 + xpart(angle - ANGLE_16, petal_3b_medium);
		petal_3b[3] = big_petal_char_dim / 2 + ypart(angle - ANGLE_16, petal_3b_medium);
		petal_3b[4] = big_petal_char_dim / 2 - xpart(angle + ANGLE_4 + ANGLE_8, petal_3b_short);
		petal_3b[5] = big_petal_char_dim / 2 - ypart(angle + ANGLE_4 + ANGLE_8, petal_3b_short);
		petal_3b[6] = big_petal_char_dim / 2 + xpart(angle, petal_3b_nano);
		petal_3b[7] = big_petal_char_dim / 2 + ypart(angle, petal_3b_nano);
		petal_3b[8] = big_petal_char_dim / 2 - xpart(angle - ANGLE_4 - ANGLE_8, petal_3b_short);
		petal_3b[9] = big_petal_char_dim / 2 - ypart(angle - ANGLE_4 - ANGLE_8, petal_3b_short);
		petal_3b[10]= big_petal_char_dim / 2 + xpart(angle + ANGLE_16, petal_3b_medium);
		petal_3b[11]= big_petal_char_dim / 2 + ypart(angle + ANGLE_16, petal_3b_medium);
		fast_bordered_poly(temp_bitmap, 6, petal_3b, TRANS_SH2_IN, TRANS_SH2_OUT);

		circlefill(temp_bitmap, 60 + xpart(angle, 25), 60 + ypart(angle, 25), 4, TRANS_SH1_IN);
		circle(temp_bitmap, 60 + xpart(angle, 25), 60 + ypart(angle, 25), 4, TRANS_SH2_OUT);

// circlefill(temp_bitmap, 60 + xpart(angle, 22), 60 + ypart(angle, 22), 3, TRANS_SH1_IN);
// circle(temp_bitmap, 60 + xpart(angle, 22), 60 + ypart(angle, 22), 3, TRANS_SH2_OUT);

		circlefill(temp_bitmap, 60 + xpart(angle, 38), 60 + ypart(angle, 38), 3, TRANS_SH1_IN);
		circle(temp_bitmap, 60 + xpart(angle, 38), 60 + ypart(angle, 38), 3, TRANS_SH2_OUT);

		new_rle_struct(temp_bitmap, "petal1 RLE", &petal3 [i], -1);
	}/* endfor (i = 0; i < SMALL_ROTATIONS; i ++) */

	destroy_bitmap(temp_bitmap);
}

void prepare_multi_rles(void)
{

	strncpy(filename_buffer, data_directory, sizeof(char) * DATADIR_SIZE);
	strncat(filename_buffer, "gfx/multi.bmp", sizeof(char) * DATADIR_SIZE);
	BITMAP *file_bitmap = load_up_bitmap(filename_buffer);

	fix_outline(file_bitmap);

	int i;

	RLE_multi [WPN_CANNON] [0].sprite = extract_flip_rle_sprite(file_bitmap, 3, 1, 11, 11, FLIP_NONE);
	RLE_multi [WPN_CANNON] [0].x = 5;
	RLE_multi [WPN_CANNON] [0].y = 5;
	RLE_multi [WPN_CANNON] [1].sprite = extract_flip_rle_sprite(file_bitmap, 2, 13, 13, 11, FLIP_NONE);
	RLE_multi [WPN_CANNON] [1].x = 6;
	RLE_multi [WPN_CANNON] [1].y = 5;
	RLE_multi [WPN_CANNON] [2].sprite = extract_flip_rle_sprite(file_bitmap, 1, 25, 15, 11, FLIP_NONE);
	RLE_multi [WPN_CANNON] [2].x = 7;
	RLE_multi [WPN_CANNON] [2].y = 5;
	RLE_multi [WPN_CANNON] [3].sprite = extract_flip_rle_sprite(file_bitmap, 1, 37, 15, 11, FLIP_NONE);
	RLE_multi [WPN_CANNON] [3].x = 7;
	RLE_multi [WPN_CANNON] [3].y = 5;
	RLE_multi [WPN_BLADE] [0].sprite = extract_flip_rle_sprite(file_bitmap, 17, 1, 9, 9, FLIP_NONE);
	RLE_multi [WPN_BLADE] [0].x = 4;
	RLE_multi [WPN_BLADE] [0].y = 4;
	RLE_multi [WPN_BLADE] [1].sprite = extract_flip_rle_sprite(file_bitmap, 19, 11, 7, 7, FLIP_NONE);
	RLE_multi [WPN_BLADE] [1].x = 3;
	RLE_multi [WPN_BLADE] [1].y = 3;
	for (i = 0; i < 5; i ++)
	{
		RLE_multi [WPN_MGUN] [i].sprite = extract_flip_rle_sprite(file_bitmap, 27, 1 + i * 12, 19, 11, FLIP_NONE);
		RLE_multi [WPN_MGUN] [i].x = 9;
		RLE_multi [WPN_MGUN] [i].y = 6;
	}
	for (i = 0; i < 4; i ++)
	{
		RLE_multi [WPN_SCATTER] [i].sprite = extract_flip_rle_sprite(file_bitmap, 47, 1 + i * 13, 13, 11, FLIP_NONE);
		RLE_multi [WPN_SCATTER] [i].x = 6;
		RLE_multi [WPN_SCATTER] [i].y = 5;
	}
	RLE_multi [WPN_LWBEAM] [0].sprite = extract_flip_rle_sprite(file_bitmap, 101, 1, 11, 11, FLIP_NONE);
	RLE_multi [WPN_LWBEAM] [0].x = 5;
	RLE_multi [WPN_LWBEAM] [0].y = 5;
	RLE_multi [WPN_LWBEAM] [1].sprite = extract_flip_rle_sprite(file_bitmap, 100, 13, 13, 11, FLIP_NONE);
	RLE_multi [WPN_LWBEAM] [1].x = 6;
	RLE_multi [WPN_LWBEAM] [1].y = 5;
	RLE_multi [WPN_LWBEAM] [2].sprite = extract_flip_rle_sprite(file_bitmap, 99, 25, 15, 11, FLIP_NONE);
	RLE_multi [WPN_LWBEAM] [2].x = 7;
	RLE_multi [WPN_LWBEAM] [2].y = 5;
	RLE_multi [WPN_SWBEAM] [0].sprite = extract_flip_rle_sprite(file_bitmap, 73, 1, 11, 11, FLIP_NONE);
	RLE_multi [WPN_SWBEAM] [0].x = 5;
	RLE_multi [WPN_SWBEAM] [0].y = 5;
	RLE_multi [WPN_SWBEAM] [1].sprite = extract_flip_rle_sprite(file_bitmap, 73, 13, 11, 11, FLIP_NONE);
	RLE_multi [WPN_SWBEAM] [1].x = 5;
	RLE_multi [WPN_SWBEAM] [1].y = 4;
	RLE_multi [WPN_SWBEAM] [2].sprite = extract_flip_rle_sprite(file_bitmap, 73, 25, 11, 11, FLIP_NONE);
	RLE_multi [WPN_SWBEAM] [2].x = 5;
	RLE_multi [WPN_SWBEAM] [2].y = 3;
	RLE_multi [WPN_SEEKER] [0].sprite = extract_flip_rle_sprite(file_bitmap, 85, 1, 13, 12, FLIP_NONE);
	RLE_multi [WPN_SEEKER] [0].x = 6;
	RLE_multi [WPN_SEEKER] [0].y = 7;
	RLE_multi [WPN_SEEKER] [1].sprite = extract_flip_rle_sprite(file_bitmap, 85, 14, 13, 12, FLIP_NONE);
	RLE_multi [WPN_SEEKER] [1].x = 6;
	RLE_multi [WPN_SEEKER] [1].y = 7;
	RLE_multi [WPN_SEEKER] [2].sprite = extract_flip_rle_sprite(file_bitmap, 85, 27, 13, 12, FLIP_NONE);
	RLE_multi [WPN_SEEKER] [2].x = 6;
	RLE_multi [WPN_SEEKER] [2].y = 7;
	RLE_multi [WPN_SEEKER] [3].sprite = extract_flip_rle_sprite(file_bitmap, 85, 40, 13, 12, FLIP_NONE);
	RLE_multi [WPN_SEEKER] [3].x = 6;
	RLE_multi [WPN_SEEKER] [3].y = 7;
	RLE_multi [WPN_SEEKER] [4].sprite = extract_flip_rle_sprite(file_bitmap, 85, 53, 13, 12, FLIP_NONE);
	RLE_multi [WPN_SEEKER] [4].x = 6;
	RLE_multi [WPN_SEEKER] [4].y = 7;

	RLE_multi [WPN_RECT] [0].sprite = extract_flip_rle_sprite(file_bitmap, 115, 1, 15, 11, FLIP_NONE);
	RLE_multi [WPN_RECT] [0].x = 7;
	RLE_multi [WPN_RECT] [0].y = 5;
	RLE_multi [WPN_RECT] [1].sprite = extract_flip_rle_sprite(file_bitmap, 116, 13, 13, 11, FLIP_NONE);
	RLE_multi [WPN_RECT] [1].x = 6;
	RLE_multi [WPN_RECT] [1].y = 5;
	RLE_multi [WPN_RECT] [2].sprite = extract_flip_rle_sprite(file_bitmap, 117, 25, 11, 11, FLIP_NONE);
	RLE_multi [WPN_RECT] [2].x = 5;
	RLE_multi [WPN_RECT] [2].y = 5;
	RLE_multi [WPN_RECT] [3].sprite = extract_flip_rle_sprite(file_bitmap, 117, 37, 11, 11, FLIP_NONE);
	RLE_multi [WPN_RECT] [3].x = 5;
	RLE_multi [WPN_RECT] [3].y = 5;

	RLE_multi [WPN_EIGHT] [0].sprite = extract_flip_rle_sprite(file_bitmap, 131, 1, 13, 13, FLIP_NONE);
	RLE_multi [WPN_EIGHT] [0].x = 6;
	RLE_multi [WPN_EIGHT] [0].y = 6;

	destroy_bitmap(file_bitmap);
}

void prepare_platform_rles(void)
{

	strncpy(filename_buffer, data_directory, sizeof(char) * DATADIR_SIZE);
	strncat(filename_buffer, "gfx/platform.bmp", sizeof(char) * DATADIR_SIZE);
	BITMAP *file_bitmap = load_up_bitmap(filename_buffer);

// int i, j;

// exit(getpixel(file_bitmap, 0, 0));

	fix_outline(file_bitmap);

// int i;

// for (i = 0; i < 2; i ++)
// {/


	platform_RLE [PLATFORM_LR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_LR * PLATFORM_SIZE, 1, 91, 91, FLIP_NONE);
	platform_RLE [PLATFORM_UD] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UD * PLATFORM_SIZE, 1, 91, 91, FLIP_NONE);

	platform_RLE [PLATFORM_UL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, FLIP_NONE);
	platform_RLE [PLATFORM_DL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, FLIP_V);
	platform_RLE [PLATFORM_UR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, FLIP_H);
	platform_RLE [PLATFORM_DR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, FLIP_VH);

	platform_RLE [PLATFORM_UDL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UDL * PLATFORM_SIZE, 1, 91, 91, FLIP_NONE);
	platform_RLE [PLATFORM_UDR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UDL * PLATFORM_SIZE, 1, 91, 91, FLIP_H);

	platform_RLE [PLATFORM_ULR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_ULR * PLATFORM_SIZE, 1, 91, 91, FLIP_NONE);
	platform_RLE [PLATFORM_DLR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_ULR * PLATFORM_SIZE, 1, 91, 91, FLIP_V);

	platform_RLE [PLATFORM_L] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_L * PLATFORM_SIZE, 1, 91, 91, FLIP_NONE);
	platform_RLE [PLATFORM_R] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_L * PLATFORM_SIZE, 1, 91, 91, FLIP_H);
	platform_RLE [PLATFORM_U] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_U * PLATFORM_SIZE, 1, 91, 91, FLIP_NONE);
	platform_RLE [PLATFORM_D] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_U * PLATFORM_SIZE, 1, 91, 91, FLIP_V);


	platform_RLE [PLATFORM_UR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, FLIP_H);
	platform_RLE [PLATFORM_DR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, FLIP_VH);

	platform_RLE [PLATFORM_NODE] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_NODE * PLATFORM_SIZE, 1, 91, 91, FLIP_NONE);
	platform_RLE [PLATFORM_NODE2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_NODE2 * PLATFORM_SIZE, 1, 91, 91, FLIP_NONE);
	platform_RLE [PLATFORM_NODE3] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_NODE3 * PLATFORM_SIZE, 1, 91, 91, FLIP_NONE);


	island1_RLE [ISLAND1_D] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_D * PLATFORM_SIZE, 93, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_U] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_D * PLATFORM_SIZE, 93, 91, 91, FLIP_V);

	island1_RLE [ISLAND1_R] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_R * PLATFORM_SIZE, 93, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_L] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_R * PLATFORM_SIZE, 93, 91, 91, FLIP_H);

	island1_RLE [ISLAND1_DR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 93, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_DL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 93, 91, 91, FLIP_H);
	island1_RLE [ISLAND1_UR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 93, 91, 91, FLIP_V);
	island1_RLE [ISLAND1_UL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 93, 91, 91, FLIP_VH);

	island1_RLE [ISLAND1_NOT_UL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 93, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_NOT_UR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 93, 91, 91, FLIP_H);
	island1_RLE [ISLAND1_NOT_DL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 93, 91, 91, FLIP_V);
	island1_RLE [ISLAND1_NOT_DR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 93, 91, 91, FLIP_VH);

	island1_RLE [ISLAND1_UL_DR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 93, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_UR_DL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 93, 91, 91, FLIP_H);

	island1_RLE [ISLAND1_FULL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL * PLATFORM_SIZE, 93, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_FULL2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL2 * PLATFORM_SIZE, 93, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_FULL3] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL3 * PLATFORM_SIZE, 93, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_FULL4] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL4 * PLATFORM_SIZE, 93, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_FULL5] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL5 * PLATFORM_SIZE, 93, 91, 91, FLIP_NONE);


	island1_RLE [ISLAND1_D2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_D * PLATFORM_SIZE, 185, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_U2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_D * PLATFORM_SIZE, 185, 91, 91, FLIP_V);

	island1_RLE [ISLAND1_R2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_R * PLATFORM_SIZE, 185, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_L2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_R * PLATFORM_SIZE, 185, 91, 91, FLIP_H);

	island1_RLE [ISLAND1_DR2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_DL2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_H);
	island1_RLE [ISLAND1_UR2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_V);
	island1_RLE [ISLAND1_UL2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_VH);

	island1_RLE [ISLAND1_NOT_UL2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_NOT_UR2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_H);
	island1_RLE [ISLAND1_NOT_DL2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_V);
	island1_RLE [ISLAND1_NOT_DR2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_VH);

	island1_RLE [ISLAND1_UL_DR2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_UR_DL2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_H);



	island1_RLE [ISLAND1_D3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_D * PLATFORM_SIZE, 185, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_U3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_D * PLATFORM_SIZE, 185, 91, 91, FLIP_V);

	island1_RLE [ISLAND1_R3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_R * PLATFORM_SIZE, 185, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_L3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_R * PLATFORM_SIZE, 185, 91, 91, FLIP_H);

	island1_RLE [ISLAND1_DR3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_DL3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_H);
	island1_RLE [ISLAND1_UR3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_V);
	island1_RLE [ISLAND1_UL3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_VH);

	island1_RLE [ISLAND1_NOT_UL3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_NOT_UR3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_H);
	island1_RLE [ISLAND1_NOT_DL3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_V);
	island1_RLE [ISLAND1_NOT_DR3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_VH);

	island1_RLE [ISLAND1_UL_DR3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_NONE);
	island1_RLE [ISLAND1_UR_DL3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_H);

	bone_RLE [BONE_HOOK1] = extract_flip_rle_sprite(file_bitmap, 1, 278, 23, 22, FLIP_NONE);
	bone_RLE [BONE_HOOK2] = extract_flip_rle_sprite(file_bitmap, 25, 278, 23, 22, FLIP_NONE);
	bone_RLE [BONE_HOOK3] = extract_flip_rle_sprite(file_bitmap, 49, 278, 23, 22, FLIP_NONE);
	bone_RLE [BONE_HOOK4] = extract_flip_rle_sprite(file_bitmap, 76, 278, 20, 22, FLIP_NONE);
	bone_RLE [BONE_HOOK5] = extract_flip_rle_sprite(file_bitmap, 97, 278, 23, 18, FLIP_NONE);
	bone_RLE [BONE_HOOK2F] = extract_flip_rle_sprite(file_bitmap, 25, 278, 23, 22, FLIP_H);
	bone_RLE [BONE_HOOK3F] = extract_flip_rle_sprite(file_bitmap, 49, 278, 23, 22, FLIP_H);
	bone_RLE [BONE_HOOK4F] = extract_flip_rle_sprite(file_bitmap, 76, 278, 20, 22, FLIP_H);
	bone_RLE [BONE_HOOK5F] = extract_flip_rle_sprite(file_bitmap, 97, 278, 23, 18, FLIP_H);


	bone_RLE [BONE_B_HOOK1] = extract_flip_rle_sprite(file_bitmap, 1, 301, 11, 11, FLIP_NONE);
	bone_RLE [BONE_B_HOOK2] = extract_flip_rle_sprite(file_bitmap, 13, 301, 11, 11, FLIP_NONE);
	bone_RLE [BONE_B_HOOK3] = extract_flip_rle_sprite(file_bitmap, 25, 301, 11, 11, FLIP_NONE);
	bone_RLE [BONE_B_HOOK4] = extract_flip_rle_sprite(file_bitmap, 38, 301, 10, 11, FLIP_NONE);
	bone_RLE [BONE_B_HOOK5] = extract_flip_rle_sprite(file_bitmap, 49, 301, 11, 9, FLIP_NONE);
	bone_RLE [BONE_B_HOOK2F] = extract_flip_rle_sprite(file_bitmap, 13, 301, 11, 11, FLIP_H);
	bone_RLE [BONE_B_HOOK3F] = extract_flip_rle_sprite(file_bitmap, 25, 301, 11, 11, FLIP_H);
	bone_RLE [BONE_B_HOOK4F] = extract_flip_rle_sprite(file_bitmap, 38, 301, 10, 11, FLIP_H);
	bone_RLE [BONE_B_HOOK5F] = extract_flip_rle_sprite(file_bitmap, 49, 301, 11, 9, FLIP_H);

	bone_RLE [BONE_DROP1] = extract_flip_rle_sprite(file_bitmap, 122, 278, 23, 35, FLIP_NONE);
	bone_RLE [BONE_DROP2] = extract_flip_rle_sprite(file_bitmap, 146, 278, 23, 35, FLIP_NONE);
	bone_RLE [BONE_DROP3] = extract_flip_rle_sprite(file_bitmap, 170, 278, 23, 35, FLIP_NONE);
	bone_RLE [BONE_DROP4] = extract_flip_rle_sprite(file_bitmap, 194, 278, 23, 35, FLIP_NONE);
	bone_RLE [BONE_DROP2F] = extract_flip_rle_sprite(file_bitmap, 146, 278, 23, 35, FLIP_H);
	bone_RLE [BONE_DROP3F] = extract_flip_rle_sprite(file_bitmap, 170, 278, 23, 35, FLIP_H);
	bone_RLE [BONE_DROP4F] = extract_flip_rle_sprite(file_bitmap, 194, 278, 23, 35, FLIP_H);

	bone_RLE [BONE_B_DROP1] = extract_flip_rle_sprite(file_bitmap, 61, 301, 11, 16, FLIP_NONE);
	bone_RLE [BONE_B_DROP2] = extract_flip_rle_sprite(file_bitmap, 73, 301, 11, 16, FLIP_NONE);
	bone_RLE [BONE_B_DROP3] = extract_flip_rle_sprite(file_bitmap, 85, 301, 10, 16, FLIP_NONE);
	bone_RLE [BONE_B_DROP4] = extract_flip_rle_sprite(file_bitmap, 96, 304, 10, 13, FLIP_NONE);
	bone_RLE [BONE_B_DROP2F] = extract_flip_rle_sprite(file_bitmap, 73, 301, 11, 16, FLIP_H);
	bone_RLE [BONE_B_DROP3F] = extract_flip_rle_sprite(file_bitmap, 85, 301, 10, 16, FLIP_H);
	bone_RLE [BONE_B_DROP4F] = extract_flip_rle_sprite(file_bitmap, 96, 304, 10, 13, FLIP_H);

	bone_RLE [BONE_RING1] = extract_flip_rle_sprite(file_bitmap, 1, 322, 39, 39, FLIP_NONE);
	bone_RLE [BONE_RING2] = extract_flip_rle_sprite(file_bitmap, 41, 322, 39, 39, FLIP_NONE);
	bone_RLE [BONE_RING3] = extract_flip_rle_sprite(file_bitmap, 81, 322, 39, 39, FLIP_NONE);
	bone_RLE [BONE_RING4] = extract_flip_rle_sprite(file_bitmap, 121, 322, 39, 39, FLIP_NONE);
	bone_RLE [BONE_RING5] = extract_flip_rle_sprite(file_bitmap, 161, 322, 39, 39, FLIP_NONE);
	bone_RLE [BONE_RING2FH] = extract_flip_rle_sprite(file_bitmap, 41, 322, 39, 39, FLIP_H);
	bone_RLE [BONE_RING3FH] = extract_flip_rle_sprite(file_bitmap, 81, 322, 39, 39, FLIP_H);
	bone_RLE [BONE_RING4FH] = extract_flip_rle_sprite(file_bitmap, 121, 322, 39, 39, FLIP_H);
	bone_RLE [BONE_RING5FH] = extract_flip_rle_sprite(file_bitmap, 161, 322, 39, 39, FLIP_H);
	bone_RLE [BONE_RING2FV] = extract_flip_rle_sprite(file_bitmap, 41, 322, 39, 39, FLIP_V);
	bone_RLE [BONE_RING3FV] = extract_flip_rle_sprite(file_bitmap, 81, 322, 39, 39, FLIP_V);
	bone_RLE [BONE_RING4FV] = extract_flip_rle_sprite(file_bitmap, 121, 322, 39, 39, FLIP_V);
	bone_RLE [BONE_RING5FV] = extract_flip_rle_sprite(file_bitmap, 161, 322, 39, 39, FLIP_V);
	bone_RLE [BONE_RING2FVH] = extract_flip_rle_sprite(file_bitmap, 41, 322, 39, 39, FLIP_VH);
	bone_RLE [BONE_RING3FVH] = extract_flip_rle_sprite(file_bitmap, 81, 322, 39, 39, FLIP_VH);
	bone_RLE [BONE_RING4FVH] = extract_flip_rle_sprite(file_bitmap, 121, 322, 39, 39, FLIP_VH);
	bone_RLE [BONE_RING5FVH] = extract_flip_rle_sprite(file_bitmap, 161, 322, 39, 39, FLIP_VH);

	bone_RLE [BONE_B_RING1] = extract_flip_rle_sprite(file_bitmap, 1, 362, 23, 23, FLIP_NONE);
	bone_RLE [BONE_B_RING2] = extract_flip_rle_sprite(file_bitmap, 25, 362, 23, 23, FLIP_NONE);
	bone_RLE [BONE_B_RING3] = extract_flip_rle_sprite(file_bitmap, 49, 362, 23, 23, FLIP_NONE);
	bone_RLE [BONE_B_RING4] = extract_flip_rle_sprite(file_bitmap, 73, 362, 23, 23, FLIP_NONE);
	bone_RLE [BONE_B_RING5] = extract_flip_rle_sprite(file_bitmap, 97, 362, 23, 23, FLIP_NONE);
	bone_RLE [BONE_B_RING2FH] = extract_flip_rle_sprite(file_bitmap, 25, 362, 23, 23, FLIP_H);
	bone_RLE [BONE_B_RING3FH] = extract_flip_rle_sprite(file_bitmap, 49, 362, 23, 23, FLIP_H);
	bone_RLE [BONE_B_RING4FH] = extract_flip_rle_sprite(file_bitmap, 73, 362, 23, 23, FLIP_H);
	bone_RLE [BONE_B_RING5FH] = extract_flip_rle_sprite(file_bitmap, 97, 362, 23, 23, FLIP_H);
	bone_RLE [BONE_B_RING2FV] = extract_flip_rle_sprite(file_bitmap, 25, 362, 23, 23, FLIP_V);
	bone_RLE [BONE_B_RING3FV] = extract_flip_rle_sprite(file_bitmap, 49, 362, 23, 23, FLIP_V);
	bone_RLE [BONE_B_RING4FV] = extract_flip_rle_sprite(file_bitmap, 73, 362, 23, 23, FLIP_V);
	bone_RLE [BONE_B_RING5FV] = extract_flip_rle_sprite(file_bitmap, 97, 362, 23, 23, FLIP_V);
	bone_RLE [BONE_B_RING2FVH] = extract_flip_rle_sprite(file_bitmap, 25, 362, 23, 23, FLIP_VH);
	bone_RLE [BONE_B_RING3FVH] = extract_flip_rle_sprite(file_bitmap, 49, 362, 23, 23, FLIP_VH);
	bone_RLE [BONE_B_RING4FVH] = extract_flip_rle_sprite(file_bitmap, 73, 362, 23, 23, FLIP_VH);
	bone_RLE [BONE_B_RING5FVH] = extract_flip_rle_sprite(file_bitmap, 97, 362, 23, 23, FLIP_VH);

	bone_RLE [BONE_HUNTER1] = extract_flip_rle_sprite(file_bitmap, 201, 322, 45, 40, FLIP_NONE);
	bone_RLE [BONE_HUNTER2] = extract_flip_rle_sprite(file_bitmap, 247, 322, 45, 40, FLIP_NONE);
	bone_RLE [BONE_HUNTER3] = extract_flip_rle_sprite(file_bitmap, 293, 322, 45, 40, FLIP_NONE);
	bone_RLE [BONE_HUNTER4] = extract_flip_rle_sprite(file_bitmap, 339, 322, 41, 40, FLIP_NONE);
	bone_RLE [BONE_HUNTER5] = extract_flip_rle_sprite(file_bitmap, 381, 322, 45, 40, FLIP_NONE);
	bone_RLE [BONE_HUNTER2F] = extract_flip_rle_sprite(file_bitmap, 247, 322, 45, 40, FLIP_H);
	bone_RLE [BONE_HUNTER3F] = extract_flip_rle_sprite(file_bitmap, 293, 322, 45, 40, FLIP_H);
	bone_RLE [BONE_HUNTER4F] = extract_flip_rle_sprite(file_bitmap, 339, 322, 41, 40, FLIP_H);
	bone_RLE [BONE_HUNTER5F] = extract_flip_rle_sprite(file_bitmap, 381, 322, 45, 40, FLIP_H);

	bone_RLE [BONE_B_HUNTER1] = extract_flip_rle_sprite(file_bitmap, 199, 364, 27, 23, FLIP_NONE);
	bone_RLE [BONE_B_HUNTER2] = extract_flip_rle_sprite(file_bitmap, 227, 364, 27, 23, FLIP_NONE);
	bone_RLE [BONE_B_HUNTER3] = extract_flip_rle_sprite(file_bitmap, 255, 364, 27, 23, FLIP_NONE);
	bone_RLE [BONE_B_HUNTER4] = extract_flip_rle_sprite(file_bitmap, 285, 364, 25, 23, FLIP_NONE);
	bone_RLE [BONE_B_HUNTER5] = extract_flip_rle_sprite(file_bitmap, 311, 364, 27, 23, FLIP_NONE);
	bone_RLE [BONE_B_HUNTER2F] = extract_flip_rle_sprite(file_bitmap, 227, 364, 27, 23, FLIP_H);
	bone_RLE [BONE_B_HUNTER3F] = extract_flip_rle_sprite(file_bitmap, 255, 364, 27, 23, FLIP_H);
	bone_RLE [BONE_B_HUNTER4F] = extract_flip_rle_sprite(file_bitmap, 285, 364, 25, 23, FLIP_H);
	bone_RLE [BONE_B_HUNTER5F] = extract_flip_rle_sprite(file_bitmap, 311, 364, 27, 23, FLIP_H);

	bone_RLE [BONE_B_SPINNER] = extract_flip_rle_sprite(file_bitmap, 427, 288, 65, 107, FLIP_NONE);

	bone_RLE [BONE_B_WALKER1] = extract_flip_rle_sprite(file_bitmap, 493, 285, 89, 75, FLIP_NONE);
	bone_RLE [BONE_B_WALKER2] = extract_flip_rle_sprite(file_bitmap, 583, 285, 89, 75, FLIP_NONE);
	bone_RLE [BONE_B_WALKER2F] = extract_flip_rle_sprite(file_bitmap, 583, 285, 89, 75, FLIP_H);

	destroy_bitmap(file_bitmap);

}

/**
 * This function sets the eRLE_flower [0] structure to contain nice flower with petals
 */
void prepare_flowers(void)
{
	/* Make the dimensions independent form resolution */
	int char_dim = 200;
	int smaller_petal_length = char_dim / 50 * 9;
	int full_petal_length = char_dim / 40 * 11;
	int fill_petal_length = char_dim / 20 * 3;
	/* Prepare a nice bitmap */
	BITMAP *tmp = new_bitmap(char_dim, char_dim, "prepare_flowers");
	/* We are going to use these in the loops  */
	int point [8];
	int fillx, filly;

	int i, j, angle = 0;
	for (j = 0; j < 40; j ++)
	{
		clear_bitmap(tmp);
		angle = j * (ANGLE_10 / 20);
		/* Creating five petals, aren't we? */
		for (i = 0; i < 5; i ++)
		{
			int main_angle = angle + (ANGLE_5 * i);
			/* the middle */
			point[0] = char_dim / 2;
			point[1] = char_dim / 2;
			/* the rest */
			point[2] = point[0] + xpart(main_angle - 46, smaller_petal_length);
			point[3] = point[1] + ypart(main_angle - 46, smaller_petal_length);
			point[4] = point[0] + xpart(main_angle, full_petal_length);
			point[5] = point[1] + ypart(main_angle, full_petal_length);
			point[6] = point[0] + xpart(main_angle + 46, smaller_petal_length);
			point[7] = point[1] + ypart(main_angle + 46, smaller_petal_length);
			fillx = point[0] + xpart(main_angle, fill_petal_length);
			filly = point[1] + ypart(main_angle, fill_petal_length);
			/* draw the polygon on the bitmap */
			/*fast_bordered_poly4(tmp,
				point[0], point[1], point[2], point[3], point[4], point[5], point[6], point[7],
			                    COL_BACK5,COL_OUTLINE, fillx, filly);*/
			fast_bordered_poly(tmp, 4, point, COL_BACK5, COL_OUTLINE);
		}/* endfor (i = 0; i < 5; i ++) */
		/* Maybe the flower middle? */
		circlefill(tmp, char_dim / 2, char_dim / 2, 8, COL_COL3);
		/* Maybe the flower middle border? */
		circle(tmp, char_dim / 2, char_dim / 2, 8, COL_OUTLINE);

// extract_rle_struct(tmp, 0, 0, 100, 100, "flower RLE", &eRLE_flower [0] [j], -1);
		new_rle_struct(tmp, "flower RLE", & eRLE_flower [0] [j], -2);
	}/* endfor (j = 0; j < 40; j ++) */
	destroy_bitmap(tmp);
	tmp = NULL;
}

/*
 * A COMMENTED FUNCTION
 * void prepare_mshockwaves(void)
 * WAS HERE AND IS NOW REMOVED. 
 * CHECK THE SVN REPOSITORY IF YOU MISS IT
 */

void prepare_rings(void)
{
	int i, j;

	BITMAP *tmp = new_bitmap(62, 62, "prepare_rings tmp bitmap");

	int rad1, rad2, c1, c2;
	i = 0;
	c1 = TRANS_ORANGE_OUT;
	c2 = TRANS_ORANGE_IN;

	rad1 = 1;

//  rad2 = -5;

	for (j = 0; j < 20; j ++)
	{
		clear_bitmap(tmp);

		rad1 ++;
//    if (j % 2 == 0)
//     rad1 ++;
		rad2 = rad1 - 3;
		if (rad2 < 1)
			rad2 = 0;

		circlefill(tmp, rad1, rad1, rad1, c2);
		circle(tmp, rad1, rad1, rad1, c1);
		if (rad2 > 0)
		{
			circlefill(tmp, rad1, rad1, rad2, 0);
			circle(tmp, rad1, rad1, rad2, c1);
		}/* endif (rad2 > 0) */

//    new_rle_struct(tmp, "mshockwaves rle", &RLE_mshock [i] [j], -1);
		extract_rle_struct2(tmp, &RLE_ring [j], 0, 0, rad1 * 2 + 1, rad1 * 2 + 1, -1);

	}/* endfor (j = 0; j < 20; j ++) */
	destroy_bitmap(tmp);
}

void prepare_lwbeamwaves(void)
{
	int i, j;

	BITMAP *tmp = new_bitmap(62, 62, "prepare_lwbeamwaves tmp bitmap");

	int rad1, rad2, c1, c2;
	i = 0;
	c1 = TRANS_ORANGE_OUT;
	c2 = TRANS_ORANGE_IN;

	rad1 = 10;

	rad2 = -5;

	for (j = 0; j < 15; j ++)
	{
		clear_bitmap(tmp);

		rad2 += 2;
//    if (j % 2 == 0)
		rad1 ++;

		if (j >= 18)
		{
			circle(tmp, rad1, rad1, rad1, c1);
		}/* endif (j >= 18) */
		else
		{
			circlefill(tmp, rad1, rad1, rad1, c2);
			circle(tmp, rad1, rad1, rad1, c1);
			if (rad2 > 1)
			{
				circlefill(tmp, rad1, rad1, rad2, 0);
				circle(tmp, rad1, rad1, rad2, c1);
			}/* endif (rad2 > 1) */
		}/* endelse (j >= 18) */

//    new_rle_struct(tmp, "mshockwaves rle", &RLE_mshock [i] [j], -1);
		extract_rle_struct2(tmp, &RLE_lwbeamshock [j], 0, 0, rad1 * 2 + 1, rad1 * 2 + 1, -1);

	}/* endfor (j = 0; j < 15; j ++) */
	destroy_bitmap(tmp);
}

void prepare_mshockwaves(void)
{
	int i, j;

	BITMAP *tmp = new_bitmap(90, 90, "prepare_mshockwaves tmp bitmap");

	int rad1, rad2, c1 = 0, c2 = 0;
	i = 0;
	for (i = 0; i < 4; i ++)
	{

		switch (i)
		{
		case 0:
			c1 = TRANS_ORANGE_OUT;
			c2 = TRANS_ORANGE_IN;
			break;
		case 1:
			c1 = TRANS_YELLOW_OUT;
			c2 = TRANS_YELLOW_IN;
			break;
		case 2:
			c1 = TRANS_SH1_OUT;
			c2 = TRANS_SH1_IN;
			break;
		case 3:
			c1 = TRANS_SH2_OUT;
			c2 = TRANS_SH2_IN;
			break;
		}/* endswitch (i) */
		rad1 = 30;
		rad2 = 1;
		for (j = 0; j < 20; j ++)
		{
			clear_bitmap(tmp);

			rad2 += 2;
			if (j % 2 == 0)
				rad1 ++;

			if (j >= 18)
			{
				circle(tmp, rad1, rad1, rad1, c1);
			}/* endif (j >= 18) */
			else
			{
				circlefill(tmp, rad1, rad1, rad1, c2);
				circle(tmp, rad1, rad1, rad1, c1);
				if (rad2 > 1)
				{
					circlefill(tmp, rad1, rad1, rad2, 0);
					circle(tmp, rad1, rad1, rad2, c1);
				}
			}/* endelse (j >= 18) */

//    new_rle_struct(tmp, "mshockwaves rle", &RLE_mshock [i] [j], -1);
			extract_rle_struct2(tmp, &RLE_mshock [i] [j], 0, 0, rad1 * 2 + 1, rad1 * 2 + 1, -1);

		}/* endfor (j = 0; j < 20; j ++) */
	}/* endfor (i = 0; i < 4; i ++) */
	destroy_bitmap(tmp);
}

void prepare_lshockwaves(void)
{
	int i, j;

	BITMAP *tmp = new_bitmap(200, 200, "prepare_lshockwaves tmp bitmap");

	int rad1, rad2, c1 = 0, c2 = 0;
	i = 0;
	for (i = 0; i < 4; i ++)
	{
		switch (i)
		{
		case 0:
			c1 = TRANS_ORANGE_OUT;
			c2 = TRANS_ORANGE_IN;
			break;
		case 1:
			c1 = TRANS_YELLOW_OUT;
			c2 = TRANS_YELLOW_IN;
			break;
		case 2:
			c1 = TRANS_SH1_OUT;
			c2 = TRANS_SH1_IN;
			break;
		case 3:
			c1 = TRANS_SH2_OUT;
			c2 = TRANS_SH2_IN;
			break;
		}/* endswitch (i) */
		rad1 = 30;
		rad2 = 1;
		for (j = 0; j < 50; j ++)
		{
			clear_bitmap(tmp);

			if (j > 10)
				rad2 += 2;
			rad1 ++;


			/*    circlefill(tmp, 100, 100, rad1, c2);
			    circle(tmp, 100, 100, rad1, c1);
			    circlefill(tmp, 100, 100, rad2, 0);
			    circle(tmp, 100, 100, rad2, c1);

			    new_rle_struct(tmp, "large_shockwaves2 rle", &RLE_lshock [i] [j], -1);*/

			circlefill(tmp, rad1, rad1, rad1, c2);
			circle(tmp, rad1, rad1, rad1, c1);
			if (rad2 > 1)
			{
				circlefill(tmp, rad1, rad1, rad2, 0);
				circle(tmp, rad1, rad1, rad2, c1);
			}

			extract_rle_struct2(tmp, &RLE_lshock [i] [j], 0, 0, rad1 * 2 + 1, rad1 * 2 + 1, -1);

//    new_rle_struct(tmp, "large_shockwaves2 rle", &RLE_lshock [i] [j], -1);

		}/* endfor (j = 0; j < 50; j ++) */
	}/* endfor (i = 0; i < 4; i ++) */

	destroy_bitmap(tmp);
}



void prepare_hshockwaves(void)
{
	int i, j;

	BITMAP *tmp = new_bitmap(300, 300, "prepare_hshockwaves tmp bitmap");


	int rad1, rad2, c1 = 0, c2 = 0;
	i = 0;
	for (i = 0; i < 4; i ++)
	{
		switch (i)
		{
		case 0:
			c1 = TRANS_ORANGE_OUT;
			c2 = TRANS_ORANGE_IN;
			break;
		case 1:
			c1 = TRANS_YELLOW_OUT;
			c2 = TRANS_YELLOW_IN;
			break;
		case 2:
			c1 = TRANS_SH1_OUT;
			c2 = TRANS_SH1_IN;
			break;
		case 3:
			c1 = TRANS_SH2_OUT;
			c2 = TRANS_SH2_IN;
			break;
		}/* endswitch (i) */
		rad1 = 68;
		rad2 = 0;
		for (j = 0; j < 50; j ++)
		{
			clear_bitmap(tmp);

			if (j > 10)
				rad2 += 3;
			rad1 += 1;//2;


			circlefill(tmp, rad1, rad1, rad1, c2);
			circle(tmp, rad1, rad1, rad1, c1);
			if (rad2 > 1)
			{
				circlefill(tmp, rad1, rad1, rad2, 0);
				circle(tmp, rad1, rad1, rad2, c1);
			}

			extract_rle_struct2(tmp, &RLE_hshock [i] [j], 0, 0, rad1 * 2 + 1, rad1 * 2 + 1, -1);

		}/* endfor (j = 0; j < 50; j ++) */
	}/* endfor (i = 0; i < 4; i ++) */

	destroy_bitmap(tmp);
}


void prepare_rshockwaves(void)
{
	int i, j;

	BITMAP *tmp = new_bitmap(250, 250, "prepare_rshockwaves tmp bitmap");

	int rad1, rad2, c1, c2;
	i = 0;
	c1 = TRANS_ORANGE_OUT;
	c2 = TRANS_ORANGE_IN;

	for (i = 0; i < 5; i ++)
	{

		rad1 = 20 + i * 5;
		rad2 = 1 + i * 5;
		for (j = 0; j < 20; j ++)
		{
			clear_bitmap(tmp);

//    if (j % 2 == 0)
			rad1 += 3;
			rad2 += 4;

			if (j >= 18)
			{
				circle(tmp, rad1, rad1, rad1, c1);
			}/* endif (j >= 18) */
			else
			{
				circlefill(tmp, rad1, rad1, rad1, c2);
				circle(tmp, rad1, rad1, rad1, c1);
				if (rad2 > 1)
				{
					circlefill(tmp, rad1, rad1, rad2, 0);
					circle(tmp, rad1, rad1, rad2, c1);
				}
			}/* endelse (j >= 18) */

//    new_rle_struct(tmp, "mshockwaves rle", &RLE_mshock [i] [j], -1);
			extract_rle_struct2(tmp, &RLE_rshock [i] [j], 0, 0, rad1 * 2 + 1, rad1 * 2 + 1, -1);

		}
	}

	destroy_bitmap(tmp);
}

/*
 * A COMMENTED FUNCTION
 * void prepare_various_effects(void)
 * WAS HERE AND IS NOW REMOVED. 
 * CHECK THE SVN REPOSITORY IF YOU MISS IT
 */

/*
 * A COMMENTED FUNCTION
 * void prepare_underlay(void)
 * WAS HERE AND IS NOW REMOVED. 
 * CHECK THE SVN REPOSITORY IF YOU MISS IT
 */

/**
 * If bitmap allocation fails, terminate the program...
 * \param x Bitmap width
 * \param y Bitmap height
 * \param errtxt Error message if failed
 * \return Pointer to newly allocated bitmap
 */
BITMAP *new_bitmap(int x, int y, const char errtxt [])
{
	/* Normally create an Allegro bitmap */
	BITMAP *bmp = create_bitmap(x, y);
	/* Were we succesful? */
	if (bmp == NULL)
	{
		/* Bail out */
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		/* Tell the user what happened */
		allegro_message("Bitmap creation failed!  %s %s", allegro_error, errtxt);
		/* Run away... */
		exit(1);
	}
	/* Everything went right, return the bitmap */
	return bmp;
}

void bordered_triangle(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int col, int border_col)
{

	BITMAP *temp_bitmap = new_bitmap(target->w, target->h, "Bordered triangle temp_bitmap");

	clear_bitmap(temp_bitmap);

	triangle(temp_bitmap, x1, y1, x2, y2, x3, y3, col);

	coat_sprite(temp_bitmap, border_col);

	draw_sprite(target, temp_bitmap, 0, 0);

	destroy_bitmap(temp_bitmap);

}

/**
 * \param target What bitmap should we examine?
 * \param col How should we colour the border between black and non-black?
 * \param x1 Specify a sub-bitmap
 */
void coat_file_sprite_xy(BITMAP *target, int col, int x1, int y1, int x2, int y2)
{
	int i, j;
	/* go through all pixels in a subbitmap */
	for (i = x1; i < x2; i ++)
		for (j = y1; j < y2; j ++)
		{
			/* Black color here? */
			if (getpixel(target, i, j) == 0)
			{
				/* Are we on a border between black and non-coat color? */
				if (	   (getpixel(target, i - 1, j) != 0 && getpixel(target, i - 1, j) != col)
				        || (getpixel(target, i + 1, j) != 0 && getpixel(target, i + 1, j) != col)
				        || (getpixel(target, i, j - 1) != 0 && getpixel(target, i, j - 1) != col)
				        || (getpixel(target, i, j + 1) != 0 && getpixel(target, i, j + 1) != col))
					/* Color the formerly black color to coat color! */
					putpixel(target, i, j, col);
			}/* endif (getpixel(target, i, j) == 0) */
		}/* endfor (j = y1; j < y2; j ++) */
	/* Yup, we are finished! */
}

void coat_sprite(BITMAP *target, int col)
{
	coat_file_sprite_xy(target, col, 1, 1, target->w - 1, target->h - 1);
}

/**
 * \param target Where do we want to draw the bordered poly?
 * \param num_points How many points does the poly consist of?
 * \param col Color of the inside
 * \param border_col Color of the border
 */
void fast_bordered_poly(BITMAP * target, int num_points, const int * coords, int col, int border_col)
{
	/* Safely take a new bitmap... */
	BITMAP * temp_bitmap = new_bitmap(target->w, target->h, "Bordered poly4 temp_bitmap");
	clear_bitmap(temp_bitmap);
	/* Draw the inner polygon first */
	polygon(target, num_points, coords, col);
	/* And then come the borders */
	int i;
	for (i = 0; i < num_points - 1; i++)
		line(temp_bitmap, 
			coords[2 * i], coords[2 * i + 1], coords[2 * (i + 1)], coords[2 * (i + 1) + 1], 
			border_col);
	/* Closing the borders... */
	/* TODO: The borders are not 100% accurate! */
	line(temp_bitmap, coords[2 * (num_points - 1)], coords[2 * (num_points - 1) + 1], coords[0], coords[1], border_col);
	/* Output the result */
	draw_sprite(target, temp_bitmap, 0, 0);
	/* Return what is not needed any more... */
	destroy_bitmap(temp_bitmap);
	temp_bitmap = NULL;
}

BITMAP * load_up_bitmap(const char fname [])
{
	RGB temp_palette [256];

	BITMAP *temp_bitmap = load_bitmap(fname, temp_palette);
	if (temp_bitmap == NULL)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Failed to load in bitmap! (File not found?) %s", fname);
		exit(1);
	}

	return temp_bitmap;
}

/**
 * \param source The mother bitmap
 * \param x_source The X coord of the RLE
 * \param y_source The Y coord...
 * \param x Width of the result RLE
 * \param y Height of the result
 * \param flip_type Do we want to flip the bitmap? If so, how?
 * \return The RLE sprite extracted from source
 */
RLE_SPRITE * extract_flip_rle_sprite(BITMAP *source, int x_source, int y_source, int x, int y, int flip_type)
{
	BITMAP * tmp = new_bitmap(x, y, "extract_hflip_rle_sprite");
	BITMAP *tmp2 = new_bitmap(x, y, "extract_hflip_rle_sprite"); // Only one bitmap should be OK */

	clear_bitmap(tmp2);
	blit(source, tmp, x_source, y_source, 0, 0, x, y);

	switch (flip_type)
	{
		case FLIP_H:
			draw_sprite_h_flip(tmp2, tmp, 0, 0); 
			/* draw_sprite_h_flip(tmp, tmp, 0, 0); Really doesn't work :-( */
			break;
		case FLIP_V:
			draw_sprite_v_flip(tmp2, tmp, 0, 0);
			break;
		case FLIP_VH:
			draw_sprite_vh_flip(tmp2, tmp, 0, 0);
			break;
		case FLIP_NONE: /* do nothing if filp is FLIP_NONE */
			draw_sprite(tmp2, tmp, 0, 0);
			break;
	}/* endswitch (flip_type) */

	RLE_SPRITE * retval = get_rle_sprite(tmp2);

	if (retval == NULL)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Rle_sprite extraction failed!  %s %s", allegro_error, "extract_flip_rle_sprite", FLIP_NONE);
		exit(1);
	}

	destroy_bitmap(tmp);
	destroy_bitmap(tmp2);

	return retval;
}

/**
 * \param source The bitmap containing the RLE
 * \param x Width of the RLE in the bitmap
 * \param y Height...
 * \param errtxt What to tell if it fails
 */
RLE_SPRITE *sized_rle_sprite(BITMAP *source, int x, int y, const char errtxt [])
{
	BITMAP *tmp = new_bitmap(x, y, errtxt);

	/* We get the "subbitmap" of dimensions (x,y) */
	blit(source, tmp, 0, 0, 0, 0, x, y);

	RLE_SPRITE * retval = get_rle_sprite(tmp);

	if (retval == NULL)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Rle_sprite sized creation failed!  %s %s", allegro_error, errtxt);
		exit(1);
	}

	destroy_bitmap(tmp);

	return retval;
}

/**
 * Errors-aware version of the get_rle_sprite() function
 * \param source Passed to get_rle_sprite
 * \param errtxt What to tell if something goes wrong
 */
RLE_SPRITE *new_rle_sprite(BITMAP *source, const char errtxt [])
{
	RLE_SPRITE *retval = get_rle_sprite(source);
	if (retval == NULL)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Rle_sprite creation failed!  %s %s", allegro_error, errtxt);
		exit(1);
	}

	return retval;
}

/**
 * \param[in] source 
 * \param errtxt What to tell if things go wrong
 * \param[out] str 
 * \param bl unused here !!
 * \param index2 Index of an element of the str array
 * \param col_0 Some "first" color (or column?)
 * \param col_n "Last" col\<?something?\>
 */
void new_rle_struct_colours(BITMAP * source, const char errtxt [], struct RLE_STRUCT str [SMALL_ROTATIONS] [9], int bl, int index2, int col_0, int col_n)
{
	int i;
	//int count = 0; //fuckin unused!
	int trans_col = 0;

	for (i = col_0; i <= col_n; i ++)
	{
		trans_col = trans_converts[i];
		
		drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
		/* We fill the entire source with the transparent colour.
		 * Each time with a different one, though. */
		rectfill(source, 0, 0, source->w, source->h, trans_col);
		drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
		/* We do something to a RLE that is passed to us */
		new_rle_struct(source, errtxt, str [index2], i - col_0);
		//count ++;
	}
}

void extract_rle_struct2(BITMAP *source, struct RLE_STRUCT *str, int x_source, int y_source, int x, int y, int bl)
{

	BITMAP *tmp = new_bitmap(x, y, "extract_rle_struct");

	blit(source, tmp, x_source, y_source, 0, 0, x, y);

// rect(tmp, 0, 0, tmp->w - 1, tmp->h - 1, TRANS_YELLOW_OUT);

	if (bl == -1)
	{
		str->sprite = get_rle_sprite(tmp);
		str->x = tmp->w / 2;
		str->y = tmp->h / 2;
	}
	else
	{
		str [bl].sprite = get_rle_sprite(tmp);
		str [bl].x = tmp->w / 2;
		str [bl].y = tmp->h / 2;
	}

	if (str->sprite == NULL)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Rle_struct extraction failed!  %s %s", allegro_error, "extract_rle_struct");
		exit(1);
	}
	destroy_bitmap(tmp);
}

/**
 * \param source[in] The bitmap we want to make RLE from
 * \param errtxt The error message we tell to the user if something goes wrong
 * \param str[out] Pointer to RLE_STRUCT we want to make
 * \param bl How to behave to the RLE structure 
 *
 * We detect the subbitmap of source that doesn't contain any rows/columns containing only black.
 */
void new_rle_struct(BITMAP *source, const char errtxt [], struct RLE_STRUCT *str, int bl)
{
	int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
	/* We are going to need these if we want to vectorize the search for the clipping rectangle
	 * or whatever that is...  */
	int * bounds[4] = {& x1, & y1, & x2, & y2};
	int valid_bounds[4] = {0, 0, source->w, source->h};
	int start_i, start_j, end_i, end_j;
	int * x, * y;

	int i, j, n, n_2;
	/* 
	 * It seems that we have to find the rectangle - subsprite, that contains non-zero pixels
	 * Let's do it in a stylish way!
	 */
	for (n = 0; n < 4; n++)
	{/* First of all, we have to find four subsprite coords*/
		n_2 = n % 2;
		if (n_2 == 0)
		{/* Searching for the X bound */
			x = & i;	y = & j;
		}
		else
		{/* Searching for the Y bound */
			x = & j;	y = & i;
		}
		/* Setting boundaries for 'i' and 'j'. Basically the same as things in the previous 'if' block */
		start_i = valid_bounds[  + n_2];
		start_j = valid_bounds[1 - n_2];
		end_i = valid_bounds[2 + n_2];
		end_j = valid_bounds[3 - n_2];

		if (n / 2 == 0)
		{/* Checking for lower bounds */		
			for (i = start_i; i < end_i; i++) 
			{/* The 'i' value will be the bound value */
				for (j = start_j; j < end_j; j++) 
				{/* We are checking 'i' by scanning through all 'j's */
					/* give real coords to getpixel */
					if (getpixel(source, * x, * y) != 0)
					/* if (source->line[* y][* x] != 0) This causes esoteric problems */
						goto nonzero_found;
				}/* endfor (j = start_j; j < end_j; j++) */
			}/* endfor (i = start_i; i < end_i; i ++) */
		}/* endif (n / 2 == 0) */
		else
		{/* Checking for upper bounds => from the other side */
			for (i = end_i - 1; i >= start_i; i--)
			{/* The 'i' value will be the bound value */
				for (j = end_j - 1; j >= start_j; j--)
				{/* We are checking 'i' by scanning through all 'j's */
					/* give real coords to getpixel */
					if (getpixel(source, * x, * y) != 0)
					/* if (source->line[* y][* x] != 0) This causes esoteric problems */
						goto nonzero_found;
				}/* endfor (j = 0; j < end_j; j ++) */
			}/* endfor (i = 0; i < end_i; i ++) */
		}/* endelse (n / 2 == 0) */
		/* Damn, nothing has been found. Go again! */
		continue;
		nonzero_found:
		/* The fact that we are here means that we have found a nonzero value earlier...*/
		valid_bounds[n] = * bounds[n] = i;
	}/* endfor (n = 0; n < 4; n++) */

	/* Here we create a bitmap that will contain the nonzero subset of the original */
	BITMAP *tmp = new_bitmap(x2 - x1 + 1, y2 - y1 + 1, "new_rle_struct temp bitmap");
	/* and we fill it here */
	blit(source, tmp, x1, y1, 0, 0, x2 - x1 + 1, y2 - y1 + 1);

//rect(tmp, 0, 0, tmp->w - 1, tmp->h - 1, TRANS_RED_OUT);
	if (bl == -1)
	{
		str->sprite = get_rle_sprite(tmp);
		str->x = tmp->w / 2;//source->w / 2 - x1;
		str->y = tmp->h / 2;//source->h / 2 - y1;
	}/* endif (bl == -1) */
	else if (bl == -2)
	{
		str->sprite = get_rle_sprite(tmp);
		str->x = source->w / 2 - x1;
		str->y = source->h / 2 - y1;
	}/* endelif (bl == -2) */
	else
	{
		str[bl].sprite = get_rle_sprite(tmp);
		str[bl].x = tmp->w / 2; //source->w / 2 - x1;
		str[bl].y = tmp->h / 2; //source->h / 2 - y1;
	}/* endelelse(bl == -2) */
	/* The str is OK, we don't need tmp any more...  */
	destroy_bitmap(tmp);	tmp = NULL;
}

void fix_outline(BITMAP *source)
{
	int i, j, px;

	for (i = 0; i < source->w; i ++)
	{
		for (j = 0; j < source->h; j ++)
		{
			px = getpixel(source, i, j);
			switch (px)
			{
			case 116:
				putpixel(source, i, j, COL_OUTLINE);
				break;
			case 61:
				putpixel(source, i, j, COL_COL4);
				break;
			}/* endswitch (px) */
		}/* endfor (j = 0; j < source->h; j ++) */
	}/* endfor (i = 0; i < source->w; i ++) */
}

void fix_trans(BITMAP *source)
{
	int i, j, px;
	for (i = 0; i < source->w; i ++)
	{
		for (j = 0; j < source->h; j ++)
		{
			px = getpixel(source, i, j);
			switch (px)
			{
			case 51://COL_WHITE:
				putpixel(source, i, j, TRANS_ORANGE_OUT);
				break;
			case 30://COL_LGREY:
				putpixel(source, i, j, TRANS_ORANGE_IN);
				break;
			}/* endswitch (px) */
		}/* endfor (j = 0; j < source->h; j ++) */
	}/* endfor (i = 0; i < source->w; i ++) */
}

void extract_rle_struct(BITMAP *source, int x1, int y1, int x2, int y2, const char errtxt [], struct RLE_STRUCT *str, int bl)
{
	BITMAP *tmp = new_bitmap(x2, y2, "extract_rle_struct temp bitmap");
	blit(source, tmp, x1, y1, 0, 0, x2, y2);
	new_rle_struct(tmp, errtxt, str, bl);
	destroy_bitmap(tmp);
	tmp = NULL;
}

