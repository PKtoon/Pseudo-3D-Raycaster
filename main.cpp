#include <iostream>
#include <math.h>
#include <chrono>
#include <vector>

#include "display.h"
#include "player.h"
#include "world.h"
#include "raycast.h"

#include "sdl2_wrap.h"

using namespace std;
using namespace SDL2Wrap;

int main()
{
    SDL2Wrap::SDL sdl{SDL_INIT_VIDEO,0};
    World w;
    Player p;
    const double pi = std::acos ( -1 );
//     int xres = 220;
//     int yres = 55;
    int xres = 640;
    int yres = 480;
    std::vector<char> screen;
    int buffer = ( xres+1 ) * ( yres );
    screen.reserve ( buffer );

    p.angle = 0+pi/2+pi/3;
    p.fov = pi/3;
    p.depth=8;
    auto t1= chrono::system_clock::now();
    auto t2= chrono::system_clock::now();
    int iter = 1;
    double scale = yres;

    p.xpos=w.xmax/4;
    p.ypos=w.ymax/4;
    
    Window window {"3D demo", 10,10,xres,yres,SDL_WINDOW_SHOWN};
    Renderer renderer {window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC};
    SDL_Event e;
    bool quit{false};
    while(!quit)
    {
        t2= chrono::system_clock::now();
        chrono::duration<double> elapsedTime = t2-t1;
        t1=t2;
        double delta = elapsedTime.count();
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }
            if(e.type == SDL_KEYDOWN)
            {
                switch(e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_UP:
                        p.xpos+=1;
                        break;
                    case SDLK_DOWN:
                        p.xpos-=1;
                    case SDLK_LEFT:
                        p.angle-=1*delta;
                        break;
                    case SDLK_RIGHT:
                        p.angle+=1*delta;
                        break;
                    default:
                        break;
                }
            }
        }
//         p.angle+=0.5*delta;
        drawHorizon(renderer,xres,yres);
        castRays(renderer,p,w,xres,yres,scale);
        
        renderer.present();
        renderer.clear();
    }
//     while ( iter ) 
//     {
//         t2= chrono::system_clock::now();
//         chrono::duration<double> elapsedTime = t2-t1;
//         t1=t2;
//         double delta = elapsedTime.count();
//         p.angle+=0.2*delta;
//        iter=0;
//         drawHorizon ( screen,xres,yres,'-',' ' );
//         castRays(p,w,xres,yres,screen,scale);
//         clearScr ( std::cout );
//         std::cout<<"angle= "<<p.angle<<" size: "<<screen.capacity() <<"  "<<buffer<<std::endl;
//         alignDisplay ( screen,xres,yres );
//         display ( screen );
//     }
    return 0;
}
