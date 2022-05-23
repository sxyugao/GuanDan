#include "scoreboard.h"

Scoreboard::Scoreboard(int id, QWidget *parent) : QWidget{parent}
{
    resize(parent->width(), parent->height() / 5);
    move(0, 0);
    QFont font("Microsoft YaHei", -1, QFont::Bold);
    // 用户编号
    userInfo = new QLabel(this);
    userInfo->move(width() - margin * 5.7, margin);
    userInfo->setText(QString("你的标号为：%1").arg(id));
    userInfo->setFont(font);
    userInfo->setStyleSheet("color:white;");
    userInfo->show();
    // 用户组别
    group = new QLabel(this);
    group->move(width() - margin * 5.7, margin * 2);
    group->setText(QString("你的阵营为：%1").arg(id % 2));
    group->setFont(font);
    group->setStyleSheet("color:white;");
    group->show();
    // 用户次序
    for (int i = 0; i < 4; ++i) {
        rank[i] = new QLabel(this);
        rank[i]->move(margin, margin * (i + 1));
        rank[i]->setText(text[i].arg(-1));
        rank[i]->setFont(font);
        rank[i]->setStyleSheet("color:white;");
        rank[i]->show();
    }
    // 组别级数
    for (int i = 0; i < 2; ++i) {
        levels[i] = new QLabel(this);
        levels[i]->move(width() - margin * 5.7, margin * (i + 3));
        levels[i]->setText(QString("阵营%1的级数：%2").arg(i).arg(2));
        levels[i]->setFont(font);
        levels[i]->setStyleSheet("color:white;");
        levels[i]->show();
    }
    show();
}

void Scoreboard::updateScore(Score score)
{
    for (int i = 0; i < 4; ++i) {
        rank[i]->setText(text[i].arg(score.rank[i]));
        rank[i]->show();
    }
    for (int i = 0; i < 2; ++i) {
        levels[i]->setText(QString("阵营%1的级数：%2").arg(i).arg(score.levels[i]));
        levels[i]->show();
    }
}
