#include "gameserver.h"

GameServer::GameServer(QObject *parent) : QObject{parent}
{
    game = new Game();
    tcpServer = new QTcpServer();

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(joinGame()));

    connect(game, SIGNAL(handOutCards()), this, SLOT(handOutCards()));
    connect(game, SIGNAL(handOutRound()), this, SLOT(handOutRound()));
    connect(game, SIGNAL(roundFinish()), this, SLOT(roundFinish()));
    connect(game, SIGNAL(payTribute(bool)), this, SLOT(payTribute(bool)));
}

void GameServer::run()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // 寻找本机 IP
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList[i] != QHostAddress::LocalHost && ipAddressesList[i].toIPv4Address()) {
            ipAddress = ipAddressesList[i].toString();
            break;
        }
    }
    if (ipAddress.isEmpty()) {
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }

    // 启动服务器
    if (!tcpServer->listen(QHostAddress(ipAddress), 6666)) {
        cout << "Failed to bind port, please close other servers or change the port!" << endl;
        system("pause");
        exit(0);
    }
    cout << "The server is running on" << endl;
    cout << QString("IP: %1 port: %2")
                .arg(tcpServer->serverAddress().toString())
                .arg(tcpServer->serverPort())
                .toStdString()
         << endl;
}

void GameServer::sendMsg(QTcpSocket *tcp, QByteArray msg)
{
    Delay::exec(25);
    tcp->write(msg);
}

void GameServer::recvMsg()
{
    client = qobject_cast<QTcpSocket *>(sender());
    QByteArray ori = client->readAll();
    QJsonObject data = QJsonDocument::fromJson(ori).object();
    qDebug() << data;
    switch (data.value("opt").toInt()) {
    case opt_handOutCardsFinish:
        ++cnt;
        if (cnt == player_num) {
            cnt = 0;
            handOutScore();
        }
        break;
    case opt_handOutScoreFinish:
        ++cnt;
        if (cnt == player_num) {
            cnt = 0;
            game->preparePayTribute();
        }
        break;
    case opt_showTributesFinish:
        ++cnt;
        if (cnt == player_num) {
            cnt = 0;
            st = !st;
            if (st) {
                giveBackTribute();
            } else {
                tributeTimeFinish();
            }
        }
        break;
    case opt_gameGoingOn:
        game->handleRound(data);
        break;
    case opt_readyForNewRound:
        ++cnt;
        if (cnt == player_num) {
            cnt = 0;
            game->upgrade();
        }
        break;
    case opt_payTribute:
        ++cnt;
        game->recvTribute(data);
        if (cnt == game->tributeList().size()) {
            cnt = 0;
            game->prepareGiveBackTribute();
            handOutTributes();
            game->tributes.clear();
        }
        break;

    case opt_giveBackTribute:
        ++cnt;
        game->recvTribute(data);
        if (cnt == game->tributeList().size()) {
            cnt = 0;
            game->giveBackTributeAllRecv();
            handOutTributes();
            game->tributes.clear();
        }
        break;
    case opt_antiTribute:
        ++cnt;
        if (cnt == player_num) {
            cnt = 0;
            game->newRound();
        }
        break;
    }
}

void GameServer::joinGame()
{
    client = tcpServer->nextPendingConnection();
    QJsonObject msg;
    QJsonDocument data;

    // 服务器已到人数上限
    if (clients.size() == player_num) {
        msg.insert("opt", opt_err);
        msg.insert("msg", "服务器人数已满");
        data.setObject(msg);
        sendMsg(client, data.toJson());
        return;
    }

    // 将新的客户端加入列表
    clients.append(client);
    connect(client, SIGNAL(readyRead()), this, SLOT(recvMsg()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    cout << QString("A new connection from %1").arg(client->peerAddress().toString()).toStdString()
         << endl;

    // 告知客户端等待其他玩家
    msg.insert("opt", opt_waitForOthers);
    data.setObject(msg);
    sendMsg(client, data.toJson());

    // 人满了，游戏开始
    if (clients.size() == player_num) {
        // 延时500毫秒，让上一个 opt_waitForOthers 先被客户端收到
        Delay::exec(500);
        cout << "Game Start!" << endl;
        game->init();
    }
}

void GameServer::roundFinish()
{
    Delay::exec(500);
    cout << "roundFinish!" << endl;
    QJsonObject msg;
    msg.insert("opt", opt_roundFinish);
    QJsonDocument data(msg);
    for (auto &&client : clients) {
        sendMsg(client, data.toJson());
    }
}

void GameServer::payTribute(bool isAnti)
{
    QJsonObject msg;
    QJsonDocument data;
    if (!isAnti) {
        msg.insert("opt", opt_payTribute);
        data.setObject(msg);
        for (int id : game->tributeList()) {
            cout << QString("Player %1 needs to pay tribute.").arg(id).toStdString() << endl;
            sendMsg(clients[id], data.toJson());
        }
    } else {
        cout << "Refuse to pay tribute!" << endl;
        msg.insert("opt", opt_antiTribute);
        data.setObject(msg);
        for (auto &&client : clients) {
            sendMsg(client, data.toJson());
        }
    }
}

void GameServer::giveBackTribute()
{
    QJsonObject msg;
    QJsonDocument data;
    msg.insert("opt", opt_giveBackTribute);
    data.setObject(msg);
    for (int id : game->tributeList()) {
        cout << QString("Player %1 needs to give back tribute.").arg(id).toStdString() << endl;
        sendMsg(clients[id], data.toJson());
    }
}

void GameServer::tributeTimeFinish()
{
    QJsonObject msg;
    QJsonDocument data;
    msg.insert("opt", opt_antiTribute);
    data.setObject(msg);
    for (auto &&client : clients) {
        sendMsg(client, data.toJson());
    }
}

void GameServer::handOutScore()
{
    for (auto &&client : clients) {
        sendMsg(client, game->scoreToStream());
    }
}

void GameServer::handOutCards()
{
    for (int i = 0; i < player_num; ++i) {
        sendMsg(clients[i], game->playerToStream(i));
    }
}

void GameServer::handOutRound()
{
    for (auto &&client : clients) {
        sendMsg(client, game->roundToStream());
    }
}

void GameServer::handOutTributes()
{
    for (auto &&client : clients) {
        sendMsg(client, game->tributesToStream());
    }
}

void GameServer::disconnected()
{
    QJsonObject msg;
    msg.insert("opt", opt_err);
    msg.insert("msg", "有玩家退出，游戏结束！");
    QJsonDocument data(msg);
    for (int i = 0; i < clients.size(); ++i) {
        if (sender() != clients[i]) {
            sendMsg(clients[i], data.toJson());
        }
    }
    Delay::exec(500);
    cout << "Someone exits, game finished!" << endl;
    system("pause");
    exit(0);
}
