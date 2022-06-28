#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H
#include<mainwindow.h>
#include<QMainWindow>
#include "networkdata.h"
#include "networkserver.h"
#include "networksocket.h"
#include <QtNetwork>
#include"QTcpServer"
#include<QString>
#include"stone.h"
#include"location.h"
#include<QLabel>



namespace Ui {
class ServerWindow;
}

//class ServerWindow : public MainWindow
class ServerWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();
    NetworkServer *server;
    QTcpSocket *_socket[6]={NULL};

    QTcpSocket *s_send;    //记录发送给其他玩家的行棋数据
//    NetworkData Move(OPCODE::MOVE_OP,"","");
    QString data_1,data_2;   //记录MOVE_OP的两个数据
    void receive(QTcpSocket *a,NetworkData b);
    QString room="";
    static int num;
    QString user[6]={""};
    QString ready[6];
    bool judge_route(QString rut);
    void dfs_markSingle(int floor,int i);
    void wipe_dfs_markSinge();
    void move(NetworkData data);
    int time_id3;
    void timerEvent(QTimerEvent*);

    bool move_complete;

    //以下是MainWindow直接复制

    int state;
    int pattern = 0;
    int pt(int x);
    Stone s[75];
//    char a[120];
//    char pos_rut[120];
    bool occupied[121];
    bool Possible_Location[121];
    void dbfs(int count ,int i,QPainter &painter,QString a);
    void wipe_dbfs(QPainter &painter);
    void init_occupied();
    Location l[121];
    int r=12;
    int _selected_id;
    int _pst;
    int current_x,current_y;//记录当前落下棋子坐标，防止被wipe_dbfs()刷白
    int signal1=0;
    QPoint center(int x,int y);
    QPoint center(int id);
    virtual void paintEvent(QPaintEvent *);
    void drawstone(QPainter &painter,int i);
    void markstone();
//    virtual void mouseReleaseEvent(QMouseEvent *);
    void init_Possible_Location();
    bool getPos(QPoint pt,int &B_id);
    bool getPos(int x0,int y0, int &B_loc);
    bool canMove(int x1,int y1,int x2,int y2);


    void init_cvrt_Loc();
    void init_cvrt_st();
    int Area_num; //颜色编号
    int route[250]; //记录路径
    void change_char_to_int(char *p);  //将字符创表示的路径转为整形数组
    virtual void move_Rt(int x,int y); //坐标输入版的移动函数
    int change_mark(char mark); //将区域转为颜色编号
    int routeLength;
    //以下函数负责路径记录和更新操作
    void init_psr();
    void clear_psr();
    void init_psr_set();
    QString psr_m;   //传入函数接收棋子路径
    QString psr_mt; //最终信号要发送的路径,最后用的是这个

    bool getCvt(int x,int y,int &t);

    QString receive_route;
    int cnt=0;//输入版函数的计时器
    QLabel *edit = new QLabel;
    bool next=false;

    int time_id1;

    bool If_Win;
    void print();
    void change_state();

    //以上是MainWindow直接复制

    //以下为倒计时部分
    int count_down;
    int time_id_clock_down;
    void restart_time();
    void wipe_player(int id);  //时间到不行棋，判负，扫除玩家棋子

    bool if_overtime;     //判断是否有超时情况出现
    int wipe_stones[15];   //记录要染白的棋子在棋盘上对应位置的编号

    int failed_num;      //记录被清除的玩家数，用来跳过轮转
    int fall_players[6];     //记录被清除的玩家编号，用来跳过轮转
    int win_num;
    int win_players[6];

private:
    Ui::ServerWindow *ui;
};

#endif // SERVERWINDOW_H
