#ifndef RAYCAST_H_INCLUDED
#define RAYCAST_H_INCLUDED

#include "display.h"
#include "lense.h"

bool hitWall(int x, int y, const World& w)
{
    if ( x>=0 && x<w.xmax && y >=0 && y<w.ymax )
    {
        if ( w.map[getIndex ( x, w.xmax, y )]!=w.wall )
        {
            return false;
        }
    }
    return true;
}

void castRays(const Player& p, const World& w, int x_max, int y_max, std::vector<char>& screen, double scale)
{
    int previous = -1;
    int column = 0;
    double angleAdd = p.fov/x_max;
    std::pair<int,int> coordinates;
    for ( double deltaAngle = 0; deltaAngle<= p.fov; deltaAngle+=angleAdd ) 
    {
        double wallDist = 1;
        while ( wallDist<=p.depth ) 
        {
            coordinates = nonfisheye ( p.xpos,p.ypos,p.angle,p.fov,deltaAngle,wallDist );

            int x = coordinates.first;
            int y = coordinates.second;

            if( hitWall(x,y,w) )
            {
                int height = scale/wallDist;
                char wall=w.wall;
                int index = getIndex( x, x_max, y );
                if(previous != index )
                {
                    previous=index;
                    wall = '_';
                }
                drawColumn ( screen, x_max, y_max, column, height, wall );
                break;
            }
            wallDist+=0.1;
        }
        column++;
    }
}

void castRays(SDL2Wrap::Renderer& renderer, const Player& p, const World& w, int x_max, int y_max, double scale)
{
    int previous = -1;
    int column = 0;
    double angleAdd = p.fov/x_max;
    std::pair<int,int> coordinates;
    for ( double deltaAngle = 0; deltaAngle<= p.fov; deltaAngle+=angleAdd ) 
    {
        double wallDist = 1;
        while ( wallDist<=p.depth ) 
        {
            coordinates = nonfisheye ( p.xpos,p.ypos,p.angle,p.fov,deltaAngle,wallDist );

            int x = coordinates.first;
            int y = coordinates.second;

            if( hitWall(x,y,w) )
            {
                int height = scale/wallDist;
                char wall=w.wall;
                int index = getIndex( x, x_max, y );
                if(previous != index )
                {
                    previous=index;
                    wall = '_';
                    drawNewColumn(renderer,y_max,column,height);
                }
                else
                drawColumn(renderer,y_max,column,height);
                break;
            }
            wallDist+=0.1;
        }
        column++;
    }
}
#endif // RAYCAST_H_INCLUDED
