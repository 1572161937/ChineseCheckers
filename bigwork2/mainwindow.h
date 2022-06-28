#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QString>
#include <QMainWindow>
#include"stone.h"
#include"location.h"
#include<QLabel>
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

    bool single_mode = false;

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
    virtual void mouseReleaseEvent(QMouseEvent *);
    void init_Possible_Location();
    bool getPos(QPoint pt,int &B_id);
    bool getPos(int x0,int y0, int &B_loc);
    bool canMove(int x1,int y1,int x2,int y2);

    virtual void timerEvent(QTimerEvent*);

    void green_init_cvrt_Loc();
    void green_init_cvrt_st();
    void m2_purple_init_cvrt_st();
    int Area_num; //颜色编号
    int route[250]; //记录路径
    virtual void change_char_to_int(char *p);  //将字符创表示的路径转为整形数组
    virtual void move_Rt(int x,int y); //坐标输入版的移动函数
    int change_mark(char mark); //将区域转为颜色编号
    int routeLength;
    //以下函数负责路径记录和更新操作
    void init_psr();
    void clear_psr();  //清理记录过的路径
    void init_psr_set();
    QString psr_m;   //传入函数接收棋子路径
    QString psr_mt; //最终信号要发送的路径,最后用的是这个

    bool getCvt(int x,int y,int &t);

    QString receive_route;
    int cnt=0;//输入版函数的计数器
    QLabel *edit = new QLabel;
    bool next=false;

    int time_id1;

    bool If_Win;
    void print();
    void change_state();

    bool if_overtime;
    int wipe_stones[15];

    int failed_num;      //记录被清除的玩家数，用来跳过轮转
    int fall_players[6];     //记录被清除的玩家编号，用来跳过轮转

    //AI设计
    int win_player[6];  //记录胜利玩家编号，跳过其轮转
    int win_num;   //记录胜利玩家数，用来跳过轮转
    QString AI_psr_mt;  //记录AI的行棋路径，防止与server传入的混淆，重新用一个变量

    //以下为保留路径设计
    int drawed_route[300];
    int drawed_route_length;
    bool cd_former_route = false;


    //以下为三人模式的实现
    void blue_init_cvrt_Loc();
    void red_init_cvrt_Loc();
    void m3_purple_init_cvrt_st();
    void m3_blue_init_cvrt_st();
    void m3_red_init_cvrt_st();


signals:


};

#endif // MAINWINDOW_H
