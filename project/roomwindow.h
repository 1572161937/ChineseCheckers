#ifndef ROOMWINDOW_H
#define ROOMWINDOW_H
#include<networkdata.h>
#include<networksocket.h>
#include<networkserver.h>
#include <QMainWindow>
#include<QString>
#include<QLabel>

namespace Ui {
class RoomWindow;
}

class RoomWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RoomWindow(QWidget *parent = nullptr);
    ~RoomWindow();
    void updateInform(NetworkData data);
    void single_update(NetworkData data);
    QString playerList[6]; //“房间内玩家”数组
    QString playerstate[6];  //“房间内玩家状态“数组
    int Player_num = 0; //房间内玩家数
    void Dele(NetworkData data); //一个玩家退出房间
    void Ready_change(NetworkData data);  //准备状态改变
    QLabel *player_name[6]; //六个框中的玩家名称
    QLabel *ListState[6];   //六个框中的玩家状态
    void paintEvent(QPaintEvent *);

    QString Final_Ranking[6];  //记录最后的玩家排名，便于最后显示玩家列表
    void Loc_me();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();


private:
    Ui::RoomWindow *ui;
};

#endif // ROOMWINDOW_H
