//#include "screenhack.h"
#include <X11/Xlib.h>

#define STACK_MAX 256

struct state {
	Display *dpy;
	Window window;
	int xlim, ylim;
	Colormap cmap;

	int grid[];
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

static int mapIn(int x,int y,state *st){
	
}

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
	st->width = get_integer_resource (st->dpy, "width", "Integer");
	st->height = get_integer_resource (st->dpy, "height", "Integer");
	st->roomTries = get_integer_resource (st->dpy, "roomTries", "Integer");
	st->loopFac = get_integer_resource (st->dpy, "loopFac", "Integer");



	int grid[st->width*2+1][st->heigth*2+1] = {0};
	st->grid = grid;
	st->action = 0;

	return st;
}

static unsigned long dungeon_draw(Display *dpy, Window window, void *closure){
	struct state *st = (struct state *) closure;
	_calculate(st);
}

bool _calculate(state *st){
	case (st->action){
		//Reinitilise the dungeon floor
		case 0:
			XClearWindow(dpy,window)
			st->grid = {0};
			st->fillTrace = {0};
			st->fillTrace = 0;
			st->partNo = 1;
			st->action ++;
			st->x = 0;
			st->y = 0;
			return false
		//Placing rooms
		case 1:
			int rW = random()%(st->maxSize-st->minSize)+st->minSize;
			int rh = random()%(st->maxSize-st->minSize)+st->minSize;
			int rX = random()%(st->width-w);
			int rY = random()%(st->height-h);
			bool clear = true;
			for(int i=rX;i<=rX+rW;i++){
				for(int j=rY;j<=y+rH;j++){
					if (grid[i*2+1][j*2+1]){
						clear = false;
						break;
					}
				}
				if (!clear) break;
			}
			if (clear){
				for(int i=rX*2+1;i<=(rX+rW)*2+1;i++){
					for(int j=rY*2+1;j<=(rY+rH)*2+1;j++){
						grid[i][j] = st->partNo;
					}
				}
			}
			st->partNo++;
			if(st->partNo > st->roomTries) st->action ++;
			return clear
		//Locate somewhere we can start a coridoor
		case 2:
			for( ; st->x < st->width; st->x++){
				for( ; st->y < st->height; st->y++){
					st->lastDir = -1;
					st->state = 3;
					st->fillTraceX[0] = st->x;
					st->fillTraceY[0] = st->y;
					st->fillTraceTop = 0;
					st->partNo++;
					grid[st->x][st->y] = partNo;
					return true;
				}
				st->y = 0;
			}
			st->state = 4;
			return false;
		//Moving along the coridoor
		case 3:
			int x = st->fillTraceX[st->fillTraceTop];
			int y = st->fillTraceY[st->fillTraceTop];

			int gX=0;
			int gY=0;
			int c=0;

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
					st->state = 2;
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

		//Remove deadends
		case 5:

		//Just look at how nice it is
		case 6:
	}
}