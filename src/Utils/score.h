#ifndef SCORE_H
#define SCORE_H

#include "transdata.h"

class Score : public virtual TransData
{
public:
    Score();
    Score(QJsonObject data);
    virtual void generateJson() final;

    int rank[4] = {-1, -1, -1, -1}, levels[2] = {2, 2};
};

#endif // SCORE_H
