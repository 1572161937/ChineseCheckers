#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include<QString>
#include <QMainWindow>
#include<mainwindow.h>
#include <networksocket.h>
#include<QString>
#include<networkdata.h>
#include"ErrorWindow.h"
namespace Ui {
class ClientWindow;
}


class ClientWindow :public MainWindow
{
    Q_OBJECT

public:
    ClientWindow();
    ~ClientWindow();
    NetworkSocket *socket;

    QString host;

    quint16 port;


    QString loc_Area();   //确定玩家初始区域
    QString myArea = "";   //玩家初始区域
    QString get_id(QString a);    //根据区域返回玩家编号
    void connected();
    void displayError();
    void receive(NetworkData w);
    QString player[10];
    ErrorWindow error;

    int rank = 0;  //记录玩家排名

    void move_op(NetworkData data);
    void game_win(NetworkData data);
    void mouseReleaseEvent(QMouseEvent *);
    void timerEvent(QTimerEvent*);
    void move_Rt(int x,int y);
    void change_char_to_int(char *p);


    int time_id2;
    int time_id3_clock_down;
    int count_down;
    void restart_time();
    void wipe_player(int id);  //20秒内不行棋则判负，扫除该玩家

    QLabel *scn;   //显示倒计时
    QLabel * lab; //显示用户名

    /*bool if_overtime;
    int wipe_stones[15];*/  //记录将要被染白的棋盘坐标   这两行需要定义到mainwindow中，因为paintevent需要使用

    /*int failed_num;        //记录被清除的玩家数，用来跳过轮转
    int fall_players[6];*/  //记录被清除的玩家，跳过其轮转     经检验，这两组也应当放在mainwindow中，因为change_int_to_char函数中的轮转会用到

    //以下为AI设计
    double sumDist = 0;
    double straight_score = 3;
    double biasFine_light = 1;
    double biasFine_serious = 4;//偏移罚分
    double lastchessFine = 3;
    double lastchessFine_serious = 10;//末子距离罚分
    double lastchessPos;
    double mozi;      //末子编号
    double mozi_dis;       //末子距离


    int fnl;  //记录最终选择的落子点
    int father_fnl;  //记录搜索到落子点的父节点
    int begin_point;  //记录起始点在棋群中的编号，确定l中的psr_AI[begin_point]用来行棋
    void clear_score();  //清零所有棋盘点的落子分数
    void AI_op();  //AI行棋函数（总）
    void wipe_AIPossible();  //清除AI搜索的Possible_Location标记，这里因为不用将棋子染白，所以不能调用原来的wipe_dbfs()
//    void No3_AI_search(int begin_point); //三号玩家（最下方，s.color=3，me=3，被分配到三号棋，对应原五号三角形和五号棋群)的行棋函数
    void dfs_markSingle(int floor,int i,int begin_point/*,double base_score*/,QString s);  //详细说明函数用法：找以l[i]为起点，所能够走到的点，然后分别将起点替换为终点，计算每个状态下的分数
    int begin_serial;   //用来记录AI搜索路径起始点在棋盘上的坐标编号

    bool processing;    //记录AI是否运行
    void wipe_AIroute();     //清除AI搜索时储存在每个点的路径
    void AI_move_Rt(QString rut);
    int time_AI;
    bool spc_st();   //判断两种特殊情况
    bool win_stu();
    int AI_D;   //判断是否应该AI走棋
    void mid_AIfct();
    long long me_time;    //用来接收时间戳
    long long send_time;  //用来接收时间戳
    QString player_zone(int mode,int pn);
    int tip;   //弥补AI不走的bug，检测state的值是否变化
    int record_AImove;
    int record_AImove2;

     bool continue_jump;  //判断是否是点击连跳
    QString AI_sended;   //me的AI行棋完毕之后再发送的路径
    void TimerEvent2(QTimerEvent*);

    void click_move_cj(NetworkData data);   //点击专用连跳函数，与接收路径的连跳函数分开，避免出现bug，这个函数中不需要设置AI走棋，因为这个函数调用的时候本身就是自己在行棋
    int click_move_cj_time;

    int turn = 0;



    //以下为保留路径设计

    void show_former();
    QString former_route[8];   //记录玩家之前的路径
    void ccti_former(QString rut);
       //drawed_route作为一个int类型的数组，定义在mainwindow中，作为update重画的路径
    int former_route_number;


    //防止AI发生信息接受不到的错误
    int aisend_countdown;
    int asc_mark;
    bool AI_complete;

    int step_count = 0;  //记录步数


    //联机调试
    bool turnning;

private:
    Ui::ClientWindow *ui;
};

#endif // CLIENTWINDOW_H
