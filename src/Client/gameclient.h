#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "gamepanel.h"
#include "transdata.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QTcpSocket>

class GameClient : public QObject
{
    Q_OBJECT
public:
    explicit GameClient(QMainWindow *parent = nullptr);
    void connectToServer(QString ip, int port);

signals:
    void waitForOthers();
    void gameStart();

private slots:
    void recvMsg();
    void handOutCardsFinish();
    void handOutScoreFinish();
    void showTributesFinish();
    void playCard();
    void readyForNewRound();
    void sendTribute();

private:
    void sendMsg(QByteArray data);

    QMainWindow *parent;
    GamePanel *gamePanel;
    QTcpSocket *tcpClient;
};

#endif // GAMECLIENT_H
