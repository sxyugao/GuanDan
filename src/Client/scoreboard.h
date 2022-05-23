#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "score.h"

#include <QLabel>
#include <QWidget>

const int margin = 20;

class Scoreboard : public QWidget
{
    Q_OBJECT
public:
    explicit Scoreboard(int id, QWidget *parent = nullptr);
    void updateScore(Score score);

signals:

private:
    QString text[4] = {"上游：%1", "二游：%1", "三游：%1", "下游：%1"};
    QLabel *rank[4], *levels[2], *group, *userInfo;
};

#endif // SCOREBOARD_H
