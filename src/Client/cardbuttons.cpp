#include "cardbuttons.h"

CardButtons::CardButtons() {}

void CardButtons::add(CardButton *card)
{
    list.append(card);
}

void CardButtons::remove(Card card)
{
    for (int i = 0; i < list.size(); ++i) {
        if (card == list[i]->card) {
            delete list[i];
            list.removeAt(i);
            break;
        }
    }
}

void CardButtons::removeAt(int pos)
{
    delete list[pos];
    list.removeAt(pos);
}

CardButton *&CardButtons::operator[](int pos)
{
    return list[pos];
}

int CardButtons::size()
{
    return list.size();
}

void CardButtons::clear()
{
    for (auto p : list) {
        delete p;
    }
    list.clear();
}

bool cmp(const CardButton *a, const CardButton *b)
{
    return !(a->card < b->card);
}

void CardButtons::sort()
{
    std::sort(list.begin(), list.end(), cmp);
}
