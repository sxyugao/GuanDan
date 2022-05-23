#include "cards.h"

Cards::Cards(){}

Cards::Cards(QJsonObject data)
{
    QJsonArray cards = data.value("cards").toArray();
    for (auto &&card : cards) {
        int id = card.toObject().value("id").toInt();
        bool isLvl = card.toObject().value("isLevelCard").toBool();
        add(Card(id, isLvl));
    }
}

void Cards::add(Card card)
{
    cards.append(card);
}

void Cards::remove(Card card)
{
    for (int i = 0; i < cards.size(); ++i) {
        if (card == cards[i]) {
            cards.removeAt(i);
            break;
        }
    }
}

Card &Cards::operator[](int pos)
{
    return cards[pos];
}

int Cards::size()
{
    return cards.size();
}

void Cards::clear()
{
    cards.clear();
}

void Cards::sortByPoint()
{
    std::sort(cards.begin(), cards.end());
}

bool cmp(const Card &a, const Card &b)
{
    return a.figure < b.figure;
}

void Cards::sortByFigure()
{
    std::sort(cards.begin(), cards.end(), cmp);
}

void Cards::generateJson()
{
    QJsonArray jsonArr;
    for (auto &&card : cards) {
        jsonArr.append(card.toJsonObj());
    }
    data.insert("cards", jsonArr);
}
