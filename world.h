#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <string>

class World
{
public:
    World(){}
    std::string map
    //0123456789012345678901234567890
    {"####################"
     "#..................#"
     "#..................#"
     "#..................#"
     "#...........#......#"
     "#..................#"
     "####################"};
    int xmax=20,ymax=6;
    std::pair<int,int> playerPos{100,100};
    char wall='#',floor='.';
};

#endif // WORLD_H
