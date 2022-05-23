#ifndef TRANSDATA_H
#define TRANSDATA_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

enum Opt {
    opt_err,
    opt_gameStart,
    opt_gameFinish,
    opt_gameGoingOn,
    opt_waitForOthers,
    opt_handOutCardsFinish,
    opt_handOutScore,
    opt_handOutScoreFinish,
    opt_roundFinish,
    opt_readyForNewRound,
    opt_getTribute,
    opt_payTribute,
    opt_giveBackTribute,
    opt_antiTribute,
    opt_showTributes,
    opt_showTributesFinish
};

class TransData
{
public:
    virtual ~TransData();
    virtual void generateJson() = 0;
    QJsonObject toJsonObj();
    QByteArray toStream();

    QJsonObject data;
};

#endif // TRANSDATA_H
