#include "gamepanel.h"

GamePanel::GamePanel(QWidget *parent) : QWidget{parent}
{
    // 初始化大小和位置
    resize(parent->size());
    move(0, 0);
    this->hide();
    // 初始化中心点位置
    centralX = this->width() / 2 - card_width;
    centralY = this->height() / 2 - card_height;
    // 初始化各个方向位移终点
    dx[0] = card_width - centralX;
    dx[2] = this->width() - 2 * card_width - centralX;
    dy[1] = this->height() - 1.5 * card_height - centralY;
    dy[3] = card_height - centralY;
    reviseDx[0] = dx[0] + card_width * 2;
    reviseDx[2] = dx[2] - card_width * 2;
    reviseDy[1] = dy[1] - card_height * 2 - 20;
    reviseDy[3] = dy[3] + card_height * 1.5;
}

void GamePanel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap background(":images/table.jpg");
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(rect(), background);
}

void GamePanel::waitForOthers()
{
    this->show();
    // 新建一张临时中心牌
    centralCard = new CardButton(Card(), this);
    centralCard->move(centralX, centralY);
    centralCard->show();
    // 初始化出牌按钮
    btn_play = new QPushButton("出牌", this);
    btn_play->setStyleSheet("background:white");
    btn_play->move(centralX, centralY + dy[1] - 60);
    btn_play->setDisabled(true);
    btn_play->setVisible(false);
    btn_play->hide();
    connect(btn_play, SIGNAL(clicked()), this, SLOT(playHand()));
    // 初始化不要按钮
    btn_skip = new QPushButton("不要", this);
    btn_skip->setStyleSheet("background:white");
    btn_skip->move(btn_play->x() + 100, btn_play->y());
    btn_skip->setDisabled(true);
    btn_skip->setVisible(false);
    btn_skip->hide();
    connect(btn_skip, SIGNAL(clicked()), this, SLOT(skipRound()));
}

void GamePanel::initTributeUI(bool f)
{
    // 将按钮解绑出牌
    disconnect(btn_play, SIGNAL(clicked()), this, SLOT(playHand()));
    // 将按钮绑定贡牌
    connect(btn_play, SIGNAL(clicked()), this, SLOT(handleTribute()));
    // 设置按钮文本
    btn_play->setText(f ? "贡牌" : "还贡");
    // 启用按钮
    btn_play->setDisabled(false);
    // 初始化是贡牌还是还贡
    isPayTribute = f;
}

void GamePanel::handleTribute()
{
    if (tmpCards.size() != 1) {
        return;
    }
    if (isPayTribute) {
        if (tmpCards[0].point != player->maxCardPoint()) {
            return;
        }
        if (tmpCards[0].suit == Suit::Heart) {
            return;
        }
    } else {
        if (tmpCards[0].point > 10) {
            return;
        }
    }
    tribute = Tribute(isPayTribute ? opt_payTribute : opt_giveBackTribute, tmpCards[0]);
    tribute.fromPlayer = player->id;
    cardButtons[1].remove(tmpCards[0]);
    updateCards(1);
    tmpCards.clear();
    // 将按钮恢复
    disconnect(btn_play, SIGNAL(clicked()), this, SLOT(handleTribute()));
    connect(btn_play, SIGNAL(clicked()), this, SLOT(playHand()));
    btn_play->setText("出牌");
    btn_play->setDisabled(true);
    // 发送给服务器
    emit payTribute();
}

void GamePanel::handleTributes(QJsonObject data)
{
    Tributes tributes = Tributes(data);
    for (int i = 0; i < tributes.size(); ++i) {
        int p = playerPlaces[tributes[i].fromPlayer];
        playedCards[p].clear();
        playedCards[p].add(new CardButton(tributes[i].card, this));
        if (p != 1) {
            cardButtons[p].removeAt(0);
        }
        updatePlayedCards(p);
        if (tributes[i].toPlayer == player->id) {
            CardButton *tmpCard = new CardButton(tributes[i].card, this);
            tmpCard->setDisabled(false);
            connect(tmpCard,
                    SIGNAL(cardSelected(bool, CardButton *)),
                    this,
                    SLOT(selectCard(bool, CardButton *)));
            cardButtons[1].add(tmpCard);
        } else {
            cardButtons[playerPlaces[tributes[i].toPlayer]].add(new CardButton(Card(), this));
        }
        cardButtons[playerPlaces[tributes[i].toPlayer]].sort();
        updateCards(playerPlaces[tributes[i].toPlayer]);
    }
    emit showTributesFinish();
}

void GamePanel::initData()
{
    centralCard->show();
    for (int i = 0; i < 4; ++i) {
        playedCards[i].clear();
        cardButtons[i].clear();
    }
    btn_play->hide();
    btn_skip->hide();
    tmpCards.clear();
    tribute = Tribute(opt_antiTribute, Card());
    player->clear();
    emit initDataFinish();
}

void GamePanel::initScore(QJsonObject data)
{
    scoreborad->updateScore(Score(data));
    emit handOutScoreFinish();
}

void GamePanel::initPlayer(QJsonObject data)
{
    player = new Player(data.value("id").toInt(), data.value("lvl").toInt());
    if (scoreborad == nullptr) {
        scoreborad = new Scoreboard(player->id, this);
    }
    QJsonArray cards = data.value("cards").toArray();
    for (auto &&card : cards) {
        int id = card.toObject().value("id").toInt();
        bool isLvl = card.toObject().value("isLevelCard").toBool();
        player->getCard(Card(id, isLvl));
    }
    // 玩家位置映射
    for (int i = 0; i < 4; ++i) {
        playerPlaces[(player->id + i) % 4] = (1 + i) % 4;
    }
}

void GamePanel::updatePlayedCards(int id)
{
    int cnt = playedCards[id].size();
    int stX = 0, stY = 0, dX = 0, dY = 0;
    if (id % 2) {
        stX = (this->width() - (card_width / 2 * (cnt - 1) + card_width)) / 2;
        stY = centralY + reviseDy[id];
        dX = card_width / 2;
    } else {
        stX = centralX + reviseDx[id];
        stY = (this->height() - (card_height / 4 * (cnt - 1) + card_height)) / 2;
        dY = card_height / 4;
    }
    for (int i = 0; i < cnt; ++i) {
        playedCards[id][i]->move(stX + dX * i, stY + dY * i);
        playedCards[id][i]->raise();
        playedCards[id][i]->show();
    }
}

void GamePanel::handleRound(QJsonObject data)
{
    // 处理数据
    round = Round(data);
    // 更新手牌数和出牌区的列表
    playedCards[playerPlaces[round.lstPlayer]].clear();
    // 只有上一个出牌的人可能需要更新
    if (round.lstPlayedPlayer == round.lstPlayer) {
        for (int i = 0; i < round.nowHand.size(); ++i) {
            playedCards[playerPlaces[round.lstPlayer]].add(new CardButton(round.nowHand[i], this));
            if (round.lstPlayer == player->id) {
                cardButtons[playerPlaces[round.lstPlayer]].remove(round.nowHand[i]);
            } else {
                cardButtons[playerPlaces[round.lstPlayer]].removeAt(0);
            }
        }
        // 更新手牌区
        playedCards[playerPlaces[round.lstPlayer]].sort();
        updateCards(playerPlaces[round.lstPlayer]);
    }
    // 更新出牌区
    updatePlayedCards(playerPlaces[round.lstPlayer]);
    // 轮到自己
    if (round.nowPlayer == player->id) {
        // 启用按钮
        btn_play->setDisabled(false);
        btn_skip->setDisabled(false);
        // 牌出完了
        if (round.remains[player->id] == 0) {
            skipRound();
        }
    }
}

void GamePanel::playHand()
{
    Hand tmpHand(tmpCards);
    // 不是接风，牌也没大过上家
    if (!round.fromFriend && !tmpHand.isBigger(round.nowHand)) {
        return;
    }
    // 禁用按钮
    btn_play->setDisabled(true);
    btn_skip->setDisabled(true);
    tmpCards.clear();
    // 更新要发给服务器的round
    round.nowHand = tmpHand;
    round.fromFriend = false;
    round.lstPlayedPlayer = player->id;
    round.remains[player->id] = cardButtons[1].size() - tmpHand.size();
    emit playCard();
}

void GamePanel::skipRound()
{
    btn_play->setDisabled(true);
    btn_skip->setDisabled(true);
    emit playCard();
}

void GamePanel::updateCards(int id)
{
    int cnt = cardButtons[id].size();
    int stX = 0, stY = 0, dX = 0, dY = 0;
    if (id % 2) {
        stX = (this->width() - (card_width / 2 * (cnt - 1) + card_width)) / 2;
        stY = centralY + dy[id];
        dX = card_width / 2;
    } else {
        stX = centralX + dx[id];
        stY = (this->height() - (card_height / 4 * (cnt - 1) + card_height)) / 2 + 20;
        dY = card_height / 4;
    }
    for (int i = 0; i < cnt; ++i) {
        cardButtons[id][i]->move(stX + dX * i, stY + dY * i);
        cardButtons[id][i]->raise();
        cardButtons[id][i]->show();
    }
}

void GamePanel::handOutCards()
{
    QRect endValues[4];
    for (int i = 0; i < 4; ++i) {
        endValues[i] = QRect(QPoint(centralX + dx[i], centralY + dy[i]), card_size);
    }
    // 新建一张临时卡牌
    CardButton *tmpCard = new CardButton(Card(), this);
    tmpCard->show();
    for (int i = 0; i < player->cards.size(); ++i) {
        for (int j = 0; j < 4; ++j) {
            // 将临时卡牌移动到牌桌中心
            tmpCard->move(centralX, centralY);
            // 设置动画播放100毫秒
            QPropertyAnimation *animation = new QPropertyAnimation(tmpCard, "geometry");
            animation->setDuration(100);
            animation->setStartValue(tmpCard->geometry());
            animation->setEndValue(endValues[j]);
            animation->start();
            // 添加卡牌
            if (j == 1) {
                cardButtons[j].add(new CardButton(player->cards[i], this));
            } else {
                cardButtons[j].add(new CardButton(Card(), this));
            }
            // 延时阻塞线程200毫秒
            Delay::exec(200);
            updateCards(j);
        }
    }
    centralCard->hide();
    // 释放指针
    cardButtons[1].sort();
    updateCards(1);
    delete tmpCard;
    // 将卡牌点击事件和卡牌列表相联系
    for (int i = 0; i < cardButtons[1].size(); ++i) {
        tmpCard = cardButtons[1][i];
        tmpCard->setDisabled(false);
        connect(tmpCard,
                SIGNAL(cardSelected(bool, CardButton *)),
                this,
                SLOT(selectCard(bool, CardButton *)));
    }
    // 将按钮可视化
    btn_play->setVisible(true);
    btn_play->show();
    btn_skip->setVisible(true);
    btn_skip->show();
    emit handOutCardsFinish();
}

void GamePanel::antiTribute()
{
    for (int i = 0; i < 4; i++) {
        playedCards[i].clear();
    }
    tribute = Tribute(opt_antiTribute, Card());
    emit payTribute();
}

void GamePanel::selectCard(bool f, CardButton *card)
{
    if (f) {
        tmpCards.add(card->card);
    } else {
        tmpCards.remove(card->card);
    }
}

QByteArray GamePanel::roundToStream()
{
    return round.toStream();
}

QByteArray GamePanel::tributeToStream()
{
    return tribute.toStream();
}
