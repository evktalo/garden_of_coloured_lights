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

void init_trig (void);

float angle_to_radians (int angle);
int radians_to_angle (float angle);
fixed angle_to_fixed (int angle);
int grand (int number);
int crandom (int number);
int turn_towards_angle (int angle, int tangle, int turning);
int turn_towards_xy (int x1, int y1, int x2, int y2, int angle, int turning);
inline int xpart (int angle, int length);
inline int ypart (int angle, int length);
int pulsate (int speed, int amount, int county);
int angle_difference (int a1, int a2);

void error_message_out (const char *errm);
int delta_turn_towards_angle (int angle, int tangle, int turning);
int delta_turn_towards_xy (int x1, int y1, int x2, int y2, int angle,
			   int turning);
int pos_or_neg (int a);
int odd_even (int v);
