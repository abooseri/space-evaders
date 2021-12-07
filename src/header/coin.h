
#ifndef COIN_H
#define COIN_H

class coin{
public:
    coin(int x, int y);
    coin();
    void showCoin();
    bool getCollected();
    int getX();
    int getY();
    void setCollected(bool collect);
    void isCollected();
private:
    int posX;
    int posY;
    bool collected;
};

#endif //COIN_H