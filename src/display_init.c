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

 
#include "config.h"

#include "allegro.h"

#include <math.h>

#include "globvars.h"
#include "palette.h"
#include "stuff.h"
#include "display.h"

enum
{
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

BITMAP *new_bitmap(int x, int y, const char errtxt []);

void bordered_poly4(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int col, int border_col);
void bordered_poly6(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int x6, int y6, int col, int border_col);
void bordered_triangle(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int col, int border_col);
void fast_bordered_poly4(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int col, int border_col, int fillx, int filly);
void fast_bordered_triangle(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int col, int border_col, int fillx, int filly);
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

void prepare_various_effects(void);

void fix_outline(BITMAP *source);
void fix_trans(BITMAP *source);
void coat_sprite(BITMAP *target, int col);
void coat_file_sprite_xy(BITMAP *target, int col, int x1, int y1, int x2, int y2);
void prepare_flowers(void);
void extract_rle_struct(BITMAP *source, int x1, int y1, int x2, int y2, const char errtxt [], struct RLE_STRUCT *str, int bl);
void extract_rle_struct2(BITMAP *source, struct RLE_STRUCT *str, int x_source, int y_source, int x, int y, int b1);
void prepare_underlay(void);

void new_rle_struct_colours(BITMAP *source, const char errtxt [], struct RLE_STRUCT str [SMALL_ROTATIONS] [9], int bl, int index2, int col1, int col2);


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


BITMAP *load_up_bitmap(const char fname []);
RLE_SPRITE *extract_rle_sprite(BITMAP *source, int x_source, int y_source, int x, int y);
RLE_SPRITE *extract_flip_rle_sprite(BITMAP *source, int x_source, int y_source, int x, int y, int flip_type);
RLE_SPRITE *sized_rle_sprite(BITMAP *source, int x, int y, const char errtxt []);
RLE_SPRITE *new_rle_sprite(BITMAP *source, const char errtxt []);


/*

Basic display init functions.

*/

/*
Call during start-up.
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

 DATAFILE *datf = load_datafile(DIRECTORY(DATADIR,gfx/garden.dat));
 if (datf == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Error: Couldn't find data.dat (expected it at %s)! 
", DIRECTORY(DATADIR,gfx/garden.dat) );
  allegro_message("
");
  exit(1);
 }
// int i, j;

 font = (FONT *)datf[0].dat;

/*

splash bitmap

*/

 RGB splash_palette [256];


 BITMAP *splash_bitmap = load_bitmap(DIRECTORY(DATADIR,gfx/splash.bmp), splash_palette);

 if (splash_bitmap == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Failed to load in bitmap! (File not found?)  
%s", "splash.bmp");
  exit(1);
 }

/* BITMAP *wait_bitmap = load_bitmap(DIRECTORY(DATADIR,gfx/wait.bmp), splash_palette);

 if (wait_bitmap == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Failed to load in bitmap! (File not found?)  
%s", "wait.bmp");
  exit(1);
 }*/


 vsync();
 set_palette(splash_palette);

 blit(splash_bitmap, screen, 0, 0, 0, 0, 640, 480);
// blit(wait_bitmap, screen, 0, 0, 0, 0, 440, 300);

 destroy_bitmap(splash_bitmap);

/*

end splash

*/

// define_palette(0);

// textprintf_right_ex(screen, font, 600, 200, -1, -1, "LOADING");
// textprintf_right_ex(screen, font, 600, 240, -1, -1, "PLEASE WAIT");
// textprintf_ex(screen, font, 100, 120, -1, -1, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

//#define FIX_FONT

#ifdef FIX_FONT

 int i, j;

 BITMAP *font_bitmap = load_up_bitmap("g_font4.bmp");

/*
 do
 {
  i ++;
 } while(key [KEY_SPACE] == 0);*/



 int back_col = getpixel(font_bitmap, 0, 0);
 int back_col2 = getpixel(font_bitmap, 1, 0);
 int out_col = getpixel(font_bitmap, 4, 17);
 int white_col = getpixel(font_bitmap, 4, 18);
 int lgrey_col = getpixel(font_bitmap, 8, 19);

 for (i = 0; i < font_bitmap->w; i ++)
 {
  for (j = 0; j < font_bitmap->h; j ++)
  {
   if (getpixel(font_bitmap, i, j) == back_col
       || getpixel(font_bitmap, i, j) == back_col2)
   {
       putpixel(font_bitmap, i, j, 255);
       continue;
   }
   if (getpixel(font_bitmap, i, j) == out_col)
   {
       putpixel(font_bitmap, i, j, COL_OUTLINE);
       continue;
   }
   if (getpixel(font_bitmap, i, j) == white_col)
   {
       putpixel(font_bitmap, i, j, COL_WHITE);
       continue;
   }
   if (getpixel(font_bitmap, i, j) == lgrey_col)
   {
       putpixel(font_bitmap, i, j, COL_LGREY);
       continue;
   }
  }
 }

 save_bitmap("g_font3.bmp", font_bitmap, palet [0]);

#endif

/*
 for (i = 0; i < 13; i ++)
 {
  for (j = 0; j < 19; j ++)
  {
     rectfill(screen, i * 10, j * 10, i * 10 + 10, j * 10 + 10, j * 13 + i + 1);
  }
 }



circlefill(screen, 250, 250, 100, COL_COL2);
circle(screen, 250, 250, 100, COL_OUTLINE);

      drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);

 rectfill(screen, 200, 10, 300, 200, TRANS_ORANGE_IN);
 rect(screen, 199, 9, 301, 201, TRANS_ORANGE_OUT);

 rectfill(screen, 210, 20, 410, 110, TRANS_B1_IN);
 rect(screen, 209, 19, 411, 111, TRANS_B1_OUT);

 rectfill(screen, 320, 10, 400, 230, TRANS_ORANGE_IN);
 rect(screen, 319, 9, 401, 231, TRANS_ORANGE_OUT);

 rectfill(screen, 310, 120, 450, 230, TRANS_ORANGE_IN);
 rect(screen, 309, 119, 451, 231, TRANS_ORANGE_OUT);

*/
      drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);



 prepare_various_effects();
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


 BITMAP *file_bitmap = load_up_bitmap(DIRECTORY(DATADIR,gfx/small.bmp));

 fix_outline(file_bitmap);

    eRLE_small1 [S_ENEMY_SWOOPER1_1] = extract_rle_sprite(file_bitmap, 1, 1, 31, 32);
    eRLE_small1 [S_ENEMY_SWOOPER1_2] = extract_rle_sprite(file_bitmap, 33, 1, 33, 32);
    eRLE_small1 [S_ENEMY_SWOOPER1_3] = extract_rle_sprite(file_bitmap, 67, 1, 35, 32);
    eRLE_small1 [S_ENEMY_SWOOPER1_4] = extract_rle_sprite(file_bitmap, 103, 1, 37, 34);

    eRLE_small1 [S_ENEMY_DARTER1_MAIN] = extract_rle_sprite(file_bitmap, 141, 1, 49, 51);
    eRLE_small1 [S_ENEMY_DARTER1_CENTRE] = extract_rle_sprite(file_bitmap, 219, 1, 27, 28);
    eRLE_small1 [S_ENEMY_DARTER1_TOP] = extract_rle_sprite(file_bitmap, 191, 1, 27, 14);
    eRLE_small1 [S_ENEMY_DARTER1_INNER_L] = extract_rle_sprite(file_bitmap, 192, 16, 14, 41);
    eRLE_small1 [S_ENEMY_DARTER1_OUTER_L] = extract_rle_sprite(file_bitmap, 207, 16, 9, 31);
    eRLE_small1 [S_ENEMY_DARTER1_INNER_R] = extract_flip_rle_sprite(file_bitmap, 192, 16, 14, 41, FLIP_H);
    eRLE_small1 [S_ENEMY_DARTER1_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 207, 16, 9, 31, FLIP_H);

    player_icon_RLE = extract_rle_sprite(file_bitmap, 277, 38, 23, 31);

    eRLE_small1 [S_ENEMY_PLAYER] = extract_rle_sprite(file_bitmap, 219, 30, 21, 18);
    eRLE_small1 [S_ENEMY_PLAYER_UP_L] = extract_rle_sprite(file_bitmap, 242, 32, 10, 8);
    eRLE_small1 [S_ENEMY_PLAYER_UP_R] = extract_flip_rle_sprite(file_bitmap, 242, 32, 10, 8, FLIP_H);
    eRLE_small1 [S_ENEMY_PLAYER_DOWN_L] = extract_rle_sprite(file_bitmap, 242, 41, 9, 20);
    eRLE_small1 [S_ENEMY_PLAYER_DOWN_R] = extract_flip_rle_sprite(file_bitmap, 242, 41, 9, 20, FLIP_H);
    eRLE_small1 [S_ENEMY_PLAYER1] = extract_rle_sprite(file_bitmap, 249, 1, 15, 10);
    eRLE_small1 [S_ENEMY_PLAYER2] = extract_rle_sprite(file_bitmap, 265, 1, 17, 11);
    eRLE_small1 [S_ENEMY_PLAYER3] = extract_rle_sprite(file_bitmap, 283, 1, 19, 12);

    eRLE_small1 [S_ENEMY_DIVER1_CENTRE] = extract_rle_sprite(file_bitmap, 325, 35, 35, 43);
    eRLE_small1 [S_ENEMY_DIVER1_INNER_L] = extract_rle_sprite(file_bitmap, 314, 35, 10, 42);
    eRLE_small1 [S_ENEMY_DIVER1_OUTER_L] = extract_rle_sprite(file_bitmap, 304, 37, 9, 13);
    eRLE_small1 [S_ENEMY_DIVER1_INNER_R] = extract_flip_rle_sprite(file_bitmap, 314, 35, 10, 42, FLIP_H);
    eRLE_small1 [S_ENEMY_DIVER1_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 304, 37, 9, 13, FLIP_H);

    eRLE_small1 [S_ENEMY_C1P1_CORE] = extract_rle_sprite(file_bitmap, 368, 38, 25, 25);
    eRLE_small1 [S_ENEMY_C1P1_TOP] = extract_rle_sprite(file_bitmap, 395, 38, 17, 5);
    eRLE_small1 [S_ENEMY_C1P1_BOTTOM1] = extract_rle_sprite(file_bitmap, 395, 57, 23, 6);
    eRLE_small1 [S_ENEMY_C1P1_BOTTOM2] = extract_rle_sprite(file_bitmap, 395, 64, 17, 5);
    eRLE_small1 [S_ENEMY_C1P1_MIDL] = extract_rle_sprite(file_bitmap, 395, 44, 13, 12);
    eRLE_small1 [S_ENEMY_C1P1_MIDR] = extract_flip_rle_sprite(file_bitmap, 395, 44, 13, 12, FLIP_H);

    eRLE_small1 [S_ENEMY_SWOOPER2_1] = extract_rle_sprite(file_bitmap, 458, 47, 41, 43);
    eRLE_small1 [S_ENEMY_SWOOPER2_2] = extract_rle_sprite(file_bitmap, 500, 47, 43, 42);
    eRLE_small1 [S_ENEMY_SWOOPER2_3] = extract_rle_sprite(file_bitmap, 544, 47, 45, 41);
    eRLE_small1 [S_ENEMY_SWOOPER2_4] = extract_rle_sprite(file_bitmap, 590, 47, 47, 41);

    eRLE_small1 [S_ENEMY_DARTER2_CORE] = extract_rle_sprite(file_bitmap, 689, 9, 25, 25);
    eRLE_small1 [S_ENEMY_DARTER2_TOP] = extract_rle_sprite(file_bitmap, 675, 5, 13, 21);
    eRLE_small1 [S_ENEMY_DARTER2_BOTTOM] = extract_rle_sprite(file_bitmap, 675, 31, 13, 16);
    eRLE_small1 [S_ENEMY_DARTER2_INNER_L] = extract_rle_sprite(file_bitmap, 660, 2, 13, 48);
    eRLE_small1 [S_ENEMY_DARTER2_OUTER_L] = extract_rle_sprite(file_bitmap, 638, 2, 16, 48);
    eRLE_small1 [S_ENEMY_DARTER2_INNER_R] = extract_flip_rle_sprite(file_bitmap, 660, 2, 13, 48, FLIP_H);
    eRLE_small1 [S_ENEMY_DARTER2_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 638, 2, 16, 48, FLIP_H);

    eRLE_small1 [S_ENEMY_SMALL_OUTER_L] = extract_rle_sprite(file_bitmap, 544, 3, 15, 29);
    eRLE_small1 [S_ENEMY_SMALL_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 544, 3, 15, 29, FLIP_H);
    eRLE_small1 [S_ENEMY_SMALL_CORE1] = extract_rle_sprite(file_bitmap, 561, 2, 15, 19);
    eRLE_small1 [S_ENEMY_SMALL_CORE2] = extract_rle_sprite(file_bitmap, 561, 21, 15, 19);

    eRLE_small1 [S_ENEMY_LARGER_OUTER_L] = extract_rle_sprite(file_bitmap, 578, 4, 18, 35);
    eRLE_small1 [S_ENEMY_LARGER_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 578, 4, 18, 35, FLIP_H);
    eRLE_small1 [S_ENEMY_LARGER_CORE1] = extract_rle_sprite(file_bitmap, 597, 2, 21, 18);
    eRLE_small1 [S_ENEMY_LARGER_CORE2] = extract_rle_sprite(file_bitmap, 597, 20, 21, 18);

    eRLE_small1 [S_ENEMY_WINGS_OUTER_L] = extract_rle_sprite(file_bitmap, 716, 4, 22, 43);
    eRLE_small1 [S_ENEMY_WINGS_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 716, 4, 22, 43, FLIP_H);
    eRLE_small1 [S_ENEMY_WINGS_CORE1] = extract_rle_sprite(file_bitmap, 739, 3, 23, 32);
    eRLE_small1 [S_ENEMY_WINGS_CORE2] = extract_rle_sprite(file_bitmap, 739, 36, 23, 32);

    eRLE_small1 [S_ENEMY_MEGA_OUTER_L] = extract_rle_sprite(file_bitmap, 776, 4, 39, 76);
    eRLE_small1 [S_ENEMY_MEGA_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 776, 4, 39, 76, FLIP_H);
    eRLE_small1 [S_ENEMY_MEGA_CORE] = extract_rle_sprite(file_bitmap, 816, 4, 43, 62);

    eRLE_small1 [S_ENEMY_BOSS3_CORE] = extract_rle_sprite(file_bitmap, 1030, 1, 43, 62);

    eRLE_small1 [S_ENEMY_DRIFTER1_CORE] = extract_rle_sprite(file_bitmap, 638, 53, 39, 31);
    eRLE_small1 [S_ENEMY_DRIFTER1_L] = extract_rle_sprite(file_bitmap, 678, 64, 20, 19);
    eRLE_small1 [S_ENEMY_DRIFTER1_R] = extract_flip_rle_sprite(file_bitmap, 678, 64, 20, 19, FLIP_H);

    eRLE_small1 [S_ENEMY_BEAMER_CORE] = extract_rle_sprite(file_bitmap, 862, 12, 39, 42);
    eRLE_small1 [S_ENEMY_BEAMER_L] = extract_rle_sprite(file_bitmap, 901, 19, 35, 42);
    eRLE_small1 [S_ENEMY_BEAMER_R] = extract_flip_rle_sprite(file_bitmap, 901, 19, 35, 42, FLIP_H);
    eRLE_small1 [S_ENEMY_BEAMER_U] = extract_rle_sprite(file_bitmap, 904, 3, 23, 15);
    eRLE_small1 [S_ENEMY_BEAMER_D] = extract_rle_sprite(file_bitmap, 873, 56, 19, 12);

/*    eRLE_small1 [S_ENEMY_RISER1_CORE] = extract_rle_sprite(file_bitmap, 911, 69, 11, 15);
    eRLE_small1 [S_ENEMY_RISER1_L] = extract_rle_sprite(file_bitmap, 894, 63, 16, 29);
    eRLE_small1 [S_ENEMY_RISER1_R] = extract_flip_rle_sprite(file_bitmap, 894, 63, 16, 29, FLIP_H);*/

    eRLE_small1 [S_ENEMY_RISER1_CORE] = extract_rle_sprite(file_bitmap, 905, 62, 27, 30);
    eRLE_small1 [S_ENEMY_RISER1_L] = extract_rle_sprite(file_bitmap, 894, 62, 11, 23);
    eRLE_small1 [S_ENEMY_RISER1_R] = extract_flip_rle_sprite(file_bitmap, 894, 62, 11, 23, FLIP_H);

    eRLE_small1 [S_ENEMY_DRIFTER2_CORE] = extract_rle_sprite(file_bitmap, 936, 2, 39, 31);
    eRLE_small1 [S_ENEMY_DRIFTER2_L] = extract_rle_sprite(file_bitmap, 976, 3, 16, 39);
    eRLE_small1 [S_ENEMY_DRIFTER2_R] = extract_flip_rle_sprite(file_bitmap, 976, 3, 16, 39, FLIP_H);

    eRLE_small1 [S_ENEMY_ESCORT_CORE] = extract_rle_sprite(file_bitmap, 934, 57, 41, 25);
    eRLE_small1 [S_ENEMY_ESCORT_L] = extract_rle_sprite(file_bitmap, 977, 58, 16, 25);
    eRLE_small1 [S_ENEMY_ESCORT_R] = extract_flip_rle_sprite(file_bitmap, 977, 58, 16, 25, FLIP_H);

    eRLE_small1 [S_ENEMY_SHOOTER_L] = extract_rle_sprite(file_bitmap, 995, 17, 33, 42);
    eRLE_small1 [S_ENEMY_SHOOTER_R] = extract_flip_rle_sprite(file_bitmap, 995, 17, 33, 42, FLIP_H);
    eRLE_small1 [S_ENEMY_SHOOTER_U] = extract_rle_sprite(file_bitmap, 998, 1, 23, 14);

    eRLE_small1 [S_ENEMY_HOOK3] = extract_rle_sprite(file_bitmap, 1075, 1, 33, 32);
    eRLE_small1 [S_ENEMY_HOOK2] = extract_rle_sprite(file_bitmap, 1109, 1, 33, 32);
    eRLE_small1 [S_ENEMY_HOOK1] = extract_rle_sprite(file_bitmap, 1143, 1, 33, 33);

    eRLE_small1 [S_ENEMY_DROPPER1] = extract_rle_sprite(file_bitmap, 1080, 35, 29, 43);
    eRLE_small1 [S_ENEMY_DROPPER2] = extract_rle_sprite(file_bitmap, 1110, 34, 29, 44);
    eRLE_small1 [S_ENEMY_DROPPER3] = extract_rle_sprite(file_bitmap, 1140, 34, 29, 45);

    eRLE_small1 [S_ENEMY_RING1] = extract_rle_sprite(file_bitmap, 1177, 1, 49, 49);
    eRLE_small1 [S_ENEMY_RING2] = extract_rle_sprite(file_bitmap, 1227, 0, 51, 51);
    eRLE_small1 [S_ENEMY_RING3] = extract_rle_sprite(file_bitmap, 1279, 0, 53, 53);

    eRLE_small1 [S_ENEMY_HUNTER3] = extract_rle_sprite(file_bitmap, 1333, 1, 57, 51);
    eRLE_small1 [S_ENEMY_HUNTER2] = extract_rle_sprite(file_bitmap, 1391, 0, 57, 52);
    eRLE_small1 [S_ENEMY_HUNTER1] = extract_rle_sprite(file_bitmap, 1449, 0, 57, 53);

    eRLE_small1 [S_ENEMY_HOOK2_3] = extract_rle_sprite(file_bitmap, 1170, 56, 35, 35);
    eRLE_small1 [S_ENEMY_HOOK2_2] = extract_rle_sprite(file_bitmap, 1206, 56, 35, 35);
    eRLE_small1 [S_ENEMY_HOOK2_1] = extract_rle_sprite(file_bitmap, 1242, 56, 35, 35);


    destroy_bitmap(file_bitmap);

}
/*
void prepare_icon_rles(void)
{

 BITMAP *file_bitmap = load_up_bitmap(DIRECTORY(DATADIR,gfx/icons.bmp));

 fix_outline(file_bitmap);
  int i;

  for (i = 0; i < 11; i ++)
  {

    icon_RLEs [i] = extract_rle_sprite(file_bitmap, 1 + i * 41, 1, 40, 40);

  }

    destroy_bitmap(file_bitmap);

}
*/
//#define FIXING

void prepare_l_enemy_rles(void)
{

//#define FIXING
// add_outline(file_bitmap, 0, 0);
#ifdef FIXING

 RGB temp_palette [256];

 BITMAP *temp_bitmap = load_bitmap(DIRECTORY(DATADIR,gfx/large.bmp), temp_palette);

// set_palette(temp_palette);

 int col = getpixel(temp_bitmap, 2058, 23);

 coat_file_sprite_xy(temp_bitmap, col, 2200, 0, 2474, 199);

 save_bitmap("olarge.bmp", temp_bitmap, temp_palette);

#endif

 BITMAP *file_bitmap = load_up_bitmap(DIRECTORY(DATADIR,gfx/large.bmp));

 fix_outline(file_bitmap);



    eRLE_large1 [L_ENEMY_STOPPER1] = extract_rle_sprite(file_bitmap, 1, 1, 93, 146);

    eRLE_large1 [L_ENEMY_STOPPER1_FRONT] = extract_rle_sprite(file_bitmap, 100, 1, 43, 20);
    eRLE_large1 [L_ENEMY_STOPPER1_CENTRE] = extract_rle_sprite(file_bitmap, 130, 100, 91, 95);
    eRLE_large1 [L_ENEMY_STOPPER1_F_L] = extract_rle_sprite(file_bitmap, 100, 23, 27, 29);
    eRLE_large1 [L_ENEMY_STOPPER1_F_R] = extract_flip_rle_sprite(file_bitmap, 100, 23, 27, 29, FLIP_H);
    eRLE_large1 [L_ENEMY_STOPPER1_M_L] = extract_rle_sprite(file_bitmap, 97, 53, 33, 86);
    eRLE_large1 [L_ENEMY_STOPPER1_M_R] = extract_flip_rle_sprite(file_bitmap, 97, 53, 33, 86, FLIP_H);
    eRLE_large1 [L_ENEMY_STOPPER1_B_L] = extract_rle_sprite(file_bitmap, 132, 21, 22, 77);
    eRLE_large1 [L_ENEMY_STOPPER1_B_R] = extract_flip_rle_sprite(file_bitmap, 132, 21, 22, 77, FLIP_H);

    eRLE_large1 [L_ENEMY_CARRIER1_CORE] = extract_rle_sprite(file_bitmap, 224, 6, 137, 154);
    eRLE_large1 [L_ENEMY_CARRIER1_TOP1] = extract_rle_sprite(file_bitmap, 366, 1, 65, 12);
    eRLE_large1 [L_ENEMY_CARRIER1_TOP2] = extract_rle_sprite(file_bitmap, 366, 14, 105, 40);
    eRLE_large1 [L_ENEMY_CARRIER1_TOP3] = extract_rle_sprite(file_bitmap, 365, 55, 129, 44);
    eRLE_large1 [L_ENEMY_CARRIER1_MIDL1] = extract_rle_sprite(file_bitmap, 364, 100, 51, 31);
    eRLE_large1 [L_ENEMY_CARRIER1_MIDL2] = extract_rle_sprite(file_bitmap, 423, 99, 49, 31);
    eRLE_large1 [L_ENEMY_CARRIER1_MIDR1] = extract_flip_rle_sprite(file_bitmap, 364, 100, 51, 31, FLIP_H);
    eRLE_large1 [L_ENEMY_CARRIER1_MIDR2] = extract_flip_rle_sprite(file_bitmap, 423, 99, 49, 31, FLIP_H);
    eRLE_large1 [L_ENEMY_CARRIER1_BOTTOM1] = extract_rle_sprite(file_bitmap, 367, 131, 125, 37);
    eRLE_large1 [L_ENEMY_CARRIER1_BOTTOM2] = extract_rle_sprite(file_bitmap, 378, 168, 93, 31);

    eRLE_large1 [L_ENEMY_POD1_CORE] = extract_rle_sprite(file_bitmap, 555, 101, 65, 94);
    eRLE_large1 [L_ENEMY_POD1_BODYL] = extract_rle_sprite(file_bitmap, 500, 48, 53, 90);
    eRLE_large1 [L_ENEMY_POD1_BODYR] = extract_flip_rle_sprite(file_bitmap, 500, 48, 53, 90, FLIP_H);
    eRLE_large1 [L_ENEMY_POD1_TOPC] = extract_rle_sprite(file_bitmap, 528, 139, 19, 36);
    eRLE_large1 [L_ENEMY_POD1_TOPL] = extract_rle_sprite(file_bitmap, 500, 139, 22, 44);
    eRLE_large1 [L_ENEMY_POD1_TOPR] = extract_flip_rle_sprite(file_bitmap, 500, 139, 22, 44, FLIP_H);

    eRLE_large1 [L_ENEMY_CRUISER1] = extract_rle_sprite(file_bitmap, 633, 17, 99, 125);

    eRLE_large1 [L_ENEMY_CRUISER2] = extract_rle_sprite(file_bitmap, 744, 4, 111, 60);

    eRLE_large1 [L_ENEMY_CRUISER3] = extract_rle_sprite(file_bitmap, 748, 68, 119, 120);

    eRLE_large1 [L_ENEMY_CRUISER4] = extract_rle_sprite(file_bitmap, 869, 3, 95, 95);

    eRLE_large1 [L_ENEMY_BOSS1_CORE] = extract_rle_sprite(file_bitmap, 976, 12, 155, 95);
    eRLE_large1 [L_ENEMY_BOSS1_UL] = extract_rle_sprite(file_bitmap, 957, 119, 102, 63);
    eRLE_large1 [L_ENEMY_BOSS1_UR] = extract_flip_rle_sprite(file_bitmap, 957, 119, 102, 63, FLIP_H);
    eRLE_large1 [L_ENEMY_BOSS1_DR] = extract_flip_rle_sprite(file_bitmap, 957, 119, 102, 63, FLIP_VH);
    eRLE_large1 [L_ENEMY_BOSS1_DL] = extract_flip_rle_sprite(file_bitmap, 957, 119, 102, 63, FLIP_V);

    eRLE_large1 [L_ENEMY_BOSS1_OUT_L] = extract_rle_sprite(file_bitmap, 888, 122, 58, 65);
    eRLE_large1 [L_ENEMY_BOSS1_OUT_R] = extract_flip_rle_sprite(file_bitmap, 888, 122, 58, 65, FLIP_H);

    eRLE_large1 [L_ENEMY_BOSS1_U] = extract_rle_sprite(file_bitmap, 1069, 118, 69, 31);
    eRLE_large1 [L_ENEMY_BOSS1_D] = extract_flip_rle_sprite(file_bitmap, 1069, 118, 69, 31, FLIP_V);

    eRLE_large1 [L_ENEMY_BOSS1_L] = extract_rle_sprite(file_bitmap, 1146, 127, 31, 67);
    eRLE_large1 [L_ENEMY_BOSS1_R] = extract_flip_rle_sprite(file_bitmap, 1146, 127, 31, 67, FLIP_H);

    eRLE_large1 [L_ENEMY_BOSS2_CORE] = extract_rle_sprite(file_bitmap, 1255, 36, 55, 73);

    eRLE_large1 [L_ENEMY_BOSS2_IN_L] = extract_rle_sprite(file_bitmap, 1182, 20, 70, 105);
    eRLE_large1 [L_ENEMY_BOSS2_IN_R] = extract_flip_rle_sprite(file_bitmap, 1182, 20, 70, 105, FLIP_H);

    eRLE_large1 [L_ENEMY_BOSS2_OUT_L] = extract_rle_sprite(file_bitmap, 1143, 29, 32, 58);
    eRLE_large1 [L_ENEMY_BOSS2_OUT_R] = extract_flip_rle_sprite(file_bitmap, 1143, 29, 32, 58, FLIP_H);

    eRLE_large1 [L_ENEMY_WALKER2] = extract_rle_sprite(file_bitmap, 1312, 2, 153, 127);
    eRLE_large1 [L_ENEMY_WALKER1] = extract_rle_sprite(file_bitmap, 1466, 2, 153, 127);
    eRLE_large1 [L_ENEMY_WALKER3] = extract_rle_sprite(file_bitmap, 1620, 2, 153, 127);

    eRLE_large1 [L_ENEMY_SPINNER3] = extract_rle_sprite(file_bitmap, 1774, 3, 105, 166);
    eRLE_large1 [L_ENEMY_SPINNER2] = extract_rle_sprite(file_bitmap, 1882, 2, 105, 168);
    eRLE_large1 [L_ENEMY_SPINNER1] = extract_rle_sprite(file_bitmap, 1988, 1, 105, 170);

    eRLE_large1 [L_ENEMY_BOSS4] = extract_rle_sprite(file_bitmap, 2098, 0, 229, 202);


    destroy_bitmap(file_bitmap);

}

void prepare_trans_rles(void)
{


 BITMAP *file_bitmap = load_up_bitmap(DIRECTORY(DATADIR,gfx/trans.bmp));

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

// trans_RLE [RLE_TRANS_MGBULLET1] = extract_rle_sprite(file_bitmap, 1, 1, 5, 23);

/*    trans_RLE [RLE_TRANS_MGBULLET1] = extract_rle_sprite(file_bitmap, 1, 1, 5, 23);
    trans_RLE [RLE_TRANS_MGBULLET2] = extract_rle_sprite(file_bitmap, 7, 1, 5, 23);
    trans_RLE [RLE_TRANS_MGBULLET3] = extract_rle_sprite(file_bitmap, 13, 1, 5, 23);
    trans_RLE [RLE_TRANS_MGBULLET4] = extract_rle_sprite(file_bitmap, 19, 1, 5, 23);*/

    destroy_bitmap(file_bitmap);

}


void prepare_enemy_bullets(void)
{
 int i;//, o;

 BITMAP *temp_bitmap;

 temp_bitmap = new_bitmap(50, 50, "prepare_glass_bullets");

 int inc = ANGLE_1 / SMALL_ROTATIONS;
 int angle;
 int col = TRANS_B1_IN, border = TRANS_B1_OUT;

/* for (o = 0; o < 3; o ++)
 {
    switch(o)
    {
        default:
     case 0: col = TRANS_RED_IN; border = TRANS_RED_OUT; break;
     case 1: col = TRANS_GREEN_IN; border = TRANS_GREEN_OUT; break;
     case 2: col = TRANS_BLUE_IN; border = TRANS_BLUE_OUT; break;
    }*/
 for (i = 0; i < SMALL_ROTATIONS; i ++)
 {
  clear_bitmap(temp_bitmap);
  angle = i * inc;

bordered_poly4(temp_bitmap,
  25 + xpart(angle, 23), 25 + ypart(angle, 23),
  25 + xpart(angle + ANGLE_8, 11), 25 + ypart(angle + ANGLE_8, 11),
  25, 25,
  25 + xpart(angle - ANGLE_8, 11), 25 + ypart(angle - ANGLE_8, 11),
  col, border);
/*
 bordered_poly4(temp_bitmap,
  25 + xpart(angle, 12), 25 + ypart(angle, 12),
  25 + xpart(angle + ANGLE_4 + ANGLE_8, 12), 25 + ypart(angle + ANGLE_4 + ANGLE_8, 12),
  25 - xpart(angle, 15), 25 - ypart(angle, 15),
  25 + xpart(angle - ANGLE_4 - ANGLE_8, 12), 25 + ypart(angle - ANGLE_4 - ANGLE_8, 12),
  col, border);*/

/* bordered_triangle(temp_bitmap,
  15 + xpart(angle, 12), 15 + ypart(angle, 12),
  15 + xpart(angle + ANGLE_4 + ANGLE_8, 12), 15 + ypart(angle + ANGLE_4 + ANGLE_8, 12),
  15 + xpart(angle - ANGLE_4 - ANGLE_8, 12), 15 + ypart(angle - ANGLE_4 - ANGLE_8, 12),
  col, border);*/

//  new_rle_struct(temp_bitmap, "prepare dart bullets", &dart_bullet [o] [i], -1);

  new_rle_struct_colours(temp_bitmap, "prepare dart bullets", dart_bullet, -1, i, TCOL_B1, TCOL_B5);

// Now let's make small darts:

  clear_bitmap(temp_bitmap);
  angle = i * inc;

/* bordered_poly4(temp_bitmap,
  25 + xpart(angle, 20), 25 + ypart(angle, 20),
  25 + xpart(angle + ANGLE_4, 12), 25 + ypart(angle + ANGLE_4, 12),
  25 - xpart(angle, 20), 25 - ypart(angle, 20),
  25 + xpart(angle - ANGLE_4, 12), 25 + ypart(angle - ANGLE_4, 12),
  col, border);*/

  bordered_poly4(temp_bitmap,
  25 + xpart(angle, 17), 25 + ypart(angle, 17),
  25 + xpart(angle + ANGLE_8, 7), 25 + ypart(angle + ANGLE_8, 7),
  25, 25,
  25 + xpart(angle - ANGLE_8, 7), 25 + ypart(angle - ANGLE_8, 7),
  col, border);



  new_rle_struct_colours(temp_bitmap, "prepare sdart bullets", sdart_bullet, -1, i, TCOL_B1, TCOL_B5);




// Now let's make diamond bullets:

  clear_bitmap(temp_bitmap);
  angle = i * inc;

 bordered_poly4(temp_bitmap,
  25 + xpart(angle, 15), 25 + ypart(angle, 15),
  25 + xpart(angle + ANGLE_4, 9), 25 + ypart(angle + ANGLE_4, 9),
  25 - xpart(angle, 15), 25 - ypart(angle, 15),
  25 + xpart(angle - ANGLE_4, 9), 25 + ypart(angle - ANGLE_4, 9),
  col, border);
/*
 bordered_poly4(temp_bitmap,
  25 + xpart(angle, 20), 25 + ypart(angle, 20),
  25 + xpart(angle + ANGLE_4, 12), 25 + ypart(angle + ANGLE_4, 12),
  25 - xpart(angle, 20), 25 - ypart(angle, 20),
  25 + xpart(angle - ANGLE_4, 12), 25 + ypart(angle - ANGLE_4, 12),
  col, border);
*/
//  circlefill(temp_bitmap, 25, 25, 3, border);

//  new_rle_struct(temp_bitmap, "prepare diamond bullets", &diamond_bullet [o] [i], -1);
  new_rle_struct_colours(temp_bitmap, "prepare diamond bullets", diamond_bullet, -1, i, TCOL_B1, TCOL_B5);


 }

 destroy_bitmap(temp_bitmap);

// now let's do circles!!
 int j;

 for (i = 0; i < RLE_CIRCLES; i ++)
 {
  temp_bitmap = new_bitmap((i + 1) * 2, (i + 1) * 2, "prepare circles");
  clear_bitmap(temp_bitmap);
//  clear_to_color(temp_bitmap, TRANS_YELLOW_OUT);
  for (j = 0; j < 9; j ++)
  {
   circlefill(temp_bitmap, i + 1, i + 1, i, 28 + j * 26);
   circle(temp_bitmap, i + 1, i + 1, i, 15 + j * 26);
   RLE_circle [i] [j] = get_rle_sprite(temp_bitmap);
  }
  destroy_bitmap(temp_bitmap);
 }

 // now it's beam time!

 temp_bitmap = new_bitmap(60, 60, "prepare beams");

 for (i = 0; i < SMALL_ROTATIONS; i ++)
 {
  clear_bitmap(temp_bitmap);
  angle = i * inc;
  fast_bordered_poly4(temp_bitmap,
   30 + xpart(angle, 25) + xpart(angle + ANGLE_4, 4), 30 + ypart(angle, 25) + ypart(angle + ANGLE_4, 4),
   30 + xpart(angle, 25) + xpart(angle - ANGLE_4, 4), 30 + ypart(angle, 25) + ypart(angle - ANGLE_4, 4),
   30 - xpart(angle, 25) + xpart(angle - ANGLE_4, 4), 30 - ypart(angle, 25) + ypart(angle - ANGLE_4, 4),
   30 - xpart(angle, 25) + xpart(angle + ANGLE_4, 4), 30 - ypart(angle, 25) + ypart(angle + ANGLE_4, 4),
   TRANS_B5_IN,TRANS_B5_OUT,
   30, 30);

   new_rle_struct(temp_bitmap, "beam RLE", &RLE_beam [i], -1);

 }

 destroy_bitmap(temp_bitmap);

}



void prepare_blades(void)
{

 int i;

 BITMAP *temp_bitmap;

 temp_bitmap = new_bitmap(80, 80, "prepare_blades");

 int inc = ANGLE_1 / SMALL_ROTATIONS;
 int angle;

 for (i = 0; i < SMALL_ROTATIONS; i ++)
 {

 clear_bitmap(temp_bitmap);
 angle = i * inc;

 bordered_poly4(temp_bitmap,
  40 + xpart(angle, 23), 40 + ypart(angle, 23),
  40 - xpart(angle + ANGLE_4 + ANGLE_8, 8), 40 - ypart(angle + ANGLE_4 + ANGLE_8, 8),
  40, 40,
  40 - xpart(angle - ANGLE_4 - ANGLE_8, 8), 40 - ypart(angle - ANGLE_4 - ANGLE_8, 8),
/*  25 + xpart(angle, 12), 25 + ypart(angle, 12),
  25 + xpart(angle + ANGLE_4 + ANGLE_8, 8), 25 + ypart(angle + ANGLE_4 + ANGLE_8, 8),
  25 - xpart(angle, 15), 25 - ypart(angle, 15),
  25 + xpart(angle - ANGLE_4 - ANGLE_8, 8), 25 + ypart(angle - ANGLE_4 - ANGLE_8, 8),*/
  TRANS_YELLOW_IN, TRANS_YELLOW_OUT);

 new_rle_struct(temp_bitmap, "blade RLE", &RLE_blade [i], -1);

 } // end for i

  destroy_bitmap(temp_bitmap);


}


void prepare_petals(void)
{

 int i;

 BITMAP *temp_bitmap;

 temp_bitmap = new_bitmap(80, 80, "prepare_petals");

 int inc = ANGLE_1 / SMALL_ROTATIONS;
 int angle;

 for (i = 0; i < SMALL_ROTATIONS; i ++)
 {

 clear_bitmap(temp_bitmap);
 angle = i * inc;

 bordered_poly4(temp_bitmap,
  40 + xpart(angle, 23), 40 + ypart(angle, 23),
  40 - xpart(angle + ANGLE_4 + ANGLE_8, 8), 40 - ypart(angle + ANGLE_4 + ANGLE_8, 8),
  40, 40,
  40 - xpart(angle - ANGLE_4 - ANGLE_8, 8), 40 - ypart(angle - ANGLE_4 - ANGLE_8, 8),
  TRANS_SH1_IN, TRANS_SH1_OUT);

 bordered_poly4(temp_bitmap,
  40 + xpart(angle, 33), 40 + ypart(angle, 33),
  40 - xpart(angle + ANGLE_4 + ANGLE_8, 12), 40 - ypart(angle + ANGLE_4 + ANGLE_8, 12),
  40 + xpart(angle, 5), 40 + ypart(angle, 5),
  40 - xpart(angle - ANGLE_4 - ANGLE_8, 12), 40 - ypart(angle - ANGLE_4 - ANGLE_8, 12),
  TRANS_SH2_IN, TRANS_SH2_OUT);

 circlefill(temp_bitmap, 40 + xpart(angle, 12), 40 + ypart(angle, 12), 3, TRANS_SH1_IN);
 circle(temp_bitmap, 40 + xpart(angle, 12), 40 + ypart(angle, 12), 3, TRANS_SH2_OUT);

/* bordered_poly4(temp_bitmap,
  40 + xpart(angle, 33), 40 + ypart(angle, 33),
  40 - xpart(angle + ANGLE_4 + ANGLE_8, 12), 40 - ypart(angle + ANGLE_4 + ANGLE_8, 12),
  40 + xpart(angle, 15), 40 + ypart(angle, 15),
  40 - xpart(angle - ANGLE_4 - ANGLE_8, 12), 40 - ypart(angle - ANGLE_4 - ANGLE_8, 12),
  TRANS_SH2_IN, TRANS_SH2_OUT);*/

 new_rle_struct(temp_bitmap, "petal1 RLE", &petal1 [i], -1);

 } // end for i

 for (i = 0; i < SMALL_ROTATIONS; i ++)
 {

 clear_bitmap(temp_bitmap);
 angle = i * inc;

 bordered_poly4(temp_bitmap,
  40 + xpart(angle, 22), 40 + ypart(angle, 22),
  40 - xpart(angle + ANGLE_4 + ANGLE_8, 7), 40 - ypart(angle + ANGLE_4 + ANGLE_8, 6),
  40, 40,
  40 - xpart(angle - ANGLE_4 - ANGLE_8, 7), 40 - ypart(angle - ANGLE_4 - ANGLE_8, 6),
  TRANS_SH1_IN, TRANS_SH1_OUT);
/*
 bordered_poly4(temp_bitmap,
  40 + xpart(angle, 33), 40 + ypart(angle, 33),
  40 - xpart(angle + ANGLE_4 + ANGLE_8, 12), 40 - ypart(angle + ANGLE_4 + ANGLE_8, 12),
  40 + xpart(angle, 5), 40 + ypart(angle, 5),
  40 - xpart(angle - ANGLE_4 - ANGLE_8, 12), 40 - ypart(angle - ANGLE_4 - ANGLE_8, 12),
  TRANS_SH2_IN, TRANS_SH2_OUT);

 circlefill(temp_bitmap, 40 + xpart(angle, 12), 40 + ypart(angle, 12), 3, TRANS_SH1_IN);
 circle(temp_bitmap, 40 + xpart(angle, 12), 40 + ypart(angle, 12), 3, TRANS_SH2_OUT);*/


 new_rle_struct(temp_bitmap, "petal2 RLE", &petal2 [i], -1);

 } // end for i

  destroy_bitmap(temp_bitmap);

 temp_bitmap = new_bitmap(120, 120, "prepare petal3");


 for (i = 0; i < SMALL_ROTATIONS; i ++)
 {

 clear_bitmap(temp_bitmap);
 angle = i * inc;

 bordered_poly4(temp_bitmap,
  60 + xpart(angle, 45), 60 + ypart(angle, 45),
  60 - xpart(angle + ANGLE_4 + ANGLE_8, 12), 60 - ypart(angle + ANGLE_4 + ANGLE_8, 12),
  60, 60,
  60 - xpart(angle - ANGLE_4 - ANGLE_8, 12), 60 - ypart(angle - ANGLE_4 - ANGLE_8, 12),
  TRANS_SH1_IN, TRANS_SH1_OUT);

 bordered_poly6(temp_bitmap,
  60 + xpart(angle, 55), 60 + ypart(angle, 55),
  60 + xpart(angle - ANGLE_16, 45), 60 + ypart(angle - ANGLE_16, 45),
  60 - xpart(angle + ANGLE_4 + ANGLE_8, 18), 60 - ypart(angle + ANGLE_4 + ANGLE_8, 18),
  60 + xpart(angle, 9), 60 + ypart(angle, 9),
  60 - xpart(angle - ANGLE_4 - ANGLE_8, 18), 60 - ypart(angle - ANGLE_4 - ANGLE_8, 18),
  60 + xpart(angle + ANGLE_16, 45), 60 + ypart(angle + ANGLE_16, 45),
  TRANS_SH2_IN, TRANS_SH2_OUT);

/* bordered_poly4(temp_bitmap,
  60 + xpart(angle, 55), 60 + ypart(angle, 55),
  60 - xpart(angle + ANGLE_4 + ANGLE_8, 18), 60 - ypart(angle + ANGLE_4 + ANGLE_8, 18),
  60 + xpart(angle, 9), 60 + ypart(angle, 9),
  60 - xpart(angle - ANGLE_4 - ANGLE_8, 18), 60 - ypart(angle - ANGLE_4 - ANGLE_8, 18),
  TRANS_SH2_IN, TRANS_SH2_OUT);*/

 circlefill(temp_bitmap, 60 + xpart(angle, 25), 60 + ypart(angle, 25), 4, TRANS_SH1_IN);
 circle(temp_bitmap, 60 + xpart(angle, 25), 60 + ypart(angle, 25), 4, TRANS_SH2_OUT);

// circlefill(temp_bitmap, 60 + xpart(angle, 22), 60 + ypart(angle, 22), 3, TRANS_SH1_IN);
// circle(temp_bitmap, 60 + xpart(angle, 22), 60 + ypart(angle, 22), 3, TRANS_SH2_OUT);

 circlefill(temp_bitmap, 60 + xpart(angle, 38), 60 + ypart(angle, 38), 3, TRANS_SH1_IN);
 circle(temp_bitmap, 60 + xpart(angle, 38), 60 + ypart(angle, 38), 3, TRANS_SH2_OUT);

 new_rle_struct(temp_bitmap, "petal1 RLE", &petal3 [i], -1);

 } // end for i


  destroy_bitmap(temp_bitmap);


}



void prepare_multi_rles(void)
{

 BITMAP *file_bitmap = load_up_bitmap(DIRECTORY(DATADIR,gfx/multi.bmp));

 fix_outline(file_bitmap);

 int i;

 RLE_multi [WPN_CANNON] [0].sprite = extract_rle_sprite(file_bitmap, 3, 1, 11, 11);
 RLE_multi [WPN_CANNON] [0].x = 5;
 RLE_multi [WPN_CANNON] [0].y = 5;
 RLE_multi [WPN_CANNON] [1].sprite = extract_rle_sprite(file_bitmap, 2, 13, 13, 11);
 RLE_multi [WPN_CANNON] [1].x = 6;
 RLE_multi [WPN_CANNON] [1].y = 5;
 RLE_multi [WPN_CANNON] [2].sprite = extract_rle_sprite(file_bitmap, 1, 25, 15, 11);
 RLE_multi [WPN_CANNON] [2].x = 7;
 RLE_multi [WPN_CANNON] [2].y = 5;
 RLE_multi [WPN_CANNON] [3].sprite = extract_rle_sprite(file_bitmap, 1, 37, 15, 11);
 RLE_multi [WPN_CANNON] [3].x = 7;
 RLE_multi [WPN_CANNON] [3].y = 5;
 RLE_multi [WPN_BLADE] [0].sprite = extract_rle_sprite(file_bitmap, 17, 1, 9, 9);
 RLE_multi [WPN_BLADE] [0].x = 4;
 RLE_multi [WPN_BLADE] [0].y = 4;
 RLE_multi [WPN_BLADE] [1].sprite = extract_rle_sprite(file_bitmap, 19, 11, 7, 7);
 RLE_multi [WPN_BLADE] [1].x = 3;
 RLE_multi [WPN_BLADE] [1].y = 3;
 for (i = 0; i < 5; i ++)
 {
  RLE_multi [WPN_MGUN] [i].sprite = extract_rle_sprite(file_bitmap, 27, 1 + i * 12, 19, 11);
  RLE_multi [WPN_MGUN] [i].x = 9;
  RLE_multi [WPN_MGUN] [i].y = 6;
 }
 for (i = 0; i < 4; i ++)
 {
  RLE_multi [WPN_SCATTER] [i].sprite = extract_rle_sprite(file_bitmap, 47, 1 + i * 13, 13, 11);
  RLE_multi [WPN_SCATTER] [i].x = 6;
  RLE_multi [WPN_SCATTER] [i].y = 5;
 }
 RLE_multi [WPN_LWBEAM] [0].sprite = extract_rle_sprite(file_bitmap, 101, 1, 11, 11);
 RLE_multi [WPN_LWBEAM] [0].x = 5;
 RLE_multi [WPN_LWBEAM] [0].y = 5;
 RLE_multi [WPN_LWBEAM] [1].sprite = extract_rle_sprite(file_bitmap, 100, 13, 13, 11);
 RLE_multi [WPN_LWBEAM] [1].x = 6;
 RLE_multi [WPN_LWBEAM] [1].y = 5;
 RLE_multi [WPN_LWBEAM] [2].sprite = extract_rle_sprite(file_bitmap, 99, 25, 15, 11);
 RLE_multi [WPN_LWBEAM] [2].x = 7;
 RLE_multi [WPN_LWBEAM] [2].y = 5;
 RLE_multi [WPN_SWBEAM] [0].sprite = extract_rle_sprite(file_bitmap, 73, 1, 11, 11);
 RLE_multi [WPN_SWBEAM] [0].x = 5;
 RLE_multi [WPN_SWBEAM] [0].y = 5;
 RLE_multi [WPN_SWBEAM] [1].sprite = extract_rle_sprite(file_bitmap, 73, 13, 11, 11);
 RLE_multi [WPN_SWBEAM] [1].x = 5;
 RLE_multi [WPN_SWBEAM] [1].y = 4;
 RLE_multi [WPN_SWBEAM] [2].sprite = extract_rle_sprite(file_bitmap, 73, 25, 11, 11);
 RLE_multi [WPN_SWBEAM] [2].x = 5;
 RLE_multi [WPN_SWBEAM] [2].y = 3;
 RLE_multi [WPN_SEEKER] [0].sprite = extract_rle_sprite(file_bitmap, 85, 1, 13, 12);
 RLE_multi [WPN_SEEKER] [0].x = 6;
 RLE_multi [WPN_SEEKER] [0].y = 7;
 RLE_multi [WPN_SEEKER] [1].sprite = extract_rle_sprite(file_bitmap, 85, 14, 13, 12);
 RLE_multi [WPN_SEEKER] [1].x = 6;
 RLE_multi [WPN_SEEKER] [1].y = 7;
 RLE_multi [WPN_SEEKER] [2].sprite = extract_rle_sprite(file_bitmap, 85, 27, 13, 12);
 RLE_multi [WPN_SEEKER] [2].x = 6;
 RLE_multi [WPN_SEEKER] [2].y = 7;
 RLE_multi [WPN_SEEKER] [3].sprite = extract_rle_sprite(file_bitmap, 85, 40, 13, 12);
 RLE_multi [WPN_SEEKER] [3].x = 6;
 RLE_multi [WPN_SEEKER] [3].y = 7;
 RLE_multi [WPN_SEEKER] [4].sprite = extract_rle_sprite(file_bitmap, 85, 53, 13, 12);
 RLE_multi [WPN_SEEKER] [4].x = 6;
 RLE_multi [WPN_SEEKER] [4].y = 7;

 RLE_multi [WPN_RECT] [0].sprite = extract_rle_sprite(file_bitmap, 115, 1, 15, 11);
 RLE_multi [WPN_RECT] [0].x = 7;
 RLE_multi [WPN_RECT] [0].y = 5;
 RLE_multi [WPN_RECT] [1].sprite = extract_rle_sprite(file_bitmap, 116, 13, 13, 11);
 RLE_multi [WPN_RECT] [1].x = 6;
 RLE_multi [WPN_RECT] [1].y = 5;
 RLE_multi [WPN_RECT] [2].sprite = extract_rle_sprite(file_bitmap, 117, 25, 11, 11);
 RLE_multi [WPN_RECT] [2].x = 5;
 RLE_multi [WPN_RECT] [2].y = 5;
 RLE_multi [WPN_RECT] [3].sprite = extract_rle_sprite(file_bitmap, 117, 37, 11, 11);
 RLE_multi [WPN_RECT] [3].x = 5;
 RLE_multi [WPN_RECT] [3].y = 5;

 RLE_multi [WPN_EIGHT] [0].sprite = extract_rle_sprite(file_bitmap, 131, 1, 13, 13);
 RLE_multi [WPN_EIGHT] [0].x = 6;
 RLE_multi [WPN_EIGHT] [0].y = 6;

  destroy_bitmap(file_bitmap);


}




void prepare_platform_rles(void)
{

 BITMAP *file_bitmap = load_up_bitmap(DIRECTORY(DATADIR,gfx/platform.bmp));

// int i, j;

// exit(getpixel(file_bitmap, 0, 0));

 fix_outline(file_bitmap);

// int i;

// for (i = 0; i < 2; i ++)
// {/


      platform_RLE [PLATFORM_LR] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_LR * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_UD] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UD * PLATFORM_SIZE, 1, 91, 91);

      platform_RLE [PLATFORM_UL] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_DL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, 1);
      platform_RLE [PLATFORM_UR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, 0);
      platform_RLE [PLATFORM_DR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, 2);

      platform_RLE [PLATFORM_UDL] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UDL * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_UDR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UDL * PLATFORM_SIZE, 1, 91, 91, 0);

      platform_RLE [PLATFORM_ULR] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_ULR * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_DLR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_ULR * PLATFORM_SIZE, 1, 91, 91, 1);

      platform_RLE [PLATFORM_L] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_L * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_R] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_L * PLATFORM_SIZE, 1, 91, 91, 0);
      platform_RLE [PLATFORM_U] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_U * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_D] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_U * PLATFORM_SIZE, 1, 91, 91, 1);


      platform_RLE [PLATFORM_UR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, 0);
      platform_RLE [PLATFORM_DR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, 2);

      platform_RLE [PLATFORM_NODE] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_NODE * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_NODE2] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_NODE2 * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_NODE3] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_NODE3 * PLATFORM_SIZE, 1, 91, 91);


      island1_RLE [ISLAND1_D] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_D * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_U] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_D * PLATFORM_SIZE, 93, 91, 91, FLIP_V);

      island1_RLE [ISLAND1_R] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_R * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_L] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_R * PLATFORM_SIZE, 93, 91, 91, FLIP_H);

      island1_RLE [ISLAND1_DR] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_DL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 93, 91, 91, FLIP_H);
      island1_RLE [ISLAND1_UR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 93, 91, 91, FLIP_V);
      island1_RLE [ISLAND1_UL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 93, 91, 91, FLIP_VH);

      island1_RLE [ISLAND1_NOT_UL] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_NOT_UR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 93, 91, 91, FLIP_H);
      island1_RLE [ISLAND1_NOT_DL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 93, 91, 91, FLIP_V);
      island1_RLE [ISLAND1_NOT_DR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 93, 91, 91, FLIP_VH);

      island1_RLE [ISLAND1_UL_DR] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_UR_DL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 93, 91, 91, FLIP_H);

      island1_RLE [ISLAND1_FULL] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_FULL2] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL2 * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_FULL3] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL3 * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_FULL4] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL4 * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_FULL5] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL5 * PLATFORM_SIZE, 93, 91, 91);


      island1_RLE [ISLAND1_D2] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_D * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_U2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_D * PLATFORM_SIZE, 185, 91, 91, FLIP_V);

      island1_RLE [ISLAND1_R2] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_R * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_L2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_R * PLATFORM_SIZE, 185, 91, 91, FLIP_H);

      island1_RLE [ISLAND1_DR2] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_DL2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_H);
      island1_RLE [ISLAND1_UR2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_V);
      island1_RLE [ISLAND1_UL2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_VH);

      island1_RLE [ISLAND1_NOT_UL2] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_NOT_UR2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_H);
      island1_RLE [ISLAND1_NOT_DL2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_V);
      island1_RLE [ISLAND1_NOT_DR2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_VH);

      island1_RLE [ISLAND1_UL_DR2] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_UR_DL2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_H);



      island1_RLE [ISLAND1_D3] = extract_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_D * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_U3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_D * PLATFORM_SIZE, 185, 91, 91, FLIP_V);

      island1_RLE [ISLAND1_R3] = extract_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_R * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_L3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_R * PLATFORM_SIZE, 185, 91, 91, FLIP_H);

      island1_RLE [ISLAND1_DR3] = extract_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_DL3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_H);
      island1_RLE [ISLAND1_UR3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_V);
      island1_RLE [ISLAND1_UL3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_VH);

      island1_RLE [ISLAND1_NOT_UL3] = extract_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_NOT_UR3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_H);
      island1_RLE [ISLAND1_NOT_DL3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_V);
      island1_RLE [ISLAND1_NOT_DR3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_VH);

      island1_RLE [ISLAND1_UL_DR3] = extract_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_UR_DL3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_H);

      bone_RLE [BONE_HOOK1] = extract_rle_sprite(file_bitmap, 1, 278, 23, 22);
      bone_RLE [BONE_HOOK2] = extract_rle_sprite(file_bitmap, 25, 278, 23, 22);
      bone_RLE [BONE_HOOK3] = extract_rle_sprite(file_bitmap, 49, 278, 23, 22);
      bone_RLE [BONE_HOOK4] = extract_rle_sprite(file_bitmap, 76, 278, 20, 22);
      bone_RLE [BONE_HOOK5] = extract_rle_sprite(file_bitmap, 97, 278, 23, 18);
      bone_RLE [BONE_HOOK2F] = extract_flip_rle_sprite(file_bitmap, 25, 278, 23, 22, FLIP_H);
      bone_RLE [BONE_HOOK3F] = extract_flip_rle_sprite(file_bitmap, 49, 278, 23, 22, FLIP_H);
      bone_RLE [BONE_HOOK4F] = extract_flip_rle_sprite(file_bitmap, 76, 278, 20, 22, FLIP_H);
      bone_RLE [BONE_HOOK5F] = extract_flip_rle_sprite(file_bitmap, 97, 278, 23, 18, FLIP_H);


      bone_RLE [BONE_B_HOOK1] = extract_rle_sprite(file_bitmap, 1, 301, 11, 11);
      bone_RLE [BONE_B_HOOK2] = extract_rle_sprite(file_bitmap, 13, 301, 11, 11);
      bone_RLE [BONE_B_HOOK3] = extract_rle_sprite(file_bitmap, 25, 301, 11, 11);
      bone_RLE [BONE_B_HOOK4] = extract_rle_sprite(file_bitmap, 38, 301, 10, 11);
      bone_RLE [BONE_B_HOOK5] = extract_rle_sprite(file_bitmap, 49, 301, 11, 9);
      bone_RLE [BONE_B_HOOK2F] = extract_flip_rle_sprite(file_bitmap, 13, 301, 11, 11, FLIP_H);
      bone_RLE [BONE_B_HOOK3F] = extract_flip_rle_sprite(file_bitmap, 25, 301, 11, 11, FLIP_H);
      bone_RLE [BONE_B_HOOK4F] = extract_flip_rle_sprite(file_bitmap, 38, 301, 10, 11, FLIP_H);
      bone_RLE [BONE_B_HOOK5F] = extract_flip_rle_sprite(file_bitmap, 49, 301, 11, 9, FLIP_H);

      bone_RLE [BONE_DROP1] = extract_rle_sprite(file_bitmap, 122, 278, 23, 35);
      bone_RLE [BONE_DROP2] = extract_rle_sprite(file_bitmap, 146, 278, 23, 35);
      bone_RLE [BONE_DROP3] = extract_rle_sprite(file_bitmap, 170, 278, 23, 35);
      bone_RLE [BONE_DROP4] = extract_rle_sprite(file_bitmap, 194, 278, 23, 35);
      bone_RLE [BONE_DROP2F] = extract_flip_rle_sprite(file_bitmap, 146, 278, 23, 35, FLIP_H);
      bone_RLE [BONE_DROP3F] = extract_flip_rle_sprite(file_bitmap, 170, 278, 23, 35, FLIP_H);
      bone_RLE [BONE_DROP4F] = extract_flip_rle_sprite(file_bitmap, 194, 278, 23, 35, FLIP_H);

      bone_RLE [BONE_B_DROP1] = extract_rle_sprite(file_bitmap, 61, 301, 11, 16);
      bone_RLE [BONE_B_DROP2] = extract_rle_sprite(file_bitmap, 73, 301, 11, 16);
      bone_RLE [BONE_B_DROP3] = extract_rle_sprite(file_bitmap, 85, 301, 10, 16);
      bone_RLE [BONE_B_DROP4] = extract_rle_sprite(file_bitmap, 96, 304, 10, 13);
      bone_RLE [BONE_B_DROP2F] = extract_flip_rle_sprite(file_bitmap, 73, 301, 11, 16, FLIP_H);
      bone_RLE [BONE_B_DROP3F] = extract_flip_rle_sprite(file_bitmap, 85, 301, 10, 16, FLIP_H);
      bone_RLE [BONE_B_DROP4F] = extract_flip_rle_sprite(file_bitmap, 96, 304, 10, 13, FLIP_H);

      bone_RLE [BONE_RING1] = extract_rle_sprite(file_bitmap, 1, 322, 39, 39);
      bone_RLE [BONE_RING2] = extract_rle_sprite(file_bitmap, 41, 322, 39, 39);
      bone_RLE [BONE_RING3] = extract_rle_sprite(file_bitmap, 81, 322, 39, 39);
      bone_RLE [BONE_RING4] = extract_rle_sprite(file_bitmap, 121, 322, 39, 39);
      bone_RLE [BONE_RING5] = extract_rle_sprite(file_bitmap, 161, 322, 39, 39);
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

      bone_RLE [BONE_B_RING1] = extract_rle_sprite(file_bitmap, 1, 362, 23, 23);
      bone_RLE [BONE_B_RING2] = extract_rle_sprite(file_bitmap, 25, 362, 23, 23);
      bone_RLE [BONE_B_RING3] = extract_rle_sprite(file_bitmap, 49, 362, 23, 23);
      bone_RLE [BONE_B_RING4] = extract_rle_sprite(file_bitmap, 73, 362, 23, 23);
      bone_RLE [BONE_B_RING5] = extract_rle_sprite(file_bitmap, 97, 362, 23, 23);
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

      bone_RLE [BONE_HUNTER1] = extract_rle_sprite(file_bitmap, 201, 322, 45, 40);
      bone_RLE [BONE_HUNTER2] = extract_rle_sprite(file_bitmap, 247, 322, 45, 40);
      bone_RLE [BONE_HUNTER3] = extract_rle_sprite(file_bitmap, 293, 322, 45, 40);
      bone_RLE [BONE_HUNTER4] = extract_rle_sprite(file_bitmap, 339, 322, 41, 40);
      bone_RLE [BONE_HUNTER5] = extract_rle_sprite(file_bitmap, 381, 322, 45, 40);
      bone_RLE [BONE_HUNTER2F] = extract_flip_rle_sprite(file_bitmap, 247, 322, 45, 40, FLIP_H);
      bone_RLE [BONE_HUNTER3F] = extract_flip_rle_sprite(file_bitmap, 293, 322, 45, 40, FLIP_H);
      bone_RLE [BONE_HUNTER4F] = extract_flip_rle_sprite(file_bitmap, 339, 322, 41, 40, FLIP_H);
      bone_RLE [BONE_HUNTER5F] = extract_flip_rle_sprite(file_bitmap, 381, 322, 45, 40, FLIP_H);

      bone_RLE [BONE_B_HUNTER1] = extract_rle_sprite(file_bitmap, 199, 364, 27, 23);
      bone_RLE [BONE_B_HUNTER2] = extract_rle_sprite(file_bitmap, 227, 364, 27, 23);
      bone_RLE [BONE_B_HUNTER3] = extract_rle_sprite(file_bitmap, 255, 364, 27, 23);
      bone_RLE [BONE_B_HUNTER4] = extract_rle_sprite(file_bitmap, 285, 364, 25, 23);
      bone_RLE [BONE_B_HUNTER5] = extract_rle_sprite(file_bitmap, 311, 364, 27, 23);
      bone_RLE [BONE_B_HUNTER2F] = extract_flip_rle_sprite(file_bitmap, 227, 364, 27, 23, FLIP_H);
      bone_RLE [BONE_B_HUNTER3F] = extract_flip_rle_sprite(file_bitmap, 255, 364, 27, 23, FLIP_H);
      bone_RLE [BONE_B_HUNTER4F] = extract_flip_rle_sprite(file_bitmap, 285, 364, 25, 23, FLIP_H);
      bone_RLE [BONE_B_HUNTER5F] = extract_flip_rle_sprite(file_bitmap, 311, 364, 27, 23, FLIP_H);

      bone_RLE [BONE_B_SPINNER] = extract_rle_sprite(file_bitmap, 427, 288, 65, 107);

      bone_RLE [BONE_B_WALKER1] = extract_rle_sprite(file_bitmap, 493, 285, 89, 75);
      bone_RLE [BONE_B_WALKER2] = extract_rle_sprite(file_bitmap, 583, 285, 89, 75);
      bone_RLE [BONE_B_WALKER2F] = extract_flip_rle_sprite(file_bitmap, 583, 285, 89, 75, FLIP_H);

 destroy_bitmap(file_bitmap);

}




void prepare_flowers(void)
{
 BITMAP *tmp = new_bitmap(200, 200, "prepare_flowers");

 clear_bitmap(tmp);

 int i, j, angle = 0;



 for (j = 0; j < 40; j ++)
 {

 angle = j * (ANGLE_10 / 20);


 for (i = 0; i < 5; i ++)
 {

  fast_bordered_poly4(tmp,
   100, 100,
   100 + xpart(angle + (ANGLE_5 * i) - 46, 36), 100 + ypart(angle + (ANGLE_5 * i) - 46, 36),
   100 + xpart(angle + (ANGLE_5 * i), 55), 100 + ypart(angle + (ANGLE_5 * i), 55),
   100 + xpart(angle + (ANGLE_5 * i) + 46, 36), 100 + ypart(angle + (ANGLE_5 * i) + 46, 36),
   COL_BACK5,COL_OUTLINE,
   100 + xpart(angle + (ANGLE_5 * i), 30), 100 + ypart(angle + (ANGLE_5 * i), 30));

 }


 circlefill(tmp, 100, 100, 8, COL_COL3);
 circle(tmp, 100, 100, 8, COL_OUTLINE);

// extract_rle_struct(tmp, 0, 0, 100, 100, "flower RLE", &eRLE_flower [0] [j], -1);
 new_rle_struct(tmp, "flower RLE", &eRLE_flower [0] [j], -2);

 clear_bitmap(tmp);



 }

destroy_bitmap(tmp);

}



/*
void prepare_mshockwaves(void)
{
 int i, j;

 BITMAP *tmp = new_bitmap(90, 90, "prepare_mshockwaves tmp bitmap");

 int rad1, rad2, c1, c2;
 i = 0;
 for (i = 0; i < 4; i ++)
 {

  switch(i)
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
}
  rad1 = 30;
  rad2 = 1;
  for (j = 0; j < 20; j ++)
  {
    clear_bitmap(tmp);

    rad2 += 2;
    if (j % 2 == 0)
     rad1 ++;

    circlefill(tmp, 45, 45, rad1, c2);
    circle(tmp, 45, 45, rad1, c1);
    circlefill(tmp, 45, 45, rad2, 0);
    circle(tmp, 45, 45, rad2, c1);

    new_rle_struct(tmp, "mshockwaves rle", &RLE_mshock [i] [j], -1);
    extract_rle_struct2(tmp, &RLE_mshock [i] [j], 1, 1, 1, 1, -1);

  }
 }


 destroy_bitmap(tmp);
}
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
    }

//    new_rle_struct(tmp, "mshockwaves rle", &RLE_mshock [i] [j], -1);
    extract_rle_struct2(tmp, &RLE_ring [j], 0, 0, rad1 * 2 + 1, rad1 * 2 + 1, -1);

 }


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
    }
     else
     {
      circlefill(tmp, rad1, rad1, rad1, c2);
      circle(tmp, rad1, rad1, rad1, c1);
      if (rad2 > 1)
      {
       circlefill(tmp, rad1, rad1, rad2, 0);
       circle(tmp, rad1, rad1, rad2, c1);
      }
     }

//    new_rle_struct(tmp, "mshockwaves rle", &RLE_mshock [i] [j], -1);
    extract_rle_struct2(tmp, &RLE_lwbeamshock [j], 0, 0, rad1 * 2 + 1, rad1 * 2 + 1, -1);

 }


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

  switch(i)
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
}
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
    }
     else
     {
      circlefill(tmp, rad1, rad1, rad1, c2);
      circle(tmp, rad1, rad1, rad1, c1);
      if (rad2 > 1)
      {
       circlefill(tmp, rad1, rad1, rad2, 0);
       circle(tmp, rad1, rad1, rad2, c1);
      }
     }

//    new_rle_struct(tmp, "mshockwaves rle", &RLE_mshock [i] [j], -1);
    extract_rle_struct2(tmp, &RLE_mshock [i] [j], 0, 0, rad1 * 2 + 1, rad1 * 2 + 1, -1);

  }
 }


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
  switch(i)
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
  }
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

  }
 }

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
  switch(i)
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
  }
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

  }
 }

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
    }
     else
     {
      circlefill(tmp, rad1, rad1, rad1, c2);
      circle(tmp, rad1, rad1, rad1, c1);
      if (rad2 > 1)
      {
       circlefill(tmp, rad1, rad1, rad2, 0);
       circle(tmp, rad1, rad1, rad2, c1);
      }
     }

//    new_rle_struct(tmp, "mshockwaves rle", &RLE_mshock [i] [j], -1);
    extract_rle_struct2(tmp, &RLE_rshock [i] [j], 0, 0, rad1 * 2 + 1, rad1 * 2 + 1, -1);

  }
 }


 destroy_bitmap(tmp);
}




void prepare_various_effects(void)
{
/*
 BITMAP *temp_bitmap;

 int i;
 int rad1, rad2;

 BITMAP *sbitmap = new_bitmap(210, 30, "sbitmap");
 clear_bitmap(sbitmap);

 for (i = 0; i < 9; i ++)
 {
  temp_bitmap = new_bitmap((i + 3) * 2, (i + 3) * 2, "prepare mg shock");
  clear_bitmap(temp_bitmap);
//  clear_to_color(temp_bitmap, TRANS_YELLOW_OUT);
  rad1 = i + 2;
  rad2 = 0;
  circlefill(temp_bitmap, i + 1, i + 1, i, TRANS_ORANGE_IN);
  circle(temp_bitmap, i + 1, i + 1, i, TRANS_ORANGE_OUT);
  if (i > 3)
  {
   rad2 = (i - 3) * 2;
   circlefill(temp_bitmap, i + 1, i + 1, rad2, 0);
   circle(temp_bitmap, i + 1, i + 1, rad2, TRANS_ORANGE_OUT);
  }
  if (i == 8)
  {
   clear_bitmap(temp_bitmap);
   circle(temp_bitmap, i + 1, i + 1, 9, TRANS_ORANGE_OUT);
  }
  RLE_mg_shock [i] = get_rle_sprite(temp_bitmap);
  draw_sprite(sbitmap, temp_bitmap, i * 20, 1);
  destroy_bitmap(temp_bitmap);
 }

  save_bitmap("shock.bmp", sbitmap, palet [0]);
*/
}







/*
void prepare_underlay(void)
{
 return;

 underlay = new_bitmap(640, 1000, "underlay");

 clear_to_color(underlay, COL_BACK1);

 int i, j, out_of_loop;
 int x, y, rad, x1, y1, x2, y2, col;

 int cx [20];
 int cy [20];
 int crad [20];

 for (i = 0; i < 20; i ++)
 {
  cx [i] = -1;
 }

 for (i = 0; i < 20; i ++)
 {
     out_of_loop = 0;
     do
     {
         cx [i] = grand(640);
         cy [i] = grand(1000);
         crad [i] = 30 + grand(20) + grand(10);
         if (i > 0)
         {
          for (j = 0; j < i; j ++)
          {
              if (cx [i] - crad [i] > cx [j] + crad [j]
                  || cx [i] + crad [i] < cx [j] - crad [j]
                  || cy [i] - crad [i] > cy [j] + crad [j]
                  || cy [i] + crad [i] < cy [j] - crad [j])
                   out_of_loop = 1;
          }
         }
     } while (out_of_loop == 0);
 }

 for (i = 0; i < 20; i ++)
 {
   x = cx [i];
   y = cy [i];
   rad = crad [i];
   col = COL_BACK2;
   do
   {
    circlefill(underlay, x, y, rad, col);
    circle(underlay, x, y, rad, COL_OUTLINE);
    if (col == COL_BACK2)
    {
     col = COL_BACK1;
     rad -= 8 + grand(8);
    }
      else
      {
       col = COL_BACK2;
       rad -= 4 + grand(4);
      }
   } while(rad > 5);

  }

*/
/*
 int i, j;
 int x, y, rad, x1, y1, x2, y2;

 int bx [7] [10];
 int by [7] [10];
 int brad [7] [10];

 for (i = 0; i < 7; i ++)
 {
  for (j = 0; j < 10; j ++)
  {
      bx [i] [j] = i * 100 + grand(20) - grand(20);
      by [i] [j] = j * 100 + grand(20) - grand(20);
      brad [i] [j] = 20 + grand(10) + grand(10);
  }
 }

 int x3;

 for (i = 0; i < 7; i ++)
 {
  for (j = 0; j < 10; j ++)
  {
     x = bx [i] [j];
     y = by [i] [j];
     rad = brad [i] [j];
     circlefill(underlay, x, y, rad, COL_BACK2);
     drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
     circle(underlay, x, y, rad + 1, TRANS_BACK_OUTLINE);
     drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
     x1 = x - grand(15) + grand(5);
     x2 = x + grand(15) - grand(5);
     if (x1 > x2)
     {
      x3 = x1;
      x1 = x2;
      x2 = x3;
     }
     if (x1 == x2)
      x2 += 3;

     rectfill(underlay, x1, y, x2, y + 100, COL_BACK2);
     drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
     rect(underlay, x1 - 1, y, x2 + 1, y + 100, TRANS_BACK_OUTLINE);
     drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

     rectfill(underlay, x, y - grand(15) - 3, x + 100, y + grand(15) + 3, COL_BACK2);


     if (i == 0)
     {
      rectfill(underlay, 0, y - grand(10) - 5, x, y + grand(10) + 5, COL_BACK2);
     }
  }
 }

// circlefill(underlay, 320, 500, 10, COL_BACK2);
// circle(underlay, 320, 500, 10, COL_OUTLINE);
*/
//}



/*
Safe wrapper for the Allegro create_bitmap function.
*/
BITMAP *new_bitmap(int x, int y, const char errtxt [])
{

 BITMAP *bmp = create_bitmap(x, y);
 if (bmp == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Bitmap creation failed!  
%s

%s", allegro_error, errtxt);
  exit(1);
 }

 return bmp;

}




void bordered_poly4(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int col, int border_col)
{

  BITMAP *temp_bitmap = new_bitmap(target->w, target->h, "Bordered poly4 temp_bitmap");

  clear_bitmap(temp_bitmap);

  int points [8];
  points [0] = x1;
  points [1] = y1;
  points [2] = x2;
  points [3] = y2;
  points [4] = x3;
  points [5] = y3;
  points [6] = x4;
  points [7] = y4;

  polygon(temp_bitmap, 4, points, col);

  coat_sprite(temp_bitmap, border_col);

  draw_sprite(target, temp_bitmap, 0, 0);

  destroy_bitmap(temp_bitmap);

}

void bordered_poly6(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int x6, int y6, int col, int border_col)
{

  BITMAP *temp_bitmap = new_bitmap(target->w, target->h, "Bordered poly4 temp_bitmap");

  clear_bitmap(temp_bitmap);

  int points [12];
  points [0] = x1;
  points [1] = y1;
  points [2] = x2;
  points [3] = y2;
  points [4] = x3;
  points [5] = y3;
  points [6] = x4;
  points [7] = y4;
  points [8] = x5;
  points [9] = y5;
  points [10] = x6;
  points [11] = y6;

  polygon(temp_bitmap, 6, points, col);

  coat_sprite(temp_bitmap, border_col);

  draw_sprite(target, temp_bitmap, 0, 0);

  destroy_bitmap(temp_bitmap);

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




void coat_sprite(BITMAP *target, int col)
{
 int i, j;

 for (i = 1; i < target->w - 1; i ++)
 {
  for (j = 1; j < target->h - 1; j ++)
  {
   if (_getpixel(target, i, j) == 0)
   {
    if ((_getpixel(target, i - 1, j) != 0 && _getpixel(target, i - 1, j) != col)
       || (_getpixel(target, i + 1, j) != 0 && _getpixel(target, i + 1, j) != col)
       || (_getpixel(target, i, j - 1) != 0 && _getpixel(target, i, j - 1) != col)
       || (_getpixel(target, i, j + 1) != 0 && _getpixel(target, i, j + 1) != col))
         putpixel(target, i, j, col);
   }
  }
 }

}

void coat_file_sprite_xy(BITMAP *target, int col, int x1, int y1, int x2, int y2)
{
 int i, j;

 for (i = x1; i < x2; i ++)
 {
  for (j = y1; j < y2; j ++)
  {
   if (getpixel(target, i, j) == 0)
   {
    if ((getpixel(target, i - 1, j) != 0 && getpixel(target, i - 1, j) != col)
       || (getpixel(target, i + 1, j) != 0 && getpixel(target, i + 1, j) != col)
       || (getpixel(target, i, j - 1) != 0 && getpixel(target, i, j - 1) != col)
       || (getpixel(target, i, j + 1) != 0 && getpixel(target, i, j + 1) != col))
         putpixel(target, i, j, col);
   }
  }
 }

}



void fast_bordered_poly4(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int col, int border_col, int fillx, int filly)
{

  BITMAP *temp_bitmap = new_bitmap(target->w, target->h, "Bordered poly4 temp_bitmap");

  clear_bitmap(temp_bitmap);

  line(temp_bitmap, x1, y1, x2, y2, border_col);
  line(temp_bitmap, x2, y2, x3, y3, border_col);
  line(temp_bitmap, x3, y3, x4, y4, border_col);
  line(temp_bitmap, x4, y4, x1, y1, border_col);

  floodfill(temp_bitmap, fillx, filly, col);

  draw_sprite(target, temp_bitmap, 0, 0);

  destroy_bitmap(temp_bitmap);

}

void fast_bordered_triangle(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int col, int border_col, int fillx, int filly)
{

  BITMAP *temp_bitmap = new_bitmap(target->w, target->h, "Bordered triangle temp_bitmap");

  clear_bitmap(temp_bitmap);

  line(temp_bitmap, x1, y1, x2, y2, border_col);
  line(temp_bitmap, x2, y2, x3, y3, border_col);
  line(temp_bitmap, x3, y3, x1, y1, border_col);

  floodfill(temp_bitmap, fillx, filly, col);

  draw_sprite(target, temp_bitmap, 0, 0);

  destroy_bitmap(temp_bitmap);

}



BITMAP *load_up_bitmap(const char fname [])
{
 RGB temp_palette [256];

 BITMAP *temp_bitmap = load_bitmap(fname, temp_palette);
 if (temp_bitmap == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Failed to load in bitmap! (File not found?)  
%s", fname);
  exit(1);
 }

 return temp_bitmap;

}



RLE_SPRITE *extract_rle_sprite(BITMAP *source, int x_source, int y_source, int x, int y)
{

 BITMAP *tmp = new_bitmap(x, y, "extract_rle_sprite");

 blit(source, tmp, x_source, y_source, 0, 0, x, y);

 RLE_SPRITE *retval = get_rle_sprite(tmp);

 if (retval == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Rle_sprite extraction failed!  
%s

%s", allegro_error, "extract_rle_sprite");
  exit(1);
 }

 destroy_bitmap(tmp);

 return retval;

}

RLE_SPRITE *extract_flip_rle_sprite(BITMAP *source, int x_source, int y_source, int x, int y, int flip_type)
{

 BITMAP *tmp = new_bitmap(x, y, "extract_hflip_rle_sprite");
 BITMAP *tmp2 = new_bitmap(x, y, "extract_hflip_rle_sprite");

 clear_bitmap(tmp2);
 blit(source, tmp, x_source, y_source, 0, 0, x, y);

 switch(flip_type)
 {
  case FLIP_H: draw_sprite_h_flip(tmp2, tmp, 0, 0); break;
  case FLIP_V: draw_sprite_v_flip(tmp2, tmp, 0, 0); break;
  case FLIP_VH: draw_sprite_vh_flip(tmp2, tmp, 0, 0); break;
 }


 RLE_SPRITE *retval = get_rle_sprite(tmp2);

 if (retval == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Rle_sprite extraction failed!  
%s

%s", allegro_error, "extract_rle_sprite");
  exit(1);
 }

 destroy_bitmap(tmp);
 destroy_bitmap(tmp2);

 return retval;

}



RLE_SPRITE *sized_rle_sprite(BITMAP *source, int x, int y, const char errtxt [])
{
 BITMAP *tmp = new_bitmap(x, y, errtxt);

 blit(source, tmp, 0, 0, 0, 0, x, y);

 RLE_SPRITE *retval = get_rle_sprite(tmp);

 if (retval == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Rle_sprite sized creation failed!  
%s

%s", allegro_error, errtxt);
  exit(1);
 }

 destroy_bitmap(tmp);

 return retval;

}



RLE_SPRITE *new_rle_sprite(BITMAP *source, const char errtxt [])
{

 RLE_SPRITE *retval = get_rle_sprite(source);
 if (retval == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Rle_sprite creation failed!  
%s

%s", allegro_error, errtxt);
  exit(1);
 }

 return retval;

}

void new_rle_struct_colours(BITMAP *source, const char errtxt [], struct RLE_STRUCT str [SMALL_ROTATIONS] [9], int bl, int index2, int col1, int col2)
{

 int i;
 int count = 0;
 int trans_col = 0;

 for (i = col1; i < col2 + 1; i ++)
 {
      switch(i)
      {
       case 0: trans_col = TRANS_CONVERT_1; break;
       case 1: trans_col = TRANS_CONVERT_2; break;
       case 2: trans_col = TRANS_CONVERT_3; break;
       case 3: trans_col = TRANS_CONVERT_4; break;
       case 4: trans_col = TRANS_CONVERT_5; break;
       case 5: trans_col = TRANS_CONVERT_6; break;
       case 6: trans_col = TRANS_CONVERT_7; break;
       case 7: trans_col = TRANS_CONVERT_8; break;
       case 8: trans_col = TRANS_CONVERT_9; break;
      }
      drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
      rectfill(source, 0, 0, source->w, source->h, trans_col);
      drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
      new_rle_struct(source, "new_rle_struct_colours", str [index2], i - col1);
      count ++;
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
  allegro_message("Rle_struct extraction failed!  
%s

%s", allegro_error, "extract_rle_struct");
  exit(1);
 }

  destroy_bitmap(tmp);

}



void new_rle_struct(BITMAP *source, const char errtxt [], struct RLE_STRUCT *str, int bl)
{


 int x1 = -1, y1 = -1, x2 = -1, y2 = -1;

 int i, j;

 for (i = 0; i < source->w; i ++)
 {
  for (j = 0; j < source->h; j ++)
  {
    if (getpixel(source, i, j) != 0)
    {
     x1 = i;
     break;
    }
  }
  if (x1 != -1)
   break;
 }

  for (j = 0; j < source->h; j ++)
  {
   for (i = 0; i < source->w; i ++)
   {
    if (getpixel(source, i, j) != 0)
    {
     y1 = j;
     break;
    }
  }
  if (y1 != -1)
   break;
 }

 for (i = source->w - 1; i >= 0; i --)
 {
  for (j = 0; j < source->h; j ++)
  {
    if (getpixel(source, i, j) != 0)
    {
     x2 = i;
     break;
    }
  }
  if (x2 != -1)
   break;
 }


  for (j = source->h - 1; j >= 0; j --)
  {
   for (i = 0; i < source->w; i ++)
   {
    if (getpixel(source, i, j) != 0)
    {
     y2 = j;
     break;
    }
  }
  if (y2 != -1)
   break;
 }



 BITMAP *tmp = new_bitmap(x2 - x1 + 1, y2 - y1 + 1, "new_rle_struct temp bitmap");

 blit(source, tmp, x1, y1, 0, 0, x2 - x1 + 1, y2 - y1 + 1);

//rect(tmp, 0, 0, tmp->w - 1, tmp->h - 1, TRANS_RED_OUT);
 if (bl == -1)
 {
   str->sprite = get_rle_sprite(tmp);
//   str->x = tmp->w / 2;//source->w / 2 - x1;
//   str->y = tmp->h / 2;//source->h / 2 - y1;
   str->x = tmp->w / 2;//source->w / 2 - x1;
   str->y = tmp->h / 2;//source->h / 2 - y1;
 }
  else
  {
   if (bl == -2)
   {
     str->sprite = get_rle_sprite(tmp);
//   str->x = tmp->w / 2;//source->w / 2 - x1;
//   str->y = tmp->h / 2;//source->h / 2 - y1;
     str->x = source->w / 2 - x1;
     str->y = source->h / 2 - y1;
   }
    else
    {
     str [bl].sprite = get_rle_sprite(tmp);
     str [bl].x = tmp->w / 2; //source->w / 2 - x1;
     str [bl].y = tmp->h / 2; //source->h / 2 - y1;
    }
  }

 destroy_bitmap(tmp);
}



void fix_outline(BITMAP *source)
{
 int i, j, px;

 for (i = 0; i < source->w; i ++)
 {
  for (j = 0; j < source->h; j ++)
  {
   px = getpixel(source, i, j);
   switch(px)
   {
       case 116:
        putpixel(source, i, j, COL_OUTLINE);
        break;
       case 61:
        putpixel(source, i, j, COL_COL4);
        break;
   }
  }
 }

}

void fix_trans(BITMAP *source)
{
 int i, j, px;

 for (i = 0; i < source->w; i ++)
 {
  for (j = 0; j < source->h; j ++)
  {
   px = getpixel(source, i, j);
   switch(px)
   {
       case 51://COL_WHITE:
        putpixel(source, i, j, TRANS_ORANGE_OUT);
        break;
       case 30://COL_LGREY:
        putpixel(source, i, j, TRANS_ORANGE_IN);
        break;
   }
  }
 }

}



void extract_rle_struct(BITMAP *source, int x1, int y1, int x2, int y2, const char errtxt [], struct RLE_STRUCT *str, int bl)
{

 BITMAP *tmp = new_bitmap(x2, y2, "extract_rle_struct temp bitmap");

 blit(source, tmp, x1, y1, 0, 0, x2, y2);

 new_rle_struct(tmp, errtxt, str, bl);

 destroy_bitmap(tmp);

}
