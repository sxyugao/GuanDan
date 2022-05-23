#include "card.h"

Card::Card() {}

Card::Card(int id, int level)
{
    this->id = id;
    if (id > 52) { // 大小王
        this->figure = this->oriPoint = 16 + (id == 54);
        this->point = this->oriPoint;
        this->suit = Joker;
    } else {
        this->figure = (id - 1) / 4 + 1;
        this->suit = Suit(id % 4);
        if (id < 5) { // A
            this->oriPoint = 14;
        } else {
            this->oriPoint = this->figure;
        }
        this->point = this->oriPoint;
        if (this->oriPoint == level) { // 是级牌
            this->point = 15;
            this->isLevelCard = true;
        }
    }
}

Card::Card(int id, bool isLevel)
{
    this->id = id;
    if (id > 52) { // 大小王
        this->figure = this->oriPoint = 16 + (id == 54);
        this->point = this->oriPoint;
        this->suit = Joker;
    } else {
        this->figure = (id - 1) / 4 + 1;
        this->suit = Suit(id % 4);
        if (id < 5) { // A
            this->oriPoint = 14;
        } else {
            this->oriPoint = this->figure;
        }
        this->point = this->oriPoint;
        this->isLevelCard = isLevel;
        if (isLevel) {
            this->point = 15;
        }
    }
}

bool Card::operator<(const Card &other) const
{
    return this->point < other.point;
}

bool Card::operator==(const Card &other) const
{
    return this->id == other.id;
}

bool Card::operator!=(const Card &other) const
{
    return this->id != other.id;
}

void Card::generateJson()
{
    data.insert("id", id);
    data.insert("isLevelCard", isLevelCard);
}
