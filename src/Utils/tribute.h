#ifndef TRIBUTE_H
#define TRIBUTE_H

#include "card.h"
#include "transdata.h"

class Tribute : public virtual TransData
{
public:
    Tribute();
    Tribute(Opt opt, Card card);
    Tribute(QJsonObject data);
    virtual void generateJson() final;

    Opt opt = opt_antiTribute;
    Card card;
    int fromPlayer = -1, toPlayer = -1;
};

#endif // TRIBUTE_H
