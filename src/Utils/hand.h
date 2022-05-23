#ifndef HAND_H
#define HAND_H

#include "cards.h"

/// 牌型
/// 0: 非法
/// 1: 单张
/// 2: 对子
/// 3: 三连对
/// 4: 三同张
/// 5: 二连三
/// 6: 三带二
/// 7: 顺子
/// 8: 四张炸弹
/// 9: 五张炸弹
/// 10: 同花顺
/// 11: 六张炸弹
/// 12: 七张炸弹
/// 13: 八张炸弹
/// 14: 天王炸
enum HandType {
    illegal,
    singleCard,
    onePair,
    threePairs,
    threeOfAKind,
    twoThreeOfAKind,
    threeOfAKindWithOnePair,
    straight,
    fourBomb,
    fiveBomb,
    straightFlush,
    sixBomb,
    sevenBomb,
    eightBomb,
    fourJokers
};

class Hand : public Cards
{
public:
    Hand();
    Hand(Cards cards);
    void reset();
    bool isBigger(const Hand &other) const;

    /// 牌型
    HandType type = illegal;

private:
    void preHandle();

    /// 优先级
    int point = -1;
};

#endif // HAND_H
