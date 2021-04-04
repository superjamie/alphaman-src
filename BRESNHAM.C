/*
** Public Domain mode 13h Bresenham line/circle algorithms
** By Brian Dessent
**
** Written for Borland, modified for others by Bob Stout
*/

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <time.h>       /* for randomize  */

#ifndef __TURBOC__
 #define random(num) (int)(((rand())*(long)(num))/(((long)RAND_MAX)+1))
 #define randomize() srand((unsigned)time(NULL)|1)
#else
#endif

#ifndef MK_FP
 #define MK_FP(seg,offset) \
	 ((void far *)(((unsigned long)(seg)<<16) | (unsigned)(offset)))
#endif

#define SCREEN_WIDTH 320
#define SCREEN_HEIGTH 200
#define MAX_X (SCREEN_WIDTH-1)
#define MAX_Y (SCREEN_HEIGTH-1)

/* prototypes */

void setmode(int mode);
void plotdot(int x, int y, char c);
void bresenham_line(int x, int y, int x2, int y2, char c);
void bresenham_circle(int xc, int yc, int r, char c);
void main(void);

/* code begins */

/* uses BIOS to set video mode */

void setmode(int mode)
{
      union REGS r;

      r.x.ax = mode;
      int86(0x10, &r, &r);
}


/* plots a dot at (x, y) with color c */

void plotdot(int x, int y, char c)
{
      register char far *addr;

      if(x < 0 || x > MAX_X || y < 0 || y > MAX_Y)
            return;

      addr = MK_FP(0xa000, (SCREEN_WIDTH * y) + x);
      *addr = c;
}


/* draws a line from (x, y) to (x2, y2) in color c */

void bresenham_line(int x, int y, int x2, int y2, char c)
{
      int i, steep = 0, sx, sy, dx, dy, e;

      dx = abs(x2 - x);
      sx = ((x2 - x) > 0) ? 1 : -1;
      dy = abs(y2 - y);
      sy = ((y2 - y) > 0) ? 1 : -1;

      if(dy > dx)
      {
            steep = 1;
            x ^= y;  /* swap x and y */
            y ^= x;
            x ^= y;
            dx ^= dy; /* swap dx and dy */
            dy ^= dx;
            dx ^= dy;
            sx ^= sy; /* swap sx and sy */
            sy ^= sx;
            sx ^= sy;
      }

      e = 2 * dy - dx;
      for(i = 0;i < dx;i++)
      {
            if(steep)
                  plotdot(y, x, c);
            else  plotdot(x, y, c);
            while(e >= 0)
            {
                  y += sy;
                  e -= 2 * dx;
            }
            x += sx;
            e += 2 * dy;
      }
      plotdot(x2, y2, c);
}

/* draws a circle at (xc, yc) with radius r in color c
**
** note: the scaling factor of (SCREEN_WIDTH / SCREEN_HEIGTH) is used when
** updating d.  This makes round circles.  If you want ellipses, you can
** modify that ratio.
*/

void bresenham_circle(int xc, int yc, int r, char c)
{
      int x = 0, y = r, d = 2 * (1 - r);

      while(y > 0)
      {
            plotdot(xc + x, yc + y, c);
            plotdot(xc + x, yc - y, c);
            plotdot(xc - x, yc + y, c);
            plotdot(xc - x, yc - y, c);
            if(d + y > 0)
            {
                  y -= 1;
                  d -= (2 * y * SCREEN_WIDTH / SCREEN_HEIGTH) - 1;
            }
            if(x > d)
            {
                  x += 1;
                  d += (2 * x) + 1;
            }
      }
}

/* draws random lines and circles until a key is pressed in mode 13h */
/* (draws in colors 0 - 63 only) */

void main(void)
{
      int i=0;

      randomize();
      setmode(0x13);
      while(!kbhit())
      {
            bresenham_line(random(SCREEN_WIDTH), random(SCREEN_HEIGTH),
                  random(SCREEN_WIDTH), random(SCREEN_HEIGTH), i = ++i % 64);
            bresenham_circle(random(SCREEN_WIDTH), random(SCREEN_HEIGTH),
                  random(50), i = ++i % 64);
      }
      getch();
      setmode(0x03);  /* set to color text mode, clearing screen */
}
