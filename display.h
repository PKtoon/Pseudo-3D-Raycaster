#pragma once

#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

int getIndex ( int x_offset, int x_max, int y_offset )
{
    return ( y_offset*x_max )+x_offset;
}

void clearScr ( std::ostream& out )
{
    out<<"\033[2J\033[1;1H";
}

void display ( const std::vector<char>& screen )
{
    for ( int i = 0; i<screen.capacity(); i++ )
        std::cout<<screen[i];
}

void alignDisplay ( std::vector<char>& screen, int x_max, int y_max )
{
    for ( int y = 0; y<=y_max; y++ )
        screen[getIndex ( x_max-1, x_max, y )]='\n';
}

void drawHorizon ( std::vector<char>& screen, int x_max, int y_max, char sky, char floor )
{
    int y;
    for ( y = 0; y < y_max/2; y++ )
        for ( int x = 0; x < x_max; x++ )
            screen[getIndex ( x,x_max,y )]=sky;
    for ( ; y < y_max; y++ )
        for ( int x = 0; x < x_max; x++ )
            screen[getIndex ( x,x_max,y )]=floor;
}

void drawColumn ( std::vector<char>& screen,int x_max, int y_max, int x_offset, int height, char wall )
{
    for ( int y = ( y_max/2 )- ( height/2 ); y< ( y_max/2 )+ ( height/2 ); y++ )
        screen[getIndex ( x_offset, x_max, y )]=wall;
}

#endif // DISPLAY_H_INCLUDED
