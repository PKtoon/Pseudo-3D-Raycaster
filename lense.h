#ifndef LENSE_H_INCLUDED
#define LENSE_H_INCLUDED

// std::pair<int,int> nonfish2(int xpos, int ypos, double playerAngle, double fov, double deltaAngle, double distance)
// {
//     //still confusion in angles, sort it out
//     double arm, depth;
// 
//     double alpha = abs((fov/2)-deltaAngle); //wrong here
//     double beta = playerAngle - ((fov/2)-deltaAngle);
// 
//     depth = std::tan(alpha) * distance;
//     arm = depth / std::sin(alpha);
// 
//     int x = (std::sin(beta) * arm) + xpos;
//     int y = (std::cos(beta) * arm) + ypos;
// 
//     return std::pair<int,int>(x,y);
// }

std::pair<int,int> nonfisheye(int xpos, int ypos, double playerAngle, double fov, double deltaAngle, double distance)
{
    double alpha = (fov/2)-(fov-deltaAngle);
    
    double absalpha = abs(alpha);
    double depth = (std::tan(absalpha)/std::sin(absalpha))*distance;
    
    double beta = playerAngle + alpha;

    int x = depth * std::sin(beta) + xpos;
    int y = depth * std::cos(beta) + ypos;

    return std::pair<int,int>(x,y);
}

std::pair<int,int> fisheye(int xpos, int ypos, double playerAngle, double fov, double deltaAngle, double distance)
{
    double alpha = (fov/2)-(fov-deltaAngle);
    double beta = playerAngle + alpha;
    
    int x = distance * std::sin(beta) + xpos;
    int y = distance * std::cos(beta) + ypos;
    
    return std::pair<int,int>(x,y);
}

#endif // LENSE_H_INCLUDED
