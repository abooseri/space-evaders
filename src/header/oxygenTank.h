//
// Created by tommy on 2021-12-01.
//

#ifndef SPACE_EVADERS_OXYGENTANK_H
#define SPACE_EVADERS_OXYGENTANK_H

class tank{
public:
    tank(int x, int y);
    tank();
    void showTank();
    bool getCollected();
    int getX();
    int getY();
    void setCollected(bool collect);
    void isCollected();
    Rectangle GetRecBounds();
private:
    float width;
    float height;
    int posX;
    int posY;
    bool collected;
};

#endif //SPACE_EVADERS_OXYGENTANK_H
