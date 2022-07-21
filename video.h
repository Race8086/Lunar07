/*
video.h cabecera con prototipos de funciones
*/
/*
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
*/
#include <SDL/SDL.h>

#include <string.h>
#include <SDL/SDL_image.h>
#include <SDL_rotozoom.h>
#include "math-sll.h"
#ifdef GP2X                 // Handheld typical resolution
    #define WIDTH 320
    #define HEIGHT 240
#else
    #define WIDTH 640     // Standard monitor VGA resolution
    #define HEIGHT 480
#endif
#define ASPECT 320          // Const for scaling at higher resolutions
#define RESLANDER 23
#define RES_EXPLO 16
#define FC_Y 13
#define FC_X 39  // FC_Y * 3
#define BASE 50
#define LEVEL 4
#define MOON_MAX_X 6400 // const about moon terrain
#ifdef FALSE
#undef FALSE
#endif
#ifdef TRUE
#undef TRUE
#endif
enum { FALSE, TRUE };

typedef struct color_type {
  Uint8 r;
  Uint8 g;
  Uint8 b;
} color_type;
typedef struct terrain {
int x;
int y;
} terrain;

typedef struct landing_zone {
  int x, y;
  int points;
  Uint8 landed;
} landing_zone;

typedef struct eagle {
int x0;
int y0;
int x1;
int y1;
} eagle;
/* Prototipos de funciones */

/*** Funciones matemáticas ***/

int fast_cos(int angle);
int fast_sin(int angle);

/*** Funciones para el pintado de línea de color */

color_type mkcolor(int r, int g, int b);
void drawvertline(int x, int y1, Uint8 col, int y2);
void sdl_drawline(int x1, int y1, color_type c1,int x2, int y2, color_type c2);
void draw_line(int x1, int y1, color_type c1,int x2, int y2, color_type c2);
void draw_line2(int x1, int y1, color_type c1,int x2, int y2, color_type c2,int shift);
void draw_thick_line(int x1, int y1, color_type c1,int x2, int y2, color_type c2);

/*** Funciones para el pintado de Caracteres ***/

void draw_char(char c, int x, int y, int r, color_type cl);
void draw_text(char * str, int x, int y, int s, color_type c);
void draw_centered_text(char * str, int y, int s, color_type c);

/*** Gestión de vetana de visualización **/
void update_camera(int xl, int yl, int scale, float *pscx,float *pscy);
int check_scale(int hh,int *pscale, int *pscale_old);

/*** Funciones para el terreno, las bases, el LEM y los indicadores ***/

void draw_terrain(terrain t_a[],int scr,int level);
void draw_bases(landing_zone bs[],int scr,int level);
void draw_stars(terrain s[],int level);
void draw_lander(int lander[][4], int x0,int y0, int angle2,int tl, int r,int level,color_type cf);
void draw_explo(int l_exp[][4], int x0,int y0, int angle2, int r,int level,color_type cf);
void draw_score(float lfuel,int lhy,int lhx,int lym, int lxm);
void draw_fuel(int nivel, int x, int y);
void draw_image(char *cual,int xi,int yi);

