#ifndef TRIBUTES_H
#define TRIBUTES_H

#include "transdata.h"
#include "tribute.h"

#include <QList>

class Tributes : virtual public TransData
{
public:
    Tributes();
    Tributes(QJsonObject data);
    void add(Tribute tribute);
    void clear();
    Tribute &operator[](int pos);
    int size();
    virtual void generateJson() final;

    QList<Tribute> list;
};

#endif // TRIBUTES_H
