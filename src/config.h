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


#define ALLEGRO_STATICLINK

/* Transforms foo to "foo" */
#define TO_STRING(str) # str
/* A step between DIRECTORY and TO_STRING */
#define DIRECTORY_LOWLEVEL(root,file) TO_STRING(root/file)
/* transforms path,file to "path/file" */
#define DIRECTORY(x,y) DIRECTORY_LOWLEVEL(x,y)

/* The data directory will be here... */
#define DATADIR_SIZE 512
extern char data_directory[DATADIR_SIZE];

#define NO_KEYS CKEY_END
#define X_MIN 5000
#define Y_MIN 5000
#define X_MAX 635000
#define Y_MAX 475000

#define GRAIN 1000

#define ANGLEFULL_HEX 0x400
#define ANGLE_1 1024
#define ANGLE_2 512
#define ANGLE_3 341
// 3 is not exact
#define ANGLE_4 256
#define ANGLE_5 205
// not exact
#define ANGLE_6 171
// 6 is not exact
#define ANGLE_7 146
// 7 not exact
#define ANGLE_8 128
#define ANGLE_9 114
// 9 not exact
#define ANGLE_10 102
#define ANGLE_12 85
#define ANGLE_16 64
#define ANGLE_8_3 384
#define ANGLE_16_3 192
#define ANGLE_32 32
#define ANGLE_64 16
#define ANGLE_128 8
#define ANGLE_TO_FIXED 4

#ifndef PI
#define PI 3.141592
#endif

#define NO_ENEMIES 50
#define NO_EBULLETS 500
#define NO_PBULLETS 60
#define NO_CLOUDS 500

#define SMALL_ROTATIONS 128

#define MAX_TARGETS 40

#define NO_STARS 40

#define RAND_X (grand(X_MAX - (X_MIN * 4)) + X_MIN * 2)
#define RAND_Y (grand(Y_MAX - (Y_MIN * 4)) + Y_MIN * 2)

#define RAND_X_IN (grand(X_MAX - (X_MIN * 32)) + X_MIN * 16)
#define RAND_Y_IN (grand(Y_MAX - (Y_MIN * 32)) + Y_MIN * 16)

#define X_MIDDLE 320000
#define Y_MIDDLE 240000

#define RLE_CIRCLES 30

#define NO_PARTS 6

#define NO_PETALS 8

#define NO_BONES 40
// there are only 20 in front, though

enum
{
  CKEY_LEFT,
  CKEY_RIGHT,
  CKEY_UP,
  CKEY_DOWN,
  CKEY_FIRE1,
  CKEY_FIRE2,
  CKEY_FIRE3,
  CKEY_SLOW,
  CKEY_END
};

enum
{
  WPN_MGUN,
  WPN_SCATTER,
  WPN_LWBEAM,
  WPN_CANNON,
  WPN_RECT,
  WPN_SWBEAM,
  WPN_SEEKER,
  WPN_EIGHT,
  WPN_BLADE,
  WPN_TYPES
};

enum
{
  PBULLET_NONE,
  PBULLET_CANNON,
  PBULLET_MGUN,
  PBULLET_SEEKER,
  PBULLET_SCATTER,
  PBULLET_BLADE,
  PBULLET_RING,
  PBULLET_RECT
};

enum
{
  WSTYLE_ORBITAL,
  WSTYLE_TRAIL,
  WSTYLE_FRONT,
  WSTYLE_BLADE
};

enum
{
  BLADE_LEFT,
  BLADE_RIGHT,
  BLADE_CW_SLASH,
  BLADE_ACW_SLASH,
  BLADE_CW_SWING,
  BLADE_ACW_SWING,
  BLADE_RETURN_LEFT,
  BLADE_RETURN_RIGHT
};

enum
{
  SLOW_WEAPON,
  FAST_WEAPON
};

struct playerstruct
{

  int key[CKEY_END];
  int x;
  int y;
  int lives;
  int score;
  int respawning;
  int in_play;
  int grace;
  int recycle_speed[3];
  int recycle[3];
  int wstrength[3];
  int rpixels[3];
  int wtype[3];
  int weapon[3];
  int wx[3];
  int wy[3];
  int wstyle[3];
  int wstatus[3];
  int wlevel[3];
  int wxp[3];
  int wpixels[3];
  int wpixels2[3];
  int wgained[3];
  int wfired;
  int wframe;
  int wframe_speed;
  int wframe_max;

  int orbitals;
  int orbital_angle;
  int firing;

  int beam[3];
  int beam_counter[3];
  int beam_fade[3];
// int beam_index;
  int beam_open[3];
  char beam_flicker[3];

  int swbeam[3];
  int swbeam_counter[3];
  int swbeam_x1[3], swbeam_y1[3], swbeam_x2[3], swbeam_y2[3];
  int swbeam_level[3];

  int wings1;
  int wings2;

  int swing_x[10];
  int swing_y[10];
  int swing_x2[10];
  int swing_y2[10];
  int swing_strength[10];
  int swing_pos;
// int swing_pos2;
  int swing_angle;
  int swing_sprite_angle;
// int swing_speed_x;
  //int swing_speed_y;
  int swing_state;
  int swing_length;
  int swing_hold;
  int blade_index;

  int ring_angle;
  int ring_hold;
  int ring_index;
  int move_x;
  int move_y;


};

struct eclassstruct
{
  int max_hp;
  int xsize;
  int ysize;
  int xoffset;
  int yoffset;
  int ai;
  int speed1;
  int speed2;
  int speed3;
  char priority;
  int score;
  int xp;

// int speed;


};

enum
{
  CLOUD_NONE,
  CLOUD_MGSHOCK,
  CLOUD_CIRCLE1,
  CLOUD_CIRCLE2,
  CLOUD_CIRCLE3,
  CLOUD_CIRCLE5,
  CLOUD_CIRCLE10,
  CLOUD_MSHOCK,
  CLOUD_LSHOCK,
  CLOUD_HSHOCK,
  CLOUD_SLINE,
  CLOUD_SPAWNER,
  CLOUD_CARRIER1_BANG,
  CLOUD_PRING,
  CLOUD_RSHOCK1,
  CLOUD_RSHOCK2,
  CLOUD_RSHOCK3,
  CLOUD_RSHOCK4,
  CLOUD_RSHOCK5,
  CLOUD_TSHOCK,			// tiny - mgshock without parts
  CLOUD_PETAL1,
  CLOUD_PETAL1_DEBRIS,
  CLOUD_PETAL2_DEBRIS,
  CLOUD_PETAL3_DEBRIS,
  CLOUD_BURN,
  CLOUD_UBURN,
  CLOUD_ESLINE,
  CLOUD_DELAY1,
  CLOUD_DELAY2,
  CLOUD_DELAY3,
  CLOUD_DELAY4,
  CLOUD_SCIRCLE
};

struct cloudstruct
{
  int type;
  int x;
  int y;
  int x2;
  int y2;
  int x_speed;
  int y_speed;
  int timeout;
  int colour;
  int drag;
  int angle;
};

struct enemystruct
{
  int type;
  int x;
  int y;
  int x_speed;
  int y_speed;
  int recycle;
  int persist;
  int hp;
  int pulse1;
  int pulse2;

  int x_target[MAX_TARGETS];
  int y_target[MAX_TARGETS];
  int target_time;
  int max_target_time;
  int target;
  int phase;
  int phase2;

  int angle;
  int angle1;
  int angle2;

  int angle_turning;
  int angle_turning_count;
  int angle1_turning;
  int angle1_turning_count;

  int distance;

  int whurt[3];

  int part[NO_PARTS];
  int part_index;
  int parent;

  int status;

  int petal_angle[NO_PETALS];
  int petal_angle_speed[NO_PETALS];

  int escorting;

};

enum
{
  ENEMY_NONE,

// L1
  ENEMY_SWOOPER1,
  ENEMY_DARTER1,
  ENEMY_STOPPER1,
  ENEMY_DIVER1,
  ENEMY_CARRIER1,
  ENEMY_CARRIER1_PART,
  ENEMY_POD1,
  ENEMY_SWOOPER2,
  ENEMY_DARTER2,
  ENEMY_BOSS1,
  ENEMY_BOSS1_PART,

// L2
  ENEMY_DRIFTER1,
  ENEMY_BEAMER,
  ENEMY_RISER,
  ENEMY_CRUISER1,
  ENEMY_CRUISER2,
  ENEMY_CRUISER3,
  ENEMY_CRUISER4,
  ENEMY_CRUISER5,
  ENEMY_DRIFTER2,
  ENEMY_ESCORT,
  ENEMY_SHOOTER,
  ENEMY_BOSS2,

// L3
  ENEMY_SMALL,
  ENEMY_LARGER,
  ENEMY_SMALL2,
  ENEMY_LARGER2,
  ENEMY_WINGS,
  ENEMY_WINGS2,
  ENEMY_MEGA,
  ENEMY_BOSS3,

// L4
  ENEMY_HOOK,
  ENEMY_WALKER,
  ENEMY_DROPPER,
  ENEMY_RING,
  ENEMY_HUNTER,
  ENEMY_SPINNER,
  ENEMY_HOOK2,
  ENEMY_BOSS4,

  NO_ENEMY_TYPES
};


enum
{
  AI_SWOOPER,
  AI_DSWOOPER,
  AI_DARTER,
  AI_STOPPER,
  AI_DIVER,
  AI_DART_STOPPER,
  AI_DRIFTER,
  AI_CRUISER,
  AI_BOSS,
  AI_HOOK,
  AI_WALK,

  AI_FALL,
  AI_DANCER,
  AI_GLIDER,
  AI_MARCH,
  AI_ATTACKER,
  AI_BEAMER,
  AI_PAUSER,
  AI_BOSS1
};

struct ebulletstruct
{
  int type;
  int x;
  int y;
  int x_speed;
  int y_speed;
  int angle;
  int angle2;
  int timeout;
  int time;
  int accel;
  int sprite_angle;
  int colour;
  int cord_angle;
  int cord_x;
  int cord_y;
  int cord_x_speed;
  int cord_y_speed;
  int cord_wait;
  int cord_colour;
  int status;
  char new_ebullet;
};

enum
{
  EBULLET_NONE,
  EBULLET_SHOT,
  EBULLET_SHOT2,
  EBULLET_DDART,
  EBULLET_WING_DIAMOND,
  EBULLET_SEED,
  EBULLET_SEED2,
  EBULLET_BURST,
  EBULLET_STREAM,
  EBULLET_SDART,
  EBULLET_DOT,
  EBULLET_BEAM,
  EBULLET_LSEED,
  EBULLET_LSEED2,
  EBULLET_CURVE,
  EBULLET_BEAM2,
  EBULLET_SPIN,
  EBULLET_TRACKER,
  EBULLET_HEAVY
};

struct pbulletstruct
{
  int x;
  int y;
  int type;
  int x_speed;
  int y_speed;
  int angle;
  int timeout;
  int time;
  int persist;
  int xsize;
  int ysize;
  int dam;
  int source;
  int target;
  int status;
  int level;
};



enum
{
  MULTI_NONE,
  MULTI_BASIC
};



struct arenastruct
{
  int level;
  int difficulty;

  int level_finished;
  int game_over;
  int new_level_sign;
//  int player_lives;
//  int players;
  int change_palette;
  int shift_palette;
  int target_palette;

  int shift_message;

  int phase;

  int underlay_position;

  unsigned char counter;

  int block1_x[20];
  int block1_y[20];
  int block1_y_speed[20];
  int block1_size[20];
  int block2_x[20];
  int block2_y[20];
  int block2_y_speed[20];
  int block2_size[20];

  int flower_turn1;
  int flower_turn2;

  int beam;
  int old_beam;

  int part_x1[18][16];		// just needs to be [18] [14] but I added padding after a stupid mistake.
  int part_y1[18][16];
  int part_x2[18][16];
  int part_y2[18][16];
  int part_cx[18][16];
  int part_cy[18][16];
  int part_flow;
  int part_rank;
  int wave_y;
  int part_pulse1;
  int part_pulse2;

  int cruiser_escorts;

  char just_got_highscore;

  int stage_score;
  int max_stage_score;
  int phase_bonus;
  int next_phase[8];

  int bone_type[2][NO_BONES];
  int bone_subtype[2][NO_BONES];
  int bone_x[2][NO_BONES];
  int bone_y[2][NO_BONES];


};

struct bossstruct
{
  int fight;
  int index;

  int hp;
  int max_hp;
  int hp_bar_size;

  int phase;
  int phase_count;
  int recycle;
  int status;
  int t_angle[4];
  int t_angle_turning[4];
  int t_status;
  int t_status_change;
  int t_recycle[4];
  int t_beam[4];

  int out_sides;
  int out_sides2;
  int out_up;
  int out_out_up;

  int petal_angle[3];
  int petal_out[3];
  int petal_speed[3];
  int phase2;
  int next_small;

  int spin_size;
  int spin_rad;
  int spin_angle;
  int spin_angle2;
  int spin_size2;
  int spin_rad2;

  int spin_rad3;
  int spin_size3;
  int spin_counter;
  int sides_recycle;

  int outer_fire_type;
  int outer_fire_count;


};


struct optionstruct
{
  int sound_init;		// if 0, sound isn't initialised at all. Changed in config file only.
  int sound_mode;		// mono, stereo, reversed, off
  int run_vsync;		// on or off
  int sound_volume;		// sound fx volume; if 0 sound effects not played
  int ambience_volume;		// if 0 ambience not played
  int joystick;
  int highscore[3];
  int joy_button[4];
  int joy_sensitivity;
  int init_joystick;
  int joy_analogue;
  int key_or_joy;
  int joy_stick;

  int tourist;			// this is a cheat code entered via the initfile. See menu.c and game.c.

};
