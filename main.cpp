#include <iostream>
#include <math.h>
#include <chrono>
#include <vector>

#include "display.h"
#include "player.h"
#include "world.h"
#include "raycast.h"

using namespace std;

int main()
{
    World w;
    Player p;
    const double pi = std::acos ( -1 );
    int xres = 220;
    int yres = 55;
    std::vector<char> screen;
    int buffer = ( xres+1 ) * ( yres );
    screen.reserve ( buffer );

    p.angle = 0+pi/2+pi/3;
    p.fov = pi/2;
    p.depth=15;
    auto t1= chrono::system_clock::now();
    auto t2= chrono::system_clock::now();
    int iter = 1;
    double scale = yres;

    p.xpos=w.xmax/4;
    p.ypos=w.ymax/4;

    while ( iter ) 
    {
        t2= chrono::system_clock::now();
        chrono::duration<double> elapsedTime = t2-t1;
        t1=t2;
        double delta = elapsedTime.count();
        p.angle+=0.2*delta;
//        iter=0;
        drawHorizon ( screen,xres,yres,'-',' ' );
        castRays(p,w,xres,yres,screen,scale);
        clearScr ( std::cout );
        std::cout<<"angle= "<<p.angle<<" size: "<<screen.capacity() <<"  "<<buffer<<std::endl;
        alignDisplay ( screen,xres,yres );
        display ( screen );
    }
    return 0;
}
