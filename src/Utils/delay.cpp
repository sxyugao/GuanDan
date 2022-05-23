#include "delay.h"

Delay::Delay() {}

void Delay::exec(int ms)
{
    QEventLoop delay;
    QTimer::singleShot(ms, &delay, SLOT(quit()));
    delay.exec();
}
