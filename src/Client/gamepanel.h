#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include "cardbuttons.h"
#include "delay.h"
#include "player.h"
#include "round.h"
#include "scoreboard.h"
#include "tributes.h"

#include <QList>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QWidget>

class GamePanel : public QWidget
{
    Q_OBJECT
public:
    explicit GamePanel(QWidget *parent = nullptr);
    void initPlayer(QJsonObject data);
    void initData();
    void handleRound(QJsonObject data);
    void handleTributes(QJsonObject data);
    void initScore(QJsonObject data);
    void handOutCards();
    void antiTribute();
    void initTributeUI(bool f);
    QByteArray roundToStream();
    QByteArray tributeToStream();

signals:
    void handOutCardsFinish();
    void handOutScoreFinish();
    void showTributesFinish();
    void playCard();
    void payTribute();
    void initDataFinish();

private slots:
    void waitForOthers();
    void selectCard(bool f, CardButton *card);
    void playHand();
    void handleTribute();
    void skipRound();
    void adjust();

private:
    // 更新卡牌界面
    void updateCards(int id);
    // 更新打出的卡牌
    void updatePlayedCards(int id);

    /// 计分板
    Scoreboard *scoreborad = nullptr;
    /// 中心临时卡牌
    CardButton *centralCard;
    /// 玩家
    Player *player;
    int playerPlaces[4];
    /// 中心位置
    int centralX, centralY;
    /// 中心位置到四个方向的变化量
    int dx[4] = {0}, dy[4] = {0};
    /// 出牌区的修正量
    int reviseDx[4] = {0}, reviseDy[4] = {0};
    // 可交互的卡牌
    CardButtons cardButtons[4];
    /// 临时被选中的卡牌
    Cards tmpCards;
    /// 出牌区的牌
    CardButtons playedCards[4];
    // 一些按钮
    QPushButton *btn_play, *btn_skip, *btn_adjust;
    /// 收到的Round
    Round round;
    // 贡牌相关
    bool isPayTribute;
    Tribute tribute;
    /// 上一次出牌的人
    int lstPlayer = -1;
    int st = 0;

protected:
    virtual void paintEvent(QPaintEvent *event) final;
};

#endif // GAMEPANEL_H
