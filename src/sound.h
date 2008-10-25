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


void init_sound (void);
void init_beat (void);
void init_beat_new_level (void);
void run_beat (void);
void change_music (int mto);

void play_effect (int sample);
void play_effectwfvx (int sample, int f, int v, int x);
void play_effectwfvp (int sample, int f, int v, int pan);

#define FIRE1_SWOOPER1 1
#define FIRE1_DARTER1 2
#define FIRE1_DIVER1 4
#define FIRE1_CARRIER1 8
#define FIRE1_SWOOPER2 16
#define FIRE1_DARTER2 32
#define FIRE1_BOSS1_2 64
#define FIRE1_CARRIER_PART 128

#define FIRE1_BOSS1_1 2

#define FIRE2_DRIFTER1 1

#define FIRE2_CRUISER2 1
#define FIRE2_CRUISER1 2
#define FIRE2_CRUISER3 4
#define FIRE2_BEAMER 8
#define FIRE2_CRUISER4 16
#define FIRE2_SHOOTER 32
#define FIRE2_BOSS2_1 64
#define FIRE2_BOSS2_2 128


#define FIRE3_SMALL 1
#define FIRE3_MEGA 2
#define FIRE3_BOSS3_1 4
#define FIRE3_BOSS3_2 8

#define FIRE3_SMALL2 1
#define FIRE3_LARGER 2
#define FIRE3_LARGER2 4
#define FIRE3_WINGS 8
#define FIRE3_WINGS2 16
#define FIRE3_BOSS3_3 32
#define FIRE3_BOSS3_4 64

#define FIRE4_HOOK 1
#define FIRE4_DROPPER 2
#define FIRE4_SPINNER 4
#define FIRE4_HOOK2 8

#define FIRE4_WALKER 1
#define FIRE4_HUNTER 2


enum
{
  OVERMUSIC_L1_1,
  OVERMUSIC_L1_2,

  OVERMUSIC_L2_1,
  OVERMUSIC_L2_2,
  OVERMUSIC_L2_3,

  OVERMUSIC_L3_1,
  OVERMUSIC_L3_2,
  OVERMUSIC_L3_3,
  OVERMUSIC_L3_4,
// OVERMUSIC_L2_4,

  OVERMUSIC_L4_1,
  OVERMUSIC_L4_2,

  NO_OVERMUSICS
};

enum
{
  WAV_NONE,
  WAV_FIRE,
  WAV_BLOP,
  WAV_FLUTE,
  WAV_DRUM1,
  WAV_DRUM2,
  WAV_DRUM3,
  WAV_DRUM4,
  WAV_SWBEAM,
  WAV_LINE1,
  WAV_LINE2,
  WAV_CYMBAL1,
  WAV_NICE,
  WAV_NICE2,
  WAV_SAW,
  WAV_BLIP,
  WAV_SCATTER,
  WAV_SEEKER,
  WAV_MGUN,
  WAV_CANNON,
  WAV_BLADE,
  WAV_SQUARE,
  WAV_BANG,
  WAV_RECT,
  WAV_RECT2,
  WAV_WHOOSH,
  WAV_P_BANG,
  WAV_WORMS,
  WAV_MOUTH,
  WAV_LWBEAM,
  WAV_LONG,
  WAV_LONG2,
  WAV_XLINE,
  WAV_FLUTE2_S,
  WAV_FLUTE2_L,
  WAV_FLUTE_L,
  WAV_FUNNY,
  WAV_PIANO,
  WAV_PAN,
  WAV_STRING,
  WAV_DLINE,
  WAV_SYNTH,
  WAV_FOOD,
  WAV_LIGHT,
  WAV_BEAM,
  WAV_ARP,
  WAV_LINE3,
  WAV_STAGE4,
  WAV_MULTI,
  WAV_MULTI2,
  WAV_THING4,
  WAV_SPINNER,
  WAV_WALKER,
  WAV_LEVEL,
  WAV_EXTRA,
  WAV_STAGE1,
  WAV_STAGE2,
  WAV_STAGE3,
  WAV_STAGE4_2,
  NO_WAVS
};


enum
{
  EVENT_NONE,
  EVENT_PULSE,
  EVENT_SWOOPER1_FIRE,
  EVENT_STOPPER1_FIRE,
  EVENT_PULSE1,
  EVENT_PULSE2,
  EVENT_FIRE2_1,
  EVENT_FIRE2_2,
  EVENT_FIRE3_1,
  EVENT_FIRE3_2,
  EVENT_FIRE4_1,
  EVENT_FIRE4_2,
  NO_EVENTS
};

enum
{
  DONE_SWOOPER1_FIRE,
  DONE_STOPPER1_FIRE,
  DONE_FIRE2_1,
  DONE_FIRE2_2,
  DONE_FIRE3_1,
  DONE_FIRE3_2,
  DONE_FIRE4_1,
  DONE_FIRE4_2,
  NO_DONE
};


enum
{
  NOTE_0C,
  NOTE_0CS,
  NOTE_0D,
  NOTE_0DS,
  NOTE_0E,
  NOTE_0F,
  NOTE_0FS,
  NOTE_0G,
  NOTE_0GS,
  NOTE_0A,
  NOTE_0AS,
  NOTE_0B,
  NOTE_1C,
  NOTE_1CS,
  NOTE_1D,
  NOTE_1DS,
  NOTE_1E,
  NOTE_1F,
  NOTE_1FS,
  NOTE_1G,
  NOTE_1GS,
  NOTE_1A,
  NOTE_1AS,
  NOTE_1B,
  NOTE_2C,
  NOTE_2CS,
  NOTE_2D,
  NOTE_2DS,
  NOTE_2E,
  NOTE_2F,
  NOTE_2FS,
  NOTE_2G,
  NOTE_2GS,
  NOTE_2A,
  NOTE_2AS,
  NOTE_2B,
  NOTE_3C,
  NOTE_3CS,
  NOTE_3D,
  NOTE_3DS,
  NOTE_3E,
  NOTE_3F,
  NOTE_3FS,
  NOTE_3G,
  NOTE_3GS,
  NOTE_3A,
  NOTE_3AS,
  NOTE_3B,
  NOTE_ENDNOTE
};

#define NO_TONES 56


#ifndef FROM_SOUND_C

// when on the next tick we're going to play a note with an attached event, set this to something
//  for the relevant event so that if the event is ready it can be done
extern int synch_ready[NO_EVENTS];

// when we've done something, set this to the note that we want to play.
extern int synch_done[NO_DONE];

extern int tone[NO_TONES];

#endif
