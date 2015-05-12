#include "screenhack.h"
//#include <X11/Xlib.h>

#define STACK_MAX 256

typedef enum { false, true } bool;

struct state {
	Display *dpy;
	Window window;
	int xlim, ylim;
	Colormap cmap;

	int grid[200][200];
	int x,y;
	int action;

	int maxSize;
	int minSize;
	int width;
	int height; 
	int roomTries;
	int loopFac;

	int fillTraceX[STACK_MAX];
	int fillTraceY[STACK_MAX];
	int fillTraceTop;

	int partNo;	
};
static bool _calculate(struct state *st);

static void * dungeon_init(Display *dpy, Window window){
	struct state *st = (struct state *) calloc (1,sizeof(*st));
	XGCValues gcv;
	XWindowAttributes xgwa;
	st->dpy = dpy;
	st->window = window;

	XGetWindowAttributes (st->dpy, st->window, &xgwa);
	st->xlim = xgwa.width;
	st->ylim = xgwa.height;
	st->cmap = xgwa.colormap;


	st->maxSize = get_integer_resource (st->dpy, "maxSize", "Integer");
	st->minSize = get_integer_resource (st->dpy, "minSize", "Integer");
	st->width = 200;//get_integer_resource (st->dpy, "width", "Integer");
	st->height = 200;//get_integer_resource (st->dpy, "height", "Integer");
	st->roomTries = get_integer_resource (st->dpy, "roomTries", "Integer");
	st->loopFac = get_integer_resource (st->dpy, "loopFac", "Integer");

	return st;
}

static unsigned long dungeon_draw(Display *dpy, Window window, void *closure){
	struct state *st = (struct state *) closure;
	_calculate(st);
}

static bool _calculate(struct state *st){
	switch (st->action){
		int rW,rH,rX,rY,x,y,gX,gY,c;
		bool clear;

		//Reinitilise the dungeon floor
		case 0:
			st->fillTraceTop = 0;
			st->partNo = 1;
			st->action ++;
			st->x = 0;
			st->y = 0;
			return false;
		//Placing rooms
		case 1:
			rW = random()%(st->maxSize-st->minSize) + st->minSize;
			rH = random()%(st->maxSize-st->minSize) + st->minSize;
			rX = random()%(st->width-rW);
			rY = random()%(st->height-rH);
			clear = true;
			int i,j;
			for(i=rX;i<=rX+rW;i++){
				for(j=rY;j<=y+rH;j++){
					if (st->grid[i*2+1][j*2+1]){
						clear = false;
						break;
					}
				}
				if (!clear) break;
			}
			if (clear){
				for(i=rX*2+1;i<=(rX+rW)*2+1;i++){
					for(j=rY*2+1;j<=(rY+rH)*2+1;j++){
						st->grid[i][j] = st->partNo;
					}
				}
			}
			st->partNo++;
			if(st->partNo > st->roomTries) st->action ++;
			return clear;
		//Locate somewhere we can start a coridoor
		case 2:
			for( ; st->x < st->width; st->x++){
				for( ; st->y < st->height; st->y++){
					st->action = 3;
					st->fillTraceX[0] = st->x;
					st->fillTraceY[0] = st->y;
					st->fillTraceTop = 0;
					st->partNo++;
					st->grid[st->x][st->y] = st->partNo;
					return true;
				}
				st->y = 0;
			}
			st->action = 4;
			return false;
		//Moving along the coridoor
		case 3:
			x = st->fillTraceX[st->fillTraceTop];
			y = st->fillTraceY[st->fillTraceTop];

			gX=0;
			gY=0;
			c=0;

			if (st->fillTraceTop < STACK_MAX-1){
				if (x > 0 && !st->grid[(x-1)*2+1][y*2+1] && (random()%(++c) == 0)){
					gX=-1;
					gY=0;
				}
				if (x < st->width-1 && !st->grid[(x+1)*2+1][y*2+1] && (random()%(++c) == 0)){
					gX = 1;
					gY = 0;
				}
				if (y > 0 && !st->grid[x*2+1][(y-1)*2+1] && (random()%(++c) == 0)){
					gX=0;
					gY=-1;
				}
				if (y < st->height-1 && !st->grid[x*2+1][(y+1)*2+1] && (random()%(++c) == 0)){
					gX = 0;
					gY = 1;
				}
			}
			if (c==0){ //No where to go, backtrack
				st->fillTraceTop--;
				if (st->fillTraceTop<0){
					st->action = 2;
				}
				return false;
			}
			st->grid[x*2+1+gX][y*2+1+gY] = st->partNo;
			st->grid[x*2+1+gX+gX][y*2+1+gY+gY] = st->partNo;
			st->fillTraceTop++;
			st->fillTraceX[st->fillTraceTop] = x+gX;
			st->fillTraceY[st->fillTraceTop] = y+gY;
			return true;
		//Merge things?
		case 4:
		break;
		//Remove deadends
		case 5:
		break;
		//Just look at how nice it is
		case 6:
		break;
	}
}




static void dungeon_reshape (Display *dpy, Window window, void *closure, unsigned int w, unsigned int h){}
static Bool dungeon_event (Display *dpy, Window window, void *closure, XEvent *event){return False;}
static void dungeon_free (Display *dpy, Window window, void *closure){}

static const char *dungeon_defaults [] = {
  ".background:	black",
  ".foreground:	white",
  "*fpsSolid:	true",
  "*delay:	10000",
  "*grey:	false",
  0
};

static XrmOptionDescRec dungeon_options [] = {
  { "-delay",		".delay",	XrmoptionSepArg, 0 },
  { "-grey",		".grey",	XrmoptionNoArg, "True" },
  { 0, 0, 0, 0 }
};



XSCREENSAVER_MODULE ("dungeon", dungeon)