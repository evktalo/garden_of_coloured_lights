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

#include "system.h"
#include "data.h"

#include "allegro.h"

//#include <string.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>	/* For file permissions check */

#include "globvars.h"
#include "stuff.h"
#include "game.h"
#include "display_init.h"
#include "sound.h"
#include "menu.h"
#include "input.h"
#include "palette.h"

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

char data_directory[DATADIR_SIZE] = "\0";

/** A FPS measurement stuff
 * This gets called currently each second and sets the frames_per_second variable
 * to (hopefully) reflect FPS
 */
void framecount(void)
{
	frames_per_second = framecounter;
	framecounter = 0;
}
END_OF_FUNCTION (framecount);

/** Is called every 25ms; lets the game know that it's time to move on to the next frame.
*/
void tickover(void)
{
	ticked ++;
}
END_OF_FUNCTION (tickover);

int main(void)
{
	/* Ok, init Allegro first and see whether it is going to work */
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
	/* Let's settle where to look for the data */
#ifdef DATADIR /* we were told during compilation */
	strncpy(data_directory, DATADIR, sizeof(data_directory));
	strcat(data_directory, "/");
#else	/* the data should better be in the executable directory */
	get_executable_name(data_directory, sizeof(data_directory));
	replace_filename(data_directory, data_directory, "", sizeof(data_directory));
#endif /* DATADIR */
	/* See that later in this file... */
	init_at_startup();
	/* Yeah, this game consists only of a menu :-( */
	startup_menu();

	return 0;
}
END_OF_MAIN()

/** Sets options at startup 
 * Just a convenience function, called during init_at_startup
 * It also sets GFX mode... 
 */
void set_options(void)
{
	options.run_vsync = get_config_int("Misc", "vsync", 0);

	options.highscore [0] = get_config_int("Scores", "Easy", 100);
	options.highscore [1] = get_config_int("Scores", "Normal", 100);
	options.highscore [2] = get_config_int("Scores", "Hard", 100);

	int randseed = get_config_int("Misc", "Seed", 0);
	srand(randseed);

	/* Let's see how to set the GFX mode... */
	int windowed2 = get_config_int("Misc", "Windowed", 0);

	/* Let's prepare to set the GFX mode according to config file...  */
	int windowed;
	switch (windowed2)
	{
	default:
	case 1:
		windowed = GFX_AUTODETECT_WINDOWED;
		break;
	case 0:
		windowed = GFX_AUTODETECT_FULLSCREEN;
		break;
	}

	/* Let's set the GFX mode now... */
	if (set_gfx_mode(windowed, 640, 480, 0, 0) != 0)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Unable to set 640x480 display mode %s", allegro_error);
		exit(1);
	}

	/* Let's set the player keys... */
	player.key [CKEY_UP] = get_config_int("Misc", "key_up", KEY_UP);
	player.key [CKEY_DOWN] = get_config_int("Misc", "key_down", KEY_DOWN);
	player.key [CKEY_LEFT] = get_config_int("Misc", "key_left", KEY_LEFT);
	player.key [CKEY_RIGHT] = get_config_int("Misc", "key_right", KEY_RIGHT);
	player.key [CKEY_FIRE1] = get_config_int("Misc", "key_fire1", KEY_Z);
	player.key [CKEY_FIRE2] = get_config_int("Misc", "key_fire2", KEY_X);
	player.key [CKEY_FIRE3] = get_config_int("Misc", "key_fire3", KEY_C);
	player.key [CKEY_SLOW] = get_config_int("Misc", "key_slow", KEY_LSHIFT);

	/* and the joy stuff */
	options.joy_button [0] = get_config_int("Misc", "joy_button_1", 0);
	options.joy_button [1] = get_config_int("Misc", "joy_button_2", 1);
	options.joy_button [2] = get_config_int("Misc", "joy_button_3", 2);
	options.joy_button [3] = get_config_int("Misc", "joy_button_slow", 3);

	options.joy_stick = get_config_int("Misc", "joy_stick", 0);

	options.joy_sensitivity = get_config_int("Misc", "joy_sensitivity", 70);
	options.init_joystick = get_config_int("Misc", "joy_init", 1);
	options.joystick = 0;
	options.key_or_joy = 0; // don't put in initfile!

	/* and the cheat mode */
	options.tourist = get_config_int("Misc", "Tourist", 0);
	if (options.tourist < 2 || options.tourist > 4)
		options.tourist = 0;

// set_config_int("Misc", "Tourist", 3);
// set_config_int("Misc", "joy_stick", 0);
}

/** Linley has said: Self-explanatory. Ha, ha, ha.
*/
void init_at_startup(void)
{
	/* The timer callbacks need some special care */
	LOCK_FUNCTION (framecount);
	LOCK_FUNCTION (tickover);
	LOCK_VARIABLE (ticked);
	LOCK_VARIABLE (frames_per_second);
	LOCK_VARIABLE (framecounter);
	LOCK_VARIABLE (turns_per_second);
	LOCK_VARIABLE (turncounter);

	/* computes FPS, every second */
	install_int (framecount, 1000);
	/* Tick each 25 secs. This is the FPS, I guess... */
	install_int (tickover, 25);
	/* yeah, that's right - we are OK with 256 colors! */
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
	char filename_buffer [DATADIR_SIZE];
	/* we have set the data_directory in the main() function */
	strncpy(filename_buffer, data_directory, sizeof(filename_buffer));
	/* init.txt is the name of config file in Allegro format containing some settings */
	strncat(filename_buffer, "init.txt", sizeof(filename_buffer));
	/* Can we write into the config file? */
	if (access(filename_buffer, W_OK) == 0)
	{/* Yes, we can */
		set_config_file(filename_buffer);
	}
	else
	{/*We can not write the init where it is*/
		char right_path[512];
		/* OK, we will do that in our home directory... */
		const char * unix_path = getenv("HOME");
		const char * vista_path = getenv("APPDATA");
		/* We will consider a subdir in our home directory... */
		strncpy(right_path, (unix_path != NULL ? unix_path : vista_path), sizeof(right_path) );
		/* with a unix-like name :-) */
		strncat(right_path, "/.garden", sizeof(right_path) );
		if (access(right_path, R_OK) != 0 ) /* we have to create that subdir first */
		{
			/* platform-specific function, see system.h */
			MKDIR(right_path);
		}
		/* We probably have the subdir now, let's look into the file */
		strncat(right_path, "/init.txt", sizeof(right_path) );
		/* Maybe there is not the file... */
		if (access(right_path, R_OK) != 0 )
		{
			char buffer[128];
			int bytes_read;
			/* Let's just copy the unwritable to our new location */
			FILE * unwritable_file = fopen(filename_buffer, "r");
			FILE * init_file = fopen(right_path, "w");
			while (bytes_read = fread(buffer, 1, sizeof(buffer), unwritable_file) )
				fwrite ( buffer, 1, bytes_read, init_file );
			fclose(init_file);
			fclose(unwritable_file);
		}
		/* Phew, we can finally use our config file... */
		set_config_file(right_path);
	}

	/* A huge number of things need to be done... */
	set_options();

	if (options.init_joystick)
		init_joystick();

	init_trig();
	prepare_display();
	init_sound();

	do
	{
		rest(20);
	} while (ticked < 200 ); // 5 second delay in accordance with competition rules 

	ticked = 0;

	clear_bitmap(screen);
	vsync();
	define_palette();
}
