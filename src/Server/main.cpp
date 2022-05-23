#include "gameserver.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    GameServer *gameServer = new GameServer();
    gameServer->run();
    return a.exec();
}
