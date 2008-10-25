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

//#include <string.h>
#include <math.h>

#include "globvars.h"
#include "stuff.h"
#include "game.h"
#include "display_init.h"
#include "sound.h"
#include "menu.h"
#include "input.h"
#include "palette.h"

// timer interupt functions and variables:
void framecount(void);

volatile int framecounter;
volatile int frames_per_second;

void tickover(void);

volatile unsigned char ticked;
int slacktime;

void init_at_startup(void);
void begin_game(void);

void game_loop(void);

struct playerstruct player;
struct enemystruct enemy [NO_ENEMIES];
struct ebulletstruct ebullet [NO_EBULLETS];
struct pbulletstruct pbullet [NO_PBULLETS];
struct arenastruct arena;
struct eclassstruct eclass [NO_ENEMY_TYPES];
struct cloudstruct cloud [NO_CLOUDS];
struct bossstruct boss;

struct optionstruct options;

void framecount(void)
{
   frames_per_second = framecounter;
   framecounter = 0;
}
END_OF_FUNCTION (framecount);

/*
Is called every 25ms; lets the game know that it's time to move on to the next frame.
*/
void tickover(void)
{
 ticked ++;
}
END_OF_FUNCTION (tickover);



int main(void)
{

int allint =  allegro_init();
   if (allint == -1)
   {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message("Failed to initialise Allegro! This isn't going to work, sorry.");
      exit(1);
   }


   install_keyboard();
   install_timer();

   three_finger_flag = 0;
   key_led_flag = 0;

 init_at_startup();

 startup_menu();

 return 0;

}

END_OF_MAIN();

/*
Self-explanatory.
*/
void init_at_startup(void)
{


   LOCK_FUNCTION (framecount);
   LOCK_FUNCTION (tickover);
   LOCK_VARIABLE (ticked);
   LOCK_VARIABLE (frames_per_second);
   LOCK_VARIABLE (framecounter);
   LOCK_VARIABLE (turns_per_second);
   LOCK_VARIABLE (turncounter);

   install_int (framecount, 1000);
   install_int (tickover, 25);

   set_color_depth(8);

/*
The following code is for use if your system is set up to prevent an application writing to its own directory.
Normally this will prevent highscores and key configurations being saved, but the following code puts the
initfile in a better place. This may be a default on some systems (eg Unixes and possibly Windows Vista)
and can be set on some other systems.

The only problem with this is that you'll have to manually type or copy the various other options (windowed,
vsync, joystick buttons etc) into the initfile in this new location (or just copy the initfile across).

Thanks to Thomas Harte for this code! I had no idea it was necessary (it isn't on my Windows XP computer, and
I haven't tested it).

*/
#ifdef UNIX_OSX_VISTA_ETC

   {

     char *HPath = getenv("HOME");

// use this if for some reason you're running Windows Vista:
//        char *HPath = getenv("APPDATA");

	 char ConfigPath[2048];

	 sprintf(ConfigPath, "%s/.GardenOfColouredLights", HPath);
	 set_config_file(ConfigPath);

   }
#else
   set_config_file(DIRECTORY(DATADIR,init.txt));
#endif


   options.run_vsync = get_config_int("Misc", "vsync", 0);

   options.highscore [0] = get_config_int("Scores", "Easy", 100);
   options.highscore [1] = get_config_int("Scores", "Normal", 100);
   options.highscore [2] = get_config_int("Scores", "Hard", 100);

   int randseed = get_config_int("Misc", "Seed", 0);
   srand(randseed);

   int windowed2 = get_config_int("Misc", "Windowed", 0);


 int windowed;
 switch(windowed2)
 {
    default:
     case 1: windowed = GFX_AUTODETECT_WINDOWED; break;
     case 0: windowed = GFX_AUTODETECT_FULLSCREEN; break;
 }


   if (set_gfx_mode(windowed, 640, 480, 0, 0) != 0)
   {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message("Unable to set 640x480 display mode
%s
", allegro_error);
      exit(1);
   }

 init_trig();

 prepare_display();
 init_sound();


 player.key [CKEY_UP] = get_config_int("Misc", "key_up", KEY_UP);
 player.key [CKEY_DOWN] = get_config_int("Misc", "key_down", KEY_DOWN);
 player.key [CKEY_LEFT] = get_config_int("Misc", "key_left", KEY_LEFT);
 player.key [CKEY_RIGHT] = get_config_int("Misc", "key_right", KEY_RIGHT);
 player.key [CKEY_FIRE1] = get_config_int("Misc", "key_fire1", KEY_Z);
 player.key [CKEY_FIRE2] = get_config_int("Misc", "key_fire2", KEY_X);
 player.key [CKEY_FIRE3] = get_config_int("Misc", "key_fire3", KEY_C);
 player.key [CKEY_SLOW] = get_config_int("Misc", "key_slow", KEY_LSHIFT);


 options.joy_button [0] = get_config_int("Misc", "joy_button_1", 0);
 options.joy_button [1] = get_config_int("Misc", "joy_button_2", 1);
 options.joy_button [2] = get_config_int("Misc", "joy_button_3", 2);
 options.joy_button [3] = get_config_int("Misc", "joy_button_slow", 3);

 options.joy_stick = get_config_int("Misc", "joy_stick", 0);

 options.joy_sensitivity = get_config_int("Misc", "joy_sensitivity", 70);
 options.init_joystick = get_config_int("Misc", "joy_init", 1);
 options.joystick = 0;
 options.key_or_joy = 0; // don't put in initfile!

 options.tourist = get_config_int("Misc", "Tourist", 0);
 if (options.tourist < 2 || options.tourist > 4)
  options.tourist = 0;

// set_config_int("Misc", "Tourist", 3);
// set_config_int("Misc", "joy_stick", 0);

 if (options.init_joystick)
  init_joystick();

//#ifdef ASDFASDF
 do
 {
      rest(15);
 } while(ticked < 200); // 5 second delay in accordance with competition rules

 ticked = 0;
//#endif

 clear_bitmap(screen);
 vsync();
 define_palette();

}



