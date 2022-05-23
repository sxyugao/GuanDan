#ifndef ROUND_H
#define ROUND_H

#include "hand.h"
#include "transdata.h"

class Round : virtual public TransData
{
public:
    Round();
    Round(int playerId, int lvl);
    Round(QJsonObject data);
    virtual void generateJson() final;
    void update();

    /// 当前牌型
    Hand nowHand;
    /// 当前这轮最后一个出牌的玩家
    int lstPlayedPlayer;
    /// 上一个玩家
    int lstPlayer;
    /// 当前轮到哪个玩家
    int nowPlayer;
    /// 是否是接风
    bool fromFriend;
    /// 级数
    int level;
    /// 每个玩家剩的牌数
    int remains[4];
};

#endif // ROUND_H
