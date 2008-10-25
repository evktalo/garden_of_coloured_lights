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
#include "pbullet.h"
#include "cloud.h"
#include "sound.h"
#include "enemy.h"

#include "stuff.h"

#define TARGET_ARRAY_SIZE 25

#include "palette.h"

#define SCREENSHOT


#ifdef SCREENSHOT
#include <string.h>
#include <stdio.h>
extern RGB palet [1] [256];
#endif /* SCREENSHOT */

void multi_fire(int m);
void mflash(int b, int w, int m);
int find_enemies(int targets [TARGET_ARRAY_SIZE], int x, int y, int number, int start_number, int force);
void fire_seekers(int m);

#define SHOW_DAMAGE

#ifdef SHOW_DAMAGE
void calc_damage(int dam, int speed);
extern int damage_per_time;
#endif


enum
{
MOVE_NO,
MOVE_L,
MOVE_R,
MOVE_U,
MOVE_D,
MOVE_UL,
MOVE_DL,
MOVE_UR,
MOVE_DR
};

void get_input(void)
{


#ifdef SCREENSHOT

static int scrs = 0;
static int sshot_counter = 0;


 char sfile [64];
 char istr [64];

 if (sshot_counter > 0)
  sshot_counter --;

 if (key [KEY_F1] && sshot_counter <= 0)
 {
  BITMAP *scrshot_bmp;
   scrshot_bmp = create_bitmap(640, 480);
   blit(screen, scrshot_bmp, 0,0,0,0,640,480);

  strcpy(sfile, "screenshot_");
  sprintf(sfile, "%s%02d", sfile, scrs);
  //strcat(sfile, itoa(scrs, istr, 10));
  strcat(sfile, ".bmp");
  save_bitmap(sfile, scrshot_bmp, palet [0]);
  clear_to_color(screen, COL_WHITE);
  scrs ++;
  sshot_counter = 15;
  destroy_bitmap(scrshot_bmp);
 }

 if (arena.counter % 10 == 0)
 {
  if (key [KEY_1] && player.wlevel [0] < 9)
  {
   player.wlevel [0] ++;
   player.wxp [0] = 0;
   set_wpixels();
   play_effectwfvx(WAV_LEVEL, tone[NOTE_2G], 250, player.x);
   player.wgained [0] = 20;
  }
  if (key [KEY_2] && player.wlevel [1] < 9)
  {
   player.wlevel [1] ++;
   player.wxp [1] = 0;
   set_wpixels();
  }
  if (key [KEY_3] && player.wlevel [2] < 9)
  {
   player.wlevel [2] ++;
   player.wxp [2] = 0;
   set_wpixels();
  }
  if (key [KEY_4])
  {
   player.lives ++;
//   play_effectwfvx(WAV_EXTRA, tone [NOTE_3C], 100, 320);
   play_effectwfvx(WAV_LEVEL, tone[NOTE_1G], 250, player.x);
  }
  if (key [KEY_5] && arena.shift_message == 0)
  {
   arena.shift_message = 300;
   arena.phase ++;
  }
  if (key [KEY_6])
   arena.level_finished = 50;

 }

#endif


    if (player.respawning > 0)
    {
     player.y -= 4000;
     player.respawning --;
     return;
    }

    int xc = 0, yc = 0;

//   int move_which = 0;

  int i;

  for (i = 0; i < 3; i ++)
  {

    if (player.beam [i] == -1 && player.beam_flicker [i] == 0)
     player.beam_counter [i] = 0;
      else
       player.beam [i] = -1;
  }

/*   if (key [player.key [CKEY_FIRE1]])
   {
    multi_fire(0);
   }
     else
     {
      if (key [player.key [CKEY_FIRE2]])
       multi_fire(1);
        else
        {
         if (key [player.key [CKEY_FIRE3]])
          multi_fire(2);
        }
     }*/

 if (options.key_or_joy == 1)
 {
   poll_joystick();

   if (joy[0].button[options.joy_button [0]].b)
    multi_fire(0);
   if (joy[0].button[options.joy_button [1]].b)
    multi_fire(1);
   if (joy[0].button[options.joy_button [2]].b)
    multi_fire(2);

   if (player.swing_hold == 1 && joy[0].button [options.joy_button[player.blade_index]].b == 0)
    player.swing_hold = 0;

   if (player.ring_hold == 1 && joy[0].button [options.joy_button[player.ring_index]].b == 0)
    player.ring_hold = 0;

   if (joy[0].button[options.joy_button [0]].b
    || joy[0].button[options.joy_button [1]].b
    || joy[0].button[options.joy_button [2]].b)
     player.firing = 1;

//   int moving = MOVE_NO;

   xc = joy[0].stick[options.joy_stick].axis[0].pos * options.joy_sensitivity;// / 5120 * 6000;
   yc = joy[0].stick[options.joy_stick].axis[1].pos * options.joy_sensitivity;// / 5120 * 6000;

   int jangle = radians_to_angle(atan2(yc, xc));
   jangle &= 1023;
   if (xc == 0 && yc == 0)
    jangle = ANGLE_4;
   int dist = hypot(yc, xc);

   if (dist > 6000)
    dist = 6000;

   if (joy[0].button[options.joy_button [3]].b)
    dist /= 2;

   xc = xpart(jangle, dist);
   yc = ypart(jangle, dist);

  if (xc < -6000)
   xc = -6000;
  if (yc < -6000)
   yc = -6000;
  if (xc > 6000)
   xc = 6000;
  if (yc > 6000)
   yc = 6000;

// NO!!! Need to calc angle then move in that angle

   player.wings1 += xc / 1000;

   if (player.wings1 > 20)
    player.wings1 = 20;
   if (player.wings1 < -20)
    player.wings1 = -20;

   player.wings2 += yc / 1000;

   if (player.wings2 > 20)
    player.wings2 = 20;
   if (player.wings2 < -20)
    player.wings2 = -20;

   if (xc == 0)
   {
    if (player.wings1 > 0)
     player.wings1 -= 2;
    if (player.wings1 < 0)
     player.wings1 += 2;
   }

   if (yc == 0)
   {
    if (player.wings2 > 0)
     player.wings2 -= 2;
    if (player.wings2 < 0)
     player.wings2 += 2;

   }

     player.x += xc;
     player.y += yc;
     if (player.x < X_MIN)
       player.x = X_MIN;
     if (player.y < Y_MIN)
       player.y = Y_MIN;
     if (player.x > X_MAX)
       player.x = X_MAX;
     if (player.y > Y_MAX)
       player.y = Y_MAX;


     if (player.ring_index != -1)
     {
      player.move_x = 0;
      player.move_y = 0;
      if (jangle > ANGLE_16)// && jangle <= ANGLE_8 + ANGLE_16)
      {
       player.move_x = 1;
       player.move_y = 1;
      }
      if (jangle > ANGLE_8 + ANGLE_16)// && jangle <= ANGLE_8 + ANGLE_16)
      {
       player.move_x = 0;
       player.move_y = 1;
      }
      if (jangle > ANGLE_4 + ANGLE_16)// && jangle <= ANGLE_8 + ANGLE_16)
      {
       player.move_x = -1;
       player.move_y = 1;
      }
      if (jangle > ANGLE_4 + ANGLE_8 + ANGLE_16)// && jangle <= ANGLE_8 + ANGLE_16)
      {
       player.move_x = -1;
       player.move_y = 0;
      }
      if (jangle > ANGLE_2 + ANGLE_16)// && jangle <= ANGLE_8 + ANGLE_16)
      {
       player.move_x = -1;
       player.move_y = -1;
      }
      if (jangle > ANGLE_2 + ANGLE_8 + ANGLE_16)// && jangle <= ANGLE_8 + ANGLE_16)
      {
       player.move_x = 0;
       player.move_y = -1;
      }
      if (jangle > ANGLE_2 + ANGLE_4 + ANGLE_16)// && jangle <= ANGLE_8 + ANGLE_16)
      {
       player.move_x = 1;
       player.move_y = -1;
      }
      if (jangle > ANGLE_1-ANGLE_16 || jangle < ANGLE_16)// && jangle <= ANGLE_16)
      {
       player.move_x = 1;
       player.move_y = 0;
      }
/*      if (key_right)
       player.move_x = 1;
      if (key_left)
       player.move_x = -1;
      if (key_down)
       player.move_y = 1;
      if (key_up)
       player.move_y = -1;*/
     }


 }
  else
  {

 if (key [player.key [CKEY_FIRE1]])
    multi_fire(0);

 if (key [player.key [CKEY_FIRE2]])
    multi_fire(1);

 if (key [player.key [CKEY_FIRE3]])
    multi_fire(2);

   if (player.swing_hold == 1 && !key[player.key [CKEY_FIRE1 + player.blade_index]])
    player.swing_hold = 0;

   if (player.ring_hold == 1 && !key[player.key [CKEY_FIRE1 + player.ring_index]])
    player.ring_hold = 0;

   if (key [player.key [CKEY_FIRE1]]
    || key [player.key [CKEY_FIRE2]]
    || key [player.key [CKEY_FIRE3]])
     player.firing = 1;

   int moving = MOVE_NO;

   char key_left = key [player.key [CKEY_LEFT]];
   char key_right = key [player.key [CKEY_RIGHT]];
   char key_up = key [player.key [CKEY_UP]];
   char key_down = key [player.key [CKEY_DOWN]];

   if (key_left)
   {
    if (key_right || player.x <= X_MIN)
     xc = 0;
      else xc = -2000;
   }
    else
    {
     if (key_right)
     {
      if (player.x < X_MAX)
       xc = 2000;
     }
    }

   if (key_up)
   {
    if (key_down || player.y <= Y_MIN)
     yc = 0;
      else yc = -2000;
   }
    else
     if (key_down)
     {
      if (player.y < Y_MAX - 10000)
       yc = 2000;
     }

   if (xc > 0 && yc > 0)
   {
    xc = 1414;
    yc = 1414;
    moving = MOVE_DR;
   }
   if (xc < 0 && yc > 0)
   {
    xc = -1414;
    yc = 1414;
    moving = MOVE_DL;
   }
   if (xc < 0 && yc < 0)
   {
    xc = -1414;
    yc = -1414;
    moving = MOVE_UL;
   }
   if (xc > 0 && yc < 0)
   {
    xc = 1414;
    yc = -1414;
    moving = MOVE_UR;
   }

 xc *= 3;
 yc *= 3;


   player.wings1 += xc / 1000;

   if (player.wings1 > 20)
    player.wings1 = 20;
   if (player.wings1 < -20)
    player.wings1 = -20;

   player.wings2 += yc / 1000;

   if (player.wings2 > 20)
    player.wings2 = 20;
   if (player.wings2 < -20)
    player.wings2 = -20;

   if (xc == 0)
   {
    if (player.wings1 > 0)
     player.wings1 -= 2;
    if (player.wings1 < 0)
     player.wings1 += 2;
   }

   if (yc == 0)
   {
    if (player.wings2 > 0)
     player.wings2 -= 2;
    if (player.wings2 < 0)
     player.wings2 += 2;

   }

 if (key [player.key [CKEY_SLOW]])
 {
  xc /= 2;
  yc /= 2;
 }

     player.x += xc;
     player.y += yc;
     if (player.x < X_MIN)
       player.x = X_MIN;
     if (player.y < Y_MIN)
       player.y = Y_MIN;
     if (player.x > X_MAX)
       player.x = X_MAX;
     if (player.y > Y_MAX)
       player.y = Y_MAX;


     if (player.ring_index != -1)
     {
      player.move_x = 0;
      player.move_y = 0;
      if (key_right)
       player.move_x = 1;
      if (key_left)
       player.move_x = -1;
      if (key_down)
       player.move_y = 1;
      if (key_up)
       player.move_y = -1;
     }


  } // end keyboard control

}


void multi_fire(int m)
{

 if (player.weapon [m] == WPN_BLADE)
 {
#ifdef SHOW_DAMAGE
   calc_damage(100 + player.wlevel [player.blade_index] * 22, 1000);
#endif

   if (player.swing_state == BLADE_LEFT)
   {
//    if (player.recycle > 0)
//     return;
    player.swing_state = BLADE_CW_SLASH;
    player.swing_hold = 1;
    play_effectwfvx(WAV_WHOOSH, 600, 120, player.x);
    return;
   }
   if (player.swing_state == BLADE_RIGHT)
   {
//    if (player.recycle > 0)
//     return;
    player.swing_state = BLADE_ACW_SLASH;
    player.swing_hold = 1;
    play_effectwfvx(WAV_WHOOSH, 600, 120, player.x);
    return;
   }

  return;
 }

 if (player.recycle [m] > 0)
  return;

 int angle, b;

 switch(player.weapon [m])
 {
  case WPN_RECT:
   b = create_pbullet(PBULLET_RECT);
   if (b != -1)
   {
    pbullet[b].x = player.wx [m];
    pbullet[b].y = player.wy [m] - 4000;
    pbullet[b].x_speed = 0;
    pbullet[b].y_speed = - 5500;
    pbullet[b].xsize = 9000 + player.wlevel [m] * 500;
    pbullet[b].ysize = 9000 + player.wlevel [m] * 500;
    pbullet[b].level = player.wlevel [m];
    pbullet[b].dam = 10;
    pbullet[b].source = m;
    mflash(b, player.weapon [m], m);
   }
   player.recycle [m] = 1000;
   player.recycle_speed [m] = 15;
#ifdef SHOW_DAMAGE
   calc_damage(pbullet[b].dam, player.recycle_speed [m]);
#endif
   player.wfired = m;
//   player.wframe_speed = 10;
//   player.wframe_max = 39;
   play_effectwfvx(WAV_RECT2, 800, 100, player.x);
   break;
  case WPN_CANNON:
   b = create_pbullet(PBULLET_CANNON);
   if (b != -1)
   {
    pbullet[b].x = player.wx [m];
    pbullet[b].y = player.wy [m] - 4000;
    pbullet[b].x_speed = 0;
    pbullet[b].y_speed = - 7000;
    pbullet[b].xsize = 9000 + player.wlevel [m] * 500;
    pbullet[b].ysize = 9000 + player.wlevel [m] * 500;
    pbullet[b].level = player.wlevel [m];
    pbullet[b].dam = 2000 + player.wlevel [m] * 360;
    pbullet[b].source = m;
    mflash(b, player.weapon [m], m);
   }
   player.recycle [m] = 1000;
   player.recycle_speed [m] = 15;
#ifdef SHOW_DAMAGE
   calc_damage(pbullet[b].dam, player.recycle_speed [m]);
#endif
   player.wfired = m;
//   player.wframe_speed = 10;
//   player.wframe_max = 39;
   play_effectwfvx(WAV_CANNON, 1300, 250, player.x);
   break;
  case WPN_EIGHT:
   if (player.ring_hold == 0)
   {
    if (player.move_x == -1)
     player.ring_angle = 0;
    if (player.move_x == 1)
     player.ring_angle = ANGLE_2;
    if (player.move_y == -1)
     player.ring_angle = ANGLE_4;
    if (player.move_y == 1)
     player.ring_angle = ANGLE_2 + ANGLE_4;
    if (player.move_x == -1 && player.move_y == -1)
     player.ring_angle = ANGLE_8;
    if (player.move_x == 1 && player.move_y == -1)
     player.ring_angle = ANGLE_4 + ANGLE_8;
    if (player.move_x == -1 && player.move_y == 1)
     player.ring_angle = ANGLE_2 + ANGLE_4 + ANGLE_8;
    if (player.move_x == 1 && player.move_y == 1)
     player.ring_angle = ANGLE_2 + ANGLE_8;
   }
   b = create_pbullet(PBULLET_RING);
   if (b != -1)
   {
    pbullet[b].x_speed = xpart(player.ring_angle, 6000);
    pbullet[b].y_speed = ypart(player.ring_angle, 6000);
    pbullet[b].x = player.wx [m] - pbullet[b].x_speed;
    pbullet[b].y = player.wy [m] - pbullet[b].y_speed;
    pbullet[b].xsize = 10000; // need to set this better!
    pbullet[b].ysize = 10000;
    pbullet[b].status = 45;
    pbullet[b].level = player.wlevel [m];
    pbullet[b].dam = 120 + player.wlevel [m] * 11;
    pbullet[b].source = m;
    mflash(b, player.weapon [m], m);
   }
   player.recycle [m] = 1000;
   player.recycle_speed [m] = 70 + player.wlevel [m] * 6;
#ifdef SHOW_DAMAGE
   calc_damage(pbullet[b].dam, player.recycle_speed [m]);
#endif
   player.wfired = m;
   player.ring_hold = 1;
//   player.wframe_speed = 10;
//   player.wframe_max = 39;
//   play_effectwfvx(WAV_CANNON, 1300, 250, player.x);
   play_effectwfvx(WAV_MOUTH, 1250 - player.wlevel [m] * 70, 90, player.x);
   break;
  case WPN_MGUN:
   b = create_pbullet(PBULLET_MGUN);
   if (b != -1)
   {
    pbullet[b].x = player.wx [m] - 4000;
    pbullet[b].y = player.wy [m] - 13000 - player.wlevel [m] * 1000;
    pbullet[b].x_speed = 0;
    pbullet[b].y_speed = - 14000;
    pbullet[b].xsize = 2000;
    pbullet[b].ysize = 15000;
    pbullet[b].dam = 70 + player.wlevel [m] * 5;
    pbullet[b].source = m;
    pbullet[b].level = player.wlevel [m];
    mflash(b, WPN_MGUN, m);
   }

   b = create_pbullet(PBULLET_MGUN);
   if (b != -1)
   {
    pbullet[b].x = player.wx [m] + 4000;
    pbullet[b].y = player.wy [m] - 13000 - player.wlevel [m] * 1000;
    pbullet[b].x_speed = 0;
    pbullet[b].y_speed = - 14000;
    pbullet[b].xsize = 2000;
    pbullet[b].ysize = 15000;
    pbullet[b].dam = 70 + player.wlevel [m] * 5;
    pbullet[b].source = m;
    pbullet[b].level = player.wlevel [m];
    mflash(b, WPN_MGUN, m);
   }
   player.recycle [m] = 1000;
   player.recycle_speed [m] = 70 + player.wlevel [m] * 7;
#ifdef SHOW_DAMAGE
   calc_damage(pbullet[b].dam * 2, player.recycle_speed [m]);
#endif
   player.wfired = m;
   play_effectwfvx(WAV_MGUN, 800, 60, player.x);
   break;
  case WPN_LWBEAM:
   mflash(b, player.weapon [m], m);
   player.beam_fade [m] = 10;
   player.beam_counter [m] ++;
   player.beam_open [m] ++;
   if (player.beam_open [m] > 8)
    player.beam_open [m] = 8;
   if (player.beam_counter [m] % 15 == 1)
    play_effectwfvx(WAV_LWBEAM, 1000, 150, player.x);
   if (player.wstrength [m] == 2 && arena.counter % 2 == 0)
   {
    player.beam_counter [m] --;
    player.beam_flicker [m] = 2;
    break;
   }
   if (player.wstrength [m] == 3 && arena.counter % 3 != 0)
   {
    player.beam_flicker [m] = 2;
    player.beam_counter [m] --;
    break;
   }
//   if (arena.counter % 6 != 0)
//    break;
   player.beam_flicker [m] = 0;
   player.beam [m] = m;
   beam_collision(m);
   player.wfired = m;
#ifdef SHOW_DAMAGE
   calc_damage(60 + player.wlevel [m] * 15, 100);
// see lwbeam function in pbullet.c for damage
#endif
   break;
  case WPN_SWBEAM:
   player.swbeam [m] = 10 + player.wlevel [m];
   player.swbeam_level [m] = player.wlevel [m];
   player.swbeam_x1 [m] = player.wx [m];
   player.swbeam_y1 [m] = player.wy [m];
   mflash(b, player.weapon [m], m);
   swbeam_collision(m);
   player.recycle [m] = 1000;
   player.recycle_speed [m] = 15;
   player.swbeam_counter [m] = 50;
   player.wfired = m;
   play_effectwfvx(WAV_SWBEAM, 1000, 200, player.x);
#ifdef SHOW_DAMAGE
   calc_damage(1400 + player.wlevel [m] * 240, player.recycle_speed [m]);
// see swbeam function in pbullet.c for damage
#endif
   break;
  case WPN_SEEKER:
   fire_seekers(m);
   player.recycle [m] = 1000;
   player.recycle_speed [m] = 17;
   player.wfired = m;
#ifdef SHOW_DAMAGE
   calc_damage(pbullet[0].dam * (3 + player.wlevel [m]), player.recycle_speed [m]);
// see lwbeam function in pbullet.c for damage
#endif
   play_effectwfvx(WAV_WORMS, 2500 - player.wlevel [m] * 100, 90, player.x);
   break;
  case WPN_SCATTER:
   b = create_pbullet(PBULLET_SCATTER);
   if (b != -1)
   {
    pbullet[b].x = player.wx [m];
    pbullet[b].y = player.wy [m] - 4000;
    angle = -ANGLE_4 + grand(ANGLE_16) - grand(ANGLE_16);
    pbullet[b].angle = angle;
    pbullet[b].x_speed = xpart(angle, 9000);
    pbullet[b].y_speed = ypart(angle, 9000);
    pbullet[b].xsize = 1000;
    pbullet[b].ysize = 1000;
    pbullet[b].level = player.wlevel [m];
    pbullet[b].dam = 50 + player.wlevel [m] * 6;
    pbullet[b].source = m;
    mflash(b, player.weapon [m], m);
   }
   player.recycle [m] = 1000;
   player.recycle_speed [m] = 220 + player.wlevel [m] * 16;
#ifdef SHOW_DAMAGE
   calc_damage(pbullet[b].dam, player.recycle_speed [m]);
#endif
   player.wfired = m;
   play_effectwfvx(WAV_SCATTER, 800, 50, player.x);
   break;
  case WPN_BLADE:
   break;

 }


}

#ifdef SHOW_DAMAGE
void calc_damage(int dam, int speed)
{
 damage_per_time = (dam * speed) / 100;

}
#endif

void fire_seekers(int m)
{
     int i;

 int angles [13] = {ANGLE_2, ANGLE_2 + ANGLE_16, ANGLE_2 - ANGLE_16, ANGLE_2 + ANGLE_8, ANGLE_2 - ANGLE_8,
               ANGLE_2 + ANGLE_8 + ANGLE_16, ANGLE_2 - ANGLE_8 - ANGLE_16,
               ANGLE_4, -ANGLE_4, ANGLE_4 - ANGLE_8, -ANGLE_4 + ANGLE_8, ANGLE_4 - ANGLE_16, -ANGLE_4 + ANGLE_16};

 int targets [TARGET_ARRAY_SIZE];

 int t;

 for (t = 0; t < TARGET_ARRAY_SIZE; t ++)
 {
  targets [t] = -1;
 }

 int number = 3 + player.wlevel [m];//player.charge / 100;

 int which_angle = 0;
  if (number % 2 == 0)
   which_angle = 1;

 find_enemies(targets, player.x, player.y, number, 0, 0);

 int b, angle;

 for (i = 0; i < number; i ++)
 {
  b = create_pbullet(PBULLET_SEEKER);
  if (b != -1)
  {
//   angle = angles [which_angle] - ANGLE_4;
   pbullet[b].target = targets [i];
/*   pbullet[b].x = player.wx [m];
   pbullet[b].y = player.wy [m];
   if (pbullet[b].target == -1)
   {
    angle = angles [which_angle] - ANGLE_4;
   }
    else
    {
     angle = radians_to_angle(atan2(pbullet[b].y - enemy[pbullet[b].target].y, pbullet[b].x - enemy[pbullet[b].target].x)) - ANGLE_2;
    }*/
   angle = angles [which_angle] - ANGLE_4;
   pbullet[b].x = player.wx [m] + xpart(angle, 8000);
   pbullet[b].y = player.wy [m] + ypart(angle, 8000);
//   pbullet[b].x += xpart(angle, 8000);
//   pbullet[b].y += ypart(angle, 8000);
   quick_cloud(CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, 0, 0, 25, TCOL_ORANGE, 0);
   pbullet[b].x_speed = xpart(angle, 5000);
   pbullet[b].y_speed = ypart(angle, 5000);
   pbullet[b].angle = angle;
   pbullet[b].dam = 80;// + player.wlevel [m] * 5;
   pbullet[b].persist = 1;
   pbullet[b].timeout = 140;//220 + player.wlevel [m] * 7;
   pbullet[b].source = m;
   pbullet[b].level = player.wlevel [m];

  }

  which_angle ++;


 }

// play_effectwfvx(player.sides, NWAV_CIRCLE, 400 + number * 50, 100 + number * 15, player.x);




}



int find_enemies(int targets [TARGET_ARRAY_SIZE], int x, int y, int number, int start_number, int force)
{

 int t;
 int targets_found = 0;
 int enemies_found = 0;

 int e;

 for (e = 0; e < NO_ENEMIES; e ++)
 {
  if (enemy[e].type == ENEMY_NONE)
  {
   enemy[e].distance = 900000;
   continue;
  }

  enemy[e].distance = hypot(enemy[e].y - y, enemy[e].x - x) / 100;
  enemies_found ++;

 }

 if (enemies_found == 0)
  return 0;

 int closest = -1;
 int smallest_distance = 900000;

 for (t = start_number; t < TARGET_ARRAY_SIZE; t ++)
 {
  for (e = 0; e < NO_ENEMIES; e ++)
  {
   if (enemy[e].distance < smallest_distance)
   {
    closest = e;
    smallest_distance = enemy[e].distance;
   }
  }
  if (smallest_distance == 900000)
  {
   return targets_found;
  }
  targets [t] = closest;
  enemy[closest].distance = 900000;
  targets_found ++;
  closest = -1;
  smallest_distance = 900000;
 }

 return targets_found;
}




void mflash(int b, int w, int m)
{

 int i, xa, ya, angle;
 int level = player.wlevel [m];

 switch(w)
 {
   case WPN_SWBEAM:
    xa = player.wx [m];
    ya = player.wy [m] - 8000;
//    quick_cloud(CLOUD_CIRCLE1, xa, ya, 0, 0, 15, 8, 0);
//    quick_cloud(CLOUD_CIRCLE2, xa, ya, 0, 0, 30, 7, 0);
    angle = 0;
    for (i = 0; i < 16; i ++)
    {
     angle += ANGLE_16;
//     quick_cloud(CLOUD_CIRCLE3, xa, ya, xpart(angle, 2500), ypart(angle, 2500), 35, 8, 970);
    }
    quick_cloud(CLOUD_MSHOCK, xa, ya, 0, 0, 20, 0, 0);
    break;
/*   case WPN_LWBEAM:
    angle = grand(ANGLE_1);
    xa = 2000 + grand(1000);
    quick_cloud(CLOUD_CIRCLE1, player.wx [m], player.wy [m] - 4000, xpart(angle, xa), ypart(angle, xa), 5 + grand(5), 7, 970);
    break;*/
   case WPN_CANNON:
    xa = pbullet[b].x;
    ya = pbullet[b].y;
    quick_cloud(CLOUD_CIRCLE1, xa, ya, 0, 0, 15 + level, TCOL_YELLOW, 0);
    quick_cloud(CLOUD_CIRCLE2, xa, ya, 0, 0, 30 + level, TCOL_ORANGE, 0);
    for (i = 0; i < 4; i ++)
    {
     quick_cloud(CLOUD_CIRCLE2, xa - 1000 * i, ya, -1000 - i * 1000 - level * 100, i * 400 + level * 30, 20 + i * 4 + level, TCOL_ORANGE, 920);
     quick_cloud(CLOUD_CIRCLE2, xa + 1000 * i, ya, 1000 + i * 1000 + level * 100, i * 400 + level * 30, 20 + i * 4 + level, TCOL_ORANGE, 920);
    }
    break;
   case WPN_RECT:
    xa = pbullet[b].x;
    ya = pbullet[b].y;
    quick_cloud(CLOUD_CIRCLE1, xa, ya, 0, 0, 15, TCOL_YELLOW, 0);
    quick_cloud(CLOUD_CIRCLE2, xa, ya, 0, 0, 30, TCOL_ORANGE, 0);
    angle = -ANGLE_8;
    for (i = 0; i < 4; i ++)
    {
     angle += ANGLE_4;
     quick_cloud(CLOUD_CIRCLE5, xa + xpart(angle, 10000), ya + ypart(angle, 10000), xpart(angle, 3000), ypart(angle, 3000), 60, TCOL_ORANGE, 920);
     quick_cloud(CLOUD_CIRCLE5, xa + xpart(angle, 10000), ya + ypart(angle, 10000), xpart(angle, 3000), ypart(angle, 3000), 40, TCOL_YELLOW, 920);
    }
    break;
   case WPN_MGUN:
    xa = pbullet[b].x;
    ya = pbullet[b].y + player.wlevel [m] * 1000;
    quick_cloud(CLOUD_CIRCLE2, xa, ya, 0, -1500, 15 + level / 4, TCOL_YELLOW, 930);
    quick_cloud(CLOUD_CIRCLE3, xa, ya, 0, -1500, 30 + level / 2, TCOL_ORANGE, 930);
    for (i = 0; i < 4; i ++)
    {
     quick_cloud(CLOUD_CIRCLE2, xa, ya, 0 - grand(1000) + grand(1000), (i * -1500) - grand(2000), 10 + grand(10) + level, TCOL_ORANGE, 930);
    }
    break;
   case WPN_SCATTER:
    xa = pbullet[b].x;
    ya = pbullet[b].y;
    quick_cloud(CLOUD_CIRCLE1, xa, ya - 7000, 0, -2000, 10 + level / 3, TCOL_YELLOW, 960);
    quick_cloud(CLOUD_CIRCLE2, xa, ya - 7000, 0, -2000, 20 + level / 1, TCOL_ORANGE, 960);
/*    angle = grand(ANGLE_1);
    for (i = 0; i < 6; i ++)
    {
     quick_cloud(CLOUD_CIRCLE5, xa, ya, xpart(angle, 2000),  + ypart(angle, 2000), 12 + grand(12), 7, 970);
     angle += ANGLE_6;
     angle &= 1023;
    }*/
    break;
   case WPN_EIGHT:
    xa = pbullet[b].x;
    ya = pbullet[b].y;
    quick_cloud(CLOUD_PRING, xa, ya, 0, 0, 135, 0, 0);
      break;

 }

}





/*

****************************************************

Joystick routines

****************************************************

*/

void init_joystick(void)
{

   options.joystick = 0;

   if (install_joystick(JOY_TYPE_AUTODETECT) != 0)
      return;

   if (num_joysticks == 0)
    return;

//   if (options.joy_analogue)
//    analogmode = TRUE;
//     else


   options.joystick = 1;

}


void jstick_calibrate(void)
{

   AL_CONST char *msg;

   if (num_joysticks == 0)
      return;


   while (joy[0].flags & JOYFLAG_CALIBRATE)
   {
      vsync();
      clear_bitmap(screen);

      msg = calibrate_joystick_name(0);

      textprintf_centre_ex(screen, font, 320, 170, -1, -1, msg);
      textprintf_centre_ex(screen, font, 320, 200, -1, -1, "AND PRESS SPACE");

      do
      {} while (key [KEY_SPACE] == 0);

      if (calibrate_joystick(0) != 0)
	   return;

   }

 return;

}














