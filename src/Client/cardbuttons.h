#ifndef CARDBUTTONS_H
#define CARDBUTTONS_H

#include "cardbutton.h"

#include <QList>

class CardButtons
{
public:
    CardButtons();
    void add(CardButton *card);
    void remove(Card card);
    void removeAt(int pos);
    CardButton *&operator[](int pos);
    int size();
    void clear();
    void sort();

private:
    QList<CardButton *> list;
};

#endif // CARDBUTTONS_H
