#include <iostream>
#include <math.h>
#include <chrono>
#include <vector>

#include "lense.h"
#include "display.h"
#include "player.h"
#include "world.h"

using namespace std;

int main()
{
    World w;
    Player p;
    const double pi = std::acos ( -1 );
    int xres = 180;
    int yres = 50;
    std::vector<char> screen;
    int buffer = ( xres+1 ) * ( yres );
    screen.reserve ( buffer );

    p.angle = 0;
    p.fov = pi/2;
    double column = p.fov/xres;
    p.depth=15;
    int count = 0;
    auto t1= chrono::system_clock::now();
    auto t2= chrono::system_clock::now();
    int iter = 1;
    double scale = yres;
    double height=4;
    p.xpos=1;//w.xmax/4;
    p.ypos=1;//w.ymax/4;
    std::pair<int,int> coordinates;

    while ( iter ) {
        t2= chrono::system_clock::now();
        chrono::duration<double> elapsedTime = t2-t1;
        t1=t2;
        double delta = elapsedTime.count();
        p.angle+=0.8*delta;
//        iter=0;
        drawHorizon ( screen,xres,yres,'-','*' );
        count=0;
        for ( double deltaAngle = 0; deltaAngle<= ( p.fov ); deltaAngle+=column ) {
            double wallDist = 1;
            bool hitWall = false;
            while ( !hitWall && wallDist<=p.depth ) {
                coordinates = fisheye ( p.xpos,p.ypos,p.angle,p.fov,deltaAngle,wallDist );

                int x = coordinates.first;
                int y = coordinates.second;

                if ( x>=0 && x<w.xmax && y >=0 && y<w.ymax ) {
                    if ( w.map[getIndex ( x, w.xmax, y )]==w.wall ) {
                        hitWall=true;
                        height = scale/wallDist;
                        drawColumn ( screen, xres, yres, count, height, w.wall );
                    }
                } else {
                    hitWall=true;
                    height = scale/wallDist;
                    drawColumn ( screen, xres, yres, count, height, w.wall );
                }
                wallDist+=0.1;
            }
            count++;
        }

        clearScr ( std::cout );
        std::cout<<"angle= "<<p.angle<<" size: "<<screen.capacity() <<"  "<<buffer<<std::endl;

        alignDisplay ( screen,xres,yres );
        display ( screen );

    }

    return 0;
}
