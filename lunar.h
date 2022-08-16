/*
  Lunar 2006.h

  Main File
  A 'Lunar Lander clone' with vector graphics.
  Comienzo del proyecto : April 22, 2006
*/


#define SOUND
#define MUSIC
#define DEBUG
//#define GP2X /***** No es necesario está definida en el compilador cuando
//                    se elige la opción gp2x para generar el código
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<unistd.h>
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#ifdef SOUND
#include <SDL/SDL_mixer.h>
#endif

#include "math-sll.h"
#ifdef GP2X
	#include "gp2x_buttons.h"
#endif
#include "DT_drawtext.h"


#define STATE_FILE "./data/vector.dat"

/* Constraints: */

//#define NUM_BULLETS 3

//#define NUM_ASTEROIDS 15
//#define NUM_BITS 25

//#define AST_SIDES 6
//#define AST_RADIUS 7
//#define SHIP_RADIUS 12

#define ZOOM_START 40
//#define ONEUP_SCORE 10000


#define FPS  (1000 / 30)
#define FPS2 (FPS / 2)

#define LEFT_EDGE   0x0001
#define RIGHT_EDGE  0x0002
#define TOP_EDGE    0x0004
#define BOTTOM_EDGE 0x0008


/* Types: */

typedef struct letter_type {
  int x, y;
  int xm, ym;
} letter_type;
/*
typedef struct bullet_type {
  int timer;
  int x, y;
  int xm, ym;
} bullet_type;
*/
/*
typedef struct shape_type {
  int radius;
  int angle;
} shape_type;
*/
/*
typedef struct asteroid_type {
  int alive, size;
  int x, y;
  int xm, ym;
  int angle, angle_m;
  shape_type shape[AST_SIDES];
} asteroid_type;
*/
/*
typedef struct bit_type {
  int timer;
  int x, y;
  int xm, ym;
} bit_type;
*/
enum {left_pressed,right_pressed,up_pressed,down_pressed,keyw_pressed,keys_pressed,keyo_pressed, keyp_pressed,keyq_pressed, keya_pressed, zoom_pressed, shift_pressed};
