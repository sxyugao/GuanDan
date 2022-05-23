#include "tributes.h"

Tributes::Tributes() {}

Tributes::Tributes(QJsonObject data)
{
    QJsonArray arr = data.value("tributes").toArray();
    for (auto x : arr) {
        list.append(Tribute(x.toObject()));
    }
}

void Tributes::add(Tribute tribute)
{
    list.append(tribute);
}

void Tributes::clear()
{
    list.clear();
}

int Tributes::size()
{
    return list.size();
}

Tribute &Tributes::operator[](int pos)
{
    return list[pos];
}

void Tributes::generateJson()
{
    data.insert("opt", opt_showTributes);
    QJsonArray arr;
    for (auto x : list) {
        arr.append(x.toJsonObj());
    }
    data.insert("tributes", arr);
}
