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

#include "globvars.h"

#include "palette.h"
#include "game.h"
#include "input.h"

#include "display.h"

#include "stuff.h"

#define TRANS_MODE drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
#define END_TRANS_MODE drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

extern RGB palet[2][256];

RGB splash_palette[256];

BITMAP *splash_bitmap;

RLE_SPRITE *icon_RLEs[30];	// externed in display_init

extern struct RLE_STRUCT eRLE_flower[5][50];

extern struct RLE_STRUCT RLE_multi[WPN_TYPES][8];

RLE_SPRITE *player_icon_RLE;

void key_box (const char ktext[], int command);
int acceptable_char (int scode);
void define_keys (void);
int ship_select (void);
void print_wpn_name (int x, int y, int w);
void print_wpn_text (int x, int y, int w);
void print_standard_names (int x, int y, int w);


void congratulations (void);
void congrat_wait (int waiting);


char skippable_column (int colm);


extern volatile unsigned char ticked;

extern BITMAP *display;

int menu_select;
int key_wait;

int counter2;
int flower_dir;

enum
{
	MENU_START,
	MENU_DIFFICULTY,
	MENU_JOY_OR_KEYS,
	MENU_KEYS,
	MENU_CALIBRATE,
	MENU_EXIT
};


int standard_config[6][3] = {
	{WPN_SCATTER, WPN_CANNON, WPN_SEEKER},
	{WPN_MGUN, WPN_RECT, WPN_BLADE},
	{WPN_LWBEAM, WPN_CANNON, WPN_SEEKER},
	{WPN_SCATTER, WPN_SWBEAM, WPN_EIGHT},
	{WPN_LWBEAM, WPN_SWBEAM, WPN_BLADE},
	{WPN_EIGHT, WPN_SEEKER, WPN_BLADE}

};

void reset_menu_palette (void);
void run_menu_background (void);

int custom_config[3];



#define NO_CFLOWERS 7

int cflower_x[NO_CFLOWERS];
int cflower_y[NO_CFLOWERS];
int cflower_y_offset[NO_CFLOWERS];
int cflower_centre_col[NO_CFLOWERS];
int cflower_centre_size[NO_CFLOWERS];
int cflower_col[NO_CFLOWERS][2];
int cflower_out1[NO_CFLOWERS][2];
int cflower_out2[NO_CFLOWERS][2];
int cflower_angle[NO_CFLOWERS][2];
int cflower_petals[NO_CFLOWERS][2];
int cflower_rot[NO_CFLOWERS][2];
int cflower_rot_inc[NO_CFLOWERS][2];
int made_cflower;

void run_cflowers (void);
void new_cflower (int f);
void cflower_poly4 (BITMAP * target, int x1, int y1, int x2, int y2, int x3,
                    int y3, int x4, int y4, int col);
void init_cflowers (void);



void reset_menu_palette (void)
{

	clear_bitmap (screen);
	vsync ();
	clear_bitmap (display);

	arena.target_palette = 0;
	set_base_palette ();

	set_base_palette ();
	int newpal = build_new_palette (arena.change_palette, 0, 0, 0);
	vsync ();
//   clear_bitmap(screen);
	set_palette (palet[newpal]);
}



void startup_menu (void)
{
	reset_menu_palette ();

	// get custom config from initfile
	custom_config[0] = WPN_SCATTER;
	custom_config[1] = WPN_CANNON;
	custom_config[2] = WPN_SEEKER;

	menu_select = 0;
	key_wait = 30;

	int counter;

	counter = 0;
	counter2 = 0;

	flower_dir = pos_or_neg (1);

	arena.difficulty = 0;

	int y1 = 170;
	int y2 = 190 + menu_select * 30;
	int y3 = 218 + menu_select * 30;
	int y4 = 218 + menu_select * 30;

	int anykey = 0;

	int i;

	while (TRUE)
	{
		clear_bitmap (display);
		run_menu_background ();

		counter += 4;
		if (counter >= 40)
			counter = 0;

		counter2++;
		if (counter2 >= 256)
			counter2 = 0;

		textprintf_centre_ex (display, font, 320, 90, -1, -1,
		                      "G A R D E N   O F   C O L O U R E D   L I G H T S");
		if (options.tourist > 0)
		{/* One plays in (cheat) tourist mode */
			textprintf_centre_ex (display, font, 320, 130, -1, -1,
			                      "TOURIST  MODE:  STAGE  %i", options.tourist);
		}
// textprintf_centre_ex(display, font, 320, 80, -1, -1, "C O L O U R E D   L I G H T S");
// textprintf_centre_ex(display, font, 320, 110, -1, -1, "remember 5 seconds!");

		int my = 250;

		y1 = my - 23;
		y2 = my - 10 + menu_select * 30;
		y3 = my + 19 + menu_select * 30;
		y4 = my + 150;


// rectfill(display, 370, y1, 600, y2, COL_COL1);
		rectfill (display, 370, y2 + 5, 640, y3 - 2, COL_BACK4);
		rect (display, 368, y2 + 3, 641, y3 - 0, COL_BACK3);
// rectfill(display, 370, y3, 600, y4, TRANS_DGREEN);

		textprintf_ex (display, font, 400, my, -1, -1, "START GAME");
		switch (arena.difficulty)
		{
		case 0:
			textprintf_ex (display, font, 400, my + 30, -1, -1, "DIFFICULTY - NORMAL");
			break;
		case 1:
			textprintf_ex (display, font, 400, my + 30, -1, -1, "DIFFICULTY - HARD");
			break;
		case 2:
			textprintf_ex (display, font, 400, my + 30, -1, -1, "DIFFICULTY - PUNISHMENT");
			break;
		}
// textprintf_ex(display [2], font, 400, 260, -1, -1, "STAGE - %i", arena.starting_level);
		textprintf_ex (display, font, 400, my + 90, -1, -1, "SET  KEYS");
		textprintf_ex (display, font, 400, my + 150, -1, -1, "EXIT");

		if (options.joystick)
			textprintf_ex (display, font, 400, my + 120, -1, -1,
			               "CALIBRATE JOYSTICK");
		else
		{
			textprintf_ex (display, font, 400, my + 120, -1, -1, "NO JOYSTICK");
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			rectfill (display, 399, my + 110, 530, my + 135, CONVERT_WHITE_TO_GREY);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);
		}

		if (options.joystick)
		{
			if (options.key_or_joy == 1)
				textprintf_ex (display, font, 400, my + 60, -1, -1, "CONTROLS - JOYSTICK");
			else
				textprintf_ex (display, font, 400, my + 60, -1, -1, "CONTROLS - KEYBOARD");
		}
		else
		{
			textprintf_ex (display, font, 400, my + 60, -1, -1, "NO JOYSTICK");
			drawing_mode (DRAW_MODE_TRANS, NULL, 0, 0);
			rectfill (display, 399, my + 50, 530, my + 75, CONVERT_WHITE_TO_GREY);
			drawing_mode (DRAW_MODE_SOLID, NULL, 0, 0);

		}

		my = 270;

		y2 = my + 7 + arena.just_got_highscore * 30;
		y3 = my + 29 + arena.just_got_highscore * 30;

		if (arena.just_got_highscore > 0)
		{
			rectfill (display, 0, y2 - 5, 250, y3, COL_COL3);
			rect (display, -1, y2 - 5 - 2, 250 + 2, y3 + 2, COL_COL4);
		}

		textprintf_right_ex (display, font, 137, my, -1, -1, "HIGH");
		textprintf_ex (display, font, 157, my, -1, -1, "SCORES");

		textprintf_right_ex (display, font, 137, my + 40, -1, -1, "NORMAL");
		textprintf_ex (display, font, 157, my + 40, -1, -1, "%i",
		               options.highscore[0]);

		textprintf_right_ex (display, font, 137, my + 70, -1, -1, "HARD");
		textprintf_ex (display, font, 157, my + 70, -1, -1, "%i",
		               options.highscore[1]);

		textprintf_right_ex (display, font, 137, my + 100, -1, -1, "PUNISHMENT");
		textprintf_ex (display, font, 157, my + 100, -1, -1, "%i",
		               options.highscore[2]);

		if ( ( keypressed() ) && key_wait == 0)
		{
			int pressed_key = readkey() >> 8;
			clear_keybuf();
			if (pressed_key == KEY_UP || pressed_key == KEY_8_PAD)
			{
				menu_select--;
				if (menu_select < 0)
					menu_select = MENU_EXIT;
				if (menu_select == MENU_CALIBRATE && options.joystick == 0)
					menu_select = MENU_KEYS;
				if (menu_select == MENU_JOY_OR_KEYS && options.joystick == 0)
					menu_select = MENU_JOY_OR_KEYS - 1;
				key_wait = 7;
			}
			if (pressed_key == KEY_DOWN || pressed_key == KEY_2_PAD)
			{
				menu_select++;
				if (menu_select > MENU_EXIT)
					menu_select = 0;
				if (menu_select == MENU_CALIBRATE && options.joystick == 0)
					menu_select = MENU_EXIT;
				if (menu_select == MENU_JOY_OR_KEYS && options.joystick == 0)
					menu_select = MENU_KEYS;
				key_wait = 7;
			}
			if (pressed_key == KEY_LEFT || pressed_key == KEY_4_PAD)
			{
				if (menu_select == MENU_JOY_OR_KEYS)
				{
					if (options.key_or_joy == 0)
						options.key_or_joy = 1;
					else
						options.key_or_joy = 0;
				}
				if (menu_select == MENU_DIFFICULTY)
				{
					arena.difficulty--;
					if (arena.difficulty < 0)
						arena.difficulty = 0;
				}
				key_wait = 11;
			}
			if (pressed_key == KEY_RIGHT || pressed_key == KEY_6_PAD)
			{
				if (menu_select == MENU_JOY_OR_KEYS)
				{
					if (options.key_or_joy == 0)
						options.key_or_joy = 1;
					else
						options.key_or_joy = 0;
				}
				if (menu_select == MENU_DIFFICULTY)
				{
					arena.difficulty++;
					if (arena.difficulty > 2)
						arena.difficulty = 2;
				}
				key_wait = 11;
			}

			if (pressed_key == KEY_ESC)
				if (run_pause () == 1)
					exit(0);

			if (pressed_key == KEY_ENTER || pressed_key == KEY_SPACE || pressed_key == KEY_Z)
			{
				if (menu_select == MENU_EXIT)
					if (run_pause () == 1)
						exit(0);

				if (menu_select == MENU_KEYS)
				{
					key_wait = 10;
					define_keys ();
					key_wait = 10;
				}

				if (menu_select == MENU_CALIBRATE)
				{
					jstick_calibrate ();
					key_wait = 20;
					ticked = 0;
				}

				if (menu_select == MENU_START)
				{
					ticked = 0;
					key_wait = 30;
					if (ship_select () == 1)
					{
						clear_bitmap (display);
						new_game ();
						game_loop ();
						if (arena.level == 5)
							congratulations ();
						reset_menu_palette ();
						key_wait = 10;	// was 1
						flower_dir *= -1;
						ticked = 0;
					}
					else
						key_wait = 20;
				}
				else
					key_wait = 20;
			}
		}//endif (keypressed() && key_wait == 0)
		else
		{
			int keys_before = 1;
			if (keypressed())
			{
				clear_keybuf(); /* we are not interested in keys in the queue */
				rest(20);
			}
			else
			{
				keys_before = 0; /* there were no keys pressed before */
				rest(10);
			}

			jstick_to_keypressed();
			key_wait -= 2;
			if ( ( !keypressed() && !keys_before ) || key_wait <= 0)
			{/* we can process next keypress since the previous keypress has ended */
				key_wait = 0; 
			}
		}

		textprintf_right_ex (display, font, 625, 460, -1, -1, "COPYRIGHT 2007 LINLEY HENZELL");

		do
		{
			rest(5);
		}
		while (ticked == 0);
		ticked = 0;

		vsync ();
		blit (display, screen, 0, 0, 0, 0, 640, 480);


		/*


		if (key_wait == 0)
		{
			if (key[KEY_UP] || key[KEY_8_PAD])
			{
				menu_select--;
				if (menu_select < 0)
					menu_select = MENU_EXIT;
				if (menu_select == MENU_CALIBRATE && options.joystick == 0)
					menu_select = MENU_KEYS;
				if (menu_select == MENU_JOY_OR_KEYS && options.joystick == 0)
					menu_select = MENU_JOY_OR_KEYS - 1;
				key_wait = 7;
			}
			if (key[KEY_DOWN] || key[KEY_2_PAD])
			{
				menu_select++;
				if (menu_select > MENU_EXIT)
					menu_select = 0;
				if (menu_select == MENU_CALIBRATE && options.joystick == 0)
					menu_select = MENU_EXIT;
				if (menu_select == MENU_JOY_OR_KEYS && options.joystick == 0)
					menu_select = MENU_KEYS;
				key_wait = 7;
			}
			if (key[KEY_LEFT] || key[KEY_4_PAD])
			{
				if (menu_select == MENU_JOY_OR_KEYS)
				{
					if (options.key_or_joy == 0)
						options.key_or_joy = 1;
					else
						options.key_or_joy = 0;
				}
				if (menu_select == MENU_DIFFICULTY)
				{
					arena.difficulty--;
					if (arena.difficulty < 0)
						arena.difficulty = 0;
				}
				key_wait = 7;
			}
			if (key[KEY_RIGHT] || key[KEY_6_PAD])
			{
				if (menu_select == MENU_JOY_OR_KEYS)
				{
					if (options.key_or_joy == 0)
						options.key_or_joy = 1;
					else
						options.key_or_joy = 0;
				}
				if (menu_select == MENU_DIFFICULTY)
				{
					arena.difficulty++;
					if (arena.difficulty > 2)
						arena.difficulty = 2;
				}
				key_wait = 7;
			}

			if (key[KEY_ENTER] || key[KEY_SPACE] || key[KEY_Z])
			{
				if (menu_select == MENU_EXIT)
					exit (0);

				if (menu_select == MENU_KEYS)
				{
					key_wait = 10;
					define_keys ();
					key_wait = 10;
				}

				if (menu_select == MENU_CALIBRATE)
				{
					jstick_calibrate ();
					key_wait = 20;
					ticked = 0;
				}

				if (menu_select == MENU_START)
				{
					ticked = 0;
					key_wait = 30;
					if (ship_select () == 1)
					{
						clear_bitmap (display);
						new_game ();
						game_loop ();
						if (arena.level == 5)
							congratulations ();
						reset_menu_palette ();
						key_wait = 10;	// was 1
						flower_dir *= -1;
						ticked = 0;
					}
					else
						key_wait = 20;
				}
				else
					key_wait = 20;
			}
		}
		else
			key_wait--;


// rectfill(display, 360, 430, 635, 461, TRANS_SH1_IN);

		textprintf_right_ex (display, font, 625, 460, -1, -1, "COPYRIGHT 2007 LINLEY HENZELL");



		do
		{
			thing++;
		}
		while (ticked == 0);
		ticked = 0;

		vsync ();
		blit (display, screen, 0, 0, 0, 0, 640, 480); */



	};


}

void define_keys (void)
{

	key_box ("PRESS KEY FOR UP", CKEY_UP);
	key_box ("PRESS KEY FOR LEFT", CKEY_LEFT);
	key_box ("PRESS KEY FOR RIGHT", CKEY_RIGHT);
	key_box ("PRESS KEY FOR DOWN", CKEY_DOWN);
	key_box ("PRESS KEY FOR FIRE 1", CKEY_FIRE1);
	key_box ("PRESS KEY FOR FIRE 2", CKEY_FIRE2);
	key_box ("PRESS KEY FOR FIRE 3", CKEY_FIRE3);
	key_box ("PRESS KEY FOR SLOW", CKEY_SLOW);

	set_config_int ("Misc", "key_up", player.key[CKEY_UP]);
	set_config_int ("Misc", "key_left", player.key[CKEY_LEFT]);
	set_config_int ("Misc", "key_right", player.key[CKEY_RIGHT]);
	set_config_int ("Misc", "key_down", player.key[CKEY_DOWN]);
	set_config_int ("Misc", "key_fire1", player.key[CKEY_FIRE1]);
	set_config_int ("Misc", "key_fire2", player.key[CKEY_FIRE2]);
	set_config_int ("Misc", "key_fire3", player.key[CKEY_FIRE3]);
	set_config_int ("Misc", "key_slow", player.key[CKEY_SLOW]);

}


void key_box (const char ktext[], int command)
{
	rectfill (display, 210, 200, 430, 250, COL_OUTLINE);
	rect (display, 211, 201, 429, 249, COL_COL1);

	textprintf_centre_ex (display, font, 320, 222, -1, -1, ktext);

	vsync ();
	blit (display, screen, 0, 0, 0, 0, 640, 480);

	int inputted = KEY_ESC;

	int i;
	int anykey = 0;

	do
	{
		rest(200);
		clear_keybuf();
		while (keypressed() != TRUE)
			rest(20);
		inputted = readkey() >> 8; /* the conversion to the right key format */
	}/* if inputted is acceptable, then we want out of this loop  */
	while (acceptable_char(inputted) != 1);
	player.key[command] = inputted;
}





int ship_select (void)
{

	int row = 0;
	int column[4] = { 0, WPN_SCATTER, WPN_CANNON, WPN_SEEKER };
	int i, j, k;
	int out_count = 0;

	int x1, y1, anykey, out;
	char column_changed = 0;

	while (TRUE)
	{

		clear_to_color (display, COL_OUTLINE);

		counter += 4;
		if (counter >= 40)
			counter = 0;

		out_count += 32;
		out_count &= 1023;

		y1 = 170;
		x1 = 55;

		int col1;
		int col2;

// TRANS_MODE
		for (i = 1; i < 4; i++)
		{
			col1 = COL_BACK2;
			col2 = COL_BACK3;
			if (skippable_column (column[i]))
			{
				col1 = COL_BACK2;
				col2 = COL_BACK3;
				rectfill (display, 5 + x1 + column[i] * 60, y1 + 110,
				          34 + x1 + column[i] * 60, 460, col1);
				rect (display, 5 + x1 + column[i] * 60 - 2, y1 + 110 - 2,
				      34 + x1 + column[i] * 60 + 2, 460 + 2, col2);
			}
			else
			{
				rectfill (display, -3 + x1 + column[i] * 60, y1 + (i + 1) * 60 - 4,
				          42 + x1 + column[i] * 60, y1 + (i + 1) * 60 + 39 + 4, col1);
				rect (display, -3 + x1 + column[i] * 60 - 2,
				      y1 + (i + 1) * 60 - 2 - 4, 42 + x1 + column[i] * 60 + 2,
				      y1 + (i + 1) * 60 + 41 + 4, col2);
			}
		}
// END_TRANS_MODE

		for (j = 0; j < 4; j++)
		{
			if (row == j)
			{
				TRANS_MODE
				rectfill (display, 0, y1 + j * 60 + 10, 640, y1 + 30 + j * 60,
				          TRANS_ORANGE_IN);
				hline (display, 0, y1 + j * 60 + 9, 640, TRANS_ORANGE_OUT);
				hline (display, 0, y1 + 31 + j * 60, 640, TRANS_ORANGE_OUT);
				END_TRANS_MODE
			}
			for (i = 0; i < 9; i++)
			{
//   if (j == 0)
				if (j == 0)
				{

					if (i > 0 && i < 7)
					{
						draw_rle_sprite (display, player_icon_RLE, 20 + x1 + i * 60 - 11,
						                 20 + y1 + j * 60 - 15);
						for (k = 0; k < 3; k++)
						{
							draw_rle_sprite (display,
							                 RLE_multi[standard_config[i - 1][k]][0].sprite,
							                 20 + x1 + i * 60 -
							                 RLE_multi[standard_config[i - 1][k]][0].x,
							                 20 + y1 + j * 60 -
							                 RLE_multi[standard_config[i - 1][k]][0].y +
							                 30 + k * 15);
						}
					}
					if (i == 7)
					{
						draw_rle_sprite (display, player_icon_RLE, 20 + x1 + i * 60 - 11,
						                 20 + y1 + j * 60 - 15);
						for (k = 0; k < 3; k++)
						{
							draw_rle_sprite (display, RLE_multi[custom_config[k]][0].sprite,
							                 20 + x1 + i * 60 -
							                 RLE_multi[custom_config[k]][0].x,
							                 20 + y1 + j * 60 -
							                 RLE_multi[custom_config[k]][0].y + 30 +
							                 k * 15);
						}
					}
					if (i == 8)
					{
						textprintf_centre_ex (display, font, 20 + x1 + i * 60,
						                      13 + y1 + j * 60, -1, -1, "EXIT");
					}
					if (i == 0)
					{
						textprintf_centre_ex (display, font, 20 + x1 + i * 60,
						                      13 + y1 + j * 60, -1, -1, "HELP");
					}

					continue;
				}

				if (column[j] == i)
				{
//      rectfill(display, x1 + i * 60, y1 + j * 60, x1 + 40 + i * 60, y1 + 40 + j * 60, COL_COL3);
					draw_rle_sprite (display, RLE_multi[i][0].sprite,
					                 20 + x1 + i * 60 - RLE_multi[i][0].x,
					                 20 + y1 + j * 60 - RLE_multi[i][0].y);

//      draw_rle_sprite(display, icon_RLEs [i], x1 + i * 60, y1 + j * 60);
				}
				else
				{
					//rectfill(display, x1 + i * 60, y1 + j * 60, x1 + 40 + i * 60, y1 + 40 + j * 60, COL_COL4);
					draw_rle_sprite (display, RLE_multi[i][0].sprite,
					                 20 + x1 + i * 60 - RLE_multi[i][0].x,
					                 20 + y1 + j * 60 - RLE_multi[i][0].y);
//      draw_rle_sprite(display, RLE_multi [i] [0].sprite, 20 + x1 + i * 60 - RLE_multi [0] [k].x, 20 + y1 + j * 60 - RLE_multi [i] [0].y);
//      draw_rle_sprite(display, icon_RLEs [i], x1 + i * 60, y1 + j * 60);
				}

//      rectfill(display, x1 + i * 60, y1 + j * 60, x1 + 40 + i * 60, y1 + 40 + j * 60, COL_COL4);

			}
			if (j == 0)
				y1 += 60;
		}

		TRANS_MODE y1 = 170;

		for (j = 0; j < 4; j++)
		{
			if (j == 0)
			{
				if (column[0] > 0 && column[0] < 8)
				{
					rect (display, x1 + (column[j] * 60), y1 + j * 60,
					      x1 + (column[j] * 60) + 39, y1 + j * 60 + 92,
					      TRANS_YELLOW_OUT);
					rect (display, x1 + (column[j] * 60) + 1, y1 + j * 60 + 1,
					      x1 + (column[j] * 60) + 38, y1 + j * 60 + 91,
					      TRANS_YELLOW_IN);
					rect (display, x1 + (column[j] * 60) + 2, y1 + j * 60 + 2,
					      x1 + (column[j] * 60) + 37, y1 + j * 60 + 90,
					      TRANS_YELLOW_OUT);
				}
				else
				{
					rect (display, x1 + (column[j] * 60) - 4, y1 + j * 60,
					      x1 + (column[j] * 60) + 43, y1 + j * 60 + 39,
					      TRANS_YELLOW_OUT);
					rect (display, x1 + (column[j] * 60) - 3, y1 + j * 60 + 1,
					      x1 + (column[j] * 60) + 42, y1 + j * 60 + 38,
					      TRANS_YELLOW_IN);
					rect (display, x1 + (column[j] * 60) - 2, y1 + j * 60 + 2,
					      x1 + (column[j] * 60) + 41, y1 + j * 60 + 37,
					      TRANS_YELLOW_OUT);
				}
			}
			else
			{
				rect (display, x1 + (column[j] * 60), y1 + j * 60,
				      x1 + (column[j] * 60) + 39, y1 + j * 60 + 39, TRANS_YELLOW_OUT);
				rect (display, x1 + (column[j] * 60) + 1, y1 + j * 60 + 1,
				      x1 + (column[j] * 60) + 38, y1 + j * 60 + 38, TRANS_YELLOW_IN);
				rect (display, x1 + (column[j] * 60) + 2, y1 + j * 60 + 2,
				      x1 + (column[j] * 60) + 37, y1 + j * 60 + 37, TRANS_YELLOW_OUT);
			}

			if (row == j)
			{
				out = xpart (out_count, 6);
				if (row == 0)
				{
					out += 10;
					if (column[0] > 0 && column[0] < 8)
					{
						rect (display, x1 + (column[j] * 60) - out, y1 + j * 60 - out,
						      x1 + (column[j] * 60) + 39 + out, y1 + j * 60 + 92 + out,
						      TRANS_ORANGE_OUT);
						rect (display, x1 + (column[j] * 60) + 1 - out,
						      y1 + j * 60 + 1 - out, x1 + (column[j] * 60) + 38 + out,
						      y1 + j * 60 + 91 + out, TRANS_ORANGE_IN);
						rect (display, x1 + (column[j] * 60) + 2 - out,
						      y1 + j * 60 + 2 - out, x1 + (column[j] * 60) + 37 + out,
						      y1 + j * 60 + 90 + out, TRANS_ORANGE_OUT);
					}
					else
					{
						rect (display, x1 + (column[j] * 60) - out, y1 + j * 60 - out,
						      x1 + (column[j] * 60) + 39 + out, y1 + j * 60 + 39 + out,
						      TRANS_ORANGE_OUT);
						rect (display, x1 + (column[j] * 60) + 1 - out,
						      y1 + j * 60 + 1 - out, x1 + (column[j] * 60) + 38 + out,
						      y1 + j * 60 + 38 + out, TRANS_ORANGE_IN);
						rect (display, x1 + (column[j] * 60) + 2 - out,
						      y1 + j * 60 + 2 - out, x1 + (column[j] * 60) + 37 + out,
						      y1 + j * 60 + 37 + out, TRANS_ORANGE_OUT);
					}
				}
				else
				{
					rect (display, x1 + (column[j] * 60) - out, y1 + j * 60 - out,
					      x1 + (column[j] * 60) + 39 + out, y1 + j * 60 + 39 + out,
					      TRANS_ORANGE_OUT);
					rect (display, x1 + (column[j] * 60) + 1 - out,
					      y1 + j * 60 + 1 - out, x1 + (column[j] * 60) + 38 + out,
					      y1 + j * 60 + 38 + out, TRANS_ORANGE_IN);
					rect (display, x1 + (column[j] * 60) + 2 - out,
					      y1 + j * 60 + 2 - out, x1 + (column[j] * 60) + 37 + out,
					      y1 + j * 60 + 37 + out, TRANS_ORANGE_OUT);
				}

			}

			if (j > 0 || (column[0] > 0 && column[0] < 8))
			{
				if (j == 0)
				{
					vline (display, x1 + (column[j] * 60) + 19, y1 + j * 60 + 92,
					       y1 + j * 60 + 101, TRANS_YELLOW_IN);
					vline (display, x1 + (column[j] * 60) + 18, y1 + j * 60 + 91,
					       y1 + j * 60 + 102, TRANS_YELLOW_OUT);
					vline (display, x1 + (column[j] * 60) + 20, y1 + j * 60 + 90,
					       y1 + j * 60 + 102, TRANS_YELLOW_OUT);

					vline (display, x1 + (column[j + 1] * 60) + 19, y1 + j * 60 + 102,
					       y1 + j * 60 + 121, TRANS_YELLOW_IN);
					vline (display, x1 + (column[j + 1] * 60) + 18, y1 + j * 60 + 100,
					       y1 + j * 60 + 120, TRANS_YELLOW_OUT);
					vline (display, x1 + (column[j + 1] * 60) + 20, y1 + j * 60 + 100,
					       y1 + j * 60 + 119, TRANS_YELLOW_OUT);

					hline (display, x1 + (column[j] * 60) + 19, y1 + j * 60 + 101,
					       x1 + (column[j + 1] * 60) + 19, TRANS_YELLOW_IN);
					hline (display, x1 + (column[j] * 60) + 19, y1 + j * 60 + 102,
					       x1 + (column[j + 1] * 60) + 19, TRANS_YELLOW_OUT);
					hline (display, x1 + (column[j] * 60) + 19, y1 + j * 60 + 100,
					       x1 + (column[j + 1] * 60) + 19, TRANS_YELLOW_OUT);
				}
				else
				{
					if (j < 3)
					{
						vline (display, x1 + (column[j] * 60) + 19, y1 + j * 60 + 38,
						       y1 + j * 60 + 48, TRANS_YELLOW_IN);
						vline (display, x1 + (column[j] * 60) + 18, y1 + j * 60 + 38,
						       y1 + j * 60 + 49, TRANS_YELLOW_OUT);
						vline (display, x1 + (column[j] * 60) + 20, y1 + j * 60 + 38,
						       y1 + j * 60 + 49, TRANS_YELLOW_OUT);

						vline (display, x1 + (column[j + 1] * 60) + 19, y1 + j * 60 + 48,
						       y1 + j * 60 + 61, TRANS_YELLOW_IN);
						vline (display, x1 + (column[j + 1] * 60) + 18, y1 + j * 60 + 47,
						       y1 + j * 60 + 61, TRANS_YELLOW_OUT);
						vline (display, x1 + (column[j + 1] * 60) + 20, y1 + j * 60 + 47,
						       y1 + j * 60 + 61, TRANS_YELLOW_OUT);

						hline (display, x1 + (column[j] * 60) + 19, y1 + j * 60 + 48,
						       x1 + (column[j + 1] * 60) + 19, TRANS_YELLOW_IN);
						hline (display, x1 + (column[j] * 60) + 19, y1 + j * 60 + 47,
						       x1 + (column[j + 1] * 60) + 19, TRANS_YELLOW_OUT);
						hline (display, x1 + (column[j] * 60) + 19, y1 + j * 60 + 49,
						       x1 + (column[j + 1] * 60) + 19, TRANS_YELLOW_OUT);
					}
				}
			}

			if (j == 0)
				y1 += 60;


		}


		END_TRANS_MODE if (row > 0)
		{
			if (column[row] < 3 || column[row] > 6)
				textprintf_right_ex (display, font, 280, 40, -1, -1, "FAST  WEAPON:");
			else
				textprintf_right_ex (display, font, 280, 40, -1, -1, "SLOW  WEAPON:");
			print_wpn_name (290, 40, column[row]);
			print_wpn_text (290, 80, column[row]);

			if (column[row] == 0 || column[row] > 6)
				textprintf_ex (display, font, 290, 130, -1, -1, "ONE  ONLY");

		}

		if (row == 0 && column[0] == 0)
		{
			textprintf_centre_ex (display, font, 320, 20, -1, -1,
			                      "SELECT  YOUR  ARMAMENT");
			textprintf_centre_ex (display, font, 320, 60, -1, -1,
			                      "SELECT  A  STANDARD CONFIGURATION  ALONG  THE  TOP  ROW");
			textprintf_centre_ex (display, font, 320, 80, -1, -1,
			                      "OR  CUSTOMISE  BY  CHOOSING  SPECIFIC  WEAPONS");
			textprintf_centre_ex (display, font, 320, 100, -1, -1,
			                      "ON  THE  OTHER  ROWS");
			textprintf_centre_ex (display, font, 320, 130, -1, -1,
			                      "PRESS  FIRE 1  TO  BEGIN");
		}

		if (row == 0 && column[0] == 8)
		{
			textprintf_centre_ex (display, font, 320, 40, -1, -1,
			                      "PRESS  FIRE 1  TO  EXIT");
		}

		if (row == 0 && column[0] > 0 && column[0] < 8)
		{
			if (column[0] == 7)
				textprintf_centre_ex (display, font, 320, 40, -1, -1,
				                      "SPECIAL  CONFIGURATION");
			else
			{
				if (column[0] == 6)
					textprintf_centre_ex (display, font, 320, 40, -1, -1,
					                      "CHALLENGE  CONFIGURATION");
				else
					textprintf_centre_ex (display, font, 320, 40, -1, -1,
					                      "STANDARD  CONFIGURATION");
			}
			print_standard_names (320, 80, column[row] - 1);

		}
/*
		poll_joystick();
		if (joy[0].stick[options.joy_stick].axis[0].d1)
		{ simulate_keypress(KEY_LEFT << 8); }
		if (joy[0].stick[options.joy_stick].axis[0].d2)
		{ simulate_keypress(KEY_RIGHT << 8); }
		if (joy[0].stick[options.joy_stick].axis[1].d1)
		{ simulate_keypress(KEY_UP << 8); }
		if (joy[0].stick[options.joy_stick].axis[1].d2)
		{ simulate_keypress(KEY_DOWN << 8); }
		if (joy[0].button[options.joy_button[0]].b)
		{ simulate_keypress(KEY_ENTER << 8); }*/

		if ( ( keypressed() ) && key_wait == 0)
		{
			int pressed_key = readkey() >> 8;
			clear_keybuf();
			if (pressed_key == KEY_UP || pressed_key == KEY_8_PAD)
			{
				row--;
				if (row < 0)
					row = 3;
				key_wait = 7;
			}
			if (pressed_key == KEY_DOWN || pressed_key == KEY_2_PAD)
			{
				row++;
				if (row > 3)
					row = 0;
				key_wait = 7;
			}
			if (pressed_key == KEY_LEFT || pressed_key == KEY_4_PAD)
			{
				do
				{
					column[row]--;
					if (column[row] < 0)
						column[row] = 8;
					column_changed = 1;
				}
				while (row > 0 && skippable_column (column[row])
				        && ((column[row] == column[1]) + (column[row] == column[2]) +
				            (column[row] == column[3]) > 1));
				key_wait = 7;
			}
			if (pressed_key == KEY_RIGHT || pressed_key == KEY_6_PAD)
			{
				do
				{
					column[row]++;
					if (column[row] > 8)
						column[row] = 0;
					column_changed = 1;
				}
				while (row > 0 && skippable_column (column[row])
				        && ((column[row] == column[1]) + (column[row] == column[2]) +
				            (column[row] == column[3]) > 1));
				key_wait = 7;
			}

			if (column_changed == 1)
			{
				column_changed = 0;
				if (row > 0 && column[0] <= 7)
				{
					column[0] = 7;
					custom_config[0] = column[1];
					custom_config[1] = column[2];
					custom_config[2] = column[3];
				}

				if (row == 0)
				{
					if (column[0] > 0 && column[0] < 7)
					{
						column[1] = standard_config[column[0] - 1][0];
						column[2] = standard_config[column[0] - 1][1];
						column[3] = standard_config[column[0] - 1][2];
					}
					if (column[0] == 7)
					{
						column[1] = custom_config[0];
						column[2] = custom_config[1];
						column[3] = custom_config[2];
					}
				}

			}

			if (pressed_key == KEY_ESC)
				return 0;

			if (pressed_key == KEY_ENTER || pressed_key == KEY_SPACE || pressed_key == KEY_Z)
			{
				if (row == 0 && column[0] == 8)
					return 0;
				break;
			}
		}//endif (keypressed() && key_wait == 0)
		else
		{		
			int keys_before = 1;
			if (keypressed())
				clear_keybuf(); /* we are not interested in keys in the queue */
			else
				keys_before = 0; /* there were no keys pressed before */

			rest(20);

			jstick_to_keypressed();
			key_wait -= 2;
			if ( ( !keypressed() && !keys_before ) || key_wait <= 0)
			{/* we can process next keypress since the previous keypress has ended */
				key_wait = 0; 
			}
		}

		/*
		do
		{
			rest(5);
		}
		while (ticked == 0);
		*/
		ticked = 0;

		vsync ();
		blit (display, screen, 0, 0, 0, 0, 640, 480);



	};

	player.orbitals = 3;
	player.orbital_angle = 0;

	for (i = 0; i < 3; i++)
	{
		player.weapon[i] = column[i + 1];
		player.wx[i] = -1000;
		player.wy[i] = -1000;
		player.wstatus[i] = 0;
		player.wstyle[i] = WSTYLE_ORBITAL;
		player.wtype[i] = FAST_WEAPON;

		switch (player.weapon[i])
		{
		case WPN_MGUN:
			player.wstyle[i] = WSTYLE_FRONT;
			player.orbitals--;
			break;
		case WPN_BLADE:
			player.wstyle[i] = WSTYLE_BLADE;
			player.blade_index = i;
			player.orbitals--;
			break;
		case WPN_EIGHT:
			player.ring_index = i;
			break;
		case WPN_LWBEAM:
//     player.beam_index = i;
			break;
		case WPN_CANNON:
			player.wtype[i] = SLOW_WEAPON;
			break;
		case WPN_RECT:
			player.wtype[i] = SLOW_WEAPON;
			break;
		case WPN_SWBEAM:
			player.wtype[i] = SLOW_WEAPON;
			break;
		case WPN_SEEKER:
			player.wtype[i] = SLOW_WEAPON;
			break;



		}


	}

	return 1;

}

char skippable_column (int colm)
{
	switch (colm)
	{
	case WPN_MGUN:
		return 1;
	case WPN_BLADE:
		return 1;
	case WPN_EIGHT:
		return 1;
	}

	return 0;

}


int acceptable_char (int scode)
{

	switch (scode)
	{
	case KEY_ESC:
		key_wait = 7;
		return 0;
	case KEY_ENTER:
	case KEY_ENTER_PAD:
		key_wait = 7;
		return 0;
	}
	return 1;

}

void print_wpn_name (int x, int y, int w)
{

	switch (w)
	{
	case WPN_CANNON:
		textprintf_ex (display, font, x, y, -1, -1, "VAPOUR  ORB");
		break;
	case WPN_MGUN:
		textprintf_ex (display, font, x, y, -1, -1, "SOLAR  BITE");
		break;
	case WPN_SCATTER:
		textprintf_ex (display, font, x, y, -1, -1, "SUNSHOWER");
		break;
	case WPN_SEEKER:
		textprintf_ex (display, font, x, y, -1, -1, "WORMS  OF  TORMENT");
		break;
	case WPN_EIGHT:
		textprintf_ex (display, font, x, y, -1, -1, "EIGHT  MOUTHS  OF  HELL");
		break;
	case WPN_LWBEAM:
		textprintf_ex (display, font, x, y, -1, -1, "ENDURING  FILAMENT");
		break;
	case WPN_SWBEAM:
		textprintf_ex (display, font, x, y, -1, -1, "SUDDEN  FILAMENT");
		break;
	case WPN_RECT:
		textprintf_ex (display, font, x, y, -1, -1, "EYE  OF  RECTIFICATION");
		break;
	case WPN_BLADE:
		textprintf_ex (display, font, x, y, -1, -1, "TOOTH  OF  XOM");
		break;

	}

}

void print_wpn_text (int x, int y, int w)
{

	switch (w)
	{
	case WPN_CANNON:
		textprintf_ex (display, font, x, y, -1, -1, "VERY  EFFECTIVE  AGAINST");
		textprintf_ex (display, font, x, y + 15, -1, -1,
		               "SINGLE  LARGE  TARGETS");
		break;
	case WPN_MGUN:
		textprintf_ex (display, font, x, y, -1, -1, "PRECISE  STRIKE  AGAINST");
		textprintf_ex (display, font, x, y + 15, -1, -1, "SMALL  TARGETS");
		break;
	case WPN_SCATTER:
		textprintf_ex (display, font, x, y, -1, -1, "SCATTERED  FIRE  AGAINST");
		textprintf_ex (display, font, x, y + 15, -1, -1, "SMALL  TARGETS");
		break;
	case WPN_SEEKER:
		textprintf_ex (display, font, x, y, -1, -1, "LOCKS  ON  TO");
		textprintf_ex (display, font, x, y + 15, -1, -1, "MULTIPLE  TARGETS");
		textprintf_ex (display, font, x, y + 30, -1, -1, "BEHIND  YOU");
		break;
	case WPN_RECT:
		textprintf_ex (display, font, x, y, -1, -1, "POWERFUL  AREA  STRIKE");
		textprintf_ex (display, font, x, y + 15, -1, -1,
		               "AGAINST  MULTIPLE  TARGETS");
		break;
	case WPN_LWBEAM:
		textprintf_ex (display, font, x, y, -1, -1, "PIERCES  MULTIPLE  TARGETS");
		break;
	case WPN_SWBEAM:
		textprintf_ex (display, font, x, y, -1, -1, "PIERCES  MULTIPLE  TARGETS");
		textprintf_ex (display, font, x, y + 15, -1, -1, "FOR  GREAT  DAMAGE");
		break;
	case WPN_EIGHT:
		textprintf_ex (display, font, x, y, -1, -1, "MOVE  AND  FIRE  IN");
		textprintf_ex (display, font, x, y + 15, -1, -1,
		               "ALL  EIGHT  DIRECTIONS");
		break;
	case WPN_BLADE:
		textprintf_ex (display, font, x, y, -1, -1, "TAP  TO  SLASH");
		textprintf_ex (display, font, x, y + 15, -1, -1, "HOLD  TO  SWING");
		break;
	}
}


void print_standard_names (int x, int y, int w)
{

	switch (w)
	{
	case 0:
		textprintf_centre_ex (display, font, x, y, -1, -1, "FIGHTER  TYPE");
		break;
	case 1:
		textprintf_centre_ex (display, font, x, y, -1, -1, "RIOT  TYPE");
		break;
	case 2:
		textprintf_centre_ex (display, font, x, y, -1, -1, "FILAMENT  TYPE  1");
		break;
	case 3:
		textprintf_centre_ex (display, font, x, y, -1, -1, "FILAMENT  TYPE  2");
		break;
	case 4:
		textprintf_centre_ex (display, font, x, y, -1, -1, "PURE  TYPE");
		break;
	case 5:
		textprintf_centre_ex (display, font, x, y, -1, -1, "ARENA  TYPE");
		break;
	case 6:
		textprintf_centre_ex (display, font, x, y, -1, -1, "CUSTOM  TYPE");
		break;
	}
}


void run_menu_background (void)
{

	static int flower_counter = 0;

//    if (counter2 & 1)
	flower_counter += flower_dir;
	if (flower_counter >= 40)
		flower_counter = 0;
	if (flower_counter < 0)
		flower_counter = 39;

	int x = 275;
	int y = 150;

	draw_rle_sprite (display, eRLE_flower[0][flower_counter].sprite,
	                 x - eRLE_flower[0][flower_counter].x + 42 + 3,
	                 y - eRLE_flower[0][flower_counter].y + 47);

//  vline(display, 320, 0, 480, COL_WHITE);


}

void congratulations (void)
{

	clear_bitmap (screen);
	vsync ();
	clear_bitmap (display);

	arena.target_palette = BASECOL_CFLOWER;
	set_base_palette ();

	int newpal = build_new_palette (arena.change_palette, 0, 0, 0);
	vsync ();
	set_palette (palet[newpal]);

	int y;
	int y_int = 35;

	int c_count = 0;

	init_cflowers ();

	do
	{

		run_cflowers ();

		y = 90;

		textprintf_centre_ex (display, font, 320, y, -1, -1,
		                      "C O N G R A T U L A T I O N S ! !");

		y += y_int * 2;

		if (c_count >= 100)
			textprintf_centre_ex (display, font, 320, y, -1, -1,
			                      "YOU  HAVE  CLEANSED  THE  INNER  GARDEN,");
		y += y_int;

		if (c_count >= 200)
			textprintf_centre_ex (display, font, 320, y, -1, -1,
			                      "RAIDED  THE  FLEET,");
		y += y_int;

		if (c_count >= 300)
			textprintf_centre_ex (display, font, 320, y, -1, -1,
			                      "TRAVERSED  THE  OUTER  REGIONS,");

		y += y_int;
		if (c_count >= 400)
			textprintf_centre_ex (display, font, 320, y, -1, -1,
			                      "AND  ELIMINATED  THE  SOURCE  OF  THE  PARASITES.");

		y += y_int * 2;
		if (c_count >= 550)
		{
			switch (arena.difficulty)
			{
			case 0:
				textprintf_centre_ex (display, font, 320, y, -1, -1,
				                      "BUT  CAN  YOU  DEFEAT  A  MORE  CHALLENGING  LEVEL?");
				break;
			case 1:
				textprintf_centre_ex (display, font, 320, y, -1, -1,
				                      "YOUR  ACHIEVEMENT  IS  IMPRESSIVE!");
				break;
			case 2:
				textprintf_centre_ex (display, font, 320, y, -1, -1,
				                      "YOUR  WORK  HERE  IS  COMPLETE.");
				break;
			}
		}

		if (c_count >= 700)
		{
			y += y_int * 2;
			textprintf_centre_ex (display, font, 320, y, -1, -1,
			                      "PRESS  SPACE  TO  CONTINUE");

			TRANS_MODE
			rectfill (display, 100, y, 500, y + 15, CONVERT_WHITE_TO_GREY);
			END_TRANS_MODE if (key[KEY_SPACE])
				break;
		}

		blit (display, screen, 0, 0, 0, 0, 640, 480);

		do
		{
			rest (5);
		}
		while (ticked == 0);

		ticked--;
		c_count++;
		if (c_count > 30000)
			c_count = 30000;

	}
	while (TRUE);

	clear_bitmap (screen);
	rest (200);




	clear_bitmap (screen);
	vsync ();
	clear_bitmap (display);

	arena.target_palette = BASECOL_L1_SPRING;
	set_base_palette ();

	newpal = build_new_palette (arena.change_palette, 0, 0, 0);
	vsync ();
	set_palette (palet[newpal]);



}

void init_cflowers (void)
{
	int i;

	for (i = 0; i < NO_CFLOWERS; i++)
	{
		new_cflower (i);

		cflower_y[i] = -100 - (i * 100);
	}

	made_cflower = 20 * 100 * -1;

}

void run_cflowers (void)
{

	clear_to_color (display, COL_OUTLINE);

	int i, j;

	int k = 0;

	int angle, y;

	for (i = 0; i < NO_CFLOWERS; i++)
	{
		if (cflower_y[i] > 550 && made_cflower >= 100)
		{
			new_cflower (i);
			made_cflower = 0;
		}
		cflower_y[i] += 2;
		made_cflower += 2;

		cflower_rot[i][k] += cflower_rot_inc[i][k];
		cflower_rot[i][k] &= 1023;

		angle = cflower_rot[i][k];

		if (i == NO_CFLOWERS - 1)
			continue;

		y = cflower_y[i] + cflower_y_offset[i];

		for (j = 0; j < cflower_petals[i][k]; j++)
		{

			cflower_poly4 (display,
			               cflower_x[i], y,
			               cflower_x[i] + xpart (angle + cflower_angle[i][k],
			                                     cflower_out1[i][k]),
			               y + ypart (angle + cflower_angle[i][k],
			                          cflower_out1[i][k]),
			               cflower_x[i] + xpart (angle, cflower_out2[i][k]),
			               y + ypart (angle, cflower_out2[i][k]),
			               cflower_x[i] + xpart (angle - cflower_angle[i][k],
			                                     cflower_out1[i][k]),
			               y + ypart (angle - cflower_angle[i][k],
			                          cflower_out1[i][k]), cflower_col[i][k]);

			circlefill (display, cflower_x[i], y, cflower_centre_size[i],
			            cflower_centre_col[i]);
			circle (display, cflower_x[i], y, cflower_centre_size[i], COL_OUTLINE);

			angle += ANGLE_1 / cflower_petals[i][k];

		}


	}


}

void new_cflower (int f)
{
	cflower_x[f] = 320 - grand (150) - grand (150) + grand (150) + grand (150);
	cflower_y[f] = -100;
	cflower_y_offset[f] = grand (10);
	cflower_centre_col[f] = COL_COL1 + grand (8);
	if (grand (3) == 0)
		cflower_centre_col[f] = COL_WHITE;
	cflower_centre_size[f] = 6 + grand (5);
	cflower_col[f][0] = COL_COL1 + grand (9);
	cflower_out1[f][0] = 25 + grand (10);	// inner corners of petal
	cflower_out2[f][0] = 36 + grand (10);	// end of petal
	cflower_angle[f][0] = ANGLE_32 + grand (ANGLE_32);	// angle of inner corners
	cflower_petals[f][0] = 3 + grand (2) + grand (2) + grand (2) + grand (2);
	cflower_rot_inc[f][0] = pos_or_neg (3 + grand (8));
	cflower_rot[f][0] = grand (ANGLE_1);

}




void cflower_poly4 (BITMAP * target, int x1, int y1, int x2, int y2, int x3,
                    int y3, int x4, int y4, int col)
{

	int points[8];
	points[0] = x1;
	points[1] = y1;
	points[2] = x2;
	points[3] = y2;
	points[4] = x3;
	points[5] = y3;
	points[6] = x4;
	points[7] = y4;

	polygon (target, 4, points, col);

}


/*
void loading_screen(void)
{

 splash_bitmap = load_bitmap("gfx//splash.bmp", splash_palette);

 if (splash_bitmap == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Failed to load in bitmap! (File not found?)
%s", "splash.bmp");
  exit(1);
 }

 BITMAP *wait_bitmap = load_bitmap("gfx//wait.bmp", splash_palette);

 if (wait_bitmap == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Failed to load in bitmap! (File not found?)
%s", "wait.bmp");
  exit(1);
 }


 set_palette(splash_palette);

 vsync();
 blit(splash_bitmap, screen, 0, 0, 0, 0, 640, 480);
// blit(wait_bitmap, screen, 0, 0, 0, 0, 440, 300);
 draw_sprite(screen, wait_bitmap, 440, 300);

    destroy_bitmap(wait_bitmap);


}*/

/*
void loading_screen_wait(void)
{

 BITMAP *press_bitmap = load_bitmap("gfx//press.bmp", splash_palette);

 if (press_bitmap == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Failed to load in bitmap! (File not found?)
%s", "press.bmp");
  exit(1);
 }

 vsync();
 blit(splash_bitmap, screen, 0, 0, 0, 0, 640, 480);
 draw_sprite(screen, press_bitmap, 390, 320);

    do
    {
     if (key [KEY_ESC])
      exit(0);
    } while (key [KEY_SPACE] == 0);

    destroy_bitmap(splash_bitmap);
    destroy_bitmap(press_bitmap);

    vsync();
    clear_bitmap(screen);
    init_palette();

}
*/
