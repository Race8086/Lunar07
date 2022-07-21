/*
  Lunar 2006.c

  Main File
 Eagle 2006 a A 'Lunar Lander clone' with vector graphics.


*/

/* change log

11-5-2006

He implementado el zoom en función de la distancia y ahora solo falta 'centrar' la escena en los alrederores de la nave
Hay que encontrar el desplazamiento x para el terreno, tiene que haber una relación entre la x de pantalla, la x real y los factores
de escala anterior y posterior

27-12-2006

Revisión general del juego, puesta al día tras el parón

* Inclusión de librería DT_ para escritura de texto en B/N
* Creación del módulo video.c
* Uso de ficheros .h
* movidas todas las funciones de dibujo de vectores a video.c

28-12-2006

* Eliminado fichero gpdata.c
* Limpieza general de código y restructuración del mismo:
*           nueva función para encapsular la gestión de eventos
*

20-1-2007

* Incluido modelo final en su release 1 para la física del juego
* Limpieza de variables y funciones obsoloetas de versiones anteriores

21-1-2007

* Añadida función indicador nivel de motores
* cambios en draw_socre : más texto y cambio de colores
* Parametrizado internamente el color del lander. Cambio a color blanco
** Cambio automatico niveles de zoom , reducidos a 4.
** Reescalado LEM en función del nivel de zoom

24-1-2007 Incluida primera versión de dibujado de estrellas
          Operativa primera versión de comprobación de alunizaje
          en función del nivel de juego elegido (aún no implementado)
26-1-2007 Simplificada la función title (eliminación codigo inecesario)
27-1-2007 Incluida función add_score, para puntuación
04-02-2007 check_ground, check_dist terminadas
09-02-2006 bug en funciones check_ground, check_dist
18-2-2007  Implementación parcial de menús
19-2-2007 Implementación gráfica de todos los menus. Falta coherencia con
          las variables de configuración
21-2-2007 Implementada fisica dependiente de la masa. Sería necesario pensarse
          si vale la escala de motores para las dos configuraciones o hay
          que genrar una escala para cada nivel. Esto puede se mucho mas
          apropiado y variando la escala también podríamos simular el efecto
          Este efecto de la masa se empezará a percibir en las últimas bases
          ya que aunque se bonifique el fuel neto será menor en cada alunizaje

28-4-2007 ************** PENDIENTE ***************************

*** Lógica juego

[ ]  Implementación de niveles de dificultad
[ ]  Calculo puntuación dependiente además del nivel
[ ]  Terminar partida cuando aluniza en las 6 bases
[x]  Menú de inicio de juego y selección de nivel
[ ]  Tabla de puntuaciones



*** Motor gráfico y física

[X]  Comprobación de distancia a terreno exacta: distancia de punto a recta
[ ]  Scroll horizontal cuando se alcanzan los límites de pantalla
[ ]  Scroll vertical , dentro de área de zoom
[ ]  Terreno continuo en eje X
[ ]  Control de los retrocohetes más preciso
[x]  Ajustar valores empuje motores
[x]  fisica dependiente de masa de cohete dependiente consumo de fuel
[-]  Implementar más letras y caracteres vectoriales  10-8-2007
[X]  Repasar LEM y estela de motores
[X]  Rutina de Explosión de la nave
[X]  Perfeccionar rutina que informa del resultado del alunizaje
[ ]  Inclusión imágenes menús, puntuaciones, presentación
< >  Aumentar resolución del terreno y pintar más o menos en función escala

*** Sonido
[ ]  Inclusión efectos ( cohetes con gradación en función del impulso
                       ( explosión
                       ( sonido ambiente :
                       ( Alunizaje : mensaje N.Amtrong

[ ] Musica             ( Presentación del Juego / Menú de selección
                       ( in-game
                       ( creditos / extras
*** Plataforma GP2X
 [X] Añadir lectura de joy & botones     13-8-2007

27/06/2022 ************ Revision integral

Se han rescatado los fuentes y se ha modificado el proyecto C::B para que compile sin
problemas en plataforma PC ( Linux -Lubuntu ) , no ha sido posible conseguirlo en Windows 10

Durante la reestructuracion del proyecto se han rescatado las librerias SDL y se han insertado en
el sistema operativo. Queda pendiente su empaquetado.

Cambios en el codigo  y errores detectados
----------------------------------------------
El juego a pesar de estar desarrollado en SDL, solo se podia correr a resolución 320 x 240
Existe una estructura de datos ligada al ancho, que ocasiona errores en la rutinas de dibujo de líneas
por lo que el texto y las líneas en general no pintan bien.
Se ha incluido la librería SDL_gfx de la que la copia utilizada no hacía uso y se efectuado un recubrimiento
de la rutina sdl_drawline, sobre la que se apoya la práctica totalidad del motor. De esta forma todo funciona
con escalado a bajo nivel.
Se han tenido que parametrizar todos los menús ya que estaban construidos para la dimensión original.
También se ha utilizado una primitiva de la librería gfx (rotozoom) para escalar las imágenes de los lem al elegir
el nivel de dicultad.
El escalado solo funciona correctamente a 640x480 sin hacer más ajustes.
Falta por parametrizar el dibujo de terreno, los indicadores, el campo de estrellas y los zoom.
*
19/7/2022

- Corregido nivel de zoom, no pintaba bien en máximo nivel.
- Ajustado aumento zoom del lander durante la simulación
- TODO
     La variable dx se desborda en un momento dado, depurar
     Comprobar el desplazamiento del escenario en escala PC

*/

#include "lunar.h"
#include "video.h"
#include <SDL/SDL_gfxPrimitives.h>
#define FUEL 200
#define MAX_V_Y 10
#define MAX_V_X 3
#define MAX_ANGLE 10
#define CHAN_THRUST 0
#define DATAPATH "small/"
#define DEP
//#define SOUND
//#define MUSIC
#define DEBUG
extern Uint8 *pixels;
extern Uint32 MUL320[240];

FILE *fi;

char svar[10][32]=
{
     "V0","V1","V2","V3","V4","V5","V6","V7","V8","V9"
};  // Array para depuración
int i=0;


/* Data: */

enum {
  SND_BULLET,
  SND_AST1,
  SND_AST2,
  SND_AST3,
  SND_AST4,
  SND_THRUST,
  SND_EXPLODE,
  SND_GAMEOVER,
  SND_EXTRALIFE,
  NUM_SOUNDS
};

char * sound_names[NUM_SOUNDS] = {
  "./data/h.dat",
  "./data/d.dat",
  "./data/e.dat",
  "./data/f.dat",
  "./data/g.dat",
  "./data/l.dat",
  "./data/i.dat",
  "./data/k.dat",
  "./data/j.dat"
};



char * mus_game_name = "./data/a.dat";


/* Globals: */

SDL_Surface *screen;
SDL_Surface *bkgd , *icon, *portby;
#ifdef GP2X
	SDL_Joystick *joystick;
#endif
#ifdef SOUND
    Mix_Chunk *sounds[NUM_SOUNDS];
    #ifdef MUSIC
    Mix_Music *game_music;
    #endif
#endif

int text_zoom;
int DT_font;
char zoom_str[24];

int player_alive, player_die_timer;
int lives, score, high, level, game_ended;

Uint64 next_time;
int if_frame;

int cfg_var[4]=
{0,                            // 0. Idioma ( 0 _esp , 1_ eng
0,                             // 1. Musica ( 0 _off, 1_on )
0,                             // 2. Rozamiento Hor 0_off 1_on
1,                             // 3. Fisica Realista 0_off 1_on
};

enum {IDIOMA,MUSICA,ROZ,FIS};       // Enumerado para las configuraciones
/*************** VARIABLES DEL MODELO *****************************************/
/*************** Fisica *******************************************************/
#define PI 3.14159             // Constante PI
#define U 7000.0            // Velocidad escape gases ( m/s)
#define G  4.67              // Gravedad Lunar
#define y0 2500             // Altura inicial


/*************** Posicionamiento X,Y  *****************************************/
int x, y;            // Posición absoluta del LEM , int
int angle;           // Angulo de inclinación del LEM
float t;             // tiempo universal
extern int zoom[5];  // Niveles de Zoom ( declarado en video.c)
float m0;            // Masa inicial del LEM
/***************** Variable con modelo ecuaciones diferenciales ******************/
#define CNV 0.0174532928;   // factor conversión grados a radianes pi/180
float x_thrust,y_thrust; // Componentes x,y del vector impulso
float x_a,y_a; // Componentes x,y del vector aceleración
float x_vel,y_vel; // Componentes x,y del vector velocidad
float x_vant,y_vant; // Componentes x,y del vector velocidad
float x_pos,y_pos; // Componentes x,y de posición
float x_posant,y_posant; // Componentes x,y de posición
/**************** Combustible y motores ***************************************/
float fuel;       // Combustible Kg;
int mu=3900;         // Carga util Kg
float impulso[] =    // Impulso de los motores ( 10 niveles ) en Kg/s
{
      0.0,
      1.0,
      2.0,
      3.0,
      4.0,
      5.0,
      6.0,
      7.0,
      8.50,
      20.0,
};

/*************** Variables gestión representación en pantalla *****************/
float Scx,           // origen del viewport para centrar la cámara en zonas de
      Scy;           // mayor superficie que la pantalla
float x_scroll,      // scroll en eje X
      y_scroll;      // scroll en eje Y
int   scroll=0;      // Flag scroll
float fSct;

/**************** Datos del terreno, bases , LEM , espacio ********************/
int res_x = 50;      // Resolucion eje X

int Plander[25][4] ={                        // LEM (vector inicio,vector final)
                                             //      coordenadas polares
               {3,   270,  5,   293},
               {5,   293,  6,   315},
               {6,   315,  7,   338},
			   {7,   338,  7,     0},
               {7,    0,  7,  23},
               {7,  23,  6,  45},
			   {6,  45,  5,  68},
			   {5,  68,  3,  90},
			   {3,  90,  4,  120},
			   {4,  120,  4,248},
			   {4, 248,   3,  270},
               {3,  270,  5,  270},
	   		   {5,   270,  6,  247},
			   {6,  247,  8,  247},
			   {3,   90,  5,   90},
			   {5,  90,  6 ,   115},
			   {6,115,   8,115},
			   {3,135,4,180},
			   {4,180,3,236},
			   {3,135,8,180},
			   {8,180,3,236},
			   {3,135,16,180},
			   {16,180,3,236},
			   {3,135,24,180},
			   {24,180,3,236},
			};
int explo_lem[16][4] =
{
   { 9,0,3,23},
   {3,23,9,45},
   {9,45,3,68},
   {3,68,9,90},
   {9,90,3,112},
   {3,112,6,135},
   {6,135,3,157},
   {3,157,9,180},
   {9,180,3,202},
   {3,202,6,225},
   {6,225,3,247},
   {3,247,9,270},
   {9,270,3,292},
   {3,292,6,315},
   {6,315,3,337},
   {3,337,9,0}


};

terrain  moon_a [129] = {          // Terreno, cada punto X,Y absolutas
{	0		,	1600	},
{	50  	,	1800	},
{	100		,	2006	},
{	150		,	2200	},         // Máxima cota del terreno
{	200		,	2100	},
{	250		,	2150	},
{	300		,	2100	},
{	350		,	2050	},
{	400		,	1975	},
{	450		,	1900	},
{	500		,	1850	},
{	550		,	1825	},
{	600		,	1815	},
{	650		,	1700	},
{	700		,	1650	},
{	750		,	1600	},
{	800		,	1600	},    // base 1
{	850		,	1600	},
{	900		,	1578	},
{	950		,	1500	},
{	1000	,	1400	},
{	1050	,	1350	},
{	1100	,	1300	},
{	1150	,	1200	},
{	1200	,	1100	},
{	1250	,	1000	},
{	1300	,	950		},
{	1350	,	1100	},
{	1400	,	1150	},
{	1450	,	1150	},
{	1500	,	1200	},
{	1550	,	1225	},
{	1600	,	1300	},
{	1650	,	1325	},
{	1700	,	1400	},
{	1750	,	1450	},
{	1800	,	1500	},
{	1850	,	1500	},
{	1900	,	1550	},
{	1950	,	1600	},
{	2000	,	1610	},
{	2050	,	1620	},
{	2100	,	1640	},
{	2150	,	1660	},
{	2200	,	1680	},
{	2250	,	1700	},
{	2300	,	1700	}, // base 2
{	2350	,	1700	},
{	2400	,	1725	},
{	2450	,	1750	},
{	2500	,	1770	},
{	2550	,	1790	},
{	2600	,	1800	},
{	2650	,	1820	},
{	2700	,	1880	},
{	2750	,	1900	},
{	2800	,	2000	},
{	2850	,	2050	},
{	2900	,	2100	},
{	2950	,	2000	},
{	3000	,	1900	},
{	3050	,	1850	},
{	3100	,	1800	},
{	3150	,	1700	},
{	3200	,	1600	},
{	3250	,	1500	},
{	3300	,	1400	},
{	3350	,	1300	},
{	3400	,	1200	},
{	3450	,	1100	},
{	3500	,	1000	},
{	3550	,	900		},
{	3600	,	800		},
{	3650	,	700		},
{	3700	,	750		},
{	3750	,	600		},
{	3800	,	600		}, // base 3
{	3850	,	600		},
{	3900	,	700		},
{	3950	,	740		},
{	4000	,	770		},
{	4050	,	775		},
{	4100	,	800		},
{	4150	,	850		},
{	4200	,	850		}, // base 4
{	4250	,	850		},
{	4300	,	900		},
{	4350	,	950		},
{	4400	,	960		},
{	4450	,	1000	},
{	4500	,	1100	},
{	4550	,	1200	},
{	4600	,	1500	},
{	4650	,	1700	},
{	4700	,	1800	},
{	4750	,	1825	},
{	4800	,	1900	},
{	4850	,	1950	},
{	4900	,	1900	},
{	4950	,	2100	},
{	5000	,	2100	}, // base 5
{	5050	,	2100	},
{	5100	,	2000	},
{	5150	,	1900	},
{	5200	,	1800	},
{	5250	,	1700	},
{	5300	,	1500	},
{	5350	,	1300	},
{	5400	,	1200	},
{	5450	,	900		},
{	5500	,	700		},
{	5550	,	500		},
{	5600	,	430		},
{	5650	,	400		},
{	5700	,	400		}, // base 6
{	5750	,	400		},
{	5800	,	450		},
{	5850	,	500		},
{	5900	,	600		},
{	5950	,	650		},
{	6000	,	700		},
{	6050	,	800		},
{	6100	,	900		},
{	6150	,	1000	},
{	6200	,	1200	},
{	6250	,	1300	},
{	6300	,	1400	},
{	6350	,	1500	},
{	6400	,	1600	}
};

// Game Bases

landing_zone base[6] ={            // Bases, X,Y,Puntos,
                                   //    estado : 0= sin alunizar, 1= alunizada
{800,1600,200,0},
{2300,1700,300,0},
{3800,600,500,0},
{4200,850,650,0},
{5000,2100,150,0},
{5700,400,800,0}
};


terrain star[11] =                  // Stars, cada punto X,Y absolutas
{
{   320     ,   3100    },
{	650		,	4500	},
{	1200	,	2200	},
{	2250	,	3200	},
{	3000	,	2200	},
{	3500	,	3900	},
{	4500	,	2700	},
{	5000	,	3500	},
{	5500	,	3700	},
{	6000	,	2600	},
{	6250	,	3100	},
};


void finish(void)
{
#ifdef DEBUG
fclose(fi);
#endif
#ifdef SOUND
	   Mix_CloseAudio();
#endif
#ifdef GP2X
			// Retorno al Menu
			chdir("/usr/gp2x");
			execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
#else
			SDL_Quit();

#endif

  exit(0);
}
/**************************************************************************
* setup
*      Inicialización de sistema de vídeo, audio, input y timers SDL
*      Carga de imágenes y archivos de sonido
*
***************************************************************************/
void setup(void)
{
    #ifdef SOUND
  int i;
  #endif // SOUND
  SDL_Surface * tmp;
  //Uint32 bpp;
  score = 0;
  srand(SDL_GetTicks());   /* Seed random number generator: */
  /* Init SDL video: */
#ifdef DEBUG
fi = fopen( "./log", "w" );
#endif
#ifdef GP2X
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
#else
    	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0)
#endif
    {
#ifdef DEBUG
      printf("\nERR: No Init video \n" "SDL code:\n" "%s\n\n", SDL_GetError());
#endif

      finish();	  exit(1);
    }
    //printf("Checking mode 640x480@8bpp.\n");
    //bpp=SDL_VideoModeOK(640, 480, 8, SDL_HWSURFACE);

   // if(!bpp){
   //   printf("Mode not available.\n");
    //  exit(-1);
    //}

    //printf("SDL Recommends 640x480@%dbpp.\n", bpp);
    //screen=SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE);
    screen = SDL_SetVideoMode(WIDTH,HEIGHT,8,SDL_SWSURFACE);
      if (screen == NULL)
	{
#ifdef DEBUG
	  fprintf(fi,
		  "\nERR: Not open Display.\n" "SDL code:\n"  "%s\n\n", SDL_GetError());
#endif
	  finish();
	  exit(1);
	}

 //     Draw_Init();// Probamos SDL_draw
      SDL_ShowCursor(SDL_ENABLE); SDL_ShowCursor(SDL_DISABLE);
      pixels=(Uint8 *) screen->pixels;
      printf(" Pixels (puntero de inicio = %d\n",*pixels);
printf(" w (Ancho en pixels = %d\n",screen->w);
printf(" h (Alto en pixels = %d\n",screen->h);
printf(" Pitch (Long bytes linea = %d\n",screen->pitch);


  /* Load background image: */
#ifdef GP2X
  tmp = IMG_Load("./data/atari.png");
#else
  tmp = IMG_Load("./data/c.dat");
#endif
  if (tmp == NULL)
    {
#ifdef DEBUG
      fprintf(fi,
	      "\nERR: background image:\n"  "SDL code:\n" "%s\n\n", SDL_GetError());
#endif
      exit(1);
    }
  bkgd = SDL_DisplayFormat(tmp);
  if (bkgd == NULL)
    {
#ifdef DEBUG
      fprintf(fi,
	      "\nERR: Disp_format in background image" "SDL code:\n" "%s\n\n", SDL_GetError());
#endif
      exit(1);
    }

  SDL_FreeSurface(tmp);
#ifdef SOUND
  /* Init sound: */

 if (Mix_OpenAudio(22050, AUDIO_S16, 2, 256) < 0)
	{
#ifdef DEBUG
	  fprintf(fi, "\nERR: Open audio 22050-16bit-st.\n"  "SDL code:\n" "%s\n\n", SDL_GetError());
#endif
	  finish();  exit(1);
	}
  /* Load sound files: */

      for (i = 0; i < NUM_SOUNDS; i++)
	{
	  sounds[i] = Mix_LoadWAV(sound_names[i]);
          if (sounds[i] == NULL)
            {
#ifdef DEBUG
              fprintf(fi,  "\nERR: loading sound file:\n" "%s\n"  "SDL code:\n" "%s\n\n", sound_names[i], SDL_GetError());
#endif
	  	      finish();  exit(1);
            }
	}
#ifdef MUSIC
      game_music = Mix_LoadMUS(mus_game_name);
      if (game_music == NULL)
	{
#ifdef DEBUG
	  fprintf(fi,"\nERR: loading music file:\n" "%s\n" "SDL code:\n" "%s\n\n", mus_game_name, SDL_GetError());
#endif
	  finish();  exit(1);
	}
#endif
#endif

      next_time=SDL_GetTicks();
      if_frame=-1;

#ifdef GP2X
			joystick = SDL_JoystickOpen(0);
			if (!joystick)
			{
			fprintf(fi,"Error al inicializar el JoyStick %s\n",SDL_GetError());
			finish(); exit(1);

			}
#endif
}
static inline int wait_fps()
{
	int ret=0;
	Uint64 now=SDL_GetTicks();
	long end=next_time-now;

	if ( end > -FPS2 )
	{
		if ((if_frame)&&(end>(FPS+FPS2)))
		SDL_Delay(end-FPS);
		ret=-1;
	}
	else
		ret=0;
	next_time+=FPS;
	return ret;
}
static inline void playsound(int snd)
{


#ifdef SOUND
  int which, i;
      which = (rand() % 3) + CHAN_THRUST;
      for (i = CHAN_THRUST; i < 4; i++)
	{
	  if (!Mix_Playing(i))
	    which = i;
	}
      Mix_PlayChannel(which, sounds[snd], 0);
#endif
}
/*****************************************************************************
* add_score : Calcula la puntación tras el aterrizaje
*           Entradas : px: Posición X del LEM
*                      lvl : Nivel de simulación
*                      bs[] : array de bases de juego
*           Salidas : Puntuación actualizada
*   24-1-2007 No está terminada la codificación de esta función
*****************************************************************************
*/

void add_score(int px, int lvl, landing_zone bs[])
{
int i;
int amount;

// buscar la base en la que se ha alunizado

amount=0;                        // Sin puntuación
for (i=0; i<6;i++)
{
 if ( abs(bs[i].x -px )<= 5 )
    {
       amount=bs[i].points;      // Recoje puntuación
       bs[i].landed = 1;   // Base alunizada
       i = 1000;            // Salir bucle comprobación
    }
}
/* Add to score: */

  score = score + amount;
  //fprintf(stderr,"score=%d\n",score);
}


static inline void reset_level(void)
{

  sprintf(zoom_str, "LEVEL %d", level);
  text_zoom = ZOOM_START;
}


void reset_eventos(int action[])

{
 action[right_pressed] = 0;
 action[left_pressed] = 0;
 //action[up_pressed] = 0;
 //action[down_pressed] = 0;
 //action[zoom_pressed] = 0;
 //action[zoom_pressed] = 0;
 //action[shift_pressed] = 0;
 //action[shift_pressed] = 0;
}


/******************************************************
* maneja_eventos: función para la lectura del teclado /joystick
*
*
********************************************************/

void maneja_eventos(int action[], int *pdone, int *pquit, int *gass)
{
  SDLKey key;                     // entrada datos PC
#ifdef GP2X
  Uint8 keyg;                     // entrada datos GP2X
#endif
  SDL_Event event;


while (SDL_PollEvent(&event) > 0) /*1*/
 {
	switch(event.type)
	{
      case SDL_QUIT:           // Tecla de escape
      {
           	*pdone = -1;        // Quit!
            *pquit = 1;
      }
      break;
      case SDL_KEYDOWN:        // Tecla pulsada o soltada
      case SDL_KEYUP:
        {
         	key = event.key.keysym.sym;
        	if (event.type == SDL_KEYDOWN) //--------------TECLA PULSADA
    		{
      		if (key == SDLK_RETURN) { 	*pdone = -1; }               /* Salir al menú principal */
    		if (key == SDLK_RIGHT)                                  /* Rotate CW */
    		{
            	action[left_pressed] = 0; action[right_pressed] = 1;
            	action[up_pressed] =0;	  action[down_pressed] = 0;
    		}
    		else if (key == SDLK_LEFT)                              /* Rotate CCW */
    		{
    			action[left_pressed] = 1; action[right_pressed] = 0;
    			action[up_pressed] =0;	  action[down_pressed] = 0;
    		}
    		else if (key == SDLK_UP)                                /* Thrust UP! */
    		{
    			action[left_pressed] = 0; action[right_pressed] = 0;
    			action[up_pressed] = 1;   action[down_pressed] = 0;
    		}
    		else if (key == SDLK_DOWN)                              /* Thrust DOWN! */
    		{
    			action[left_pressed] = 0; action[right_pressed] = 0;
    			action[up_pressed] =0;	  action[down_pressed] = 1;
    		}
    		else if (key == SDLK_s)   { action[zoom_pressed] = -1;  }  /* Zoom _out */
    		else if (key == SDLK_w)   { action[zoom_pressed] = 1;   }  /* Zoom_in */
    		else if (key == SDLK_e)   { action[shift_pressed] = -1; }  /* shift left  */
    		else if (key == SDLK_d)   { action[shift_pressed] = 1;  }  /* shift right */

    	}
    	else if (event.type == SDL_KEYUP)  //--------------- TECLA LIBERADA
    	{
        	if (key == SDLK_RIGHT) 	   { action[right_pressed] = 0; }
        	else if (key == SDLK_LEFT) { action[left_pressed] = 0;  }
        	else if (key == SDLK_UP)   { action[up_pressed] = 0;    }
        	else if (key == SDLK_DOWN) { action[down_pressed] = 0; 	}
        	else if (key == SDLK_s)    { action[zoom_pressed] = 0; 	}
        	else if (key == SDLK_w)    { action[zoom_pressed] = 0;	}
            else if (key == SDLK_e)    { action[shift_pressed] = 0; }
        	else if (key == SDLK_d)    { action[shift_pressed] = 0;	}
        	if (key == SDLK_LALT)      { action[shift_pressed] = 0; }
    	}
    }
    break;
#ifdef GP2X
	case SDL_JOYBUTTONDOWN:
		{
         keyg = (event.jbutton.button); // Capturamos el valor generado
        if (keyg == GP2X_BUTTON_START) { 	*pdone = -1; }               /* Salir al menú principal */
    		if (keyg == GP2X_JOYSTICK_E)                                  /* Rotate CW */
    		{
            	action[left_pressed] = 0; action[right_pressed] = 1;
            	action[up_pressed] =0;	  action[down_pressed] = 0;
    		}
    		else if (keyg == GP2X_JOYSTICK_W)                              /* Rotate CCW */
    		{
    			action[left_pressed] = 1; action[right_pressed] = 0;
    			action[up_pressed] =0;	  action[down_pressed] = 0;
    		}
    		else if (keyg == GP2X_JOYSTICK_N)                                /* Thrust UP! */
    		{
    			action[left_pressed] = 0; action[right_pressed] = 0;
    			action[up_pressed] = 1;   action[down_pressed] = 0;
    		}
    		else if (keyg == GP2X_JOYSTICK_S)                              /* Thrust DOWN! */
    		{
    			action[left_pressed] = 0; action[right_pressed] = 0;
    			action[up_pressed] =0;	  action[down_pressed] = 1;
    		}
    		else if (keyg == GP2X_BUTTON_A)   { action[zoom_pressed] = -1;  }  /* Zoom _out */
    		else if (keyg == GP2X_BUTTON_B)   { action[zoom_pressed] = 1;   }  /* Zoom_in */
    		else if (keyg == GP2X_BUTTON_X)   { action[shift_pressed] = -1; }  /* shift left  */
    		else if (keyg == GP2X_BUTTON_Y)   { action[shift_pressed] = 1;  }  /* shift right */
     		if (keyg == GP2X_BUTTON_SELECT)   { *pdone = -1;  *pquit = 1;   }  // Quit!
        }
		break;
/*
    case SDL_JOYBUTTONUP:
		{
            fprintf(fi,"joybuttonup\n");
        	if      (keyg == GP2X_JOYSTICK_E) { action[right_pressed] = 0; }
        	else if (keyg == GP2X_JOYSTICK_W) { action[left_pressed] = 0;  }
        	else if (keyg == GP2X_JOYSTICK_N) { action[up_pressed] = 0;    }
        	else if (keyg == GP2X_JOYSTICK_S) { action[down_pressed] = 0; 	}
        	else if (keyg == GP2X_BUTTON_A)    { action[zoom_pressed] = 0; 	}
        	else if (keyg == GP2X_BUTTON_B)    { action[zoom_pressed] = 0;	}
            else if (keyg == GP2X_BUTTON_Y)    { action[shift_pressed] = 0; }
        	else if (keyg == GP2X_BUTTON_X)    { action[shift_pressed] = 0;	}
        }
		break;
*/
#endif
	default:
#ifdef GP2X
           reset_eventos(action);
#endif
		    break;
	}// endswitch

	} // endwhile  /*1*/


}

/***********************************************************************
* check_base: Comprueba si el LEM está en las cercanías de una base
*   Entradas : x : posición X
*              bs : lista de bases del satélite
*   Salidas  : 0 : Fuera de la base
*              1 : Dentro de la base
************************************************************************
*/

int check_base(int px,landing_zone bs[])
{
    int result = 0;
    int i;

    for (i=0;( (i<6) && (!result) );i++)
    {
     if ( abs(bs[i].x -px )<= 5 )  result =1;
    }

    return result;
}

/***********************************************************************
* check_ground: Compurba la toma de contacto con la superficie
*               en función del nivel de dificultad elegido las condiciones
                del aterrizaje serán más o menos exigentes
*   Entradas : h : distancia a tierra
*              x_v,y_v : Velocidades en el momento de la toma de contacto
*              niveld: Nivel de dificultad
*   nota 1     Para el eje Y consideramos que existen amortiguadores y
*              se permite un margen de tolerancia en altura
*              En el eje X no hay tolerancia
************************************************************************
*/

int check_ground(int h, int hx,int x_v, int y_v, int nivel_d)
{
    int result; /* Resultado del alunizaje 0 = no tomó tierra
                                           1 = exito
                                          -1 = fallido
                */
    int uh,ux,uy;
    result = 0;

    switch (nivel_d)
    {
      case 1:       // Principiante
            {
              uh = 4;
              uy = ux = 7;
              break;
            }
      case 2:       // Intermedio
            {
              uh = 2;
              uy = ux = 5;
              break;
            }
      case 3:       // Experto
            {
              uh = 2;
              uy = ux = 2;
              break;
            }
      default:
              break;
    }

    if ( (abs(hx)-FC_X >0 )&& ((h-FC_Y) > 0) )  return result;
    // Estamos en una base?
    if (check_base(x,base)) // Cercanias de una base
    {
        h = abs(h-FC_Y);
        y_v = abs(y_v);
        x_v = abs(x_v);
        if ((h <= uh) && (y_v <=uy) &&  (x_v <= ux) && (abs(hx)-FC_X)>0)
            result = 1;
        else result = -1;
    }
    else result = -1 ; // No estamos en ninguna base
    return result;
}

/***********************************************************************
* check_fuel: Gestiona el consumo de fuel en función del impulso
*   Entradas : impulso : Impulso aplicado
*              dt : Intrevalo de tiempo
************************************************************************
*/

float check_fuel(float thrust, float dt)
{
     float dfuel;

     if (thrust || fuel)
     {
         dfuel = thrust * dt;
         fuel -= dfuel;
         if (fuel <0)
         {
           fuel = 0;
           dfuel= 0;
         }
     }
    return dfuel;
}


/***********************************************************************
* check_dist: Calcula distancia al terreno
*   Entradas : x0: Posicion X absoluta
*              y0: Posición Y absoluta
*              topo : Terreno
*   Salidas  : hx : Distancia horizontal
*              hy : Distancia vertical
*
*    y = y1 + ( y2-y1)/(x2-x1) * ( x -x1)
*
*   nota 1: Siempre que la posisción Y sea > Cota máxima del terreno
*                 no es necesario efectuar los cálculos de distancia
*                 en horizontal
************************************************************************
*/

int check_dist(int *distx,int *disty, int xp, int yp, terrain topo[])
{
 float m;
 int i;
 float j;
 int dist = 0;
 //int x1,x2,y1,y2;

 j = xp/50.0;      // ubicar los puntos de la sección de terreno
 i = (int) j;
 if ((i-j)==0)
 { // Punto exacto
            dist = yp - topo[i].y;
            *disty = dist;
            *distx = dist;  // Este caso es complicado
            // no lo implementamos aún.
 }
else
 { // Interseccion con recta entre dos puntos
     m = (float)(topo[i+1].y-topo[i].y)/(topo[i+1].x-topo[i].x); // Pendiente de la  recta
     j = topo[i].y + m * (xp-topo[i].x); // Distancia eje y
     dist = yp - (int) j;
     *disty = dist;
     if (yp < 2100)   // ver nota 1
     {
        j = topo[i].x + (yp -topo[i].y) / m; // Distancia eje x
        dist = abs (xp - (int)j);
     }
     *distx = dist;
 }



 return dist;

}




/***********************************************************************
* fisica : Calcula los vecrtores  aceleración, velocidad y la posición
*  Entradas : impulso: Impulso instantaneo programado
*             angulo : Angulo del LEM, con respecto a la vertical
*             dt   : Intervalo de tiempo
************************************************************************
*/

void fisica(float thrust,int gangle, float dt)
{

  static int loop=0;
  float theta;
  float dmasa=0;

// Calcula consumo de fuel
 dmasa=check_fuel(thrust,dt);
// Calculamos componentes x e y del impulso
 theta = gangle * CNV;  // convertir a radianes

 if (cfg_var[FIS])
 {
  m0 = m0 - dmasa;                 // El peso neto dismuye si dismuye
  thrust = U * thrust;
 }                                 // el fuel de los tanques
 else
 {
 m0=1; // No tenemos encuenta el efecto de masa del LEM
 }
 x_thrust = thrust * cos (theta);
 y_thrust = thrust * sin (theta);
 x_a = x_thrust/m0;             // Acelaración
 y_a = y_thrust/m0 - G;
 x_vel = x_vant + x_a * dt;     // Velocidad
 y_vel = y_vant + y_a * dt;
 x_pos = x_posant + x_vel * dt; // Desplazmiento
 y_pos = y_posant + y_vel * dt;
 loop++;
 // Preparamos variables para siguiente interación
 x_vant = x_vel;
 y_vant = y_vel;
 x_posant = x_pos;
 y_posant = y_pos;
}

/******************************************************************************
* auto_zoom : Ajusta el nivel de zoom en función de la distancia a la superficie
*   Entradas : h: Distancia a la superfice
*               scale: Zoom actual
*               scale_old: Zoom anterior
*   Salidas  : 0 : no se hace nada
*              -1 : zoom in
*              1 : zoom out
*
*
*******************************************************************************/

int auto_zoom(int hl)
{
  //  int static flip=0; /* NOUSE */
    int scale=1;

    if (hl > 600) scale = 4;
    else if ((hl<=600)&& (hl>300)) scale = 3;
    else if ((hl<=300)&& (hl>100)) scale = 2;
    else if (hl<=100) scale =1;
    return scale;
}


/*********************************************************************
* int draw_result : Genera los mensajes tras el alunizaje
*  entradas  : result : -1 Usuario abandona
*                        1 Usuario se estrella
*                        2 Usuario Aluniza
**********************************************************************
*/

int draw_result (int gas,int scale,int result)
{
    char str[64];
    color_type c;
    int d,size;
    int i,j=1;
    float rms=2.0;

    size = WIDTH/ASPECT;
  switch (result)
  {
    case -1:
         return 0;
    case 1:
     {
         #ifdef SOUND
           // Sonido explosión. Inicio
           playsound(SND_EXPLODE);
         #endif
         sprintf ( str,"ALUNIZAJE FALLIDO");
         c = mkcolor(255,0,0);
         d = 1;
         draw_lander(Plander,x,y,angle,gas,2,scale,mkcolor(0,0,0));
         for (i=0;i<60;i++)
         {
          if (j)
          draw_explo(explo_lem,x,y,angle,(int)rms,scale,mkcolor(255,255,255));
          else
          draw_explo(explo_lem,x,y,angle,(int)rms,scale,mkcolor(0,0,0));
          j=!j;
          if (i%2)
          {
          angle +=5;
          rms = rand() % 5 +2;
          if (angle >= 360) angle = 0;
          }
          /* Flush and pause! */
          SDL_Flip(screen);
          SDL_Delay(1);
          SDL_Delay(30);
         }
     }
     break;
    case 2:
    {
     #ifdef SOUND
       // Sonido Alunizaje. Inicio
        playsound(SND_EXTRALIFE);
     #endif
     sprintf ( str,"ALUNIZAJE CON EXITO");
     c = mkcolor(0,255,0);
     d=2;
     break;
    }
  }
     //draw_terrain(moon_a,0,scale);             // Dibuja Terreno
     //draw_bases(base,0,scale);

     draw_text(str, 100*size, 220*size , 5*size, c);
  SDL_Flip(screen);
  SDL_Delay(3000);
  #ifdef SOUND
         // Parar sonidos
  #endif
  return d;
}


/*******************************************************************************
* game () : Implementación del modelo de alunizaje
*          Inicializa variables
*          repetir
*                 Lee teclado/joy
*                 Procesa acciones
*                 Fisica de la simulación
*                 Visualiza la simulación
*                       Gestión escala representación y scroll
*                           Ajuste de cámara en función distancia a superficie
*                           Gestión scroll si necesario
*                           Simular Escenario contínuo si necesario
*                       Dibuja objetos
*                              Pinta terreno
*                              Pinta bases de alunizaje
*                              Pinta LEM
*                 Gestión alunizaje
*           hasta fin ( abandona, exito, fracaso)

     #ifdef DEP
        sprintf(svar[0],"scale %d",scale);
        sprintf(svar[1],"zoom %d",zoom[scale]);
        sprintf(svar[2],"yf %4.1f",y_pos);
        sprintf(svar[3],"thrust %2.3f",impulso[gas]);
        sprintf(svar[4],"fuel %4.0f",fuel);
        sprintf(svar[5],"v_y %4.1f",*d_y);
        sprintf(svar[6],"dt %4.6f",dt);
        sprintf(svar[7],"angle%3.2d",langle);
        sprintf(svar[8],"%.6d",variable);
        sprintf(svar[9],"%.6d",variable);
     #endif
#ifdef DEP
        i = 30;
        DT_DrawText(svar[0], screen, DT_font, 0, 0 + i );
        DT_DrawText(svar[1], screen, DT_font, 0, 14 + i );
        DT_DrawText(svar[2], screen, DT_font, 0, 28 + i );
        DT_DrawText(svar[3], screen, DT_font, 0, 42 + i );
        DT_DrawText(svar[4], screen, DT_font, 0, 56 + i );
        DT_DrawText(svar[5], screen, DT_font, 0, 70 + i );
        DT_DrawText(svar[6], screen, DT_font, 0, 84 + i );
        DT_DrawText(svar[7], screen, DT_font, 0, 98 + i );
        DT_DrawText(svar[8], screen, DT_font, 0, 112 + i);
        DT_DrawText(svar[9], screen, DT_font, 0, 126 + i);
#endif



*******************************************************************************/
int game(void)
{
  int done, quit;

//  Uint64 t0;		// Tiempo de referencia
  Uint64 curr_time;
  Uint64 prev_time;
//  char str[10];
  int gas;          // nivel de impulso que se da al LEM
  int h,hx,hy;      // Altura restante para llegar al suelo
  int scale;        // Nivel de zoom actual
  int scale_old;    // Nivel de zoom anterior
  int actiond[6];   // Lista de posibles acciones en el juego
  int shift_dir;    // Sentido del scroll
  float xf,yf;      // Variables auxiliareas para gestión pantalla
 // float v,vant;
  float dtime;       // diferencial de tiempo entre cada iteración

  // auxiliares
//  int xx;
//  int yy;
  int j;
  //float acc=0;
  //int flag=0;
  /***************** Inicialización de variables ***************************/
  done = 0;
  quit = 0;
  actiond[left_pressed] = 0;	  actiond[right_pressed] = 0;
  actiond[up_pressed] = 0;		  actiond[down_pressed] = 0;
  actiond[shift_pressed] = 0;	  actiond[zoom_pressed] = 0;
  scale = LEVEL;                  scale_old = scale;
  lives = 3;
  level = 1;
  player_alive = 1;
  player_die_timer = 0;
  angle = 90;
 // xx=0;     	yy=0;
  Scx =0;     Scy = 0;
  fSct =0;
  x_scroll=0; y_scroll=0;
  //*********************************************************
  gas =0;
  fuel=10800.0;
  if (cfg_var[FIS])  m0 = fuel + mu;
  else m0 = 1;

  prev_time = SDL_GetTicks(); // toma referencia de tiempos
  curr_time = prev_time;
  x=430;
  y=2600;
  x_pos = x;
  y_pos = y;
  x_posant = x;
  y_posant = y;


   /********************** Bucle principal ************************************/
  do
    {
     maneja_eventos(actiond,&done,&quit,&gas);            /* Maneja Eventos: */
     /************** Procesa acciones **************/
    if (actiond[right_pressed])   // Rotate ship
	{
	  angle = angle - 5;
	  if (angle < 0)
	    angle = angle + 360;
	}
      else if (actiond[left_pressed]) // Rotate ship
	{
	  angle = angle + 5;
	  if (angle >= 360)
	    angle = angle - 360;
	}
    if (actiond[up_pressed])
	   {
         gas+=1;
         if (gas>9)  gas=9;
         actiond[up_pressed] =0;
         #ifdef SOUND //  Start thruster sound:
         if (!Mix_Playing(CHAN_THRUST))
	   	 {
		  Mix_PlayChannel(CHAN_THRUST, sounds[SND_THRUST], -1);
		 }
         #endif
	    }
      else if (actiond[down_pressed])
        {
         gas-=1;
         if (gas<0)  gas=0;
         actiond[down_pressed]=0;
         #ifdef SOUND
         if ((gas==0) && Mix_Playing(CHAN_THRUST))
           	  {
           		Mix_HaltChannel(CHAN_THRUST);
              }
         #endif
	}

    /************** Fisica del Juego ******************************************/
    curr_time = SDL_GetTicks();              // Calcular
    dtime = curr_time - prev_time;           // el paso
    prev_time = curr_time;                   //  del iempo
    //fisica(impulso[gas],angle,dtime/1000);   //  acc, vel, desp, consumo de fuel
    xf = x_pos;
    yf = y_pos;
    x = (int)round(xf);
    y = (int)round(yf);
    // Comprueba distancia eje Y
    //h = y - (moon_a[x/50].y);
    check_dist(&hx,&hy,x,y,moon_a);
    h = hy;
    scale = auto_zoom(h);
    /****************** Ajuste de cámara en función de la distancia al suelo **/
    //if (check_scale(actiond[zoom_pressed],&scale,&scale_old)) update_camera(x,y,scale,&Scx,&Scy);
    if (scale != scale_old)
    {
       update_camera(x,y,scale,&Scx,&Scy);
       scale_old = scale;
    }
    actiond[zoom_pressed]=0;
    fSct = (xf/zoom[scale])- Scx;
    if (fSct<0) fSct = fSct + MOON_MAX_X/zoom[scale];
    else if (fSct>WIDTH) fSct = fSct - MOON_MAX_X/zoom[scale];

    // Check if scroll needed
    if (fSct >= WIDTH -32) shift_dir = 1; // Ajuste del scroll a la escala
    if (fSct <= 32) shift_dir = -1;
    if (actiond[shift_pressed])
    {
      x_scroll = (float)(5* shift_dir)/zoom[scale]; // toma el sentido del desplazamiento
      Scx = Scx  + x_scroll;
      /*
        No podemos incrementar indefinidamente Scx en un scroll contínuo cuando hemos dado la vuelta a la superfice
        hay que inicializar Scx al desplazamiento inicial cuando se genero el viewport.
        La siguiente secuencia solo sirve en la escala máxima, donde da la casualidad que Scx = 0;
      */
      if (Scx>=MOON_MAX_X/zoom[scale])
      {
           printf("Hemos dado la vuelta\n");
           Scx =0;
       };
      xf = xf + x_scroll*zoom[scale];
      actiond[shift_pressed] = 0;
    }
    else x_scroll =0;   // Reajusta valores para dar sensación de escenario continuo
    if (xf<=0) xf = xf + MOON_MAX_X;
    else if (xf>=MOON_MAX_X) xf = xf -MOON_MAX_X;
    x = (int)round(xf);
    y = (int)round(yf);

   if (if_frame)               // Si toca volcar a pantalla
    {
     SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));   // LimpiaPantalla
     draw_terrain(moon_a,0,scale);                                      // Dibuja Terreno
     draw_bases(base,0,scale);                                          // Dibuja Bases
     draw_stars(star,scale);                                            // Dibuja estrellas
     draw_score(fuel,hy,hx,(int) y_vel, (int) x_vel);                   // Dibuja Indicadores
     draw_fuel (gas,120,5);
     draw_lander(Plander,x,y,angle,gas,2*WIDTH/ASPECT,scale,mkcolor(255,255,255));   // Dibuja el LEM

     /**** gestión Alunizaje *********************************/
     j = check_ground(h,hx,x_vel, y_vel,1);
     if (j)
     {
             if (j == -1) done = 1;
             else done = 2;
             if (done == 2)
             {
                       add_score(x,1,base);
             }
     }
     /******************/
     SDL_Flip(screen);
     SDL_Delay(1);

     }
    if_frame=wait_fps();
    }
  while (!done);
  j = draw_result(gas,scale,done);
  return(done);
}
/* Title screen: ****************************************************/

int title(void)
{
  int done,  quit;
  //float rms;
  int i;
  //int snapped;
 //  int angle;
 int size= WIDTH/ASPECT; //It only work properly on windows with 4:3 factor
   int counter;
   //int x, y, xm;
  // int ym;
   int  z1, z2, z3;
  SDL_Event event;
  SDLKey key;
  char * titlestr = "LUNAR 2007";
  char str[20];

  //snapped = 0;
  counter = 0;
 // angle = 90;
  //size = 40;
  done = 0;
  quit = 0;
  //rms = 2.0;
  do
  {
    counter++;
    i=0;
    /* Handle events: */
    while (SDL_PollEvent(&event) > 0)
    {
      if (event.type == SDL_QUIT)
      {	done = 1;  quit = 1; }
      else if (key == SDLK_SPACE)
	  { done = 1;  quit = 1; }
      else if (event.type == SDL_KEYDOWN)
      {
        key = event.key.keysym.sym;
    	if (key == SDLK_RETURN)
        {  done = 1;}
    	else if (key == SDLK_SPACE)
    	{ done = 1; quit = 1;	}
       }
#ifdef GP2X
      if(event.type == SDL_JOYBUTTONDOWN)
      {
			switch (event.jbutton.button)
				{
					case GP2X_BUTTON_START :
						done = 1;				// ENTER
						break;
					case GP2X_BUTTON_B :
						done = 1;				// FIN
						quit = 1;
						break;
      					default:
						break;
				}
      }
 #endif
    } // while

  if (if_frame)
    {
      SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));


      z1 = (i + counter) % 255;   z2 = ((i + counter + 128) * 2) % 255;    z3 = ((i + counter) * 5) % 255;

      draw_centered_text(titlestr, size*100, 9*size,mkcolor(255, 255,255));
      draw_centered_text("BY RACE", size*140, 5*size,mkcolor(128, 128,255));
      draw_centered_text("HANDHELD DEVS", 155, 5*size,mkcolor(255, 0, 0));
      sprintf(str, "HIGH %.6d", score);
      draw_text(str, (WIDTH - 110) / 2, size*5, 5*size, mkcolor(255, 255, 0));
      draw_text("GP2X", (WIDTH - 30) / 2, size*170, 5*size, mkcolor(0, 255, 0));

// a more direct implementation avoiding draw_line use
      lineRGBA(screen,40,40,WIDTH-40,40,z1,z2,z3,255);
      lineRGBA(screen,WIDTH-40,40,WIDTH-40,HEIGHT-40,z1,z2,z3,255);
      lineRGBA(screen,WIDTH-40,HEIGHT-40,40,HEIGHT-40,z1,z2,z3,255);
      lineRGBA(screen,40,40,40,HEIGHT-40,z1,z2,z3,255);
/*
Old implementation, it works since draw_line has been recoded using lineRGBA,
but unused color parameters make more inneficient
      draw_line(40,80,mkcolor(z3, z1, z2),280,80,mkcolor(z3, z1, z2));
      draw_line(280,80,mkcolor(z1, z3, z2),280,200,mkcolor(z1, z3, z2));
      draw_line(280,200, mkcolor(z1, z2, z3),40,200, mkcolor(z1, z2, z3));
      draw_line(40,200,mkcolor(z2, z3, z1),40,80,mkcolor(z2, z3, z1));*/
     // Flush and pause!
      SDL_Flip(screen);
      SDL_Delay(1);
     }
   if_frame=wait_fps();
   }
  while (!done);
return(quit);
}

/*******************************************************************
*  menu : Menú de opciones del juego
*      entradas ninguna
*      salidas : Selección del usuario
*                1   Selección de nivel
*                2   Jugar
*                3   Abrir opciones de configuración
*                4   Abandonar el juego
********************************************************************/

int menu()
{
  SDLKey key;
  #ifdef GP2X
        Uint8 keyg;
  #endif // GP2X
  SDL_Event event;

  int size = WIDTH / ASPECT;

  char opciones [2][4][20] =
   {
     {                      // Español
     {"ELEGIR NIVEL"},{"COMENZAR MISION"},{"CONFIGURAR"},{"SALIR"}
     },
     {                      // Inglés
     {"CHOOSE LEVEL"},{"START MISSION"},{"CONFIGURE"},{"EXIT"}
     }
  };
  int elegida[4]={0,1,0,0};
  int j=1; // misma opción a 1 que elegida
  char * titlestr = "LUNAR 2007: MENU";
  int i;
  //int k;
  int fin=0;

  color_type crojo,cverde,camarillo,cazul,cblanco;  // Colores básicos
  color_type colores[4];

  crojo = mkcolor(255,0,0);
  cverde= mkcolor(0,255,0);
  camarillo = mkcolor ( 255,255,0);
  cazul= mkcolor(0,0,255);
  cblanco= mkcolor(255,255,255);
  colores[0]=cverde;
  colores[1]=camarillo;
  colores[2]=cazul;
  colores[3]=crojo;
  //k=0;

 do
 {
    /* Handle events: */


while (SDL_PollEvent(&event) > 0) /*1*/
 {
	switch(event.type)
	{
      case SDL_QUIT:           // Tecla de escape
      {
           fin=1;
      }
      break;
      case SDL_KEYDOWN:        // Tecla pulsada o soltada
      case SDL_KEYUP:
        {
         	key = event.key.keysym.sym;
        	if (event.type == SDL_KEYDOWN) //--------------TECLA PULSADA
    		{
      		if (key == SDLK_RETURN) { 	fin=1; }               /* Salir al menú principal */
    		else if (key == SDLK_UP)                                /* Thrust UP! */
    		{
                 elegida[j]=0;
                 j --;
                 if (j<0) j=3;
    		}
    		else if (key == SDLK_DOWN)                              /* Thrust DOWN! */
    		{
                 elegida[j]=0;
                 j = (j+1)%4;
            }
        	}
        	else if (event.type == SDL_KEYUP)  //--------------- TECLA LIBERADA
        	{
                    j=j;
        	}
    }
    break;
#ifdef GP2X
	case SDL_JOYBUTTONDOWN:
 	case SDL_JOYBUTTONUP:
		{
         	keyg = event.jbutton.button;
        	if (event.type == SDL_JOYBUTTONDOWN) //--------------TECLA PULSADA
    		{
      		if (keyg == GP2X_BUTTON_SELECT) { 	fin=1; }               /* Salir al menú principal */
    		else if (keyg == GP2X_JOYSTICK_N)                                /* Thrust UP! */
    		{
                 elegida[j]=0;
                 j --;
                 if (j<0) j=3;
    		}
    		else if (keyg == GP2X_JOYSTICK_S)                              /* Thrust DOWN! */
    		{
                 elegida[j]=0;
                 j = (j+1)%4;
            }
        	}
        	else if (event.type == SDL_JOYBUTTONUP)  //--------------- TECLA LIBERADA
        	{
                    j=j;
        	}

        }

        break;

#endif
	default:
		break;
	}// endswitch

	} // endwhile  /*1*/



    /* Paint */

        {
          SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));   // LimpiaPantalla
          // dibuja todos elementos
          draw_text(titlestr, size*80, size*16, size*6, cblanco);
          draw_line(size*20,size*40,mkcolor(128, 128,255),size*300,size*40,mkcolor(128, 128,255));
          draw_line(size*20,size*208,mkcolor(128, 128,255),size*300,size*208,mkcolor(128, 128,255));
          draw_centered_text("RACE HANDHELD DEVS 2007", size*220, size*3,mkcolor(244, 244, 200));
          elegida[j]=1;
          for(i=0;i<4;i++)
          {
            draw_text(opciones[cfg_var[IDIOMA]][i], size*90, size*(82+(i*26)), size*6, colores[i]);
            if (elegida[i])
            {
            draw_text(opciones[cfg_var[IDIOMA]][i], 1+size*90, size*(82+(i*26)), size*6, colores[i]);
            //draw_text(opciones[cfg_var[IDIOMA]][i], 2+size*90, size*(82+(i*26)), size*6, colores[i]);
            draw_lander(Plander,size*40,size* (HEIGHT-92-(i*26)),90,5,size*2,0,mkcolor(255,255,255));
            };
          };

          /* Flush and pause! */
          SDL_Flip(screen);
          SDL_Delay(1);
         }
      if_frame=wait_fps();
       }
  while (!fin);
SDL_Delay(250); // 3 seg.

    return (j+1);
}


/*******************************************************************
*  sel_config : Menú de opciones del configuracion

********************************************************************/

void sel_config()
{
static int elegida[3] = {1,0,0};
  SDLKey key;
#ifdef GP2X
    Uint8 keyg;
#endif // GP2X
  SDL_Event event;
char opciones [2][3][20] =
{
  {                      // Español
  {"IDIOMA"},{"MUSICA"},{"SALIR"}
  },
  {                      // Inglés
  {"LANGUAGE"},{"MUSIC"},{"EXIT"}
  }
};

  char valor[2][2][20] = {{"ESP","ENG"},{"ON","OFF"}};
  int j=0;
  char * titlestr = "LUNAR 2007: CONFIG";
  int i;
  int fin=0;
  int size = WIDTH / ASPECT;
  color_type crojo,cverde,camarillo,cazul,cblanco;  // Colores básicos
  color_type colores[4];

  crojo = mkcolor(255,0,0);
  cverde= mkcolor(0,255,0);
  camarillo = mkcolor ( 255,255,0);
  cazul= mkcolor(0,0,255);
  cblanco= mkcolor(255,255,255);
  colores[0]=cverde;
  colores[1]=camarillo;
  colores[2]=cazul;
  colores[3]=crojo;
//  k=0;
 do
 {
    /* Handle events: */


while (SDL_PollEvent(&event) > 0) /*1*/
 {
	switch(event.type)
	{
      case SDL_QUIT:           // Tecla de escape
      {
           fin=1;
      }
      break;
      case SDL_KEYDOWN:        // Tecla pulsada o soltada
      case SDL_KEYUP:
        {
         	key = event.key.keysym.sym;
        	if (event.type == SDL_KEYDOWN) //--------------TECLA PULSADA
    		{
      		if (key == SDLK_RETURN)
              {
                if (j!=2)
                {
                cfg_var[j+IDIOMA]=!cfg_var[j+IDIOMA];
                }
                else
                fin=1;                                   /* Salir al menú principal */
              }
    		else if (key == SDLK_UP)                                /* Thrust UP! */
    		{
                 elegida[j]=0;
                 j --;
                 if (j<0) j=2;
    		}
    		else if (key == SDLK_DOWN)                              /* Thrust DOWN! */
    		{
                 elegida[j]=0;
                 j = (j+1)%3;
            }
    	}
    	else if (event.type == SDL_KEYUP)  //--------------- TECLA LIBERADA
    	{
                j=j;
    	}
    }
    break;
#ifdef GP2X
	case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
		{
         	keyg = event.jbutton.button;
        	if (event.type == SDL_JOYBUTTONDOWN) //--------------TECLA PULSADA
    		{
      		if (keyg == GP2X_BUTTON_SELECT)
              {
                if (j!=2)
                {
                cfg_var[j+IDIOMA]=!cfg_var[j+IDIOMA];
                }
                else
                fin=1;                                   /* Salir al menú principal */
              }
    		else if (keyg == GP2X_JOYSTICK_N)                                /* Thrust UP! */
    		{
                 elegida[j]=0;
                 j --;
                 if (j<0) j=2;
    		}
    		else if (keyg == GP2X_JOYSTICK_S)                              /* Thrust DOWN! */
    		{
                 elegida[j]=0;
                 j = (j+1)%3;
            }
    	}
    	else if (event.type == SDL_JOYBUTTONUP)  //--------------- TECLA LIBERADA
    	{
                j=j;
    	}
		}
		break;
#endif
	default:
		break;

	}// endswitch

	} // endwhile  /*1*/



    /* Paint */

        {
          SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));   // LimpiaPantalla
          // dibuja todos elementos
          draw_text(titlestr, size*80, size*16, size*6, cblanco);
          draw_line(size*20,size*40,mkcolor(128, 128,255),size*300,size*40,mkcolor(128, 128,255));
          draw_line(size*20,size*208,mkcolor(128, 128,255),size*300,size*208,mkcolor(128, 128,255));
          draw_centered_text("RACE HANDHELD DEVS 2007", size*220, size*3,mkcolor(244, 244, 200));
          elegida[j]=1;
          for(i=0;i<3;i++)
          {
            draw_text(opciones[cfg_var[IDIOMA]][i], size*90, size*(92+(i*26)), size*6, colores[i]);
            if (i!=2)  draw_text(valor[i][cfg_var[IDIOMA+i]], size*190, size*(92+(i*26)), size*6, colores[i]);
            if (elegida[i])
            {
            draw_text(opciones[cfg_var[IDIOMA]][i], 1+size*90, size*(92+(i*26)), size*6, colores[i]);

            draw_line(size*90, size*(109+(i*26)),mkcolor(230, 230,230),size*176, size*(109+(i*26)),mkcolor(230, 230,230));
            };
          };

          /* Flush and pause! */
          SDL_Flip(screen);
          SDL_Delay(1);
         }
      if_frame=wait_fps();
       }
  while (!fin);
SDL_Delay(250); // 3 seg.



    //return 0;
}


/*******************************************************************
*  sel_nivel : Menú de selección de nivel

********************************************************************/

void sel_nivel()
{

  SDLKey key;
  #ifdef GP2X
        Uint8 keyg;
  #endif
  SDL_Event event;

  static int elegida[3] = {1,0,0};
  int size = WIDTH / ASPECT;
  int i=0;
  int j=0;
  //static int rms=0; /* NOUSE */
  int termina;
  char * titlestr = "LUNAR 2007: MISION";
  char opciones [2][3][20] =
		{
		{"FACIL","MEDIA","DIFICIL"},
		{"EASY","MEDIUM","HARD"}
		};
  int pos_opc [2][3] =
        {
              {0,92,180},
              {0,88,190}
        };
  char grafico[2][3][20] =
  {
	{"./LEM1.bmp","./LEM2.bmp","./LEM3.bmp"},      // Imagen sin seleccionar
	{"./LEM1W.bmp","./LEM2W.bmp","./LEM3W.bmp"}	  // Imagen seleccionada
  };

   termina = 0;
while (!elegida[j]) j++;

 do
 {
 // lee_teclado

    while (SDL_PollEvent(&event) > 0)
     {
    	switch(event.type)
    	{
          case SDL_QUIT:           // Tecla de escape
          {
               termina=1;
          }
          break;
          case SDL_KEYDOWN:        // Tecla pulsada o soltada
          case SDL_KEYUP:
            {
             	key = event.key.keysym.sym;
            	if (event.type == SDL_KEYDOWN) //--------------TECLA PULSADA
        		{
          		  if (key == SDLK_RETURN)
                    {
                       termina=1;
                    }
        		  else if (key == SDLK_LEFT)
        		    {
                      elegida[j]=0;
                      j --;
                      if (j<0) j=2;
                      elegida[j]=1;
        		    }
        		  else if (key == SDLK_RIGHT)                                      		{
                    {
                      elegida[j]=0;
                      j = (j+1)%3;
                      elegida[j]=1;
                     }
                 }
        	}
        	else if (event.type == SDL_KEYUP)  //--------------- TECLA LIBERADA
        	{
                    j=j;
        	}
        }
        break;
    #ifdef GP2X
    	case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:

        {
             keyg = event.jbutton.button;
 	         if (event.type == SDL_JOYBUTTONDOWN) //--------------TECLA PULSADA
        		{
          		  if (keyg == GP2X_BUTTON_SELECT)
                    {
                       termina=1;
                    }
        		  else if (keyg == GP2X_JOYSTICK_W)
        		    {
                      elegida[j]=0;
                      j --;
                      if (j<0) j=2;
                      elegida[j]=1;
        		    }
        		  else if (keyg == GP2X_JOYSTICK_E)                                      		{
                    {
                      elegida[j]=0;
                      j = (j+1)%3;
                      elegida[j]=1;
                     }
                 }
        	 }
        	 else if (event.type == SDL_JOYBUTTONUP)  //--------------- TECLA LIBERADA
        	    {
                    j=j;
        	    }
        }

    		break;
    #endif
    	default:
    		break;
    	}// endswitch

    	} // endwhile

     // borrar
          SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
     // pinta
          draw_text(titlestr, size *80, size *16, size *6, mkcolor(255, 255,255));
          draw_line(size *20,size *40,mkcolor(128, 128,255),size *300,size *40,mkcolor(128, 128,255));
          draw_line(size *20,size *208,mkcolor(128, 128,255),size *300,size *208,mkcolor(128, 128,255));
          draw_centered_text("RACE HANDHELD DEVS 2007", size *220, size *3,mkcolor(244, 244, 200));
     for (i=0;i<3;i++)
     {

    	draw_image(grafico[elegida[i]][i], size*(32+(i*92)),size*82);
    	//draw_image("LEM3W.bmp", 32+(i*92),82);
     	draw_text(opciones[cfg_var[IDIOMA]][i], size *(48+pos_opc[cfg_var[IDIOMA]][i]), size *158, size *4, mkcolor(255, 255, 255));
      }
     SDL_Flip(screen);
     SDL_Delay(1);
     if_frame=wait_fps();
 }
 while (!termina);


}



/* --- MAIN --- */

int main(int argc, char * argv[])
{
  int done;
  int opcion = 0;
  //int banderin;
  setup(); /* --- Inicialización general --- */
  DT_font= DT_LoadFont("./fonts/ConsoleFont.bmp", TRANS_FONT) ;
  score = 0;
  high = 0;
  game_ended = 0;
  done = 0;
  /* --- Play music: ---*/
  #ifdef MUSIC
	  if (!Mix_PlayingMusic())
	    Mix_PlayMusic(game_music, -1);
  #endif
  score = 0;
 /*
  draw_image("test.bmp",0,0);
  SDL_Flip(screen);
  SDL_Delay(5000);
 */
 game_ended = title();  /* Splash screen */
/* --- Main app loop! ---*/
  do
  {

    opcion = menu();

        switch (opcion)
        {
          case 1:   // menú de selección de nivel
          {
            sel_nivel();
            break;
          }
          case 2: // Jugar
          {
            done = game();
            break;
          }
          case 3: // Menú de configuración
          {
           sel_config();
            break;
          }
          case 4: // Abandonar juego
          {
            game_ended = 1;
            break;
          }
        }
    }
  while ((!game_ended) && (!done));
 finish();  // System shutdown
return 0;
}
