#ifndef CARDS_H
#define CARDS_H

#include "card.h"
#include "transdata.h"

#include <algorithm>
#include <QList>

class Cards : virtual public TransData
{
public:
    Cards();
    Cards(QJsonObject data);
    virtual void generateJson();
    void add(Card card);
    void remove(Card card);
    void sortByPoint();
    void sortByFigure();
    Card &operator[](int pos);
    int size();
    void clear();

    QList<Card> cards;
};

#endif // CARDS_H
