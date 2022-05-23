#include "gameclient.h"

GameClient::GameClient(QMainWindow *parent)
{
    this->parent = parent;
    this->gamePanel = new GamePanel(parent);
    tcpClient = new QTcpSocket();
    // 连接成功后查看是否加入成功
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(recvMsg()));
    // 连接成功后将背景清空
    connect(this, SIGNAL(waitForOthers()), parent, SLOT(initBackGround()));

    connect(gamePanel, SIGNAL(playCard()), this, SLOT(playCard()));
    connect(gamePanel, SIGNAL(handOutCardsFinish()), this, SLOT(handOutCardsFinish()));
    connect(gamePanel, SIGNAL(handOutScoreFinish()), this, SLOT(handOutScoreFinish()));
    connect(gamePanel, SIGNAL(initDataFinish()), this, SLOT(readyForNewRound()));
    connect(gamePanel, SIGNAL(payTribute()), this, SLOT(sendTribute()));
    connect(gamePanel, SIGNAL(showTributesFinish()), this, SLOT(showTributesFinish()));

    connect(parent, SIGNAL(initBackGroundFinished()), gamePanel, SLOT(waitForOthers()));
}

void GameClient::recvMsg()
{
    QByteArray ori = tcpClient->readAll();
    QJsonObject data = QJsonDocument::fromJson(ori).object();
    qDebug() << data;
    switch (data.value("opt").toInt()) {
    case opt_err:
        QMessageBox::warning(parent, "错误", data.value("msg").toString(), QMessageBox::Ok);
        exit(0);
        break;
    case opt_waitForOthers:
        emit waitForOthers();
        break;
    case opt_gameStart:
        gamePanel->initPlayer(data);
        gamePanel->handOutCards();
        break;
    case opt_handOutScore:
        gamePanel->initScore(data);
        break;
    case opt_gameGoingOn:
        gamePanel->handleRound(data);
        break;
    case opt_roundFinish:
        gamePanel->initData();
        break;
    case opt_payTribute:
        gamePanel->initTributeUI(true);
        break;
    case opt_giveBackTribute:
        gamePanel->initTributeUI(false);
        break;
    case opt_showTributes:
        gamePanel->handleTributes(data);
        break;
    case opt_antiTribute:
        gamePanel->antiTribute();
        break;
    case opt_gameFinish:
        QMessageBox::warning(parent, "游戏结束", "游戏结束，请看计分板", QMessageBox::Ok);
        break;
    }
}

void GameClient::sendMsg(QByteArray data)
{
    tcpClient->write(data);
}

void GameClient::playCard()
{
    sendMsg(gamePanel->roundToStream());
}

void GameClient::sendTribute()
{
    sendMsg(gamePanel->tributeToStream());
}

void GameClient::readyForNewRound()
{
    QJsonObject data;
    data.insert("opt", opt_readyForNewRound);
    QJsonDocument jsonDoc(data);
    sendMsg(jsonDoc.toJson());
}

void GameClient::handOutCardsFinish()
{
    QJsonObject data;
    data.insert("opt", opt_handOutCardsFinish);
    QJsonDocument msg(data);
    sendMsg(msg.toJson());
}

void GameClient::handOutScoreFinish()
{
    QJsonObject data;
    data.insert("opt", opt_handOutScoreFinish);
    QJsonDocument jsonDoc(data);
    sendMsg(jsonDoc.toJson());
}

void GameClient::showTributesFinish()
{
    QJsonObject data;
    data.insert("opt", opt_showTributesFinish);
    QJsonDocument jsonDoc(data);
    sendMsg(jsonDoc.toJson());
}

void GameClient::connectToServer(QString ip, int port)
{
    tcpClient->abort();
    tcpClient->connectToHost(ip, port);
    if (!tcpClient->waitForConnected(1000)) {
        QMessageBox::warning(parent, "错误", tcpClient->errorString(), QMessageBox::Ok);
    }
}
