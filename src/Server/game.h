#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "round.h"
#include "score.h"
#include "tributes.h"

#include <algorithm>
#include <vector>
#include <QObject>

const int pack_of_cards = 2;
const int cards_in_pack = 54;
const int player_num = 4;
const int player_cards = (pack_of_cards * cards_in_pack) / player_num;

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);
    void upgrade();
    QByteArray roundToStream();
    QByteArray playerToStream(int id);
    QByteArray scoreToStream();
    QByteArray tributesToStream();
    void init();
    void handleRound(QJsonObject data);
    void newRound();
    void preparePayTribute();
    void prepareGiveBackTribute();
    void giveBackTributeAllRecv();
    void recvTribute(QJsonObject data);
    QList<int> &tributeList();

    Tributes tributes;

signals:
    void handOutCards();
    void handOutScore();
    void handOutRound();
    void roundFinish();
    void payTribute(bool);
    void giveBackTribute();
    void gameFinish();

private slots:

private:
    void initCards();

    Round round;
    Player *players[player_num];
    Score score;
    int level, firstPlayer;
    bool vis[4];
    int giveTributeTo[4];
    QList<int> escaped, needToPayTribute;
    int tryTimes[2];
};

#endif // GAME_H
