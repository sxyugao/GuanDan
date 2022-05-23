#ifndef DELAY_H
#define DELAY_H

#include <QEventLoop>
#include <QTimer>

class Delay
{
public:
    Delay();
    static void exec(int ms);
};

#endif // DELAY_H
