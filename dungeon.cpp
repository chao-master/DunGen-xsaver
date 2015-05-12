#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <X11/Xlib.h>
#include <unistd.h>
#include "dungeon.h"
#include "vroot.h"

Display *dpy;
	Window root;
	XWindowAttributes wa;
	GC g;
	XColor greyx, greys;
	XColor blackx, blacks;

int main () {

	//Get the display
	dpy = XOpenDisplay (getenv ("DISPLAY"));

	//Get the root window
	root = DefaultRootWindow (dpy);

	//Get Attribute of root window
	XGetWindowAttributes(dpy, root, &wa);

	//Create Graphic Content (GC)
	g = XCreateGC(dpy,root,0,NULL);

	//Set up colour
	XAllocNamedColor(dpy,
                     DefaultColormapOfScreen(DefaultScreenOfDisplay(dpy)),
                     "grey", &greys, &greyx);
	XAllocNamedColor(dpy,
                     DefaultColormapOfScreen(DefaultScreenOfDisplay(dpy)),
                     "black", &blacks, &blackx);

    

    //Start the generation
    while (1){
		int grid[WIDTH*2+1][HEIGHT*2+1] = {0};

		XSetForeground(dpy, g, greys.pixel);
		manyRooms(grid);
		manyPaths(grid);
		connectAll(grid);

		XSetForeground(dpy, g, blacks.pixel);
		clearOutTheDeads(grid);
		usleep(10*1000*1000);
		XClearWindow(dpy, root);
	}
	
}

void paintFrame(int grid[][HEIGHT*2+1],int delay,bool inv){
	int min = wa.height;
	if (wa.width < min){min = wa.width;}
	int scale = min/(WIDTH*2+5);

	int offX = (wa.width-((WIDTH*2+1)*scale))/2;
	int offY = (wa.height-((HEIGHT*2+1)*scale))/2;
	for(int i=0;i<WIDTH*2+1;i++){
		for(int j=0;j<HEIGHT*2+1;j++){
			if (!grid[i][j] xor !inv){
				XFillRectangle (dpy, root, g, offX+i*scale, offY+j*scale, scale, scale);
			}
		}
	}
	XFlush(dpy);
	usleep(delay*1000);
}

void manyRooms(int grid[][HEIGHT*2+1]){
	for(int rN=1;rN<=ROOM_TRIES;rN++){
		int w = random()%(MAX_SIZE-MIN_SIZE)+MIN_SIZE;
		int h = random()%(MAX_SIZE-MIN_SIZE)+MIN_SIZE;
		int x = random()%(WIDTH-w);
		int y = random()%(HEIGHT-h);
		room(grid,x,y,w,h,rN);
	}
}

void room(int grid[][HEIGHT*2+1],int x,int y,int w,int h,int roomNo){
	for(int i=x;i<=x+w;i++){
		for(int j=y;j<=y+h;j++){
			if (grid[i*2+1][j*2+1]){return;}
		}
	}
	for(int i=x*2+1;i<=(x+w)*2+1;i++){
		for(int j=y*2+1;j<=(y+h)*2+1;j++){
			grid[i][j] = roomNo;
		}
	}
	paintFrame(grid,DELAY_ROOM,false);
}

void manyPaths(int grid[][HEIGHT*2+1]){
	int pN = ROOM_TRIES+1;
	for(int x=0;x<WIDTH;x++){
		for(int y=0;y<HEIGHT;y++){
			if (grid[x*2+1][y*2+1] == 0){
				path(grid,x,y,pN);
				pN++;
			}
		}
	}
}

void path(int grid[][HEIGHT*2+1], int x, int y, int pathNo){
	grid[x*2+1][y*2+1] = pathNo;
	while (true){
		int gX=0;
		int gY=0;
		int c=0;

		if (x>0 && !grid[(x-1)*2+1][y*2+1] && (random()%(++c) == 0)){
			gX=-1;
			gY=0;
		}
		if (x<WIDTH-1 && !grid[(x+1)*2+1][y*2+1] && (random()%(++c) == 0)){
			gX = 1;
			gY = 0;
		}
		if (y>0 && !grid[x*2+1][(y-1)*2+1] && (random()%(++c) == 0)){
			gX=0;
			gY=-1;
		}
		if (y<HEIGHT-1 && !grid[x*2+1][(y+1)*2+1] && (random()%(++c) == 0)){
			gX = 0;
			gY = 1;
		}
		if (c==0){return;}
		grid[x*2+1+gX][y*2+1+gY] = pathNo;
		path(grid,x+gX,y+gY,pathNo);
		paintFrame(grid,DELAY_PATH,false);
	}
}

void connectAll(int grid[][HEIGHT*2+1]){
	int n=-1;
	while(n=connect(grid,n)){
		paintFrame(grid,DELAY_CONN,false);
	}
}

int connect(int grid[][HEIGHT*2+1], int last){
	int pX,pY,fr,to;
	int c=0;
	for(int i=1;i<WIDTH*2;i++){
		for(int j=1;j<HEIGHT*2;j++){
			if (i%2==0 && j%2==0) continue;
			if (!grid[i][j]){
				int l = grid[i-1][j];
				int r = grid[i+1][j];
				int u = grid[i][j-1];
				int d = grid[i][j+1];
				int s = l+r+u+d;
				if (s==l||s==r||s==d||s==u) continue;
				if (s/2==last && (random()%LOOP_FAC)==0){
					grid[i][j] = last;
				}

				if (l && (l==r || l==u || l==d)) continue;
				if (r && (r==u || r==d)) continue;
				if (u && u==d) continue;
				if (random()%(++c) == 0){
					pX = i;
					pY = j;
					if (l){
						fr = l;
						to = r+u+d;
					} else if (r){
						fr = r;
						to = u+d;
					} else if (u){
						fr = u;
						to = d;
					}
				}
			}
		}
	}
	if(c==0) {return 0;}
	grid[pX][pY] = to;
	for(int i=1;i<WIDTH*2;i++){
		for(int j=1;j<HEIGHT*2;j++){
			if (grid[i][j] == fr){
				grid[i][j] = to;
			}
		}
	}
	return to;
}

void clearOutTheDeads(int grid[][HEIGHT*2+1]){
	for(int x=1;x<WIDTH*2;x++){
		for(int y=1;y<HEIGHT*2;y++){
			bringOutYourDead(grid,x,y);
		}
	}
}

void bringOutYourDead(int grid[][HEIGHT*2+1], int x, int y){
	if (grid[x][y] == 0) return;
	if (x==0 || y==0 || x==WIDTH-1 || y == HEIGHT-1) return;
	int s = grid[x-1][y]+grid[x+1][y]+grid[x][y-1]+grid[x][y+1];
	//std::cerr << s << "|" << grid[x][y] << std::endl;
	if (s==grid[x][y]){
		grid[x][y] = 0;
		paintFrame(grid,DELAY_DEAD,true);
		bringOutYourDead(grid,x-1,y);
		bringOutYourDead(grid,x+1,y);
		bringOutYourDead(grid,x,y-1);
		bringOutYourDead(grid,x,y+1);
	}
}

void printGrid(int grid[][HEIGHT*2+1]){
	for(int i=0;i<WIDTH*2+1;i++){
		for(int j=0;j<HEIGHT*2+1;j++){
			std::cout << grid[i][j] << ",";
		}
		std::cout << std::endl;
	}
}