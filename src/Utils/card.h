#ifndef CARD_H
#define CARD_H

#include "transdata.h"

#include <QString>

/// 卡牌花色 0: 草花，1: 黑桃，2: 红桃，3: 方片，4: 大小王
enum Suit { Club, Spade, Heart, Diamond, Joker };

class Card : public TransData
{
public:
    Card();
    Card(int id, int level);
    Card(int id, bool isLevel);
    virtual void generateJson() final;
    bool operator<(const Card &other) const;
    bool operator==(const Card &ohter) const;
    bool operator!=(const Card &ohter) const;

    /// 卡牌编号
    int id = -1;
    /// 卡牌类型
    int figure;
    /// 卡牌花色
    Suit suit;
    /// 卡牌点数
    int point;
    /// 原始点数
    int oriPoint;
    /// 是否是级牌
    bool isLevelCard = false;
};

#endif // CARD_H
