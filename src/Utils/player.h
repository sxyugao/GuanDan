#ifndef PLAYER_H
#define PLAYER_H

#include "cards.h"
#include "transdata.h"

class Player : virtual public TransData
{
public:
    Player();
    Player(int id, int level);
    ~Player();
    virtual void generateJson() final;
    void getCard(Card card);
    void sortCards();
    void clear();
    int maxCardPoint();
    int countRedJokers();

    Cards cards;
    int id = -1;
    int level;
};

#endif // PLAYER_H
