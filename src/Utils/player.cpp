#include "player.h"

Player::Player() {}

Player::Player(int id, int level)
{
    this->id = id;
    this->level = level;
}

Player::~Player()
{
    cards.clear();
}

void Player::getCard(Card card)
{
    cards.add(card);
}

void Player::sortCards()
{
    cards.sortByPoint();
}

int Player::maxCardPoint()
{
    for (int i = cards.size() - 1; i >= 0; --i) {
        if (cards[i].suit != Suit::Heart) {
            return cards[i].point;
        }
    }
    return -1;
}

int Player::countRedJokers()
{
    int cnt = 0;
    for (int i = 0; i < cards.size(); ++i) {
        cnt += cards[i].id == 54;
    }
    return cnt;
}

void Player::clear()
{
    cards.clear();
}

void Player::generateJson()
{
    data = cards.toJsonObj();
    data.insert("opt", opt_gameStart);
    data.insert("id", id);
    data.insert("lvl", level);
}
