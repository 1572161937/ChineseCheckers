#ifndef LOCATION_H
#define LOCATION_H
#include<QString>

class Location
{
public:
    Location();
    int x1;
    int y1;
    int cv_x;
    int cv_y;
    QString psr;
    bool psr_set;

    //以下为AI设计
    double score = 0;   //记录AI搜索落子在这个点上的总分（写完发现好像没有用过）
    QString psr_AI[70]; //需要为十个棋子都配置可能路径的储存数组
};

#endif // LOCATION_H
