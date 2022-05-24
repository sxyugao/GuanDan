#include "hand.h"

Hand::Hand() {}

void Hand::preHandle()
{
    // 按照类型排序而不是牌点
    sortByFigure();

    if (cards.size() == 4 && cards[0].suit == Joker) { // 天王炸
        this->type = fourJokers;
        this->point = 999;
        return;
    }

    // 预处理牌的种类、数量以及花色，cards 中是有序的，因此 figureList 也是有序的
    bool sameSuit = 1;
    int cnt[20], pri[20], oriPri[20];
    memset(cnt, 0, sizeof(cnt));
    memset(pri, 0, sizeof(pri));
    memset(oriPri, 0, sizeof(oriPri));
    QList<int> figureList;
    for (auto &&card : cards) {
        cnt[card.figure]++;
        if (cnt[card.figure] == 1) {
            figureList.append(card.figure);
            pri[card.figure] = card.point;
            oriPri[card.figure] = card.oriPoint;
        }
        sameSuit &= card.suit == cards[0].suit;
    }

    if (figureList.size() == 1) { // 只有一种牌
        this->point = cards[0].point;
        int num = cnt[figureList[0]];
        if (num == 1) { //单牌
            this->type = singleCard;
        } else if (num == 2) { //对子
            this->type = onePair;
        } else if (num == 3) { // 三同张
            this->type = threeOfAKind;
        } else { // 炸弹
            if (num > 5) {
                this->type = HandType(sixBomb + num - 6);
            } else {
                this->type = HandType(fourBomb + num - 4);
            }
        }
        return;
    }

    for (auto &&card : cards) { // 其他牌型都没有王了
        if (card.suit == Joker) {
            return;
        }
    }

    if (figureList.size() == 2) { // 有两种牌
        int t1 = figureList[0], t2 = figureList[1];
        if (cnt[t1] == 3 && cnt[t2] == 3) { // 二连三
            if (t2 - t1 == 1) {             // 直接相邻
                this->type = twoThreeOfAKind;
                this->point = pri[t2];
            } else if (t2 - t1 == 12 && t1 == 1) { // A K
                this->type = twoThreeOfAKind;
                this->point = pri[t1];
            }
        } else if (cnt[t1] == 3 && cnt[t2] == 2) { // 三带二
            this->type = threeOfAKindWithOnePair;
            this->point = pri[t1];
        } else if (cnt[t1] == 2 && cnt[t2] == 3) { // 另一种三带二
            this->type = threeOfAKindWithOnePair;
            this->point = pri[t2];
        }
        return;
    }

    if (figureList.size() == 3) { // 有三种牌
        // 只能是三连对
        int t1 = figureList[0], t2 = figureList[1], t3 = figureList[2];
        if (cnt[t1] != 2 || cnt[t2] != 2 || cnt[t3] != 2) { // 不是对子
            return;
        }
        if (t3 - t1 == 2) { // 直接相邻
            this->type = threePairs;
            this->point = pri[t3];
        } else if (t2 - t1 == 11 && t1 == 1) { // A Q K
            this->type = threePairs;
            this->point = pri[t1];
        }
        return;
    }

    if (figureList.size() == 5) { // 顺子或者同花顺
        if (cards.size() != 5) {  // 有牌不止一张
            return;
        }
        int t1 = figureList[0], t2 = figureList[1], t3 = figureList[4];
        if (t3 - t1 == 4) { // 直接相邻
            this->type = sameSuit ? straightFlush : straight;
            this->point = oriPri[t3];
        } else if (t2 - t1 == 9 && t1 == 1) { // A 10 J Q K
            this->type = sameSuit ? straightFlush : straight;
            this->point = oriPri[t1];
        }
        return;
    }
}

Hand::Hand(Cards cards)
{
    this->cards = cards.cards;
    preHandle();
    this->cards = cards.cards;
    for (int i = 0; i < this->cards.size(); ++i) {
        if (this->cards[i].isLevelCard && this->cards[i].suit == Suit::Heart) {
            Card tmpCard = this->cards[i];
            Hand tmpHand = *this;
            for (int j = 52; j > 0; --j) {
                this->cards[i] = Card(j, false);
                preHandle();
                if (this->isBigger(tmpHand)) {
                    tmpHand = *this;
                }
                this->cards = cards.cards;
            }
            *this = tmpHand;
            this->cards = cards.cards;
            return;
        }
    }
}

void Hand::reset()
{
    this->type = illegal;
    this->point = -1;
    this->cards.clear();
}

bool Hand::isBigger(const Hand &other) const
{
    if (this->type != other.type) { // 不同牌型
        if (other.type == HandType::illegal) {
            return true;
        }
        if (this->type < HandType::fourBomb) { // 不能压
            return false;
        }
        return this->type > other.type;
    }
    return this->point > other.point;
}
