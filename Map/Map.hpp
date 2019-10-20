#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Layer{
    public:
        std::string name;
        int **matrix;
};

class Point{
    public:
        double x,y;
        Point(double x, double y):x(x),y(y){}
};

class Map{
    public:
        int width, heigth;
        std::vector<Layer *> layers;
        std::vector<Point> enemiesPos, ringPos;
        Map(int width, int heigth);
        sf::FloatRect start, exit;
};

Map::Map(int width, int heigth):width(width),heigth(heigth){

}