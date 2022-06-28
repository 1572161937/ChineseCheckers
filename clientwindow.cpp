#include "clientwindow.h"
#include "ui_clientwindow.h"
#include "QTcpServer"
#include"ErrorWindow.h"
#include "roomwindow.h"
#include<mainwindow.h>
#include<winwindow.h>
#include<rankinglist.h>
#include<QDebug>
#include<QMouseEvent>
#include<QPushButton>
#include<networkdata.h>
#include<QTimer>
#include<QTimerEvent>
#include<QLabel>
#include<math.h>
#include <ctime>

extern ClientWindow *Board;
extern RoomWindow *room;
extern NetworkSocket *socket;
extern QString PlayerName;
extern int num;
extern int me;
extern QString C_host;
extern quint16 C_port;
ClientWindow::ClientWindow():MainWindow()
//    QMainWindow(parent),
//    ui(new Ui::ClientWindow)
{

    socket = new NetworkSocket(new QTcpSocket(), this);
    connect(socket, &NetworkSocket::receive, this, &ClientWindow::receive);
    connect(socket->base(), &QAbstractSocket::errorOccurred, this, &ClientWindow::displayError);
    connect(socket->base(), &QAbstractSocket::connected, this, &ClientWindow::connected);

//    QString host="10.46.252.71";   //测试时替换

//    quint16 port=9999;  //测试时替换


    host = C_host;
    port = C_port;


    socket->hello(host, port);

    QFont font;
    font.setFamily("楷体");
    font.setPointSize(15);

    record_AImove = 0;
    record_AImove2 = 0;

    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::white);
    scn = new QLabel(this);
    scn->move(100,150);
    scn->resize(150,40);
    scn->setPalette(pe);
    scn->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    scn->setFrameShape(QFrame::Box);
    scn->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);");
    scn->show();


    count_down = 15;
    failed_num = 0;  //超时玩家的人数
    if_overtime = false;  //记录是否有超时情况
    for(int i=0;i<6;i++)
    {
        fall_players[i] = -1;
    }
    win_num = 0;
    for(int i=0;i<6;i++)
    {
        win_player[i]=-1;
    }
    next = false;
    lab = new QLabel(this);
    lab->move(100,200);
    lab->resize(150,40);
    lab->setPalette(pe);
    lab->setFrameShape(QFrame::Box);
    lab->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    lab->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);");
    lab->show();


    //AI设计
    sumDist = -900000000;
    double mozi = -1; //末子编号
    double mozi_dis = -1;  //末子距离

    processing = false;
    QPushButton * AI_openBtn = new QPushButton(this);
    AI_openBtn->move(100,360);
    AI_openBtn->setText("AI模式");
    connect(AI_openBtn,&QPushButton::clicked,[=](){
        processing = true;
        if(me == state)
          AI_op();
//       while(processing)
//       {
//           if(me == state)
//            AI_op();
//       }
    });
    AI_openBtn->show();
    QPushButton * AI_closeBtn = new QPushButton(this);
    AI_closeBtn->move(100,400);
    AI_closeBtn->setText("关闭AI");
    connect(AI_closeBtn,&QPushButton::clicked,[=](){
        processing = false;
    });
    AI_closeBtn->show();
    AI_D = 0;

    //以下为路径显示设计
}

ClientWindow::~ClientWindow()
{
    delete ui;
}
void ClientWindow::connected()
{
    //连接成功后调出的函数，需要暂时弹出errorwindow告知用户连接成功


//    qDebug()<<"书不能席以";

    error.show();
    //房间界面
    //输入房间号和用户名
    //send
}
void ClientWindow::displayError()
{
    //
}
void ClientWindow::receive(NetworkData w)
{
    if(w.op==OPCODE::JOIN_ROOM_OP)
    {
        //新玩家进入房间，data1为用户名，状态为0
        //将新玩家存入数组
        //更新房间界面
        room->single_update(w);
    }
    else if(w.op==OPCODE::JOIN_ROOM_REPLY_OP)
    {
        //成功加入房间
        //data1为已加入的用户名，data2为状态
        //将多个老玩家存入数组
        //把自己存入数组
        //更新房间界面
//        qDebug()<<"12";
        room->updateInform(w);

    }
    else if(w.op==OPCODE::LEAVE_ROOM_OP)
    {
        //用户离开房间
        //data1为用户名
        //删除数组中对应的玩家
        //更新房间界面
        room->Dele(w);
    }
    else if(w.op==OPCODE::CLOSE_ROOM_OP)
    {
        //没说干啥的
    }
    else if(w.op==OPCODE::PLAYER_READY_OP)
    {
        //data1为用户名
        //将用户名对应的状态数组改为1
        //更新房间界面
        room->Ready_change(w);
    }
    else if(w.op==OPCODE::START_GAME_OP)
    {

        single_mode = false;
        room->close();
        room->Loc_me();
        lab->setText(PlayerName);
        this->pattern = room->Player_num;
        myArea = loc_Area();
        this->markstone();  //转棋盘，变换坐标的编号
        this->show_former();
        if(pattern == 2)   //转棋盘，变换坐标
        {
            if(me == 1)   //如果是绿棋，则按照正常的染色规则
            {
                green_init_cvrt_Loc();
                green_init_cvrt_st();

            }
            if(me == 0)  //如果是紫棋，则旋转180度
            {
                green_init_cvrt_Loc();
                m2_purple_init_cvrt_st();


                for(int i =0;i<121;i++)
                {
                    l[i].cv_x*=-1;
                    l[i].cv_y*=-1;
                }
            }
        }
        else if(pattern == 3)
        {
            if(me == 0)
            {
                green_init_cvrt_Loc();
                green_init_cvrt_st();


                for(int i =0;i<121;i++)
                {
                    l[i].cv_x*=-1;
                    l[i].cv_y*=-1;
                }
            }
            else if(me == 1)
            {
                red_init_cvrt_Loc();

            }
            else if(me == 2)
            {
                blue_init_cvrt_Loc();
            }
        }
        else if(pattern == 6)
        {
            if(me == 0)
            {
                green_init_cvrt_Loc();
                green_init_cvrt_st();


                for(int i =0;i<121;i++)
                {
                    l[i].cv_x*=-1;
                    l[i].cv_y*=-1;
                }
            }
            else if(me == 1)
            {
                blue_init_cvrt_Loc();
                for(int i =0;i<121;i++)
                {
                    l[i].cv_x*=-1;
                    l[i].cv_y*=-1;
                }
            }
            else if(me == 2)
            {
                red_init_cvrt_Loc();

            }
            else if(me == 3)
            {
                green_init_cvrt_Loc();
                green_init_cvrt_st();
            }
            else if(me == 4)
            {
                blue_init_cvrt_Loc();

            }
            else if(me == 5)
            {
                red_init_cvrt_Loc();
                for(int i =0;i<121;i++)
                {
                    l[i].cv_x*=-1;
                    l[i].cv_y*=-1;
                }
            }
        }
        update();
//        state = -1;     //联机测试修改1
        init_occupied();   //必须要在确定pattern之后才能更新occupied数组的状态
        this->show();
        time_id3_clock_down = startTimer(1000);

    }
    else if(w.op==OPCODE::START_TURN_OP)
    {
//        if(turnning)   //保证第一次的turn不会轮转
//        {


//            state++;
//            state%=pattern;
//            for(int i=0;i<failed_num;i++)      //轮转修改1
//            {
//                if(state == fall_players[i])
//                {
//                    state++;
//                    state%=pattern;
//                }
//            }

//            for(int i=0;i<win_num;i++)
//            {
//                if(state == win_player[i])
//                {
//                    state++;
//                    state%=pattern;
//                }
//            }

//            this->update();      //background


//        }

//        turnning = true;





        std::time_t t = std::time(0);
        send_time = w.data2.toLong();         //-----6.27保命
        me_time = t;
        restart_time();




    }
    else if(w.op==OPCODE::MOVE_OP)
    {
        //data1为玩家初始区域
        //data2为旗子轨迹
        //更新棋盘
//        if(me == 0)
//            qDebug()<<w.data2<<"绿棋路径";
         move_op(w);

    }
    else if(w.op==OPCODE::END_TURN_OP)
    {
        //弹出获胜窗口（新窗口）
        WinWindow * win = new WinWindow;
        win->show();
    }
    else if(w.op==OPCODE::END_GAME_OP)
    {
        //游戏结束
        //data1为玩家排名列表，弹出窗口显示（新窗口）
        //回到房间界面，清空所有数组，重新输入加入房间
        game_win(w);
        socket->bye();
        this->close();
    }
    else if(w.op==OPCODE::ERROR_OP)
    {
        if(w.data1=="NOT_IN_ROOM")
        {
            //加入房间失败。可能由于用户名冲突引起
            //重新进入加房间界面
        }
        else if(w.data1=="ROOM_IS_RUNNING")
        {
            //棋子移动非法
            //重新发送mode_up
        }
        else if(w.data1=="ROOM_NOT_RUNNING")
        {
            //无法解析该请求
        }
        else if(w.data1=="INVALID_JOIN")
        {
            //当前用户不在房间内
        }
        else if(w.data1=="OUTTURN_MOVE")
        {
            //当前不在自己行动的轮次
        }
        else if(w.data1=="INVALID_MOVE")
        {
            //当前房间内的游戏正在进行
        }
        else if(w.data1=="INVALID_REQ")
        {
            //当前房间内没有正在进行的游戏
        }
        else if(w.data1=="OTHER_ERROR")
        {
            //前面没有提及的其他错误
        }
    }

}

void ClientWindow::game_win(NetworkData data)
{
    RankingList * list = new RankingList;
    list->Rank(data);
    list->show();
}

void ClientWindow::mouseReleaseEvent(QMouseEvent *ev)
{


    if(me != state)
        return ;
    QPoint pt=ev->pos();
    int B_loc;
    bool clk=getPos(pt,B_loc);
    if(clk==false&&_selected_id!=-1)
    {
        _selected_id=-1;

        update();
        return ;
    }
    if(clk==false)
    {

        return ;
    }
    int clickid=-1;
    int i;


    //三种情况的选择
    if(pattern==6) //在六个棋子的位置中搜索        //一号bug位
    {
        for(i=0;i<60;i++)
        {
            if(s[i].x0==l[B_loc].x1&&s[i].y0==l[B_loc].y1)
                break ;
        }
        if(i<60)
        {


            if(i>=0&&i<10&&(state%pattern==5))
            {
                clickid=i;

            }
            if(i>=10&&i<20&&(state%pattern==0))
            {
                clickid=i;

            }
            if(i>=20&&i<30&&(state%pattern==1))
            {
                clickid=i;

            }
            if(i>=30&&i<40&&(state%pattern==2))
            {
                clickid=i;

            }
            if(i>=40&&i<50&&(state%pattern==3))
            {
                clickid=i;

            }
            if(i>=50&&i<60&&(state%pattern==4))
            {
                clickid=i;

            }
        }
    }

    else if(pattern==3)  //在三种棋子的位置中搜索    //三号bug位_____________________________________________________________________
    {
        int cnt1=0;
        for(i=10;i<20;i++)
        {

            if(s[i].x0==l[B_loc].x1&&s[i].y0==l[B_loc].y1)

            {
                cnt1=1;
                break;
            }

        }
        if(!cnt1)
        {
            for(i=30;i<40;i++)
            {

                if(s[i].x0==l[B_loc].x1&&s[i].y0==l[B_loc].y1)
                {
                    cnt1=1;
                    break;
                }
            }
        }
        if(!cnt1)
        {
            for(i=50;i<60;i++)
            {

                if(s[i].x0==l[B_loc].x1&&s[i].y0==l[B_loc].y1)
                {
                    cnt1=1;
                    break;
                }
            }
        }
        if(cnt1)
        {

            if(i>=10&&i<20&&(state%pattern==0))
            {
                clickid=i;

            }
            if(i>=30&&i<40&&(state%pattern==1))
            {
                clickid=i;

            }
            if(i>=50&&i<60&&(state%pattern==2))
            {
                clickid=i;

            }
        }
    }

    else //在两种棋子的位置中搜索
    {
        int cnt1=0;
        for(i=10;i<20;i++)
        {

            if(s[i].x0==l[B_loc].x1&&s[i].y0==l[B_loc].y1)

            {
                cnt1=1;
                break;
            }

        }
        if(!cnt1)
        {
            for(i=40;i<50;i++)
            {

                if(s[i].x0==l[B_loc].x1&&s[i].y0==l[B_loc].y1)

                {
                    cnt1=1;
                    break;
                }

            }
        }
        if(cnt1)
        {

            if(i>=10&&i<20&&(state%pattern==0))
            {
                clickid=i;

            }
            if(i>=40&&i<50&&(state%pattern==1))
            {
                clickid=i;

            }
        }
    }


    bool continue_jump = false;
    NetworkData ct_j(OPCODE::MOVE_OP,"","");

    if(_selected_id==-1)
    {
        if(clickid != -1)
        {
            _selected_id=clickid;
            update();
        }
    }
    else
    {
        if(canMove(s[_selected_id].x0,s[_selected_id].y0,l[B_loc].x1,l[B_loc].y1))
        {
            if(!occupied[B_loc])
            {


                psr_mt=l[B_loc].psr;

                NetworkData data(OPCODE::MOVE_OP,myArea,psr_mt);


                if(pattern == 2)
                {
                    if(me == 0)   //条件变为processing的状态
                    {
                        continue_jump = true;
                        ct_j = data;
                    }
                    if(me == 1)    //在翻转棋盘的试验阶段先这样设计
                    {
                        continue_jump = true;
                        ct_j = data;

                    }
                }
                else if(pattern == 3)
                {
                    continue_jump = true;
                    ct_j = data;
                }
                else if(pattern == 6)
                {
                    continue_jump = true;
                    ct_j = data;
                }





            }
         }
            _selected_id=-1;

            update();
            if(state == me)
            {
                if(continue_jump)
                    move_op(ct_j);
            }


    }
}

void ClientWindow::move_op(NetworkData data)
{
    int t=1;
    for(int i=0;i<room->Player_num&&t;i++)     //确定当前发送路径的玩家的编号，以便存入对应储存前步路径的former_route
    {
        if(room->playerList[i] == get_id(data.data1))
        {

            former_route_number = i;
            former_route[i] = data.data2;
            t=0;
            break;
        }
    }
    former_route[former_route_number] = data.data2;
    char *au;
    QByteArray ba2 = data.data1.toLatin1();
    au=ba2.data();
    char ai=*au;
    Area_num=change_mark(ai);
    QString str = data.data2;
    str.remove(QRegularExpression("\\s"));
    char * ch;
    QByteArray ba = str.toLatin1(); // must
    ch=ba.data();
    change_char_to_int(ch);
//    qDebug()<<"DEBUG";
    time_id2 = startTimer(1000);
}


void ClientWindow::timerEvent(QTimerEvent *ev)
{

    if(ev->timerId() == time_id2)
    {
//        qDebug()<<"长安大道连狭斜";

//            qDebug()<<routeLength;
        if(cnt<routeLength-2)
        {
            move_Rt(route[cnt],route[cnt+1]);
            move_Rt(route[cnt+2],route[cnt+3]);
            cnt+=2;
        }
        else if(cnt == routeLength-2)
        {
            if(pattern == 2)
            {
                if(state == me&&!this->processing&&state == 0)
                {
                    NetworkData data(OPCODE::MOVE_OP,myArea,psr_mt);
                    this->socket->send(data);
                }
                if(state == me&&!this->processing&&state == 1)
                {
                    NetworkData data(OPCODE::MOVE_OP,myArea,psr_mt);
                    this->socket->send(data);
                }
            }   //最后一步走完后再发送信号，防止AI行棋因为上一步未走完而被直接过滤
            else if(pattern == 3)
            {
                if(state == me&&!this->processing)
                {
                    NetworkData data(OPCODE::MOVE_OP,myArea,psr_mt);
                    this->socket->send(data);
                }
            }
            else if(pattern == 6)
            {
                if(state == me&&!this->processing)
                {
                    NetworkData data(OPCODE::MOVE_OP,myArea,psr_mt);
                    this->socket->send(data);
                }
            }

            state++;
            state%=pattern;
            for(int i=0;i<failed_num;i++)      //轮转修改1
            {
                if(state == fall_players[i])
                {
                    state++;
                    state%=pattern;
                }
            }

            for(int i=0;i<win_num;i++)
            {
                if(state == win_player[i])
                {
                    state++;
                    state%=pattern;
                }
            }

            this->update();      //background

            cnt+=2;

//            return ;
        }
    }
    else if(ev->timerId() == time_id3_clock_down)
    {
        if(!count_down||count_down<0)
        {

            if_overtime = true;
            qDebug()<<state<<"！@#￥% 更卷西兴暮雨来 %￥#@！";
            wipe_player(state%pattern);
            update();


            state++;
            state = state%pattern;



            for(int i=0;i<failed_num;i++)      //轮转修改1
            {
                if(state == fall_players[i])
                {
                    state++;
                    state%=pattern;
                }
            }
            for(int i=0;i<win_num;i++)      //轮转修改1
            {
                if(state == win_player[i])
                {
                    state++;
                    state%=pattern;
                }
            }

            this->update();    //background


//            restart_time();  //进入下一轮计时


//            std::time_t t = std::time(0);
//            QString s_t = QString::number(t,10);
//            QString zone_p = player_zone(pattern,me);
//            NetworkData d(OPCODE::START_TURN_OP,zone_p,s_t);     /* 6.27改*/
//            socket->send(d);

            return ;

        }
        else if(count_down==10)
        {
            if(me == state&&this->processing)
            {
                AI_op();
            }
            else
              scn->setText(QString::number(count_down--));
        }
        else if(count_down<=7)
        {
            if(me == state&&this->processing)
            {
                AI_op();
            }
            else
              scn->setText(QString::number(count_down--));
        }
        else
         scn->setText(QString::number(count_down--));
    }
    else if(ev->timerId() == time_AI)
    {
//        qDebug()<<"白雪却嫌春色晚"<<AI_psr_mt;
        if(cnt<routeLength-2)
        {
            move_Rt(route[cnt],route[cnt+1]);
            move_Rt(route[cnt+2],route[cnt+3]);
            cnt+=2;
        }
        else if(cnt == routeLength-2)
        {

            AI_complete = true;


            state++;
            state%=pattern;
            for(int i=0;i<failed_num;i++)      //轮转修改1
            {
                if(state == fall_players[i])
                {
                    state++;
                    state%=pattern;
                }
            }

            for(int i=0;i<win_num;i++)
            {
                if(state == win_player[i])
                {
                    state++;
                    state%=pattern;
                }
            }


            this->update();    //background


            cnt+=2;



            //            return ;
        }
    }


//    else
//        return ;
}

void ClientWindow::move_Rt(int x,int y)
{
    int B_loc;

    bool clk=getCvt(x,y,B_loc);
    if(clk==false&&_selected_id!=-1)
    {
        _selected_id=-1;
        next=false;
        update();
        return ;
    }
    if(clk==false)
    {
        next=false;
        return ;
    }
    int clickid=-1;
    int i;


    //三种情况的选择
    if(pattern==6) //在六个棋子的位置中搜索        //一号bug位
    {
        for(i=0;i<60;i++)
        {
            if(s[i].x0==l[B_loc].x1&&s[i].y0==l[B_loc].y1)
                break ;
        }
        if(i<60)
        {


            if(i>=0&&i<10&&(state%pattern==5))
            {
                clickid=i;

            }
            if(i>=10&&i<20&&(state%pattern==0))
            {
                clickid=i;

            }
            if(i>=20&&i<30&&(state%pattern==1))
            {
                clickid=i;

            }
            if(i>=30&&i<40&&(state%pattern==2))
            {
                clickid=i;

            }
            if(i>=40&&i<50&&(state%pattern==3))
            {
                clickid=i;

            }
            if(i>=50&&i<60&&(state%pattern==4))
            {
                clickid=i;

            }
        }
    }

    else if(pattern==3)  //在三种棋子的位置中搜索    //三号bug位_____________________________________________________________________
    {
        int cnt1=0;
        for(i=10;i<20;i++)
        {

            if(s[i].x0==l[B_loc].x1&&s[i].y0==l[B_loc].y1)

            {
                cnt1=1;
                break;
            }

        }
        if(!cnt1)
        {
            for(i=30;i<40;i++)
            {

                if(s[i].x0==l[B_loc].x1&&s[i].y0==l[B_loc].y1)
                {
                    cnt1=1;
                    break;
                }
            }
        }
        if(!cnt1)
        {
            for(i=50;i<60;i++)
            {

                if(s[i].x0==l[B_loc].x1&&s[i].y0==l[B_loc].y1)
                {
                    cnt1=1;
                    break;
                }
            }
        }
        if(cnt1)
        {

            if(i>=10&&i<20&&(state%pattern==0))
            {
                clickid=i;

            }
            if(i>=30&&i<40&&(state%pattern==1))
            {
                clickid=i;

            }
            if(i>=50&&i<60&&(state%pattern==2))
            {
                clickid=i;

            }
        }
    }

    else if(pattern==2) //在两种棋子的位置中搜索
    {
        int cnt1=0;
        for(i=10;i<20;i++)
        {

            if(s[i].x0==l[B_loc].x1&&s[i].y0==l[B_loc].y1)

            {
                cnt1=1;
                break;
            }

        }
        if(!cnt1)
        {
            for(i=40;i<50;i++)
            {

                if(s[i].x0==l[B_loc].x1&&s[i].y0==l[B_loc].y1)

                {
                    cnt1=1;
                    break;
                }

            }
        }
        if(cnt1)
        {

            if(i>=10&&i<20&&(state%pattern==0))
            {
                clickid=i;

            }
            if(i>=40&&i<50&&(state%pattern==1))
            {
                clickid=i;

            }
        }
    }




    if(_selected_id==-1)
    {
        if(clickid != -1)
        {
            _selected_id=clickid;
            update();
        }
    }
    else
    {
        if(canMove(s[_selected_id].x0,s[_selected_id].y0,l[B_loc].x1,l[B_loc].y1))
        {
            if(!occupied[B_loc])
            {
                int i;
                next=true;

                occupied[B_loc]=true;
                getPos(s[_selected_id].x0,s[_selected_id].y0,i);
                occupied[i] = false;
                s[_selected_id].x0=l[B_loc].x1;
                s[_selected_id].y0=l[B_loc].y1;
                s[_selected_id].cv_xs = l[B_loc].cv_x;
                s[_selected_id].cv_ys = l[B_loc].cv_y;
                current_x=s[_selected_id].x0;  //此行及下面一行标记当前选中棋子，防止被wipe_dbfs()覆盖
                current_y=s[_selected_id].y0;


            }
         }
            _selected_id=-1;

            update(); //注意这两行代码，后期可能会有错误出现


    }

}

void ClientWindow::restart_time()
{
    count_down = 15-(me_time-send_time);
    time_id3_clock_down = startTimer(1000);
}

void ClientWindow::wipe_player(int id)
{
    int cnt = 0 ;
    if(pattern == 2)
    {
        if(id == 0)
        {
            for(int i=10;i<20;i++)
            {
                int t;
                getPos(s[i].x0,s[i].y0,t);
                occupied[t] = false;
                s[i].x0 = -100;
                s[i].y0 = -100;
                wipe_stones[cnt++] = t;
            }
        }
        else if(id == 1)
        {
            for(int i=40;i<50;i++)
            {
                int t;
                getPos(s[i].x0,s[i].y0,t);
                occupied[t] = false;
                s[i].x0 = -100;
                s[i].y0 = -100;
                wipe_stones[cnt++] = t;
            }
        }
    }
    else if(pattern == 3)
    {
        if(id == 0)
        {
            for(int i=10;i<20;i++)
            {
                int t;
                getPos(s[i].x0,s[i].y0,t);
                occupied[t] = false;
                s[i].x0 = -100;
                s[i].y0 = -100;
                wipe_stones[cnt++] = t;
            }
        }
        else if(id == 1)
        {
            for(int i=30;i<40;i++)
            {
                int t;
                getPos(s[i].x0,s[i].y0,t);
                occupied[t] = false;
                s[i].x0 = -100;
                s[i].y0 = -100;
                wipe_stones[cnt++] = t;
            }
        }
        else if(id == 2)
        {
            for(int i=50;i<60;i++)
            {
                int t;
                getPos(s[i].x0,s[i].y0,t);
                occupied[t] = false;
                s[i].x0 = -100;
                s[i].y0 = -100;
                wipe_stones[cnt++] = t;
            }
        }
    }
    else if(pattern == 6)
    {
        if(id == 0)
        {
            for(int i=10;i<20;i++)
            {
                int t;
                getPos(s[i].x0,s[i].y0,t);
                occupied[t] = false;
                s[i].x0 = -100;
                s[i].y0 = -100;
                wipe_stones[cnt++] = t;
            }
        }
        else if(id == 1)
        {
            for(int i=20;i<30;i++)
            {
                int t;
                getPos(s[i].x0,s[i].y0,t);
                occupied[t] = false;
                s[i].x0 = -100;
                s[i].y0 = -100;
                wipe_stones[cnt++] = t;
            }
        }
        else if(id == 2)
        {
            for(int i=30;i<40;i++)
            {
                int t;
                getPos(s[i].x0,s[i].y0,t);
                occupied[t] = false;
                s[i].x0 = -100;
                s[i].y0 = -100;
                wipe_stones[cnt++] = t;
            }
        }
        else if(id == 3)
        {
            for(int i=40;i<50;i++)
            {
                int t;
                getPos(s[i].x0,s[i].y0,t);
                occupied[t] = false;
                s[i].x0 = -100;
                s[i].y0 = -100;
                wipe_stones[cnt++] = t;
            }
        }
        else if(id == 4)
        {
            for(int i=50;i<60;i++)
            {
                int t;
                getPos(s[i].x0,s[i].y0,t);
                occupied[t] = false;
                s[i].x0 = -100;
                s[i].y0 = -100;
                wipe_stones[cnt++] = t;
            }
        }
        else if(id == 5)
        {
            for(int i=0;i<9;i++)
            {
                int t;
                getPos(s[i].x0,s[i].y0,t);
                occupied[t] = false;
                s[i].x0 = -100;
                s[i].y0 = -100;
                wipe_stones[cnt++] = t;
            }
        }
    }
    fall_players[failed_num++] = id;
}

void ClientWindow::change_char_to_int(char *p)
{
//    qDebug()<<"change_char_to_int";
    memset(route,0,sizeof(route));
//    next=true;
    routeLength=0;
    cnt=0;




//    std::time_t t = std::time(0);
//    QString s_t = QString::number(t,10);
//    QString zone_p = player_zone(pattern,me);
//    NetworkData d(OPCODE::START_TURN_OP,zone_p,s_t);     /*6.27改*/
//    socket->send(d);





    int mo=0; // 这是整形数组的下标，字符数组会有负号，如果和字符数组共享下标，在跳过负号的时候就会留下空挡
    int len = strlen(p);
        int i;
        for (i = 0; i < len;)
        {
            if (p[i] >= '0' && p[i] <= '9')
            {
                route[mo] = p[i] - '0';
                i++;
                mo++;
            }
            else if (p[i] == '-')
            {
                route[mo] = p[i + 1] - '0';
                route[mo] *= -1;
                i += 2;
                mo++;
            }
            routeLength++;
        }
        /*print();*/
}







//此下为AI设计






void ClientWindow::clear_score()
{
    for(int i=0;i<121;i++)
    {
        if(l[i].score)
            l[i].score = 0;
    }
}

void ClientWindow::AI_op()
{
    if(pattern == 2)
    {
        if(me == 1)
        {
            QString poo = "";

            for(int i=40;i<50;i++)
            {
                int tmp;
                getCvt(s[i].cv_xs,s[i].cv_ys,tmp);
                dfs_markSingle(0,tmp,i,poo);
                wipe_AIPossible();

//                qDebug()<<sumDist<<" "<<"0号玩家检查分数"<<i<<s[i].cv_xs<<s[i].cv_ys;

                poo.clear();
                mozi = -1;
                mozi_dis = -10000;
            }

            AI_move_Rt(AI_psr_mt);

            NetworkData data(OPCODE::MOVE_OP,myArea,AI_psr_mt);
            this->socket->send(data);

            sumDist = -100000000;    //更新总分，为下一个状态的计算做准备
        }
        if(me == 0)
        {
            QString poo = "";

//            for(int i=10;i<20;i++)
//            {
//                qDebug()<<s[i].cv_xs<<" "<<s[i].cv_ys;
//            }


            for(int i=10;i<20;i++)
            {
                int tmp;
                getPos(s[i].x0,s[i].y0,tmp);
                dfs_markSingle(0,tmp,i,poo);
                wipe_AIPossible();



                poo.clear();
                mozi = -1;
                mozi_dis = -10000;
            }

            AI_move_Rt(AI_psr_mt);

            NetworkData data(OPCODE::MOVE_OP,myArea,AI_psr_mt);
            this->socket->send(data);

            sumDist = -100000000;
        }
    }
    else  if(pattern == 3)
    {
        if(me == 0)
        {

            QString poo = "";

//            for(int i=10;i<20;i++)
//            {
//                qDebug()<<s[i].cv_xs<<" "<<s[i].cv_ys;
//            }


            for(int i=10;i<20;i++)
            {
                int tmp;
                getPos(s[i].x0,s[i].y0,tmp);
                dfs_markSingle(0,tmp,i,poo);
                wipe_AIPossible();



                poo.clear();
                mozi = -1;
                mozi_dis = -10000;
            }

            AI_move_Rt(AI_psr_mt);

            NetworkData data(OPCODE::MOVE_OP,myArea,AI_psr_mt);
            this->socket->send(data);
            sumDist = -100000000;
        }
        else if(me == 1)
        {
            QString poo = "";

//            for(int i=10;i<20;i++)
//            {
//                qDebug()<<s[i].cv_xs<<" "<<s[i].cv_ys;
//            }


            for(int i=30;i<40;i++)
            {
                int tmp;
                getPos(s[i].x0,s[i].y0,tmp);
                dfs_markSingle(0,tmp,i,poo);
                wipe_AIPossible();



                poo.clear();
                mozi = -1;
                mozi_dis = -10000;
            }

            AI_move_Rt(AI_psr_mt);

            NetworkData data(OPCODE::MOVE_OP,myArea,AI_psr_mt);
            this->socket->send(data);

            sumDist = -100000000;
        }
        else if(me == 2)
        {
            QString poo = "";

//            for(int i=10;i<20;i++)
//            {
//                qDebug()<<s[i].cv_xs<<" "<<s[i].cv_ys;
//            }


            for(int i=50;i<60;i++)
            {
                int tmp;
                getPos(s[i].x0,s[i].y0,tmp);
                dfs_markSingle(0,tmp,i,poo);
                wipe_AIPossible();



                poo.clear();
                mozi = -1;
                mozi_dis = -10000;
            }

            AI_move_Rt(AI_psr_mt);

            NetworkData data(OPCODE::MOVE_OP,myArea,AI_psr_mt);
            this->socket->send(data);

            sumDist = -100000000;
        }

        qDebug()<<AI_psr_mt;
    }



//        NetworkData data(OPCODE::MOVE_OP,myArea,AI_psr_mt);
//        this->socket->send(data);


}

void ClientWindow::dfs_markSingle(int count,int i,int begin_point/*,double base_score*/,QString a)   //记忆路径，搜点，定最大
{
    int u=0;



    if(!getPos(l[i].x1,l[i].y1,u))  //或许可以优化？
     return ;
    QString p=a;
    if(!count)
    {
        QString s1= QString::number(l[i].cv_x,10);
        p.append(s1+" ");
        QString s2= QString::number(l[i].cv_y,10);
        p.append(s2);
    }

    int d[6][2]={{33,44},{-33,-44},{33,-44},{-33,44},{66,0},{-66,0}};
    int nx,ny;
    int interval_x,interval_y; //隔空x，y的坐标
    int pst,pst2;//pst定周边位，pst2定隔空位
    for(int j=0;j<6;j++) //探测周围六个点
    {
        nx=l[i].x1+d[j][0];
        ny=l[i].y1+d[j][1];
        if(getPos(nx,ny,pst)) //条件：在棋盘内
        {
            if(!occupied[pst]&&!count&&!Possible_Location[pst]) //判断相邻位，并且只在第一层做这个搜索————（改了这里（加了第三个条件）计算就没问题了，但是还是没有想通为什么是这里（也可能不是））
            {
                Possible_Location[pst]=true;


                //记录路径
                QString s0=p;

                QString s1=QString::number(l[pst].cv_x,10);
                QString s2=QString::number(l[pst].cv_y,10);
                s0.append(" "+s1);
                s0.append(" "+s2);

                l[pst].psr_AI[begin_point] = s0;
//                qDebug()<<l[pst].cv_x<<" "<<l[pst].cv_y;
//                qDebug()<<s0;

                //以下为判分过程
                if(pattern == 2)
                {
                    if(me == 1)
                    {
                        double t1 = s[begin_point].cv_xs;  //记录原标准坐标
                        double t2 = s[begin_point].cv_ys;
                        double t3 = s[begin_point].x0;   //记录原像素坐标
                        double t4 = s[begin_point].y0;
                        s[begin_point].cv_xs = l[pst].cv_x;
                        s[begin_point].cv_ys = l[pst].cv_y;
                        s[begin_point].x0 = l[pst].x1;
                        s[begin_point].y0 = l[pst].y1;
                        //                    double rpc_x = s[begin_point].cv_xs;
                        //                    double rpc_y = s[begin_point].cv_ys;

                        int max_s = -100000,min_s = 100000;    //1
                        for(int i=40;i<50;i++)  //寻找末子
                        {
                            if(max_s<(s[i].y0-100)/44.0)
                                max_s = (s[i].y0-100)/44.0;
                            if((s[i].y0-100)/44.0<min_s)
                            {
                                min_s = (s[i].y0-100)/44.0;
                            }
                            if(mozi_dis<(s[i].y0-100)/44.0)
                            {
                                mozi_dis = (s[i].y0-100)/44.0;
                                mozi = i;
                            }
                        }
                        double psb_score = 0;  //如果走了这步，当前的分数
                        //                    psb_score = base_score+qAbs(t1-650)/66*biasFine+(8-t2)-qAbs(rpc_x-650)/66*biasFine-(8-rpc_y);
                        for(int i=40;i<50;i++)
                        {
                            double bias = qAbs(s[i].x0-650)/66;
                            if(bias <= 3)
                                psb_score+=-bias*biasFine_light-((s[i].y0-100)/44.0)*straight_score;
                            else
                                psb_score+=-bias*biasFine_serious-((s[i].y0-100)/44.0)*straight_score;
                        }
                        double moscore = 0;   //2
                        if(max_s - min_s>10)    //3
                            moscore = 100000;
                        else
                            moscore = mozi_dis*lastchessFine;
                        if(spc_st())
                            psb_score = 100000000;
                        else if(win_stu())
                            psb_score = 1000000000;
                        else
                          psb_score+=-moscore;
//                        qDebug()<<psb_score;
//                        if(win_stu())
//                            psb_score = 200000000;

                        if(psb_score>sumDist)
                        {
                            sumDist = psb_score;
//                            father_fnl = begin_serial;
//                            fnl = pst;
                            AI_psr_mt = l[pst].psr_AI[begin_point];   //确定了最大分数，继承路径
                        }

                        s[begin_point].cv_xs=t1;
                        s[begin_point].cv_ys=t2;
                        s[begin_point].x0 = t3;
                        s[begin_point].y0 = t4;
//                        mozi = -1;
//                        mozi_dis = -1;
                    }
                    if(me == 0)
                    {
                        double t1 = s[begin_point].cv_xs;  //记录原标准坐标
                        double t2 = s[begin_point].cv_ys;
                        double t3 = s[begin_point].x0;   //记录原像素坐标
                        double t4 = s[begin_point].y0;
                        s[begin_point].cv_xs = l[pst].cv_x;
                        s[begin_point].cv_ys = l[pst].cv_y;
                        s[begin_point].x0 = l[pst].x1;
                        s[begin_point].y0 = l[pst].y1;
                        //                    double rpc_x = s[begin_point].cv_xs;
                        //                    double rpc_y = s[begin_point].cv_ys;

                        int max_s = -100000,min_s = 100000;    //1
                        for(int i=10;i<20;i++)  //寻找末子
                        {
                            if(max_s<(s[i].y0-100)/44.0)
                                max_s = (s[i].y0-100)/44.0;
                            if((s[i].y0-100)/44.0<min_s)
                            {
                                min_s = (s[i].y0-100)/44.0;
                            }
                            if(mozi_dis<(s[i].y0-100)/44.0)
                            {
                                mozi_dis = (s[i].y0-100)/44.0;
                                mozi = i;
                            }
                        }
                        double psb_score = 0;  //如果走了这步，当前的分数
                        //                    psb_score = base_score+qAbs(t1-650)/66*biasFine+(8-t2)-qAbs(rpc_x-650)/66*biasFine-(8-rpc_y);
                        for(int i=10;i<20;i++)
                        {
                            double bias = qAbs(s[i].x0-650)/66;
                            if(bias <= 3)
                                psb_score+=-bias*biasFine_light-((s[i].y0-100)/44.0)*straight_score;
                            else
                                psb_score+=-bias*biasFine_serious-((s[i].y0-100)/44.0)*straight_score;
                        }
                        double moscore = 0;   //2
                        if(max_s - min_s>10)    //3
                            moscore = 100000;
                        else
                            moscore = mozi_dis*lastchessFine;
                        if(spc_st())
                            psb_score += 100000000;
                        else if(win_stu())
                            psb_score += 1000000000;
                        else
                          psb_score+=-moscore;
//                        qDebug()<<psb_score;
//                        if(win_stu())
//                            psb_score = 200000000;

                        if(psb_score>sumDist)
                        {
                            sumDist = psb_score;
//                            father_fnl = begin_serial;
//                            fnl = pst;
                            AI_psr_mt = l[pst].psr_AI[begin_point];   //确定了最大分数，继承路径
                        }

                        s[begin_point].cv_xs=t1;
                        s[begin_point].cv_ys=t2;
                        s[begin_point].x0 = t3;
                        s[begin_point].y0 = t4;
//                        mozi = -1;
//                        mozi_dis = -1;
                    }
                }
                else if(pattern == 3)
                {
                    if(me == 0)
                    {
                        double t1 = s[begin_point].cv_xs;  //记录原标准坐标
                        double t2 = s[begin_point].cv_ys;
                        double t3 = s[begin_point].x0;   //记录原像素坐标
                        double t4 = s[begin_point].y0;
                        s[begin_point].cv_xs = l[pst].cv_x;
                        s[begin_point].cv_ys = l[pst].cv_y;
                        s[begin_point].x0 = l[pst].x1;
                        s[begin_point].y0 = l[pst].y1;
                        //                    double rpc_x = s[begin_point].cv_xs;
                        //                    double rpc_y = s[begin_point].cv_ys;

                        int max_s = -100000,min_s = 100000;    //1
                        for(int i=10;i<20;i++)  //寻找末子
                        {
                            if(max_s<(s[i].y0-100)/44.0)
                                max_s = (s[i].y0-100)/44.0;
                            if((s[i].y0-100)/44.0<min_s)
                            {
                                min_s = (s[i].y0-100)/44.0;
                            }
                            if(mozi_dis<(s[i].y0-100)/44.0)
                            {
                                mozi_dis = (s[i].y0-100)/44.0;
                                mozi = i;
                            }
                        }
                        double psb_score = 0;  //如果走了这步，当前的分数
                        //                    psb_score = base_score+qAbs(t1-650)/66*biasFine+(8-t2)-qAbs(rpc_x-650)/66*biasFine-(8-rpc_y);
                        for(int i=10;i<20;i++)
                        {
                            double bias = qAbs(s[i].x0-650)/66;
                            if(bias <= 3)
                                psb_score+=-bias*biasFine_light-((s[i].y0-100)/44.0)*straight_score;
                            else
                                psb_score+=-bias*biasFine_serious-((s[i].y0-100)/44.0)*straight_score;
                        }
                        double moscore = 0;   //2
                        if(max_s - min_s>10)    //3
                            moscore = 100000;
                        else
                            moscore = mozi_dis*lastchessFine;
                        if(spc_st())
                            psb_score = 100000000;
                        else if(win_stu())
                            psb_score = 1000000000;
                        else
                          psb_score+=-moscore;
//                        qDebug()<<psb_score;
//                        if(win_stu())
//                            psb_score = 200000000;

                        if(psb_score>sumDist)
                        {
                            sumDist = psb_score;
//                            father_fnl = begin_serial;
//                            fnl = pst;
                            AI_psr_mt = l[pst].psr_AI[begin_point];   //确定了最大分数，继承路径
                        }

                        s[begin_point].cv_xs=t1;
                        s[begin_point].cv_ys=t2;
                        s[begin_point].x0 = t3;
                        s[begin_point].y0 = t4;
//                        mozi = -1;
//                        mozi_dis = -1;
                    }
                    else if(me == 1)
                    {
                        double t1 = s[begin_point].cv_xs;  //记录原标准坐标
                        double t2 = s[begin_point].cv_ys;
                        double t3 = s[begin_point].x0;   //记录原像素坐标
                        double t4 = s[begin_point].y0;
                        s[begin_point].cv_xs = l[pst].cv_x;
                        s[begin_point].cv_ys = l[pst].cv_y;
                        s[begin_point].x0 = l[pst].x1;
                        s[begin_point].y0 = l[pst].y1;
                        //                    double rpc_x = s[begin_point].cv_xs;
                        //                    double rpc_y = s[begin_point].cv_ys;

                        int max_s = -100000,min_s = 100000;    //1
                        for(int i=30;i<40;i++)  //寻找末子
                        {
                            if(max_s<(s[i].y0-100)/44.0)
                                max_s = (s[i].y0-100)/44.0;
                            if((s[i].y0-100)/44.0<min_s)
                            {
                                min_s = (s[i].y0-100)/44.0;
                            }
                            if(mozi_dis<(s[i].y0-100)/44.0)
                            {
                                mozi_dis = (s[i].y0-100)/44.0;
                                mozi = i;
                            }
                        }
                        double psb_score = 0;  //如果走了这步，当前的分数
                        //                    psb_score = base_score+qAbs(t1-650)/66*biasFine+(8-t2)-qAbs(rpc_x-650)/66*biasFine-(8-rpc_y);
                        for(int i=30;i<40;i++)
                        {
                            double bias = qAbs(s[i].x0-650)/66;
                            if(bias <= 3)
                                psb_score+=-bias*biasFine_light-((s[i].y0-100)/44.0)*straight_score;
                            else
                                psb_score+=-bias*biasFine_serious-((s[i].y0-100)/44.0)*straight_score;
                        }
                        double moscore = 0;   //2
                        if(max_s - min_s>10)    //3
                            moscore = 100000;
                        else
                            moscore = mozi_dis*lastchessFine;
                        if(spc_st())
                            psb_score = 100000000;
                        else if(win_stu())
                            psb_score = 1000000000;
                        else
                          psb_score+=-moscore;
//                        qDebug()<<psb_score;
//                        if(win_stu())
//                            psb_score = 200000000;

                        if(psb_score>sumDist)
                        {
                            sumDist = psb_score;
//                            father_fnl = begin_serial;
//                            fnl = pst;
                            AI_psr_mt = l[pst].psr_AI[begin_point];   //确定了最大分数，继承路径
                        }

                        s[begin_point].cv_xs=t1;
                        s[begin_point].cv_ys=t2;
                        s[begin_point].x0 = t3;
                        s[begin_point].y0 = t4;
//                        mozi = -1;
//                        mozi_dis = -1;
                    }
                    else if(me == 2)
                    {
                        double t1 = s[begin_point].cv_xs;  //记录原标准坐标
                        double t2 = s[begin_point].cv_ys;
                        double t3 = s[begin_point].x0;   //记录原像素坐标
                        double t4 = s[begin_point].y0;
                        s[begin_point].cv_xs = l[pst].cv_x;
                        s[begin_point].cv_ys = l[pst].cv_y;
                        s[begin_point].x0 = l[pst].x1;
                        s[begin_point].y0 = l[pst].y1;
                        //                    double rpc_x = s[begin_point].cv_xs;
                        //                    double rpc_y = s[begin_point].cv_ys;

                        int max_s = -100000,min_s = 100000;    //1
                        for(int i=50;i<60;i++)  //寻找末子
                        {
                            if(max_s<(s[i].y0-100)/44.0)
                                max_s = (s[i].y0-100)/44.0;
                            if((s[i].y0-100)/44.0<min_s)
                            {
                                min_s = (s[i].y0-100)/44.0;
                            }
                            if(mozi_dis<(s[i].y0-100)/44.0)
                            {
                                mozi_dis = (s[i].y0-100)/44.0;
                                mozi = i;
                            }
                        }
                        double psb_score = 0;  //如果走了这步，当前的分数
                        //                    psb_score = base_score+qAbs(t1-650)/66*biasFine+(8-t2)-qAbs(rpc_x-650)/66*biasFine-(8-rpc_y);
                        for(int i=50;i<60;i++)
                        {
                            double bias = qAbs(s[i].x0-650)/66;
                            if(bias <= 3)
                                psb_score+=-bias*biasFine_light-((s[i].y0-100)/44.0)*straight_score;
                            else
                                psb_score+=-bias*biasFine_serious-((s[i].y0-100)/44.0)*straight_score;
                        }
                        double moscore = 0;   //2
                        if(max_s - min_s>10)    //3
                            moscore = 100000;
                        else
                            moscore = mozi_dis*lastchessFine;
                        if(spc_st())
                            psb_score = 100000000;
                        else if(win_stu())
                            psb_score = 1000000000;
                        else
                          psb_score+=-moscore;
//                        qDebug()<<psb_score;
//                        if(win_stu())
//                            psb_score = 200000000;

                        if(psb_score>sumDist)
                        {
                            sumDist = psb_score;
//                            father_fnl = begin_serial;
//                            fnl = pst;
                            AI_psr_mt = l[pst].psr_AI[begin_point];   //确定了最大分数，继承路径
                        }

                        s[begin_point].cv_xs=t1;
                        s[begin_point].cv_ys=t2;
                        s[begin_point].x0 = t3;
                        s[begin_point].y0 = t4;
//                        mozi = -1;
//                        mozi_dis = -1;
                    }
                }


            }
            if(occupied[pst]) //条件：在棋盘内——判断隔空位
            {
                interval_x=2*l[pst].x1-l[i].x1;  //隔空位的x坐标
                interval_y=2*l[pst].y1-l[i].y1;  //隔空位的y坐标
                if(getPos(interval_x,interval_y,pst2))
                {
                    if(!occupied[pst2]&&!Possible_Location[pst2]) //第二个条件是为了不重复标记
                    {
                        Possible_Location[pst2]=true; //可能位置标记

                        //记录路径
                        QString s0=p;

                        QString s1=QString::number(l[pst2].cv_x,10);
                        QString s2=QString::number(l[pst2].cv_y,10);
                        s0.append(" "+s1);
                        s0.append(" "+s2);
                        l[pst2].psr_AI[begin_point]=s0;

//                        qDebug()<<"LOL_TIM";

//                        qDebug()<<l[pst2].cv_x<<" "<<l[pst2].cv_y;
//                        qDebug()<<s0;

                        //以下为判分过程
                        if(pattern == 2)
                        {
                            if(me==1)
                            {
                                double t1 = s[begin_point].cv_xs;  //记录原标准坐标
                                double t2 = s[begin_point].cv_ys;
                                double t3 = s[begin_point].x0;   //记录原像素坐标
                                double t4 = s[begin_point].y0;
                                s[begin_point].cv_xs = l[pst2].cv_x;
                                s[begin_point].cv_ys = l[pst2].cv_y;
                                s[begin_point].x0 = l[pst2].x1;
                                s[begin_point].y0 = l[pst2].y1;


                                int max_s = -100000,min_s = 100000;
                                for(int i=40;i<50;i++)  //寻找末子
                                {
                                    if(max_s<(s[i].y0-100)/44.0)
                                        max_s = (s[i].y0-100)/44.0;
                                    if((s[i].y0-100)/44.0<min_s)
                                    {
                                        min_s = (s[i].y0-100)/44.0;     //记录棋子的最大间距
                                    }
                                    if(mozi_dis<(s[i].y0-100)/44.0)
                                    {
                                        mozi_dis = (s[i].y0-100)/44.0;
                                        mozi = i;
                                    }
                                }


                                double psb_score = 0;  //如果走了这步，当前的分数
                                for(int i=40;i<50;i++)
                                {
                                    double bias = qAbs(s[i].x0-650)/66;
                                    if(bias <= 3)
                                        psb_score+=-bias*biasFine_light-((s[i].y0-100)/44.0)*straight_score;
                                    else
                                        psb_score+=-bias*biasFine_serious-((s[i].y0-100)/44.0)*straight_score;
                                }
                                double moscore = 0;   //2
                                if(max_s - min_s>10)    //3
                                    moscore = 100000;
                                else
                                    moscore = mozi_dis*lastchessFine;
                                if(spc_st())
                                    psb_score = 100000000;
                                else if(win_stu())
                                    psb_score = 1000000000;
                                else
                                  psb_score+=-moscore;

//                                qDebug()<<psb_score;

                                if(psb_score>sumDist)
                                {
                                    sumDist = psb_score;
//                                    father_fnl = begin_serial;
//                                    fnl = pst;
                                    AI_psr_mt = l[pst2].psr_AI[begin_point];      //确定了最大分数，继承路径
                                }

                                s[begin_point].cv_xs=t1;
                                s[begin_point].cv_ys=t2;
                                s[begin_point].x0 = t3;
                                s[begin_point].y0 = t4;
//                                mozi = -1;
//                                mozi_dis = -1;
                            }
                            if(me == 0)
                            {
                                double t1 = s[begin_point].cv_xs;  //记录原标准坐标
                                double t2 = s[begin_point].cv_ys;
                                double t3 = s[begin_point].x0;   //记录原像素坐标
                                double t4 = s[begin_point].y0;
                                s[begin_point].cv_xs = l[pst2].cv_x;
                                s[begin_point].cv_ys = l[pst2].cv_y;
                                s[begin_point].x0 = l[pst2].x1;
                                s[begin_point].y0 = l[pst2].y1;


                                int max_s = -100000,min_s = 100000;
                                for(int i = 10;i<20;i++)  //寻找末子
                                {
                                    if(max_s<(s[i].y0-100)/44.0)
                                        max_s = (s[i].y0-100)/44.0;
                                    if((s[i].y0-100)/44.0<min_s)
                                    {
                                        min_s = (s[i].y0-100)/44.0;     //记录棋子的最大间距
                                    }
                                    if(mozi_dis<(s[i].y0-100)/44.0)
                                    {
                                        mozi_dis = (s[i].y0-100)/44.0;
                                        mozi = i;
                                    }
                                }


                                double psb_score = 0;  //如果走了这步，当前的分数
                                for(int i=10;i<20;i++)
                                {
                                    double bias = qAbs(s[i].x0-650)/66;
                                    if(bias <= 3)
                                        psb_score+=-bias*biasFine_light-((s[i].y0-100)/44.0)*straight_score;
                                    else
                                        psb_score+=-bias*biasFine_serious-((s[i].y0-100)/44.0)*straight_score;
                                }
                                double moscore = 0;   //2
                                if(max_s - min_s>10)    //3
                                    moscore = 100000;
                                else
                                    moscore = mozi_dis*lastchessFine;
                                if(spc_st())
                                    psb_score = 100000000;
                                else if(win_stu())
                                {
                                    psb_score = 1000000000;
                                    qDebug()<<"juius";
                                }
                                else
                                  psb_score+=-moscore;

                                qDebug()<<psb_score;

                                if(psb_score>sumDist)
                                {
                                    sumDist = psb_score;
//                                    father_fnl = begin_serial;
//                                    fnl = pst;
                                    AI_psr_mt = l[pst2].psr_AI[begin_point];      //确定了最大分数，继承路径
                                }

                                s[begin_point].cv_xs=t1;
                                s[begin_point].cv_ys=t2;
                                s[begin_point].x0 = t3;
                                s[begin_point].y0 = t4;
//                                mozi = -1;
//                                mozi_dis = -1;
                            }


                            dfs_markSingle(count+1,pst2,begin_point/*,base_score*/,l[pst2].psr_AI[begin_point]); //深度优先搜索
                        }
                        else if(pattern == 3)
                        {
                            if(me == 0)
                            {
                                double t1 = s[begin_point].cv_xs;  //记录原标准坐标
                                double t2 = s[begin_point].cv_ys;
                                double t3 = s[begin_point].x0;   //记录原像素坐标
                                double t4 = s[begin_point].y0;
                                s[begin_point].cv_xs = l[pst2].cv_x;
                                s[begin_point].cv_ys = l[pst2].cv_y;
                                s[begin_point].x0 = l[pst2].x1;
                                s[begin_point].y0 = l[pst2].y1;


                                int max_s = -100000,min_s = 100000;
                                for(int i=10;i<20;i++)  //寻找末子
                                {
                                    if(max_s<(s[i].y0-100)/44.0)
                                        max_s = (s[i].y0-100)/44.0;
                                    if((s[i].y0-100)/44.0<min_s)
                                    {
                                        min_s = (s[i].y0-100)/44.0;     //记录棋子的最大间距
                                    }
                                    if(mozi_dis<(s[i].y0-100)/44.0)
                                    {
                                        mozi_dis = (s[i].y0-100)/44.0;
                                        mozi = i;
                                    }
                                }


                                double psb_score = 0;  //如果走了这步，当前的分数
                                for(int i=10;i<20;i++)
                                {
                                    double bias = qAbs(s[i].x0-650)/66;
                                    if(bias <= 3)
                                        psb_score+=-bias*biasFine_light-((s[i].y0-100)/44.0)*straight_score;
                                    else
                                        psb_score+=-bias*biasFine_serious-((s[i].y0-100)/44.0)*straight_score;
                                }
                                double moscore = 0;   //2
                                if(max_s - min_s>10)    //3
                                    moscore = 100000;
                                else
                                    moscore = mozi_dis*lastchessFine;
                                if(spc_st())
                                    psb_score = 100000000;
                                else if(win_stu())
                                    psb_score = 1000000000;
                                else
                                  psb_score+=-moscore;

//                                qDebug()<<psb_score;

                                if(psb_score>sumDist)
                                {
                                    sumDist = psb_score;
//                                    father_fnl = begin_serial;
//                                    fnl = pst;
                                    AI_psr_mt = l[pst2].psr_AI[begin_point];      //确定了最大分数，继承路径
                                }

                                s[begin_point].cv_xs=t1;
                                s[begin_point].cv_ys=t2;
                                s[begin_point].x0 = t3;
                                s[begin_point].y0 = t4;
//                                mozi = -1;
//                                mozi_dis = -1;
                            }
                            else if(me == 1)
                            {
                                double t1 = s[begin_point].cv_xs;  //记录原标准坐标
                                double t2 = s[begin_point].cv_ys;
                                double t3 = s[begin_point].x0;   //记录原像素坐标
                                double t4 = s[begin_point].y0;
                                s[begin_point].cv_xs = l[pst2].cv_x;
                                s[begin_point].cv_ys = l[pst2].cv_y;
                                s[begin_point].x0 = l[pst2].x1;
                                s[begin_point].y0 = l[pst2].y1;


                                int max_s = -100000,min_s = 100000;
                                for(int i=30;i<40;i++)  //寻找末子
                                {
                                    if(max_s<(s[i].y0-100)/44.0)
                                        max_s = (s[i].y0-100)/44.0;
                                    if((s[i].y0-100)/44.0<min_s)
                                    {
                                        min_s = (s[i].y0-100)/44.0;     //记录棋子的最大间距
                                    }
                                    if(mozi_dis<(s[i].y0-100)/44.0)
                                    {
                                        mozi_dis = (s[i].y0-100)/44.0;
                                        mozi = i;
                                    }
                                }


                                double psb_score = 0;  //如果走了这步，当前的分数
                                for(int i=30;i<40;i++)
                                {
                                    double bias = qAbs(s[i].x0-650)/66;
                                    if(bias <= 3)
                                        psb_score+=-bias*biasFine_light-((s[i].y0-100)/44.0)*straight_score;
                                    else
                                        psb_score+=-bias*biasFine_serious-((s[i].y0-100)/44.0)*straight_score;
                                }
                                double moscore = 0;   //2
                                if(max_s - min_s>10)    //3
                                    moscore = 100000;
                                else
                                    moscore = mozi_dis*lastchessFine;
                                if(spc_st())
                                    psb_score = 100000000;
                                else if(win_stu())
                                    psb_score = 1000000000;
                                else
                                  psb_score+=-moscore;

//                                qDebug()<<psb_score;

                                if(psb_score>sumDist)
                                {
                                    sumDist = psb_score;
//                                    father_fnl = begin_serial;
//                                    fnl = pst;
                                    AI_psr_mt = l[pst2].psr_AI[begin_point];      //确定了最大分数，继承路径
                                }

                                s[begin_point].cv_xs=t1;
                                s[begin_point].cv_ys=t2;
                                s[begin_point].x0 = t3;
                                s[begin_point].y0 = t4;
//                                mozi = -1;
//                                mozi_dis = -1;
                            }
                            else if(me == 2)
                            {
                                double t1 = s[begin_point].cv_xs;  //记录原标准坐标
                                double t2 = s[begin_point].cv_ys;
                                double t3 = s[begin_point].x0;   //记录原像素坐标
                                double t4 = s[begin_point].y0;
                                s[begin_point].cv_xs = l[pst2].cv_x;
                                s[begin_point].cv_ys = l[pst2].cv_y;
                                s[begin_point].x0 = l[pst2].x1;
                                s[begin_point].y0 = l[pst2].y1;


                                int max_s = -100000,min_s = 100000;
                                for(int i=50;i<60;i++)  //寻找末子
                                {
                                    if(max_s<(s[i].y0-100)/44.0)
                                        max_s = (s[i].y0-100)/44.0;
                                    if((s[i].y0-100)/44.0<min_s)
                                    {
                                        min_s = (s[i].y0-100)/44.0;     //记录棋子的最大间距
                                    }
                                    if(mozi_dis<(s[i].y0-100)/44.0)
                                    {
                                        mozi_dis = (s[i].y0-100)/44.0;
                                        mozi = i;
                                    }
                                }


                                double psb_score = 0;  //如果走了这步，当前的分数
                                for(int i=50;i<60;i++)
                                {
                                    double bias = qAbs(s[i].x0-650)/66;
                                    if(bias <= 3)
                                        psb_score+=-bias*biasFine_light-((s[i].y0-100)/44.0)*straight_score;
                                    else
                                        psb_score+=-bias*biasFine_serious-((s[i].y0-100)/44.0)*straight_score;
                                }
                                double moscore = 0;   //2
                                if(max_s - min_s>10)    //3
                                    moscore = 100000;
                                else
                                    moscore = mozi_dis*lastchessFine;
                                if(spc_st())
                                    psb_score = 100000000;
                                else if(win_stu())
                                    psb_score = 1000000000;
                                else
                                  psb_score+=-moscore;

//                                qDebug()<<psb_score;

                                if(psb_score>sumDist)
                                {
                                    sumDist = psb_score;
//                                    father_fnl = begin_serial;
//                                    fnl = pst;
                                    AI_psr_mt = l[pst2].psr_AI[begin_point];      //确定了最大分数，继承路径
                                }

                                s[begin_point].cv_xs=t1;
                                s[begin_point].cv_ys=t2;
                                s[begin_point].x0 = t3;
                                s[begin_point].y0 = t4;
//                                mozi = -1;
//                                mozi_dis = -1;
                            }

                            dfs_markSingle(count+1,pst2,begin_point/*,base_score*/,l[pst2].psr_AI[begin_point]); //深度优先搜索

                        }

                    }
                }
            }
        }
    }
}

//void ClientWindow::No3_AI_search(int begin_point)
//{
//    for(int i=40;i<50;i++)
//    {
//        dfs_markSingle(0,i,i);
//    }
//}
void ClientWindow::wipe_AIPossible()
{
    for(int i=0;i<121;i++)
    {
          Possible_Location[i] = false ;
    }
}


void ClientWindow::wipe_AIroute()
{
    for(int i=0;i<121;i++)
    {
        for(int j=0;j<70;j++)
        {
            l[i].psr_AI[j].clear();
        }
    }
}

void ClientWindow::AI_move_Rt(QString rut)   //AI自动行棋
{
    former_route[me] = rut;                      //因为AI用的移动函数和点击以及传参的不一样，AI行棋的地方也要保留路径
    QString str = rut;
    str.remove(QRegularExpression("\\s"));
    char * ch;
    QByteArray ba = str.toLatin1(); // must
    ch=ba.data();

    change_char_to_int(ch);

    time_AI = startTimer(2000);
}


bool ClientWindow::spc_st()
{
    if(pattern == 2)
    {
        if(me == 1)
        {
            int count_spc = 0;
            for(int i=40;i<50;i++)
            {
                if((l[61].x1 == s[i].x0&&l[61].y1 == s[i].y0&&!occupied[6])||(l[63].x1 == s[i].x0&&l[63].y1 == s[i].y0&&!occupied[9]))
                    count_spc++;
            }
            for(int j=0;j<10;j++)
            {
                for(int i=40;i<50;i++)
                {
                    if(s[i].x0 == l[j].x1&&s[i].y0 == l[j].y1)
                    {
                        count_spc++;
                    }
                }
            }
            if(count_spc == 10)
                return true;
            else
                return false;
        }
        else if(me == 0)
        {
            int count_spc = 0;
            for(int i=10;i<20;i++)
            {
                if((l[61].x1 == s[i].x0&&l[61].y1 == s[i].y0&&!occupied[6])||(l[63].x1 == s[i].x0&&l[63].y1 == s[i].y0&&!occupied[9]))
                    count_spc++;
            }
            for(int j=0;j<10;j++)
            {
                for(int i=10;i<20;i++)
                {
                    if(s[i].x0 == l[j].x1&&s[i].y0 == l[j].y1)
                    {
                        count_spc++;
                    }
                }
            }
            if(count_spc == 10)
                return true;
            else
                return false;
        }
    }
    else if(pattern == 3)
    {
        if(me == 0)
        {
            int count_spc = 0;
            for(int i=10;i<20;i++)
            {
                if((l[61].x1 == s[i].x0&&l[61].y1 == s[i].y0&&!occupied[6])||(l[63].x1 == s[i].x0&&l[63].y1 == s[i].y0&&!occupied[9]))
                    count_spc++;
            }
            for(int j=0;j<10;j++)
            {
                for(int i=10;i<20;i++)
                {
                    if(s[i].x0 == l[j].x1&&s[i].y0 == l[j].y1)
                    {
                        count_spc++;
                    }
                }
            }
            if(count_spc == 10)
                return true;
            else
                return false;
        }
        else if(me == 1)
        {
            int count_spc = 0;
            for(int i=30;i<40;i++)
            {
                if((l[61].x1 == s[i].x0&&l[61].y1 == s[i].y0&&!occupied[6])||(l[63].x1 == s[i].x0&&l[63].y1 == s[i].y0&&!occupied[9]))
                    count_spc++;
            }
            for(int j=0;j<10;j++)
            {
                for(int i=30;i<40;i++)
                {
                    if(s[i].x0 == l[j].x1&&s[i].y0 == l[j].y1)
                    {
                        count_spc++;
                    }
                }
            }
            if(count_spc == 10)
                return true;
            else
                return false;
        }
        else if(me == 2)
        {
            int count_spc = 0;
            for(int i=50;i<60;i++)
            {
                if((l[61].x1 == s[i].x0&&l[61].y1 == s[i].y0&&!occupied[6])||(l[63].x1 == s[i].x0&&l[63].y1 == s[i].y0&&!occupied[9]))
                    count_spc++;
            }
            for(int j=0;j<10;j++)
            {
                for(int i=50;i<60;i++)
                {
                    if(s[i].x0 == l[j].x1&&s[i].y0 == l[j].y1)
                    {
                        count_spc++;
                    }
                }
            }
            if(count_spc == 10)
                return true;
            else
                return false;
        }
    }
    return false;
}

bool ClientWindow::win_stu()
{
    if(pattern == 2)
    {
        if(me == 1)
        {
            int count_win = 0;
            for(int j=0;j<10;j++)
            {
                for(int i=40;i<50;i++)
                {
                    if(s[i].x0 == l[j].x1&&s[i].y0 == l[j].y1)
                    {
                        count_win++;
                    }
                }
            }
            if(count_win == 10)
                return true;
            else
                return false;
        }
        else if(me == 0)
        {
            int count_win = 0;
            for(int j=0;j<10;j++)
            {
                for(int i=10;i<20;i++)
                {
                    if(s[i].x0 == l[j].x1&&s[i].y0 == l[j].y1)
                    {
                        count_win++;
                    }
                }
            }
            if(count_win == 10)
                return true;
            else
                return false;
        }
    }
    else if(pattern == 3)
    {
        if(me == 0)
        {
            int count_win = 0;
            for(int j=0;j<10;j++)
            {
                for(int i=10;i<20;i++)
                {
                    if(s[i].x0 == l[j].x1&&s[i].y0 == l[j].y1)
                    {
                        count_win++;
                    }
                }
            }
            if(count_win == 10)
                return true;
            else
                return false;
        }
        else if(me == 1)
        {
            int count_win = 0;
            for(int j=0;j<10;j++)
            {
                for(int i=30;i<40;i++)
                {
                    if(s[i].x0 == l[j].x1&&s[i].y0 == l[j].y1)
                    {
                        count_win++;
                    }
                }
            }
            if(count_win == 10)
                return true;
            else
                return false;
        }
        else if(me == 2)
        {
            int count_win = 0;
            for(int j=0;j<10;j++)
            {
                for(int i=50;i<60;i++)
                {
                    if(s[i].x0 == l[j].x1&&s[i].y0 == l[j].y1)
                    {
                        count_win++;
                    }
                }
            }
            if(count_win == 10)
                return true;
            else
                return false;
        }
    }
    return false;
}

void ClientWindow::mid_AIfct()
{
    tip = state;
    qDebug()<<"AI还可以! 正常运作！";
    if(this->processing)
     {
      AI_op();
      this->processing = true;
    }

}

QString ClientWindow::player_zone(int pattern, int me)
{
    if(pattern == 2)
    {
        if(me == 0)
            return "A";
        if(me == 1)
            return "D";
    }
    else if(pattern == 3)
    {
        if(me == 0)
        {
            return "A";
        }
        if(me == 1)
        {
            return "C";
        }
        if(me == 2)
        {
            return "E";
        }
    }
    else if(pattern == 6)
    {
        if(me == 0)
        {
            return "A";
        }
        if(me == 1)
        {
            return "B";
        }
        if(me == 2)
        {
            return "C";
        }
        if(me == 3)
        {
            return "D";
        }
        if(me == 4)
        {
            return "E";
        }
        if(me == 5)
        {
            return "F";
        }
    }
}



//填补缺陷
void ClientWindow::click_move_cj(NetworkData data)
{
    char *au;
    QByteArray ba2 = data.data1.toLatin1();
    au=ba2.data();
    char ai=*au;
    Area_num=change_mark(ai);
    QString str = data.data2;
    str.remove(QRegularExpression("\\s"));
    char * ch;
    QByteArray ba = str.toLatin1(); // must
    ch=ba.data();
    change_char_to_int(ch);
//    qDebug()<<"DEBUG";
    click_move_cj_time = startTimer(1500);
}


void ClientWindow::show_former()
{
    if(pattern == 2)
    {
        QPushButton *btn1 = new QPushButton(this);
        btn1->move(220,900);
        btn1->setText("Debug");
        btn1->show();

        connect(btn1,&QPushButton::clicked,[=](){
            for(int i=0;i<room->Player_num;i++)
            {
                qDebug()<<room->playerList[i];
            }
        });


        QPushButton *m2_old0 = new QPushButton(this);
        m2_old0->move(80,600);
        m2_old0->setText("紫棋路径");
        m2_old0->show();
        QPushButton *m2_old1 = new QPushButton(this);
        m2_old1->move(80,640);
        m2_old1->setText("绿棋路径");
        m2_old1->show();
        QPushButton *ers_m2_old0 = new QPushButton(this);
        ers_m2_old0 ->move(80,900);
        ers_m2_old0->setText("消除");
        ers_m2_old0->show();
//        QPushButton *ers_m2_old1 = new QPushButton(this);
//        ers_m2_old1 ->move(1150,130);
//        ers_m2_old1->setText("消除绿棋路径");
//        ers_m2_old1->show();
        connect(m2_old0,&QPushButton::clicked,[=](){
//            qDebug()<<former_route[0]<<"紫棋上一步";
            ccti_former(former_route[0]);
            cd_former_route = true;
            update();
        });
        connect(m2_old1,&QPushButton::clicked,[=](){
//            qDebug()<<former_route[1]<<"绿棋上一步";
            ccti_former(former_route[1]);
            cd_former_route = true;
            update();
        });
        connect(ers_m2_old0,&QPushButton::clicked,[=](){

            cd_former_route = false;
            update();
        });
//        connect(ers_m2_old1,&QPushButton::clicked,[=](){

//            cd_former_route = false;
//            update();
//        });
    }
    else if(pattern == 3)
    {
        QPushButton *btn1 = new QPushButton(this);
        btn1->move(220,900);
        btn1->setText("Debug");
        btn1->show();

        connect(btn1,&QPushButton::clicked,[=](){
            for(int i=0;i<room->Player_num;i++)
            {
                qDebug()<<room->playerList[i];
            }
        });
        QPushButton *m3_old0 = new QPushButton(this);
        m3_old0->move(80,600);
        m3_old0->setText("紫棋路径");
        m3_old0->show();
        QPushButton *m3_old1 = new QPushButton(this);
        m3_old1->move(80,640);
        m3_old1->setText("红棋路径");
        m3_old1->show();
        QPushButton *m3_old2 = new QPushButton(this);
        m3_old2->move(80,680);
        m3_old2->setText("蓝棋路径");
        m3_old2->show();
        QPushButton *ers_m3_old0 = new QPushButton(this);
        ers_m3_old0 ->move(80,900);
        ers_m3_old0->setText("消除");
        ers_m3_old0->show();
//        QPushButton *ers_m3_old1 = new QPushButton(this);
//        ers_m3_old1 ->move(1150,130);
//        ers_m3_old1->setText("消除绿棋路径");
//        ers_m3_old1->show();
        connect(m3_old0,&QPushButton::clicked,[=](){

            ccti_former(former_route[0]);
            cd_former_route = true;
            update();
        });
        connect(m3_old1,&QPushButton::clicked,[=](){


            ccti_former(former_route[1]);
            cd_former_route = true;
            update();
        });
        connect(m3_old2,&QPushButton::clicked,[=](){

            ccti_former(former_route[2]);
            cd_former_route = true;
            update();
        });
        connect(ers_m3_old0,&QPushButton::clicked,[=](){

            cd_former_route = false;
            update();
        });
    }
    else if(pattern == 6)
    {
        QPushButton *btn1 = new QPushButton(this);
        btn1->move(220,900);
        btn1->setText("Debug");
        btn1->show();

        connect(btn1,&QPushButton::clicked,[=](){
            for(int i=0;i<room->Player_num;i++)
            {
                qDebug()<<room->playerList[i];
            }
        });

        QPushButton *m6_old0 = new QPushButton(this);
        m6_old0->move(80,600);
        m6_old0->setText("紫棋路径");
        m6_old0->show();
        QPushButton *m6_old1 = new QPushButton(this);
        m6_old1->move(80,640);
        m6_old1->setText("青棋路径");
        m6_old1->show();
        QPushButton *m6_old2 = new QPushButton(this);
        m6_old2->move(80,680);
        m6_old2->setText("红棋路径");
        m6_old2->show();
        QPushButton *m6_old3 = new QPushButton(this);
        m6_old3->move(80,720);
        m6_old3->setText("绿棋路径");
        m6_old3->show();
        QPushButton *m6_old4 = new QPushButton(this);
        m6_old4->move(80,760);
        m6_old4->setText("蓝棋路径");
        m6_old4->show();
        QPushButton *m6_old5 = new QPushButton(this);
        m6_old5->move(80,800);
        m6_old5->setText("黄棋路径");
        m6_old5->show();
        QPushButton *ers_m6_old0 = new QPushButton(this);
        ers_m6_old0 ->move(80,900);
        ers_m6_old0->setText("消除");
        ers_m6_old0->show();
        connect(m6_old0,&QPushButton::clicked,[=](){

            ccti_former(former_route[0]);
            cd_former_route = true;
            update();
        });
        connect(m6_old1,&QPushButton::clicked,[=](){

            ccti_former(former_route[1]);
            cd_former_route = true;
            update();
        });
        connect(m6_old2,&QPushButton::clicked,[=](){

            ccti_former(former_route[2]);
            cd_former_route = true;
            update();
        });
        connect(m6_old3,&QPushButton::clicked,[=](){

            ccti_former(former_route[3]);
            cd_former_route = true;
            update();
        });
        connect(m6_old4,&QPushButton::clicked,[=](){

            ccti_former(former_route[4]);
            cd_former_route = true;
            update();
        });
        connect(m6_old5,&QPushButton::clicked,[=](){

            ccti_former(former_route[5]);
            cd_former_route = true;
            update();
        });

        connect(ers_m6_old0,&QPushButton::clicked,[=](){


            cd_former_route = false;
            update();
        });
    }
}

void ClientWindow::ccti_former(QString rut)    //用于将上一步的QString路径转化为标准坐标路径
{
//    qDebug()<<rut<<"上一步是啥？"<<player_zone(pattern,me);
    QString str = rut;
    str.remove(QRegularExpression("\\s"));
    char * p;
    QByteArray ba = str.toLatin1(); // must
    p=ba.data();
    memset(drawed_route,0,sizeof(drawed_route));
//    next=true;
    drawed_route_length=0;
    int mo=0; // 这是整形数组的下标，字符数组会有负号，如果和字符数组共享下标，在跳过负号的时候就会留下空挡
    int len = strlen(p);
        int i;
        for (i = 0; i < len;)
        {
            if (p[i] >= '0' && p[i] <= '9')
            {
                drawed_route[mo] = p[i] - '0';
                i++;
                mo++;
            }
            else if (p[i] == '-')
            {
                drawed_route[mo] = p[i + 1] - '0';
                drawed_route[mo] *= -1;
                i += 2;
                mo++;
            }
            drawed_route_length++;
        }
}


QString ClientWindow::loc_Area()
{
    if(pattern == 2)
    {
        if(me == 0)
            return "A";
        else if(me == 1)
        {
            return "D";
        }
    }
    else if(pattern == 3)
    {
        if(me == 0)
        {
            return "A";
        }
        else if(me == 1)
        {
            return "C";
        }
        else if(me == 2)
        {
            return "E";
        }
    }
    else if(pattern == 6)
    {
        if(me == 0)
        {
            return "A";
        }
        else if(me == 1)
        {
            return "B";
        }
        else if(me == 2)
        {
            return "C";
        }
        else if(me == 3)
        {
            return "D";
        }
        else if(me == 4)
        {
            return "E";
        }
        else if(me == 5)
        {
            return "F";
        }
    }
}

QString ClientWindow::get_id(QString a)
{
    if(pattern == 2)
    {
        if(a == "A")
            return room->playerList[0];
        else if(a == "D")
            return room->playerList[1];
    }
    else if(pattern == 3)
    {
        if(a == "A")
            return room->playerList[0];
        else if(a == "C")
            return room->playerList[1];
        else if(a == "E")
            return room->playerList[2];
    }
    else if(pattern == 6)
    {
        if(a == "A")
            return room->playerList[0];
        else if(a == "B")
            return room->playerList[1];
        else if(a == "C")
            return room->playerList[2];
        else if(a == "D")
            return room->playerList[3];
        else if(a == "E")
            return room->playerList[4];
        else if(a == "F")
            return room->playerList[5];
    }
}
