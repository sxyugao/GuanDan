#include "cardbutton.h"

CardButton::CardButton(Card card, QWidget *parent) : QWidget{parent}
{
    setDisabled(true);
    this->selected = -1;
    this->img[0].load(":/images/back_pic.jpg");
    this->img[1].load(QString(":/images/%1(%2).jpg").arg(card.figure).arg(card.id));
    this->isFront = card.id > 0;
    resize(card_width, card_height);
    this->card = card;
}

void CardButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(rect(), img[isFront]);
}

void CardButton::mousePressEvent(QMouseEvent *event)
{
    selected = -selected;
    move(this->x(), this->y() - selected * card_height / 3);
    emit cardSelected(selected > 0, this);
}

bool CardButton::operator<(const CardButton &other) const
{
    return !(card < other.card);
}
