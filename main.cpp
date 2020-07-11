#include <SFML/Graphics.hpp>
#include <math.h>
#include <chrono>
#include <iostream>

const double pi = std::acos(-1);

void drawHorizon(sf::RenderWindow &window, int xRes, int yRes);
void drawColumn(sf::RenderWindow& window, int yMax, int xOffset, double height, int hitIndex, int hitBeforeIndex, int prevHitIndex, int prevHitBeforeIndex);
std::pair<double,double> traverse(double playerAngle, double playerFov, double deltaAngle, double length);
//std::tuple<double,double,double> traverse(double playerAngle, double playerFov, double deltaAngle, double length);
int getIndex(int row,int column,int maxColumn);
void castRays(sf::RenderWindow& window, double playerFov, double drawDistance, double playerAngle, char wall, int xRes, int yRes, double xPos, double yPos, std::string map, int maxColumn);
void hitwall(double xPos,double yPos,char wall);
double degreeToRadian(double degree)
{
    return ((pi/180) * degree);
}

int main()
{
    int xRes,yRes;
    xRes = 1200;
    yRes = 900;

    std::string map
    //0123456789012345678901234567890123456789012345678901234567890123456789
    {"######################################################################"
        "#....................................................................#"
        "#....................................................................#"
        "#....................................................................#"
        "#....................................................................#"
        "#....................................................................#"
        "#.................................#..................................#"
        "#....................................................................#"
        "#....................................................................#"
        "#....................................................................#"
        "#....................................................................#"
        "#....................................................................#"
        "#............#.......................................................#"
        "#...........................#........................................#"
        "#....................................................................#"
        "#....................................................................#"
        "#....................................................................#"
        "#....................................................................#"
        "#....................................................................#"
        "#....................................................................#"
        "#....................................................................#"
        "#....................................................................#"
        "#.............#......................................................#"
        "#....................................................................#"
        "######################################################################"
    };//24 rows

    int maxColumn = 70;
    int maxRows = 24;
    char wall = '#';
    double drawDistance = 15;

    double xPos = 3, yPos = 3;
    double playerAngle = 360-45, playerFov = 45;

    auto t1= std::chrono::system_clock::now();
    auto t2= std::chrono::system_clock::now();

    sf::RenderWindow window{sf::VideoMode(xRes,yRes),"Renderer FPS: " + std::to_string(0)};
    window.setVerticalSyncEnabled(true);

    int frame = 0;
    double ps = 0;
    bool pause {false};
    while(window.isOpen())
    {

        t2= std::chrono::system_clock::now();
        std::chrono::duration<double> elapsedTime = t2-t1;
        t1=t2;
        double delta = elapsedTime.count();
        if(ps>=1)
        {
            window.setTitle("Renderer FPS: " + std::to_string(frame));
            ps=0;
            frame = 0;
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Q)
                    window.close();
                if(event.key.code == sf::Keyboard::Up)
                {
//                    std::tuple<double,double,double> coordinates;
//                    coordinates = traverse(playerAngle,playerFov,playerFov/2,0.2);
//                    xPos+=coordinates.first*delta;
//                    yPos+=coordinates.second*delta;
//                    yPos = yPos;
                }
                if(event.key.code == sf::Keyboard::Down)
                {
//                    std::pair<double,double> coordinates;
//                    coordinates = traverse(playerAngle,playerFov,playerFov/2,0.2);
//                    xPos-=coordinates.first*delta;
//                    yPos-=coordinates.second*delta;
//                    yPos = yPos;
                }
                if(event.key.code == sf::Keyboard::Left)
                {
                    playerAngle-=50*delta;
                }
                if(event.key.code == sf::Keyboard::Right)
                {
                    playerAngle+=50*delta;
                }
                if(event.key.code == sf::Keyboard::Space)
                {
                    pause = !pause;
                }
            }
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(playerAngle>=360)
            playerAngle-=360;
        if(xPos>=maxColumn)
            xPos = maxColumn-1;
        if(xPos<=0)
            xPos = 1;
        if(yPos>=maxRows)
            yPos = maxRows-1;
        if(yPos<=0)
            yPos = 1;

        window.clear();
        drawHorizon(window,xRes,yRes);
        castRays(window,playerFov,drawDistance,playerAngle,wall,xRes,yRes,xPos,yPos,map,maxColumn);
        window.display();
        ps+=delta;
        frame++;
        if(!pause){
        playerAngle+=25*delta;
        }
    }
    return 0;
}

void castRays(sf::RenderWindow& window, double playerFov, double drawDistance, double playerAngle, char wall, int xRes, int yRes, double xPos, double yPos, std::string map, int maxColumn)
{
    int column = 0;
    int prevHitIndex = 0;
    int prevHitBeforeIndex = 0;
    int previousIndex = 0;
    int tileIndex = 0;
    double angleAdd = playerFov/xRes;
    std::pair<double,double> coordinates;
    for ( double deltaAngle = 0; deltaAngle<= playerFov; deltaAngle+=angleAdd )
    {
        double wallDist = 0.1;
        while ( wallDist<=drawDistance )
        {
            coordinates = traverse(playerAngle,playerFov,deltaAngle,wallDist);
            double x = xPos + coordinates.first;
            double y = yPos + coordinates.second;
            tileIndex = getIndex(y,x,maxColumn);
            if(wall == map[tileIndex])
            {
                double height = (1-(wallDist/drawDistance))*yRes;
                drawColumn(window,yRes,column,height,tileIndex,previousIndex,prevHitIndex,prevHitBeforeIndex);
                prevHitIndex = tileIndex;
                prevHitBeforeIndex = previousIndex;
                break;
            }
            previousIndex = tileIndex;
            wallDist+=0.1;
        }
        column++;
    }
}

void hitwall(double xPos,double yPos,char wall)
{

}

std::pair<double,double> traverse(double playerAngle, double playerFov, double deltaAngle, double length)
{
    double beta = playerAngle - (playerFov/2) + deltaAngle;
    double alpha = playerAngle - beta;

    alpha = degreeToRadian(alpha);
    beta = degreeToRadian(beta);

    double hyp = (std::tan(alpha)/std::sin(alpha))*length;

    double x = std::cos(beta)*hyp;
    double y = std::sin(beta)*hyp;

    return std::pair<double,double>{x,y};
}

void drawHorizon(sf::RenderWindow &window, int xRes, int yRes)
{
    sf::VertexArray shape {sf::Quads, 4};
    shape[0].color = sf::Color::Cyan;
    shape[1].color = sf::Color::Cyan;
    shape[2].color = sf::Color::Black;
    shape[3].color = sf::Color::Black;
    shape[0].position = sf::Vector2f{0,0};
    shape[1].position = sf::Vector2f(xRes,0);
    shape[2].position = sf::Vector2f(xRes,yRes/2);
    shape[3].position = sf::Vector2f(0,yRes/2);

    window.draw(shape);

    shape[0].color = sf::Color::Black;
    shape[1].color = sf::Color::Black;
    shape[2].color = sf::Color::Yellow;
    shape[3].color = sf::Color::Yellow;
    shape[0].position = sf::Vector2f(0,yRes/2);
    shape[1].position = sf::Vector2f(xRes,yRes/2);
    shape[2].position = sf::Vector2f(xRes,yRes);
    shape[3].position = sf::Vector2f(0,yRes);

    window.draw(shape);
}

void drawColumn(sf::RenderWindow& window, int yMax, int xOffset, double height, int hitIndex, int hitBeforeIndex, int prevHitIndex, int prevHitBeforeIndex)
{
    sf::RectangleShape shape {sf::Vector2f(1,height)};

    double yStart = ( yMax/2 )- ( height/2 ) ;

    double shade = (height/900);

    sf::Color color (25*shade,25*shade,112*shade);
    shape.setFillColor(color);

    if(hitIndex == prevHitIndex && hitBeforeIndex != prevHitBeforeIndex)
        shape.setFillColor(sf::Color::Black);

    shape.setPosition(xOffset,yStart);

    window.draw(shape);
}

int getIndex(int row,int column,int maxColumn)
{
    return ((row * maxColumn)+column);
}
