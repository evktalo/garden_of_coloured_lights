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

#include "stuff.h"
#include "ebullet.h"

#include "sound.h"

#include "palette.h"
#include "cloud.h"
#include <math.h>
#include "stuff.h"
#include "level.h"


int create_enemy(int type);
void destroy_enemy(int e);
void enemy_explode(int e);
void move_enemy(int e);
void enemy_fire(int e);
int attack_angle(int e);
void move_dancer(int e);
void move_diver(int e);
void move_dswooper(int e);
void move_stopper(int e);
void move_cruiser(int e);
void move_dart_stopper(int e);
void move_drifter(int e);
void move_walker(int e);
void set_cord_colour(int b);
int get_e_angle(int e, int x, int y);
void drag_enemy(int e, int drag);
void drag_enemy_y(int e, int drag);
int hurt_enemy(int e, int dam, int source);
void register_destroyed(int e);
void allocate_xp(int e);
void gain_score(int s);
void part_explode(int e);
void launch_part(int pe, int index, int se, int xs, int ys);
void parts_must_explode(int e);
void enemy_pulse(int e, int p_event);
void enemy_pulse_slow(int e, int p_event);
void set_wpixels(void);

void delay_cloud(int x, int y, int col, int delay, int size);

int fire_dart(int x, int y, int angle, int colour, int wait, int accel);
int fire_diamond(int x, int y, int angle, int colour, int speed, int rot, int wait);
int fire_ddart(int x, int y, int angle, int colour, int wait, int accel);
int fire_wing_diamond(int x, int y, int angle, int colour, int speed, int wait);
int fire_seed(int x, int y, int angle, int colour, int wait, int accel);
int fire_seed2(int x, int y, int angle, int colour, int wait, int accel);
int fire_burst(int x, int y, int angle, int colour, int speed, int wait);
int fire_stream(int x, int y, int angle, int colour, int speed, int wait);
int fire_sdart(int x, int y, int angle, int colour, int wait, int accel);
int fire_dot(int x, int y, int angle, int colour, int size, int speed, int wait);
int fire_beam(int e, int dist);
int fire_beam2(int e, int angle, int dist, int time);
int fire_curve(int x, int y, int angle, int colour, int speed, int rot, int wait);
int fire_tracker(int x, int y, int angle, int colour, int size, int speed);

void petal_turn(int e, int p, int target_angle, int accel, int max_speed);


void track_target(int e, int x, int y, int angle, int turn);
void track_target_zero(int e, int x, int y, int angle, int turn);
void move_darter(int e);
int next_enemy;



void move_boss(int e);
void run_boss1(int e);
void arrange_boss1(int sides, int sides2, int up, int out);
void boss1_attack1(int e, int stat);
void boss1_attack2(int e, int stat);
void boss1_attack3(int e, int stat);
void boss1_turrets(int e, int fire);
void boss2_attack1(int e, int stat);
void boss2_attack2(int e, int stat);
void boss2_attack3(int e, int stat);
void boss2_attack4(int e, int stat);
void boss2_turrets(int e, int fire);
int boss2_beam(int e, int dist, int x1, int y1, int angle, int stat);

void run_boss3(int e);
void arrange_boss3(int speed1, int out1, int speed2, int out2, int speed3, int out3);
void boss3_finish_firing(int e);
void boss3_attack1(int e, int stat);
void boss3_attack2(int e, int stat);
void boss3_attack3(int e, int stat);
void boss3_attack4(int e, int stat);

void run_boss4(int e);
void run_boss4_spin(int e);
void boss4_sides_fire(int e);
void boss4_outer_fire(int e);


void run_boss2(int e);
void arrange_boss2(int sides, int sides2);


void init_enemies(void)
{

 int e;

  for (e = 0; e < NO_ENEMIES; e ++)
  {
   enemy[e].type = ENEMY_NONE;
  }

 next_enemy = 5;
}

/*
Tries to create an enemy, and returns its index if successful (or -1 if not).
The calling function can use that index to set the enemy's properties.
*/
int create_enemy(int type)
{

  int e;

  for (e = 0; e < NO_ENEMIES + 1; e ++)
  {
    if (e == NO_ENEMIES)
     return -1;
    if (enemy[e].type == ENEMY_NONE)
     break;
  }

 enemy[e].type = type;
 enemy[e].persist = 0;
 enemy[e].recycle = 40;
 enemy[e].hp = eclass[type].max_hp;
 enemy[e].pulse1 = 0;
 enemy[e].pulse2 = 0;
 enemy[e].whurt [0] = 0;
 enemy[e].whurt [1] = 0;
 enemy[e].whurt [2] = 0;
 enemy[e].parent = -1;
 enemy[e].status = 0;
 enemy[e].angle_turning_count = 2;
 enemy[e].angle1_turning_count = 2;
 enemy[e].escorting = -1;
 int i;
 for (i = 0; i < NO_PARTS; i ++)
 {
  enemy[e].part [i] = -1;
 }

 return e;

}



void run_enemies(void)
{

 int e;
/*
// maybe we want to create an enemy!
 next_enemy --;
 if (next_enemy <= 0)
 {
  e = create_enemy(ENEMY_EXAMPLE);
  if (e != -1)
  {
   enemy[e].x = grand(600000) + 20000;
   enemy[e].y = grand(440000) + 20000;
   enemy[e].x_speed = grand(5000) - 2500;
   enemy[e].y_speed = grand(5000) - 2500;
  }
  // for now we'll rely on create_enemy failing if there are too many enemies.
  next_enemy = 500;
 }
*/
// now let's run through all enemies and make them work:
 for (e = 0; e < NO_ENEMIES; e ++)
 {
  if (enemy[e].type == ENEMY_NONE)
   continue;

   switch(eclass[enemy [e].type].ai)
   {
    case AI_SWOOPER:
    case AI_HOOK:
         move_dancer(e);
         break;
    case AI_WALK:
         move_walker(e);
         break;
    case AI_DSWOOPER:
         move_dswooper(e);
         break;
    case AI_DARTER:
         move_darter(e);
         break;
    case AI_STOPPER:
         move_stopper(e);
         break;
    case AI_DIVER:
         move_diver(e);
         break;
    case AI_DART_STOPPER:
         move_dart_stopper(e);
         break;
    case AI_DRIFTER:
         move_drifter(e);
         break;
    case AI_CRUISER:
         move_cruiser(e);
         break;
    case AI_BOSS:
         move_boss(e);
         break;
   }

    if (enemy[e].persist == 0 &&
           (enemy[e].x <= (eclass[enemy[e].type].xsize * -1) - 50000
         || enemy[e].y <= (eclass[enemy[e].type].ysize * -1) - 50000
         || (enemy[e].x >= 690000 + eclass[enemy[e].type].xsize)
         || enemy[e].y >= 530000 + eclass[enemy[e].type].ysize))
       {
        register_destroyed(e);
        destroy_enemy(e);
       }

/*
  switch(enemy[e].type)
  {
   case ENEMY_EXAMPLE:
    enemy[e].recycle --;
    if (enemy[e].recycle <= 0)
    {
     enemy_fire(e);
     enemy[e].recycle = 20 + grand(5);
    }
    move_enemy(e);
    break;


  }
*/
 }

}

void move_boss(int e)
{

 if (boss.hp_bar_size < 100)
  boss.hp_bar_size ++;

 switch(enemy[e].type)
 {
  case ENEMY_BOSS1:
   run_boss1(e);
   break;
  case ENEMY_BOSS2:
   run_boss2(e);
   break;
  case ENEMY_BOSS3:
   run_boss3(e);
   break;
  case ENEMY_BOSS4:
   run_boss4(e);
   break;

 }

}


void run_boss1(int e)
{

 boss.phase_count --;
 boss.recycle --;
 if (boss.recycle < 0)
  boss.recycle = 0;

 if (boss.phase_count <= 0)
 {
  switch(boss.phase)
  {
   case 0:
    boss.phase = 1;
    boss.phase_count = 20;
    break;
   case 1:
    boss.phase = 2;
    boss.phase_count = 20;
    break;
   case 2:
    if (boss.hp > (boss.max_hp / 4) * 3
        || grand(4) == 0)
     boss.phase = 3;
      else
      {
       boss.phase = 6 + grand(2);
      }
//    boss.phase = 7;
    boss.phase_count = 300;
    break;
   case 3:
    boss.phase = 4;
    boss.phase_count = 20;
    break;
   case 4:
    boss.phase = 5;
    boss.phase_count = 3000;
    enemy[e].x_target [0] = 150000 + grand(390000);
    enemy[e].y_target [0] = 120000 + grand(80000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    enemy[e].x_speed = 0;
    enemy[e].y_speed = 0;
    break;
// 5 is move and has its own finishing conditions
   case 6:
    boss.phase = 4;
    boss.phase_count = 20;
    break;
   case 7:
    boss.phase = 4;
    boss.phase_count = 20;
    break;


  }


 }

 switch(boss.phase)
 {
  case 0: // entering;
   boss.recycle ++;
   break;
  case 1: // unfold 1
   drag_enemy(e, 950);
   arrange_boss1(0, 1, 0, 0);
   boss.recycle ++;
   boss1_turrets(e, 0);
   break;
  case 2: // unfold 2
   drag_enemy(e, 950);
   arrange_boss1(1, 1, 1, -1);
   boss.recycle ++;
   boss1_turrets(e, 0);
   break;
  case 3: // basic attack
   drag_enemy(e, 950);
   arrange_boss1(1, 1, 1, -1);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_STOPPER1_FIRE])
      {
       synch_done [DONE_STOPPER1_FIRE] |= FIRE1_BOSS1_1;
       boss1_attack1(e, boss.status);
       boss.recycle = 170 - arena.difficulty * 50;
      }
   }
   boss1_turrets(e, 1);
   break;
  case 4: // refold
   drag_enemy(e, 950);
   arrange_boss1(-1, -1, -1, -1);
   boss.recycle ++;
   boss1_turrets(e, 0);
   break;
  case 5: // move
   drag_enemy(e, 950);
   boss.recycle ++;
   arrange_boss1(-1, -1, -1, -1);
    enemy[e].x_speed += xpart(enemy[e].angle, 250);
    enemy[e].y_speed += ypart(enemy[e].angle, 250);
    if (enemy[e].x > enemy[e].x_target [0] - 20000
     && enemy[e].x < enemy[e].x_target [0] + 20000
     && enemy[e].y > enemy[e].y_target [0] - 20000
     && enemy[e].y < enemy[e].y_target [0] + 20000)
     {
      boss.phase = 1;
      boss.phase_count = 20;
      boss.recycle = 1;
     }
   boss1_turrets(e, 0);
   break;
  case 6: // attack2
   drag_enemy(e, 950);
   arrange_boss1(1, 1, 1, 1);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_SWOOPER1_FIRE])
      {
       synch_done [DONE_SWOOPER1_FIRE] |= FIRE1_DARTER2;
       boss1_attack2(e, boss.status);
       boss.recycle = 30 - arena.difficulty * 7;
      }
   }
   boss1_turrets(e, 1);
   break;
  case 7: // attack3
   drag_enemy(e, 950);
   arrange_boss1(1, 1, -1, 1);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_STOPPER1_FIRE])
      {
       synch_done [DONE_STOPPER1_FIRE] |= FIRE1_BOSS1_1;
       boss1_attack3(e, boss.status);
       boss.recycle = 170 - arena.difficulty * 50;
      }
   }
   boss1_turrets(e, 1);
   break;





 }

    move_enemy(e);

}


void boss1_attack1(int e, int stat)
{

 int i, angle;
 angle = 0;
 int angle1;
 int xa;

 switch(stat)
 {
  case 0:
   angle = grand(ANGLE_1);
   for (i = 0; i < 32; i ++)
   {
    angle += ANGLE_32;
    fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, 80, 50 + abs(xpart(angle * 2, 30)));
    fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle + ANGLE_64, EBCOL_B5, 100, 30 + abs(xpart((angle + ANGLE_64) * 2, 10)));
   }
   boss.status = 1;
   break;
  case 1:
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 32; i ++)
   {
    angle += ANGLE_32;
    fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, 80, 50 + ypart(angle * 2 + angle1, 30));
    fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle + ANGLE_64, EBCOL_B5, 100, 30 + abs(ypart((angle + ANGLE_64) * 2 + angle1, 10)));
   }
   boss.status = grand(4);
   break;
  case 2:
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 32; i ++)
   {
    angle += ANGLE_32;
    fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, 80, 50 + ypart(angle * 2 + angle1, 15) + abs(xpart(angle * 4, 15)));
    fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle + ANGLE_64, EBCOL_B5, 100, 30 + abs(ypart((angle + ANGLE_64) * 2 + angle1 * 4, 10)) + abs(xpart(angle * 4, 10)));
   }
   boss.status = grand(4);
   break;
  case 3:
   angle1 = grand(ANGLE_1);
   xa = 8;
   if (grand(2) == 0)
    xa = 4;
   for (i = 0; i < 32; i ++)
   {
    angle += ANGLE_32;
    fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, 80, 50 + ypart(angle * 2 + angle1, 10) + abs(xpart(angle * xa, 25)));
    fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle + ANGLE_64, EBCOL_B5, 100, 30 + abs(ypart((angle + ANGLE_64) * 2 + angle1 * 4, 5)) + abs(xpart(angle * xa, 15)));
   }
   boss.status = grand(4);
   break;

 }

}


void boss1_attack2(int e, int stat)
{

 int i, angle;
 angle = 0;


 angle = grand(ANGLE_1);

 for (i = 0; i < 3 + arena.difficulty; i ++)
 {
    fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B5, 80, 60 + i * 20);
    angle += ANGLE_3;
    fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B5, 80, 60 + i * 20);
    angle += ANGLE_3;
    fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B5, 80, 60 + i * 20);
    angle -= ANGLE_3;
    angle -= ANGLE_3;
    angle += ANGLE_32;

 }


}

void boss1_attack3(int e, int stat)
{

 int i, angle;
 angle = 0;
 int angle1;

 angle = grand(ANGLE_1);

 angle1 = ANGLE_1 / 10;

stat= 3;
 switch(stat)
 {
   case 0:
    for (i = 0; i < 10; i ++)
    {
     fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B4, 80, 30);
     fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B4, 80, 80);
     fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle + ANGLE_32, EBCOL_B4, 80, 45);
     fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle - ANGLE_32, EBCOL_B4, 80, 45);
     fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle + ANGLE_64, EBCOL_B4, 80, 65);
     fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle - ANGLE_64, EBCOL_B4, 80, 65);
     fire_diamond(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B5, 1600, 0, 80);

     angle += angle1;
    }
    boss.status = grand(3);
    break;
   case 1:
    if (arena.difficulty == 2)
    {
     for (i = 0; i < 128; i ++)
     {
      if ((i & 4) ^ (i & 5))
       fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B5, 60, 60 + xpart(i * 8, 40));
        else
         fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, 60, 45 + xpart(i * 8, 20));
       angle += ANGLE_64;
     }
    }
     else
      {
       for (i = 0; i < 64; i ++)
       {
        if ((i & 4) ^ (i & 5))
         fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B5, 60, 60 + xpart(i * 16, 40));
          else
           fire_dart(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, 60, 45 + xpart(i * 16, 20));
         angle += ANGLE_32;
       }
      }
    boss.status = grand(3);
    break;
    case 3:
    if (arena.difficulty == 2)
    {
       for (i = 0; i < 20; i ++)
       {
        fire_diamond(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B2, 2200 + xpart(i * 25, 600), xpart(i * 25, 8), 80);
        fire_diamond(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle + ANGLE_1 / 51, EBCOL_B1, 2000 + xpart(i * 25, 600), xpart(i * 25 + ANGLE_2, 12), 40);
        fire_diamond(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle + ANGLE_1 / 51, EBCOL_B1, 2400 + xpart(i * 25, 600), xpart(i * 25 + ANGLE_2, 12), 40);
        angle += ANGLE_1 / 20;
       }
    }
     else
     {
       for (i = 0; i < 12; i ++)
       {
        fire_diamond(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B2, 2200 + xpart(i * 21, 600), xpart(i * 21, 8), 80);
        fire_diamond(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle + ANGLE_1 / 21, EBCOL_B1, 2000 + xpart(i * 21, 600), xpart(i * 21 + ANGLE_2, 12), 40);
        fire_diamond(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle + ANGLE_1 / 21, EBCOL_B1, 2400 + xpart(i * 21, 600), xpart(i * 21 + ANGLE_2, 12), 40);
        angle += ANGLE_1 / 12;
       }

     }
       boss.status = grand(3);
       break;

 }


}


// ignores fire setting when badly hurt
void boss1_turrets(int e, int fire)
{

 int angle, tangle, i;

 if (boss.t_recycle [0] > 0)
  boss.t_recycle [0] --;

  int firing;

 if (boss.hp < boss.max_hp / 3)
 {
  boss.t_angle [0] -= 6;
  boss.t_angle [0] &= 1023;
  boss.t_angle [1] += 6;
  boss.t_angle [1] &= 1023;

      if (boss.t_recycle [0] <= 0)
      {
        if (synch_ready [EVENT_SWOOPER1_FIRE])
        {
           firing = boss.t_status;
           if (boss.t_status == 1)
            boss.t_status = 0;
             else
              boss.t_status = 1;
            synch_done [DONE_SWOOPER1_FIRE] |= FIRE1_BOSS1_2;
            if (firing == 0)
            {

             fire_dart(enemy[e].x - 71000 - boss.out_sides2 * 1000 + xpart(boss.t_angle [0], 13000), enemy[e].y + ypart(boss.t_angle [0], 13000), boss.t_angle [0], EBCOL_B2, 50, 60 + arena.difficulty * 20);
             fire_dart(enemy[e].x + 71000 + boss.out_sides2 * 1000 + xpart(boss.t_angle [1], 13000), enemy[e].y + ypart(boss.t_angle [1], 13000), boss.t_angle [1], EBCOL_B2, 50, 60 + arena.difficulty * 20);

             fire_dart(enemy[e].x - 71000 - boss.out_sides2 * 1000 + xpart(boss.t_angle [0] + ANGLE_2, 13000), enemy[e].y + ypart(boss.t_angle [0] + ANGLE_2, 13000), boss.t_angle [0] + ANGLE_2, EBCOL_B2, 50, 60 + arena.difficulty * 20);
             fire_dart(enemy[e].x + 71000 + boss.out_sides2 * 1000 + xpart(boss.t_angle [1] + ANGLE_2, 13000), enemy[e].y + ypart(boss.t_angle [1] + ANGLE_2, 13000), boss.t_angle [1] + ANGLE_2, EBCOL_B2, 50, 60 + arena.difficulty * 20);
/*             fire_diamond(enemy[e].x - 71000 - boss.out_sides2 * 1000 + xpart(boss.t_angle [0], 13000), enemy[e].y + ypart(boss.t_angle [0], 13000), boss.t_angle [0], EBCOL_B2, 1900 + arena.difficulty * 300, 0, 50);
             fire_diamond(enemy[e].x + 71000 + boss.out_sides2 * 1000 + xpart(boss.t_angle [1], 13000), enemy[e].y + ypart(boss.t_angle [1], 13000), boss.t_angle [1], EBCOL_B2, 1900 + arena.difficulty * 300, 0, 50);

             fire_diamond(enemy[e].x - 71000 - boss.out_sides2 * 1000 + xpart(boss.t_angle [0] + ANGLE_2, 13000), enemy[e].y + ypart(boss.t_angle [0] + ANGLE_2, 13000), boss.t_angle [0] + ANGLE_2, EBCOL_B2, 1900 + arena.difficulty * 300, 0, 50);
             fire_diamond(enemy[e].x + 71000 + boss.out_sides2 * 1000 + xpart(boss.t_angle [1] + ANGLE_2, 13000), enemy[e].y + ypart(boss.t_angle [1] + ANGLE_2, 13000), boss.t_angle [1] + ANGLE_2, EBCOL_B2, 1900 + arena.difficulty * 300, 0, 50);*/


            }
              else
              {
               fire_dart(enemy[e].x + 71000 + boss.out_sides2 * 1000 + xpart(boss.t_angle [1], 13000), enemy[e].y + ypart(boss.t_angle [1], 13000), boss.t_angle [1], EBCOL_B1, 50, 40 + arena.difficulty * 15);
               fire_dart(enemy[e].x - 71000 - boss.out_sides2 * 1000 + xpart(boss.t_angle [0], 13000), enemy[e].y + ypart(boss.t_angle [0], 13000), boss.t_angle [0], EBCOL_B1, 50, 40 + arena.difficulty * 15);

               fire_dart(enemy[e].x + 71000 + boss.out_sides2 * 1000 + xpart(boss.t_angle [1] + ANGLE_2, 13000), enemy[e].y + ypart(boss.t_angle [1] + ANGLE_2, 13000), boss.t_angle [1] + ANGLE_2, EBCOL_B1, 50, 40 + arena.difficulty * 15);
               fire_dart(enemy[e].x - 71000 - boss.out_sides2 * 1000 + xpart(boss.t_angle [0] + ANGLE_2, 13000), enemy[e].y + ypart(boss.t_angle [0] + ANGLE_2, 13000), boss.t_angle [0] + ANGLE_2, EBCOL_B1, 50, 40 + arena.difficulty * 15);
/*               fire_diamond(enemy[e].x + 71000 + boss.out_sides2 * 1000 + xpart(boss.t_angle [1], 13000), enemy[e].y + ypart(boss.t_angle [1], 13000), boss.t_angle [1], EBCOL_B1, 1300 + arena.difficulty * 250, 0, 50);
               fire_diamond(enemy[e].x - 71000 - boss.out_sides2 * 1000 + xpart(boss.t_angle [0], 13000), enemy[e].y + ypart(boss.t_angle [0], 13000), boss.t_angle [0], EBCOL_B1, 1300 + arena.difficulty * 250, 0, 50);

               fire_diamond(enemy[e].x + 71000 + boss.out_sides2 * 1000 + xpart(boss.t_angle [1] + ANGLE_2, 13000), enemy[e].y + ypart(boss.t_angle [1] + ANGLE_2, 13000), boss.t_angle [1] + ANGLE_2, EBCOL_B1, 1300 + arena.difficulty * 250, 0, 50);
               fire_diamond(enemy[e].x - 71000 - boss.out_sides2 * 1000 + xpart(boss.t_angle [0] + ANGLE_2, 13000), enemy[e].y + ypart(boss.t_angle [0] + ANGLE_2, 13000), boss.t_angle [0] + ANGLE_2, EBCOL_B1, 1300 + arena.difficulty * 250, 0, 50);*/
              }
            boss.t_recycle [0] = 5 - arena.difficulty;
            return;
           }

      }

  return;
 }


  for (i = 0; i < 2; i ++)
 {


  angle = boss.t_angle [i];

  if (i == 0)
  {
   tangle =
    radians_to_angle(atan2((player.y - enemy[e].y), (player.x - (enemy[e].x - 71000 - boss.out_sides2 * 1000))));
  }
   else
    tangle =
     radians_to_angle(atan2((player.y - enemy[e].y), (player.x - (enemy[e].x + 71000 + boss.out_sides2 * 1000))));

   if (tangle < 0)
    tangle += ANGLE_1;
   if (tangle > ANGLE_1)
    tangle -= ANGLE_1;

   if ((angle < tangle && tangle > angle + ANGLE_2)
     || (angle > tangle && tangle > angle - ANGLE_2))
   {
    boss.t_angle_turning [i] -= 2;
    if (boss.t_angle_turning [i] < -8)
     boss.t_angle_turning [i] = -8;
   }
    else
    {
     boss.t_angle_turning [i] += 2;
     if (boss.t_angle_turning [i] > 8)
      boss.t_angle_turning [i] = 8;
    }

   boss.t_angle [i] += boss.t_angle_turning [i];
   boss.t_angle [i] &= 1023;

  }

   if (fire == 0)
    return;

      if (boss.t_recycle [0] <= 0)
      {
        if (synch_ready [EVENT_SWOOPER1_FIRE])
        {
           firing = boss.t_status;
           if (boss.t_status == 1)
            boss.t_status = 0;
             else
              boss.t_status = 1;
           if (boss.hp < (boss.max_hp / 3) * 2)
           {
            synch_done [DONE_SWOOPER1_FIRE] |= FIRE1_SWOOPER2;
            if (firing == 0)
            {
             fire_diamond(enemy[e].x - 71000 - boss.out_sides2 * 1000 + xpart(boss.t_angle [0], 13000), enemy[e].y + ypart(boss.t_angle [0], 13000), boss.t_angle [0], EBCOL_B2, 1700 + arena.difficulty * 300, 0, 50);
             fire_diamond(enemy[e].x - 71000 - boss.out_sides2 * 1000 + xpart(boss.t_angle [0], 13000), enemy[e].y + ypart(boss.t_angle [0], 13000), boss.t_angle [0] - ANGLE_32, EBCOL_B2, 1600 + arena.difficulty * 250, -8, 50);
             fire_diamond(enemy[e].x - 71000 - boss.out_sides2 * 1000 + xpart(boss.t_angle [0], 13000), enemy[e].y + ypart(boss.t_angle [0], 13000), boss.t_angle [0] + ANGLE_32, EBCOL_B2, 1600 + arena.difficulty * 250, 8, 50);
            }
              else
              {
               fire_diamond(enemy[e].x + 71000 + boss.out_sides2 * 1000 + xpart(boss.t_angle [1], 13000), enemy[e].y + ypart(boss.t_angle [1], 13000), boss.t_angle [1], EBCOL_B2, 1700 + arena.difficulty * 300, 0, 50);
               fire_diamond(enemy[e].x + 71000 + boss.out_sides2 * 1000 + xpart(boss.t_angle [1], 13000), enemy[e].y + ypart(boss.t_angle [1], 13000), boss.t_angle [1] - ANGLE_32, EBCOL_B2, 1600 + arena.difficulty * 250, 8, 50);
               fire_diamond(enemy[e].x + 71000 + boss.out_sides2 * 1000 + xpart(boss.t_angle [1], 13000), enemy[e].y + ypart(boss.t_angle [1], 13000), boss.t_angle [1] + ANGLE_32, EBCOL_B2, 1600 + arena.difficulty * 250, -8, 50);
              }
            boss.t_recycle [0] = 20 - arena.difficulty * 5;
            return;
           }
            synch_done [DONE_SWOOPER1_FIRE] |= FIRE1_SWOOPER1;
            if (firing == 0)
             fire_dart(enemy[e].x - 71000 - boss.out_sides2 * 1000 + xpart(boss.t_angle [0], 13000), enemy[e].y + ypart(boss.t_angle [0], 13000), boss.t_angle [0], EBCOL_B1, 50, 80 + arena.difficulty * 10);
              else
               fire_dart(enemy[e].x + 71000 + boss.out_sides2 * 1000 + xpart(boss.t_angle [1], 13000), enemy[e].y + ypart(boss.t_angle [1], 13000), boss.t_angle [1], EBCOL_B1, 50, 80 + arena.difficulty * 10);
            boss.t_recycle [0] = 20 - arena.difficulty * 5;
            return;
        }
      }

// may have returned by here!

}

void arrange_boss1(int sides, int sides2, int up, int out)
{
 if (sides > 0)
 {
   if ((boss.phase_count & 1) && boss.out_sides < 10)
    boss.out_sides ++;
 }
 if (sides < 0)
 {
   if ((boss.phase_count & 1) && boss.out_sides > 0)
    boss.out_sides --;
 }
 if (sides2 > 0)
 {
   if (boss.out_sides2 < 25)
    boss.out_sides2 ++;
 }
 if (sides2 < 0)
 {
   if (boss.out_sides2 > 0)
    boss.out_sides2 --;
 }
 if (up > 0)
 {
   if ((boss.phase_count & 1) && boss.out_up < 10)
    boss.out_up ++;
 }
 if (up < 0)
 {
   if ((boss.phase_count & 1) && boss.out_up > 0)
    boss.out_up --;
 }
 if (out > 0)
 {
   if (boss.out_out_up < 18)
    boss.out_out_up ++;
 }
 if (out < 0)
 {
   if (boss.out_out_up > 0)
    boss.out_out_up --;
 }


}

/*

****************************************************************

BOSS2

****************************************************************

*/

void run_boss2(int e)
{

 quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y + 74000, enemy[e].x_speed + grand(500) - grand(500), enemy[e].y_speed + 3500 + grand(2500), 8 + grand(8), TCOL_ORANGE, 0);
 quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y + 85000, enemy[e].x_speed + grand(500) - grand(500), enemy[e].y_speed + 3500 + grand(2500), 17 + grand(17), TCOL_SH2, 0);

 boss.phase_count --;
 boss.recycle --;
 if (boss.recycle < 0)
  boss.recycle = 0;

 if (boss.phase_count <= 0)
 {
  switch(boss.phase)
  {
   case 0:
    boss.phase = 1;
    boss.phase_count = 20;
    break;
   case 1:
    boss.phase = 3;
    boss.phase_count = 400;
    boss.status = 0;
    break;
   case 2:
    boss.phase = 2 + grand(3);
    boss.phase_count = 400;
    boss.status = 0;
    break;
   case 3:
    boss.phase = 2 + grand(3);
    boss.phase_count = 400;
    boss.status = 0;
    break;
   case 4:
    boss.phase = 2 + grand(3);
    boss.phase_count = 400;
    boss.status = 0;
    break;

  }


 }

 if (boss.hp < boss.max_hp / 4)
 {
  boss.phase = 5;
  boss.phase_count = 300;

 }

 if (boss.phase > 1)
 {

   drag_enemy(e, 960);
   enemy[e].x_speed += xpart(enemy[e].angle, 250);
   enemy[e].y_speed += ypart(enemy[e].angle, 250);
   if (enemy[e].x > enemy[e].x_target [0] - 40000
     && enemy[e].x < enemy[e].x_target [0] + 40000
     && enemy[e].y > enemy[e].y_target [0] - 40000
     && enemy[e].y < enemy[e].y_target [0] + 40000)
     {
      enemy[e].x_target [0] = 150000 + grand(390000);
      enemy[e].y_target [0] = 120000 + grand(80000);
      enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
     }
   if ((boss.phase_count & 32)
     && (enemy[e].x < 80000
     || enemy[e].x > 560000
     || enemy[e].y < 80000
     || enemy[e].y > 350000))
     {
      enemy[e].x_target [0] = 150000 + grand(390000);
      enemy[e].y_target [0] = 120000 + grand(80000);
      enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
     }

 }

 switch(boss.phase)
 {
  case 0: // entering;
   boss.recycle ++;
   break;
  case 1: // unfold 1
   drag_enemy(e, 950);
   arrange_boss2(1, 0);
   boss.recycle ++;
   boss2_turrets(e, 0);
   break;
  case 2: // basic attack
   drag_enemy(e, 960);
   arrange_boss2(1, -1);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_FIRE2_2])
      {
       synch_done [DONE_FIRE2_2] |= FIRE2_BOSS2_1;
       boss2_attack1(e, boss.status);
       boss.recycle = 170 - arena.difficulty * 50;
      }
   }
   boss2_turrets(e, 1);
   break;
  case 3: // attack2
   drag_enemy(e, 960);
   arrange_boss2(-1, 1);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_FIRE2_1])
      {
       synch_done [DONE_FIRE2_1] |= FIRE2_DRIFTER1;
       boss2_attack2(e, boss.status);
       boss.recycle = 30 - arena.difficulty * 7;
      }
   }
   boss2_turrets(e, 1);
   break;
  case 4: // basic attack
   drag_enemy(e, 960);
   arrange_boss2(1, 1);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_FIRE2_2])
      {
       synch_done [DONE_FIRE2_2] |= FIRE2_BOSS2_2;
       boss2_attack3(e, boss.status);
       boss.recycle = 170 - arena.difficulty * 50;
      }
   }
   boss2_turrets(e, 1);
   break;
  case 5: // attack2
   drag_enemy(e, 940);
   arrange_boss2(1, 1);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_FIRE2_1])
      {
       synch_done [DONE_FIRE2_1] |= FIRE2_DRIFTER1;
       boss2_attack4(e, boss.status);
       boss.recycle = 20 - arena.difficulty * 5;
      }
   }
   boss2_turrets(e, 1);
   break;

 }

    move_enemy(e);

}



void arrange_boss2(int sides, int sides2)
{
 if (sides > 0)
 {
   if ((boss.phase_count & 3) && boss.out_sides < 4)
    boss.out_sides ++;
 }
 if (sides < 0)
 {
   if ((boss.phase_count & 3) && boss.out_sides > 0)
    boss.out_sides --;
 }
 if (sides2 > 0)
 {
   if ((boss.phase_count & 1) && boss.out_sides2 < 4)
    boss.out_sides2 ++;
 }
 if (sides2 < 0)
 {
   if ((boss.phase_count & 1) && boss.out_sides2 > 0)
    boss.out_sides2 --;
 }


}



void boss2_attack1(int e, int stat)
{

 int i, angle;
 angle = 0;
 int angle1;

 switch(stat)
 {
  case 0:
   angle = grand(ANGLE_1);
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 32; i ++)
   {
    angle += ANGLE_32;
    angle1 += ANGLE_32;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, 9, 3000 + xpart(angle1 * 6, 1000), 7);
//    fire_dot(enemy[e].x + xpart(angle, 15000) + xpart(angle - ANGLE_3, 25000), enemy[e].y + ypart(angle, 15000) + ypart(angle - ANGLE_3, 25000), angle, EBCOL_B2, 3, 3000 + xpart(angle * 8, 1000), 7);
//    fire_dot(enemy[e].x + xpart(angle, 15000) + xpart(angle + ANGLE_3, 25000), enemy[e].y + ypart(angle, 15000) + ypart(angle + ANGLE_3, 25000), angle, EBCOL_B2, 3, 3000 + xpart(angle * 8, 1000), 7);
//    fire_dot(enemy[e].x + xpart(angle, 40000), enemy[e].y + ypart(angle, 30000), angle, EBCOL_B2, 3, 3000 + xpart(angle * 8, 1000), 7);
    fire_dot(enemy[e].x + xpart(angle, 1000), enemy[e].y + ypart(angle, 1000), angle, EBCOL_B3, 4, 3000 + xpart(angle1 * 6, 1000), 7);
   }
   boss.status = 1;
   break;
  case 1:
   angle = grand(ANGLE_1);
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 32; i ++)
   {
    angle += ANGLE_32;
    angle1 += ANGLE_32;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, 9, 3000 + xpart(angle1 * 6, 1000), 7);
    fire_dot(enemy[e].x + xpart(angle, 1000), enemy[e].y + ypart(angle, 1000), angle, EBCOL_B3, 4, 3000 + xpart(angle1 * 6, 1000), 7);
   }
   angle = grand(ANGLE_1);
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 32; i ++)
   {
    angle += ANGLE_32;
    angle1 += ANGLE_32;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B2, 9, 2000 + xpart(angle1 * 6, 600), 7);
    fire_dot(enemy[e].x + xpart(angle, 1000), enemy[e].y + ypart(angle, 1000), angle, EBCOL_B2, 4, 2000 + xpart(angle1 * 6, 600), 7);
   }
   boss.status = 2;
   break;
  case 2:
   angle = grand(ANGLE_1);
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 32; i ++)
   {
    angle += ANGLE_32;
    angle1 += ANGLE_32;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, 9, 3000 + xpart(angle1 * 6, 1000), 7);
    fire_dot(enemy[e].x + xpart(angle, 1000), enemy[e].y + ypart(angle, 1000), angle, EBCOL_B3, 4, 3000 + xpart(angle1 * 6, 1000), 7);
   }
   angle = grand(ANGLE_1);
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 32; i ++)
   {
    angle += ANGLE_32;
    angle1 += ANGLE_32;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B2, 9, 2000 + xpart(angle1 * 6, 600), 7);
    fire_dot(enemy[e].x + xpart(angle, 1000), enemy[e].y + ypart(angle, 1000), angle, EBCOL_B2, 4, 2000 + xpart(angle1 * 6, 600), 7);
   }
   angle = grand(ANGLE_1);
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 32; i ++)
   {
    angle += ANGLE_32;
    angle1 += ANGLE_32;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B1, 9, 1000 + xpart(angle1 * 6, 300), 7);
    fire_dot(enemy[e].x + xpart(angle, 1000), enemy[e].y + ypart(angle, 1000), angle, EBCOL_B1, 4, 1000 + xpart(angle1 * 6, 300), 7);
   }
   boss.status = 2;
   break;
 }

}


void boss2_attack2(int e, int stat)
{


 int i, j, angle;
 angle = 0;

 angle = grand(ANGLE_1);

 for (j = 0; j < 5; j ++)
 {
  for (i = 0; i < 4 + arena.difficulty; i ++)
  {
    fire_dot(enemy[e].x + xpart(angle, i * 11000), enemy[e].y + ypart(angle, i * 11000), angle, EBCOL_B5, 7, 2000 + i * 400, 7);
    if (i > 0)
    {
        if (i & 1)
     fire_dot(enemy[e].x + xpart(angle, i * 11000) + xpart(angle - ANGLE_3, 19000), enemy[e].y + ypart(angle, i * 11000) + ypart(angle - ANGLE_3, 19000), angle, EBCOL_B3, 4, 1800 + i * 400, 1);
      else
     fire_dot(enemy[e].x + xpart(angle, i * 11000) + xpart(angle + ANGLE_3, 19000), enemy[e].y + ypart(angle, i * 11000) + ypart(angle + ANGLE_3, 19000), angle, EBCOL_B3, 4, 1800 + i * 400, 1);
    }
  }
    angle += ANGLE_5;
    angle &= 1023;

 }




}

#define ANGLE_48 21
#define ANGLE_96 42
#define B2_SIZE1 8
#define B2_SIZE2 6

void boss2_attack3(int e, int stat)
{

 int i, angle;
 angle = 0;
 int angle1;

 switch(stat)
 {
  case 0:
   angle = grand(ANGLE_4) - ANGLE_4 - ANGLE_8;
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 48; i ++)
   {
    angle += ANGLE_48;
    angle1 += ANGLE_48;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, B2_SIZE1 + xpart(angle1 * 4, B2_SIZE2), 4000, 7);
   }
   boss.status = 1;
   break;
  case 1:
   angle = grand(ANGLE_4) - ANGLE_4 - ANGLE_8;
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 48; i ++)
   {
    angle += ANGLE_48;
    angle1 += ANGLE_96;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, B2_SIZE1 + xpart(angle1 * 4, B2_SIZE2), 4000, 7);
   }
   angle = grand(ANGLE_4) - ANGLE_4 - ANGLE_8;
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 48; i ++)
   {
    angle += ANGLE_48;
    angle1 += ANGLE_96;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B2, B2_SIZE1 + xpart(angle1 * 4, B2_SIZE2), 3000, 7);
   }
   boss.status = 2;
   break;
  case 2:
   angle = grand(ANGLE_4) - ANGLE_4 - ANGLE_8;
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 48; i ++)
   {
    angle += ANGLE_48;
    angle1 += ANGLE_96;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, B2_SIZE1 + xpart(angle1 * 4, B2_SIZE2), 4000, 7);
   }
   angle = grand(ANGLE_4) - ANGLE_4 - ANGLE_8;
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 48; i ++)
   {
    angle += ANGLE_48;
    angle1 += ANGLE_96;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B2, B2_SIZE1 + xpart(angle1 * 4, B2_SIZE2), 3000, 7);
   }
   angle = grand(ANGLE_4) - ANGLE_4 - ANGLE_8;
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 48; i ++)
   {
    angle += ANGLE_48;
    angle1 += ANGLE_96;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B1, B2_SIZE1 + xpart(angle1 * 4, B2_SIZE2), 2000, 7);
   }
   boss.status = 2;
   break;
 }

}



void boss2_attack4(int e, int stat)
{


 int i, angle;
 angle = 0;

  for (i = 0; i < 12 + arena.difficulty * 4; i ++)
  {
    angle = grand(ANGLE_1);
    fire_dot(enemy[e].x + xpart(angle, 3000), enemy[e].y + ypart(angle, 3000), angle, EBCOL_B1 + grand(3), 5 + grand(3), 2000 + grand(3000), 2 + grand(7));
  }


}


void boss2_turrets(int e, int fire)
{

 int angle, tangle, i;

 if (boss.t_recycle [0] > 0)
  boss.t_recycle [0] --;

  int firing;


 for (i = 0; i < 4; i ++)
 {
  if (boss.t_beam [i] > 0)
   continue;

  angle = boss.t_angle [i];

  if (i == 0)
  {
   tangle =
    radians_to_angle(atan2((player.y - enemy[e].y), (player.x - (enemy[e].x - 71000 - boss.out_sides2 * 1000))));
  }
   else
    tangle =
     radians_to_angle(atan2((player.y - enemy[e].y), (player.x - (enemy[e].x + 71000 + boss.out_sides2 * 1000))));

    tangle &= 1023;

   if ((angle < tangle && tangle > angle + ANGLE_2)
     || (angle > tangle && tangle > angle - ANGLE_2))
   {
    boss.t_angle_turning [i] -= 2;
    if (boss.t_angle_turning [i] < -8)
     boss.t_angle_turning [i] = -8;
   }
    else
    {
     boss.t_angle_turning [i] += 2;
     if (boss.t_angle_turning [i] > 8)
      boss.t_angle_turning [i] = 8;
    }

   boss.t_angle [i] += boss.t_angle_turning [i];
   boss.t_angle [i] &= 1023;

  }

  int x, y, a;


   for (i = 0; i < 4; i ++)
   {
       if (boss.t_beam [i] == 0)
        continue;

           a = boss.t_angle [i];

           switch(i)
           {
            case 0:
             x = enemy[e].x - 53000;
             y = enemy[e].y + 40000;
             break;
            case 1:
             x = enemy[e].x + 32000;
             y = enemy[e].y - 20000;
             break;
            case 2:
             x = enemy[e].x + 53000;
             y = enemy[e].y + 40000;
             break;
            case 3:
             x = enemy[e].x - 32000;
             y = enemy[e].y - 20000;
             break;
           }
           x += xpart(a, 7000);
           y += ypart(a, 7000);

           boss2_beam(e, 1, x, y, a, boss.t_beam [i]);


           boss.t_beam [i] --;
           if (boss.t_beam [i] == 90)
            boss.t_beam [i] = 30;

           if (arena.difficulty < 2 && (arena.counter & 1) == 0
               && (boss.t_beam [i] < 20 || boss.t_beam [i] > 100))
                boss.t_beam [i] ++;

           if (arena.difficulty == 2 && (arena.counter & 3) == 0
               && (boss.t_beam [i] < 20 || boss.t_beam [i] > 100))
                boss.t_beam [i] ++;

   }

   if (fire == 0)
    return;


      if (boss.t_recycle [0] <= 0)
      {

       if (boss.hp < boss.max_hp / 4 && boss.phase == 5)
       {
        if (synch_ready [EVENT_FIRE2_2])
        {
           firing = boss.t_status;
           boss.t_status += boss.t_status_change;
           if (boss.t_status == 4)
            boss.t_status = 0;
           if (boss.t_status == -1)
            boss.t_status = 3;
           if (grand(20) == 0)
            boss.t_status_change *= -1;

           if (boss.t_beam [firing] > 0)
            return;

           boss.t_beam [firing] = 120;

           synch_done [DONE_FIRE2_2] |= FIRE2_BEAMER;
        }

        return;
       }




        if (synch_ready [EVENT_FIRE2_1])
        {
           firing = boss.t_status;
           boss.t_status += boss.t_status_change;
           if (boss.t_status == 4)
            boss.t_status = 0;
           if (boss.t_status == -1)
            boss.t_status = 3;
           if (grand(20) == 0)
            boss.t_status_change *= -1;

           a = boss.t_angle [firing];

           switch(firing)
           {
            case 0:
             x = enemy[e].x - 53000;
             y = enemy[e].y + 40000;
             break;
            case 1:
             x = enemy[e].x + 32000;
             y = enemy[e].y - 20000;
             break;
            case 2:
             x = enemy[e].x + 53000;
             y = enemy[e].y + 40000;
             break;
            case 3:
             x = enemy[e].x - 32000;
             y = enemy[e].y - 20000;
             break;
           }
           x += xpart(a, 7000);
           y += ypart(a, 7000);

           int which = 0;

           if (boss.hp < (boss.max_hp / 4) * 3)
            which = 1;

           if (boss.hp < (boss.max_hp / 2))
            which = 2;

           switch(which)
           {
            default:
             synch_done [DONE_FIRE2_1] |= FIRE2_DRIFTER1;
             fire_dot(x + xpart(a, 15000), y + ypart(a, 15000), a, EBCOL_B5, 8, 4000, 0);
             fire_dot(x, y, a, EBCOL_B5, 3, 3900, 5);
             boss.t_recycle [0] = 20 - arena.difficulty * 5;
             synch_done [DONE_FIRE2_1] |= FIRE2_DRIFTER1;
             break;
            case 1:
             synch_done [DONE_FIRE2_1] |= FIRE2_DRIFTER1;
             fire_dot(x, y, a, EBCOL_B5, 10, 3000, 5);
             fire_dot(x, y, a + ANGLE_32, EBCOL_B4, 7, 3000, 0);
             fire_dot(x, y, a - ANGLE_32, EBCOL_B4, 7, 3000, 0);
             fire_dot(x, y, a + ANGLE_16, EBCOL_B3, 4, 3000, 0);
             fire_dot(x, y, a - ANGLE_16, EBCOL_B3, 4, 3000, 0);
             boss.t_recycle [0] = 35 - arena.difficulty * 7;
             synch_done [DONE_FIRE2_1] |= FIRE2_DRIFTER1;
             break;
            case 2:
             for (i = 0; i < 8 + arena.difficulty * 2; i ++)
             {
              a = boss.t_angle [firing] + grand(ANGLE_32) - grand(ANGLE_32);
              fire_dot(x, y, a, EBCOL_B3, 8 + grand(4), 2000 + grand(1000), 0);
              fire_dot(x, y, a, EBCOL_B2, 5 + grand(4), 2000 + grand(1000), 0);
              fire_dot(x, y, a, EBCOL_B1, 2 + grand(4), 2000 + grand(1000), 0);
             }
             boss.t_recycle [0] = 60 - arena.difficulty * 10;
             synch_done [DONE_FIRE2_1] |= FIRE2_DRIFTER1;
             break;
           }
           return;
        }
      }

// may have returned by here!

}



int boss2_beam(int e, int dist, int x1, int y1, int angle, int stat)
{
 int length = 1;
 int x = x1, y = y1;

 do
 {
  length ++;
  x += xpart(angle, 45000);
  y += ypart(angle, 45000);
  if (x > 690000
   || x < -50000
   || y > 530000
   || y < - 50000)
    break;
 } while(TRUE);
 int b = create_ebullet(EBULLET_BEAM);
 if (b != -1)
 {
  angle &= 1023;
  if (stat > 100 || stat < 20)
  {
    ebullet[b].x = x1 + xpart(angle, dist + 4000);
    ebullet[b].y = y1 + ypart(angle, dist + 4000);
  }
   else
   {
    ebullet[b].x = x1 + xpart(angle, dist + 4000 - (stat % 6) * GRAIN * 4);
    ebullet[b].y = y1 + ypart(angle, dist + 4000 - (stat % 6) * GRAIN * 4);
   }
  ebullet[b].status = length;
  ebullet[b].angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = x1 + xpart(angle, dist);
  ebullet[b].y_speed = y1 + ypart(angle, dist);
  ebullet[b].timeout = 2;
  ebullet[b].angle2 = stat;
  if (stat > 100)
   ebullet[b].angle2 = 120 - stat;
  return b;
 }
 return -1;
}



/*

****************************************************************

BOSS3

****************************************************************

*/

void run_boss3(int e)
{

 boss.phase_count --;
 boss.recycle --;
 if (boss.recycle < 0)
  boss.recycle = 0;

 if (boss.phase_count <= 0)
 {
  switch(boss.phase)
  {
   case 0:
    boss.phase = 1;
    boss.phase_count = 50;
    break;
   case 1:
    enemy[e].y_speed = 0;
    enemy[e].x_speed = 0;
    boss.phase2 = 1;
    boss3_finish_firing(e);
    break;
   case 2:
   case 3:
   case 4:
   case 5:
   case 6:
    boss3_finish_firing(e);
    break;

  }


 }

/* if (boss.hp < boss.max_hp / 4)
 {
  boss.phase = 5;
  boss.phase_count = 300;

  }*/
 int i;

 for (i = 0; i < 3; i ++)
 {
     boss.petal_angle [i] += boss.petal_speed [i];
     boss.petal_angle [i] &= 1023;
 }

 if (boss.phase > 1)
 {

//   enemy[e].x_speed += xpart(enemy[e].angle, 250);
//   enemy[e].y_speed += ypart(enemy[e].angle, 250);

 }

 boss.next_small --;

 if (boss.next_small <= 0)
 {
  boss.next_small = 80 - arena.difficulty * 15;
  if (grand(2))
   create_boss3_dancer(ENEMY_SMALL);
    else
     create_boss3_dancer(ENEMY_SMALL2);
 }

 switch(boss.phase)
 {
  case 0: // entering;
   boss.recycle ++;
   break;
  case 1: // unfold 1
   drag_enemy(e, 960);
//   arrange_boss3(1, 0);
   arrange_boss3(-8, 220, 7, 90, -3, 150);
   boss.recycle ++;
   break;
   case 2: // movement
   drag_enemy(e, 960);
   arrange_boss3(8, 180, -8, 200, -6, 160);
   enemy[e].x_speed += xpart(enemy[e].angle, 100);
   enemy[e].y_speed += ypart(enemy[e].angle, 100);
   if (enemy[e].y < 50 && boss.phase_count > 5)
    boss.phase_count = 4;
   break;
   case 3: // attack1
   arrange_boss3(-4, 70, 6, 180, 3, 120);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_FIRE3_1])
      {
       synch_done [DONE_FIRE3_1] |= FIRE3_BOSS3_1;
       boss3_attack1(e, boss.status);
       boss.recycle = 20 - arena.difficulty * 4;
      }
   }
   drag_enemy(e, 960);
   break;
   case 4: // attack2
   arrange_boss3(3, 60, -4, 80, 4, 100);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_FIRE3_1])
      {
       synch_done [DONE_FIRE3_1] |= FIRE3_BOSS3_2;
       boss3_attack2(e, boss.status);
       boss.recycle = 20 - arena.difficulty * 4;
      }
   }
   drag_enemy(e, 960);
   break;
   case 5: // attack3
   arrange_boss3(-6, 120, 2, 110, 5, 170);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_FIRE3_2])
      {
       synch_done [DONE_FIRE3_2] |= FIRE3_BOSS3_3;
       boss3_attack3(e, boss.status);
       boss.recycle = 20 - arena.difficulty * 4;
      }
   }
   drag_enemy(e, 960);
   break;
   case 6: // attack4
   arrange_boss3(2, 180, 4, 120, -8, 70);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_FIRE3_2])
      {
       synch_done [DONE_FIRE3_2] |= FIRE3_BOSS3_4;
       boss3_attack4(e, boss.status);
       boss.recycle = 40 - arena.difficulty * 9;
      }
   }
   drag_enemy(e, 960);
   break;
/*  case 2: // basic attack
//   arrange_boss2(1, -1);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_FIRE2_2])
      {
       synch_done [DONE_FIRE2_2] |= FIRE2_BOSS2_1;
       boss2_attack1(e, boss.status);
       boss.recycle = 170 - arena.difficulty * 50;
      }
   }
   boss2_turrets(e, 1);
   break;
  case 3: // attack2
   drag_enemy(e, 960);
   arrange_boss2(-1, 1);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_FIRE2_1])
      {
       synch_done [DONE_FIRE2_1] |= FIRE2_DRIFTER1;
       boss2_attack2(e, boss.status);
       boss.recycle = 30 - arena.difficulty * 7;
      }
   }
   boss2_turrets(e, 1);
   break;
  case 4: // basic attack
   drag_enemy(e, 960);
   arrange_boss2(1, 1);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_FIRE2_2])
      {
       synch_done [DONE_FIRE2_2] |= FIRE2_BOSS2_2;
       boss2_attack3(e, boss.status);
       boss.recycle = 170 - arena.difficulty * 50;
      }
   }
   boss2_turrets(e, 1);
   break;
  case 5: // attack2
   drag_enemy(e, 940);
   arrange_boss2(1, 1);
   if (boss.recycle <= 0)
   {
      if (synch_ready [EVENT_FIRE2_1])
      {
       synch_done [DONE_FIRE2_1] |= FIRE2_DRIFTER1;
       boss2_attack4(e, boss.status);
       boss.recycle = 20 - arena.difficulty * 5;
      }
   }
   break;*/

 }

    move_enemy(e);

}


void boss3_attack1(int e, int stat)
{


 int i, angle;
 angle = 0;
 int angle1;

 angle = grand(ANGLE_1);
 angle1 = ANGLE_2 + ANGLE_4;

  for (i = 0; i < 8; i ++)
  {
    angle1 += ANGLE_16;
    angle1 &= 1023;
    fire_dart(enemy[e].x + xpart(angle, 9000), enemy[e].y + ypart(angle, 9000), angle, EBCOL_B3, 80, 50 + xpart(angle1, 5 + boss.status));
    angle += ANGLE_2;
    fire_dart(enemy[e].x + xpart(angle, 9000), enemy[e].y + ypart(angle, 9000), angle, EBCOL_B5, 80, 50 + xpart(angle1, 5 + boss.status));
    angle -= ANGLE_2;
    angle += ANGLE_32;
    angle &= 1023;
  }

 boss.status += 5;

 if (boss.status > 35)
  boss.status = 35;

}

void boss3_attack2(int e, int stat)
{


 int i, angle;
 angle = 0;

 angle = grand(ANGLE_1);

  for (i = 0; i < 5; i ++)
  {
    fire_ddart(enemy[e].x + xpart(angle, 9000), enemy[e].y + ypart(angle, 9000), angle, EBCOL_B5, 60, 90);
    fire_dart(enemy[e].x + xpart(angle, 9000), enemy[e].y + ypart(angle, 9000), angle + ANGLE_32, EBCOL_B4, 50, 80);
    fire_dart(enemy[e].x + xpart(angle, 9000), enemy[e].y + ypart(angle, 9000), angle - ANGLE_32, EBCOL_B4, 50, 80);
    fire_sdart(enemy[e].x + xpart(angle, 9000), enemy[e].y + ypart(angle, 9000), angle + ANGLE_16, EBCOL_B3, 40, 70);
    fire_sdart(enemy[e].x + xpart(angle, 9000), enemy[e].y + ypart(angle, 9000), angle - ANGLE_16, EBCOL_B3, 40, 70);

    angle += ANGLE_5;
    angle &= 1023;
  }


}

void boss3_attack3(int e, int stat)
{


 int i, angle;
 angle = 0;
 int angle1;

// if (boss.status

 angle = boss.status;

  for (i = 0; i < 4; i ++)
  {
    angle1 = angle;
    if (arena.difficulty > 0)
     fire_ddart(enemy[e].x + xpart(angle1, 9000), enemy[e].y + ypart(angle1, 9000), angle1, EBCOL_B5, 50, 90);
    angle1 += ANGLE_16;
    fire_dart(enemy[e].x + xpart(angle1, 9000), enemy[e].y + ypart(angle1, 9000), angle1, EBCOL_B4, 50, 80);
    angle1 += ANGLE_16;
    fire_sdart(enemy[e].x + xpart(angle1, 9000), enemy[e].y + ypart(angle1, 9000), angle1, EBCOL_B3, 40, 70);
    angle += ANGLE_4;
    angle &= 1023;
  }

 boss.status += 30;
 boss.status &= 1023;

}

void boss3_attack4(int e, int stat)
{

  int i, angle;

  for (i = 0; i < 4 + arena.difficulty; i ++)
  {
    angle = grand(ANGLE_1);
    fire_seed2(enemy[e].x + xpart(angle, 9000), enemy[e].y + ypart(angle, 9000), grand(ANGLE_1), EBCOL_B5, 18 + grand(20), 2000 + grand(2500));
  }


}



void boss3_finish_firing(int e)
{

    if (boss.phase == 2)
     boss.phase2 = 2 + grand(2);

    boss.phase2 --;

    if (boss.phase2 <= 0)
    {
      boss.phase = 2;
      boss.phase_count = 40 + grand(40);
      if (enemy[e].x <= 320000 && enemy[e].y <= 100000)
       enemy[e].angle = grand(ANGLE_16);
      if (enemy[e].x > 320000 && enemy[e].y <= 100000)
       enemy[e].angle = ANGLE_2 - grand(ANGLE_16);
      if (enemy[e].x <= 320000 && enemy[e].y > 100000)
       enemy[e].angle = ANGLE_2 + ANGLE_4 + ANGLE_8 + ANGLE_16 + grand(ANGLE_16);
      if (enemy[e].x > 320000 && enemy[e].y > 100000)
       enemy[e].angle = grand(ANGLE_16) + ANGLE_2;
      return;
    }

    boss.phase = 3 + grand(4);
    boss.phase_count = 200 + grand(100);
    boss.status = 0;

    if (boss.phase == 5)
     boss.status = grand(ANGLE_1);

}

void arrange_boss3(int speed1, int out1, int speed2, int out2, int speed3, int out3)
{

 if (boss.petal_speed [0] > speed1)
  boss.petal_speed [0] --;
 if (boss.petal_speed [0] < speed1)
  boss.petal_speed [0] ++;
 if (boss.petal_speed [1] > speed2)
  boss.petal_speed [1] --;
 if (boss.petal_speed [1] < speed2)
  boss.petal_speed [1] ++;
 if (boss.petal_speed [2] > speed3)
  boss.petal_speed [2] --;
 if (boss.petal_speed [2] < speed3)
  boss.petal_speed [2] ++;

 if (boss.petal_out [0] > out1)
  boss.petal_out [0] -= 1;
 if (boss.petal_out [0] < out1)
  boss.petal_out [0] += 1;
 if (boss.petal_out [1] > out2)
  boss.petal_out [1] -= 1;
 if (boss.petal_out [1] < out2)
  boss.petal_out [1] += 1;
 if (boss.petal_out [2] > out3)
  boss.petal_out [2] -= 1;
 if (boss.petal_out [2] < out3)
  boss.petal_out [2] += 1;

}




void run_boss4(int e)
{

 boss.phase_count --;
 boss.recycle --;
 if (boss.recycle < 0)
  boss.recycle = 0;

 if (boss.phase_count <= 0)
 {
     switch(boss.phase)
     {
        case 0:
         boss.phase = 1;
         break;

     }
 }


if (boss.phase > 0)
 {

   drag_enemy(e, 920);
   enemy[e].x_speed += xpart(enemy[e].angle, 200);
   enemy[e].y_speed += ypart(enemy[e].angle, 200);
   if (enemy[e].x > enemy[e].x_target [0] - 40000
     && enemy[e].x < enemy[e].x_target [0] + 40000
     && enemy[e].y > enemy[e].y_target [0] - 40000
     && enemy[e].y < enemy[e].y_target [0] + 40000)
     {
      enemy[e].x_target [0] = 100000 + grand(440000);
      enemy[e].y_target [0] = 120000 + grand(180000);
      enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
     }
   if ((boss.phase_count & 32)
     && (enemy[e].x < 80000
     || enemy[e].x > 560000
     || enemy[e].y < 80000
     || enemy[e].y > 350000))
     {
      enemy[e].x_target [0] = 100000 + grand(440000);
      enemy[e].y_target [0] = 120000 + grand(180000);
      enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
     }

 }


 if (boss.phase > 0)
 {
     run_boss4_spin(e);

     if (boss.spin_size < 12)
      boss.spin_size ++;
     if (boss.spin_rad < 135)
      boss.spin_rad ++;
     boss.spin_angle += 4;
     boss.spin_angle &= 1023;

     if (boss.hp < boss.max_hp / 2)
     {
      if (boss.spin_size2 < 12)
       boss.spin_size2 ++;
      if (boss.spin_rad2 < 135)
       boss.spin_rad2 ++;
      boss.spin_counter ++;
      boss.spin_counter &= 1023;
      boss.spin_angle2 += xpart(boss.spin_counter * 2, 8);
      boss.spin_angle2 &= 1023;

      if (boss.hp < boss.max_hp / 3)
      {

       if (boss.spin_size3 < 7)
        boss.spin_size3 ++;
       if (boss.spin_rad3 < 85)
        boss.spin_rad3 ++;
      }

     }

 }



 switch(boss.phase)
 {
  case 0:
   if (enemy[e].y > 100000)
   {
    boss.phase_count = 1;
   }
   break;
   case 1:
    boss4_sides_fire(e);
    boss4_outer_fire(e);
    break;
 }

    move_enemy(e);

}


void run_boss4_spin(int e)
{

     int b = create_ebullet(EBULLET_SPIN);
     if (b != -1)
     {
      ebullet[b].x = enemy[e].x - 70000;
      ebullet[b].y = enemy[e].y - 61000;
      ebullet[b].x_speed = boss.spin_size;
//      if (ebullet[b].x_speed > 6)
//       ebullet[b].x_speed = 6;
      ebullet[b].y_speed = 5;
      ebullet[b].status = boss.spin_rad;
      ebullet[b].angle = boss.spin_angle;
      ebullet[b].timeout = 2;
      ebullet[b].colour = TCOL_B3;
      ebullet[b].accel = TCOL_B2;
      ebullet[b].cord_colour = TRANS_B2_OUT;
     }

     b = create_ebullet(EBULLET_SPIN);
     if (b != -1)
     {
      ebullet[b].x = enemy[e].x + 70000;
      ebullet[b].y = enemy[e].y - 61000;
      ebullet[b].x_speed = boss.spin_size;
//      if (ebullet[b].x_speed > 6)
//       ebullet[b].x_speed = 6;
      ebullet[b].y_speed = 5;
      ebullet[b].status = boss.spin_rad;
      ebullet[b].angle = ANGLE_1 - boss.spin_angle;
      ebullet[b].timeout = 2;
      ebullet[b].colour = TCOL_B3;
      ebullet[b].accel = TCOL_B2;
      ebullet[b].cord_colour = TRANS_B2_OUT;
     }

     if (boss.hp > boss.max_hp / 2)
      return;

     b = create_ebullet(EBULLET_SPIN);
     if (b != -1)
     {
      ebullet[b].x = enemy[e].x;
      ebullet[b].y = enemy[e].y + 25000;
      ebullet[b].x_speed = boss.spin_size2;
//      if (ebullet[b].x_speed > 6)
//       ebullet[b].x_speed = 6;
      ebullet[b].y_speed = 3;
      ebullet[b].status = boss.spin_rad2;
//      if (ebullet[b].status > 100)
//       ebullet[b].status = 100;
      ebullet[b].angle = boss.spin_angle2;
      ebullet[b].timeout = 2;
      ebullet[b].colour = TCOL_B4;
      ebullet[b].accel = TCOL_B3;
      ebullet[b].cord_colour = TRANS_B3_OUT;
     }

     if (boss.hp > boss.max_hp / 3)
      return;

 int i;
 int angle = boss.spin_angle2;

 for (i = 0; i < 3; i ++)
 {

     b = create_ebullet(EBULLET_SPIN);
     if (b != -1)
     {
      ebullet[b].x = enemy[e].x + xpart(angle, boss.spin_rad2 * GRAIN);
      ebullet[b].y = enemy[e].y + 25000 + ypart(angle, boss.spin_rad2 * GRAIN);
      ebullet[b].x_speed = boss.spin_size3;
      ebullet[b].y_speed = 3;
      ebullet[b].status = boss.spin_rad3;
      ebullet[b].angle = ANGLE_1 - boss.spin_angle2;
      ebullet[b].timeout = 2;
      ebullet[b].colour = TCOL_B2;
      ebullet[b].accel = TCOL_B1;
      ebullet[b].cord_colour = TRANS_B1_OUT;
     }
     angle += ANGLE_3;
 }

}


void boss4_outer_fire(int e)
{

 if (boss.recycle > 0)
  return;

 int angle = grand(ANGLE_1);
 int i;

 switch(boss.outer_fire_type)
 {
  default:
  case 0:
   if (synch_ready [EVENT_FIRE4_2])
   {
    synch_done [DONE_FIRE4_2] |= FIRE4_WALKER;
    for (i = 0; i < 6; i ++)
    {
       fire_dart(enemy[e].x - 70000 + xpart(angle, 10000), enemy[e].y - 61000 + ypart(angle, 10000), angle, EBCOL_B2, 50, 90);
       fire_curve(enemy[e].x - 70000 + xpart(angle, 10000), enemy[e].y - 61000 + ypart(angle, 10000), angle + ANGLE_12 / 2, EBCOL_B3, 3000, xpart(angle * 2, 8), 5);
       angle += ANGLE_2;
       fire_dart(enemy[e].x + 70000 + xpart(angle, 10000), enemy[e].y - 61000 + ypart(angle, 10000), angle, EBCOL_B2, 50, 90);
       fire_curve(enemy[e].x + 70000 + xpart(angle, 10000), enemy[e].y - 61000 + ypart(angle, 10000), angle + ANGLE_12 / 2, EBCOL_B3, 3000, xpart(angle * 2, 8), 5);
       angle += ANGLE_6;
       angle -= ANGLE_2;
       angle &= 1023;
     }
    boss.recycle = 120 - arena.difficulty * 10;
    boss.outer_fire_count --;
   }
   break;
  case 1:
   if (synch_ready [EVENT_FIRE4_1])
   {
    synch_done [DONE_FIRE4_1] |= FIRE4_DROPPER;
    angle = attack_angle(e);
    i = pos_or_neg(4 + grand(4));
    fire_curve(enemy[e].x - 70000 + xpart(angle, 10000), enemy[e].y - 61000 + ypart(angle, 10000), angle, EBCOL_B1, 3000, i, 5);
    i *= -1;
    fire_curve(enemy[e].x + 70000 + xpart(angle, 10000), enemy[e].y - 61000 + ypart(angle, 10000), angle, EBCOL_B1, 3000, i, 5);
     boss.recycle = 40 - arena.difficulty * 10;
     boss.outer_fire_count --;
   }
   break;

 }


 if (boss.outer_fire_count <= 0)
 {
  if (boss.outer_fire_type == 0)
   boss.outer_fire_type = 1;
    else
     boss.outer_fire_type = 0;
  boss.outer_fire_count = 10 + grand(5);
 }


}

void boss4_sides_fire(int e)
{

 boss.sides_recycle --;
 if (boss.sides_recycle > 0)
  return;

 int angle = grand(ANGLE_1);


 if (boss.hp < (boss.max_hp / 3))
 {
  if (synch_ready [EVENT_FIRE4_2])
  {
   synch_done [DONE_FIRE4_2] |= FIRE4_HUNTER;

   fire_tracker(enemy[e].x - 77000, enemy[e].y - 8000, ANGLE_2 + ANGLE_64, EBCOL_B2, 4, 500);
   fire_tracker(enemy[e].x - 80000, enemy[e].y + 24000, ANGLE_2 - ANGLE_32, EBCOL_B2, 4, 500);
   fire_tracker(enemy[e].x - 68000, enemy[e].y + 50000, ANGLE_2 - ANGLE_16, EBCOL_B2, 4, 500);
   fire_tracker(enemy[e].x - 45000, enemy[e].y + 70000, ANGLE_2 - ANGLE_8, EBCOL_B2, 4, 500);


   fire_tracker(enemy[e].x + 77000, enemy[e].y - 8000, ANGLE_1 - ANGLE_64, EBCOL_B2, 4, 500);
   fire_tracker(enemy[e].x + 80000, enemy[e].y + 24000, ANGLE_32, EBCOL_B2, 4, 500);
   fire_tracker(enemy[e].x + 68000, enemy[e].y + 50000, ANGLE_16, EBCOL_B2, 4, 500);
   fire_tracker(enemy[e].x + 45000, enemy[e].y + 70000, ANGLE_8, EBCOL_B2, 4, 500);
   boss.sides_recycle = 80;// - arena.difficulty * 15;
  }
  return;
 }



 if (boss.hp < (boss.max_hp / 3) * 2)
 {
  if (synch_ready [EVENT_FIRE4_1])
  {
   synch_done [DONE_FIRE4_1] |= FIRE4_HOOK;

   fire_ddart(enemy[e].x - 77000, enemy[e].y - 8000, angle - 60, EBCOL_B2, 70, 80);
   fire_ddart(enemy[e].x - 80000, enemy[e].y + 24000, angle - 40, EBCOL_B2, 70, 77);
   fire_ddart(enemy[e].x - 68000, enemy[e].y + 50000, angle - 20, EBCOL_B2, 70, 74);
   fire_ddart(enemy[e].x - 45000, enemy[e].y + 70000, angle, EBCOL_B2, 70, 70);
   angle = ANGLE_1 - angle + ANGLE_2;

   fire_ddart(enemy[e].x + 77000, enemy[e].y - 8000, angle + 60, EBCOL_B2, 70, 90);
   fire_ddart(enemy[e].x + 80000, enemy[e].y + 24000, angle + 40, EBCOL_B2, 70, 77);
   fire_ddart(enemy[e].x + 68000, enemy[e].y + 50000, angle + 20, EBCOL_B2, 70, 74);
   fire_ddart(enemy[e].x + 45000, enemy[e].y + 70000, angle, EBCOL_B2, 70, 70);
   boss.sides_recycle = 40 - arena.difficulty * 10;
  }
  return;
 }




 if (synch_ready [EVENT_FIRE4_1])
 {
  synch_done [DONE_FIRE4_1] |= FIRE4_HOOK2;

  fire_dart(enemy[e].x - 77000, enemy[e].y - 8000, angle + 60, EBCOL_B5, 70, 50);
  fire_dart(enemy[e].x - 80000, enemy[e].y + 24000, angle + 40, EBCOL_B5, 70, 50);
  fire_dart(enemy[e].x - 68000, enemy[e].y + 50000, angle + 20, EBCOL_B5, 70, 50);
  fire_dart(enemy[e].x - 45000, enemy[e].y + 70000, angle, EBCOL_B5, 70, 50);
  angle = ANGLE_1 - angle + ANGLE_2;

  fire_dart(enemy[e].x + 77000, enemy[e].y - 8000, angle - 60, EBCOL_B5, 70, 50);
  fire_dart(enemy[e].x + 80000, enemy[e].y + 24000, angle - 40, EBCOL_B5, 70, 50);
  fire_dart(enemy[e].x + 68000, enemy[e].y + 50000, angle - 20, EBCOL_B5, 70, 50);
  fire_dart(enemy[e].x + 45000, enemy[e].y + 70000, angle, EBCOL_B5, 70, 50);
  boss.sides_recycle = 40 - arena.difficulty * 10;
 }

}


// RANK - remember boss.hp!!!!!!!!!!!!!!!!!!!!!

/*
void boss2_attack3(int e, int stat)
{

 int i, angle;
 angle = 0;
 int angle1;
 int xa;

 switch(stat)
 {
  case 0:
   angle = grand(ANGLE_1);
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 64; i ++)
   {
    angle += ANGLE_64;
    angle1 += ANGLE_64;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, 7 + xpart(angle1 * 6, 6), 4000, 7);
   }
   boss.status = 1;
   break;
  case 1:
   angle = grand(ANGLE_1);
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 64; i ++)
   {
    angle += ANGLE_128;
    angle1 += ANGLE_128;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, 7 + xpart(angle1 * 6, 6), 4000, 7);
    angle += ANGLE_128;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B2, 7 + xpart(angle1 * 6, 6), 2500, 7);
   }
   boss.status = 2;
   break;
  case 2:
   angle = grand(ANGLE_1);
   angle1 = grand(ANGLE_1);
   for (i = 0; i < 64; i ++)
   {
    angle += ANGLE_128;
    angle1 += ANGLE_128;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B3, 7 + xpart(angle1 * 6, 6), 4000, 7);
    angle += 4;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B2, 7 + xpart(angle1 * 6, 6), 2500, 7);
    angle += 4;
    fire_dot(enemy[e].x + xpart(angle, 19000), enemy[e].y + ypart(angle, 19000), angle, EBCOL_B1, 7 + xpart(angle1 * 6, 6), 1300, 7);
   }
   boss.status = 2;
   break;
 }

}
*/



void move_dswooper(int e)
{

 enemy[e].recycle --;
 if (enemy[e].recycle == 0)
 {
  if (enemy[e].y < 470000)
  {
//   create_cloud(w, CLOUD_EXPLODE, 0, enemy[e].x, enemy[e].y, enemy[e].x_speed, enemy[e].y_speed, 14 + grand(5));
   enemy_fire(e);
  }
  enemy[e].recycle = 100;
 }

     switch(enemy[e].type)
     {
     }


 if (enemy [e].target_time > 0 && enemy [e].x_target [enemy [e].target] != -1)
  enemy [e].target_time --;
   else
   {
    if (enemy [e].x_target [enemy [e].target] != -1)
    {
     enemy [e].target_time = enemy [e].max_target_time;
     enemy [e].target ++;
     enemy[e].recycle = 30;
    }
   }


    if (enemy [e].x / 15000 == enemy [e].x_target [enemy [e].target] / 15000
      && enemy [e].y / 15000 == enemy [e].y_target [enemy [e].target] / 15000)
    {
     if (enemy [e].x_target [enemy [e].target] != -1)
     {
      enemy [e].target_time = enemy [e].max_target_time;
      enemy [e].target ++;
//      enemy[e].recycle = 30;
     }
    }

    if (enemy [e].x_target [enemy [e].target] != -1 && enemy [e].y < 470000)
    {
     track_target(e, enemy [e].x_target [enemy [e].target], enemy [e].y_target [enemy [e].target], enemy [e].angle1, eclass[enemy [e].type].speed2);
     enemy [e].angle1 &= 1023;
    }
    enemy [e].x_speed = xpart (enemy [e].angle1, eclass[enemy [e].type].speed1);
    enemy [e].y_speed = ypart (enemy [e].angle1, eclass[enemy [e].type].speed1);

    if (enemy[e].x_target [enemy[e].target] == -1)
     enemy[e].persist = 0;

    move_enemy(e);

}



void move_dancer(int e)
{

 char ready_to_fire = 0;
 int angle;

 if (enemy[e].recycle > 0)
  enemy[e].recycle --;
 if (enemy[e].recycle <= 0)
 {
  if (enemy[e].y < 470000)
  {
//   create_cloud(w, CLOUD_EXPLODE, 0, enemy[e].x, enemy[e].y, enemy[e].x_speed, enemy[e].y_speed, 14 + grand(5));
//   enemy_fire(e);
     ready_to_fire = 1;
  }
 }

     switch(enemy[e].type)
     {
      case ENEMY_SWOOPER2:
       if (ready_to_fire)
       {
        if (synch_ready [EVENT_SWOOPER1_FIRE])
        {
         angle = attack_angle(e);
         fire_dart(enemy[e].x, enemy[e].y, angle, EBCOL_B2, 50, 100);
//         fire_dart(enemy[e].x, enemy[e].y, angle + ANGLE_32, EBCOL_B5, 50, 80);
//         fire_dart(enemy[e].x, enemy[e].y, angle - ANGLE_32, EBCOL_B5, 50, 80);
         enemy[e].recycle = 140 - arena.difficulty * 50 + grand(20);
         synch_done [DONE_SWOOPER1_FIRE] |= FIRE1_SWOOPER2;
        }
       }
       enemy_pulse(e, EVENT_PULSE);
       break;
      case ENEMY_SWOOPER1:
       if (ready_to_fire)
       {
        if (synch_ready [EVENT_SWOOPER1_FIRE])
        {
         enemy_fire(e);
         enemy[e].recycle = 160 - arena.difficulty * 30;
         synch_done [DONE_SWOOPER1_FIRE] |= FIRE1_SWOOPER1;
        }
       }
       enemy_pulse(e, EVENT_PULSE);
       break;
      case ENEMY_SMALL:
       if (ready_to_fire)
       {
        if (synch_ready [EVENT_FIRE3_1])
//           && (synch_done [DONE_FIRE3_1] & FIRE3_SMALL) == 0)
        {
         enemy_fire(e);
         enemy[e].recycle = 50 - arena.difficulty * 20;
         synch_done [DONE_FIRE3_1] |= FIRE3_SMALL;
        }
       }
       petal_turn(e, 0, enemy[e].angle1 + ANGLE_2, 3, 8);
       petal_turn(e, 1, enemy[e].angle1 + ANGLE_2 - ANGLE_6, 3, 8);
       petal_turn(e, 2, enemy[e].angle1 + ANGLE_2 + ANGLE_6, 3, 8);
//       petal_turn(e, 1, enemy[e].angle1 - ANGLE_3, 2, 8);
//       petal_turn(e, 2, enemy[e].angle1 + ANGLE_3, 2, 8);

       enemy_pulse(e, EVENT_PULSE1);
       break;
      case ENEMY_SMALL2:
       if (ready_to_fire)
       {
        if (synch_ready [EVENT_FIRE3_2])
        {
         fire_dart(enemy[e].x, enemy[e].y, attack_angle(e), EBCOL_B4, 30, 110 + arena.difficulty * 10);
         enemy[e].recycle = 30 - arena.difficulty * 10;
         synch_done [DONE_FIRE3_2] |= FIRE3_SMALL2;
        }
       }
       petal_turn(e, 0, enemy[e].angle1 + ANGLE_2, 4, 12);
       petal_turn(e, 1, enemy[e].angle1 + ANGLE_2 - ANGLE_6, 4, 12);
       petal_turn(e, 2, enemy[e].angle1 + ANGLE_2 + ANGLE_6, 4, 12);
//       petal_turn(e, 1, enemy[e].angle1 - ANGLE_3, 2, 8);
//       petal_turn(e, 2, enemy[e].angle1 + ANGLE_3, 2, 8);

       enemy_pulse(e, EVENT_PULSE1);
       break;
      case ENEMY_HOOK:
       if (ready_to_fire)
       {
        if (synch_ready [EVENT_FIRE4_1])
        {
         fire_curve(enemy[e].x - 8000, enemy[e].y, attack_angle(e), EBCOL_B2, 4000, pos_or_neg(3 + grand(4)), 2);
         fire_curve(enemy[e].x + 8000, enemy[e].y, attack_angle(e), EBCOL_B2, 4000, pos_or_neg(3 + grand(4)), 2);
         enemy[e].recycle = 160 - arena.difficulty * 30;
         synch_done [DONE_FIRE4_1] |= FIRE4_HOOK;
        }
       }
       enemy_pulse_slow(e, EVENT_PULSE);
       break;
      case ENEMY_HOOK2:
       if (ready_to_fire)
       {
        if (synch_ready [EVENT_FIRE4_1])
        {
         fire_curve(enemy[e].x - 11000, enemy[e].y, attack_angle(e), EBCOL_B5, 4600, pos_or_neg(5 + grand(4)), 2);
         fire_curve(enemy[e].x + 11000, enemy[e].y, attack_angle(e), EBCOL_B5, 4600, pos_or_neg(5 + grand(4)), 2);
         enemy[e].recycle = 150 - arena.difficulty * 35;
         synch_done [DONE_FIRE4_1] |= FIRE4_HOOK2;
        }
       }
       enemy_pulse_slow(e, EVENT_PULSE);
       break;
     }


 if (enemy [e].target_time > 0 && enemy [e].x_target [enemy [e].target] != -1)
  enemy [e].target_time --;
   else
   {
    if (enemy [e].x_target [enemy [e].target] != -1)
    {
     enemy [e].target_time = enemy [e].max_target_time;
     enemy [e].target ++;
     enemy[e].recycle = 30;
    }
   }


    if (enemy [e].x / 15000 == enemy [e].x_target [enemy [e].target] / 15000
      && enemy [e].y / 15000 == enemy [e].y_target [enemy [e].target] / 15000)
    {
     if (enemy [e].x_target [enemy [e].target] != -1)
     {
      enemy [e].target_time = enemy [e].max_target_time;
      enemy [e].target ++;
//      enemy[e].recycle = 30;
     }
    }

    if (enemy [e].x_target [enemy [e].target] != -1 && enemy [e].y < 470000)
    {
     track_target(e, enemy [e].x_target [enemy [e].target], enemy [e].y_target [enemy [e].target], enemy [e].angle1, eclass[enemy [e].type].speed2);
     enemy [e].angle1 %= ANGLE_1;
    }
    enemy [e].x_speed = xpart (enemy [e].angle1, eclass[enemy [e].type].speed1);
    enemy [e].y_speed = ypart (enemy [e].angle1, eclass[enemy [e].type].speed1);

    move_enemy(e);

    if (enemy[e].x_target [enemy[e].target] == -1)
     enemy[e].persist = 0;


}

void petal_turn(int e, int p, int target_angle, int accel, int max_speed)
{

 target_angle &= 1023;

 if ((enemy[e].petal_angle [p] < target_angle && target_angle > enemy[e].petal_angle [p] + ANGLE_2)
     || (enemy[e].petal_angle [p] > target_angle && target_angle > enemy[e].petal_angle [p] - ANGLE_2))
 {
  enemy[e].petal_angle_speed [p] -= accel;
  if (enemy[e].petal_angle_speed [p] < max_speed * -1)
   enemy[e].petal_angle_speed [p] = max_speed * -1;
  enemy[e].petal_angle [p] += enemy[e].petal_angle_speed [p];
  enemy[e].petal_angle [p] &= 1023;
  return;
 }

  enemy[e].petal_angle_speed [p] += accel;
  if (enemy[e].petal_angle_speed [p] > max_speed)
   enemy[e].petal_angle_speed [p] = max_speed;
  enemy[e].petal_angle [p] += enemy[e].petal_angle_speed [p];
  enemy[e].petal_angle [p] &= 1023;

}

void enemy_pulse(int e, int p_event)
{
       if (synch_ready [p_event] == 1)
        enemy[e].pulse2 = 1;
       if (enemy[e].pulse2 == 1) // outwards
       {
        enemy[e].pulse1 += 2;
        if (enemy[e].pulse1 >= 3)
        {
            enemy[e].pulse1 = 3;
            enemy[e].pulse2 = 2;
        }
       }
        else
        {
          if (enemy[e].pulse2 == 2 && arena.counter % 2 == 0) // inwards
          {
           enemy[e].pulse1 --;
           if (enemy[e].pulse1 <= 0)
           {
               enemy[e].pulse1 = 0;
               enemy[e].pulse2 = 0;
           }
          }
        }
}

void enemy_pulse_slow(int e, int p_event)
{
       if (synch_ready [p_event] == 1)
        enemy[e].pulse2 = 1;
       if (enemy[e].pulse2 == 1) // outwards
       {
        enemy[e].pulse1 += 2;
        if (enemy[e].pulse1 >= 3)
        {
            enemy[e].pulse1 = 3;
            enemy[e].pulse2 = 2;
        }
       }
        else
        {
          if (enemy[e].pulse2 == 2 && arena.counter % 4 == 0) // inwards
          {
           enemy[e].pulse1 --;
           if (enemy[e].pulse1 <= 0)
           {
               enemy[e].pulse1 = 0;
               enemy[e].pulse2 = 0;
           }
          }
        }
}



void move_darter(int e)
{
 enemy[e].target_time --;
 if (enemy[e].recycle > 0)
  enemy[e].recycle --;

 if (enemy[e].target_time <= 0)
 {
  switch(enemy[e].phase)
  {
   case 0: // diving in first
    enemy[e].phase = 1; // opening
    enemy[e].target_time = 30; // also below
    break;
   case 1: // opening
    enemy[e].phase = 2; // shooting
    enemy[e].x_speed = 0;
    enemy[e].y_speed = 0;
    enemy[e].target_time = 90;
    break;
   case 2: // shooting
    enemy[e].phase = 3; // closing
    enemy[e].target_time = 30;
    break;
   case 3: // closing
    if (enemy[e].phase2 >= 3)
    {
     enemy[e].phase = 5; // departing
     enemy[e].target_time = 1000; // important! see below
     enemy[e].persist = 0;
     break;
    }
    enemy[e].phase = 4; // moving
    enemy[e].phase2 ++;
    enemy[e].target_time = 1000; // important! see below
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = RAND_Y_IN;
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    enemy[e].x_speed = 0;
    enemy[e].y_speed = 0;
    break;

  }
 }
    track_target(e, player.x, player.y, enemy[e].angle1, 8);

  switch(enemy[e].phase)
  {
   case 2: // shooting
    switch(enemy[e].type)
    {
     case ENEMY_DARTER1:
      if (enemy[e].recycle <= 0)
      {
        if (synch_ready [EVENT_SWOOPER1_FIRE])
        {
           synch_done [DONE_SWOOPER1_FIRE] |= FIRE1_DARTER1;
           fire_dart(enemy[e].x + xpart(enemy[e].angle1, 7000), enemy[e].y + ypart(enemy[e].angle1, 7000), enemy[e].angle1, EBCOL_B3, 50, 80 + arena.difficulty * 10);
           enemy[e].recycle = 40 - arena.difficulty * 10;
        }
      }
      break;
     case ENEMY_CARRIER1_PART:
      enemy_pulse(e, EVENT_PULSE);
      if (enemy[e].recycle <= 0)
      {
        if (synch_ready [EVENT_SWOOPER1_FIRE])
        {
           synch_done [DONE_SWOOPER1_FIRE] |= FIRE1_CARRIER_PART;
           fire_dart(enemy[e].x + xpart(enemy[e].angle1, 7000), enemy[e].y + ypart(enemy[e].angle1, 7000), enemy[e].angle1, EBCOL_B5, 50, 80 + arena.difficulty * 10);
           enemy[e].recycle = 30 - arena.difficulty * 10;
        }
      }
      break;
     case ENEMY_DARTER2:
      enemy_pulse(e, EVENT_PULSE);
      if (enemy[e].recycle <= 0)
      {
        if (synch_ready [EVENT_SWOOPER1_FIRE])
        {
           synch_done [DONE_SWOOPER1_FIRE] |= FIRE1_DARTER2;
           fire_dart(enemy[e].x + xpart(enemy[e].angle1, 7000), enemy[e].y + ypart(enemy[e].angle1, 7000), enemy[e].angle1, EBCOL_B4, 50, 90 + arena.difficulty * 15);
           fire_dart(enemy[e].x + xpart(enemy[e].angle1, 7000), enemy[e].y + ypart(enemy[e].angle1, 7000), enemy[e].angle1 - ANGLE_32, EBCOL_B5, 50, 80 + arena.difficulty * 10);
           fire_dart(enemy[e].x + xpart(enemy[e].angle1, 7000), enemy[e].y + ypart(enemy[e].angle1, 7000), enemy[e].angle1 + ANGLE_32, EBCOL_B5, 50, 80 + arena.difficulty * 10);
           enemy[e].recycle = 30 - arena.difficulty * 7;
        }
      }
      break;
    }
    break;
   case 1: // opening
   case 3: // closing
    drag_enemy(e, 910);
//    if (arena.counter % 10 != 0)
//     enemy[e].target_time ++;
    break;
   case 4: // moving
    if (enemy[e].target_time > 980)
    {
     enemy[e].x_speed += xpart(enemy[e].angle, 250);
     enemy[e].y_speed += ypart(enemy[e].angle, 250);
    }
    if (enemy[e].x > enemy[e].x_target [0] - 10000
     && enemy[e].x < enemy[e].x_target [0] + 10000
     && enemy[e].y > enemy[e].y_target [0] - 10000
     && enemy[e].y < enemy[e].y_target [0] + 10000)
     {
      enemy[e].phase = 1;
      enemy[e].target_time = 30; // also above
     }
    break;
   case 5: // departing
    if (enemy[e].target_time > 980)
     enemy[e].y_speed += 250;
    break;

  }



 move_enemy(e);
}


void move_dart_stopper(int e)
{

 int i;
 int tangle;

 if (enemy[e].recycle > 0)
  enemy[e].recycle --;

  switch(enemy[e].phase)
  {
   case 0: // entry
    enemy[e].x_speed += xpart(enemy[e].angle, eclass[enemy[e].type].speed1);
    enemy[e].y_speed += ypart(enemy[e].angle, eclass[enemy[e].type].speed1);
    drag_enemy(e, 920);
    if (enemy[e].x > enemy[e].x_target [0] - 40000
        && enemy[e].x < enemy[e].x_target [0] + 40000
        && enemy[e].y > enemy[e].y_target [0] - 40000
        && enemy[e].y < enemy[e].y_target [0] + 40000)
        {
         enemy[e].angle = get_e_angle(e, enemy[e].x_target [1], enemy[e].y_target [1]);
         enemy[e].phase = 1;
         enemy[e].persist = 0;
        }
    break;
   case 1: // then go to 2nd target
    enemy[e].x_speed += xpart(enemy[e].angle, eclass[enemy[e].type].speed1);
    enemy[e].y_speed += ypart(enemy[e].angle, eclass[enemy[e].type].speed1);
    if (arena.counter % 5 == 0)
     enemy[e].angle = get_e_angle(e, enemy[e].x_target [1], enemy[e].y_target [1]);
    drag_enemy(e, 920);
    if (enemy[e].x > enemy[e].x_target [1] - 40000
        && enemy[e].x < enemy[e].x_target [1] + 40000
        && enemy[e].y > enemy[e].y_target [1] - 40000
        && enemy[e].y < enemy[e].y_target [1] + 40000)
        {
         enemy[e].phase = 2;
         enemy[e].target_time = eclass[enemy[e].type].speed3;
        }
    break;
   case 2: // firing!
    switch(enemy[e].type)
    {
     case ENEMY_LARGER:
     if (enemy[e].recycle == 0)
     {
        if (synch_ready [EVENT_FIRE3_2])
        {
           synch_done [DONE_FIRE3_2] |= FIRE3_LARGER;
//           fire_wing_diamond(enemy[e].x, enemy[e].y, attack_angle(e), EBCOL_B2, 20, 40);
           fire_wing_diamond(enemy[e].x, enemy[e].y, grand(ANGLE_1), EBCOL_B2, 20, 15 + arena.difficulty * 5);
           if (arena.difficulty > 0)
            fire_wing_diamond(enemy[e].x, enemy[e].y, grand(ANGLE_1), EBCOL_B2, 20, 15 + arena.difficulty * 5);
           if (arena.difficulty > 1)
            fire_wing_diamond(enemy[e].x, enemy[e].y, grand(ANGLE_1), EBCOL_B2, 20, 15 + arena.difficulty * 5);
           enemy[e].recycle = 20 - arena.difficulty * 5;
        }
     }
     for (i = 0; i < 4; i ++)
     {
       tangle = enemy[e].angle + i * ANGLE_4;
       tangle &= 1023;
       if ((enemy[e].petal_angle [i] > tangle - 20
         && enemy[e].petal_angle [i] < tangle + 20)
        || (tangle < 20 && enemy[e].petal_angle [i] > ANGLE_1 - tangle)
        || (tangle > ANGLE_1 - 20 && enemy[e].petal_angle [i] < ANGLE_1 - tangle))
        {
         enemy[e].petal_angle [i] = tangle;
         enemy[e].petal_angle_speed [i] = enemy[e].status;
        }
         else
          petal_turn(e, i, tangle, 4, 24);
     }
     enemy[e].angle += enemy[e].status;
     enemy[e].angle &= 1023;

     drag_enemy(e, 910);
     enemy[e].target_time --;
     if (enemy[e].target_time <= 0)
     {
      enemy[e].phase = 4;
      enemy[e].persist = 0;
      if (enemy[e].x < 320000)
       enemy[e].angle = grand(ANGLE_8) - grand(ANGLE_8);
        else
         enemy[e].angle = ANGLE_2 + grand(ANGLE_8) - grand(ANGLE_8);
     }
     break;

     case ENEMY_LARGER2:
     if (enemy[e].recycle == 0)
     {
        if (synch_ready [EVENT_FIRE3_2])
        {
           synch_done [DONE_FIRE3_2] |= FIRE3_LARGER2;
//           fire_seed(enemy[e].x, enemy[e].y, attack_angle(e), EBCOL_B5, 18 + grand(10), 2000 + grand(1500));
           fire_seed(enemy[e].x, enemy[e].y, grand(ANGLE_1), EBCOL_B5, 18 + grand(10), 2000 + grand(1500));
           if (arena.difficulty > 0)
            fire_seed(enemy[e].x, enemy[e].y, grand(ANGLE_1), EBCOL_B5, 18 + grand(10), 2000 + grand(1500));
           if (arena.difficulty > 1)
            fire_seed(enemy[e].x, enemy[e].y, grand(ANGLE_1), EBCOL_B5, 18 + grand(10), 2000 + grand(1500));
           enemy[e].recycle = 60 - arena.difficulty * 20;
        }
     }
     for (i = 0; i < 3; i ++)
     {
       tangle = enemy[e].angle + i * ANGLE_3;
       tangle &= 1023;
       if ((enemy[e].petal_angle [i] > tangle - 20
         && enemy[e].petal_angle [i] < tangle + 20)
        || (tangle < 20 && enemy[e].petal_angle [i] > ANGLE_1 - tangle)
        || (tangle > ANGLE_1 - 20 && enemy[e].petal_angle [i] < ANGLE_1 - tangle))
        {
         enemy[e].petal_angle [i] = tangle;
         enemy[e].petal_angle_speed [i] = enemy[e].status;
        }
         else
          petal_turn(e, i, tangle, 4, 24);
     }
     for (i = 3; i < 6; i ++)
     {
       tangle = (ANGLE_1 - enemy[e].angle) + i * ANGLE_3;
       tangle &= 1023;
       if ((enemy[e].petal_angle [i] > tangle - 20
         && enemy[e].petal_angle [i] < tangle + 20)
        || (tangle < 20 && enemy[e].petal_angle [i] > ANGLE_1 - tangle)
        || (tangle > ANGLE_1 - 20 && enemy[e].petal_angle [i] < ANGLE_1 - tangle))
        {
         enemy[e].petal_angle [i] = tangle;
         enemy[e].petal_angle_speed [i] = enemy[e].status;
        }
         else
          petal_turn(e, i, tangle, 4, 24);
     }
     enemy[e].angle += enemy[e].status;
     enemy[e].angle &= 1023;

     drag_enemy(e, 910);
     enemy[e].target_time --;
     if (enemy[e].target_time <= 0)
     {
      enemy[e].phase = 4;
      enemy[e].persist = 0;
      if (enemy[e].x < 320000)
       enemy[e].angle = grand(ANGLE_8) - grand(ANGLE_8);
        else
         enemy[e].angle = ANGLE_2 + grand(ANGLE_8) - grand(ANGLE_8);
     }

    break;
    }
    break;
   case 4: // departing
    enemy[e].x_speed += xpart(enemy[e].angle, eclass[enemy[e].type].speed1);
    enemy[e].y_speed += ypart(enemy[e].angle, eclass[enemy[e].type].speed1);
    drag_enemy(e, 950);
    break;

  }

 switch(enemy[e].type)
 {
   case ENEMY_LARGER:
       enemy_pulse(e, EVENT_PULSE2);
       if (enemy[e].phase == 2)
        break;
       petal_turn(e, 0, enemy[e].angle + ANGLE_2 - ANGLE_8, 2, 8);
       petal_turn(e, 1, enemy[e].angle + ANGLE_2 - ANGLE_16, 2, 8);
       petal_turn(e, 2, enemy[e].angle + ANGLE_2 + ANGLE_16, 2, 8);
       petal_turn(e, 3, enemy[e].angle + ANGLE_2 + ANGLE_8, 2, 8);
       break;
   case ENEMY_LARGER2:
       enemy_pulse(e, EVENT_PULSE1);
       if (enemy[e].phase == 2)
        break;
       petal_turn(e, 0, enemy[e].angle + ANGLE_2 - ANGLE_8, 2, 8);
       petal_turn(e, 1, enemy[e].angle + ANGLE_2, 2, 8);
       petal_turn(e, 2, enemy[e].angle + ANGLE_2 + ANGLE_8, 2, 8);

       petal_turn(e, 3, enemy[e].angle + ANGLE_2 - ANGLE_16, 2, 10);
       petal_turn(e, 4, enemy[e].angle + ANGLE_2, 2, 8);
       petal_turn(e, 5, enemy[e].angle + ANGLE_2 + ANGLE_16, 2, 10);
       break;


 }

 move_enemy(e);
}










void move_drifter(int e)
{
 int b;


 enemy[e].target_time --;
 if (enemy[e].recycle > 0)
  enemy[e].recycle --;

 if (enemy[e].target_time <= 0)
 {
  enemy[e].phase = 1;
  enemy[e].persist = 0;
  enemy[e].target_time = 5000;
  switch(grand(4))
  {
   case 0: enemy[e].x_target [0] = -100000; break;
   case 1: enemy[e].x_target [0] = 900000; break;
   case 2: enemy[e].y_target [0] = -100000; break;
   case 3: enemy[e].y_target [0] = 900000; break;
  }
  enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
 }

 switch(enemy[e].phase)
 {
   case 0:
   if ((enemy[e].x > enemy[e].x_target [0] - 40000
     && enemy[e].x < enemy[e].x_target [0] + 40000
     && enemy[e].y > enemy[e].y_target [0] - 40000
     && enemy[e].y < enemy[e].y_target [0] + 40000)
      || (enemy[e].escorting != -1 && (enemy[e].target_time & 16) == 1))
     {
      if (enemy[e].escorting != -1)
      {
        enemy[e].x_target [0] = enemy[enemy[e].escorting].x + grand(60000) - grand(60000) + enemy[e].pulse1;
        enemy[e].y_target [0] = enemy[enemy[e].escorting].y + grand(60000) - grand(60000) + enemy[e].pulse2;
        enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
      }
       else
       {
        enemy[e].x_target [0] = RAND_X_IN;
        enemy[e].y_target [0] = RAND_Y_IN;
        if (enemy[e].type == ENEMY_RING)
        {
         enemy[e].x_target [0] = RAND_X;
         enemy[e].y_target [0] = RAND_Y;
        }
        enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
       }
     }
    break;

 }

 if (enemy[e].target_time % 10 == 0)
  enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);

 enemy[e].x_speed += xpart(enemy[e].angle, eclass[enemy[e].type].speed1);
 enemy[e].y_speed += ypart(enemy[e].angle, eclass[enemy[e].type].speed1);

 drag_enemy(e, eclass[enemy[e].type].speed2);

 move_enemy(e);

  int angle;

  switch(enemy[e].type)
  {
   case ENEMY_RING:
    enemy_pulse_slow(e, EVENT_PULSE);
    if (enemy[e].phase == 0)// && (arena.counter & 1))
    {
     enemy[e].status ++;
     if (enemy[e].status > 90)
      enemy[e].status = 90;
    }
    if (enemy[e].phase == 1)
    {
     enemy[e].status --;
     if (enemy[e].status < 1)
      enemy[e].status = 1;
    }
    if (enemy[e].status > 0)
    {
     b = create_ebullet(EBULLET_SPIN);
     if (b != -1)
     {
      ebullet[b].x = enemy[e].x;
      ebullet[b].y = enemy[e].y;
      ebullet[b].x_speed = enemy[e].status;
      if (ebullet[b].x_speed > 6)
       ebullet[b].x_speed = 6;
      ebullet[b].y_speed = 4;
      ebullet[b].status = enemy[e].status;
      ebullet[b].angle = enemy[e].angle1;
      ebullet[b].timeout = 2;
      ebullet[b].colour = TCOL_B5;
      ebullet[b].accel = TCOL_B4;
      ebullet[b].cord_colour = TRANS_B4_OUT;
     }
    }


    if (enemy[e].status > 0)
    {
     b = create_ebullet(EBULLET_SPIN);
     if (b != -1)
     {
      ebullet[b].x = enemy[e].x;
      ebullet[b].y = enemy[e].y;
      ebullet[b].x_speed = enemy[e].status;
      if (ebullet[b].x_speed > 6)
       ebullet[b].x_speed = 6;
      ebullet[b].y_speed = 2;
      ebullet[b].status = enemy[e].status / 2;
      ebullet[b].angle = ANGLE_1 - enemy[e].angle1;
      ebullet[b].timeout = 2;
      ebullet[b].colour = TCOL_B4;
      ebullet[b].accel = TCOL_B3;
      ebullet[b].cord_colour = TRANS_B3_OUT;
     }
    }
    enemy[e].angle1 += enemy[e].angle2;
    enemy[e].angle1 &= 1023;



/*    if (enemy[e].recycle <= 0
        && enemy[e].phase == 0)
    {
        if (synch_ready [EVENT_FIRE4_2])
        {
           synch_done [DONE_FIRE4_2] |= FIRE4_RING;
           enemy[e].angle1 = attack_angle(e);
//           fire_beam2(e, angle, 1000, 0);
           enemy[e].recycle = 150 - arena.difficulty * 12;
           enemy[e].status = 120;
        }
    }
      if (enemy[e].status > 0)
      {
       drag_enemy(e, 910);
       fire_beam2(e, enemy[e].angle1, 1000, 0);
       enemy[e].status --;
/ *       if (enemy[e].status == 100)
       {
         enemy[e].status = 35;
       }*/
      //}


    break;
   case ENEMY_DRIFTER1:
   case ENEMY_ESCORT:
    if (enemy[e].status > 0)
     enemy[e].status --;
    if (enemy[e].recycle <= 0
        && enemy[e].phase == 0
        && enemy[e].y < 440000)
    {
        if (synch_ready [EVENT_FIRE2_1])
        {
           synch_done [DONE_FIRE2_1] |= FIRE2_DRIFTER1;
           angle = attack_angle(e);
           fire_dot(enemy[e].x, enemy[e].y, angle, EBCOL_B3, 5, 4000 + arena.difficulty * 1000, 7);
           enemy[e].recycle = 65 - arena.difficulty * 10;
           enemy[e].status = 16;
        }
    }
      quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 15000 + (enemy[e].type == ENEMY_DRIFTER1) * 4000, enemy[e].x_speed + grand(300) - grand(300), enemy[e].y_speed + 1500 + grand(2500), 6 + grand(4), TCOL_SH2, 0);
    break;
   case ENEMY_DRIFTER2:
      quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 4000, enemy[e].y + 17000, enemy[e].x_speed + grand(300) - grand(300), enemy[e].y_speed + 1500 + grand(2500), 6 + grand(4), TCOL_SH2, 0);
      quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 4000, enemy[e].y + 17000, enemy[e].x_speed + grand(300) - grand(300), enemy[e].y_speed + 1500 + grand(2500), 6 + grand(4), TCOL_SH2, 0);
    if (enemy[e].status > 0)
     enemy[e].status --;
    if (enemy[e].recycle <= 0
        && enemy[e].phase == 0)
    {
        if (synch_ready [EVENT_FIRE2_1])
        {
           synch_done [DONE_FIRE2_1] |= FIRE2_DRIFTER1;
           angle = attack_angle(e);
           fire_dot(enemy[e].x, enemy[e].y, angle, EBCOL_B4, 6, 4000 + arena.difficulty * 1000, 8);
           fire_dot(enemy[e].x, enemy[e].y, angle - ANGLE_32, EBCOL_B3, 4, 3700 + arena.difficulty * 1000, 6);
           fire_dot(enemy[e].x, enemy[e].y, angle + ANGLE_32, EBCOL_B3, 4, 3700 + arena.difficulty * 1000, 6);
           enemy[e].recycle = 60 - arena.difficulty * 12;
           enemy[e].status = 16;
        }
    }
    break;


  }


}




void move_diver(int e)
{
 enemy[e].target_time --;
 if (enemy[e].recycle > 0)
  enemy[e].recycle --;

  int angle, i;

 if (enemy[e].target_time <= 0)
 {
  switch(enemy[e].phase)
  {
   case 0: // diving in first
    enemy[e].phase = 1; // opening
    enemy[e].target_time = 20; // also below
    break;
   case 1: // opening
    enemy[e].phase = 2; // shooting
//    enemy[e].x_speed = 0;
//    enemy[e].y_speed = 0;
    enemy[e].target_time = 54 + arena.difficulty * 10;
    break;
   case 2: // shooting
    enemy[e].phase = 3; // closing
    enemy[e].target_time = 20;
    break;
   case 3: // closing
     enemy[e].phase = 5; // departing
     enemy[e].target_time = 1000; // important! see below
     enemy[e].persist = 0;
    break;

  }
 }
    track_target(e, player.x, player.y, enemy[e].angle1, 8);

  switch(enemy[e].phase)
  {
   case 2: // shooting
    drag_enemy_y(e, 950);
    if (enemy[e].recycle <= 0)
    {
      if (synch_ready [EVENT_SWOOPER1_FIRE])
      {
         synch_done [DONE_SWOOPER1_FIRE] |= FIRE1_DIVER1;
         angle = grand(ANGLE_1);
         for (i = 0; i < 3; i ++)
         {
//          fire_dart(enemy[e].x + xpart(angle, 3000), enemy[e].y + ypart(angle, 3000), angle, 2, 50, 80);
          fire_dart(enemy[e].x, enemy[e].y, angle, EBCOL_B2, 50, 80);
          angle += ANGLE_3;
          angle &= 1023;
         }
         enemy[e].recycle = 14 - arena.difficulty * 4;
      }
    }
    break;
   case 1: // opening
   case 3: // closing
    drag_enemy_y(e, 950);
//    if (arena.counter % 10 != 0)
//     enemy[e].target_time ++;
    break;
//   case 4: // moving
//    break;
   case 5: // departing
    if (enemy[e].target_time > 960)
     enemy[e].y_speed -= 170;
    break;

  }



 move_enemy(e);
}

void move_walker(int e)
{

 int i, x, y, angle;

 if (enemy[e].recycle > 0 && enemy[e].y < 460000)
  enemy[e].recycle --;

 if (arena.counter & 15)
 {
  if (enemy[e].x < 140000)
   enemy[e].x_speed += 100;
  if (enemy[e].x > 500000)
   enemy[e].x_speed -= 100;
  if (enemy[e].x > 150000 && enemy[e].x < 490000)
   enemy[e].x_speed += grand(101) - 50;
 }


   if (enemy[e].recycle <= 0)
   {
    if (synch_ready [EVENT_FIRE4_2])// && synch_done [DONE_FIRE4_2] == 0)
    {
      synch_done [DONE_FIRE4_2] |= FIRE4_WALKER;
      enemy[e].recycle = 40 - arena.difficulty * 10;
      switch(grand(3))
      {
       case 0: x = enemy[e].x; y = enemy[e].y - 32000; break;
       case 1: x = enemy[e].x - 42000; y = enemy[e].y; break;
       case 2: x = enemy[e].x + 42000; y = enemy[e].y; break;
      }
      angle = grand(ANGLE_1);
      for (i = 0; i < 12; i ++)
      {
        fire_dart(x, y, angle, EBCOL_B2, 50, 90);
//        fire_curve(x, y, angle + ANGLE_12 / 2, EBCOL_B3, 3000, pos_or_neg(3 + grand(4)), 5);
        fire_curve(x, y, angle + ANGLE_12 / 2, EBCOL_B3, 3000, xpart(angle * 2, 8), 5);
        angle += ANGLE_12;
        angle &= 1023;
      }
    }
   }

 enemy_pulse_slow(e, EVENT_PULSE);


 if (enemy[e].y > 200000)
  enemy[e].persist = 0;

 move_enemy(e);



}

void move_cruiser(int e)
{

 int i, angle, b;

 if (enemy[e].recycle > 0 && enemy[e].y < 460000)
  enemy[e].recycle --;

 switch(enemy[e].type)
 {
  case ENEMY_SPINNER:
      if (enemy[e].recycle <= 0)
      {
       if (synch_ready [EVENT_FIRE4_1])
       {
        synch_done [DONE_FIRE4_1] |= FIRE4_SPINNER;
        enemy[e].recycle = 50 - arena.difficulty * 5;
        if (enemy [e].y > 360000)
         enemy[e].recycle = 2000;
        angle = radians_to_angle(atan2(player.y - enemy[e].y - 32000, player.x - enemy[e].x));
        fire_ddart(enemy[e].x, enemy[e].y + 32000, angle, EBCOL_B5, 40, 90 + arena.difficulty * 10);
        angle = radians_to_angle(atan2(player.y - enemy[e].y + 32000, player.x - enemy[e].x));
        fire_ddart(enemy[e].x, enemy[e].y - 32000, angle, EBCOL_B5, 40, 90 + arena.difficulty * 10);
       }
      }
   enemy_pulse_slow(e, EVENT_PULSE);
   if (enemy[e].y > 1000 && enemy[e].y < 200000)
   {
    enemy[e].status += 3;
    if (enemy[e].status > 150)
     enemy[e].status = 150;
   }
   if (enemy[e].y > 430000)
   {
    enemy[e].status -= 3;
    if (enemy[e].status < 5)
     enemy[e].status = 5;
   }
   if (enemy[e].status > 0)
   {
    b = create_ebullet(EBULLET_SPIN);
    if (b != -1)
    {
     ebullet[b].x = enemy[e].x;
     ebullet[b].y = enemy[e].y;
     ebullet[b].x_speed = enemy[e].status;
     if (ebullet[b].x_speed > 12)
      ebullet[b].x_speed = 12;
     ebullet[b].y_speed = 5;
     ebullet[b].status = enemy[e].status;
     ebullet[b].angle = enemy[e].angle1;
     ebullet[b].timeout = 2;
     ebullet[b].colour = TCOL_B4;
     ebullet[b].accel = TCOL_B2;
     ebullet[b].cord_colour = TRANS_B2_OUT;
    }
   }
   enemy[e].angle1 += enemy[e].angle2;
   enemy[e].angle1 &= 1023;
   break;











  case ENEMY_DROPPER:
      if (enemy[e].recycle <= 0)
      {
       if (synch_ready [EVENT_FIRE4_1])
       {
        synch_done [DONE_FIRE4_1] |= FIRE4_DROPPER;
        enemy[e].recycle = 50 - arena.difficulty * 5;
        if (enemy [e].y > 360000)
         enemy[e].recycle = 2000;
        for (i = 0; i < 3; i ++)
        {
         angle = grand(ANGLE_8) - grand(ANGLE_8);
         fire_dart(enemy[e].x - 7000, enemy[e].y + 14000, angle + ANGLE_2, EBCOL_B1, 50, 70 + grand(30));
         fire_dart(enemy[e].x + 7000, enemy[e].y + 14000, angle, EBCOL_B1, 50, 70 + grand(30));

        }
/*        fire_dart(enemy[e].x - 7000, enemy[e].y + 14000, ANGLE_2, EBCOL_B1, 50, 90);
        fire_dart(enemy[e].x - 7000, enemy[e].y + 14000, ANGLE_2 - ANGLE_8, EBCOL_B1, 50, 90);
        fire_dart(enemy[e].x - 7000, enemy[e].y + 14000, ANGLE_2 + ANGLE_8, EBCOL_B1, 50, 90);
        fire_dart(enemy[e].x + 7000, enemy[e].y + 14000, 0, EBCOL_B1, 50, 90);
        fire_dart(enemy[e].x + 7000, enemy[e].y + 14000, 0 - ANGLE_8, EBCOL_B1, 50, 90);
        fire_dart(enemy[e].x + 7000, enemy[e].y + 14000, 0 + ANGLE_8, EBCOL_B1, 50, 90);*/
       }
      }
   enemy_pulse_slow(e, EVENT_PULSE);
   if (enemy[e].y > 1000 && enemy[e].y < 200000)
   {
    enemy[e].status ++;
    if (enemy[e].status > 50)
     enemy[e].status = 50;
   }
   if (enemy[e].y > 450000)
   {
    enemy[e].status --;
    if (enemy[e].status < 5)
     enemy[e].status = 5;
   }
   if (enemy[e].status > 0)
   {
    b = create_ebullet(EBULLET_SPIN);
    if (b != -1)
    {
     ebullet[b].x = enemy[e].x;
     ebullet[b].y = enemy[e].y;
     ebullet[b].x_speed = enemy[e].status;
     if (ebullet[b].x_speed > 6)
      ebullet[b].x_speed = 6;
     ebullet[b].y_speed = 3;
     ebullet[b].status = enemy[e].status;
     ebullet[b].angle = enemy[e].angle1;
     ebullet[b].timeout = 2;
     ebullet[b].colour = TCOL_B3;
     ebullet[b].accel = TCOL_B1;
     ebullet[b].cord_colour = TRANS_B1_OUT;
    }
   }
   enemy[e].angle1 += enemy[e].angle2;
   enemy[e].angle1 &= 1023;
   break;
  case ENEMY_CRUISER4:
  if ((arena.counter & 1) == 0)
  {
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 14000, enemy[e].y + 49000, grand(300) - grand(300), 3500 + grand(2500), 6 + grand(6), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 14000, enemy[e].y + 49000, grand(300) - grand(300), 3500 + grand(2500), 6 + grand(6), TCOL_ORANGE, 0);
  }
   else
   {
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 14000, enemy[e].y + 49000, grand(300) - grand(300), 3500 + grand(2500), 12 + grand(12), TCOL_SH2, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 14000, enemy[e].y + 49000, grand(300) - grand(300), 3500 + grand(2500), 12 + grand(12), TCOL_SH2, 0);
   }
      if (enemy[e].status == 0)
       track_target(e, player.x, player.y, enemy[e].angle1, 5 + arena.difficulty * 3);
      if (enemy[e].status > 0)
      {
       fire_beam(e, 25000);
       enemy[e].status --;
       if (enemy[e].status == 100)
       {
/*         enemy[e].status = 40;
        if (arena.difficulty == 0)
         enemy[e].status = 20;
        if (arena.difficulty == 1)*/
         enemy[e].status = 35;
       }
      }
      if (enemy[e].recycle <= 0)
      {
       if (synch_ready [EVENT_FIRE2_2] && synch_done [DONE_FIRE2_2] == 0)
       {
        synch_done [DONE_FIRE2_2] |= FIRE2_CRUISER4;
        enemy[e].recycle = 90 - arena.difficulty * 5;
        enemy[e].status = 120; // change in fire_beam as well!
        if (enemy [e].y > 360000)
         enemy[e].recycle = 2000;
       }
        else
         enemy[e].recycle = 1;
      }
   break;
  case ENEMY_CRUISER3:
  if ((arena.counter & 1) == 0)
  {
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 47000, enemy[e].y + 64000, grand(300) - grand(300), 3500 + grand(2500), 9 + grand(9), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 47000, enemy[e].y + 64000, grand(300) - grand(300), 3500 + grand(2500), 9 + grand(9), TCOL_ORANGE, 0);
  }
   else
   {
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 47000, enemy[e].y + 64000, grand(300) - grand(300), 3500 + grand(2500), 18 + grand(18), TCOL_SH2, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 47000, enemy[e].y + 64000, grand(300) - grand(300), 3500 + grand(2500), 18 + grand(18), TCOL_SH2, 0);
   }
   track_target(e, player.x, player.y, enemy[e].angle1, 6);

   if (enemy[e].recycle <= 0)
   {
    if (synch_ready [EVENT_FIRE2_2] && synch_done [DONE_FIRE2_2] == 0)
    {
      synch_done [DONE_FIRE2_2] |= FIRE2_CRUISER3;
      for (i = 0; i < 6; i ++)
      {
       angle = enemy[e].angle1 + i * ANGLE_6;
       angle &= 1023;
       b = fire_dot(enemy[e].x + xpart(enemy[e].angle1, 23000), enemy[e].y + ypart(enemy[e].angle1, 23000), enemy[e].angle1, EBCOL_B3, 4 + i, 2500, 0);
       if (b != -1)
       {
        ebullet[b].y_speed += enemy[e].y_speed + ypart(angle, 1000);
        ebullet[b].x_speed += enemy[e].x_speed + xpart(angle, 1000);
        ebullet[b].cord_wait = -1;
       }
      }
      for (i = 0; i < 8; i ++)
      {
       angle = enemy[e].angle1 + i * ANGLE_8;
       angle &= 1023;
       b = fire_dot(enemy[e].x + xpart(enemy[e].angle1, 23000), enemy[e].y + ypart(enemy[e].angle1, 23000), enemy[e].angle1, EBCOL_B2, 12 - i, 2500, 0);
       if (b != -1)
       {
        ebullet[b].y_speed += enemy[e].y_speed + ypart(angle, 750);
        ebullet[b].x_speed += enemy[e].x_speed + xpart(angle, 750);
        ebullet[b].cord_wait = -1;
       }
      }
      for (i = 0; i < 16; i ++)
      {
       angle = enemy[e].angle1 + i * ANGLE_16;
       angle &= 1023;
       b = fire_dot(enemy[e].x + xpart(enemy[e].angle1, 23000), enemy[e].y + ypart(enemy[e].angle1, 23000), enemy[e].angle1, EBCOL_B1, 4 + i / 4, 2500, 0);
       if (b != -1)
       {
        ebullet[b].y_speed += enemy[e].y_speed + ypart(angle, 500);
        ebullet[b].x_speed += enemy[e].x_speed + xpart(angle, 500);
        ebullet[b].cord_wait = -1;

      }
      enemy[e].recycle = 130 - arena.difficulty * 19;
    quick_cloud(CLOUD_MSHOCK, enemy[e].x + xpart(enemy[e].angle1, 24000), enemy[e].y + ypart(enemy[e].angle1, 24000), enemy[e].x_speed, enemy[e].y_speed, 20, 2, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + xpart(enemy[e].angle1, 24000), enemy[e].y + ypart(enemy[e].angle1, 24000), enemy[e].x_speed, enemy[e].y_speed, 20 + grand(5), TCOL_SH1, 0);
    quick_cloud(CLOUD_CIRCLE3, enemy[e].x + xpart(enemy[e].angle1, 24000), enemy[e].y + ypart(enemy[e].angle1, 24000), enemy[e].x_speed, enemy[e].y_speed, 30 + grand(5), TCOL_SH2, 0);
    }
   }
   }


   break;
  case ENEMY_CRUISER2:
  if ((arena.counter & 1) == 0)
  {
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 47000, enemy[e].y + 14000, grand(300) - grand(300), 3000 + grand(2000), 5 + grand(5), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 47000, enemy[e].y + 14000, grand(300) - grand(300), 3000 + grand(2000), 5 + grand(5), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 43000, grand(300) - grand(300), 3000 + grand(2500), 7 + grand(7), TCOL_ORANGE, 0);
  }
   else
   {
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 47000, enemy[e].y + 14000, grand(300) - grand(300), 3000 + grand(2000), 10 + grand(10), TCOL_SH2, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 47000, enemy[e].y + 14000, grand(300) - grand(300), 3000 + grand(2000), 10 + grand(10), TCOL_SH2, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 43000, grand(300) - grand(300), 3000 + grand(2500), 14 + grand(14), TCOL_SH2, 0);
   }
   track_target(e, player.x, player.y, enemy[e].angle1, 6);

   if (enemy[e].recycle <= 0)
   {
    if (synch_ready [EVENT_FIRE2_2] && synch_done [DONE_FIRE2_2] == 0)
    {
       synch_done [DONE_FIRE2_2] |= FIRE2_CRUISER2;
       angle = enemy[e].angle1;
       for (i = 0; i < 5 + arena.difficulty * 2; i ++)
       {
        b = fire_dot(enemy[e].x + xpart(enemy[e].angle1, 9000), enemy[e].y + ypart(enemy[e].angle1, 9000), angle, EBCOL_B5, 4 + i, 1500 + (i * 600), 0);
        if (b != -1)
        {
         ebullet[b].y_speed += enemy[e].y_speed;
         ebullet[b].cord_wait = -1;
        }
        if ((i & 1) == 0)
        {
         b = fire_dot(enemy[e].x + xpart(enemy[e].angle1, 9000), enemy[e].y + ypart(enemy[e].angle1, 9000), angle - ANGLE_64, EBCOL_B5, 2 + i / 2, 1500 + (i * 550), 0);
         if (b != -1)
         {
          ebullet[b].y_speed += enemy[e].y_speed;
          ebullet[b].cord_wait = -1;
         }
        }
         else
         {
          b = fire_dot(enemy[e].x + xpart(enemy[e].angle1, 9000), enemy[e].y + ypart(enemy[e].angle1, 9000), angle + ANGLE_64, EBCOL_B5, 2 + i / 2, 1500 + (i * 550), 0);
          if (b != -1)
          {
           ebullet[b].y_speed += enemy[e].y_speed;
           ebullet[b].cord_wait = -1;
          }
         }
       }
       enemy[e].recycle = 130 - arena.difficulty * 19;
     quick_cloud(CLOUD_MSHOCK, enemy[e].x + xpart(enemy[e].angle1, 10000), enemy[e].y + ypart(enemy[e].angle1, 10000), enemy[e].x_speed, enemy[e].y_speed, 20, 0, 0);
     quick_cloud(CLOUD_CIRCLE2, enemy[e].x + xpart(enemy[e].angle1, 10000), enemy[e].y + ypart(enemy[e].angle1, 10000), enemy[e].x_speed, enemy[e].y_speed, 20 + grand(5), TCOL_ORANGE, 0);
     quick_cloud(CLOUD_CIRCLE3, enemy[e].x + xpart(enemy[e].angle1, 10000), enemy[e].y + ypart(enemy[e].angle1, 10000), enemy[e].x_speed, enemy[e].y_speed, 30 + grand(5), TCOL_YELLOW, 0);
    }
   }


   break;
  case ENEMY_CRUISER1:
  if ((arena.counter & 1) == 0)
  {
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 42000, enemy[e].y + 61000, grand(300) - grand(300), 3000 + grand(2000), 5 + grand(5), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 42000, enemy[e].y + 61000, grand(300) - grand(300), 3000 + grand(2000), 5 + grand(5), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 16000, enemy[e].y + 69000, grand(300) - grand(300), 3000 + grand(2000), 5 + grand(5), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 16000, enemy[e].y + 69000, grand(300) - grand(300), 3000 + grand(2000), 5 + grand(5), TCOL_ORANGE, 0);
  }
   else
   {
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 42000, enemy[e].y + 61000, grand(300) - grand(300), 3000 + grand(2000), 10 + grand(10), TCOL_SH2, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 42000, enemy[e].y + 61000, grand(300) - grand(300), 3000 + grand(2000), 10 + grand(10), TCOL_SH2, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 16000, enemy[e].y + 69000, grand(300) - grand(300), 3000 + grand(2000), 10 + grand(10), TCOL_SH2, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 16000, enemy[e].y + 69000, grand(300) - grand(300), 3000 + grand(2000), 10 + grand(10), TCOL_SH2, 0);
   }
   track_target(e, player.x, player.y, enemy[e].angle1, 8);
   track_target_zero(e, player.x, player.y, enemy[e].angle, 2);

   if (enemy[e].status > 0 && enemy[e].y < 450000)
    enemy[e].status --;

   if (enemy[e].status <= 0)
   {
        if (synch_ready [EVENT_FIRE2_1])
        {
           synch_done [DONE_FIRE2_1] |= FIRE2_DRIFTER1;
           angle = enemy[e].angle + grand(ANGLE_64) - grand(ANGLE_64);
           fire_dot(enemy[e].x + xpart(enemy[e].angle1, 6000), enemy[e].y + ypart(enemy[e].angle1, 6000) - 41000, enemy[e].angle1, EBCOL_B1, 4, 3000 + arena.difficulty * 700, 15);
           enemy[e].status = 30 - arena.difficulty * 9;
        }
   }

   if (enemy[e].recycle <= 0)
   {
    if (synch_ready [EVENT_FIRE2_2] && synch_done [DONE_FIRE2_2] == 0)
    {
      synch_done [DONE_FIRE2_2] |= FIRE2_CRUISER1;
      for (i = 0; i < 7; i ++)
      {
        angle = enemy[e].angle + grand(ANGLE_32) - grand(ANGLE_32);
        angle &= 1023;
        b = fire_dot(enemy[e].x + xpart(enemy[e].angle, 11000), enemy[e].y + ypart(enemy[e].angle, 11000), angle, EBCOL_B3, 6 + grand(3), 1500 + arena.difficulty * 500 + grand(2500), 0);
           if (b != -1)
           {
            ebullet[b].y_speed += enemy[e].y_speed;
            ebullet[b].cord_wait = -1;
           }
      }
      for (i = 0; i < 7; i ++)
      {
        angle = enemy[e].angle + grand(ANGLE_32) - grand(ANGLE_32);
        angle &= 1023;
        b = fire_dot(enemy[e].x + xpart(enemy[e].angle, 11000), enemy[e].y + ypart(enemy[e].angle, 11000), angle, EBCOL_B2, 5 + grand(3), 1500 + arena.difficulty * 500 + grand(2000), 0);
           if (b != -1)
           {
            ebullet[b].y_speed += enemy[e].y_speed;
            ebullet[b].cord_wait = -1;
           }
      }
    quick_cloud(CLOUD_MSHOCK, enemy[e].x + xpart(enemy[e].angle, 11000), enemy[e].y + ypart(enemy[e].angle, 11000), enemy[e].x_speed, enemy[e].y_speed, 20, 2, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + xpart(enemy[e].angle, 11000), enemy[e].y + ypart(enemy[e].angle, 11000), enemy[e].x_speed, enemy[e].y_speed, 20 + grand(5), TCOL_SH1, 0);
    quick_cloud(CLOUD_CIRCLE3, enemy[e].x + xpart(enemy[e].angle, 11000), enemy[e].y + ypart(enemy[e].angle, 11000), enemy[e].x_speed, enemy[e].y_speed, 30 + grand(5), TCOL_SH2, 0);

    enemy[e].recycle = 120 - arena.difficulty * 20;
    }
   }


   break;
 }


 if (enemy[e].y > 200000)
  enemy[e].persist = 0;

 move_enemy(e);

}


void move_stopper(int e)
{

 int i, angle, angle_inc;

 enemy[e].target_time --;
 if (enemy[e].recycle > 0)
  enemy[e].recycle --;

 if (enemy[e].target_time <= 0)
 {
  switch(enemy[e].phase)
  {
   case 0: // diving in first
    enemy[e].phase = 1; // opening
    enemy[e].target_time = eclass[enemy[e].type].speed3; // also below
    break;
   case 1: // opening
    enemy[e].phase = 2; // shooting
    enemy[e].x_speed = 0;
    enemy[e].y_speed = 0;
    enemy[e].target_time = eclass[enemy[e].type].speed2;
    if (enemy[e].type == ENEMY_RISER)
     enemy[e].target_time += grand(50);
    break;
   case 2: // shooting
    enemy[e].phase = 3; // closing
    enemy[e].target_time = eclass[enemy[e].type].speed3;
    break;
   case 3: // closing
//    if (enemy[e].phase2 >= 3)
    {
     enemy[e].phase = 4; // departing
     enemy[e].target_time = 1000; // important! see below
     enemy[e].persist = 0;
     enemy[e].x_speed = 0;
     break;
    }
    break;
  }

 }

  switch(enemy[e].phase)
  {
   case 0: // entering
    switch(enemy[e].type)
    {
     case ENEMY_BEAMER:
     case ENEMY_SHOOTER:
     if ((arena.counter & 1) == 0)
     {
       quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 27000, grand(300) - grand(300), 2000 + grand(2000) + enemy[e].y_speed, 5 + grand(5), TCOL_ORANGE, 0);
     }
      else
      {
       quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 27000, grand(300) - grand(300), 2000 + grand(2000) + enemy[e].y_speed, 10 + grand(10), TCOL_SH2, 0);
      }
      break;
    }
    break;

   case 2: // shooting
    switch(enemy[e].type)
    {
     case ENEMY_BEAMER:
     if ((arena.counter & 1) == 0)
     {
       quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 27000, grand(300) - grand(300), 2000 + grand(2000) + enemy[e].y_speed, 3 + grand(3), TCOL_ORANGE, 0);
     }
      else
      {
       quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 27000, grand(300) - grand(300), 2000 + grand(2000) + enemy[e].y_speed, 6 + grand(6), TCOL_SH2, 0);
      }

      if (enemy[e].status == 0)
       track_target(e, player.x, player.y, enemy[e].angle1, 8);
      if (enemy[e].status > 0)
      {
       fire_beam(e, 15000);
       enemy[e].status --;
       if (enemy[e].status == 100)
       {
        if (arena.difficulty == 0)
         enemy[e].status = 50;
        if (arena.difficulty == 1)
         enemy[e].status = 70;
       }
      if (enemy[e].target_time < 2)
       enemy[e].target_time ++;
      }
      if (enemy[e].recycle <= 0)
      {
       if (synch_ready [EVENT_FIRE2_2] && synch_done [DONE_FIRE2_2] == 0)
       {
        synch_done [DONE_FIRE2_2] |= FIRE2_BEAMER;
        enemy[e].recycle = 150;
        enemy[e].status = 120; // change in fire_beam as well!
       }
        else
         enemy[e].recycle = 1;
      }
      break;
     case ENEMY_SHOOTER:
     if ((arena.counter & 1) == 0)
     {
       quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 27000, grand(300) - grand(300), 2000 + grand(2000) + enemy[e].y_speed, 3 + grand(3), TCOL_ORANGE, 0);
     }
      else
      {
       quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 27000, grand(300) - grand(300), 2000 + grand(2000) + enemy[e].y_speed, 6 + grand(6), TCOL_SH2, 0);
      }

      track_target(e, player.x, player.y, enemy[e].angle1, 6);
      if (enemy[e].recycle <= 0 && enemy[e].status == 9 + arena.difficulty * 3)
      {
       if (synch_ready [EVENT_FIRE2_2] && synch_done [DONE_FIRE2_2] == 0)
       {
        synch_done [DONE_FIRE2_2] |= FIRE2_SHOOTER;
       }
        else
         enemy[e].recycle = 1;
      }

      if (enemy[e].recycle <= 0)
      {
        enemy[e].angle1 += grand(ANGLE_16);
        enemy[e].angle1 -= grand(ANGLE_16);
        enemy[e].angle1 &= 1023;
        angle = enemy[e].angle1;
        fire_dot(enemy[e].x + xpart(angle, 10000), enemy[e].y + ypart(angle, 10000), angle, EBCOL_B5, 4, 3500 + arena.difficulty * 800 + grand(500), 9);
        enemy[e].recycle = 7 - arena.difficulty * 2;
        enemy[e].status --;
        if (enemy[e].status == 0)
        {
         enemy[e].status = 9 + arena.difficulty * 3;   // also in level.c
         enemy[e].recycle = 40 - arena.difficulty * 9;
        }
      }
      break;
     case ENEMY_RISER:
      enemy[e].x_speed += xpart(arena.counter * 16, 20);
      if (enemy[e].recycle <= 0)
      {
//       if (synch_ready [EVENT_FIRE3_2] && (synch_done [DONE_FIRE3_2] & FIRE3_WINGS) == 0)
       {
//        synch_done [DONE_FIRE3_2] |= FIRE3_WINGS;
        angle = attack_angle(e);
        fire_dot(enemy[e].x, enemy[e].y, angle, EBCOL_B2, 5, 3500 + arena.difficulty * 800, 9);
        enemy[e].recycle = 65 - arena.difficulty * 12;
        enemy[e].status = 25;
       }
      }
//      quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 2000, enemy[e].y + 23000, enemy[e].x_speed + grand(300) - grand(300), enemy[e].y_speed + 1500 + grand(2500), 4 + grand(3), TCOL_SH2, 0);
      break;
     case ENEMY_POD1:
     enemy_pulse(e, EVENT_PULSE);
     if (enemy[e].recycle <= 0)
     {
      if (synch_ready [EVENT_SWOOPER1_FIRE]
          && (synch_done [DONE_SWOOPER1_FIRE] & FIRE1_DIVER1) == 0)
      {
         synch_done [DONE_SWOOPER1_FIRE] |= FIRE1_DIVER1;
         angle = enemy[e].angle1;
         switch(enemy[e].status)
         {
             case 0:
              fire_ddart(enemy[e].x - xpart(angle, 18000), enemy[e].y - ypart(angle, 18000), angle, EBCOL_B3, 50, 120);
              enemy[e].status = 1;
              break;
             case 1:
              fire_dart(enemy[e].x + xpart(angle + ANGLE_4 - ANGLE_16, 16000), enemy[e].y + ypart(angle + ANGLE_4 - ANGLE_16, 16000), angle - ANGLE_32, EBCOL_B2, 50, 90);
              fire_dart(enemy[e].x + xpart(angle - ANGLE_4 + ANGLE_16, 16000), enemy[e].y + ypart(angle - ANGLE_4 + ANGLE_16, 16000), angle + ANGLE_32, EBCOL_B2, 50, 90);
              enemy[e].status = 2;
              break;
             case 2:
              fire_dart(enemy[e].x + xpart(angle + ANGLE_4 + ANGLE_8, 16000), enemy[e].y + ypart(angle + ANGLE_4 + ANGLE_8, 16000), angle + ANGLE_16, EBCOL_B1, 50, 70);
              fire_dart(enemy[e].x + xpart(angle + ANGLE_4 + ANGLE_8, 16000), enemy[e].y + ypart(angle + ANGLE_4 + ANGLE_8, 16000), angle - ANGLE_32, EBCOL_B1, 50, 70);
              fire_dart(enemy[e].x + xpart(angle - ANGLE_4 - ANGLE_8, 16000), enemy[e].y + ypart(angle - ANGLE_4 - ANGLE_8, 16000), angle + ANGLE_32, EBCOL_B1, 50, 70);
              fire_dart(enemy[e].x + xpart(angle - ANGLE_4 - ANGLE_8, 16000), enemy[e].y + ypart(angle - ANGLE_4 - ANGLE_8, 16000), angle - ANGLE_16, EBCOL_B1, 50, 70);
              enemy[e].status = 0;
              break;
         }
         enemy[e].recycle = 20;
      }
     }
     break;
     case ENEMY_STOPPER1:
     if (enemy[e].recycle <= 0)
     {
      if (synch_ready [EVENT_STOPPER1_FIRE])
      {
      synch_done [DONE_STOPPER1_FIRE] = 1;
      angle = grand(ANGLE_1);
      angle_inc = ANGLE_1 / 30;
      if (grand(2) == 0)
       angle_inc *= -1;
      for (i = 0; i < 6; i ++)
      {
       angle &= 1023;
       fire_dart(enemy[e].x + xpart(angle, 24000), enemy[e].y + ypart(angle, 24000), angle, EBCOL_B5, 50, 120);
       angle += angle_inc;
       angle &= 1023;
       fire_dart(enemy[e].x + xpart(angle, 24000), enemy[e].y + ypart(angle, 24000), angle, EBCOL_B5, 50, 100);
       angle += angle_inc;
       angle &= 1023;
       fire_dart(enemy[e].x + xpart(angle, 24000), enemy[e].y + ypart(angle, 24000), angle, EBCOL_B5, 50, 80);
       if (arena.difficulty > 0)
       {
        angle += angle_inc;
        angle &= 1023;
        fire_dart(enemy[e].x + xpart(angle, 24000), enemy[e].y + ypart(angle, 24000), angle, EBCOL_B5, 50, 60);
        angle += angle_inc;
        angle &= 1023;
        fire_dart(enemy[e].x + xpart(angle, 24000), enemy[e].y + ypart(angle, 24000), angle, EBCOL_B5, 50, 40);
         if (arena.difficulty > 1)
         {
          angle += angle_inc;
          angle &= 1023;
          fire_dart(enemy[e].x + xpart(angle, 24000), enemy[e].y + ypart(angle, 24000), angle, EBCOL_B5, 50, 20);
          angle -= angle_inc;
          angle &= 1023;
         }
       }
        else
        {
         angle += angle_inc * 2;
         angle &= 1023;
        }
       enemy[e].recycle = 90 - arena.difficulty * 15;
      }
      }
       else enemy[e].target_time ++;
     }
     break;
     case ENEMY_CARRIER1:
      enemy[e].angle1 &= 1023;
      if (enemy[e].target_time > eclass[ENEMY_CARRIER1].speed2 - 20)
      {
       enemy[e].angle1_turning += odd_even(e);
       enemy[e].angle1 += enemy[e].angle1_turning;
       break;
      }
      if (enemy[e].target_time < 10)
      {
  /*     if (enemy[e].angle1_turning > 1
           || enemy[e].angle1_turning < -1)
            enemy[e].angle1_turning -= odd_even(e);*/
       if ((enemy[e].angle1 < ANGLE_1 -ANGLE_4
           && enemy[e].angle1 + enemy[e].angle1_turning >= ANGLE_1 -ANGLE_4)
        || (enemy[e].angle1 > ANGLE_1 -ANGLE_4
           && enemy[e].angle1 + enemy[e].angle1_turning <= ANGLE_1 -ANGLE_4))
       {
        enemy[e].angle1_turning = 0;
        enemy[e].target_time = 0;
        enemy[e].angle1 = -ANGLE_4;
       }
        else
         enemy[e].target_time ++;

       enemy[e].angle1 += enemy[e].angle1_turning;


//       if ((enemy[e].angle1 + ANGLE_4) % ANGLE_3 == 0)
//       if ((enemy[e].angle1 + ANGLE_4) % ANGLE_3 == 0)
       break;
      }
       else
       {
        if (synch_ready [EVENT_SWOOPER1_FIRE])
        {
         synch_done [DONE_SWOOPER1_FIRE] |= FIRE1_CARRIER1;
         angle = enemy[e].angle1;
         for (i = 0; i < 3; i ++)
         {
//          fire_diamond(enemy[e].x + xpart(angle, 22000), enemy[e].y + ypart(angle, 22000), angle + ANGLE_4, EBCOL_B2, 1700, 6, 50);
//          fire_diamond(enemy[e].x + xpart(angle, 22000), enemy[e].y + ypart(angle, 22000), angle - ANGLE_4, EBCOL_B3, 2000, -8, 50);
          if (arena.difficulty > 0)
           fire_diamond(enemy[e].x + xpart(angle, 22000), enemy[e].y + ypart(angle, 22000), angle + ANGLE_4, EBCOL_B2, 1700 + arena.difficulty * 50, 6, 50);
          fire_diamond(enemy[e].x + xpart(angle, 22000), enemy[e].y + ypart(angle, 22000), angle - ANGLE_4, EBCOL_B3, 1800 + arena.difficulty * 150, -8, 50);
          angle += ANGLE_3;
          angle &= 1023;
         }
       }

       }
      enemy[e].angle1 += enemy[e].angle1_turning;
      break;
     case ENEMY_WINGS:
     if (enemy[e].recycle <= 0)
     {
      if (synch_ready [EVENT_FIRE3_2] && (synch_done [DONE_FIRE3_2] & FIRE3_WINGS) == 0)
      {
       synch_done [DONE_FIRE3_2] |= FIRE3_WINGS;
       angle = attack_angle(e);
       fire_burst(enemy[e].x, enemy[e].y - 10000, angle, EBCOL_B2, 1700 + arena.difficulty * 50, 30);
       enemy[e].recycle = 40 - arena.difficulty * 10;
      }
     }
      break;
     case ENEMY_WINGS2:
     if (enemy[e].recycle <= 0)
     {
      if (synch_ready [EVENT_FIRE3_2] && (synch_done [DONE_FIRE3_2] & FIRE3_WINGS2) == 0)
      {
       synch_done [DONE_FIRE3_2] |= FIRE3_WINGS2;
       angle = attack_angle(e);
       fire_stream(enemy[e].x, enemy[e].y - 10000, angle, EBCOL_B1, 2700 + arena.difficulty * 150, 90);
       enemy[e].recycle = 20 - arena.difficulty * 7;
      }
     }
      break;
     case ENEMY_MEGA:
     if (enemy[e].recycle <= 0)
     {
      if (synch_ready [EVENT_FIRE3_1])
      {
       synch_done [DONE_FIRE3_1] |= FIRE3_MEGA;
       angle = grand(ANGLE_1);
       if (enemy[e].status == 0)
       {
        angle_inc = ANGLE_1 / (12 + arena.difficulty * 3);
        for (i = 0; i < 12 + arena.difficulty * 3; i ++)
        {
           fire_sdart(enemy[e].x + xpart(angle, 9000), enemy[e].y + ypart(angle, 9000), angle, EBCOL_B5, 50, 90);
           angle += angle_inc;
           angle &= 1023;
        }
        enemy[e].status = 1;
       }
        else
        {
         angle_inc = ANGLE_1 / (8 + arena.difficulty * 2);
         for (i = 0; i < 8 + arena.difficulty * 2; i ++)
         {
           fire_dart(enemy[e].x + xpart(angle, 9000), enemy[e].y + ypart(angle, 9000), angle, EBCOL_B5, 50, 70);
           angle += angle_inc;
           angle &= 1023;
           enemy[e].status = 0;
         }

        }
       enemy[e].recycle = 40 - arena.difficulty * 10;
      }
     }

      break;
     case ENEMY_HUNTER:
     if (enemy[e].recycle <= 0)
     {
      if (synch_ready [EVENT_FIRE4_2] && !(synch_done [DONE_FIRE4_2] & FIRE4_HUNTER))
      {
       synch_done [DONE_FIRE4_2] |= FIRE4_HUNTER;
       enemy[e].recycle = 90 - arena.difficulty * 15;
       fire_tracker(enemy[e].x - 6000, enemy[e].y - 18000, ANGLE_2 + ANGLE_4 - ANGLE_32, EBCOL_B2, 4, 500);
       fire_tracker(enemy[e].x + 6000, enemy[e].y - 18000, ANGLE_2 + ANGLE_4 + ANGLE_32, EBCOL_B2, 4, 500);

       fire_tracker(enemy[e].x - 13000, enemy[e].y - 10000, ANGLE_2 + ANGLE_8, EBCOL_B2, 4, 500);
       fire_tracker(enemy[e].x + 13000, enemy[e].y - 10000, - ANGLE_8, EBCOL_B2, 4, 500);

       fire_tracker(enemy[e].x - 16000, enemy[e].y + 1000, ANGLE_2, EBCOL_B2, 4, 500);
       fire_tracker(enemy[e].x + 16000, enemy[e].y + 1000, 0, EBCOL_B2, 4, 500);

       fire_tracker(enemy[e].x - 13000, enemy[e].y + 12000, ANGLE_4 + ANGLE_8, EBCOL_B2, 4, 500);
       fire_tracker(enemy[e].x + 13000, enemy[e].y + 12000, ANGLE_8, EBCOL_B2, 4, 500);

      }
       else enemy[e].target_time ++;
     }
     break;

    }
    break;
   case 1: // opening
   case 3: // closing
    drag_enemy(e, 910);
//    if (arena.counter % 10 != 0)
//     enemy[e].target_time ++;
    switch(enemy[e].type)
    {
     case ENEMY_WINGS:
     case ENEMY_WINGS2:
      if (enemy[e].phase == 1)
      {
       enemy[e].petal_angle [0] += 7;
       enemy[e].petal_angle [1] -= 7;
       enemy[e].petal_angle [2] += 5;
       enemy[e].petal_angle [3] -= 5;
       enemy[e].petal_angle [4] += 3;
       enemy[e].petal_angle [5] -= 3;
      }
       else
       {
        enemy[e].petal_angle [0] -= 7;
        enemy[e].petal_angle [1] += 7;
        enemy[e].petal_angle [2] -= 5;
        enemy[e].petal_angle [3] += 5;
        enemy[e].petal_angle [4] -= 3;
        enemy[e].petal_angle [5] += 3;
        enemy[e].y_speed -= 150;
       }
      break;

    }
    break;
   case 4: // departing
    if (enemy[e].target_time > 980)
     enemy[e].y_speed += eclass[enemy[e].type].speed1 / 17;
    switch(enemy[e].type)
    {
     case ENEMY_BEAMER:
     case ENEMY_SHOOTER:
     if ((arena.counter & 1) == 0)
     {
       quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 27000, grand(300) - grand(300), 2000 + grand(2000) + enemy[e].y_speed, 5 + grand(5), TCOL_ORANGE, 0);
     }
      else
      {
       quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 27000, grand(300) - grand(300), 2000 + grand(2000) + enemy[e].y_speed, 10 + grand(10), TCOL_SH2, 0);
      }
      break;
     case ENEMY_RISER:
      quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 24000, enemy[e].x_speed + grand(300) - grand(300), enemy[e].y_speed + 1500 + grand(2500), 8 + grand(5), TCOL_SH2, 0);
//      quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 40000, enemy[e].y + 27000, enemy[e].x_speed + grand(300) - grand(300), enemy[e].y_speed + 1500 + grand(2500), 8 + grand(4), TCOL_SH2, 0);
      break;
    }
    break;
  }


 // persistent

 move_enemy(e);

 switch(enemy[e].type)
 {
  case ENEMY_HUNTER:
     enemy_pulse_slow(e, EVENT_PULSE);
     break;

  case ENEMY_RISER:
   if (enemy[e].status > 0)
    enemy[e].status --;
   quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 24000, enemy[e].x_speed + grand(300) - grand(300), enemy[e].y_speed + 1500 + grand(2500), 4 + grand(3), TCOL_SH2, 0);
   break;
  case ENEMY_BEAMER:
   if (enemy[e].phase == 1)
    track_target(e, player.x, player.y, enemy[e].angle1, 8);
   break;
  case ENEMY_MEGA:
   enemy_pulse(e, EVENT_PULSE);
   for (i = 0; i < 7; i ++)
   {
    enemy[e].petal_angle [i] += enemy[e].petal_angle_speed [i];
   }
   break;
  case ENEMY_WINGS:
  case ENEMY_WINGS2:
  enemy_pulse(e, EVENT_PULSE2);
       enemy[e].petal_angle [0] += xpart(arena.counter * 32, 4);
       enemy[e].petal_angle [2] += xpart(arena.counter * 32, 4);
       enemy[e].petal_angle [4] += xpart(arena.counter * 32, 4);
       enemy[e].petal_angle [1] -= xpart(arena.counter * 32, 4);
       enemy[e].petal_angle [3] -= xpart(arena.counter * 32, 4);
       enemy[e].petal_angle [5] -= xpart(arena.counter * 32, 4);
   break;
  case ENEMY_POD1:
    track_target(e, player.x, player.y, enemy[e].angle1, 4);
    break;

  case ENEMY_CARRIER1:
        if (enemy[e].part [0] != -1)
        {
         enemy[enemy[e].part [0]].x = enemy[e].x - 55000;
         enemy[enemy[e].part [0]].y = enemy[e].y - 3000;
         if (enemy[e].phase == 1)
         {
          enemy[enemy[e].part [0]].target_time = 40;
          launch_part(e, 0, enemy[e].part [0], -2000, 800);
         }
        }
        if (enemy[e].part [1] != -1)
        {
         enemy[enemy[e].part [1]].x = enemy[e].x + 55000;
         enemy[enemy[e].part [1]].y = enemy[e].y - 3000;
         if (enemy[e].phase == 1)
         {
          enemy[enemy[e].part [1]].target_time = 40;
          launch_part(e, 1, enemy[e].part [1], 2000, 800);
         }
        }
      break;
 }

}

void launch_part(int pe, int index, int se, int xs, int ys)
{
 enemy[se].x_speed = xs;
 enemy[se].y_speed = ys;
 enemy[se].parent = -1;

 enemy[pe].part [index] = -1;

}

void move_enemy(int e)
{
     enemy[e].x += enemy[e].x_speed;
/*
     if (enemy[e].x > X_MAX
         || enemy[e].x < X_MIN)
      enemy[e].x_speed *= -1; // bounce*/

     enemy[e].y += enemy[e].y_speed;
/*
     if (enemy[e].y > Y_MAX
         || enemy[e].y < Y_MIN)
      enemy[e].y_speed *= -1;
*/
// the edge collision should take account of enemy size, but it doesn't for now.

}

int fire_dart(int x, int y, int angle, int colour, int wait, int accel)
{
 int b = create_ebullet(EBULLET_SHOT);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = 0;
  ebullet[b].y_speed = 0;
  ebullet[b].timeout = 500;
  ebullet[b].accel = accel;
  return b;
 }
 return -1;
}

int fire_sdart(int x, int y, int angle, int colour, int wait, int accel)
{
 int b = create_ebullet(EBULLET_SDART);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = 0;
  ebullet[b].y_speed = 0;
  ebullet[b].timeout = 500;
  ebullet[b].accel = accel;
  return b;
 }
 return -1;
}

int fire_ddart(int x, int y, int angle, int colour, int wait, int accel)
{
 int b = create_ebullet(EBULLET_DDART);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = 0;
  ebullet[b].y_speed = 0;
  ebullet[b].timeout = 500;
  ebullet[b].accel = accel;
  return b;
 }
 return -1;

}

int fire_diamond(int x, int y, int angle, int colour, int speed, int rot, int wait)
{
 int b = create_ebullet(EBULLET_SHOT2);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(angle, speed);
  ebullet[b].y_speed = ypart(angle, speed);
  ebullet[b].timeout = 500;
  ebullet[b].angle2 = rot;
  return b;
 }
 return -1;

}

int fire_curve(int x, int y, int angle, int colour, int speed, int rot, int wait)
{
 int b = create_ebullet(EBULLET_CURVE);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = speed;//xpart(angle, speed);
//  ebullet[b].y_speed = pos_or_neg(2);//ypart(angle, speed);
  ebullet[b].y_speed = 2 + grand(4);
  if (rot < 0)
   ebullet[b].y_speed = -2 - grand(4);
  ebullet[b].status = ebullet[b].angle;
  ebullet[b].timeout = 600;
  ebullet[b].angle2 = rot;
  return b;
 }
 return -1;

}


int fire_wing_diamond(int x, int y, int angle, int colour, int speed, int wait)
{
 int b = create_ebullet(EBULLET_WING_DIAMOND);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(angle, speed);
  ebullet[b].y_speed = ypart(angle, speed);
  ebullet[b].timeout = 500;
  ebullet[b].status = pos_or_neg(8);
  ebullet[b].accel = 50 + arena.difficulty * 15;
  return b;
 }
 return -1;

}


int fire_seed(int x, int y, int angle, int colour, int wait, int accel)
{
 int b = create_ebullet(EBULLET_SEED);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].colour = grand(4);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(ebullet[b].angle, accel);
  ebullet[b].y_speed = ypart(ebullet[b].angle, accel);
  ebullet[b].timeout = 500;
  ebullet[b].accel = accel;
  return b;
 }
 return -1;

}



int fire_seed2(int x, int y, int angle, int colour, int wait, int accel)
{
 int b = create_ebullet(EBULLET_LSEED);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].colour = grand(4);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(ebullet[b].angle, accel);
  ebullet[b].y_speed = ypart(ebullet[b].angle, accel);
  ebullet[b].timeout = 500;
  ebullet[b].accel = accel;
  return b;
 }
 return -1;

}

int fire_burst(int x, int y, int angle, int colour, int speed, int wait)
{
 int b = create_ebullet(EBULLET_BURST);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(angle, speed);
  ebullet[b].y_speed = ypart(angle, speed);
  ebullet[b].timeout = 60 + grand(20);
  ebullet[b].status = pos_or_neg(8);
  ebullet[b].accel = 50;
  return b;
 }
 return -1;

}

int fire_tracker(int x, int y, int angle, int colour, int size, int speed)
{
 int b = create_ebullet(EBULLET_TRACKER);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
/*  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);*/
  ebullet[b].x_speed = xpart(angle, speed);
  ebullet[b].y_speed = ypart(angle, speed);
  ebullet[b].timeout = 500;
  ebullet[b].status = size;
  ebullet[b].cord_wait = -1;
  quick_cloud(CLOUD_CIRCLE5, ebullet[b].x, ebullet[b].y, 0, 0, 30, TCOL_B2, 0);
//  ebullet[b].accel = accel;
  return b;
 }
 return -1;
}


int fire_stream(int x, int y, int angle, int colour, int speed, int wait)
{
 int b = create_ebullet(EBULLET_STREAM);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(angle, speed);
  ebullet[b].y_speed = ypart(angle, speed);
  ebullet[b].timeout = 500;
//  ebullet[b].status = pos_or_neg(8);
  ebullet[b].accel = 50;
  return b;
 }
 return -1;

}

int fire_dot(int x, int y, int angle, int colour, int size, int speed, int wait)
{
 int b = create_ebullet(EBULLET_DOT);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(angle, speed);
  ebullet[b].y_speed = ypart(angle, speed);
  ebullet[b].timeout = 500;
  ebullet[b].status = size;
//  ebullet[b].accel = accel;
  return b;
 }
 return -1;
}

int fire_beam(int e, int dist)
{
 int length = 1;
 int x = enemy[e].x + xpart(enemy[e].angle1, dist);
 int y = enemy[e].y + ypart(enemy[e].angle1, dist);

 do
 {
  length ++;
  x += xpart(enemy[e].angle1, 45000);
  y += ypart(enemy[e].angle1, 45000);
  if (x > 690000
   || x < -50000
   || y > 530000
   || y < - 50000)
    break;
 } while(TRUE);
 int b = create_ebullet(EBULLET_BEAM);
 if (b != -1)
 {
  enemy[e].angle1 &= 1023;
  if (enemy[e].status > 100 || enemy[e].status < 20)
  {
    ebullet[b].x = enemy[e].x + xpart(enemy[e].angle1, dist + 4000);
    ebullet[b].y = enemy[e].y + ypart(enemy[e].angle1, dist + 4000);
  }
   else
   {
    ebullet[b].x = enemy[e].x + xpart(enemy[e].angle1, dist + 4000 - (enemy[e].status % 6) * GRAIN * 4);
    ebullet[b].y = enemy[e].y + ypart(enemy[e].angle1, dist + 4000 - (enemy[e].status % 6) * GRAIN * 4);
   }
  ebullet[b].status = length;
  ebullet[b].angle = enemy[e].angle1;
  ebullet[b].sprite_angle = enemy[e].angle1 / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = enemy[e].x + xpart(enemy[e].angle1, dist);
  ebullet[b].y_speed = enemy[e].y + ypart(enemy[e].angle1, dist);
  ebullet[b].timeout = 2;
  ebullet[b].angle2 = enemy[e].status;
  if (enemy[e].status > 100)
   ebullet[b].angle2 = 120 - enemy[e].status;
  return b;
 }
 return -1;
}

int fire_beam2(int e, int angle, int dist, int time)
{
 int length = 1;
 int x = enemy[e].x + xpart(angle, dist);
 int y = enemy[e].y + ypart(angle, dist);

 do
 {
  length ++;
  x += xpart(enemy[e].angle1, 45000);
  y += ypart(enemy[e].angle1, 45000);
  if (x > 690000
   || x < -50000
   || y > 530000
   || y < - 50000)
    break;
 } while(TRUE);
 if (length > (100 - enemy[e].status + 7) / 6)
  length = (100 - enemy[e].status + 7) / 6;
 int b = create_ebullet(EBULLET_BEAM2);
 if (b != -1)
 {
  angle &= 1023;
  if (enemy[e].status > 100 || enemy[e].status < 20)
  {
    ebullet[b].x = enemy[e].x + xpart(enemy[e].angle1, dist + 4000);
    ebullet[b].y = enemy[e].y + ypart(enemy[e].angle1, dist + 4000);
  }
   else
   {
    ebullet[b].x = enemy[e].x + xpart(enemy[e].angle1, dist + 4000 - ((enemy[e].status * 2) % 12) * GRAIN * 4);
    ebullet[b].y = enemy[e].y + ypart(enemy[e].angle1, dist + 4000 - ((enemy[e].status * 2) % 12) * GRAIN * 4);
   }
  ebullet[b].status = length;
  ebullet[b].angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = enemy[e].x + xpart(angle, dist);
  ebullet[b].y_speed = enemy[e].y + ypart(angle, dist);
  ebullet[b].timeout = 2;
  ebullet[b].angle2 = enemy[e].status;
//  if (enemy[e].status > 100)
//   ebullet[b].angle2 = 120 - enemy[e].status;
  return b;
 }
 return -1;
}



void enemy_fire(int e)
{
 int angle = attack_angle(e);
 int btype = EBULLET_SHOT + grand(2);
 btype = EBULLET_SHOT;
 int b = create_ebullet(btype);
 if (b != -1)
 {
  switch(btype)
  {
  case EBULLET_SHOT:
  ebullet[b].x = enemy[e].x + xpart(angle, 10000);
  ebullet[b].y = enemy[e].y + ypart(angle, 10000);
  ebullet[b].angle = angle;
  ebullet[b].colour = EBCOL_B1;//grand(5);
  set_cord_colour(b);
//ebullet[b].cord_colour = TRANS_ORANGE_OUT;
  ebullet[b].cord_x = ebullet[b].x;
  ebullet[b].cord_y = ebullet[b].y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = 50;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = 0;//xpart(angle, 3000);
  ebullet[b].y_speed = 0;//ypart(angle, 3000);
  ebullet[b].timeout = 500;
//  play_effectwfvx(WAV_FIRE, 500 + grand(1000), 200, ebullet[b].x);
  enemy[e].x_speed -= xpart(angle, 400);
  enemy[e].y_speed -= ypart(angle, 400); // a little bit of recoil
   enemy[e].recycle = 50;
  ebullet[b].accel = 60;
  break;
  case EBULLET_SHOT2:
  ebullet[b].x = enemy[e].x + xpart(angle, 10000);
  ebullet[b].y = enemy[e].y + ypart(angle, 10000);
  ebullet[b].angle = angle;
  ebullet[b].colour = EBCOL_B1;//grand(3);
  if (grand(5) == 0)
   ebullet[b].colour = grand(5);
  set_cord_colour(b);
  ebullet[b].cord_x = ebullet[b].x;
  ebullet[b].cord_y = ebullet[b].y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = 50;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(angle, 2000);
  ebullet[b].y_speed = ypart(angle, 2000);
  ebullet[b].timeout = 500;
  play_effectwfvx(WAV_FIRE, 500 + grand(1000), 200, ebullet[b].x);
  enemy[e].x_speed -= xpart(angle, 400);
  enemy[e].y_speed -= ypart(angle, 400); // a little bit of recoil
  ebullet[b].angle2 = -10 + grand(11);
//  if (grand(2) == 0)
   //ebullet[b].angle2 = -10;
   enemy[e].recycle = 50;
   break;
  }
 }
//      play_effectwfvx(w, NWAV_ZAP, 1800 + grand(50), 30, enemy[e].x);
}

void set_cord_colour(int b)
{
  switch(ebullet[b].colour)
  {
   case EBCOL_B1: ebullet[b].cord_colour = TRANS_B1_OUT; break;
   case EBCOL_B2: ebullet[b].cord_colour = TRANS_B2_OUT; break;
   case EBCOL_B3: ebullet[b].cord_colour = TRANS_B3_OUT; break;
   case EBCOL_B4: ebullet[b].cord_colour = TRANS_B4_OUT; break;
   case EBCOL_B5: ebullet[b].cord_colour = TRANS_B5_OUT; break;
  }

}

// atan2 is slow, so don't use it too much.
// see stuff.c for radians_to_angle etc.
int attack_angle(int e)
{
 return radians_to_angle(atan2(player.y - enemy[e].y, player.x - enemy[e].x));
}

int get_e_angle(int e, int x, int y)
{
 return radians_to_angle(atan2(y - enemy[e].y, x - enemy[e].x));
}

void track_target(int e, int x, int y, int angle, int turn)
{


      int angle_move = delta_turn_towards_xy(enemy[e].x, enemy[e].y,
          x, y, angle, 1);

//      textprintf_ex(screen, font, 5, 5, COLOUR_11, COLOUR_2, "%i %i %i %i %i %i    ", e, enemy[w] [e].type, enemy[w] [e].x, enemy[w] [e].y, turn, angle_move);


      if (angle_move != enemy[e].angle1_turning)
      {
       if (enemy[e].angle1_turning_count <= 0)
       {
        enemy[e].angle1_turning_count = 15;
        enemy[e].angle1_turning = angle_move;
/*        if (angle_move < 0)
         enemy[e].angle1_turning = -1;
          else
           enemy[e].angle1_turning = 1;*/
        return;

       }
       enemy[e].angle1_turning_count --;
       return;
      }

      enemy[e].angle1_turning_count --;
      enemy[e].angle1 += turn * enemy[e].angle1_turning;

      enemy[e].angle1 &= 1023;

}

void track_target_zero(int e, int x, int y, int angle, int turn)
{


      int angle_move = delta_turn_towards_xy(enemy[e].x, enemy[e].y,
          x, y, angle, 1);

//      textprintf_ex(screen, font, 5, 5, COLOUR_11, COLOUR_2, "%i %i %i %i %i %i    ", e, enemy[w] [e].type, enemy[w] [e].x, enemy[w] [e].y, turn, angle_move);


      if (angle_move != enemy[e].angle_turning)
      {
       if (enemy[e].angle_turning_count <= 0)
       {
        enemy[e].angle_turning_count = 15;
        enemy[e].angle_turning = angle_move;
/*        if (angle_move < 0)
         enemy[e].angle1_turning = -1;
          else
           enemy[e].angle1_turning = 1;*/
        return;

       }
       enemy[e].angle_turning_count --;
       return;
      }

      enemy[e].angle_turning_count --;
      enemy[e].angle += turn * enemy[e].angle_turning;

      enemy[e].angle &= 1023;

}

void drag_enemy(int e, int drag)
{
 enemy[e].x_speed *= drag;
 enemy[e].x_speed /= 1000;
 enemy[e].y_speed *= drag;
 enemy[e].y_speed /= 1000;

}

void drag_enemy_y(int e, int drag)
{
 enemy[e].y_speed *= drag;
 enemy[e].y_speed /= 1000;

}


void pbullet_hits_enemy(int b, int e)
{

 if (pbullet[b].type == PBULLET_BLADE)
 {
  int rel_x = abs(pbullet[b].x_speed - enemy[e].x_speed);
  //if (rel_x < 2000)
   //rel_x = 2000;
  int rel_y = abs(pbullet[b].y_speed - enemy[e].y_speed);
//  if (rel_y < 2000)
//   rel_y = 2000;
  int dam = (pbullet[b].dam + (pbullet[b].dam * hypot(rel_y, rel_x) / 200)) / 160;
  hurt_enemy(e, dam, pbullet[b].source);
//  player.score = dam;
  return;
 }

 hurt_enemy(e, pbullet[b].dam, pbullet[b].source);
}
// note explosion in pbullet.c calls hurt_enemy directly

int hurt_enemy(int e, int dam, int source)
{

 if (enemy[e].type == ENEMY_BOSS1
  || enemy[e].type == ENEMY_BOSS2
  || enemy[e].type == ENEMY_BOSS3
  || enemy[e].type == ENEMY_BOSS4)
 {
  boss.hp -= dam;
  if (boss.hp <= 0)
  {
   boss.hp = 0;
   enemy_explode(e);
   if (arena.game_over == 0)
    arena.level_finished = 200;
  }
  return 1;
 }


 enemy[e].hp -= dam;

 if (enemy[e].hp <= 0)
 {
  enemy[e].whurt [source] += enemy[e].hp + dam;
  enemy_explode(e);
  return 0;
 }

 enemy[e].whurt [source] += dam;

 return 1;
// deal with source/xp here

}


void enemy_explode(int e)
{

 int x = enemy[e].x;
 int y = enemy[e].y;
 int angle;
 int i;
 int c;
 int xa;

 switch(enemy[e].type)
 {
  case ENEMY_SWOOPER1:
   play_effectwfvx(WAV_DRUM3, tone [NOTE_1E], 250, x);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 20, 0, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 30, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 30, TCOL_YELLOW, 0);
   angle = grand(ANGLE_1);
   for (i = 0; i < 4; i ++)
   {
    angle += ANGLE_4;
    quick_cloud(CLOUD_CIRCLE1, x, y, xpart(angle, 2000), ypart(angle, 2000), 20, TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE1, x, y, xpart(angle, 1500), ypart(angle, 1500), 20, TCOL_YELLOW, 0);
   }
/*   quick_cloud(CLOUD_CIRCLE1, x, y, -1500, 1500, 20, 7, 0);
   quick_cloud(CLOUD_CIRCLE1, x, y, 1500, -1500, 20, 7, 0);
   quick_cloud(CLOUD_CIRCLE1, x, y, -1500, -1500, 20, 7, 0);*/
   break;
  case ENEMY_SWOOPER2:
   play_effectwfvx(WAV_DRUM3, tone [NOTE_1E], 250, x);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 20, 0, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 40, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 40, TCOL_YELLOW, 0);
   angle = grand(ANGLE_1);
   for (i = 0; i < 7; i ++)
   {
    angle += ANGLE_1 / 7;
    quick_cloud(CLOUD_CIRCLE1, x, y, xpart(angle, 1200), ypart(angle, 1200), 30, TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x, y, xpart(angle, 1220), ypart(angle, 1220), 30, TCOL_YELLOW, 0);
   }
   break;
  case ENEMY_DARTER2:
   play_effectwfvx(WAV_BANG, tone [NOTE_1G], 250, x);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 30, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 30, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_MSHOCK, x - 15000, y, 0, 0, 20, 0, 0);
   quick_cloud(CLOUD_MSHOCK, x + 15000, y, 0, 0, 20, 0, 0);
   angle = -ANGLE_4 + ANGLE_6;
   for (i = 0; i < 3; i ++)
   {
    angle += ANGLE_6;
    quick_cloud(CLOUD_CIRCLE1, x + xpart(angle, 43000), y + ypart(angle, 28000), xpart(angle, 1000), ypart(angle, 1000), 20, TCOL_YELLOW, 0);
    angle += ANGLE_6;
    quick_cloud(CLOUD_CIRCLE3, x + xpart(angle, 43000), y + ypart(angle, 28000), xpart(angle, 1000), ypart(angle, 1000), 30, TCOL_ORANGE, 0);
   }
   break;
  case ENEMY_DARTER1:
   play_effectwfvx(WAV_BANG, tone [NOTE_1G], 250, x);
//   play_effectwfvx(WAV_CYMBAL1, tone [NOTE_1C], 250, x);
//   play_effectwfvx(WAV_DRUM3, tone [NOTE_2G], 250, x);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 30, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 30, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 20, 0, 0);
//   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 16, 1, 0);
   angle = -ANGLE_4 + ANGLE_6;
   for (i = 0; i < 3; i ++)
   {
    angle += ANGLE_6;
    quick_cloud(CLOUD_CIRCLE1, x + xpart(angle, 28000), y + ypart(angle, 28000), xpart(angle, 1000), ypart(angle, 1000), 20, TCOL_YELLOW, 0);
    angle += ANGLE_6;
    quick_cloud(CLOUD_CIRCLE3, x + xpart(angle, 28000), y + ypart(angle, 28000), xpart(angle, 1000), ypart(angle, 1000), 30, TCOL_ORANGE, 0);
/*    angle += ANGLE_6;
    quick_cloud(CLOUD_CIRCLE1, x + xpart(angle, 28000), y + ypart(angle, 28000), 0, 0, 20, 8, 0);
    angle += ANGLE_6;
    quick_cloud(CLOUD_CIRCLE3, x + xpart(angle, 28000), y + ypart(angle, 28000), 0, 0, 30, 7, 0);*/
   }
   break;
//  case ENEMY_DIVER1:
// something else
   play_effectwfvx(WAV_BANG, tone [NOTE_1C], 250, x);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 30, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 30, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 20, 0, 0);
//   for (i = 0; i < 5; i ++)
//   {
    quick_cloud(CLOUD_SPAWNER, x + 8000, y, 4000, 0, 12, TCOL_ORANGE, 950);
    quick_cloud(CLOUD_SPAWNER, x - 8000, y, - 4000, 0, 12, TCOL_ORANGE, 950);
    quick_cloud(CLOUD_SPAWNER, x + 8000, y, 6000, 0, 10, TCOL_YELLOW, 950);
    quick_cloud(CLOUD_SPAWNER, x - 8000, y, - 6000, 0, 10, TCOL_YELLOW, 950);
/*    quick_cloud(CLOUD_SPAWNER, x + 8000, y, 5000, -1000, 12, 7, 950);
    quick_cloud(CLOUD_SPAWNER, x - 8000, y, - 5000, -1000, 12, 7, 950);
    quick_cloud(CLOUD_SPAWNER, x + 8000, y, 5000, 1000, 12, 7, 950);
    quick_cloud(CLOUD_SPAWNER, x - 8000, y, - 5000, 1000, 12, 7, 950);*/
//    quick_cloud(CLOUD_CIRCLE10, x + 8000, y, 2000 + grand(1000) + 500 * i, grand(1000) - 500, 90, 7, 900 + i * 10);
//    quick_cloud(CLOUD_CIRCLE10, x - 8000, y, - 2000 - grand(1000) - 500 * i, grand(1000) - 500, 90, 7, 900 + i * 10);
//   }
   break;
  case ENEMY_DIVER1:
   play_effectwfvx(WAV_BANG, tone [NOTE_1C], 250, x);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 30, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 30, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 20, 0, 0);

   quick_cloud(CLOUD_CIRCLE2, x - 4000, y, -1000, 0, 40, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE3, x - 4000, y, -1000, 0, 30, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_CIRCLE2, x - 8000, y, -2000, 0, 55, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE3, x - 8000, y, -2000, 0, 45, TCOL_YELLOW, 0);

   quick_cloud(CLOUD_CIRCLE2, x + 4000, y, 1000, 0, 40, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE3, x + 4000, y, 1000, 0, 30, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_CIRCLE2, x + 8000, y, 2000, 0, 55, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE3, x + 8000, y, 2000, 0, 45, TCOL_YELLOW, 0);

/*    quick_cloud(CLOUD_SPAWNER, x + 8000, y, 4000, 0, 12, TCOL_ORANGE, 950);
    quick_cloud(CLOUD_SPAWNER, x - 8000, y, - 4000, 0, 12, TCOL_ORANGE, 950);
    quick_cloud(CLOUD_SPAWNER, x + 8000, y, 6000, 0, 10, TCOL_YELLOW, 950);
    quick_cloud(CLOUD_SPAWNER, x - 8000, y, - 6000, 0, 10, TCOL_YELLOW, 950);*/
/*    quick_cloud(CLOUD_SPAWNER, x + 8000, y, 5000, -1000, 12, 7, 950);
    quick_cloud(CLOUD_SPAWNER, x - 8000, y, - 5000, -1000, 12, 7, 950);
    quick_cloud(CLOUD_SPAWNER, x + 8000, y, 5000, 1000, 12, 7, 950);
    quick_cloud(CLOUD_SPAWNER, x - 8000, y, - 5000, 1000, 12, 7, 950);*/
//    quick_cloud(CLOUD_CIRCLE10, x + 8000, y, 2000 + grand(1000) + 500 * i, grand(1000) - 500, 90, 7, 900 + i * 10);
//    quick_cloud(CLOUD_CIRCLE10, x - 8000, y, - 2000 - grand(1000) - 500 * i, grand(1000) - 500, 90, 7, 900 + i * 10);
//   }
   break;
  case ENEMY_STOPPER1:
   play_effectwfvx(WAV_NICE, tone [NOTE_2C], 250, x);
   play_effectwfvx(WAV_DRUM4, tone [NOTE_1C], 250, x);
/*   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 70, 8, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y - 35000, 0, 0, 50, 8, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y - 60000, 0, 0, 70, 8, 0);
   quick_cloud(CLOUD_CIRCLE2, x + 30000, y + 30000, 0, 0, 60, 8, 0);
   quick_cloud(CLOUD_CIRCLE2, x - 30000, y + 30000, 0, 0, 60, 8, 0);
   quick_cloud(CLOUD_CIRCLE5, x + 50000, y - 10000, 0, 0, 50, 8, 0);
   quick_cloud(CLOUD_CIRCLE5, x - 50000, y - 10000, 0, 0, 50, 8, 0);
   quick_cloud(CLOUD_CIRCLE5, x + 35000, y - 50000, 0, 0, 30, 8, 0);
   quick_cloud(CLOUD_CIRCLE5, x - 35000, y - 50000, 0, 0, 30, 8, 0);
   quick_cloud(CLOUD_HSHOCK, x, y, 0, 0, 50, 0, 0);*/
      quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 70, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y - 35000, 0, 0, 50, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y - 60000, 0, 0, 70, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x + 30000, y + 30000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x - 30000, y + 30000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE5, x + 50000, y - 10000, 0, 0, 50, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE5, x - 50000, y - 10000, 0, 0, 50, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE3, x + 35000, y - 50000, 0, 0, 70, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE3, x - 35000, y - 50000, 0, 0, 70, TCOL_SH2, 0);
   quick_cloud(CLOUD_HSHOCK, x, y, 0, 0, 50, 2, 0);
   break;
  case ENEMY_CARRIER1:
   play_effectwfvx(WAV_NICE, tone [NOTE_1A], 250, x);
   play_effectwfvx(WAV_DRUM4, tone [NOTE_1C], 250, x);
/*   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 70, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y - 35000, 0, 0, 50, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y - 60000, 0, 0, 70, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x + 30000, y + 30000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x - 30000, y + 30000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE5, x + 50000, y - 10000, 0, 0, 50, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE5, x - 50000, y - 10000, 0, 0, 50, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE3, x + 35000, y - 50000, 0, 0, 70, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE3, x - 35000, y - 50000, 0, 0, 70, TCOL_SH2, 0);*/
   angle = quick_cloud(CLOUD_CARRIER1_BANG, x, y, 0, 0, 50, 0, 0);
   cloud[angle].x2 = 0;
   quick_cloud(CLOUD_HSHOCK, x, y, 0, 0, 50, 2, 0);
   quick_cloud(CLOUD_CIRCLE1, x, y, 0, 0, 50, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 50, TCOL_SH2, 0);
   break;
  case ENEMY_POD1:
   play_effectwfvx(WAV_BANG, tone [NOTE_0G], 250, x);
   angle = grand(ANGLE_1);
   for (i = 0; i < 8; i ++)
   {
    angle += ANGLE_8;
    angle &= 1023;
    quick_cloud(CLOUD_CIRCLE1, x + xpart(angle, 10000), y + ypart(angle, 10000), xpart(angle, 1800), ypart(angle, 1800), 60, TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x + xpart(angle, 10000), y + ypart(angle, 10000), xpart(angle, 1800), ypart(angle, 1800), 50, TCOL_YELLOW, 0);
   }
   quick_cloud(CLOUD_CIRCLE1, x, y - 20000, 0, -3000, 60, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y - 20000, 0, -3000, 50, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_HSHOCK, x, y, 0, 0, 50, 0, 0);
   break;
  case ENEMY_SMALL:
   play_effectwfvx(WAV_DRUM3, tone [NOTE_1E], 250, x);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 50, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 50, TCOL_YELLOW, 0);
   for (i = 0; i < 3; i ++)
   {
    c = quick_cloud(CLOUD_PETAL1_DEBRIS, x + xpart(enemy[e].petal_angle [i], 36000), y + ypart(enemy[e].petal_angle [i], 36000), enemy[e].x_speed + xpart(enemy[e].petal_angle [i], 1800), enemy[e].y_speed + ypart(enemy[e].petal_angle [i], 1800), 5000, 0, 0);
    if (c != -1)
    {
     cloud[c].angle = enemy[e].petal_angle [i];
     cloud[c].x2 = enemy[e].petal_angle_speed [i];
    }
   }
   break;
  case ENEMY_SMALL2:
   play_effectwfvx(WAV_DRUM3, tone [NOTE_1C], 250, x);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 50, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 50, TCOL_YELLOW, 0);
   for (i = 0; i < 3; i ++)
   {
    c = quick_cloud(CLOUD_PETAL1_DEBRIS, x + xpart(enemy[e].petal_angle [i], 36000), y + ypart(enemy[e].petal_angle [i], 36000), enemy[e].x_speed + xpart(enemy[e].petal_angle [i], 1800), enemy[e].y_speed + ypart(enemy[e].petal_angle [i], 1800), 5000, 0, 0);
    if (c != -1)
    {
     if (i > 0)
      cloud[c].type = CLOUD_PETAL2_DEBRIS;
     cloud[c].angle = enemy[e].petal_angle [i];
     cloud[c].x2 = enemy[e].petal_angle_speed [i];
    }
   }
   break;
  case ENEMY_LARGER:
   play_effectwfvx(WAV_DRUM3, tone [NOTE_0G], 250, x);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 50, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 50, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 20, 0, 0);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 40, 1, 0);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 60, 0, 0);
/*   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 30, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 30, TCOL_SH2, 0);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 20, 2, 0);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 40, 3, 0);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 60, 2, 0);*/
   for (i = 0; i < 4; i ++)
   {
    c = quick_cloud(CLOUD_PETAL1_DEBRIS, x + xpart(enemy[e].petal_angle [i], 43000), y + ypart(enemy[e].petal_angle [i], 43000), enemy[e].x_speed + xpart(enemy[e].petal_angle [i], 1800), enemy[e].y_speed + ypart(enemy[e].petal_angle [i], 1800), 5000, 0, 0);
//    c = quick_cloud(CLOUD_PETAL1, x + xpart(enemy[e].petal_angle [i], 33000), y + ypart(enemy[e].petal_angle [i], 33000), 0, 0, 50, 0, 0);
    if (c != -1)
    {
     cloud[c].angle = enemy[e].petal_angle [i];
     cloud[c].x2 = enemy[e].petal_angle_speed [i];
     if (cloud[c].x_speed == 0 && cloud[c].y_speed == 0)
     {
         cloud[c].y_speed = 500;
     }
    }
   }
   break;
  case ENEMY_LARGER2:
   play_effectwfvx(WAV_DRUM3, tone [NOTE_0G], 250, x);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 50, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 50, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 20, 0, 0);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 40, 1, 0);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 60, 0, 0);
   for (i = 0; i < 3; i ++)
   {
    c = quick_cloud(CLOUD_PETAL1_DEBRIS, x + xpart(enemy[e].petal_angle [i], 43000), y + ypart(enemy[e].petal_angle [i], 43000), enemy[e].x_speed + xpart(enemy[e].petal_angle [i], 1800), enemy[e].y_speed + ypart(enemy[e].petal_angle [i], 1800), 5000, 0, 0);
    if (c != -1)
    {
     cloud[c].angle = enemy[e].petal_angle [i];
     cloud[c].x2 = enemy[e].petal_angle_speed [i];
     if (cloud[c].x_speed == 0 && cloud[c].y_speed == 0)
     {
         cloud[c].y_speed = 500;
     }
    }
   }
   for (i = 3; i < 6; i ++)
   {
    c = quick_cloud(CLOUD_PETAL2_DEBRIS, x + xpart(enemy[e].petal_angle [i], 33000), y + ypart(enemy[e].petal_angle [i], 33000), enemy[e].x_speed + xpart(enemy[e].petal_angle [i], 2400), enemy[e].y_speed + ypart(enemy[e].petal_angle [i], 2400), 5000, 0, 0);
    if (c != -1)
    {
     cloud[c].angle = enemy[e].petal_angle [i];
     cloud[c].x2 = enemy[e].petal_angle_speed [i];
     if (cloud[c].x_speed == 0 && cloud[c].y_speed == 0)
     {
         cloud[c].y_speed = 500;
     }
    }
   }
   break;
  case ENEMY_WINGS:
   play_effectwfvx(WAV_BANG, tone [NOTE_2C], 250, x);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 80, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 80, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_LSHOCK, x, y, 0, 0, 50, 2, 0);
   for (i = 0; i < 2; i ++)
   {
    c = quick_cloud(CLOUD_PETAL3_DEBRIS, x + xpart(enemy[e].petal_angle [i], 55000), y + ypart(enemy[e].petal_angle [i], 55000), enemy[e].x_speed + xpart(enemy[e].petal_angle [i], 1800), enemy[e].y_speed + ypart(enemy[e].petal_angle [i], 1800), 5000, 0, 0);
    if (c != -1)
    {
     cloud[c].angle = enemy[e].petal_angle [i];
     cloud[c].x2 = grand(8) - grand(8);
     if (cloud[c].x_speed == 0 && cloud[c].y_speed == 0)
     {
         cloud[c].y_speed = 500;
     }
    }
   }
   for (i = 2; i < 4; i ++)
   {
    c = quick_cloud(CLOUD_PETAL1_DEBRIS, x + xpart(enemy[e].petal_angle [i], 42000), y + ypart(enemy[e].petal_angle [i], 42000), enemy[e].x_speed + xpart(enemy[e].petal_angle [i], 2100), enemy[e].y_speed + ypart(enemy[e].petal_angle [i], 2100), 5000, 0, 0);
    if (c != -1)
    {
     cloud[c].angle = enemy[e].petal_angle [i];
     cloud[c].x2 = grand(8) - grand(8);
     if (cloud[c].x_speed == 0 && cloud[c].y_speed == 0)
     {
         cloud[c].y_speed = 500;
     }
    }
   }
   for (i = 4; i < 6; i ++)
   {
    c = quick_cloud(CLOUD_PETAL2_DEBRIS, x + xpart(enemy[e].petal_angle [i], 35000), y + ypart(enemy[e].petal_angle [i], 35000), enemy[e].x_speed + xpart(enemy[e].petal_angle [i], 2400), enemy[e].y_speed + ypart(enemy[e].petal_angle [i], 2400), 5000, 0, 0);
    if (c != -1)
    {
     cloud[c].angle = enemy[e].petal_angle [i];
     cloud[c].x2 = grand(8) - grand(8);
     if (cloud[c].x_speed == 0 && cloud[c].y_speed == 0)
     {
         cloud[c].y_speed = 500;
     }
    }
   }
   break;
  case ENEMY_WINGS2:
   play_effectwfvx(WAV_BANG, tone [NOTE_1G], 250, x);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 80, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 80, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_LSHOCK, x, y, 0, 0, 50, 2, 0);
   for (i = 0; i < 6; i ++)
   {
    c = quick_cloud(CLOUD_PETAL1_DEBRIS, x + xpart(enemy[e].petal_angle [i], 42000), y + ypart(enemy[e].petal_angle [i], 42000), enemy[e].x_speed + xpart(enemy[e].petal_angle [i], 1800), enemy[e].y_speed + ypart(enemy[e].petal_angle [i], 1800), 5000, 0, 0);
    if (c != -1)
    {
     cloud[c].angle = enemy[e].petal_angle [i];
     cloud[c].x2 = grand(8) - grand(8);
     if (cloud[c].x_speed == 0 && cloud[c].y_speed == 0)
     {
         cloud[c].y_speed = 500;
     }
    }
   }
   c = quick_cloud(CLOUD_PETAL2_DEBRIS, x + xpart(ANGLE_4, 42000), y + ypart(ANGLE_4, 42000), enemy[e].x_speed + xpart(ANGLE_4, 2100), enemy[e].y_speed + ypart(ANGLE_4, 2100), 5000, 0, 0);
   if (c != -1)
   {
    cloud[c].angle = ANGLE_4;
    cloud[c].x2 = grand(8) - grand(8);
    if (cloud[c].x_speed == 0 && cloud[c].y_speed == 0)
    {
        cloud[c].y_speed = 500;
    }
   }
   break;
  case ENEMY_MEGA:
   play_effectwfvx(WAV_BANG, tone [NOTE_0E], 250, x);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 120, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 120, TCOL_SH2, 0);
   quick_cloud(CLOUD_LSHOCK, x, y, 0, 0, 40, 3, 0);
   for (i = 0; i < 4; i ++)
   {
    quick_cloud(CLOUD_CIRCLE2, x + xpart(ANGLE_8 + i * ANGLE_4, 60000), y + ypart(ANGLE_8 + i * ANGLE_4, 60000), 0, 0, 80, TCOL_SH1, 0);
    quick_cloud(CLOUD_CIRCLE3, x + xpart(ANGLE_8 + i * ANGLE_4, 60000), y + ypart(ANGLE_8 + i * ANGLE_4, 60000), 0, 0, 80, TCOL_SH2, 0);
    c = quick_cloud(CLOUD_PETAL3_DEBRIS, x + xpart(enemy[e].petal_angle [i], 75000), y + ypart(enemy[e].petal_angle [i], 75000), enemy[e].x_speed + xpart(enemy[e].petal_angle [i], 1800), enemy[e].y_speed + ypart(enemy[e].petal_angle [i], 1800), 5000, 0, 0);
    if (c != -1)
    {
     cloud[c].angle = enemy[e].petal_angle [i];
     cloud[c].x2 = grand(8) - grand(8);
     if (cloud[c].x_speed == 0 && cloud[c].y_speed == 0)
     {
         cloud[c].y_speed = 500;
     }
    }
   }
   for (i = 4; i < 7; i ++)
   {
    c = quick_cloud(CLOUD_PETAL1_DEBRIS, x + xpart(enemy[e].petal_angle [i], 65000), y + ypart(enemy[e].petal_angle [i], 65000), enemy[e].x_speed + xpart(enemy[e].petal_angle [i], 2100), enemy[e].y_speed + ypart(enemy[e].petal_angle [i], 2100), 5000, 0, 0);
    if (c != -1)
    {
     cloud[c].angle = enemy[e].petal_angle [i];
     cloud[c].x2 = grand(8) - grand(8);
     if (cloud[c].x_speed == 0 && cloud[c].y_speed == 0)
     {
         cloud[c].y_speed = 500;
     }
    }
   }
   break;
  case ENEMY_BOSS3:
   play_effectwfvx(WAV_BANG, tone [NOTE_0C], 250, x);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 120, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 120, TCOL_SH2, 0);
   quick_cloud(CLOUD_HSHOCK, x, y, 0, 0, 50, 2, 0);
   xa = grand(ANGLE_1);
   for (i = 0; i < 5; i ++)
   {
    xa += ANGLE_5;
    quick_cloud(CLOUD_CIRCLE2, x + xpart(xa, 60000), y + ypart(xa, 60000), 0, 0, 80, TCOL_SH1, 0);
    quick_cloud(CLOUD_CIRCLE3, x + xpart(xa, 60000), y + ypart(xa, 60000), 0, 0, 80, TCOL_SH2, 0);
   }
   xa = grand(ANGLE_1);
   for (i = 0; i < 9; i ++)
   {
    xa += ANGLE_9;
    quick_cloud(CLOUD_CIRCLE2, x + xpart(xa, 90000), y + ypart(xa, 90000), 0, 0, 80, TCOL_SH1, 0);
    quick_cloud(CLOUD_CIRCLE3, x + xpart(xa, 90000), y + ypart(xa, 90000), 0, 0, 80, TCOL_SH2, 0);
   }
   for (i = 0; i < 5; i ++)
   {
    xa = boss.petal_angle [2] + i * ANGLE_5;
    xa &= 1023;
    c = quick_cloud(CLOUD_PETAL3_DEBRIS,
           x + xpart(xa, boss.petal_out [2] * GRAIN), y + ypart(xa, boss.petal_out [2] * GRAIN),
           enemy[e].x_speed + xpart(xa, 1000), enemy[e].y_speed + ypart(xa, 1000), 5000, 0, 0);
    if (c != -1)
    {
     cloud[c].angle = boss.petal_angle [2] + i * ANGLE_5;
     cloud[c].x2 = grand(8) - grand(8);
     if (cloud[c].x_speed == 0 && cloud[c].y_speed == 0)
     {
         cloud[c].y_speed = 500;
     }
    }
   }
   for (i = 0; i < 7; i ++)
   {
    xa = boss.petal_angle [1] + i * ANGLE_7;
    xa &= 1023;
    c = quick_cloud(CLOUD_PETAL1_DEBRIS,
           x + xpart(xa, boss.petal_out [1] * GRAIN), y + ypart(xa, boss.petal_out [1] * GRAIN),
           enemy[e].x_speed + xpart(xa, 1000), enemy[e].y_speed + ypart(xa, 1000), 5000, 0, 0);
    if (c != -1)
    {
     cloud[c].angle = boss.petal_angle [1] + i * ANGLE_7;
     cloud[c].x2 = grand(8) - grand(8);
     if (cloud[c].x_speed == 0 && cloud[c].y_speed == 0)
     {
         cloud[c].y_speed = 500;
     }
    }
   }
   for (i = 0; i < 9; i ++)
   {
    xa = boss.petal_angle [0] + i * ANGLE_9;
    xa &= 1023;
    c = quick_cloud(CLOUD_PETAL2_DEBRIS,
           x + xpart(xa, boss.petal_out [0] * GRAIN), y + ypart(xa, boss.petal_out [0] * GRAIN),
           enemy[e].x_speed + xpart(xa, 1000), enemy[e].y_speed + ypart(xa, 1000), 5000, 0, 0);
    if (c != -1)
    {
     cloud[c].angle = boss.petal_angle [0] + i * ANGLE_7;
     cloud[c].x2 = grand(8) - grand(8);
     if (cloud[c].x_speed == 0 && cloud[c].y_speed == 0)
     {
         cloud[c].y_speed = 500;
     }
    }
   }
   break;
   case ENEMY_DRIFTER1:
   case ENEMY_ESCORT:
    play_effectwfvx(WAV_DRUM3, tone [NOTE_1C], 250, x);
    quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 30, TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 30, TCOL_YELLOW, 0);
    quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 35, 2, 0);
    quick_cloud(CLOUD_BURN, x, y, -3500 - grand(1000), -1500 - grand(1000), 40 + grand(20), 0, 990);
    quick_cloud(CLOUD_BURN, x, y, 3500 + grand(1000), -2000 - grand(1000), 40 + grand(20), 0, 990);
    for (i = 0; i < 10; i ++)
    {
     angle = grand(ANGLE_1);
     xa = 4000 + grand(3000);
     quick_cloud(CLOUD_CIRCLE5, x + xpart(angle, 5000), y + xpart(angle, 5000), xpart(angle, xa), ypart(angle, xa), 30 + grand(30), TCOL_ORANGE, 900);
    }
    break;
   case ENEMY_DRIFTER2:
    play_effectwfvx(WAV_DRUM3, tone [NOTE_0A], 250, x);
    quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 40, TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 40, TCOL_YELLOW, 0);
    quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 35, 2, 0);
    quick_cloud(CLOUD_BURN, x, y, -3500 - grand(1000), -1500 - grand(1000), 40 + grand(20), 0, 990);
    quick_cloud(CLOUD_BURN, x, y, 3500 + grand(1000), -2000 - grand(1000), 40 + grand(20), 0, 990);
    for (i = 0; i < 15; i ++)
    {
     angle = grand(ANGLE_1);
     xa = 4000 + grand(3000);
     quick_cloud(CLOUD_CIRCLE3, x + xpart(angle, 5000), y + xpart(angle, 5000), xpart(angle, xa), ypart(angle, xa), 30 + grand(30), TCOL_ORANGE, 930);
    }
    break;
   case ENEMY_RISER:
    play_effectwfvx(WAV_DRUM3, tone [NOTE_1D], 250, x);
    quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 40, TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 40, TCOL_YELLOW, 0);
    quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 20, 0, 0);
    for (i = 0; i < 10; i ++)
    {
     angle = grand(ANGLE_1);
     xa = 4000 + grand(3000);
     quick_cloud(CLOUD_CIRCLE5, x + xpart(angle, 5000), y + xpart(angle, 5000), xpart(angle, xa), ypart(angle, xa), 30 + grand(30), TCOL_ORANGE, 900);
    }
    break;
   case ENEMY_BEAMER:
   case ENEMY_SHOOTER:
    play_effectwfvx(WAV_BANG, tone [NOTE_1G], 250, x);
    quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 30, TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 30, TCOL_YELLOW, 0);
    quick_cloud(CLOUD_LSHOCK, x, y, 0, 0, 50, 2, 0);
    angle = grand(ANGLE_1);
    for (i = 0; i < 5; i ++)
    {
     angle += ANGLE_5;
     angle &= 1023;
     xa = 5000 + grand(3000);
     quick_cloud(CLOUD_BURN, x + xpart(angle, 5000), y + ypart(angle, 5000), xpart(angle, xa), ypart(angle, xa), 80 + grand(40), 8, 990);
    }
    for (i = 0; i < 10; i ++)
    {
     angle = grand(ANGLE_1);
     xa = 2000 + grand(3000);
     quick_cloud(CLOUD_CIRCLE5, x + xpart(angle, 5000), y + xpart(angle, 5000), xpart(angle, xa), ypart(angle, xa), 30 + grand(30), TCOL_ORANGE, 900);
    }
    break;

   case ENEMY_CRUISER1:
   case ENEMY_CRUISER3:
    play_effectwfvx(WAV_BANG, tone [NOTE_0C], 250, x);
    quick_cloud(CLOUD_CIRCLE1, x, y, 0, 0, 40 + grand(20), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 40 + grand(20), TCOL_YELLOW, 0);
    quick_cloud(CLOUD_CIRCLE1, x, y + 50000, 0, 0, 40 + grand(20), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x, y + 50000, 0, 0, 40 + grand(20), TCOL_YELLOW, 0);
    quick_cloud(CLOUD_CIRCLE1, x, y - 40000, 0, 0, 40 + grand(20), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x, y - 40000, 0, 0, 40 + grand(20), TCOL_YELLOW, 0);

    if (enemy[e].type == ENEMY_CRUISER1)
    {
     quick_cloud(CLOUD_CIRCLE1, x - 42000, y + 55000, 0, 0, 40 + grand(20), TCOL_ORANGE, 0);
     quick_cloud(CLOUD_CIRCLE2, x - 42000, y + 55000, 0, 0, 40 + grand(20), TCOL_YELLOW, 0);
     quick_cloud(CLOUD_CIRCLE1, x + 42000, y + 55000, 0, 0, 40 + grand(20), TCOL_ORANGE, 0);
     quick_cloud(CLOUD_CIRCLE2, x + 42000, y + 55000, 0, 0, 40 + grand(20), TCOL_YELLOW, 0);

     quick_cloud(CLOUD_CIRCLE1, x - 16000, y + 61000, 0, 0, 40 + grand(20), TCOL_ORANGE, 0);
     quick_cloud(CLOUD_CIRCLE2, x - 16000, y + 61000, 0, 0, 40 + grand(20), TCOL_YELLOW, 0);
     quick_cloud(CLOUD_CIRCLE1, x + 16000, y + 61000, 0, 0, 40 + grand(20), TCOL_ORANGE, 0);
     quick_cloud(CLOUD_CIRCLE2, x + 16000, y + 61000, 0, 0, 40 + grand(20), TCOL_YELLOW, 0);

    quick_cloud(CLOUD_LSHOCK, x, y, 0, 0, 50, 0, 0);
    quick_cloud(CLOUD_LSHOCK, x, y + 50000, 0, 0, 50, 2, 0);
    quick_cloud(CLOUD_MSHOCK, x, y - 40000, 0, 0, 20, 0, 0);

    }

    if (enemy[e].type == ENEMY_CRUISER3)
    {
     quick_cloud(CLOUD_CIRCLE1, x - 47000, y + 49000, 0, 0, 50 + grand(30), TCOL_ORANGE, 0);
     quick_cloud(CLOUD_CIRCLE2, x - 47000, y + 49000, 0, 0, 50 + grand(30), TCOL_YELLOW, 0);
     quick_cloud(CLOUD_CIRCLE1, x + 47000, y + 49000, 0, 0, 50 + grand(30), TCOL_ORANGE, 0);
     quick_cloud(CLOUD_CIRCLE2, x + 47000, y + 49000, 0, 0, 50 + grand(30), TCOL_YELLOW, 0);

    quick_cloud(CLOUD_LSHOCK, x, y, 0, 0, 35, 0, 0);
    quick_cloud(CLOUD_LSHOCK, x, y + 50000, 0, 0, 35, 2, 0);
    quick_cloud(CLOUD_MSHOCK, x, y - 40000, 0, 0, 15, 0, 0);
    }

    angle = grand(ANGLE_1);
    for (i = 0; i < 8; i ++)
    {
     angle += ANGLE_8;
     angle &= 1023;
     xa = 5000 + grand(3000);
     quick_cloud(CLOUD_BURN, x + xpart(angle, 5000), y + ypart(angle, 5000), xpart(angle, xa), ypart(angle, xa), 150 + grand(50), 8, 990);
    }
    for (i = 0; i < 15; i ++)
    {
     angle = grand(ANGLE_1);
     xa = 4000 + grand(4000);
     quick_cloud(CLOUD_CIRCLE5, x + xpart(angle, 5000), y + xpart(angle, 5000), xpart(angle, xa), ypart(angle, xa), 30 + grand(30), TCOL_ORANGE, 900);
    }
    break;
   case ENEMY_CRUISER2:
    play_effectwfvx(WAV_BANG, tone [NOTE_0C], 250, x);
    quick_cloud(CLOUD_CIRCLE1, x, y, 0, 0, 40 + grand(20), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 40 + grand(20), TCOL_YELLOW, 0);

    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 47000, enemy[e].y + 14000, grand(300) - grand(300), 3000 + grand(2000), 5 + grand(5), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 47000, enemy[e].y + 14000, grand(300) - grand(300), 3000 + grand(2000), 5 + grand(5), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 43000, grand(300) - grand(300), 3000 + grand(2500), 7 + grand(7), TCOL_ORANGE, 0);

    quick_cloud(CLOUD_CIRCLE1, x - 42000, y + 14000, 0, 0, 40 + grand(20), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x - 42000, y + 14000, 0, 0, 40 + grand(20), TCOL_YELLOW, 0);
    quick_cloud(CLOUD_CIRCLE1, x + 42000, y + 14000, 0, 0, 40 + grand(20), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x + 42000, y + 14000, 0, 0, 40 + grand(20), TCOL_YELLOW, 0);
    quick_cloud(CLOUD_CIRCLE1, x, y + 39000, 0, 0, 40 + grand(20), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x, y + 39000, 0, 0, 40 + grand(20), TCOL_YELLOW, 0);

    quick_cloud(CLOUD_LSHOCK, x, y, 0, 0, 50, 0, 0);
    angle = grand(ANGLE_1);
    for (i = 0; i < 4; i ++)
    {
     angle += ANGLE_4;
     angle &= 1023;
     xa = 4000 + grand(3000);
     quick_cloud(CLOUD_BURN, x + xpart(angle, 5000), y + ypart(angle, 5000), xpart(angle, xa), ypart(angle, xa), 130 + grand(40), 8, 990);
    }
    for (i = 0; i < 15; i ++)
    {
     angle = grand(ANGLE_1);
     xa = 4000 + grand(4000);
     quick_cloud(CLOUD_CIRCLE5, x + xpart(angle, 5000), y + xpart(angle, 5000), xpart(angle, xa), ypart(angle, xa), 30 + grand(30), TCOL_ORANGE, 900);
    }
    break;
   case ENEMY_CRUISER4:
    play_effectwfvx(WAV_BANG, tone [NOTE_0C], 250, x);
    quick_cloud(CLOUD_CIRCLE1, x, y, 0, 0, 40 + grand(20), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 40 + grand(20), TCOL_YELLOW, 0);

    quick_cloud(CLOUD_CIRCLE1, x - 14000, y + 45000, 0, 0, 40 + grand(20), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x - 14000, y + 45000, 0, 0, 40 + grand(20), TCOL_YELLOW, 0);
    quick_cloud(CLOUD_CIRCLE1, x + 14000, y + 45000, 0, 0, 40 + grand(20), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x + 14000, y + 45000, 0, 0, 40 + grand(20), TCOL_YELLOW, 0);

    quick_cloud(CLOUD_LSHOCK, x, y, 0, 0, 30, 1, 0);
    angle = grand(ANGLE_1);
    for (i = 0; i < 4; i ++)
    {
     angle += ANGLE_4;
     angle &= 1023;
     xa = 4000 + grand(3000);
     quick_cloud(CLOUD_BURN, x + xpart(angle, 5000), y + ypart(angle, 5000), xpart(angle, xa), ypart(angle, xa), 130 + grand(40), 8, 990);
    }
    for (i = 0; i < 15; i ++)
    {
     angle = grand(ANGLE_1);
     xa = 4000 + grand(4000);
     quick_cloud(CLOUD_CIRCLE5, x + xpart(angle, 5000), y + xpart(angle, 5000), xpart(angle, xa), ypart(angle, xa), 30 + grand(30), TCOL_ORANGE, 900);
    }
    break;

  case ENEMY_BOSS1:
   play_effectwfvx(WAV_NICE, tone [NOTE_1F], 250, x);
   play_effectwfvx(WAV_DRUM4, tone [NOTE_0G], 250, x);
   angle = quick_cloud(CLOUD_CARRIER1_BANG, x, y, 0, 0, 50, 0, 0);
   cloud[angle].x2 = 0;
   quick_cloud(CLOUD_HSHOCK, x, y, 0, 0, 50, 2, 0);
   quick_cloud(CLOUD_CIRCLE1, x, y, 0, 0, 50, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 50, TCOL_SH2, 0);

   quick_cloud(CLOUD_CIRCLE1, x - 71000, y, 0, 0, 50, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE2, x - 71000, y, 0, 0, 50, TCOL_SH2, 0);

   quick_cloud(CLOUD_CIRCLE1, x + 71000, y, 0, 0, 50, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE2, x + 71000, y, 0, 0, 50, TCOL_SH2, 0);

   angle = grand(ANGLE_1);
   for (i = 0; i < 8; i ++)
   {
    quick_cloud(CLOUD_CIRCLE3, x - 71000 + xpart(angle, 40000), y + ypart(angle, 40000), 0, 0, 85, TCOL_SH1, 0);
    quick_cloud(CLOUD_CIRCLE5, x - 71000 + xpart(angle, 40000), y + ypart(angle, 40000), 0, 0, 85, TCOL_SH2, 0);

    quick_cloud(CLOUD_CIRCLE3, x + 71000 + xpart(angle, 40000), y + ypart(angle, 40000), 0, 0, 85, TCOL_SH1, 0);
    quick_cloud(CLOUD_CIRCLE5, x + 71000 + xpart(angle, 40000), y + ypart(angle, 40000), 0, 0, 85, TCOL_SH2, 0);

    angle += ANGLE_8;

   }

   angle = grand(ANGLE_1);
   for (i = 0; i < 16; i ++)
   {
    quick_cloud(CLOUD_CIRCLE3, x + xpart(angle, 100000), y + ypart(angle, 70000), xpart(angle, 1600), ypart(angle, 1120), 100, TCOL_SH1, 990);
    quick_cloud(CLOUD_CIRCLE5, x + xpart(angle, 100000), y + ypart(angle, 70000), xpart(angle, 1600), ypart(angle, 1120), 100, TCOL_SH2, 990);

    angle += ANGLE_16;
   }


   break;


   case ENEMY_BOSS2:
    play_effectwfvx(WAV_BANG, tone [NOTE_0C] / 2, 250, x);
    play_effectwfvx(WAV_DRUM4, tone [NOTE_0C], 250, x);
    quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 140, TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 110, TCOL_YELLOW, 0);


    quick_cloud(CLOUD_HSHOCK, x + 40000, y + 10000, 0, 0, 50, 2, 0);

    quick_cloud(CLOUD_HSHOCK, x - 40000, y + 10000, 0, 0, 50, 2, 0);

    quick_cloud(CLOUD_CIRCLE1, x - 53000, y + 40000, 0, 0, 50, TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x - 53000, y + 40000, 0, 0, 50, TCOL_YELLOW, 0);

    quick_cloud(CLOUD_CIRCLE1, x + 53000, y + 40000, 0, 0, 50, TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x + 53000, y + 40000, 0, 0, 50, TCOL_YELLOW, 0);

    quick_cloud(CLOUD_CIRCLE1, x - 32000, y - 20000, 0, 0, 50, TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x - 32000, y - 20000, 0, 0, 50, TCOL_YELLOW, 0);

    quick_cloud(CLOUD_CIRCLE1, x + 32000, y - 20000, 0, 0, 50, TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, x + 32000, y - 20000, 0, 0, 50, TCOL_YELLOW, 0);


    angle = grand(ANGLE_1);
    for (i = 0; i < 4; i ++)
    {
     angle += ANGLE_4;
     angle &= 1023;
     xa = 6000 + grand(3000);
     quick_cloud(CLOUD_BURN, x + xpart(angle, 25000), y + ypart(angle, 25000), xpart(angle, xa), ypart(angle, xa), 250 + grand(50), 0, 970);
    }
    angle = grand(ANGLE_1);
    for (i = 0; i < 8; i ++)
    {
     angle = i * ANGLE_8 + grand(ANGLE_8);
     angle &= 1023;
     xa = 4000 + grand(4000);
     quick_cloud(CLOUD_BURN, x + xpart(angle, 25000), y + ypart(angle, 25000), xpart(angle, xa), ypart(angle, xa), 50 + grand(30), 0, 995);
    }
    for (i = 0; i < 15; i ++)
    {
     angle = grand(ANGLE_1);
     xa = 4000 + grand(4000);
     quick_cloud(CLOUD_CIRCLE5, x + xpart(angle, 25000), y + xpart(angle, 25000), xpart(angle, xa), ypart(angle, xa), 80 + grand(30), TCOL_ORANGE, 950);
    }
    break;

  case ENEMY_HOOK:
   play_effectwfvx(WAV_DRUM3, tone [NOTE_1E], 250, x);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 20, 2, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 30, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 30, TCOL_SH1, 0);
   c = quick_cloud(CLOUD_DELAY1, x - 19000, y, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 7;
    cloud[c].y2 = 16;
   }
   c = quick_cloud(CLOUD_DELAY1, x + 19000, y, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 7;
    cloud[c].y2 = 16;
   }
   break;
  case ENEMY_HOOK2:
   play_effectwfvx(WAV_DRUM3, tone [NOTE_1E], 250, x);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 20, 2, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 30, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 30, TCOL_SH1, 0);
   c = quick_cloud(CLOUD_DELAY1, x - 29000, y, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 9;
    cloud[c].y2 = 22;
   }
   c = quick_cloud(CLOUD_DELAY1, x + 29000, y, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 9;
    cloud[c].y2 = 22;
   }
   break;
  case ENEMY_DROPPER:
   play_effectwfvx(WAV_DRUM3, tone [NOTE_1E], 250, x);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 20, 2, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 40, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 40, TCOL_SH1, 0);
   quick_cloud(CLOUD_SCIRCLE, x, y, 0, 0, enemy[e].status, TRANS_B1_OUT, 0);
   c = quick_cloud(CLOUD_DELAY1, x - 15000, y + 21000, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 7;
    cloud[c].y2 = 14;
   }
   c = quick_cloud(CLOUD_DELAY1, x + 15000, y + 21000, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 7;
    cloud[c].y2 = 14;
   }
   c = quick_cloud(CLOUD_DELAY1, x, y - 19000, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 12;
    cloud[c].y2 = 20;
   }
   break;
  case ENEMY_RING:
   play_effectwfvx(WAV_DRUM3, tone [NOTE_1E], 250, x);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 20, 2, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 80, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 80, TCOL_SH1, 0);
   quick_cloud(CLOUD_SCIRCLE, x, y, 0, 0, enemy[e].status, TRANS_B4_OUT, 0);
   quick_cloud(CLOUD_SCIRCLE, x, y, 0, 0, enemy[e].status / 2, TRANS_B3_OUT, 0);
   c = quick_cloud(CLOUD_DELAY1, x - 29000, y - 29000, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 10;
    cloud[c].y2 = 20;
   }
   c = quick_cloud(CLOUD_DELAY1, x - 29000, y + 29000, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 10;
    cloud[c].y2 = 20;
   }
   c = quick_cloud(CLOUD_DELAY1, x + 29000, y - 29000, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 10;
    cloud[c].y2 = 20;
   }
   c = quick_cloud(CLOUD_DELAY1, x + 29000, y + 29000, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 10;
    cloud[c].y2 = 20;
   }
   c = quick_cloud(CLOUD_DELAY1, x + 0, y + 35000, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 26;
    cloud[c].y2 = 18;
   }
   c = quick_cloud(CLOUD_DELAY1, x + 0, y - 35000, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 26;
    cloud[c].y2 = 18;
   }
   c = quick_cloud(CLOUD_DELAY1, x + 35000, y, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 26;
    cloud[c].y2 = 18;
   }
   c = quick_cloud(CLOUD_DELAY1, x - 35000, y, 0, 0, 3000, TCOL_SH1, 0);
   if (c != 0)
   {
    cloud[c].angle = 26;
    cloud[c].y2 = 18;
   }
   break;
  case ENEMY_HUNTER:
   play_effectwfvx(WAV_DRUM4, tone [NOTE_1C], 250, x);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 35, 3, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 90, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 120, TCOL_SH1, 0);
   delay_cloud(x - 18000, y - 54000, TCOL_SH1, 15, 25);
   delay_cloud(x + 18000, y - 54000, TCOL_SH1, 15, 25);

   delay_cloud(x - 39000, y - 30000, TCOL_SH1, 25, 25);
   delay_cloud(x + 39000, y - 30000, TCOL_SH1, 25, 25);

   delay_cloud(x - 48000, y - 3000, TCOL_SH1, 35, 25);
   delay_cloud(x + 48000, y - 3000, TCOL_SH1, 35, 25);

   delay_cloud(x - 39000, y + 36000, TCOL_SH1, 45, 25);
   delay_cloud(x + 39000, y + 36000, TCOL_SH1, 45, 25);

   delay_cloud(x, y + 16000, TCOL_SH1, 20, 45);

   break;
  case ENEMY_SPINNER:
   play_effectwfvx(WAV_BANG, tone [NOTE_0C], 250, x);
   play_effectwfvx(WAV_DRUM4, tone [NOTE_0C], 250, x);
   quick_cloud(CLOUD_LSHOCK, x, y, 0, 0, 35, 3, 0);
   quick_cloud(CLOUD_LSHOCK, x, y + 50000, 0, 0, 35, 2, 0);
   quick_cloud(CLOUD_LSHOCK, x, y - 40000, 0, 0, 35, 2, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 70, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 80, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y - 50000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y - 45000, 0, 0, 70, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y + 50000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y + 45000, 0, 0, 70, TCOL_SH1, 0);
   delay_cloud(x, y - 75000, TCOL_SH1, 35, 25);
   delay_cloud(x - 27000, y - 45000, TCOL_SH1, 35, 25);
   delay_cloud(x + 27000, y - 45000, TCOL_SH1, 35, 25);
   delay_cloud(x, y + 75000, TCOL_SH1, 35, 25);
   delay_cloud(x - 27000, y + 45000, TCOL_SH1, 35, 25);
   delay_cloud(x + 27000, y + 45000, TCOL_SH1, 35, 25);
   delay_cloud(x, y - 35000, TCOL_SH1, 65, 15);
   delay_cloud(x - 27000, y - 65000, TCOL_SH1, 65, 15);
   delay_cloud(x + 27000, y - 65000, TCOL_SH1, 65, 15);
   delay_cloud(x, y + 35000, TCOL_SH1, 65, 15);
   delay_cloud(x - 27000, y + 65000, TCOL_SH1, 65, 15);
   delay_cloud(x + 27000, y + 65000, TCOL_SH1, 65, 15);

   delay_cloud(x - 40000, y - 15000, TCOL_SH1, 25, 25);
   delay_cloud(x - 40000, y + 15000, TCOL_SH1, 25, 25);
   delay_cloud(x + 40000, y - 15000, TCOL_SH1, 25, 25);
   delay_cloud(x + 40000, y + 15000, TCOL_SH1, 25, 25);

   delay_cloud(x - 65000, y, TCOL_SH1, 75, 17);
   delay_cloud(x + 65000, y, TCOL_SH1, 75, 17);

   quick_cloud(CLOUD_SCIRCLE, x, y, 0, 0, enemy[e].status, TRANS_B2_OUT, 0);
   break;
   case ENEMY_WALKER:
   play_effectwfvx(WAV_BANG, tone [NOTE_0C], 250, x);
   play_effectwfvx(WAV_DRUM4, tone [NOTE_0C], 250, x);
   quick_cloud(CLOUD_HSHOCK, x, y, 0, 0, 35, 2, 0);
   quick_cloud(CLOUD_LSHOCK, x, y, 0, 0, 45, 3, 0);
//   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 70, TCOL_SH2, 0);
//   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 80, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y - 54000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y - 48000, 0, 0, 70, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE3, x - 52000, y, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x - 49000, y, 0, 0, 70, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE3, x + 52000, y, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x + 49000, y, 0, 0, 70, TCOL_SH1, 0);
   delay_cloud(x - 82000, y, TCOL_SH1, 35, 27);
   delay_cloud(x - 55000, y - 35000, TCOL_SH1, 35, 27);
   delay_cloud(x - 55000, y + 35000, TCOL_SH1, 35, 27);
   delay_cloud(x + 82000, y, TCOL_SH1, 35, 27);
   delay_cloud(x + 55000, y - 35000, TCOL_SH1, 35, 27);
   delay_cloud(x + 55000, y + 35000, TCOL_SH1, 35, 27);
   delay_cloud(x + 35000, y - 45000, TCOL_SH1, 55, 20);
   delay_cloud(x - 35000, y - 45000, TCOL_SH1, 55, 20);

   delay_cloud(x - 102000, y, TCOL_SH1, 65, 15);
   delay_cloud(x + 102000, y, TCOL_SH1, 65, 15);
   break;
   case ENEMY_BOSS4:
    play_effectwfvx(WAV_BANG, tone [NOTE_0C] / 2, 250, x);
    play_effectwfvx(WAV_DRUM4, tone [NOTE_0C], 250, x);

    quick_cloud(CLOUD_HSHOCK, x + 56000, y + 62000, 0, 0, 50, 3, 0);
    quick_cloud(CLOUD_HSHOCK, x - 56000, y + 62000, 0, 0, 50, 3, 0);
    quick_cloud(CLOUD_HSHOCK, x + 70000, y - 61000, 0, 0, 50, 2, 0);
    quick_cloud(CLOUD_HSHOCK, x - 70000, y - 61000, 0, 0, 50, 2, 0);

   quick_cloud(CLOUD_CIRCLE3, x, y + 23000, 0, 0, 90, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y + 23000, 0, 0, 80, TCOL_SH1, 0);


   quick_cloud(CLOUD_CIRCLE3, x - 56000, y - 61000, 0, 0, 90, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x - 56000, y - 61000, 0, 0, 80, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE3, x + 56000, y - 61000, 0, 0, 90, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x + 56000, y - 61000, 0, 0, 80, TCOL_SH1, 0);

   quick_cloud(CLOUD_CIRCLE3, x - 77000, y - 8000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x - 77000, y - 8000, 0, 0, 70, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE3, x - 80000, y + 24000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x - 80000, y + 24000, 0, 0, 70, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE3, x - 68000, y + 50000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x - 68000, y + 50000, 0, 0, 70, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE3, x - 45000, y + 70000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x - 45000, y + 70000, 0, 0, 70, TCOL_SH1, 0);

   quick_cloud(CLOUD_CIRCLE3, x + 77000, y - 8000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x + 77000, y - 8000, 0, 0, 70, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE3, x + 80000, y + 24000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x + 80000, y + 24000, 0, 0, 70, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE3, x + 68000, y + 50000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x + 68000, y + 50000, 0, 0, 70, TCOL_SH1, 0);
   quick_cloud(CLOUD_CIRCLE3, x + 45000, y + 70000, 0, 0, 60, TCOL_SH2, 0);
   quick_cloud(CLOUD_CIRCLE2, x + 45000, y + 70000, 0, 0, 70, TCOL_SH1, 0);

   delay_cloud(x - 87000, y - 16000, TCOL_SH1, 35, 27);
   delay_cloud(x - 90000, y + 30000, TCOL_SH1, 35, 27);
   delay_cloud(x - 78000, y + 58000, TCOL_SH1, 35, 27);
   delay_cloud(x - 45000, y + 82000, TCOL_SH1, 35, 27);
   delay_cloud(x + 87000, y - 16000, TCOL_SH1, 35, 27);
   delay_cloud(x + 90000, y + 30000, TCOL_SH1, 35, 27);
   delay_cloud(x + 78000, y + 58000, TCOL_SH1, 35, 27);
   delay_cloud(x + 45000, y + 82000, TCOL_SH1, 35, 27);

   delay_cloud(x, y + 25000, TCOL_SH1, 55, 47);
   delay_cloud(x, y, TCOL_SH1, 45, 37);
   delay_cloud(x, y + 50000, TCOL_SH1, 55, 47);
   delay_cloud(x - 25000, y + 25000, TCOL_SH1, 55, 47);
   delay_cloud(x + 25000, y + 25000, TCOL_SH1, 55, 47);

   quick_cloud(CLOUD_SCIRCLE, x - 70000, y - 61000, 0, 0, boss.spin_rad, TRANS_B2_OUT, 0);
   quick_cloud(CLOUD_SCIRCLE, x + 70000, y - 61000, 0, 0, boss.spin_rad, TRANS_B2_OUT, 0);

   quick_cloud(CLOUD_SCIRCLE, x, y + 25000, 0, 0, boss.spin_rad2, TRANS_B3_OUT, 0);

   angle = boss.spin_angle2;

   quick_cloud(CLOUD_SCIRCLE, x + xpart(angle, boss.spin_rad2 * GRAIN), y + ypart(angle, boss.spin_rad2 * GRAIN) + 25000, 0, 0, boss.spin_rad3, TRANS_B1_OUT, 0);
   angle += ANGLE_3;
   quick_cloud(CLOUD_SCIRCLE, x + xpart(angle, boss.spin_rad2 * GRAIN), y + ypart(angle, boss.spin_rad2 * GRAIN) + 25000, 0, 0, boss.spin_rad3, TRANS_B1_OUT, 0);
   angle += ANGLE_3;
   quick_cloud(CLOUD_SCIRCLE, x + xpart(angle, boss.spin_rad2 * GRAIN), y + ypart(angle, boss.spin_rad2 * GRAIN) + 25000, 0, 0, boss.spin_rad3, TRANS_B1_OUT, 0);

    break;

  default:
   parts_must_explode(e);
   break;


 }

// remember part_explode!!

 allocate_xp(e);
 register_destroyed(e);
 destroy_enemy(e);

}

void delay_cloud(int x, int y, int col, int delay, int size)
{

   int c = quick_cloud(CLOUD_DELAY1, x, y, 0, 0, 3000, col, 0);
   if (c != 0)
   {
    cloud[c].angle = delay;
    cloud[c].y2 = size;
   }

}

// this is ONLY for parts which explode because the parent has exploded. If the part is destroyed itself,
//  don't call this!
void part_explode(int e)
{

// int x = enemy[e].x;
// int y = enemy[e].y;
 //int angle;
 //int i;

 switch(enemy[e].type)
 {

 }

 parts_must_explode(e);

// allocate_xp(e);
 register_destroyed(e);
 destroy_enemy(e);

}

void parts_must_explode(int e)
{

 int x = enemy[e].x;
 int y = enemy[e].y;
// int angle;
// int i;

 switch(enemy[e].type)
 {
  case ENEMY_CARRIER1_PART:
   play_effectwfvx(WAV_BANG, tone [NOTE_1G], 250, x);
   quick_cloud(CLOUD_CIRCLE2, x, y, 0, 0, 50, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE3, x, y, 0, 0, 50, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_MSHOCK, x, y, 0, 0, 20, 0, 0);
   quick_cloud(CLOUD_CIRCLE2, x, y - 5000, 0, -2000, 50, TCOL_ORANGE, 960);
   quick_cloud(CLOUD_CIRCLE3, x, y - 5000, 0, -2000, 50, TCOL_YELLOW, 960);
   quick_cloud(CLOUD_CIRCLE2, x, y + 5000, 0, +2000, 50, TCOL_ORANGE, 960);
   quick_cloud(CLOUD_CIRCLE3, x, y + 5000, 0, +2000, 50, TCOL_YELLOW, 960);
   break;
 }

}

#define LEVEL_CALC (3000 + (1400 * player.wlevel [i]))

void allocate_xp(int e)
{

 if (eclass[enemy[e].type].ai == AI_BOSS)
 {
  gain_score(eclass[enemy[e].type].xp);
  return;
 }

 if (boss.fight && (enemy[e].type == ENEMY_SMALL || enemy[e].type == ENEMY_SMALL2))
  return;

 int xp = (eclass[enemy[e].type].xp * 1000);
 int mhp = eclass[enemy[e].type].max_hp;

 int i;
 for (i = 0; i < NO_PARTS; i ++)
 {
   if (enemy[e].part [i] != -1)
   {
     xp += (eclass[enemy[enemy[e].part [i]].type].xp * 1000);
     gain_score(eclass[enemy[enemy[e].part [i]].type].xp);
   }
 }
// xp for parts allocated according to damage to main. Any damage to parts is ignored (but becomes relevant
//  if the part separates later).

 float xp_per_dam = (float) xp / (float) mhp;

// int i;

 gain_score(eclass[enemy[e].type].xp);

 for (i = 0; i < 3; i ++)
 {
  if (player.wlevel [i] > 8)
   continue;
  player.wxp [i] += (enemy[e].whurt [i] * xp_per_dam) / 100;
  if (player.wxp [i] >= LEVEL_CALC)
  {
   player.wxp [i] -= LEVEL_CALC;
   player.wlevel [i] ++;
   player.wgained [i] = 20;
//   play_effectwfvx(WAV_LEVEL, tone[NOTE_1G + player.wlevel [i]], 200, player.x);
   play_effectwfvx(WAV_LEVEL, tone[NOTE_2G], 250, player.x);
  }
  set_wpixels();
 }


}

void gain_score(int s)
{
 if ((player.score / 2000) != (player.score + s) / 2000)
 {
  player.lives ++;
   play_effectwfvx(WAV_LEVEL, tone[NOTE_1G], 250, player.x);
//  play_effectwfvx(WAV_EXTRA, 1500, 200, 320);
 }

 if (boss.fight == 0 && arena.stage_score < arena.next_phase [arena.phase] && arena.stage_score + s >= arena.next_phase [arena.phase])
 {
   arena.shift_message = 300;
   arena.phase ++;
   arena.phase_bonus += 5;
 }

 arena.stage_score += s;

 player.score += (s * (100 + arena.phase_bonus)) / 100;

}

void set_wpixels(void)
{
 int i;

 for (i = 0; i < 3; i ++)
 {
  player.wpixels2 [i] = LEVEL_CALC / 150;
  player.wpixels [i] = player.wxp [i] / 150;
  if (player.wpixels [i] >= player.wpixels2 [i])
   player.wpixels [i] = player.wpixels2 [i] - 1;
 }

}


void register_destroyed(int e)
{
 int b;
 int i;


 if (player.weapon [0] == WPN_SEEKER
     || player.weapon [1] == WPN_SEEKER
     || player.weapon [2] == WPN_SEEKER)
 {
  for (b = 0; b < NO_PBULLETS; b ++)
  {
      if (pbullet[b].type == PBULLET_SEEKER)
      {
       if (pbullet[b].target == e)
        pbullet[b].target = -1;
      }
  }
 }

 if (enemy[e].parent != -1)
 {
  int pe = enemy[e].parent;
  enemy[pe].part [enemy[e].part_index] = -1;
 }
  else
  {
   for (i = 0; i < NO_PARTS; i ++)
   {
     if (enemy[e].part [i] != -1)
     {
       part_explode(enemy[e].part [i]);
     }
   }
  }

 for (i = 0; i < NO_ENEMIES; i ++)
 {
  if (enemy[i].escorting == e)
   enemy[i].escorting = -1;
 }

}

void destroy_enemy(int e)
{

 enemy[e].type = ENEMY_NONE;

}
