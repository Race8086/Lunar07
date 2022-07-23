/*
Video.c Modulo con las funciones de pintado en pantalla
*/

#include "video.h"
#include <SDL/SDL_mixer.h>
// Variables Globales
extern float Scx,Scy;
extern SDL_Surface *screen;

Uint8 *pixels;
/*
BEAWARE !
This table it's only for 320 x 240 screens.
It didn't works on higher resolutions

*/
Uint32 MUL320[240]={
		0,320,640,960,1280,1600,1920,2240,2560,2880,3200,3520,3840,4160,4480,4800,
		5120,5440,5760,6080,6400,6720,7040,7360,7680,8000,8320,8640,8960,9280,9600,9920,
		10240,10560,10880,11200,11520,11840,12160,12480,12800,13120,13440,13760,14080,14400,14720,15040,
		15360,15680,16000,16320,16640,16960,17280,17600,17920,18240,18560,18880,19200,19520,19840,20160,
		20480,20800,21120,21440,21760,22080,22400,22720,23040,23360,23680,24000,24320,24640,24960,25280,
		25600,25920,26240,26560,26880,27200,27520,27840,28160,28480,28800,29120,29440,29760,30080,30400,
		30720,31040,31360,31680,32000,32320,32640,32960,33280,33600,33920,34240,34560,34880,35200,35520,
		35840,36160,36480,36800,37120,37440,37760,38080,38400,38720,39040,39360,39680,40000,40320,40640,
		40960,41280,41600,41920,42240,42560,42880,43200,43520,43840,44160,44480,44800,45120,45440,45760,
		46080,46400,46720,47040,47360,47680,48000,48320,48640,48960,49280,49600,49920,50240,50560,50880,
		51200,51520,51840,52160,52480,52800,53120,53440,53760,54080,54400,54720,55040,55360,55680,56000,
		56320,56640,56960,57280,57600,57920,58240,58560,58880,59200,59520,59840,60160,60480,60800,61120,
		61440,61760,62080,62400,62720,63040,63360,63680,64000,64320,64640,64960,65280,65600,65920,66240,
		66560,66880,67200,67520,67840,68160,68480,68800,69120,69440,69760,70080,70400,70720,71040,71360,
		71680,72000,72320,72640,72960,73280,73600,73920,74240,74560,74880,75200,75520,75840,76160,76480
	};
// Zoom levels

/*
* El nivel de zoom debe ser tal que en el último nivel (4) la x máx se corresponda con WIDTH
* se debe pues recalcular el nivel de zoom en función de la escala
* GP2X 320 x 240 --> 6400 / 320 = 20
* PC   640 x 480 --> 6400 / 640 = 10
*/
#ifdef GP2X
int zoom [5] ={
1,
2,
5,
10,
20
};
#else
int zoom [5] ={
1,
2,
3,
5,
10
};
#endif // GP2X

#ifdef GP2X
int zoom_lander [5] ={
1,
1,
2,
3,
4
};
#else
// 320 x 240
int zoom_lander [5] ={
1,
1,
2,
3,
4

};
#endif // GP2X
/*
**** 640 x 480
int zoom [5] ={
1,
2,
3,
5,
10
};
*/

/* Characters: */

int char_vectors[36][5][4] = {
  {
    /* 0 */
    { 0, 0, 1, 0 },
    { 1, 0, 1, 2 },
    { 1, 2, 0, 2 },
    { 0, 2, 0, 0 },
    { -1, -1, -1, -1 }
  },

  {
    /* 1 */
    { 1, 0, 1, 2 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* 2 */
    { 1, 0, 0, 0 },
    { 1, 0, 1, 1 },
    { 0, 1, 1, 1 },
    { 0, 1, 0, 2 },
    { 1, 2, 0, 2 },
  },

  {
    /* 3 */
    { 0, 0, 1, 0 },
    { 1, 0, 1, 2 },
    { 0, 1, 1, 1 },
    { 0, 2, 1, 2 },
    { -1, -1, -1, -1 }
  },

  {
    /* 4 */
    { 1, 0, 1, 2 },
    { 0, 0, 0, 1 },
    { 0, 1, 1, 1 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* 5 */
    { 1, 0, 0, 0 },
    { 0, 0, 0, 1 },
    { 0, 1, 1, 1 },
    { 1, 1, 1, 2 },
    { 1, 2, 0, 2 }
  },

  {
    /* 6 */
    { 1, 0, 0, 0 },
    { 0, 0, 0, 2 },
    { 0, 2, 1, 2 },
    { 1, 2, 1, 1 },
    { 1, 1, 0, 1 }
  },

  {
    /* 7 */
    { 0, 0, 1, 0 },
    { 1, 0, 1, 2 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* 8 */
    { 0, 0, 1, 0 },
    { 0, 0, 0, 2 },
    { 1, 0, 1, 2 },
    { 0, 2, 1, 2 },
    { 0, 1, 1, 1 }
  },

  {
    /* 9 */
    { 1, 0, 1, 2 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 },
    { 0, 1, 1, 1 },
    { -1, -1, -1, -1 }
  },

  {
    /* A */
    { 0, 2, 0, 1 },
    { 0, 1, 1, 0 },
    { 1, 0, 1, 2 },
    { 0, 1, 1, 1 },
    { -1, -1, -1, -1 }
  },

  {
    /* B */
    { 0, 2, 0, 0 },
    { 0, 0, 1, 0 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 2 },
    { 1, 2, 0, 2 }
  },

  {
    /* C */
    { 1, 0, 0, 0 },
    { 0, 0, 0, 2 },
    { 0, 2, 1, 2 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* D */
    { 0, 0, 1, 1 },
    { 1, 1, 0, 2 },
    { 0, 2, 0, 0 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* E */
    { 1, 0, 0, 0 },
    { 0, 0, 0, 2 },
    { 0, 2, 1, 2 },
    { 0, 1, 1, 1 },
    { -1, -1, -1, -1 }
  },

  {
    /* F */
    { 1, 0, 0, 0 },
    { 0, 0, 0, 2 },
    { 0, 1, 1, 1 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* G */
    { 1, 0, 0, 0 },
    { 0, 0, 0, 2 },
    { 0, 2, 1, 2 },
    { 1, 2, 1, 1 },
    { -1, -1, -1, -1 }
  },

  {
    /* H */
    { 0, 0, 0, 2 },
    { 1, 0, 1, 2 },
    { 0, 1, 1, 1 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* I */
    { 1, 0, 1, 2 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* J */
    { 1, 0, 1, 2 },
    { 1, 2, 0, 2 },
    { 0, 2, 0, 1 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* K */
    { 0, 0, 0, 2 },
    { 1, 0, 0, 1 },
    { 0, 1, 1, 2 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* L */
    { 0, 0, 0, 2 },
    { 0, 2, 1, 2 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* M */
    { 0, 0, 0, 2 },
    { 1, 0, 1, 2 },
    { 0, 0, 1, 1 },
    { 0, 1, 1, 0 },
    { -1, -1, -1, -1 }
  },

  {
    /* N */
    { 0, 2, 0, 0 },
    { 0, 0, 1, 2 },
    { 1, 2, 1, 0 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* O */
    { 0, 0, 1, 0 },
    { 1, 0, 1, 2 },
    { 1, 2, 0, 2 },
    { 0, 2, 0, 0 },
    { -1, -1, -1, -1 }
  },

  {
    /* P */
    { 0, 2, 0, 0 },
    { 0, 0, 1, 0 },
    { 1, 0, 1, 1 },
    { 1, 1, 0, 1 },
    { -1, -1, -1, -1 }
  },

  {
    /* Q */
    { 0, 0, 1, 0 },
    { 1, 0, 1, 2 },
    { 1, 2, 0, 2 },
    { 0, 2, 0, 0 },
    { 0, 1, 1, 2 }
  },

  {
    /* R */
    { 0, 2, 0, 0 },
    { 0, 0, 1, 0 },
    { 1, 0, 1, 1 },
    { 1, 1, 0, 1 },
    { 0, 1, 1, 2 }
  },

  {
    /* S */
    { 1, 0, 0, 0 },
    { 0, 0, 0, 1 },
    { 0, 1, 1, 1 },
    { 1, 1, 1, 2 },
    { 1, 2, 0, 2 }
  },

  {
    /* T */
    { 0, 0, 1, 0 },
    { 1, 0, 1, 2 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* U */
    { 0, 0, 0, 2 },
    { 0, 2, 1, 2 },
    { 1, 2, 1, 0 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* V */
    { 0, 0, 0, 1 },
    { 0, 1, 1, 2 },
    { 1, 2, 1, 0 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* W */
    { 0, 0, 0, 2 },
    { 1, 0, 1, 2 },
    { 0, 1, 1, 2 },
    { 0, 2, 1, 1 },
    { -1, -1, -1, -1 }
  },

  {
    /* X */
    { 0, 0, 1, 2 },
    { 0, 2, 1, 0 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* Y */
    { 0, 0, 1, 1 },
    { 1, 0, 1, 2 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  },

  {
    /* Z */
    { 0, 0, 1, 0 },
    { 1, 0, 0, 2 },
    { 0, 2, 1, 2 },
    { -1, -1, -1, -1 },
    { -1, -1, -1, -1 }
  }
};

/* Trig junk:  (thanks to Atari BASIC for this) */

int trig[12] = {
  1024,
  1014,
  984,
  935,
  868,
  784,
  685,
  572,
  448,
  316,
  117,
  0
};




/* Create a color_type struct out of RGB values: */

color_type mkcolor(int r, int g, int b)
{
  color_type c;

  if (r > 255)
    r = 255;
  if (g > 255)
    g = 255;
  if (b > 255)
    b = 255;

  c.r = (Uint8) r;
  c.g = (Uint8) g;
  c.b = (Uint8) b;

  return c;
}

/*******************************************************************************************/
// Funciones de pintado de líneas
/*******************************************************************************************/

/* funciones de apoyo */


/* Fast approximate-integer, table-based cosine! Whee! */

int fast_cos(int angle)
{
  angle = (angle % 45);

  if (angle < 12)
    return(trig[angle]);
  else if (angle < 23)
    return(-trig[10 - (angle - 12)]);
  else if (angle < 34)
    return(-trig[angle - 22]);
  else
    return(trig[45 - angle]);
}

/* Sine based on fast cosine... */

int fast_sin(int angle)
{
  return(- fast_cos((angle + 11) % 45));
}



/* Clip lines to window: */

static inline int clip(int * x1, int * y1, int * x2, int * y2)
{
  if ((*x1 < 0) || (*x1 >= WIDTH) ||
      (*y1 < 0) || (*y1 >= HEIGHT) ||
      (*x2 < 0) || (*x2 >= WIDTH) ||
      (*y2 < 0) || (*y2 >= HEIGHT))
  {
      return FALSE;
  }
  else
  {
    return TRUE;
   }
}

/**** Dibuja una línea en vertical ****/

void drawvertline(int x, int y1, Uint8 col, int y2)
{
  if ((x>=0)&&(x<WIDTH))
  {
  	int tmp, dy;
  	Uint8 *p;

  	if (y1 > y2)
    	{
      		tmp = y1;
      		y1 = y2;
      		y2 = tmp;
    	}


  	if (y1<0)
		y1=0;
  	if (y2>=HEIGHT)
		y2=HEIGHT;

 	for (dy = y1, p= pixels + MUL320[y1] + x; dy <= y2; dy++, p+=320)
		*p=col;
   }
}


/* Draw a line on an SDL surface: */
/* There are 2 implementations:
    a) GP2X for GP2X console and 320 x 240 devices
    b) other systema and 4:3 resolutions
*/
void sdl_drawline(int x1, int y1, color_type c1,
		  int x2, int y2, color_type c2)
{
#ifdef GP2X
  int dx, dy;
  sll m, b;
  color_type ctmp=mkcolor(c1.r,c1.g,c1.b);
  Uint8 col=SDL_MapRGB(screen->format, (Uint8)ctmp.r, (Uint8)ctmp.g, (Uint8)ctmp.b);
  //  if (clip(&x1, &y1, &x2, &y2))
  //  {

      dx = x2 - x1;
      dy = y2 - y1;

      if (dx != 0)
        {
          m = slldiv(int2sll(dy), int2sll(dx));
          b = sllsub(int2sll(y1), sllmul(m, int2sll(x1)));

          if (x2 >= x1)
            dx = 1;
          else
            dx = -1;

          while (x1 != x2)
            {
              y1 = sll2int(slladd(sllmul(m, int2sll(x1)), b));
              y2 = sll2int(slladd(sllmul(m, int2sll(x1 + dx)), b));

              drawvertline(x1, y1, col, y2);

              x1 = x1 + dx;
            }
        }
      else
        drawvertline(x1, y1, col, y2);
    //}
#else
// Implementation for higher resolutions, ONLY INTEL/ADM PROCESSORS (WIN / LINUX )
    lineRGBA(screen,x1,y1,x2,y2,c1.r,c1.g,c1.b,255);
#endif
}
/*****************************************************************************
*
* Draw a line: High level implementarion, calls sdl_drawline
* makes clipping without scrolling
* for scrolling views use draw_line2
******************************************************************************/


void draw_line(int x1, int y1, color_type c1,
	       int x2, int y2, color_type c2)
{
  //int i=MOON_MAX_X; // distancia máxima del terreno

  sdl_drawline(x1, y1, c1, x2, y2, c2);

  if (x1 < 0 || x2 < 0)
    {
      sdl_drawline(x1 + WIDTH, y1, c1, x2 + WIDTH, y2, c2);
    }
  else if (x1 >= WIDTH || x2 >= WIDTH)
    {
      sdl_drawline(x1 - WIDTH, y1, c1, x2 - WIDTH, y2, c2);
    }

  if (y1 < 0 || y2 < 0)
    {
      sdl_drawline(x1, y1 + HEIGHT, c1, x2, y2 + HEIGHT, c2);
    }
  else if (y1 >= HEIGHT || y2 >= HEIGHT)
    {
      sdl_drawline(x1, y1 - HEIGHT, c1, x2, y2 - HEIGHT, c2);
    }
}
/*****************************************************************************
*  draw_line2
*  Dibuja una línea, pero tiene en cuenta el scroll
*  entre los puntos (x1,y1) y (x2,y2)
*  los parámetros c1 y c2 identifican el color de cada punto
*  shift : Flag de desplazamiento 1 Si , 0 No
******************************************************************************/

void draw_line2(int x1, int y1, color_type c1,
	       int x2, int y2, color_type c2,int shift)
{


  sdl_drawline(x1, y1, c1, x2, y2, c2);

  if (shift)
 { // tener en cuenta que hay scroll

  if (x1 < 0 || x2 < 0)
    {
      sdl_drawline(x1 + WIDTH, y1, c1, x2 + WIDTH, y2, c2);
    }
  else if (x1 >= WIDTH || x2 >= WIDTH)
    {
      sdl_drawline(x1 - WIDTH, y1, c1, x2 - WIDTH, y2, c2);
    }
 }
 }
/*******************************************************************************
*
* draw_segment: Draw a line segment, rotated around a center point:
*   r1	: Modulo inicio
*   a1	: Angulo inicio
*   c1	: color 1
*   r2	: Modulo final
*   a2	: Argumento final
*   c2	: color 2
*   cx	: Punto central, coordenada x
*   cy	: Punto central, coordenada y
*   a	: angulo de referencia ( 90º vertical )
*
*******************************************************************************/

static inline void draw_segment(int r1, int a1,  color_type c1,	  int r2, int a2,  color_type c2,  int cx, int cy, int a,int index)
{
       int x0,x1;
       int y0,y1;
       int i_end;
       i_end = MOON_MAX_X/zoom[index];

       x0 = ((fast_cos((a1 + a) >> 3) * r1) >> 10) + cx;
       x1 = ((fast_cos((a2 + a) >> 3) * r2) >> 10) + cx;
       if (x0<0) x0 = x0 + i_end;
       else if (x0>WIDTH) x0 = x0 - i_end;
       if (x1 <0) x1 = x1 + i_end;
       else if (x1 >WIDTH) x1 = x1 - i_end;

       y0 = cy - ((fast_sin((a1 + a) >> 3) * r1) >> 10);
       y1 = cy - ((fast_sin((a2 + a) >> 3) * r2) >> 10);

       draw_line(x0,y0,c1,x1,y1,c2);

}


/* Draw a character: */
/*
*   draw_char : draws a char using lines (vectorised )
*   CE:     c:  Caracter Ascci a dibujar
*           x:  Posicion x
*           y:  Posicion y
*           r:  Factor de escala
*           cl: Componentes RGB del caracter
*  ACT:     27/06/2022 : Se cambia la implementacion para el dibujo de lineas
*           utilizando la libreria SDL_gfxPrimitives
*/

void draw_char(char c, int x, int y, int r, color_type cl)
{
  int i, v;

  /* Which vector is this character? */

  v = -1;
  if (c >= '0' && c <= '9')
    v = (c - '0');
  else if (c >= 'A' && c <= 'Z')
    v = (c - 'A') + 10;


  if (v != -1)
    {
      for (i = 0; i < 5; i++)
	{
	  if (char_vectors[v][i][0] != -1)
	    {
	      //lineRGBA(screen,x + (char_vectors[v][i][0] * r),
		//	y + (char_vectors[v][i][1] * r),
		//	x + (char_vectors[v][i][2] * r),
		//	y + (char_vectors[v][i][3] * r),
		//	cl.r,cl.g,cl.b,255);
	     draw_line(x + (char_vectors[v][i][0] * r),
			y + (char_vectors[v][i][1] * r),
			cl,
			x + (char_vectors[v][i][2] * r),
			y + (char_vectors[v][i][3] * r),
			cl);
	    }
	}
    }
}


void draw_text(char * str, int x, int y, int s, color_type c)

{
  int i;

  for (i = 0; i < strlen(str); i++)
    draw_char(str[i], i * (s + 4) + x+1, y, s, c);
}


/* Draw text, centered horizontally: */

void draw_centered_text(char * str, int y, int s, color_type c)
{
  draw_text(str, (WIDTH - strlen(str) * (s + 3)) / 2, y, s, c);
}

void draw_thick_line(int x1, int y1, color_type c1,
		     int x2, int y2, color_type c2)
{
  draw_line(x1, y1, c1, x2, y2, c2);
  draw_line(x1 + 1, y1 + 1, c1, x2 + 1, y2 + 1, c2);
}


/**** Funciones para pintar el lander, las bases y el terreno *****/


/*********************************************************************************************************
*
* draw_terrain
*			t_a[]: array con las coordenadas del tereno
*			scr:	uds en el eje x para desplazar el terreno
*			level: nivel de zoom 0-4
*
*
***********************************************************************************************************/
void draw_terrain(terrain t_a[],int scr,int level)
{

    int i,i_ini,i_last,i_end=0; // Loop counter
    int x0,x1;
    int y0,y1;

    i_ini = 0;
    i_last = 128;
    // Calculate terrain section to display
    i_end = t_a[i_last].x/zoom[level];

    for (i=i_ini;i!=i_last;i = (i+1) % 129)
    {

        x0=(t_a[i].x/zoom[level])-(int)Scx;
        if (x0<0) x0 = x0 + i_end;
        else if (x0>WIDTH) x0 = x0 - i_end;
        x1=(t_a[i+1].x/zoom[level])-(int)Scx;
        if (x1 <0) x1 = x1 + i_end;
        else if (x1 >WIDTH) x1 = x1 - i_end;
        y0=HEIGHT -((t_a[i].y/zoom[level])-(int)Scy);
        y1=HEIGHT-((t_a[i+1].y/zoom[level])-(int)Scy);
        if (x0<x1)
        draw_line2(x0,y0,mkcolor(255,255,255),x1,y1,mkcolor(255,255,255),0);
    };

};


/***********************************************************************
*Draw Lander : Dibuja el módulo lunar
* lander : Array con los vectores para dibujar el lander
* x0 : Coordenada x del centro del gravedad del lander
* yo : Coordenada y del centro del gravedad del lander
* angle2: Angulo con respecto a la superfice del lander
* tl : nivel del impulso aplicado al lander
* r  : factor de aumento del lander
* level : Nivel de zoom del escenario
* cf : Color del lander
************************************************************************
*/

void draw_lander(int lander[][4], int x0,int y0, int angle2,int tl, int r,int level,color_type cf)

{ /* Begin draw_lander */
  int i;
  color_type c;

	 x0=  (x0/zoom[level])-(int)Scx;
	 y0 = (y0/zoom[level])-(int)Scy;
     c = mkcolor(255,255,255);
     c = cf;

     if (tl == 1) tl =2;
     else if (tl == 3) tl = 4;
     else if ((tl == 5)||(tl==7)) tl = 6;
     else if ((tl == 8)||(tl==9)) tl = 8;


  /* Coordenadas polares */
  for (i=0; i<(RESLANDER-6+tl);i++)
  {

	  draw_segment( lander[i][0]*r/zoom_lander[level],lander[i][1], c,
		       lander[i][2]*r/zoom_lander[level],lander[i][3], c,
		        x0, HEIGHT -  y0,
		       angle2,level);
  }

} /* End draw_lander */


/***********************************************************************
*Draw explo : Dibuja explosión del módulo lunar
* l_exp : Array con los vectores para dibujar la explosión
* x0 : Coordenada x del centro de la explosión
* yo : Coordenada y del centro de la explosión
* angle2: Angulo para rotar la figura
* r  : factor de aumento
* level : Nivel de zoom del escenario
************************************************************************
*/

void draw_explo(int l_exp[][4], int x0,int y0, int angle2, int r,int level,color_type cf)

{ /* Begin draw_lander */
  int i;
  color_type c;

	 x0=  (x0/zoom[level])-(int)Scx;
	 y0 = (y0/zoom[level])-(int)Scy;
     c = mkcolor(255,255,255);
     c = cf;

  /* Coordenadas polares */
  for (i=0; i<RES_EXPLO;i++)
  {

	  draw_segment( l_exp[i][0]*r,l_exp[i][1], c, l_exp[i][2]*r,l_exp[i][3], c,
		        x0, HEIGHT -  y0, angle2,level);
  }

} /* End draw_explo */


/********************************************************
* draw_bases(bs[],src,level)
* bs[] : array con las posiciones de las bases y su estadp
* src  : sin uso
* level: Nivel de zoom del escenario
* Draw the six bases
*********************************************************/

void draw_bases(landing_zone bs[],int scr,int level)
{
    int i; // Loop counter
    int x0,x1;
    int y;
    int i_end;
    color_type color;

    i_end = MOON_MAX_X/zoom[level];  // longitud máxima del terreno elegido

    for (i=0;i<6;i++)
    {
        x0=((bs[i].x - BASE)/zoom[level])-(int)Scx;
        if (x0<0) x0 = x0 + i_end;
        else if (x0>WIDTH) x0 = x0 - i_end;
         x1=((bs[i].x +BASE)/zoom[level])-(int)Scx;
        if (x1 <0) x1 = x1 + i_end;
        else if (x1 >WIDTH) x1 = x1 - i_end;
         y =HEIGHT-((bs[i].y/zoom[level])-(int)Scy);
        if (bs[i].landed)
        	color=mkcolor(0,255,0);
        else
        	color=mkcolor(255,0,0);
        if (x0<x1)
        draw_line2(x0,y,color,x1,y,color, 0);
     };
};

void draw_stars(terrain s[],int level)
{
 int i;
// int x0,y0;
 color_type color;

 color = mkcolor(255,255,255);
 for (i=0;(i<10);i++)
 {
     draw_line2(s[i].x/zoom[level],HEIGHT-s[i].y/zoom[level],color,s[i].x/zoom[level],HEIGHT-s[i].y/zoom[level],color, 0);
    // draw_line2((s[i].x-1)/zoom[level],HEIGHT-s[i].y/zoom[level],color,(s[i].x+1)/zoom[level],HEIGHT-s[i].y/zoom[level],color, 0);
   //  draw_line2(s[i].x/zoom[level],HEIGHT-(s[i].y-1)/zoom[level],color,s[i].x/zoom[level],HEIGHT-(s[i].y+1)/zoom[level],color, 0);
 }
}

void draw_score(float lfuel,int lhy,int  lhx,int lym,int lxm)

{
     char lstr[48];
     int size = WIDTH / ASPECT;
     color_type cverde,crojo;
     cverde = mkcolor(0,255,0);
     crojo = mkcolor(255,0,0);

      // Draw score:
      sprintf(lstr,"FUEL");
      draw_text(lstr, size*3, size*5, size*3, mkcolor(128,196, 255));
      sprintf(lstr,"THRUST");
      draw_text(lstr, size*75, size*5, size*3, mkcolor(128, 196, 255));
      sprintf(lstr,"%d",(int) lfuel);
      draw_text(lstr, size*40, size*3, size*4, mkcolor(255, 255, 255));
      sprintf(lstr,"DV");
      draw_text(lstr, size*155, size*5, size*3, mkcolor(128,196, 255));
      sprintf(lstr, "%d", (int)lhy- FC_Y);
      draw_text(lstr, size*185, size*3, size*4, mkcolor(255, 255, 255));
      sprintf(lstr,"DH");
      draw_text(lstr, size*155, size*20, size*3, mkcolor(128,196, 255));
      sprintf(lstr, "%d", (int)lhx - FC_X);
      draw_text(lstr, size*185, size*17, size*4, mkcolor(255, 255, 255));
      sprintf(lstr, "VY");
      if (lym>0)
      draw_text(lstr, size*225 , size*5, size*3, cverde);
      else
      {
      draw_text(lstr, size*225, size*5, size*3, crojo);
      lym = - lym;
      }
      sprintf(lstr,"%d",(int) lym);
      draw_text(lstr, size*245, size*3, size*4, mkcolor(255, 255, 255));
      sprintf(lstr, "VX");
      if (lxm>0)
      draw_text(lstr, size*270 , size*5, size*3, cverde);
      else
      {
      draw_text(lstr, size*270, size*5, size*3, crojo);
      lxm = -lxm;
      }
      sprintf(lstr,"%d",(int) lxm);
      draw_text(lstr, size*290, size*3, size*4, mkcolor(255, 255, 255));
}

/*********************************************************************
* void draw_fuel : un contador de barras simple de 10 posiciones con color
*  entradas  : nivel : posición de los motores 0-9
*              x,y : Posición x, y de commienzo del contador
**********************************************************************
*/

void draw_fuel ( int nivel, int x, int y)
{
          color_type crojo,cverde,camarillo;  // Colores del gauge
          color_type c;
          int i,j=0;
          int size = WIDTH / ASPECT;
          crojo = mkcolor(255,0,0);
          cverde= mkcolor(0,255,0);
          camarillo = mkcolor ( 255,255,0);

          if (nivel)
          {
              for (i = 0 ; i< nivel ; i++)
              {
                  if (i<3) c = cverde;
                  else if ( i<7) c = camarillo;
                  else c =crojo;
                  #ifdef GP2X
                    draw_line(x + j, y, c,x+j, y +6, c);
                  #else
                    draw_line(size*(x + j), size*y, c,size*(x+j), size*(y +7), c);
                    draw_line(1+size*(x + j), size*y, c,1+size*(x+j), size*(y +7), c);
                  #endif
                  j += 2;
              }
          }
}


/**********************************************************************************************
* void update_camera : Cambia la posición de la ventana de visualización en función de la posición
* de la nave si es preciso un cambio de escala
* xl : posición x absoluta de la nave en el momento del cambio de escala
* yl : posición y absoluta de la nave en el momento del cambio de escala
* pscx : puntero a la variable Scx
* pscy puntero a la variable Scy
***********************************************************************************************/


void update_camera(int xl, int yl, int scale, float *pscx,float *pscy,float *psnap)
{

// Convertir coordenadas reales a la escala vigente
if (scale!=LEVEL)
{
xl = xl/zoom[scale];
yl = yl/zoom[scale];
// Calcular el nuevo origen para la ventana de visualización
*pscx = xl -(WIDTH/2);
*pscy = yl -(HEIGHT/2);
*psnap = *pscx;             // Guardamos el Scx de partida
}
else  // No hace falta centrar imagen
{
*pscx=0;
*pscy=0;
};
}

/******************************************************************************
* zoom_auto : Ajusta el nivel de zoom en función de la distancia a la superficie
*   Entradas : h: Distancia a la superfice
*               scale: Zoom actual
*               scale_old: Zoom anterior
*   Salidas  : 0 : no se hace nada
*              -1 : zoom in
*              1 : zoom out
*
*
*******************************************************************************/

int zoom_auto(int hl)
{
  //  int static flip=0; /* NOUSE */
    int scale=1;

    if (hl > 600) scale = 4;
    else if ((hl<=600)&& (hl>300)) scale = 3;
    else if ((hl<=300)&& (hl>100)) scale = 2;
    else if (hl<=100) scale =1;
    return scale;
}


/*******************************************************************************************************
*	int zoom_manual: Comprueba en función de la distancia a la superficie del terreno si es necesario
*	cambiar la escala de representación
*	hh : Nivel de zoom seleccionado 1: Zoom in
*                                  -1: Zoom out
*                                   0: no hace nada
*	pscale: Puntero a la escala actual
*	pscale_old: Puntero a la escala anterior
*
********************************************************************************************************/

int zoom_manual(int hh,int pscale)
{
int s; // Variable auxiliar
//int static flip = 0;
s =pscale;
if (hh==1)
	{
		s++;
		if (s>LEVEL) s=LEVEL;
	}
else if (hh ==-1)
	{
		s--;
		if (s<LEVEL-3) s=LEVEL-3;
	};
/*
if (s!=*pscale)
{
	*(pscale_old)=*(pscale);
	*(pscale)=s;

}
else
	s=0;

*/
return s;
}


/*******************************************************************
* draw_image : Rutina generica para volcar imágenes en la pantalla
*                activa
* entradas:      cual : nombre del fichero que contiene la imagen
*                xi,yi : coordenas relativas a la pantalla activa
*
*********************************************************************/

void draw_image(char *cual,int xi,int yi)
{

// int i;
 SDL_Surface *tmp;	   	// Superficie temporal para colocar la imagen
 SDL_Surface *image;  	// Superfice final de la imagen
 SDL_Rect dest;			// Ubicación de la imagen en la pantalla activa
 int size = WIDTH / ASPECT;
 tmp = IMG_Load(cual);					// Carga imagen y la formatea
 if (tmp!=NULL)
 {
      image = SDL_DisplayFormat(tmp);

     if (image == NULL)
     {
    	 #ifdef DEBUG
    		frpintf(fi,"\n pinta_imagen: %s ERR:%s\n",cual,SDL_GetError());
    	 #endif
     }
     else
     {
         SDL_FreeSurface(tmp);
     	 //SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF )); // Pantalla en blanco
    	 dest.x=xi;
    	 dest.y=yi;
    	 dest.w=size*image->w;
    	 dest.h=size*image->h;
    	 #ifdef GP2X
            SDL_BlitSurface(image, NULL, screen, &dest);
    	 #else
            SDL_BlitSurface( rotozoomSurface (image, 0.0,size,0),NULL,screen,&dest);
    	 #endif // GP2X


     }
}
else
{
    #ifdef DEBUG
    		frpintf(fi,"\n pinta_imagen: %s ERR:%s\n",cual,SDL_GetError());
     #endif

}

}
