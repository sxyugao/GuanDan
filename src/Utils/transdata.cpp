#include "transdata.h"

TransData::~TransData() {}

QByteArray TransData::toStream()
{
    generateJson();
    QJsonDocument jsonDoc(data);
    return jsonDoc.toJson();
}

QJsonObject TransData::toJsonObj()
{
    generateJson();
    return data;
}
