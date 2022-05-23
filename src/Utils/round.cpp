#include "round.h"

Round::Round() {}

Round::Round(int playerId, int lvl)
{
    nowHand = Hand();
    lstPlayer = nowPlayer = lstPlayedPlayer = playerId;
    level = lvl;
    fromFriend = false;
    for (int i = 0; i < 4; i++) {
        remains[i] = 27;
    }
}

Round::Round(QJsonObject data)
{
    level = data.value("level").toInt();
    nowPlayer = data.value("nowPlayer").toInt();
    lstPlayer = data.value("lstPlayer").toInt();
    lstPlayedPlayer = data.value("lstPlayedPlayer").toInt();
    fromFriend = data.value("fromFriend").toBool();
    Cards tmp(data.value("nowHand").toObject());
    nowHand = Hand(tmp);
    QJsonArray rem = data.value("remains").toArray();
    for (int i = 0; i < 4; ++i) {
        remains[i] = rem[i].toInt();
    }
}

void Round::generateJson()
{
    data.insert("opt", opt_gameGoingOn);
    data.insert("level", level);
    data.insert("nowPlayer", nowPlayer);
    data.insert("lstPlayer", lstPlayer);
    data.insert("lstPlayedPlayer", lstPlayedPlayer);
    data.insert("nowHand", nowHand.toJsonObj());
    data.insert("fromFriend", fromFriend);
    QJsonArray rem;
    for (int i = 0; i < 4; ++i) {
        rem.append(remains[i]);
    }
    data.insert("remains", rem);
}
