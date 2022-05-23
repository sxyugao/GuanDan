#include "tribute.h"

Tribute::Tribute() {}

Tribute::Tribute(Opt opt, Card card)
{
    this->opt = opt;
    this->card = card;
}

Tribute::Tribute(QJsonObject data)
{
    int id = data.value("id").toInt();
    bool isLevelCard = data.value("isLevelCard").toBool();
    card = Card(id, isLevelCard);
    opt = Opt(data.value("opt").toInt());
    fromPlayer = data.value("fromPlayer").toInt();
    toPlayer = data.value("toPlayer").toInt();
}

void Tribute::generateJson()
{
    data = card.toJsonObj();
    data.insert("opt", opt);
    data.insert("fromPlayer", fromPlayer);
    data.insert("toPlayer", toPlayer);
}
