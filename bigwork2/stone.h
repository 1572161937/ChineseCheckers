#ifndef STONE_H
#define STONE_H
#include<QString>

class Stone
{
public:
    Stone();
    double x0;
    double y0;
    double color;
    double cv_xs;
    double cv_ys;

    double Dst;  //到终点的距离
    double Bias;  //偏移的距离
    double getDst();
    double getBias();
};

#endif // STONE_H
