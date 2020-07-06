#include <SFML/Graphics.hpp>
#include <math.h>
#include <chrono>

const double pi = std::acos(-1);

void drawHorizon(sf::RenderWindow &window, int xRes, int yRes);
void drawColumn(sf::RenderWindow& window, int yMax, int xOffset, int height, int hitIndex, int hitBeforeIndex, int prevHitIndex, int prevHitBeforeIndex);
std::pair<double,double> traverse(double playerAngle, double playerFov, double deltaAngle, double length);
int getIndex(int row,int column,int maxColumn);
void castRays(sf::RenderWindow& window, double playerFov, double drawDistance, double playerAngle, char wall, int xRes, int yRes, double scale, double xPos, double yPos, std::string map, int maxColumn);
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
    double drawDistance = 80;
    double scale = yRes;

    int xPos = 3, yPos = 21;
    double playerAngle = 360-45, playerFov = 90;

    auto t1= std::chrono::system_clock::now();
    auto t2= std::chrono::system_clock::now();

    sf::RenderWindow window{sf::VideoMode(xRes,yRes),"Game"};

    while(window.isOpen())
    {

        t2= std::chrono::system_clock::now();
        std::chrono::duration<double> elapsedTime = t2-t1;
        t1=t2;
        double delta = elapsedTime.count();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Q)
                    window.close();
                if(event.key.code == sf::Keyboard::Up)
                {
                    std::pair<double,double> coordinates;
                    coordinates = traverse(playerAngle,playerFov,playerFov/2,0.2);
                    xPos+=coordinates.first*delta;
                    yPos+=coordinates.second*delta;
                    yPos = yPos;
                }
                if(event.key.code == sf::Keyboard::Down)
                {
                    std::pair<double,double> coordinates;
                    coordinates = traverse(playerAngle,playerFov,playerFov/2,0.2);
                    xPos-=coordinates.first*delta;
                    yPos-=coordinates.second*delta;
                    yPos = yPos;
                }
                if(event.key.code == sf::Keyboard::Left)
                {
                    playerAngle-=50*delta;
                }
                if(event.key.code == sf::Keyboard::Right)
                {
                    playerAngle+=50*delta;
                }
            }
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(playerAngle>359)
            playerAngle-=360;
        if(xPos>maxColumn)
            xPos = maxColumn-1;
        if(xPos<0)
            xPos = 1;
        if(yPos>maxRows)
            yPos = maxRows-1;
        if(yPos<0)
            yPos = 1;

        window.clear();
        drawHorizon(window,xRes,yRes);
        castRays(window,playerFov,drawDistance,playerAngle,wall,xRes,yRes,scale,xPos,yPos,map,maxColumn);
        window.display();
        playerAngle+=25*delta;
    }
    return 0;
}

void castRays(sf::RenderWindow& window, double playerFov, double drawDistance, double playerAngle, char wall, int xRes, int yRes, double scale, double xPos, double yPos, std::string map, int maxColumn)
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
            double x = xPos + coordinates.first;;
            double y = yPos + coordinates.second;
            tileIndex = getIndex(y,x,maxColumn);
            if(wall == map[tileIndex])
            {
                int height = 4*scale/wallDist;
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
//    double alpha = std::abs(playerAngle - beta);
     double alpha = playerAngle - beta;
//    while(beta>90)
    {
//        beta-=90;
    }

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
//    sf::RectangleShape shape {sf::Vector2f(xRes,yRes/2)};
//    shape.setPosition(0,0);
//    shape.setFillColor(sf::Color::Cyan);
    window.draw(shape);

    shape[0].color = sf::Color::Black;
    shape[1].color = sf::Color::Black;
    shape[2].color = sf::Color::Yellow;
    shape[3].color = sf::Color::Yellow;
    shape[0].position = sf::Vector2f(0,yRes/2);
    shape[1].position = sf::Vector2f(xRes,yRes/2);
    shape[2].position = sf::Vector2f(xRes,yRes);
    shape[3].position = sf::Vector2f(0,yRes);
//    shape.setPosition(0,yRes/2);
//    shape.setFillColor(sf::Color::Yellow);
    window.draw(shape);
}

void drawColumn(sf::RenderWindow& window, int yMax, int xOffset, int height, int hitIndex, int hitBeforeIndex, int prevHitIndex, int prevHitBeforeIndex)
{
    sf::RectangleShape shape {sf::Vector2f(1,height-1)};
    sf::RectangleShape shape2 {sf::Vector2f(1,1)};
    sf::RectangleShape shape3 {sf::Vector2f(1,2)};

    int yStart = ( yMax/2 )- ( height/2 ) + 1;
    int yEnd = ( yMax/2 )+ ( height/2 ) - 1;

    shape2.setPosition(xOffset,yStart-1);
    shape2.setFillColor(sf::Color::Black);
//    window.draw(shape2);

    shape3.setPosition(xOffset,yEnd+1);
    shape3.setFillColor(sf::Color::Black);
//    window.draw(shape3);

    sf::Color color (0,height*0.41,0);
//    sf::Color color (0,255,0);
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
