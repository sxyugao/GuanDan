#include "game.h"

Game::Game(QObject *parent) : QObject{parent} {}

void Game::initCards()
{
    std::vector<int> v;
    for (int i = 1; i <= cards_in_pack; ++i)
        for (int j = 0; j < pack_of_cards; ++j)
            v.push_back(i);
    std::random_shuffle(v.begin(), v.end());
    int p = 0;
    for (int i = 0; i < player_num; ++i) {
        players[i] = new Player(i, this->level);
        for (int j = 0; j < player_cards; ++j) {
            players[i]->getCard(Card(v[p], this->level));
            p++;
        }
    }
    emit handOutCards();
}

void Game::init()
{
    std::srand(time(NULL));
    level = 2;
    tryTimes[0] = tryTimes[1] = 0;
    firstPlayer = std::rand() % 4;
    initCards();
}

void Game::newRound()
{
    if (!escaped.empty()) {
        firstPlayer = score.rank[0];
    }
    escaped.clear();
    memset(vis, 0, sizeof(vis));
    round = Round(firstPlayer, level);
    emit handOutRound();
}

void Game::handleRound(QJsonObject data)
{
    round = Round(data);
    // 有没有人出完牌了
    int id = -1;
    for (int i = 0; i < player_num; ++i) {
        if (!round.remains[i] && !vis[i]) {
            vis[i] = true;
            id = i;
            escaped.append(i);
        }
    }
    round.lstPlayer = round.nowPlayer;
    // 下一个该谁出牌了
    if (id != -1) {
        round.fromFriend = true;
        round.nowPlayer = (round.nowPlayer + 2) % 4;
    } else {
        round.nowPlayer = (round.nowPlayer + 1) % 4;
    }
    // 一轮过后回到自己
    if (round.nowPlayer == round.lstPlayedPlayer) {
        round.nowHand.reset();
    }
    if (escaped.size() == player_num - 1) {
        emit roundFinish();
    } else {
        emit handOutRound();
    }
}

void Game::upgrade()
{
    int winner = escaped[0] % 2, loser = (escaped[0] + 1) % 2;
    // 打过A级了
    if (score.levels[winner] == 14) {
        // 对家不能是末游
        if (winner == escaped[1] % 2 || winner == escaped[2] % 2) {
            emit gameFinish();
            return;
        }
    }
    tryTimes[winner] = 0;
    tryTimes[loser] += 1;
    // 升级
    if (winner == escaped[1] % 2) { //双上
        score.levels[winner] += 3;
    } else if (winner == escaped[2] % 2) { // 对家不是末游
        score.levels[winner] += 2;
    } else { // 对家是末游
        score.levels[winner] += 1;
    }
    if (score.levels[winner] > 14) {
        score.levels[winner] = 14;
    }
    // 打不过A级重打
    if (score.levels[loser] == 14 && tryTimes[loser] > 3) {
        score.levels[loser] = 2;
        tryTimes[loser] = 0;
    }
    for (int i = 0; i < player_num - 1; ++i) {
        score.rank[i] = escaped[i];
    }
    for (int i = 0; i < player_num; ++i) {
        if (!vis[i]) {
            score.rank[player_num - 1] = i;
        }
    }
    level = score.levels[escaped[0] % 2];
    // 重新发牌
    for (int i = 0; i < player_num; ++i) {
        delete players[i];
    }
    initCards();
}

void Game::preparePayTribute()
{
    if (escaped.empty()) {
        emit payTribute(true);
        return;
    }
    // 预处理贡牌对象
    memset(giveTributeTo, -1, sizeof(giveTributeTo));
    needToPayTribute.clear();
    needToPayTribute.append(score.rank[player_num - 1]);
    int redJokers = players[needToPayTribute[0]]->countRedJokers();
    if (escaped[0] % 2 == escaped[1] % 2) { //双上
        needToPayTribute.append(score.rank[player_num - 2]);
        redJokers += players[needToPayTribute[1]]->countRedJokers();
    }
    emit payTribute(redJokers == 2);
}

void Game::prepareGiveBackTribute()
{
    needToPayTribute.clear();
    needToPayTribute.append(escaped[0]);
    if (escaped[0] % 2 == escaped[1] % 2) { //双上
        needToPayTribute.append(escaped[1]);
        bool f = tributes[0].card < tributes[1].card;
        giveTributeTo[escaped[!f]] = tributes[f].fromPlayer;
        giveTributeTo[tributes[f].fromPlayer] = escaped[!f];
        giveTributeTo[escaped[f]] = tributes[!f].fromPlayer;
        giveTributeTo[tributes[!f].fromPlayer] = escaped[f];

    } else {
        giveTributeTo[escaped[0]] = score.rank[player_num - 1];
        giveTributeTo[score.rank[player_num - 1]] = escaped[0];
    }
    for (int i = 0; i < tributes.size(); ++i) {
        tributes[i].toPlayer = giveTributeTo[tributes[i].fromPlayer];
    }
}

void Game::giveBackTributeAllRecv()
{
    for (int i = 0; i < tributes.size(); ++i) {
        tributes[i].toPlayer = giveTributeTo[tributes[i].fromPlayer];
    }
}

void Game::recvTribute(QJsonObject data)
{
    tributes.add(Tribute(data));
}

QByteArray Game::roundToStream()
{
    return round.toStream();
}

QByteArray Game::playerToStream(int id)
{
    return players[id]->toStream();
}

QByteArray Game::scoreToStream()
{
    return score.toStream();
}

QByteArray Game::tributesToStream()
{
    return tributes.toStream();
}

QList<int> &Game::tributeList()
{
    return needToPayTribute;
};
