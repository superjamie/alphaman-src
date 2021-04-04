/*
Copyright (c) 1995 Jeffrey R. Olson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdio.h>
#include <float.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
/* #include <string.h> */

#define getrandom( min, max ) ((rand() % (int)((max) - (min) + 1)) + (min) )
#define cint( x ) ( floor(x+.5) )
#define pi 3.14159
#define true -1
#define false 0
#define hor 205
#define ver 186
#define cen 206
#define ul 201
#define um 203
#define ur 187
#define ml 204
#define mrt 185
#define ll 200
#define lm 202
#define lr 188
#define lockeddoor 43
#define secretdoor 228

/*
struct com_block {
  int nnear, ncre[16][51], incastle, dots;
  int vpage, mainx,mainy, localx,localy, terrain,terrf,terrb;
  int currsym,currf,currb, ncastle, looksym;
};
*/

extern void fortran getsym(int near *, int near *, int near *, int near *, int near *, int near *);
extern void fortran putsym(int near *, int near *, int near *, int near *, int near *, int near *);

void near addaroom(int x, int y, int dx, int dy);
void near drawtunnel(int x, int y, int dx, int dy);
void far lair(int *xr, int *xl, int *yb, int *yt, int *xs, int *ys);
int  far walldist(int x, int y, int dx, int dy, int max, int incastle);
void far finddot(int *x, int *y, int incastle);
int  far rolldice(int size, int nroll, int nuse);
int  far confuse(int r, int i);
int  far roomit(int xr, int xl, int yb, int yt);
void far cmainmap(int irnd, int jrnd);
void far cdetailedmap(int t, int t1, int t2, int t3, int t4);
void far box(int x1, int x2, int y1, int y2, int nl, int fc, int pag);
void far creatsort(int nnear, int *tentgrab, int far ncre[16][51]);
int  far csameroom(int x1, int y1, int x2, int y2, int localx, int localy, int nnear, int far ncre[16][51]);
int  far badmovecreat(int newx, int newy, int nnear, int cre, int far ncre[16][51]);
void far clearright(int pag);
void far ccls(int pag);
int  far cgetsym(int x, int y, int pag);
int  far sgn(int num);
int  far isqrt(unsigned int num);
void far cputsym(int sym, int x, int y, int fc, int bc, int pag);
void far crandomize (float seed);
int  far croll(int max);
int  far crd(int x, int y);
int  far crdsimp(int x, int y);
int  far cfinddxdy(int x, int y, int n);

int nested,maxnested, xstair,ystair;

/*
main() {
}
*/

void near addaroom(int x, int y, int dx, int dy)   /*----------------------*/
{
	int i,j, xr,xl,yb,yt, x1,y1,dx1,dy1, dir,sym;
	int putroom=true;
	cputsym(219,x,y,9,0,2);
	cputsym(219,x-dx,y-dy,9,0,2);
	xr = getrandom(x,x+2);
	xl = getrandom(x-2,x);
	yb = getrandom(y,y+2);
	yt = getrandom(y-2,y);
	if(dx==1)          xr++;
	else if(dx==-1)    xl--;
	if(dy==1)          yb++;
	else if(dy==-1)    yt--;

	for(i=xl-1;i<=xr+1;i++) {
		for(j=yt-1;j<=yb+1;j++) {
			sym = cgetsym(i, j ,2);   sym %= 256;
			if(sym!=219)   putroom=false;
		}
	}

	if(putroom) {
		for(i=xl;i<=xr;i++) {   for(j=yt;j<=yb;j++)   cputsym(250,i,j,8,0,2); }
		for(i=1;i<=10;i++) {
			dir=getrandom(1,4);
			x1=getrandom(xl,xr);   y1=getrandom(yt,yb);   dx1=0;   dy1=0;
			switch(dir) {
				case 1:   x1=xl;   dx1=-1;   break;
				case 2:   y1=yt;   dy1=-1;   break;
				case 3:   x1=xr;   dx1=1;    break;
				case 4:   y1=yb;   dy1=1;    break;
			}
			drawtunnel(x1,y1,dx1,dy1);
		}
	}
	cputsym(250,x,y,8,0,2);
	cputsym(250,x-dx,y-dy,8,0,2);
}

void near drawtunnel(int x, int y, int dx, int dy)   /*-------------------*/
{
	int maxlen,max, i;
	int num;
	int dx1,dy1, dx2,dy2, dx3,dy3;
	nested++;
	maxlen = (dy==0) ? getrandom(3,getrandom(3,14)) : getrandom(3,getrandom(3,8));
	max=walldist(x,y,dx,dy,maxlen,1)-1;
	if (maxlen>max)   maxlen=max;
	if(maxlen<3) {    nested--;   return;   }
	for(i=0;i<maxlen;i++) {
		cputsym(250,x,y,8,0,2);
		x += dx;   y += dy;
	}
	x -= dx;   y -= dy;

	if(nested>4)   addaroom(x,y,dx,dy);

	if(nested<30) {
		dx1 = -dy;   dy1 = -dx;   dx2 = dy;   dy2 = dx;
		dx3 = dx;    dy3 = dy;
		drawtunnel(x,y,dx1,dy1);
		drawtunnel(x,y,dx2,dy2);
		drawtunnel(x,y,dx3,dy3);
	}

	if((maxnested<nested) && (croll(2)==2)) {
		maxnested=nested;   xstair=x;   ystair=y;
	}
	nested--;
	return;
}

void far lair(int *xr, int *xl, int *yb, int *yt, int *xs, int *ys)
{
	int xm,xw, ym,yw, i,j;
	int x,y, dx,dy;
	nested=0;
	maxnested=0;
	xm = getrandom(21,32);   xw = getrandom(6,10);
	ym = getrandom(10,13);   yw = getrandom(4,6);
	*xl = xm - xw/2;         *xr = *xl + xw;
	*yt = ym - yw/2;         *yb = *yt + yw;
	for(i=2;i<52;i++) {   for(j=2;j<22;j++)   cputsym(219,i,j,9,0,2);   }
	for(i=*xl; i<=*xr; i++) {
		for(j=*yt; j<=*yb; j++)  cputsym(250,i,j,8,0,2);
	}
	box(1,52,1,22,1,4,2);
	for(i=0;i<10;i++) {
		x=getrandom(*xl+1,*xr-1);   y=getrandom(*yt+1,*yb-1);
		dx=0;   dy=0;
		switch(croll(4)) {
			case 1:   x=*xl-1;   dx=-1;   break;
			case 2:   x=*xr+1;   dx=1;    break;
			case 3:   y=*yt-1;   dy=-1;   break;
			case 4:   y=*yb+1;   dy=1;    break;
		}
		drawtunnel(x,y,dx,dy);
	}
	cputsym(240,xstair,ystair,13,0,2);
	*xs = xstair;   *ys = ystair;
	return;
}

int far walldist(int x, int y, int dx, int dy, int max, int incastle) /*---*/
{
  int dist, k, sym,sym1,sym2;
  dist=max;
  k=0;
  while(k<max) {
	k++;
	x += dx;   y += dy;
	if(incastle==1) {
		sym = cgetsym(x, y ,2);           sym %= 256;
		sym1 = cgetsym(x+dy, y+dx ,2);    sym1 %= 256;
		sym2 = cgetsym(x-dy, y-dx ,2);    sym2 %= 256;
		if((sym!=219) || (sym1!=219) || (sym2!=219)) {
			if(croll(40)!=1) {   dist=k-1;   k=max+1;   }
		}
	}
	else {
	  sym = cgetsym(x+dx*k, y+dy*k ,2);   sym %= 256;
	  switch (sym) {
			case 32:   case 240:   case 250:   break;
			default:   dist=k;   k=max+1;      break;
	  }
	}
  }
  return dist;
}

void far finddot(int *x, int *y, int incastle)    /*----------------------*/
{
  int dot=false;
  int sym,fc,bc;
  while (dot==0) {
	 *x=getrandom(2,51);   *y=getrandom(2,21);
	 sym = cgetsym(*x,*y,2);   sym %= 256;
	 if(incastle)  {   if(sym==250)   dot=true;   }
	 else if(sym==250 || sym==249 || sym==32 || sym==247 || sym==126)
		 dot=true;
  }
}

int far rolldice(int size, int nroll, int nuse)   /*----------------------*/
{
  int dice[50];
  int i,j, temp;
  nroll = (nroll>50) ? 50 : nroll;
  nuse = (nuse>nroll) ? nroll : nuse;
  for(i=0;i<nroll;i++)   dice[i]=getrandom(1,size);
  if(nroll!=nuse) {
    for(i=0;i<nuse;i++) {
      for(j=i+1;j<nroll;j++) {
		  if(dice[i] < dice[j]) {
			  temp=dice[i];   dice[i]=dice[j];  dice[j]=temp;
        }
      }
    }
  }
  temp=0;   for(i=0;i<nuse;i++)   temp += dice[i];
  return temp;
}

int far confuse(int r, int i)                     /*----------------------*/
{
  int dir = -1;
  int z;
  switch(r) {
	 case 1071:   dir=7;   break;
	 case 1072:   dir=0;   break;
	 case 1073:   dir=1;   break;
	 case 1075:   dir=6;   break;
	 case 1077:   dir=2;   break;
	 case 1079:   dir=5;   break;
	 case 1080:   dir=4;   break;
	 case 1081:   dir=3;   break;
  }
  z=getrandom(1,100);
  switch(i) {
    case 1:
      if(z<68) {
			dir += getrandom(-2,2);
			dir = (dir<0) ? dir+8 : dir;
			dir = (dir>7) ? dir-8 : dir;
		}
		else if(z<95)   dir = r;
		else            dir=46;
      break;
    case 2:
      if(z<72) {
			dir += getrandom(-1,1);
			dir = (dir<0) ? dir+8 : dir;
			dir = (dir>7) ? dir-8 : dir;
		}
		else if(z<99)   dir = r;
		else            dir=46;
      break;
    default:
		if(z<68)        dir = getrandom(0,7);
		else if(z<95)   dir = r;
		else            dir=46;
      break;
  }

  switch(dir) {
	 case 0:   z=1072;   break;
	 case 1:   z=1073;   break;
	 case 2:   z=1077;   break;
	 case 3:   z=1081;   break;
	 case 4:   z=1080;   break;
	 case 5:   z=1079;   break;
	 case 6:   z=1075;   break;
	 case 7:   z=1071;   break;
	 default:  z=dir;    break;
  }
  return z;
}

int far roomit(int xr, int xl, int yb, int yt)     /*----------------------*/
{
  int sym;
  int i,j;
  int dots=0;

  sym = cgetsym(xr,yb,2);
  sym %= 256;
  switch(sym) {
	 case ll:   case hor:             sym=lm;    break;
	 case ul:   case um:   case ml:   sym=cen;   break;
	 case lr:   case lm:   case mrt:   case cen:
	 case lockeddoor:   case secretdoor:         break;
	 case ur:   case ver:             sym=mrt;   break;
	 default:                         sym=lr;    break;
  }
	cputsym(sym,xr,yb,9,0,2);
	/* cputsym(sym,xr,yb,9,0,2);  debug */
  sym = cgetsym(xr,yt,2);   sym %= 256;
  switch(sym) {
	 case ver:   case lr:             sym=mrt;   break;
	 case ml:    case ll:   case lm:  sym=cen;   break;
	 case um:    case ur:   case mrt:   case cen:
	 case lockeddoor:   case secretdoor:         break;
	 case ul:    case hor:            sym=um;    break;
	 default:                         sym=ur;    break;
  }
  cputsym(sym,xr,yt,9,0,2);
	/* cputsym(sym,xr,yt,9,0,1);  debug */

  sym = cgetsym(xl,yb,2);   sym %= 256;
  switch(sym) {
	 case ul:   case ver:             sym=ml;    break;
	 case um:   case ur:   case mrt:  sym=cen;   break;
	 case ml:   case ll:   case lm:   case cen:
	 case lockeddoor:   case secretdoor:         break;
	 case lr:   case hor:             sym=lm;    break;
	 default:                         sym=ll;    break;
  }
  cputsym(sym,xl,yb,9,0,2);
	/* cputsym(sym,xl,yb,9,0,1);  debug */

  sym = cgetsym(xl,yt,2);   sym %= 256;
  switch(sym) {
	 case ll:   case ver:             sym=ml;    break;
	 case lm:   case lr:   case mrt:  sym=cen;   break;
	 case ml:   case ul:   case um:   case cen:
	 case lockeddoor:   case secretdoor:         break;
	 case ur:   case hor:             sym=um;    break;
	 default:                         sym=ul;    break;
  }
  cputsym(sym,xl,yt,9,0,2);
	/* cputsym(sym,xl,yt,9,0,1);  debug */

  for(i=xl+1;i<xr;i++) {
	 sym = cgetsym(i,yt,2);   sym %= 256;
    switch(sym) {
		case ll:   case lm:   case lr:  sym=lm;   break;
		case cen:  case lockeddoor:   case secretdoor:   break;
		default:                        sym=hor;  break;
    }
	cputsym(sym,i,yt,9,0,2);
	/* cputsym(sym,i,yt,9,0,1);  debug */
	 sym = cgetsym(i,yb,2);   sym %= 256;
    switch(sym) {
		case ul:   case um:   case ur:   sym=um;   break;
		case cen:  case lockeddoor:   case secretdoor:   break;
		default:                         sym=hor;  break;
    }
    cputsym(sym,i,yb,9,0,2);
		/* cputsym(sym,i,yb,9,0,1);  debug */
	 for(j=yt+1;j<yb;j++) {   cputsym(250,i,j,8,0,2);   dots++;   }
  }
  for(i=yt+1;i<yb;i++) {
	 sym = cgetsym(xr,i,2);   sym %= 256;
    switch(sym) {
		case ll:   case ml:   case ul:   sym=ml;   break;
		case cen:  case lockeddoor:   case secretdoor:   break;
		default:                         sym=ver;  break;
    }
    cputsym(sym,xr,i,9,0,2);
		/* cputsym(sym,xr,i,9,0,1);  debug */
	 sym = cgetsym(xl,i,2);   sym %= 256;
    switch(sym) {
		case lr:   case mrt:   case ur:   sym=mrt;   break;
		case cen:  case lockeddoor:   case secretdoor:   break;
		default:                          sym=ver;   break;
    }
    cputsym(sym,xl,i,9,0,2);
		/* cputsym(sym,xl,i,9,0,1);  debug */
  }
  return dots;
}

void far cmainmap(int irnd, int jrnd)             /*----------------------*/
{
	int i,j;
	int z,z1, sym, a,b,c;
	float x,y,result,csx,csy;
	box(1,52,1,22,1,4,0);
	for(i=2;i<=17;i++) {
		y = (i-2)*.025;
		for(j=2;j<=51;j++) {
			x = (j-25)*.02;
			result = getrandom(4,7) / (.033 + x*x+ 5*(y-.1)*(y-.1));
			result = result + getrandom(4,7) / (.05 + (x-.5)*(x-.5) + 2*y*y);
			z = floor(result) - getrandom(16,50);
			if(getrandom(1,100)<z) {
				sym = (getrandom(1,1000)<7*z) ? 15 : 42;
				cputsym(sym, j+jrnd*(53 - 2*j), i+irnd*(23 - 2*i), 2+8*(getrandom(1,2)==1), 0, 0);
			}
		}
	}
	a=getrandom(1,40), b=getrandom(1,20), c=getrandom(1,300);
	for(i=14;i<=21;i++) {
		y = (i*33 - 420 + a);
		csy = cos(y/200);
		for(j=2;j<=51;j++) {
			x = (j*10 - 230 + b);
			csx = cos(x/200);
			z = 100*(csx*csx+csy*csy) + x*y/(3500 + c);
			z1 = z + croll(10)-croll(10);
			if(z<58)
				cputsym(247, j+jrnd*(53 - 2*j), i+irnd*(23 - 2*i), 1+8*(getrandom(1,2)==1), 0, 0);
			else if(z1<78)
				cputsym(177, j+jrnd*(53 - 2*j), i+irnd*(23 - 2*i), 6, 0, 0);
			else if(z1<100)
				cputsym(176, j+jrnd*(53 - 2*j), i+irnd*(23 - 2*i), 6, 0, 0);
		}
	}
}

void far cdetailedmap(int t, int t1, int t2, int t3, int t4)  /*----------*/
{
  int x,y, zx,zy, terr;
  int xx,yy;
  int z, sym, clr, clradd, bc;

  box(1,52,1,22,1,4,1);   box(1,52,1,22,1,4,2);
  for(x=2;x<=51;x++) {
    zx = 7*(abs(x-26) - 12);
    for(y=2;y<=21;y++) {
      zy = 14*(abs(y-11) - 3);
      xx = getrandom(1,100), yy = getrandom(1,100);
      terr=t;
		if(yy<zy)   terr = (y>12) ? t4 : t2;
		if(xx<zx)   terr = (x>25) ? t3 : t1;
      z = getrandom(1,100);
      clradd = getrandom(0,1) * 8;
      switch (terr) {
		  case 15:   case 94:     /* deep forest */
			 if(z>70)        sym=250;
			 else if(z>40)   sym = 249;
			 else if(z>11)   sym = 42;
			 else            sym = 15;
          clr = 2+clradd;
          break;
		  case 42:   case 127:     /* light forest */
			 if(z>70)        sym=32;
			 else if(z>40)   sym = 250;
			 else if(z>15)   sym = 249;
			 else if(z>5)    sym = 42;
			 else            sym = 15;
          clr = 2+clradd;
          break;
		  case 176:   case 30:     /* marsh */
          clr = 6;
			 if(z>24)        sym=32;
			 else if(z>10)   sym = 176;
			 else if(z>6) {  sym = 247;   clr = 1;   }
			 else if(z>2) {  sym = 42;
								  if(getrandom(1,3)!=1)   clr=2;   }
			 else {          sym = 15;
								  if(getrandom(1,3)!=1)   clr=2;   }
          break;
        case 177:     /* swamp */
          clr = 6;
			 if(z>44)        sym=32;
			 else if(z>17)   sym = 176;
			 else if(z>6) {  sym = 247;   clr = 1;   }
			 else if(z>2) {  sym = 42;
								  if(getrandom(1,2)!=1)   clr=2;   }
			 else {          sym = 15;
								  if(getrandom(1,2)!=1)   clr=2;   }
          break;
        case 247:
			 if(z<90)        sym=247;
			 else            sym=126;
          clr = (getrandom(1,6)==1) ? 3 : 1;
          break;
        default:
          clr = 6;
			 if(z>40)        sym=32;
			 else if(z>20)   sym = 250;
			 else if(z>2)    sym = 249;
			 else if(z==2){  sym = 42;   clr=2 + clradd;   }
			 else {          sym = 15;   clr=2 + clradd;   }
          break;
      }
      cputsym(sym,x,y,clr,0,2);
    }
  }
}

void far box(int lc, int rc, int tc, int bc, int nl, int fc, int pag)
{
  int a,b,c,d,e,f;
  int i;
  switch (nl) {
	 case 1:   a=196, b=179, c=218, d=191, e=217, f=192;   break;
	 default:  a=205, b=186, c=201, d=187, e=188, f=200;   break;
  }
  for(i=lc+1;i<rc;i++) {
		cputsym(a,i,bc,fc,0,pag);  cputsym(a,i,tc,fc,0,pag);
  }
  for(i=tc+1;i<bc;i++) {
		cputsym(b,rc,i,fc,0,pag);  cputsym(b,lc,i,fc,0,pag);
  }
  cputsym(c,lc,tc,fc,0,pag);   cputsym(d,rc,tc,fc,0,pag);
  cputsym(e,rc,bc,fc,0,pag);   cputsym(f,lc,bc,fc,0,pag);
  return;
}

void far creatsort(int nnear, int *tentgrab, int far ncre[16][51]) /*-----*/
{
  int i, j, offset, limit, switchit, switched, tempo;
  int cnt[51];
  int scratch[51];
  int temp[16][51];
  if (nnear >50)   nnear=50;
  for(i=1;i<=nnear;i++) {
	if (true) {    /* ncre[11][i] */
		scratch[i] = abs(ncre[4][i]) + abs(ncre[5][i]);
		if(ncre[6][i] < 0)   scratch[i] = -scratch[i];
    }
	 else   scratch[i] = 100+i;
    cnt[i] = i;
  }
  offset = floor(nnear/2.);
  switched = false;
  while(offset>0) {
    limit = nnear - offset;
    do {
      switchit = false;
      for(i=1;i<=limit;i++) {
        if(scratch[cnt[i]] > scratch[cnt[i+offset]]) {
          tempo = cnt[i];
          cnt[i] = cnt[i+offset];
          cnt[i+offset] = tempo;
          switchit = true;
          switched = true;
        }
      }
      limit = switchit - offset;
	 } while(switchit);
	 offset = floor(offset/2.);
  }
  if(switched) {
    for(i=1;i<=nnear;i++) {
		for(j=1;j<=15;j++) {   temp[j][i] = ncre[j][i];   }
    }
    for(i=1;i<=nnear;i++) {
		for(j=1;j<=15;j++) {   ncre[j][cnt[i]] = temp[j][i];   }
    }
	 if(*tentgrab>0){   i=*tentgrab;   *tentgrab=cnt[i];   }
  }
}

int far csameroom(int x1, int y1, int x2, int y2, int localx, int localy, int nnear, int far ncre[16][51])
{
	int same = true;
	int ddx = x2-x1;
	int ddy = y2-y1;
	int dxtemp = ddx - sgn(ddx);
	int dytemp = ddy - sgn(ddy);
	int x = dxtemp - sgn(dxtemp);
	int y = dytemp - sgn(dytemp);
	int rrr = crdsimp(x, y);
	int xstart, ystart;
	float dx = 0;
	float dy = 0;
	int attr, sym, fc, cr, i, j;

  if(rrr>0) {   dx = ((float)x) / rrr;   dy = ((float)y) / rrr;   }

  xstart = x1 + sgn(ddx);   ystart = y1 + sgn(ddy);
  for(i=0; i<=rrr; i++) {
		y = cint( ystart + dy*i );   x = cint( xstart + dx*i );
		if ( (x != x1+ddx) || (y != y1+ddy) ) {
			attr = cgetsym(x, y, 2);
			sym = attr % 256;   attr -= sym;   attr >>= 8;   fc = attr % 16;
			if (fc == 9)   same = false;
			else {
				if (isalpha(sym)) {
					cr = 0;
					for (j=1;j<=nnear;j++) {
						if ((ncre[4][j] == (x-localx)) && (ncre[5][j] == (y-localy)))
							cr = j;
					}
					if ( (cr>0) && (floor(ncre[8][cr] / 1000) == 9) )
						same = false;
				}
			}
		}
  }
  return same;
}


int far badmovecreat(int newx, int newy, int nnear, int cre, int far ncre[16][51])
{
  int bad = 0;
  int count = 1;
  while( (bad==0) && (count<=nnear) ) {
    if( (ncre[4][count] == newx) && (ncre[5][count] == newy) ) {
		if(count != cre){   bad = count;   return bad;   }
    }
    count++;
  }
  return bad;
}

void far clearright(int pag)                       /*----------------------*/
{
  int vidseg = 0xb800 + pag * 0x100;
  _asm {
    mov     es,vidseg
	 mov     dx,106
    mov     al,32
    mov     ah,7
  c_right:
    mov     di,dx
    mov     cx,27
    cld
     rep stosw
    add     dx,160
    cmp     dx,3660
    jl      c_right
  }
}

void far ccls(int pag)                             /*----------------------*/
{
  int vidseg = 0xb800 + pag * 0x100;
  _asm {
    mov     es,vidseg
    mov     di,0
    mov     al,32
    mov     ah,7
    mov     cx,2000
    cld
     rep stosw
  }
}

int far sgn(int num)                                   /*----------------------*/
{
  _asm {
    mov     cx,num
    cmp     cx,0
    jl      SHORT nega
    cmp     cx,0
    je      SHORT zro
    mov     cx,1
    jmp     SHORT lvsgn
  nega:
    mov     cx,-1
    jmp     SHORT lvsgn
  zro:
    mov     cx,0
  lvsgn:
    mov     ax,cx
  }
}

int far isqrt(unsigned int num)                        /*----------------------*/
{
  _asm {
    mov     cx,num
    mov     bx,cx
    mov     ax,1
    jmp     SHORT zero_check
  isqrt_1:
    sub     cx,ax
    add     ax,2
  zero_check:
    or      cx,cx
    jge     isqrt_1
    sar     ax,1
    mov     cx,ax
    imul    cx
    sub     ax,cx
    inc     ax
    cmp     ax,bx
    jbe     isqrt_exit
    dec     cx
  isqrt_exit:
    mov     ax,cx
  }
}

int far cgetsym (int x, int y, int pag)            /*----------------------*/
{
	int attr;
	if (pag==2) {
		int sym,fc,bc;
		getsym(&sym,&x,&y,&fc,&bc,&pag);
		attr = fc+bc*16;   attr <<= 8;   attr+=sym;
	}
	else {
		int row = --y;
		int col = --x;
		int place = row*80+col;
		int vidseg = 0xb800 + pag * 0x100;
		_asm {
			mov es,vidseg
			mov dx,place
			mov di,dx
			shl di,1
			push dx
			mov dx,03dah
			cli
			mov bx,es:[di]
			mov attr,bx
			sti
			pop dx
		}
	}
  return attr;
}

void far cputsym (int sym, int x, int y, int fc, int bc, int pag) /*------*/
{
	if (pag==2) {   putsym(&sym,&x,&y,&fc,&bc,&pag);   }
	else {
		int row = --y, col = --x;
		int place = row*80+col;
		int vidseg = 0xb800 + pag * 0x100;
		int attr = fc+bc*16;   attr <<= 8;   attr+=sym;
		_asm {
			mov es,vidseg
			mov dx,place
			mov di,dx;
			shl di,1
			push dx
			mov dx,03dah
			cli
			mov bx,attr
			mov es:[di],bx
			sti
			pop dx
		}
	}
}

void far crandomize (float seed)                   /*----------------------*/
{
  unsigned int iseed;
  for( ; seed>3276; seed /= 2) ;
  iseed = floor(seed*10);
  srand(iseed);
}

int far croll(int max)                             /*----------------------*/
{
  int result;
  result = (max>1) ? getrandom(1,max) : 1;
  return result;
}

int far crd(int x, int y)                          /*----------------------*/
{
	float result = y*y + x*x;
	int ires;
	unsigned int ures;
	if (result<65536) {   ures=cint(result);   ires=isqrt(ures);   }
	else {                ires = cint(sqrt(result));   }
	return ires;
}

int far crdsimp(int x, int y)                         /*-------------------*/
{
	int ires;
	int xx=abs(x), yy=abs(y);
	if (xx>yy) {   yy >>= 2;   ires = xx+yy;   }
	else {         xx >>= 2;   ires = xx+yy;   }
	return ires;
}

int far cfinddxdy(int x, int y, int n)             /*----------------------*/
{
  int p = 30*(n+.2);
  int mult = 3*p;
  int rx, ry;
  int dx, dy, res;

  if( mult<100 )   mult = 100;

	rx = getrandom( 1,10*mult );   ry = getrandom( 1,10*mult );

  dx = -sgn(x);   if ( abs(x)==1 && croll(4)==1)  dx = 0;

  dy = -sgn(y);   if ( abs(y)==1 && croll(4)==1)  dy = 0;

  switch( dx ) {
	 case -1:   case 1:
		if( rx<p )            dx = -dx;
		else if( rx<p*10 )    dx = 0;
      break;
    default:
		if( rx<mult*3 )       dx = -1;
		else if( rx<mult*6 )  dx = 1;
      break;
  }

  switch( dy ) {
	 case -1:   case 1:
		if( ry<p )            dy = -dy;
		else if( ry<p*10 )    dy = 0;
      break;
    default:
		if( ry<mult*3 )       dy = -1;
		else if( ry<mult*6 )  dy = 1;
      break;
  }

  res=10*(++dx) + (++dy);
  return res;
}

