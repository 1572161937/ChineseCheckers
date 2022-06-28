#ifndef RANKINGLIST_H
#define RANKINGLIST_H

#include<QLabel>
#include<QPushButton>
#include<QString>
#include <QMainWindow>
#include<networkdata.h>

class RankingList : public QMainWindow
{
    Q_OBJECT
public:
    explicit RankingList(QWidget *parent = nullptr);
    void Rank(NetworkData data);
    QLabel *player_rank[6];
    void paintEvent(QPaintEvent *);

signals:

};

#endif // RANKINGLIST_H
