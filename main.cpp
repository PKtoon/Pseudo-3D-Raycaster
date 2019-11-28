#include <iostream>
#include <math.h>
#include <chrono>
#include <vector>

#include "Player.h"
#include "world.h"

using namespace std;

int main()
{
    World w;
    Player p;
    const double pi = std::acos(-1);
    int xres = 180;
    int yres = 50;
    std::vector<char> screen;
    int buffer = (xres+1)*(yres);
    screen.reserve(buffer);
    std::vector<std::pair<int,int>> rays;
    p.angle = 0;
    p.fov = pi;
    double column = p.fov/xres;
    p.depth=15;
    int count = 0;
    auto t1= chrono::system_clock::now();
    auto t2= chrono::system_clock::now();
    int iter = 1;
    while(iter)
    {
        t2= chrono::system_clock::now();
        chrono::duration<double> elapsedTime = t2-t1;
        t1=t2;
        double delta = elapsedTime.count();
        p.angle+=0.8*delta;
//        iter=0;
        for(int i = 0; i<screen.capacity();i++)
            screen[i]='.';
        count=0;
        for(double i = 0; i<=(p.fov); i+=column)
        {
            double wallDist = 0.1;
            bool hitWall = false;
            while (!hitWall && wallDist<=p.depth)
            {
                double arm, depth;
                double alpha = abs((p.fov/2)-i); //wrong here
                double beta = p.angle - ((p.fov/2)-i);
                depth = std::tan(alpha) * wallDist;
                arm = depth / std::sin(alpha);
                int x = (std::sin(beta) * arm) + p.xpos;
                int y = (std::cos(beta) * arm) + p.ypos;

                if(x>=0 && x<w.xmax && y >=0 && y<w.ymax)
                {
                    if(w.map[(y*w.xmax)+x]==w.wall)
                    {
                        //                        std::cout<<"x: "<<x<<" y: "<<y<<" alpha: "<<count<<"\n";
                        rays.push_back(std::pair<int,int>(x,y));
                        hitWall=true;
                        for(int j = yres/4; j<3*yres/4;j++)
                            screen[j*xres+count]='#';
                    }
                }
                wallDist+=0.1;

            }
            count++;
        }
        for(int i = 0; i<=yres; i++)
            screen[i*xres+xres]='\n';
        int i = 0;
        std::cout<<"\033[2J\033[1;1H";
        while( i < buffer)
            std::cout<<screen[i++];

    }
    std::cout<<"size: "<<rays.size()<<std::endl<<std::endl;

    for(int i = 0; i< rays.size(); i++)
        std::cout<<"x: "<<rays[i].first<<"  y: "<<rays[i].second<<" tile: "<<(rays[i].second)*w.xmax+rays[i].first<<"\n";

    return 0;
}

//void ray()
//{
//    t2= chrono::system_clock::now();
//    chrono::duration<double> elapsedTime = t2-t1;
//    t1=t2;
//    double delta = elapsedTime.count();
//    p.angle+=0.8*delta;
////         iter=0;
//    for(double i = p.angle-(p.fov/2); i<=p.angle+(p.fov/2); i+=column)
//    {
//        double ux = std::sin(i);
//        double uy = std::cos(i);
//        double wallDist=0.1;
//        bool wallHit = false;
//        while(wallDist<=p.depth && !wallHit)
//        {
//            int x = (ux*wallDist)+p.xpos;
//            int y = (uy*wallDist)+p.ypos;
//            if(x<w.xmax && x>-1 && y < w.ymax && y > -1)
//            {

//                if(w.map[((y)*w.xmax+x)]==w.wall && wallDist<p.depth)
//                {
//                    //                    std::cout<<"angle: "<<i<<" x: "<<x<<" y: "<<y<<" tile: "<<(y)*w.xmax+x<<" dist: "<<wallDist<<std::endl;
//                    rays.push_back(std::pair<int,int>(x,y));
//                    int size = p.depth;
//                    wallHit=true;
//                    size-=(wallDist);
//                    if(size>yres/2)
//                        size=yres/2-1;
//                    for(int a = 0;a<=(yres/2)-size;a++)
//                        screen[a*xres+count]='.';
//                    for(int a = (yres/2)-size;a<=(yres/2)+size;a++)
//                        screen[a*xres+count]='#';
//                    for(int a = (yres/2)+size;a<yres;a++)
//                        screen[a*xres+count]='-';
//                }
//            }
//            wallDist+=0.1;
//        }
//        if(!wallHit)
//        {
//            for(int a = 0;a<=(yres/2);a++)
//                screen[a*xres+count]='.';

//            for(int a = (yres/2);a<yres;a++)
//                screen[a*xres+count]='-';
//        }
//        count++;
//    }
//    count = 0;
//    for(int i = 0; i<=yres; i++)
//        screen[i*xres+xres]='\n';
//    int i = 0;
//   std::cout<<"\033[2J\033[1;1H";
//    while( i < screen.capacity())
//        std::cout<<screen[i++];

//}
