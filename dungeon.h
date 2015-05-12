const int MAX_SIZE = 12;
const int MIN_SIZE = 2;
const int WIDTH = 200;
const int HEIGHT = 200;
const int ROOM_TRIES = 500;
const int LOOP_FAC = 1000000;

const int DELAY_ROOM = 10;
const int DELAY_PATH = 2;
const int DELAY_CONN = 50;
const int DELAY_DEAD = 2;

void manyRooms(int[][HEIGHT*2+1]);
void room(int[][HEIGHT*2+1],int,int,int,int,int);
void manyPaths(int[][HEIGHT*2+1]);
void path(int[][HEIGHT*2+1],int,int,int);
void connectAll(int[][HEIGHT*2+1]);
int connect(int[][HEIGHT*2+1],int);
void clearOutTheDeads(int[][HEIGHT*2+1]);
void bringOutYourDead(int[][HEIGHT*2+1],int,int);

void printGrid(int[][HEIGHT*2+1]);
void paintFrame(int[][HEIGHT*2+1],int,bool);