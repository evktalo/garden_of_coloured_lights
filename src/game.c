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

#include "stuff.h"

#include "input.h"
#include "ebullet.h"
#include "pbullet.h"
#include "enemy.h"
#include "level.h"
#include "cloud.h"

#include "palette.h"
#include "display.h"

#include "sound.h"

void begin_game (void);

void run_player (void);
void init_player (void);
void init_player_on_respawn (void);
void new_level (void);
void make_blocks (void);
void run_blocks (void);
void run_blade (int m);
void drag_blade (void);

extern volatile unsigned char ticked;
extern volatile int framecounter;
extern int slacktime;

unsigned char counter;

void run_stage3 (void);
void make_part_row (int row);

void make_platform (void);
void run_platform (void);
void run_background (void);
void fix_platform (int y1, int y2);
void fix_island1 (int y1, int y2);
int check_tile_flow (int t1, int t2);
int get_tile (int x, int y);
void platform_hline (int x1, int y, int x2);
void platform_vline (int x, int y1, int y2);

void make_islands (void);
void run_islands (void);
int get_island1_tile (int x, int y);
void make_stars (void);
void run_stars (void);

void make_bones (void);
void run_bones (void);

void init_stage_3 (void);

char run_pause (void);

extern int platform_tile[9][PLAT_Y];	// in display
extern int platform_position;	// in display
extern int platform_step;	// in display

extern int island1_tile[9][ISLAND1_Y];	// in display
extern int island1_position;	// in display
extern int island1_step;	// in display

// all stars in display
extern int star_x[NO_STARS][3];
extern int star_y[NO_STARS][3];
extern int star_y_speed[NO_STARS][3];
// extern int star_col [NO_STARS] [3];

void new_game (void)
{
  switch (options.tourist)
  {
  default:
  case 0:
    arena.level = 1;
    arena.change_palette = BASECOL_L1_SPRING;
    arena.target_palette = BASECOL_L1_SPRING;
    break;
  case 2:
    arena.level = 2;
    arena.change_palette = BASECOL_L2_1;
    arena.target_palette = BASECOL_L2_1;
    break;
  case 3:
    arena.level = 3;
    arena.change_palette = BASECOL_L3_1;
    arena.target_palette = BASECOL_L3_1;
    break;
  case 4:
    arena.level = 4;
    arena.change_palette = BASECOL_L4_1;
    arena.target_palette = BASECOL_L4_1;
    break;


  }




  arena.game_over = 0;

  init_player ();

  init_beat ();

  // arena.underlay_position = 0; // need to put in 'new level' function

  // arena.change_palette = BASECOL_L1_SPRING;
  arena.shift_palette = 0;
  arena.shift_message = 300;
  arena.phase = 0;
  arena.phase_bonus = 0;
  arena.just_got_highscore = 0;

  // arena.target_palette = BASECOL_L1_SPRING;

  new_level ();

  set_base_palette ();

  return;

  /*
   * arena.level = 1;
   * 
   * arena.game_over = 0;
   * 
   * init_player();
   * 
   * init_beat();
   * 
   * // arena.underlay_position = 0; // need to put in 'new level'
   * function
   * 
   * 
   * 
   * arena.change_palette = 0; arena.shift_palette = 0;
   * arena.shift_message = 300; arena.phase = 0;
   * arena.just_got_highscore = 0; arena.phase_bonus = 0;
   * 
   * arena.target_palette = 0;
   * 
   * new_level();
   * 
   * set_base_palette();
   */

}

void game_loop (void)
{
  int playing = 1;

  counter = 0;

  do
  {

    if (arena.game_over > 0)
    {
      arena.game_over--;
      if (arena.game_over == 1)
      {
	playing = 0;
	break;
      }
    }

    if (arena.level_finished > 0)
    {
      arena.level_finished--;
      if (arena.level_finished == 1)
      {
	if (options.tourist > 0)
	{
	  playing = 0;
	  break;
	}
	if (arena.level == 4)
	{
	  playing = 0;
	  arena.level = 5;
	  break;
	}
	arena.level++;
	new_level ();
	clear_bitmap (screen);
	rest (500);
	ticked = 0;
      }
    }

    if (arena.new_level_sign > 0)
      arena.new_level_sign--;

    if (arena.shift_palette > 0 && arena.counter % 4 == 0)
      run_palette_shift ();

    if (arena.shift_message > 0)
    {
      arena.shift_message--;
      if (((arena.level != 3 && arena.shift_message == 100)
	   || (arena.level == 3 && arena.shift_message == 295))
	  && arena.phase > 0)
      {
	arena.shift_palette = 1;
	switch (arena.level)
	{
	case 1:
	  arena.target_palette = arena.phase;
	  break;
	  // play_effectwfvx(WAV_STAGE1, tone [NOTE_1C + 
	  // 
	  // 
	  // 
	  // 
	  // arena.phase], 200, 320);
	  break;
	case 2:
	  arena.target_palette = BASECOL_L2_1 + arena.phase;
	  break;
	  // play_effectwfvx(WAV_STAGE2, tone [NOTE_1C + 
	  // 
	  // 
	  // 
	  // 
	  // arena.phase], 200, 320);
	  break;
	case 3:
	  arena.target_palette = BASECOL_L3_1 + arena.phase;
	  break;
	  // play_effectwfvx(WAV_STAGE3, tone [NOTE_1C + 
	  // 
	  // 
	  // 
	  // 
	  // arena.phase], 200, 320);
	  // break;
	case 4:
	  arena.target_palette = BASECOL_L4_1 + arena.phase;
	  break;
	  // play_effectwfvx(WAV_STAGE4_2, tone [NOTE_1C 
	  // 
	  // 
	  // 
	  // 
	  // + arena.phase], 200, 320);
	  // break;
	}
	arena.change_palette = 0;
      }
      /*
       * switch(arena.level) { case 2: if (arena.phase > 0 &&
       * arena.shift_message == 150) play_effectwfvx(WAV_STAGE2, 
       * tone [NOTE_2C + arena.phase], 160, 320); break; case 3:
       * if (arena.phase > 0 && arena.shift_message == 250)
       * play_effectwfvx(WAV_STAGE3, tone [NOTE_1C +
       * arena.phase], 250, 500); break; case 4: if (arena.phase
       * > 0 && arena.shift_message == 150)
       * play_effectwfvx(WAV_STAGE4_2, tone [NOTE_2C], 250, 320);
       * break;
       * 
       * }
       */
    }

    run_beat ();

    if (player.in_play == 1)
      get_input ();
    run_clouds ();
    if (player.in_play == 1)
      run_player ();
    run_level ();
    run_enemies ();
    run_ebullets ();
    run_pbullets ();
    run_background ();
    // run_underlay();


    if (ticked == 0)
    {
      run_display (1);
      framecounter++;
    }
    else
      run_display (0);		// just does what is needed for collision
    // detection

    slacktime = 0;

    do
    {
      // slacktime ++;
    }
    while (ticked == 0);
    ticked--;

    counter++;
    arena.counter++;

    if (key[KEY_ESC])
    {
      if (run_pause () == 1)
	playing = 0;
    }

  }
  while (playing == 1);


  if (player.score > options.highscore[arena.difficulty])
  {
    arena.just_got_highscore = arena.difficulty + 1;
    options.highscore[arena.difficulty] = player.score;
    switch (arena.difficulty)
    {
    case 0:
      set_config_int ("Scores", "Easy", player.score);
      break;
    case 1:
      set_config_int ("Scores", "Normal", player.score);
      break;
    case 2:
      set_config_int ("Scores", "Hard", player.score);
      break;

    }
  }

}

char run_pause (void)
{
  int pc = 0;

  display_pause (pc);

  do
  {
    pc++;
    if (pc > 256)
      pc = 0;


    do
    {
    }
    while (ticked == 0);
    ticked--;

    if (key[KEY_Y])
    {
      clear_bitmap (screen);
      vsync ();
      return 1;
    }

    if (key[KEY_N])
      return 0;
  }
  while (TRUE);

  return 0;

}

void run_player (void)
{
  int i, j, orb_angle, cycle;

  if (player.grace > 0)
    player.grace--;

  char firing_fast = 0;

  char cycling =
    (player.recycle[0] > 0) + (player.recycle[1] >
			       0) + (player.recycle[2] > 0);

  if (player.blade_index != -1
      && (player.swing_state != BLADE_LEFT
	  && player.swing_state != BLADE_RIGHT))
  {
    cycling++;
    firing_fast++;
  }
  if (player.beam[0] != -1 || player.beam_flicker[0] > 0)
  {
    cycling++;
    firing_fast++;
  }
  if (player.beam[1] != -1 || player.beam_flicker[1] > 0)
  {
    cycling++;
    firing_fast++;
  }
  if (player.beam[2] != -1 || player.beam_flicker[2] > 0)
  {
    cycling++;
    firing_fast++;
  }

  if (player.wtype[0] == FAST_WEAPON && player.recycle[0] > 0)
    firing_fast++;
  if (player.wtype[1] == FAST_WEAPON && player.recycle[1] > 0)
    firing_fast++;
  if (player.wtype[2] == FAST_WEAPON && player.recycle[2] > 0)
    firing_fast++;

  if (firing_fast > 0)
  {
    if ((player.wtype[0] == SLOW_WEAPON && player.recycle[0] > 0))
      cycling--;
    if ((player.wtype[1] == SLOW_WEAPON && player.recycle[1] > 0))
      cycling--;
    if ((player.wtype[2] == SLOW_WEAPON && player.recycle[2] > 0))
      cycling--;
  }



  for (i = 0; i < 3; i++)
  {
    // if (player.wtype [i] == SLOW_WEAPON && firing_fast == 1)
    // continue;
    if (player.recycle[i] > 0
	&& (player.wtype[i] != SLOW_WEAPON || firing_fast == 0))
    {
      cycle = player.recycle_speed[i];
      if (cycling > 0)		// probably always true, but div
	// by zero sucks
	cycle /= cycling;
      player.recycle[i] -= cycle;
      if (player.recycle <= 0)
      {
	player.recycle[i] = 0;
	player.wfired = -1;
      }
    }
    player.wstrength[i] = cycling;
    if (player.wstrength[i] == 0)
      player.wstrength[i] = 1;
    if (player.wtype[i] == SLOW_WEAPON)
    {
      player.rpixels[i] = 50 - player.recycle[i] / 20;
      if (player.rpixels[i] > 50)
	player.rpixels[i] = 50;
    }
    else
    {
      if (firing_fast <= 1)
      {
	player.rpixels[i] = 50;
	player.wstrength[i] = 1;
      }
      if (firing_fast == 2)
      {
	player.rpixels[i] = 25;
	player.wstrength[i] = 2;
      }
      if (firing_fast == 3)
      {
	player.rpixels[i] = 17;
	player.wstrength[i] = 3;
      }
    }
  }

  /*
   * if (player.recycle > 0) { cycle = player.recycle -=
   * player.recycle_speed / cycling; if (player.recycle <= 0) {
   * player.recycle = 0; player.wfired = -1; } }
   */


  /*
   * if (player.wfired != -1) { player.wframe += player.wframe_speed; if 
   * (player.wframe_speed > 0 && player.wframe >= player.wframe_max) {
   * player.wframe = player.wframe_max; switch }
   * 
   * }
   */

  if (player.orbitals > 0)
  {
    // if (player.firing == 0)
    {
      player.orbital_angle += 8;
      player.orbital_angle &= 1023;
    }
    // else
    // player.firing = 0;
    orb_angle = player.orbital_angle;

    // player.wx [0] = player.x + xpart(player.orbital_angle,
    // 12000);
    // player.wy [0] = player.y + ypart(player.orbital_angle,
    // 12000);

    if (player.orbitals == 2)
      j = ANGLE_2;
    if (player.orbitals == 3)
      j = ANGLE_3;

    for (i = 0; i < 3; i++)
    {
      if (player.wgained[i] > 0)
	player.wgained[i]--;
      if (player.wstyle[i] == WSTYLE_ORBITAL)
      {
	player.wx[i] = player.x + xpart (orb_angle, 30000);
	player.wy[i] = player.y + ypart (orb_angle, 30000);
	orb_angle += j;
      }
      if (player.wstyle[i] == WSTYLE_BLADE)
      {
	run_blade (i);
      }
      if (player.wstyle[i] == WSTYLE_FRONT)
      {
	player.wx[i] = player.x;
	player.wy[i] = player.y - 10000;
      }
      if (player.weapon[i] == WPN_LWBEAM)
      {
	if (player.beam_fade[i] > 0)
	{
	  if (player.beam_fade[i] < 10 && player.beam_open[i] > 0)
	    player.beam_open[i]--;
	  player.beam_fade[i]--;
	  if (player.beam_flicker[i] > 0)
	    player.beam_flicker[i]--;
	}
      }
      if (player.weapon[i] == WPN_SWBEAM)
      {
	if (player.swbeam[i] > 0)
	{
	  player.swbeam[i]--;
	}
	if (player.swbeam_counter[i] > 0)
	{
	  player.swbeam_counter[i]--;
	  quick_cloud (CLOUD_CIRCLE5,
		       player.wx[i] + grand (4000) -
		       grand (4000),
		       player.wy[i] - 5000 + grand (4000) -
		       grand (4000),
		       grand (1000) - grand (1000),
		       grand (1000) - grand (1000),
		       grand (20) +
		       player.swbeam_counter[i] / 2, TCOL_ORANGE, 960);
	}
      }
    }

  }

  if (player.respawning == 0)
  {
    for (i = 0; i < 3; i++)
    {
      if (player.wx[i] > 640000)
	player.wx[i] = 640000;
      if (player.wy[i] > 480000)
	player.wy[i] = 480000;
      if (player.wx[i] < 0)
	player.wx[i] = 0;
      if (player.wy[i] < 0)
	player.wy[i] = 0;
    }
  }


}

#define BLADE_DISTANCE 45000
#define BLADE_SPEED 48

void run_blade (int m)
{
  /*
   * player.wx [m] = player.x + xpart(orb_angle, 30000); player.wy [m] = 
   * player.y + ypart(orb_angle, 30000); orb_angle += j; //
   * player.swing_angle = radians_to_angle(atan2(player.y - player.wy
   * [i], player.x - player.wx [i])); 
   */


  int goal_angle;
  // int target_x, target_y;
  // char end_swing = 0;
  int old_x = player.swing_x[player.swing_pos];
  int old_y = player.swing_y[player.swing_pos];

  switch (player.swing_state)
  {
  case BLADE_LEFT:
    player.swing_angle = ANGLE_2 - ANGLE_16 - 8;
    player.wx[m] = player.x + xpart (player.swing_angle, BLADE_DISTANCE);
    player.wy[m] = player.y + ypart (player.swing_angle, BLADE_DISTANCE);
    // player.swing_speed_x = 0;
    // player.swing_speed_y = 0;
    break;
  case BLADE_RIGHT:
    goal_angle = ANGLE_16;
    player.wx[m] = player.x + xpart (player.swing_angle, BLADE_DISTANCE);
    player.wy[m] = player.y + ypart (player.swing_angle, BLADE_DISTANCE);
    // player.swing_speed_x = 0;
    // player.swing_speed_y = 0;
    break;

  case BLADE_CW_SLASH:
    // player.recycle = 1;
    player.swing_length += 2;
    if (player.swing_length >= 9)
      player.swing_length = 9;
    player.swing_angle += BLADE_SPEED;
    player.swing_angle &= 1023;
    if (player.swing_angle >= ANGLE_16 && player.swing_angle < ANGLE_4)
    {
      if (player.swing_hold == 1)
      {
	player.swing_state = BLADE_CW_SWING;
      }
      else
      {
	player.swing_state = BLADE_RIGHT;
	// player.recycle = 0;
      }
    }
    player.wx[m] = player.x + xpart (player.swing_angle, BLADE_DISTANCE);
    player.wy[m] = player.y + ypart (player.swing_angle, BLADE_DISTANCE);
    break;

  case BLADE_ACW_SLASH:
    // player.recycle = 1;
    player.swing_length += 2;
    if (player.swing_length >= 9)
      player.swing_length = 9;
    player.swing_angle -= BLADE_SPEED;
    player.swing_angle &= 1023;
    if (player.swing_angle <= ANGLE_2 - ANGLE_16 + 48
	&& player.swing_angle > ANGLE_4)
    {
      if (player.swing_hold == 1)
      {
	player.swing_state = BLADE_ACW_SWING;
      }
      else
      {
	player.swing_state = BLADE_LEFT;
	// player.recycle = 0;
      }
    }
    player.wx[m] = player.x + xpart (player.swing_angle, BLADE_DISTANCE);
    player.wy[m] = player.y + ypart (player.swing_angle, BLADE_DISTANCE);
    break;

  case BLADE_CW_SWING:
    // player.recycle = 1;
    player.swing_length += 2;
    if (player.swing_length >= 9)
      player.swing_length = 9;
    player.swing_angle += BLADE_SPEED;
    player.swing_angle &= 1023;
    if (player.swing_angle >= ANGLE_2 - ANGLE_16 - 48
	&& player.swing_angle < ANGLE_1 - ANGLE_4)
    {
      player.swing_state = BLADE_LEFT;
      // player.recycle = 0;
    }
    player.wx[m] = player.x + xpart (player.swing_angle, BLADE_DISTANCE);
    player.wy[m] = player.y + ypart (player.swing_angle, BLADE_DISTANCE);
    break;

  case BLADE_ACW_SWING:
    // player.recycle = 1;
    player.swing_length += 2;
    if (player.swing_length >= 9)
      player.swing_length = 9;
    player.swing_angle -= BLADE_SPEED;
    player.swing_angle &= 1023;
    // if (player.swing_angle <= ANGLE_2 - ANGLE_16 &&
    // player.swing_angle > ANGLE_4)
    if (player.swing_angle <= ANGLE_16 && player.swing_angle > 0)
    {
      player.swing_state = BLADE_RIGHT;
      // player.recycle = 0;
    }
    player.wx[m] = player.x + xpart (player.swing_angle, BLADE_DISTANCE);
    player.wy[m] = player.y + ypart (player.swing_angle, BLADE_DISTANCE);
    break;



    /*
     * case BLADE_LEFT: player.wx [m] = player.x - 20000; player.wy
     * [m] = player.y - 10000; player.swing_speed_x = 0;
     * player.swing_speed_y = 0; break; case BLADE_RIGHT: player.wx
     * [m] = player.x + 20000; player.wy [m] = player.y - 10000;
     * player.swing_speed_x = 0; player.swing_speed_y = 0; break;
     */


    /*
     * case BLADE_CW_SLASH: target_x = player.x + 20000; target_y =
     * player.y - 10000; goal_angle =
     * radians_to_angle(atan2(target_y - player.wy [m], target_x -
     * player.wx [m])); player.swing_speed_x += xpart(goal_angle,
     * 3000); player.swing_speed_y += ypart(goal_angle, 3000);
     * 
     * goal_angle = radians_to_angle(atan2(player.x - player.wy [m], 
     * player.y - player.wx [m])); player.swing_speed_x +=
     * xpart(goal_angle, 1800); player.swing_speed_y +=
     * ypart(goal_angle, 1800);
     * 
     * if (player.wx [m] > target_x - 5000 && player.wx [m] <
     * target_x + 5000 && player.wy [m] > target_y - 5000 &&
     * player.wy [m] < target_y + 5000) end_swing = 1;
     * 
     * if (player.wx [m] > target_x - 15000 && player.wx [m] <
     * target_x + 15000 && player.wy [m] > target_y - 15000 &&
     * player.wy [m] < target_y + 15000) { drag_blade(); }
     * 
     * drag_blade(); if (end_swing == 1) { player.swing_state =
     * BLADE_RIGHT; player.recycle = 0; } break; case
     * BLADE_ACW_SLASH: target_x = player.x - 20000; target_y =
     * player.y - 10000; goal_angle =
     * radians_to_angle(atan2(target_y - player.wy [m], target_x -
     * player.wx [m])); player.swing_speed_x += xpart(goal_angle,
     * 3000); player.swing_speed_y += ypart(goal_angle, 3000);
     * 
     * goal_angle = radians_to_angle(atan2(player.x - player.wy [m], 
     * player.y - player.wx [m])); player.swing_speed_x +=
     * xpart(goal_angle, 1800); player.swing_speed_y +=
     * ypart(goal_angle, 1800);
     * 
     * if (player.wx [m] > target_x - 5000 && player.wx [m] <
     * target_x + 5000 && player.wy [m] > target_y - 5000 &&
     * player.wy [m] < target_y + 5000) end_swing = 1;
     * 
     * if (player.wx [m] > target_x - 15000 && player.wx [m] <
     * target_x + 15000 && player.wy [m] > target_y - 15000 &&
     * player.wy [m] < target_y + 15000) { drag_blade(); }
     * 
     * drag_blade(); if (end_swing == 1) { player.swing_state =
     * BLADE_LEFT; player.recycle = 0; } break; 
     */
  }

  player.swing_sprite_angle =
    radians_to_angle (atan2
		      (player.wy[m] - player.y, player.wx[m] - player.x));
  player.swing_sprite_angle /= (ANGLE_1 / SMALL_ROTATIONS);


  /*
   * if (player.swing_state > BLADE_RIGHT) { player.wx [m] +=
   * player.swing_speed_x; player.wy [m] += player.swing_speed_y;
   * player.recycle = 1; }
   */
  if (player.swing_length > 0)
    player.swing_length--;
  player.swing_pos++;
  if (player.swing_pos == 10)
    player.swing_pos = 0;

  // player.swing_x [player.swing_pos] = player.wx [m] +
  // xpart(player.swing_angle, 1000);
  // player.swing_y [player.swing_pos] = player.wy [m] +
  // ypart(player.swing_angle, 1000);
  // player.swing_x2 [player.swing_pos] = player.wx [m] +
  // xpart(player.swing_angle, 25000);
  // player.swing_y2 [player.swing_pos] = player.wy [m] +
  // ypart(player.swing_angle, 25000);
  player.swing_x[player.swing_pos] =
    player.wx[m] + xpart (player.swing_angle, 12000);
  player.swing_y[player.swing_pos] =
    player.wy[m] + ypart (player.swing_angle, 12000);
  player.swing_x2[player.swing_pos] =
    player.wx[m] + xpart (player.swing_angle, 25000);
  player.swing_y2[player.swing_pos] =
    player.wy[m] + ypart (player.swing_angle, 25000);

  player.swing_strength[player.swing_pos] =
    player.wstrength[player.blade_index];

  if (player.swing_state == BLADE_LEFT || player.swing_state == BLADE_RIGHT)
    return;

  int b = create_pbullet (PBULLET_BLADE);
  if (b != -1)
  {
    pbullet[b].x = player.swing_x[player.swing_pos];
    pbullet[b].y = player.swing_y[player.swing_pos];
    pbullet[b].x_speed = player.swing_x[player.swing_pos] - old_x;
    pbullet[b].y_speed = player.swing_y[player.swing_pos] - old_y;
    pbullet[b].xsize = 13000;
    pbullet[b].ysize = 13000;
    pbullet[b].dam = 80 + player.wlevel[player.blade_index] * 19;	// was 
    // 
    // 
    // 
    // 
    // 100/22 
    // or 
    // 60/14
    pbullet[b].dam /= player.wstrength[player.blade_index];
    pbullet[b].source = player.blade_index;
  }
  if (player.wlevel[player.blade_index] > 6)
  {
    int bangle =
      radians_to_angle (atan2 (old_y - player.swing_y[player.swing_pos],
			       old_x - player.swing_x[player.swing_pos]));
    bangle -= ANGLE_4;
    quick_cloud (CLOUD_CIRCLE2, old_x + xpart (bangle, 11000),
		 old_y + ypart (bangle, 11000), 0, 0,
		 player.wlevel[player.blade_index] - 2, TCOL_ORANGE, 0);
    bangle += ANGLE_2;
    quick_cloud (CLOUD_CIRCLE2, old_x + xpart (bangle, 11000),
		 old_y + ypart (bangle, 11000), 0, 0,
		 player.wlevel[player.blade_index] - 2, TCOL_ORANGE, 0);
  }
  /*
   * quick_cloud(CLOUD_CIRCLE2, player.swing_x [player.swing_pos] +
   * xpart(bangle, 16000), player.swing_y [player.swing_pos] +
   * ypart(bangle, 16000), 0, 0, 10, TCOL_ORANGE, 0); bangle += ANGLE_2;
   * quick_cloud(CLOUD_CIRCLE2, player.swing_x [player.swing_pos] +
   * xpart(bangle, 16000), player.swing_y [player.swing_pos] +
   * ypart(bangle, 16000), 0, 0, 10, TCOL_ORANGE, 0);
   */

  /*
   * int e;
   * 
   * if (player.swing_state != BLADE_LEFT && player.swing_state !=
   * BLADE_RIGHT) { for (e = 0; e < NO_ENEMIES; e ++) {
   * 
   * 
   * } } 
   */

  // player.swing_pos2 ++;



}

/*
 * void drag_blade(void) { player.swing_speed_x *= 800;
 * player.swing_speed_x /= 1000; player.swing_speed_y *= 800;
 * player.swing_speed_y /= 1000;
 * 
 * }
 */

void player_hit (void)
{

  int x = player.x;
  int y = player.y;


  play_effectwfvx (WAV_BANG, tone[NOTE_0G], 250, x);
  play_effectwfvx (WAV_P_BANG, 800, 250, x);

  int i, angle, speed;

  quick_cloud (CLOUD_MSHOCK, x, y, 0, 0, 20, 0, 0);
  quick_cloud (CLOUD_CIRCLE2, x, y, 0, 0, 30, TCOL_ORANGE, 0);
  quick_cloud (CLOUD_CIRCLE3, x, y, 0, 0, 25, TCOL_YELLOW, 0);

  for (i = 0; i < 30; i++)
  {
    angle = grand (ANGLE_1);
    speed = 5000 + grand (10000);
    quick_cloud (CLOUD_CIRCLE2, x, y, xpart (angle, speed),
		 ypart (angle, speed), 20 + grand (20), TCOL_ORANGE, 910);
    angle = grand (ANGLE_1);
    speed = 5000 + grand (10000);
    quick_cloud (CLOUD_CIRCLE3, x, y, xpart (angle, speed),
		 ypart (angle, speed), 20 + grand (20), TCOL_YELLOW, 870);
  }

  for (i = 0; i < 3; i++)
  {
    x = player.wx[i];
    y = player.wy[i];
    quick_cloud (CLOUD_TSHOCK, x, y, 0, 0, 11, 0, 0);
    quick_cloud (CLOUD_CIRCLE2, x, y, 0, 0, 25, TCOL_ORANGE, 0);
    quick_cloud (CLOUD_CIRCLE3, x, y, 0, 0, 17, TCOL_YELLOW, 0);
  }

  if (player.lives == 0)
  {
    player.in_play = 0;
    arena.game_over = 200;
    arena.level_finished = 0;
  }
  else
  {
    player.lives--;
    if (player.lives > -1)
    {
      init_player_on_respawn ();
    }
  }

}


void init_player (void)
{

  int i;

  init_player_on_respawn ();

  player.lives = 2;
  // player.respawning = 0;
  player.score = 0;
  player.grace = 0;
  player.in_play = 1;

  // player.x = 270000;
  // player.y = 350000;
  player.wings1 = 0;
  player.wings2 = 0;

  for (i = 0; i < 3; i++)
  {
    // player.x [i] = player.x [0];
    // player.y [i] = player.y [0];
    // player.weapon [i] = MULTI_BASIC;
    player.wxp[i] = 0;
    if (options.tourist == 0)
      player.wlevel[i] = 0;
    else
      player.wlevel[i] = options.tourist;	// (options.tourist 
    // 
    // 
    // 
    // 
    // - 1) * 2 - 1;
    player.wstatus[i] = 0;
    player.wpixels[i] = 0;
    player.wgained[i] = 0;
    player.recycle[i] = 0;
    player.recycle_speed[i] = 0;
    player.wstrength[i] = 0;
    player.rpixels[i] = 50;

    player.beam[i] = -1;
    player.beam_counter[i] = 0;
    player.beam_fade[i] = 0;
    player.beam_open[i] = 0;
    player.beam_flicker[i] = 0;

  }
  set_wpixels ();



}

void init_player_on_respawn (void)
{

  player.respawning = 50;
  player.grace = 180;

  player.x = 320000;
  player.y = 520000;

  player.firing = 0;
  player.swing_state = BLADE_LEFT;
  player.swing_pos = 0;
  player.swing_length = 0;

  int i;


  for (i = 0; i < 3; i++)
  {
    player.recycle[i] = 0;
    player.recycle_speed[i] = 0;
    player.wstrength[i] = 0;
    player.rpixels[i] = 0;

    player.beam[i] = -1;
    player.beam_counter[i] = 0;
    player.beam_fade[i] = 0;
    player.beam_open[i] = 0;
    player.beam_flicker[i] = 0;

    player.swbeam[i] = 0;
    player.swbeam_counter[i] = 0;

  }

  player.wfired = -1;
  player.wframe = 0;
  player.wframe_speed = 0;
  player.wframe_max = 0;


  player.ring_angle = -ANGLE_4;
  player.ring_hold = 0;
  player.move_x = 0;
  player.move_y = 0;

}


void new_level (void)
{


  arena.shift_palette = 0;
  arena.shift_message = 300;
  arena.phase = 0;
  arena.just_got_highscore = 0;
  arena.stage_score = 0;
  arena.max_stage_score = get_max_stage_score ();
  // arena.phase_bonus = 0;

  switch (arena.level)
  {
  case 1:
    arena.next_phase[0] = 1400;
    arena.next_phase[1] = 2500;
    arena.next_phase[2] = 3500;
    arena.next_phase[3] = 4050;
    arena.next_phase[4] = -1;
    break;
  case 2:
    arena.next_phase[0] = 1600;
    arena.next_phase[1] = 2400;
    arena.next_phase[2] = 3200;
    arena.next_phase[3] = 3900;
    arena.next_phase[4] = -1;
    break;
  case 3:
    arena.next_phase[0] = 1000;
    arena.next_phase[1] = 2100;
    arena.next_phase[2] = 3200;
    arena.next_phase[3] = 4400;
    arena.next_phase[4] = -1;
    /*
     * arena.next_phase [0] = 1000; arena.next_phase [1] = 1900;
     * arena.next_phase [2] = 2800; arena.next_phase [3] = 3600;
     * arena.next_phase [4] = 4500; arena.next_phase [5] = -1;
     */
    break;
  case 4:
    arena.next_phase[0] = 800;
    arena.next_phase[1] = 1600;
    arena.next_phase[2] = 2500;
    arena.next_phase[3] = 3400;
    // arena.next_phase [4] = 3600;
    arena.next_phase[4] = -1;
    break;
  }


  switch (arena.level)
  {
  case 1:
    make_platform ();
    make_blocks ();
    arena.change_palette = BASECOL_L1_SPRING;
    arena.target_palette = BASECOL_L1_SPRING;
    break;
  case 2:
    make_islands ();
    make_stars ();
    arena.cruiser_escorts = 0;
    arena.change_palette = BASECOL_L2_1;
    arena.target_palette = BASECOL_L2_1;
    break;
  case 3:
    init_stage_3 ();
    arena.change_palette = BASECOL_L3_1;
    arena.target_palette = BASECOL_L3_1;
    break;
  case 4:
    make_bones ();
    arena.change_palette = BASECOL_L4_1;
    arena.target_palette = BASECOL_L4_1;
    break;
  }

  set_base_palette ();
  // arena.target_palette = 0;
  // set_base_palette();



  init_level ();
  init_beat_new_level ();

  ticked = 0;

  boss.fight = 0;

  arena.new_level_sign = 120;

}

void make_bones (void)
{

  int i;

  for (i = 0; i < NO_BONES; i++)
  {
    arena.bone_type[0][i] = 0;
    arena.bone_subtype[0][i] = 0;
    if (grand (4) == 0)
      arena.bone_subtype[0][i] = grand (9);
    arena.bone_x[0][i] = 20 + grand (620);
    // arena.bone_y [0] [i] = -30 - grand(500);
    arena.bone_y[0][i] = grand (480);

    if (grand (4) == 0)
    {
      arena.bone_type[0][i] = 1;
      arena.bone_subtype[0][i] = 0;	// grand(7);
    }
    // arena.bone_type [0] [i] = 3;
    // arena.bone_subtype [0] [i] = grand(9);

    if (i > 19)
      continue;

    arena.bone_type[1][i] = 0;
    arena.bone_subtype[1][i] = 0;
    if (grand (4) == 0)
      arena.bone_subtype[1][i] = grand (9);
    arena.bone_x[1][i] = grand (640);
    // arena.bone_y [1] [i] = -30 - grand(500);
    arena.bone_y[1][i] = grand (480);

    if (grand (4) == 0)
    {
      arena.bone_type[1][i] = 1;
      arena.bone_subtype[1][i] = 0;
    }


  }

}

void run_bones (void)
{
  int i;

  for (i = 0; i < NO_BONES; i++)
  {
    if (arena.counter % 3 == 0)
      arena.bone_y[0][i] += 1;
    if (arena.bone_y[0][i] > 560)
    {
      arena.bone_y[0][i] = -30 - grand (100);
      arena.bone_x[0][i] = grand (640);
      arena.bone_type[0][i] = 0;
      arena.bone_subtype[0][i] = 0;
      if (grand (4) == 0)
	arena.bone_subtype[0][i] = grand (9);

      if (grand (4) == 0)
      {
	arena.bone_type[0][i] = 1;
	arena.bone_subtype[0][i] = 0;
	if (grand (4) == 0)
	  arena.bone_subtype[0][i] = grand (7);

      }


      if (grand (16) == 0 && arena.phase > 0)
      {
	arena.bone_type[0][i] = 2;
	arena.bone_subtype[0][i] = 0;
	if (grand (4) == 0)
	  arena.bone_subtype[0][i] = grand (17);
      }

      if (grand (34) == 0 && arena.phase > 1)
      {
	arena.bone_type[0][i] = 3;
	arena.bone_subtype[0][i] = 0;
	if (grand (4) == 0)
	  arena.bone_subtype[0][i] = grand (9);
      }

      if (grand (68) == 0 && arena.phase > 2)
      {
	arena.bone_type[0][i] = 4;
	arena.bone_subtype[0][i] = 0;
	arena.bone_y[0][i] = -90;
      }

      if (grand (78) == 0 && arena.phase > 2)
      {
	arena.bone_type[0][i] = 5;
	arena.bone_subtype[0][i] = 0;
	if (grand (3) == 0)
	  arena.bone_subtype[0][i] = grand (3);
	arena.bone_y[0][i] = -90;
      }

    }

    if (i > 19)
      continue;

    arena.bone_y[1][i] += 1;
    if (arena.bone_y[1][i] > 500)
    {
      arena.bone_y[1][i] = -30 - grand (100);
      arena.bone_x[1][i] = grand (640);
      arena.bone_type[1][i] = 0;
      arena.bone_subtype[1][i] = 0;
      if (grand (4) == 0)
	arena.bone_subtype[1][i] = grand (9);

      if (grand (4) == 0)
      {
	arena.bone_type[1][i] = 1;
	arena.bone_subtype[1][i] = 0;
	if (grand (4) == 0)
	  arena.bone_subtype[1][i] = grand (7);

      }

      if (grand (16) == 0 && arena.phase > 0)
      {
	arena.bone_type[1][i] = 2;
	arena.bone_subtype[1][i] = 0;
	if (grand (4) == 0)
	  arena.bone_subtype[1][i] = grand (17);
      }

      if (grand (34) == 0 && arena.phase > 1)
      {
	arena.bone_type[1][i] = 3;
	arena.bone_subtype[1][i] = 0;
	if (grand (4) == 0)
	  arena.bone_subtype[1][i] = grand (9);
      }

    }


  }

}


void init_stage_3 (void)
{
  arena.part_flow = 0;
  arena.part_rank = 0;

  int j;

  for (j = 0; j < 14; j++)
  {
    /*
     * arena.part_x1 [i] [j] = 5 + grand(10); arena.part_y1 [i] [j]
     * = 5 + grand(10); arena.part_x2 [i] [j] = 5 + grand(10);
     * arena.part_y2 [i] [j] = 5 + grand(10); }
     */
    make_part_row (j);
  }
  arena.wave_y = -10000;
}


void make_part_row (int row)
{

  int i;
  int sx, sy, cx, cy;

  int base_size, size_rand, pos_rand;

  size_rand = arena.phase * 6;
  pos_rand = arena.phase * 6;
  base_size = 20 - arena.phase * 3;

  for (i = 0; i < 18; i++)
  {
    sx = base_size + grand (size_rand);
    sy = base_size + grand (size_rand);
    cx = grand (pos_rand) - grand (pos_rand);
    cy = grand (pos_rand) - grand (pos_rand);
    if (cx < 0)
    {
      arena.part_x1[i][row] = cx;
      arena.part_x2[i][row] = cx + sx;
      arena.part_cx[i][row] = cx + sx / 2;
    }
    else
    {
      arena.part_x1[i][row] = cx - sx;
      arena.part_x2[i][row] = cx;
      arena.part_cx[i][row] = cx - sx / 2;
    }
    if (cy < 0)
    {
      arena.part_y1[i][row] = cy;
      arena.part_y2[i][row] = cy + sy;
      arena.part_cy[i][row] = cy + sy / 2;
    }
    else
    {
      arena.part_y1[i][row] = cy - sy;
      arena.part_y2[i][row] = cy;
      arena.part_cy[i][row] = cy - sy / 2;
    }
  }
}

void run_stage3 (void)
{

  int new_part_row = 0;

  arena.part_flow++;
  arena.part_flow += arena.phase;
  /*
   * if (arena.phase > 0) arena.part_flow ++; if (arena.phase > 3)
   * arena.part_flow ++;
   */

  if (arena.part_flow >= 40)
  {
    arena.part_flow -= 40;
    arena.part_rank++;
    if (arena.part_rank > 13)
      arena.part_rank = 0;
    new_part_row = arena.part_rank;
    // if (new_part_row > 13)
    // new_part_row -= 13;
    make_part_row (new_part_row);
  }

  arena.wave_y += 4000 + arena.phase * 2500;
  if (arena.wave_y > 550000)
    arena.wave_y = -50000;

  if (arena.phase > 0)
  {
    int i, x, y;
    for (i = 0; i < 16 + arena.phase * 8; i++)
    {
      x = grand (18);
      y = grand (14);
      arena.part_x1[x][y] += pos_or_neg (1);
      arena.part_y1[x][y] += pos_or_neg (1);
      arena.part_x2[x][y] += pos_or_neg (1);
      arena.part_y2[x][y] += pos_or_neg (1);
      if (arena.part_x1[x][y] > arena.part_x2[x][y] - 2)
      {
	arena.part_x1[x][y]--;
	arena.part_x2[x][y]++;
      }
      if (arena.part_y1[x][y] > arena.part_y2[x][y] - 2)
      {
	arena.part_y1[x][y]--;
	arena.part_y2[x][y]++;
      }
    }

  }

  if (arena.part_pulse1 > 0)
    arena.part_pulse1--;

  if (arena.part_pulse2 > 0)
    arena.part_pulse2--;

  if (synch_ready[EVENT_PULSE1] == 1)
    arena.part_pulse1 = 8;

  if (synch_ready[EVENT_PULSE2] == 1)
    arena.part_pulse2 = 8;



}


void make_blocks (void)
{

  int i;

  for (i = 0; i < 20; i++)
  {
    arena.block1_x[i] = grand (490) + 50;
    arena.block1_y[i] = 2400 * i;	// grand(480) * 100;
    arena.block1_size[i] = 10 + grand (40);
    arena.block1_y_speed[i] = 180 + grand (20) + grand (20);

    arena.block2_x[i] = grand (490) + 50;
    arena.block2_y[i] = 2400 * i;	// grand(480) * 100;
    arena.block2_y_speed[i] = 90 + grand (10) + grand (10);
    arena.block2_size[i] = 7 + grand (28);
  }

}

void run_blocks (void)
{
  int i;

  for (i = 0; i < 20; i++)
  {
    arena.block1_y[i] += arena.block1_y_speed[i];
    arena.block2_y[i] += arena.block2_y_speed[i];

    if (arena.block1_y[i] > 48000 + (arena.block1_size[i] * 100))
    {
      arena.block1_y[i] = -10000 - grand (10000);
      arena.block1_x[i] = grand (490) + 50;
      arena.block1_size[i] = 10 + grand (40);
      arena.block1_y_speed[i] = 180 + grand (20) + grand (20);
    }
    if (arena.block2_y[i] > 48000 + (arena.block2_size[i] * 100))
    {
      arena.block2_y[i] = -10000 - grand (10000);
      arena.block2_y_speed[i] = 90 + grand (10) + grand (10);
      arena.block2_x[i] = grand (490) + 50;
      arena.block2_size[i] = 10 + grand (40);
    }

  }
}

void make_platform (void)
{

  int i, j;
  platform_position = 0;
  platform_step = 0;

  int vert1 = grand (6) + 2;
  int vert2 = 0;

  int v21 = -1;
  int v22 = -1;


  for (j = 0; j < PLAT_Y; j++)
  {
    for (i = 0; i < 9; i++)
    {
      platform_tile[i][j] = PLATFORM_EMPTY;	// empty
    }
  }



  for (j = 0; j < PLAT_Y; j++)
  {

    if (grand (7) == 0)
    {
      vert2 = vert1;
      vert1 = grand (6) + 2;
      // platform_hline(vert1, j, vert2);
      platform_hline (vert1, j, vert2);
    }

    if (grand (5) == 0)
    {
      if (grand (3) == 0)
	v21 = -1;
      else
      {
	v22 = v21;
	v21 = grand (7) + 2;
	platform_hline (v21, j, v22);
      }
    }

    if (grand (21) == 0)
      platform_hline (0, j, 8);

    if (grand (21) == 0)
      platform_hline (grand (6), j, 8);

    if (grand (21) == 0)
      platform_hline (0, j, grand (4) + 5);

    for (i = 0; i < 9; i++)
    {
      // platform_tile [i] [j] = PLATFORM_EMPTY; // empty

      if (i == v21)
	platform_tile[i][j] = 1;

      if (i == vert1)
	platform_tile[i][j] = 1;

      if (grand (7) == 0)
	platform_tile[i][j] = 1;
    }
  }

  fix_platform (0, PLAT_Y);

}

void platform_hline (int x1, int y, int x2)
{
  int i;

  if (x1 < 0)
    x1 = 0;
  if (x1 > 8)
    x1 = 8;
  if (x2 < 0)
    x2 = 0;
  if (x2 > 8)
    x2 = 8;

  if (x2 > x1)
  {
    for (i = x1; i < x2; i++)
    {
      platform_tile[i][y] = 1;
    }
  }
  else
    for (i = x2; i < x1; i++)
    {
      platform_tile[i][y] = 1;
    }


}

void run_background (void)
{
  switch (arena.level)
  {
  case 1:
    run_platform ();
    run_blocks ();
    break;
  case 2:
    run_islands ();
    run_stars ();
    break;
  case 3:
    run_stage3 ();
    break;
  case 4:
    run_bones ();
    break;

  }

}

void run_islands (void)
{

  island1_position += 7;
  if (island1_position >= 91)
  {
    island1_position -= 91;
    island1_step++;
  }
  if (island1_step > ISLAND1_Y)
    island1_step -= ISLAND1_Y;


}

void run_stars (void)
{
  int i, j;

  for (i = 0; i < NO_STARS; i++)
  {
    for (j = 0; j < 3; j++)
    {
      star_y[i][j] += star_y_speed[i][j];
      if (star_y[i][j] > 481000)
      {
	star_x[i][j] = grand (640);
	star_y[i][j] = 0;
	switch (j)
	{
	case 0:
	  star_y_speed[i][j] = 1000 + grand (1000);
	  break;
	case 1:
	  star_y_speed[i][j] = 2000 + grand (1000);
	  break;
	case 2:
	  star_y_speed[i][j] = 3000 + grand (1000);
	  break;
	}
      }
    }
  }

}

void make_stars (void)
{

  int i, j;

  for (i = 0; i < NO_STARS; i++)
  {
    for (j = 0; j < 3; j++)
    {
      star_x[i][j] = grand (640);
      star_y[i][j] = grand (480000);
      switch (j)
      {
      case 0:
	star_y_speed[i][j] = 1000 + grand (1000);
	break;
      case 1:
	star_y_speed[i][j] = 2000 + grand (1000);
	break;
      case 2:
	star_y_speed[i][j] = 3000 + grand (1000);
	break;
      }
    }
  }

}

void run_platform (void)
{

  platform_position += 4;
  if (platform_position >= 91)
  {
    platform_position -= 91;
    platform_step++;
  }
  if (platform_step > PLAT_Y)
    platform_step -= PLAT_Y;

  arena.flower_turn1 += 10;
  if (arena.flower_turn1 >= 400)
    arena.flower_turn1 -= 400;

  arena.flower_turn2 += 5;
  if (arena.flower_turn2 >= 400)
    arena.flower_turn2 -= 400;

}



void make_islands (void)
{

  int i, j;
  island1_position = 0;
  island1_step = 0;

  /*
   * int vert1 = grand(6) + 2; int vert2 = 0;
   * 
   * int v21 = -1; int v22 = -1;
   */


  for (j = 0; j < ISLAND1_Y; j++)
  {
    for (i = 0; i < 9; i++)
    {
      island1_tile[i][j] = ISLAND1_EMPTY;	// empty
      // if (grand(5) == 0)
      // island1_tile [i] [j] = ISLAND1_C;

    }
  }

  int x, y;

  for (i = 0; i < 200; i++)
  {
    x = grand (7);
    y = grand (ISLAND1_Y - 2);

    island1_tile[x][y] = ISLAND1_FULL;
    island1_tile[x + 1][y] = ISLAND1_FULL;
    island1_tile[x][y + 1] = ISLAND1_FULL;
    island1_tile[x + 1][y + 1] = ISLAND1_FULL;
    island1_tile[x + 2][y] = ISLAND1_FULL;
    island1_tile[x][y + 2] = ISLAND1_FULL;
    island1_tile[x + 2][y + 2] = ISLAND1_FULL;
    island1_tile[x + 2][y + 1] = ISLAND1_FULL;
    island1_tile[x + 1][y + 2] = ISLAND1_FULL;

  }

  fix_island1 (0, ISLAND1_Y);

}

void fix_island1 (int y1, int y2)
{
  int i, j;
  int u, d, l, r, ul, ur, dl, dr;

  for (i = 0; i < 9; i++)
  {
    for (j = y1; j < y2; j++)
    {
      if (island1_tile[i][j] == ISLAND1_EMPTY)
	continue;
      u = get_island1_tile (i, j + 1);
      d = get_island1_tile (i, j - 1);
      l = get_island1_tile (i - 1, j);
      r = get_island1_tile (i + 1, j);

      if (u == ISLAND1_EMPTY && d == ISLAND1_EMPTY)
	island1_tile[i][j] = ISLAND1_EMPTY;

      if (l == ISLAND1_EMPTY && r == ISLAND1_EMPTY)
	island1_tile[i][j] = ISLAND1_EMPTY;

    }
  }

#define ODD_CHANCE 10

  for (i = 0; i < 9; i++)
  {
    for (j = y1; j < y2; j++)
    {
      if (island1_tile[i][j] == ISLAND1_EMPTY)
	continue;
      // tile = ISLAND1_EMPTY; // need to fix this

      ur = get_island1_tile (i + 1, j + 1);
      ul = get_island1_tile (i - 1, j + 1);
      dr = get_island1_tile (i + 1, j - 1);
      dl = get_island1_tile (i - 1, j - 1);

      u = get_island1_tile (i, j + 1);
      d = get_island1_tile (i, j - 1);
      l = get_island1_tile (i - 1, j);
      r = get_island1_tile (i + 1, j);

      if (u != ISLAND1_EMPTY
	  && d != ISLAND1_EMPTY
	  && l != ISLAND1_EMPTY
	  && r == ISLAND1_EMPTY && ul != ISLAND1_EMPTY && dl != ISLAND1_EMPTY)
      {
	island1_tile[i][j] = ISLAND1_L;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_L2;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_L3;
	continue;
      }

      if (u != ISLAND1_EMPTY
	  && d != ISLAND1_EMPTY
	  && l == ISLAND1_EMPTY
	  && r != ISLAND1_EMPTY && ur != ISLAND1_EMPTY && dr != ISLAND1_EMPTY)
      {
	island1_tile[i][j] = ISLAND1_R;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_R2;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_R3;
	continue;
      }

      if (u != ISLAND1_EMPTY
	  && d == ISLAND1_EMPTY
	  && l != ISLAND1_EMPTY
	  && r != ISLAND1_EMPTY && ul != ISLAND1_EMPTY && ur != ISLAND1_EMPTY)
      {
	island1_tile[i][j] = ISLAND1_U;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_U2;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_U3;
	continue;
      }

      if (u == ISLAND1_EMPTY
	  && d != ISLAND1_EMPTY
	  && l != ISLAND1_EMPTY
	  && r != ISLAND1_EMPTY && dl != ISLAND1_EMPTY && dr != ISLAND1_EMPTY)
      {
	island1_tile[i][j] = ISLAND1_D;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_D2;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_D3;
	continue;
      }


      if (u != ISLAND1_EMPTY
	  && d != ISLAND1_EMPTY
	  && l != ISLAND1_EMPTY
	  && r != ISLAND1_EMPTY
	  && ul != ISLAND1_EMPTY
	  && ur == ISLAND1_EMPTY
	  && dl == ISLAND1_EMPTY && dr != ISLAND1_EMPTY)
      {
	island1_tile[i][j] = ISLAND1_UL_DR;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_UL_DR2;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_UL_DR3;
	continue;
      }

      if (u != ISLAND1_EMPTY
	  && d != ISLAND1_EMPTY
	  && l != ISLAND1_EMPTY
	  && r != ISLAND1_EMPTY
	  && ul == ISLAND1_EMPTY
	  && ur != ISLAND1_EMPTY
	  && dl != ISLAND1_EMPTY && dr == ISLAND1_EMPTY)
      {
	island1_tile[i][j] = ISLAND1_UR_DL;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_UR_DL2;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_UR_DL3;
	continue;
      }

      if ((u == ISLAND1_EMPTY || ur == ISLAND1_EMPTY)
	  && d != ISLAND1_EMPTY && (l == ISLAND1_EMPTY || dl == ISLAND1_EMPTY)
	  && r != ISLAND1_EMPTY)
      {
	island1_tile[i][j] = ISLAND1_DR;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_DR2;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_DR3;
	continue;
      }

      if ((u == ISLAND1_EMPTY || ul == ISLAND1_EMPTY)
	  && d != ISLAND1_EMPTY
	  && l != ISLAND1_EMPTY
	  && (r == ISLAND1_EMPTY || dr == ISLAND1_EMPTY))
      {
	island1_tile[i][j] = ISLAND1_DL;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_DL2;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_DL3;
	continue;
      }

      if (u != ISLAND1_EMPTY && (d == ISLAND1_EMPTY || dr == ISLAND1_EMPTY)
	  && (l == ISLAND1_EMPTY || ul == ISLAND1_EMPTY)
	  && r != ISLAND1_EMPTY)
      {
	island1_tile[i][j] = ISLAND1_UR;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_UR2;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_UR3;
	continue;
      }

      if (u != ISLAND1_EMPTY && (d == ISLAND1_EMPTY || dl == ISLAND1_EMPTY)
	  && l != ISLAND1_EMPTY
	  && (r == ISLAND1_EMPTY || ur == ISLAND1_EMPTY))
      {
	island1_tile[i][j] = ISLAND1_UL;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_UL2;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_UL3;
	continue;
      }
      /*
       * if (u == ISLAND1_EMPTY && d != ISLAND1_EMPTY && l ==
       * ISLAND1_EMPTY && r != ISLAND1_EMPTY) { island1_tile [i] 
       * [j] = ISLAND1_DR; continue; }
       * 
       * if (u == ISLAND1_EMPTY && d != ISLAND1_EMPTY && l !=
       * ISLAND1_EMPTY && r == ISLAND1_EMPTY) { island1_tile [i] 
       * [j] = ISLAND1_DL; continue; }
       * 
       * if (u != ISLAND1_EMPTY && d == ISLAND1_EMPTY && l ==
       * ISLAND1_EMPTY && r != ISLAND1_EMPTY) { island1_tile [i] 
       * [j] = ISLAND1_UR; continue; }
       * 
       * if (u != ISLAND1_EMPTY && d == ISLAND1_EMPTY && l !=
       * ISLAND1_EMPTY && r == ISLAND1_EMPTY) { island1_tile [i] 
       * [j] = ISLAND1_UL; continue; } 
       */
      if (ul == ISLAND1_EMPTY
	  && ur != ISLAND1_EMPTY
	  && dl != ISLAND1_EMPTY && dr != ISLAND1_EMPTY)
      {
	island1_tile[i][j] = ISLAND1_NOT_DR;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_NOT_DR2;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_NOT_DR3;
	continue;
      }

      if (ul != ISLAND1_EMPTY
	  && ur == ISLAND1_EMPTY
	  && dl != ISLAND1_EMPTY && dr != ISLAND1_EMPTY)
      {
	island1_tile[i][j] = ISLAND1_NOT_DL;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_NOT_DL2;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_NOT_DL3;
	continue;
      }

      if (ul != ISLAND1_EMPTY
	  && ur != ISLAND1_EMPTY
	  && dl == ISLAND1_EMPTY && dr != ISLAND1_EMPTY)
      {
	island1_tile[i][j] = ISLAND1_NOT_UR;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_NOT_UR2;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_NOT_UR3;
	continue;
      }

      if (ul != ISLAND1_EMPTY
	  && ur != ISLAND1_EMPTY
	  && dl != ISLAND1_EMPTY && dr == ISLAND1_EMPTY)
      {
	island1_tile[i][j] = ISLAND1_NOT_UL;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_NOT_UL2;
	if (grand (ODD_CHANCE) == 0)
	  island1_tile[i][j] = ISLAND1_NOT_UL3;
	continue;
      }

      island1_tile[i][j] = ISLAND1_FULL;

      if (grand (6) == 0)
	island1_tile[i][j] = ISLAND1_FULL + grand (5);
    }
  }
}

int get_island1_tile (int x, int y)
{

  if (x == -1 && y == -1)
    return island1_tile[8][ISLAND1_Y - 1];
  if (x == -1 && y == ISLAND1_Y)
    return island1_tile[8][0];
  if (x == 9 && y == -1)
    return island1_tile[0][ISLAND1_Y - 1];
  if (x == 9 && y == ISLAND1_Y)
    return island1_tile[0][0];
  if (x == -1)
    return island1_tile[8][y];
  if (x == 9)
    return island1_tile[0][y];
  if (y == -1)
    return island1_tile[x][ISLAND1_Y - 1];
  if (y == ISLAND1_Y)
    return island1_tile[x][0];

  return island1_tile[x][y];


}


void fix_platform (int y1, int y2)
{
  int i, j, tile;

  for (i = 0; i < 9; i++)
  {
    for (j = y1; j < y2; j++)
    {
      if (platform_tile[i][j] == PLATFORM_EMPTY)
	continue;
      tile = PLATFORM_NODE;	// need to fix this
      if (get_tile (i - 1, j) != PLATFORM_EMPTY)
	tile = PLATFORM_L;
      if (get_tile (i + 1, j) != PLATFORM_EMPTY)
	tile = PLATFORM_R;
      if (get_tile (i, j + 1) != PLATFORM_EMPTY)
	tile = PLATFORM_U;
      if (get_tile (i, j - 1) != PLATFORM_EMPTY)
	tile = PLATFORM_D;

      if (get_tile (i - 1, j) != PLATFORM_EMPTY
	  && get_tile (i + 1, j) != PLATFORM_EMPTY)
	tile = PLATFORM_LR;
      if (get_tile (i, j - 1) != PLATFORM_EMPTY
	  && get_tile (i, j + 1) != PLATFORM_EMPTY)
	tile = PLATFORM_UD;

      if (get_tile (i + 1, j) != PLATFORM_EMPTY
	  && get_tile (i, j + 1) != PLATFORM_EMPTY)
	tile = PLATFORM_UR;
      if (get_tile (i - 1, j) != PLATFORM_EMPTY
	  && get_tile (i, j - 1) != PLATFORM_EMPTY)
	tile = PLATFORM_DL;
      if (get_tile (i + 1, j) != PLATFORM_EMPTY
	  && get_tile (i, j - 1) != PLATFORM_EMPTY)
	tile = PLATFORM_DR;
      if (get_tile (i - 1, j) != PLATFORM_EMPTY
	  && get_tile (i, j + 1) != PLATFORM_EMPTY)
	tile = PLATFORM_UL;

      if (get_tile (i - 1, j) != PLATFORM_EMPTY
	  && get_tile (i, j + 1) != PLATFORM_EMPTY
	  && get_tile (i, j - 1) != PLATFORM_EMPTY)
	tile = PLATFORM_UDL;

      if (get_tile (i + 1, j) != PLATFORM_EMPTY
	  && get_tile (i, j + 1) != PLATFORM_EMPTY
	  && get_tile (i, j - 1) != PLATFORM_EMPTY)
	tile = PLATFORM_UDR;

      if (get_tile (i - 1, j) != PLATFORM_EMPTY
	  && get_tile (i + 1, j) != PLATFORM_EMPTY
	  && get_tile (i, j - 1) != PLATFORM_EMPTY)
	tile = PLATFORM_DLR;

      if (get_tile (i - 1, j) != PLATFORM_EMPTY
	  && get_tile (i + 1, j) != PLATFORM_EMPTY
	  && get_tile (i, j + 1) != PLATFORM_EMPTY)
	tile = PLATFORM_ULR;

      if (get_tile (i - 1, j) == PLATFORM_EMPTY
	  && get_tile (i + 1, j) == PLATFORM_EMPTY
	  && get_tile (i, j - 1) == PLATFORM_EMPTY
	  && get_tile (i, j + 1) == PLATFORM_EMPTY)
	tile = PLATFORM_EMPTY;

      if (get_tile (i - 1, j) != PLATFORM_EMPTY
	  && get_tile (i + 1, j) != PLATFORM_EMPTY
	  && get_tile (i, j - 1) != PLATFORM_EMPTY
	  && get_tile (i, j + 1) != PLATFORM_EMPTY)
	tile = PLATFORM_NODE + grand (3);

      platform_tile[i][j] = tile;

    }
  }

  int t = 0;

  for (i = 0; i < 9; i++)
  {
    for (j = y1; j < y2; j++)
    {
      t = get_tile (i, j);
      /*
       * if (t == PLATFORM_LR_TLR) { if (check_tile_flow(t,
       * get_tile(i + 1, j))) t = PLATFORM_LR_TL; if
       * (check_tile_flow(t, get_tile(i - 1, j))) t =
       * PLATFORM_LR_TR; if (check_tile_flow(t, get_tile(i - 1,
       * j)) && check_tile_flow(t, get_tile(i + 1, j))) t =
       * PLATFORM_LR; } if (t == PLATFORM_UD_TUD) { if
       * (check_tile_flow(t, get_tile(i, j + 1))) t =
       * PLATFORM_UD_TU; if (check_tile_flow(t, get_tile(i, j -
       * 1))) t = PLATFORM_UD_TD; if (check_tile_flow(t,
       * get_tile(i, j - 1)) && check_tile_flow(t, get_tile(i, j 
       * + 1))) t = PLATFORM_UD; }
       */
      platform_tile[i][j] = t;
    }
  }

  for (i = 0; i < 9; i++)
  {
    for (j = y1; j < y2; j++)
    {
      if (get_tile (i, j) == PLATFORM_D && get_tile (i, j + 1) == PLATFORM_U)
      {
	platform_tile[i][j] = PLATFORM_EMPTY;
	platform_tile[i][j + 1] = PLATFORM_EMPTY;
      }
    }
  }

}

/*
 * int check_tile_flow(int t1, int t2) { if (t1 == PLATFORM_LR_TLR || t1
 * == PLATFORM_LR_TL || t1 == PLATFORM_LR_TR || t1 == PLATFORM_LR) { if
 * (t2 == PLATFORM_LR_TLR || t2 == PLATFORM_LR_TR || t2 == PLATFORM_LR_TL
 * || t2 == PLATFORM_LR) return 1; return 0; } if (t1 == PLATFORM_UD_TUD
 * || t1 == PLATFORM_UD_TU || t1 == PLATFORM_UD_TD || t1 == PLATFORM_UD) {
 * if (t2 == PLATFORM_UD_TUD || t2 == PLATFORM_UD_TU || t2 ==
 * PLATFORM_UD_TD || t2 == PLATFORM_UD) return 1; return 0; } return 0; } 
 */

int get_tile (int x, int y)
{
  if (x == -1 && y == -1)
    return platform_tile[8][PLAT_Y - 1];
  if (x == -1 && y == PLAT_Y)
    return platform_tile[8][0];
  if (x == 9 && y == -1)
    return platform_tile[0][PLAT_Y - 1];
  if (x == 9 && y == PLAT_Y)
    return platform_tile[0][0];
  if (x == -1)
    return platform_tile[8][y];
  if (x == 9)
    return platform_tile[0][y];
  if (y == -1)
    return platform_tile[x][PLAT_Y - 1];
  if (y == PLAT_Y)
    return platform_tile[x][0];

  return platform_tile[x][y];


}
