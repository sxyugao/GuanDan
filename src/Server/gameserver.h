#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "delay.h"
#include "game.h"
#include "transdata.h"

#include <iostream>
#include <QList>
#include <QNetworkInterface>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

using std::cout;
using std::endl;

class GameServer : public QObject
{
    Q_OBJECT

public:
    explicit GameServer(QObject *parent = nullptr);
    void run();

signals:

private slots:
    // 处理 Game 类的消息，发送给 Client
    /// 加入游戏
    void joinGame();
    /// 分发卡牌
    void handOutCards();
    /// 分发计分信息
    void handOutScore();
    /// 分发当前回合信息
    void handOutRound();
    /// 告诉客户端当前回合结束
    void roundFinish();
    /// 让客户端贡牌
    void payTribute(bool isAnti);
    /// 让客户端还贡
    void giveBackTribute();
    // 处理 Client 发来的消息
    void recvMsg();      // 接收消息的槽
    void disconnected(); // 取消连接的槽

private:
    void handOutTributes();
    void tributeTimeFinish();
    void sendMsg(QTcpSocket *tcp, QByteArray msg);
    void newGame();

    QTcpServer *tcpServer;
    QTcpSocket *client;
    QVector<QTcpSocket *> clients;

    Game *game;
    int cnt = 0, st = 0;
};

#endif // GAMESERVER_H
