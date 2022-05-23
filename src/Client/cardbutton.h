#ifndef CARDBUTTON_H
#define CARDBUTTON_H

#include "card.h"

#include <QPainter>
#include <QPixmap>
#include <QWidget>

const int card_width = 52;
const int card_height = 74;
const QSize card_size(card_width, card_height);

class CardButton : public QWidget
{
    Q_OBJECT
public:
    explicit CardButton(Card card, QWidget *parent = nullptr);
    bool operator<(const CardButton &other) const;

    Card card;
    bool selected;

signals:
    void cardSelected(bool, CardButton *);

private:
    bool isFront;
    QPixmap img[2];

protected:
    virtual void paintEvent(QPaintEvent *event) final;
    virtual void mousePressEvent(QMouseEvent *event) final;
};

#endif // CARDBUTTON_H
