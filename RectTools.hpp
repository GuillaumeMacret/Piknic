#include <SFML/Graphics.hpp>

sf::FloatRect *getContainingRect(sf::FloatRect r1, sf::FloatRect r2){
    sf::FloatRect *leftBox, *rightBox, *topBox, *botBox;
    if(r1.left < r2.left){
         leftBox = &r1;
         rightBox = &r2;
    }else{
        leftBox = &r2;
        rightBox = &r1;
    }

    float left = leftBox->left;
    float width = (rightBox->left+rightBox->width) - leftBox->left;

    if(r1.top < r2.top){
        topBox = &r1;
        botBox = &r2;
    }else{
        topBox = &r1;
        botBox = &r2;
    }

    float top = topBox -> top;
    float height = (botBox->top + botBox -> height) - topBox->top;

    //sf::FloatRect *rect = new sf::FloatRect(leftBox->left,leftBox->top,(rightBox->left+rightBox->width) - leftBox->left, leftBox->top -rightBox->top+rightBox->height);
    sf::FloatRect *rect = new sf::FloatRect(left,top,width,height);

    return rect;
}