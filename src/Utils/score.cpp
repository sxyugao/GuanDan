#include "score.h"

Score::Score() {}

Score::Score(QJsonObject data)
{
    QJsonArray arr_rank = data.value("rank").toArray();
    QJsonArray arr_levels = data.value("levels").toArray();
    for (int i = 0; i < 4; ++i) {
        rank[i] = arr_rank[i].toInt();
    }
    for (int i = 0; i < 2; ++i) {
        levels[i] = arr_levels[i].toInt();
    }
}

void Score::generateJson()
{
    data.insert("opt", opt_handOutScore);
    QJsonArray arr_rank, arr_levels;
    for (int i = 0; i < 4; ++i) {
        arr_rank.append(rank[i]);
    }
    data.insert("rank", arr_rank);
    for (int i = 0; i < 2; ++i) {
        arr_levels.append(levels[i]);
    }
    data.insert("levels", arr_levels);
}
