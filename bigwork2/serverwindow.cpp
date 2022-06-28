#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "networkdata.h"
#include "networkserver.h"
#include "QTcpServer"
#include<QTimerEvent>
#include<QTimer>
#include<QMainWindow>
#include<QtNetwork>

#include<QPainter>
#include<QDebug>
#include<QMouseEvent>
#include<iostream>
#include<string.h>
#include<QString>
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>
#include <QRegExp>
#include<QRegularExpression>


int ServerWindow::num=0;
ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
//ServerWindow::ServerWindow():MainWindow()
{
    ui->setupUi(this);
    server=new NetworkServer(this);
    connect(server, &NetworkServer::receive, this, &ServerWindow::receive);
    quint16 port=9999;//测试时替换
    server->listen(QHostAddress::Any,port);
    for(int i=0;i<6;i++)
    {
        ready[i] = "0";
    }

    for(int i=0;i<6;i++)
    {
        fall_players[i] = -1;
        win_players[i] = -1;
    }

    count_down = 15;
    //以下来自MainWindow
    pattern = 0;
    state=0;
    resize(1300,1200);
    _selected_id=-1;
    markstone();
//    init_occupied();
    init_Possible_Location();
    init_cvrt_Loc();
    init_cvrt_st();
    init_psr();
    init_psr_set();
}

ServerWindow::~ServerWindow()
{
    delete ui;
}
void ServerWindow::receive(QTcpSocket *s,NetworkData w)
{
    if(w.op==OPCODE::JOIN_ROOM_OP)
    {
        if(room=="")
        {
//            qDebug()<<"123";     //debug用的
            room=w.data1;
            user[0]=w.data2;
            _socket[num++]=s;
            NetworkData a(OPCODE::JOIN_ROOM_REPLY_OP,"","");
            server->send(s,a);
        }
        else
        {
            if(room!=w.data1)
            {
                NetworkData a(OPCODE::ERROR_OP,"OTHER_ERROR","");
                server->send(s,a);
            }
            else
            {
                int flag=1,sum=0;
                for(int i=0;i<6;i++)
                {
                    if(user[i]==w.data2)
                    {
                        NetworkData a(OPCODE::ERROR_OP,"INVALID_JOIN","");
                        server->send(s,a);
                        break;
                        flag=0;
                    }
                }
                if(flag)
                {
                    for(int i=0;i<6;i++)
                    {
                        if(ready[i]=='1')
                            sum++;
                    }
                    if(num==sum)
                    {
                        NetworkData a(OPCODE::ERROR_OP,"ROOM_IS_RUNNING","");
                        server->send(s,a);
                    }
                    else if(num==6)
                    {
                        NetworkData a(OPCODE::ERROR_OP,"INVALID_JOIN","");
                        server->send(s,a);
                    }
                    else
                    {
                        user[num]=w.data2;
                        _socket[num]=s;
                        QString a=user[0];
                        QString b=ready[0];
//                        for(int i=0;i<num;i++)
//                            qDebug()<<ready[i];
                        for(int i=1;i<num;i++)
                        {
                            a=a+" "+user[i];
                            b=b+ready[i];
                        }
//                        qDebug()<<b<<"DEBUG";
                        NetworkData c(OPCODE::JOIN_ROOM_REPLY_OP,a,b);
                        NetworkData old(OPCODE::JOIN_ROOM_OP,w.data2,"");  //发给之前房间内玩家的数据
                        server->send(s,c);
                        for(int i=0;i<num;i++)
                        {
                            server->send(_socket[i],old);
                        }
                        num++;
                    }
                }

            }
        }
    }
    else if(w.op==OPCODE::LEAVE_ROOM_OP)
    {
        if(room==w.data1)
        {

            int i,j;
            for(i=0;i<6;i++)
            {
                if(user[i]==w.data2)
                    break;
            }
            for(j=i+1;j<6;j++)
            {
                user[j-1]=user[j];
                ready[j-1]=ready[j];
                _socket[j-1]=_socket[j];
            }
            user[5]="";
            ready[5]="0";
            _socket[5]=NULL;
            num--;
            NetworkData a(OPCODE::LEAVE_ROOM_OP,w.data2,"");
            for(int i=0;i<num;i++)
            {
                server->send(_socket[i],a);
            }
//            server->send(s,a);
        }
    }
    else if(w.op==OPCODE::PLAYER_READY_OP)
    {
        for(int i=0;i<6;i++)
            if(user[i]==w.data1)
            {
                ready[i]="1";
                break;
            }
        for(int i=0;i<num;i++)
            server->send(_socket[i],w);
        int sum=0;
        for(int i=0;i<6;i++)
            if(ready[i]=='1')
                sum++;
        if(num==sum)
        {

            QString a=user[0];
            QString b;
            for(int i=1;i<num;i++)
                a=a+" "+user[i];
            if(num==2)
                b="A D";
            else if(num==3)
                b="A C E";
            else if(num==6)
                b="A B C D E F";
            NetworkData c(OPCODE::START_GAME_OP,a,b);
            for(int i=0;i<num;i++)
             {
                server->send(_socket[i],c);
            }
            this->pattern = num;
            this->init_occupied();
            this->update();
            this->show();
            time_id_clock_down = startTimer(1000);
        }
    }
    else if(w.op==OPCODE::MOVE_OP)
    {
//        qDebug()<<w.data2<<"server check route";
        if(/*!judge_route(w.data2)*/0)
        {
            qDebug()<<"potential power!";
            NetworkData a(OPCODE::ERROR_OP,"INVALID_MOVE","");
            server->send(s,a);
        }
        else
        {
            s_send = s;

            data_1 = w.data1;
            data_2 = w.data2;
            move(w);



//            std::time_t t = std::time(0);
//            QString s_t = QString::number(t,10);
//            QString zone_p = data_1;
//            NetworkData d(OPCODE::START_TURN_OP,zone_p,s_t);
//            for(int i=0;i<num;i++)
//            {
//                server->send(_socket[i],d);
//            }
//            restart_time();
//            for(int i=0;i<num;i++)
//            {
//                if(_socket[i]!=s)
//                 server->send(_socket[i],w);
//            }


        }
    }
    else if(w.op==OPCODE::START_TURN_OP)
    {
        for(int i=0;i<num;i++)
        {
            server->send(_socket[i],w);
        }
//        restart_time();
    }
    else
    {
        NetworkData a(OPCODE::ERROR_OP,"INVALID_REQ","");
        server->send(s,a);
    }
}

bool ServerWindow::judge_route(QString rut)
{
    bool b = true;
    rut.remove(QRegularExpression("\\s"));
    char*  ch;
    QByteArray ba = rut.toLatin1(); // must
    ch=ba.data();
    change_char_to_int(ch);
    int count = 0;
    int origin;
    getCvt(route[0],route[1],origin); //找出起点，用深搜标记可能路径，为之后的合法性判断做铺垫
    dfs_markSingle(0,origin);
    while(count<routeLength-2)
    {
        int loc_2;
        getCvt(route[count+2],route[count+3],loc_2);
        if(!canMove(l[origin].x1,l[origin].y1,l[loc_2].x1,l[loc_2].y1))  //这里的思路是检测“起点”到每一个“非起点"的点的路径是否合法，所以第一组坐标一直使用origin
        {
            b = false;
        }
        count +=2;
    }
    wipe_dfs_markSinge();
    return b;
}

void ServerWindow::dfs_markSingle(int count, int i)
{
    int u=0;
    if(!getPos(l[i].x1,l[i].y1,u))  //或许可以优化？
     return ;
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
                        dfs_markSingle(count+1,pst2); //深度优先搜索

                    }
                }
            }
        }
    }
}

void ServerWindow::wipe_dfs_markSinge()
{
    for(int i=0;i<121;i++)
    {
        if(Possible_Location[i])
            Possible_Location[i] = false;
    }
    return ;
}

void ServerWindow::move(NetworkData data)
{
    QString str = data.data2;
    str.remove(QRegularExpression("\\s"));
    char*  ch;
    QByteArray ba = str.toLatin1(); // must
    ch=ba.data();
    change_char_to_int(ch);
    edit->clear();
//    move_Rt(route[0],route[1]);
//    move_Rt(route[routeLength-2],route[routeLength-1]);
//    for(int i=0;i<num;i++)
//    {
//        if(_socket[i]!=s_send)
//         server->send(_socket[i],data);
//    }
//    state++;
//    state%=pattern;
//    for(int i=0;i<failed_num;i++)      //轮转修改1
//    {
//        if(state == fall_players[i])
//        {
//              state++;
//              state%=pattern;
//        }
//    }
    time_id3 = startTimer(1000);
    move_complete = true;

}

void ServerWindow::timerEvent(QTimerEvent *ev)
{
    if(ev->timerId() == time_id3)
    {
        if(cnt<routeLength-2)
        {
            move_Rt(route[cnt],route[cnt+1]);
            move_Rt(route[cnt+2],route[cnt+3]);
            cnt+=2;
        }
        else if(cnt == routeLength-2)
        {
            NetworkData Move(OPCODE::MOVE_OP,data_1,data_2);
            restart_time();
            for(int i=0;i<num;i++)
            {
                if(_socket[i]!=s_send)
                 server->send(_socket[i],Move);
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

            for(int i=0;i<win_num;i++)      //轮转修改1
            {
                if(state == win_players[i])
                {
                    state++;
                    state%=pattern;
                }
            }

//            std::time_t t = std::time(0);
//            QString s_t = QString::number(t,10);
//            QString zone_p = data_1;
//            NetworkData d(OPCODE::START_TURN_OP,zone_p,s_t);
//            for(int i=0;i<num;i++)
//            {
//                server->send(_socket[i],d);
//            }




            cnt+=2;
            return ;
        }
    }
    if(ev->timerId() == time_id_clock_down)
    {
        if(!count_down||count_down<=0)
        {
            if_overtime = true;
            qDebug()<<state<<"轮转状态检查";
            wipe_player(state%pattern);  //因为server端的state更新是在新一轮转换路径时，而非上一轮落子后，所以未落子前state都处在上一轮，由于超时判负是通过state来完成的，所以传参时要传(state+1)%pattern
            update();
            state++;  //更新状态
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
                if(state == win_players[i])
                {
                    state++;
                    state%=pattern;
                }
            }
            restart_time();  //进入下一轮计时
        }
        else
         ui->Clock_down->setText(QString::number(count_down--));
    }
}




//以下来自MainWindow


void ServerWindow::paintEvent(QPaintEvent *)
{
//    QPainter painter(this);
//    painter.drawLine(100,100,400,500);
//    painter.drawLine(700,100,400,500);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    static const QPointF points1[3] = { //1号三角形
         QPointF(200.0, 300.0),
         QPointF(500.0, 300.0),
         QPointF(350.0, 500.0),
    };
    painter.setBrush(QColor(255,255,0));
    painter.drawPolygon(points1,3);
    painter.setBrush(Qt::NoBrush);

    static const QPointF points6[3] = { //6号三角形
         QPointF(200.0, 700.0),
         QPointF(500.0, 700.0),
         QPointF(350.0, 500.0),
    };
    painter.setBrush(QColor(0,0,255));
    painter.drawPolygon(points6,3);
    painter.setBrush(Qt::NoBrush);

    static const QPointF points2[3] = { //2号三角形
         QPointF(650.0, 100.0),
         QPointF(500.0, 300.0),
         QPointF(800.0, 300.0),
    };
    painter.setBrush(QColor(255,0,255));
    painter.drawPolygon(points2,3);
    painter.setBrush(Qt::NoBrush);

    static const QPointF points3[3] = { //3号三角形
         QPointF(800.0, 300.0),
         QPointF(1100.0, 300.0),
         QPointF(950.0, 500.0),
    };
    painter.setBrush(QColor(0,255,255));
    painter.drawPolygon(points3,3);
    painter.setBrush(Qt::NoBrush);

    static const QPointF points4[3] = { //4号三角形
         QPointF(800.0, 700.0),
         QPointF(1100.0, 700.0),
         QPointF(950.0, 500.0),
    };
    painter.setBrush(QColor(255,0,0));
    painter.drawPolygon(points4,3);
    painter.setBrush(Qt::NoBrush);

    static const QPointF points5[3] = { //5号三角形
         QPointF(650.0, 900.0),
         QPointF(500.0, 700.0),
         QPointF(800.0, 700.0),
    };
    painter.setBrush(QColor(0,255,0));
    painter.drawPolygon(points5,3);
    painter.setBrush(Qt::NoBrush);


//    painter.drawEllipse(QPointF(650,144),7,7);


// i=(33,0) j=(0,44)

//    QLineF line(10.0, 80.0, 90.0, 20.0);

//    QPainter(this);
//    painter.drawLine(line);

    for(int i=0;i<12;i++) //正三角，右倾斜
    {
        QLineF line(650+i*33, 148+i*44, 254+i*66, 676);
        painter.drawLine(line);
    }

    for(int i=0;i<12;i++) //正三角，左倾斜
    {
        QLineF line(650-i*33, 148+i*44, 1046-i*66, 676);
        painter.drawLine(line);
    }

    for(int i=0;i<12;i++)
        {
        QLineF line(254+i*33, 676-i*44, 1046-i*33, 676-i*44);
        painter.drawLine(line);
    }


    for(int i=0;i<12;i++)
    {
        QLineF line(254+i*33, 324+i*44, 1046-i*33, 324+i*44);
        painter.drawLine(line);
    }

    for(int i=0;i<12;i++)
    {
        QLineF line(254+i*66, 324, 650+i*33, 852-i*44);
        painter.drawLine(line);
    }

    for(int i=0;i<12;i++)
    {
        QLineF line(1046-i*66, 324, 650-i*33, 852-i*44);
        painter.drawLine(line);
    }
    painter.setBrush(Qt::white);
    int count=0;
    for(int i=1;i<=4;i++)   //2号三角形 (650,148)
    {
        for(int j=1;j<=i;j++)
        {
            painter.drawEllipse(QPoint(650-(i-1)*33+(j-1)*66,148+(i-1)*44),r,r);
            l[count].x1=650-(i-1)*33+(j-1)*66;
            l[count].y1=148+(i-1)*44;

            count++;
        }
    }

    for(int i=1;i<=4;i++) //1号三角形 (254,324)
    {
        for(int j=1;j<=5-i;j++)
        {
            painter.drawEllipse(QPoint(254+(i-1)*33+(j-1)*66,324+(i-1)*44),r,r);
            l[count].x1=254+(i-1)*33+(j-1)*66;
            l[count].y1=324+(i-1)*44;

            count++;
        }
    }

    for(int i=1;i<=4;i++)   //4号三角形 (947,544)
    {
        for(int j=1;j<=i;j++)
        {
            painter.drawEllipse(QPoint(947-(i-1)*33+(j-1)*66,544+(i-1)*44),r,r);
            l[count].x1=947-(i-1)*33+(j-1)*66;
            l[count].y1=544+(i-1)*44;

            count++;
        }
    }

    for(int i=1;i<=4;i++)   //6号三角形 (353,544)
    {
        for(int j=1;j<=i;j++)
        {
            painter.drawEllipse(QPoint(353-(i-1)*33+(j-1)*66,544+(i-1)*44),r,r);
            l[count].x1=353-(i-1)*33+(j-1)*66;
            l[count].y1=544+(i-1)*44;

            count++;
           }
    }

    for(int i=1;i<=4;i++) //3号三角形 (848,324)
    {
        for(int j=1;j<=5-i;j++)
        {
            painter.drawEllipse(QPoint(848+(i-1)*33+(j-1)*66,324+(i-1)*44),r,r);
            l[count].x1=848+(i-1)*33+(j-1)*66;
            l[count].y1=324+(i-1)*44;

            count++;
        }
    }

    for(int i=1;i<=4;i++) //5号三角形 (551,720)
    {
        for(int j=1;j<=5-i;j++)
        {
            painter.drawEllipse(QPoint(551+(i-1)*33+(j-1)*66,720+(i-1)*44),r,r);
            l[count].x1=551+(i-1)*33+(j-1)*66;
            l[count].y1=720+(i-1)*44;

            count++;
        }
    }

//    qDebug()<<count;

    for(int i=1;i<=5;i++)//六边形上半部分 (518,324)
    {
        for(int j=1;j<=4+i;j++)
        {
             painter.drawEllipse(QPoint(518-(i-1)*33+(j-1)*66,324+(i-1)*44),r,r);
             l[count].x1=518-(i-1)*33+(j-1)*66;
             l[count].y1=324+(i-1)*44;

             count++;
        }
    }

    for(int i=1;i<=4;i++)//六边形下半部分 (518,676)
    {
        for(int j=1;j<=4+i;j++)
        {
            painter.drawEllipse(QPoint(518-(i-1)*33+(j-1)*66,676-(i-1)*44),r,r);
            l[count].x1=518-(i-1)*33+(j-1)*66;
            l[count].y1=676-(i-1)*44;

            count++;
        }
    }

    painter.setBrush(Qt::NoBrush);
//    for(int i=0;i<60;i++)   //二号bug位
//    {
//        drawstone(painter,i);
//    }

    if(pattern)
    {

        if(pattern==2)   //二号bug位
        {
            for(int i=10;i<20;i++)
                drawstone(painter,i);
            for(int i=40;i<50;i++)
                drawstone(painter,i);
        }
        else if(pattern==3)
        {
            for(int i=10;i<20;i++)
                drawstone(painter,i);
            for(int i=30;i<40;i++)
                drawstone(painter,i);
            for(int i=50;i<60;i++)
                drawstone(painter,i);
        }
        else
        {
            for(int i=0;i<60;i++)
                drawstone(painter,i);
        }



        if(_selected_id!=-1&&signal1) //signal单纯是为了避免最初就执行这两个函数
        {
            getPos(s[_selected_id].x0,s[_selected_id].y0,_pst); //重要，必须把棋子坐标换成棋盘坐标，因为之后对于Possible_Location的搜索是在棋盘上进行的
            dbfs(0,_pst,painter,psr_m);
        }
        if(signal1&&_selected_id==-1)
        {
            wipe_dbfs(painter);
            clear_psr();
        }
        signal1=1;
    }

    if(if_overtime)   //此段代码是将超时判负的玩家棋子染成白色
    {
        QPainter p1(this);
        //染色
        p1.setBrush(Qt::white);
        for(int i=0;i<10;i++)
        {
            p1.drawEllipse(QPoint(l[wipe_stones[i]].x1,l[wipe_stones[i]].y1),r,r);
        }
        if_overtime = false;
    }
}

void ServerWindow::markstone()
{
        int count=0;
        for(int i=1;i<=4;i++) //1号三角形 (254,324)
        {
            for(int j=1;j<=5-i;j++)
            {
                s[count].x0=254+(i-1)*33+(j-1)*66;
                s[count].y0=324+(i-1)*44;
                s[count].color=5;

                count++;
            }
        }

        for(int i=1;i<=4;i++) //2号三角形 (650,148)
        {
            for(int j=1;j<=i;j++)
            {
                s[count].x0=650-(i-1)*33+(j-1)*66;
                s[count].y0=148+(i-1)*44;
                s[count].color=0;

                count++;
            }
        }

        for(int i=1;i<=4;i++) //3号三角形 (848,324)
        {
            for(int j=1;j<=5-i;j++)
            {
                s[count].x0=848+(i-1)*33+(j-1)*66;
                s[count].y0=324+(i-1)*44;
                s[count].color=1;

                count++;
            }
        }


        for(int i=1;i<=4;i++) //4号三角形 (254,324)
        {
            for(int j=1;j<=i;j++)
            {
                s[count].x0=947-(i-1)*33+(j-1)*66;
                s[count].y0=544+(i-1)*44;
                s[count].color=2;

                count++;
            }
        }

        for(int i=1;i<=4;i++) //5号三角形 (551,720)
        {
            for(int j=1;j<=5-i;j++)
            {
                s[count].x0=551+(i-1)*33+(j-1)*66;
                s[count].y0=720+(i-1)*44;
                s[count].color=3;

                count++;
            }
        }


        for(int i=1;i<=4;i++) //6号三角形 (353,544)
        {
            for(int j=1;j<=i;j++)
            {
                s[count].x0=353-(i-1)*33+(j-1)*66;
                s[count].y0=544+(i-1)*44;
                s[count].color=4;

                count++;
            }
        }
}



QPoint ServerWindow::center(int x, int y)
{
    return QPoint(x,y);
}

QPoint ServerWindow::center(int id)
{
    return QPoint(s[id].x0,s[id].y0);
}

void ServerWindow::drawstone(QPainter &painter,int i)
{

    if(i>=0&&i<10) //1号棋群
    {
        if(i==_selected_id)
             painter.drawPixmap(s[i].x0-r-11.5,s[i].y0-r-3,4*r,2.5*r,QPixmap(":/stone2/0.png"));
        else
        {
           QBrush brush1(Qt::yellow);
           painter.setBrush(brush1);

//        painter.drawEllipse(QPoint(s[i].x0,s[i].y0),r,r);
        painter.drawPixmap(s[i].x0-r-11.5,s[i].y0-r-3,4*r,2.5*r,QPixmap(":/stone2/1.png"));
        }

    }


    else if(i>=10&&i<20) //2号棋群
    {
        if(i==_selected_id)
            painter.drawPixmap(s[i].x0-r-11.5,s[i].y0-r-3,4*r,2.5*r,QPixmap(":/stone2/0.png"));
       else
        {
        QBrush brush2(QColor(255,0,255));
        painter.setBrush(brush2);

//         painter.drawEllipse(QPoint(s[i].x0,s[i].y0),r,r);
        painter.drawPixmap(s[i].x0-r-11.5,s[i].y0-r-3,4*r,2.5*r,QPixmap(":/stone2/2.png"));
        }
    }

    else if(i>=20&&i<30) //3号棋群
    {
        if(i==_selected_id)
//            painter.setBrush(QBrush(QColor(119,136,153)));
            painter.drawPixmap(s[i].x0-r-11.5,s[i].y0-r-3,4*r,2.5*r,QPixmap(":/stone2/0.png"));
        else
        {
        QBrush brush3(QColor(0,255,255));
        painter.setBrush(brush3);

//         painter.drawEllipse(QPoint(s[i].x0,s[i].y0),r,r);
        painter.drawPixmap(s[i].x0-r-11.5,s[i].y0-r-3,4*r,2.5*r,QPixmap(":/stone2/3.png"));
        }
    }

    else if(i>=30&&i<40) //4号棋群
    {
        if(i==_selected_id)
//            painter.setBrush(QBrush(QColor(119,136,153)));
            painter.drawPixmap(s[i].x0-r-11.5,s[i].y0-r-3,4*r,2.5*r,QPixmap(":/stone2/0.png"));
        else
        {
        QBrush brush4(QColor(255,0,0));
        painter.setBrush(brush4);

//         painter.drawEllipse(QPoint(s[i].x0,s[i].y0),r,r);
        painter.drawPixmap(s[i].x0-r-11.5,s[i].y0-r-3,4*r,2.5*r,QPixmap(":/stone2/4.png"));
        }
    }

    else if(i>=40&&i<50) //5号棋群
    {
        if(i==_selected_id)
//            painter.setBrush(QBrush(QColor(119,136,153)));
            painter.drawPixmap(s[i].x0-r-11.5,s[i].y0-r-3,4*r,2.5*r,QPixmap(":/stone2/0.png"));
        else
        {
        QBrush brush5(QColor(0,255,0));
        painter.setBrush(brush5);

//         painter.drawEllipse(QPoint(s[i].x0,s[i].y0),r,r);
        painter.drawPixmap(s[i].x0-r-11.5,s[i].y0-r-3,4*r,2.5*r,QPixmap(":/stone2/5.png"));
        }
    }

    else //6号棋群
       {
        if(i==_selected_id)
//           painter.setBrush(QBrush(QColor(119,136,153)));
            painter.drawPixmap(s[i].x0-r-11.5,s[i].y0-r-3,4*r,2.5*r,QPixmap(":/stone2/0.png"));
        else
        {
        QBrush brush6(QColor(0,0,255));
        painter.setBrush(brush6);

//         painter.drawEllipse(QPoint(s[i].x0,s[i].y0),r,r);
        painter.drawPixmap(s[i].x0-r-11.5,s[i].y0-r-3,4*r,2.5*r,QPixmap(":/stone2/6.png"));
        }
    }

}

bool ServerWindow::getPos(QPoint pt, int &B_loc)
{
    for(B_loc=0;B_loc<121;B_loc++)
    {
        QPoint c=center(l[B_loc].x1,l[B_loc].y1);
        int dx=c.rx()-pt.rx();
        int dy=c.ry()-pt.ry();
        int dist=dx*dx+dy*dy;
        if(dist<r*r)
            return true;
    }
    return false;
}

bool ServerWindow::getPos(int x0,int y0, int &B_loc)
{
    for(B_loc=0;B_loc<121;B_loc++)
    {
        QPoint c=center(l[B_loc].x1,l[B_loc].y1);
        int dx=c.rx()-x0;
        int dy=c.ry()-y0;
        int dist=dx*dx+dy*dy;
        if(dist<r*r)
            return true;
    }
    return false;
}


void ServerWindow::init_occupied() //标记哪些棋盘坐标被棋子占领
{
//    qDebug()<<pattern;
    if(pattern==6)  //四号bug位
    {
        for(int i=0;i<60;i++)
        {
            occupied[i]=true;
        }
        for(int i=60;i<121;i++)
        {
            occupied[i]=false;
        }
    }
    else if(pattern==3)  //_____________________________________________________________________________________________________
    {
            for(int i=0;i<10;i++)
            {
                occupied[i]=true;
            }
            for(int i=20;i<40;i++)
            {
                occupied[i]=true;
            }
            for(int i=10;i<20;i++)
            {
                occupied[i]=false;
            }
            for(int i=40;i<60;i++)
            {
                occupied[i]=false;
            }
            for(int i=60;i<121;i++)
            {
                occupied[i]=false;
            }
    }
    else
    {
            for(int i=0;i<10;i++)
            {
                occupied[i]=true;
            }
            for(int i=50;i<60;i++)
            {
                occupied[i]=true;
            }
            for(int i=10;i<50;i++)
            {
                occupied[i]=false;
            }
            for(int i=60;i<121;i++)
            {
                occupied[i]=false;
            }
    }
}

void ServerWindow::init_Possible_Location() //标记dbfs()时可能到达的位置，为后续染色做标记
{
    for(int i=0;i<121;i++)
    {
        Possible_Location[i]=false;
    }
}

bool ServerWindow::canMove(int x1,int y1,int x2,int y2) //判断移动是否合法
{
    int dx=x1-x2;
    int dy=y1-y2;
    int lct;
    int dist=dx*dx+dy*dy;
    if(dist>3026)
    {
        getPos((x1+x2)/2,(y1+y2)/2,lct); //如果选中距离大于两个棋子间的最小距离，则为隔空位，需取出中间坐标
    }
    int position1;
    getPos(x2,y2,position1);
    if(dist<=4356||(dist<=17424&&occupied[lct])||Possible_Location[position1]) //如果是相邻位未被占领或隔空位未被占领或经过判断后可以连跳的位置
        return true;
    else
        return false;
}

void ServerWindow::dbfs(int count ,int i,QPainter &painter,QString a) //这里的i是棋盘的坐标编号
{
    int u=0;
    if(!getPos(l[i].x1,l[i].y1,u))  //或许可以优化？
     return ;
    QString p=a;
    painter.setBrush(QColor(255,165,0));
    int d[6][2]={{33,44},{-33,-44},{33,-44},{-33,44},{66,0},{-66,0}};
    int nx,ny;
    int interval_x,interval_y; //隔空x，y的坐标
    int pst,pst2;//pst定周边位，pst2定隔空位
    if(!count)
    {
//        a[0]=l[i].cv_x-'0';
//        a[1]=l[i].cv_y-'0';
        QString s1= QString::number(l[i].cv_x,10);
        p.append(" "+s1);
        QString s2= QString::number(l[i].cv_y,10);
        p.append(" "+s2);
    }
    for(int j=0;j<6;j++) //探测周围六个点
    {
        nx=l[i].x1+d[j][0];
        ny=l[i].y1+d[j][1];
        if(getPos(nx,ny,pst)) //条件：在棋盘内
        {
            if(!occupied[pst]&&!count&&!Possible_Location[pst]) //判断相邻位，并且只在第一层做这个搜索————（改了这里（加了第三个条件）计算就没问题了，但是还是没有想通为什么是这里（也可能不是））
            {
                Possible_Location[pst]=true;
                painter.drawEllipse(QPoint(l[pst].x1,l[pst].y1),r,r); //必须用QPoint，不然画出来的就是铁王八
//                char p[120];
//                strcpy(p,a);
//                p[2]=l[pst].cv_x-'0';
//                p[3]=l[pst].cv_y-'0';
//                strcpy(l[pst].psr,p);
                QString s=p;
                QString s1=QString::number(l[pst].cv_x,10);
                QString s2=QString::number(l[pst].cv_y,10);
                s.append(" "+s1);
                s.append(" "+s2);
                l[pst].psr=s;
                l[pst].psr_set=true;
            }
            if(occupied[pst]) //条件：在棋盘内——判断隔空位
            {
                interval_x=2*l[pst].x1-l[i].x1;  //隔空位的x坐标
                interval_y=2*l[pst].y1-l[i].y1;  //隔空位的y坐标
                if(getPos(interval_x,interval_y,pst2))
                {
                    if(!occupied[pst2]&&!Possible_Location[pst2]) //第二个条件是为了不重复标记
                    {
//                        char p[120];
//                        strcpy(p,a);
//                        p[2*count+2]=l[pst2].cv_x-'0';
//                        p[2*count+3]=l[pst2].cv_y-'0';
//                        strcpy(l[pst2].psr,p);
                        QString s=p;

                        QString s1=QString::number(l[pst2].cv_x,10);
                        QString s2=QString::number(l[pst2].cv_y,10);
                        s.append(" "+s1);
                        s.append(" "+s2);
                        l[pst2].psr=s;
//                        qDebug()<<s;
                        l[pst2].psr_set=true;
                        painter.drawEllipse(QPoint(l[pst2].x1,l[pst2].y1),r,r); //将其染色为Possible_Location标记色
                        Possible_Location[pst2]=true; //可能位置标记
                        dbfs(count+1,pst2,painter,l[pst2].psr); //深度优先搜索
//                        a.clear();

                    }
                }
            }
        }
    }
}

void ServerWindow::wipe_dbfs(QPainter &painter) //将dbfs()染的Possibel_Location染回白色，并重置Posbible_Location数组
{

    painter.setBrush(Qt::white);
    for(int i=0;i<121;i++)
    {
        if(Possible_Location[i])
        {
            if(QPoint(current_x,current_y)!=QPoint(l[i].x1,l[i].y1)) //不能覆盖当前选中棋子
               painter.drawEllipse(QPoint(l[i].x1,l[i].y1),r,r);
            Possible_Location[i]=false;
        }
    }
//    painter.drawEllipse(l[0].x1,l[0].y1,12,12);
}

//int MainWindow::pt()
//{
//    state=pattern;
//}
void ServerWindow::init_cvrt_Loc()
{
     //No.2 triangle
    l[0].cv_x=-4;l[0].cv_y=8;
    l[1].cv_x=-4;l[1].cv_y=7;
    l[3].cv_x=-4;l[3].cv_y=6;
    l[6].cv_x=-4;l[6].cv_y=5;
    l[2].cv_x=-3;l[2].cv_y=7;
    l[4].cv_x=-3;l[4].cv_y=6;
    l[7].cv_x=-3;l[7].cv_y=5;
    l[5].cv_x=-2;l[5].cv_y=6;
    l[8].cv_x=-2;l[8].cv_y=5;
    l[9].cv_x=-1;l[9].cv_y=5;

     //No.1 triangle
    l[10].cv_x=-8;l[10].cv_y=4;
    l[11].cv_x=-7;l[11].cv_y=4;
    l[12].cv_x=-6;l[12].cv_y=4;
    l[13].cv_x=-5;l[13].cv_y=4;
    l[14].cv_x=-7;l[14].cv_y=3;
    l[15].cv_x=-6;l[15].cv_y=3;
    l[16].cv_x=-5;l[16].cv_y=3;
    l[17].cv_x=-6;l[17].cv_y=2;
    l[18].cv_x=-5;l[18].cv_y=2;
    l[19].cv_x=-5;l[19].cv_y=1;

     //No.4 triangle
    l[20].cv_x=5;l[20].cv_y=-1;
    l[21].cv_x=5;l[21].cv_y=-2;
    l[22].cv_x=6;l[22].cv_y=-2;
    l[23].cv_x=5;l[23].cv_y=-3;
    l[24].cv_x=6;l[24].cv_y=-3;
    l[25].cv_x=7;l[25].cv_y=-3;
    l[26].cv_x=5;l[26].cv_y=-4;
    l[27].cv_x=6;l[27].cv_y=-4;
    l[28].cv_x=7;l[28].cv_y=-4;
    l[29].cv_x=8;l[29].cv_y=-4;

     //No.6 triangle
    l[30].cv_x=-4;l[30].cv_y=-1;
    l[31].cv_x=-4;l[31].cv_y=-2;
    l[32].cv_x=-3;l[32].cv_y=-2;
    l[33].cv_x=-4;l[33].cv_y=-3;
    l[34].cv_x=-3;l[34].cv_y=-3;
    l[35].cv_x=-2;l[35].cv_y=-3;
    l[36].cv_x=-4;l[36].cv_y=-4;
    l[37].cv_x=-3;l[37].cv_y=-4;
    l[38].cv_x=-2;l[38].cv_y=-4;
    l[39].cv_x=-1;l[39].cv_y=-4;

     //No.3 triangle
    l[40].cv_x=1;l[40].cv_y=4;
    l[41].cv_x=2;l[41].cv_y=4;
    l[42].cv_x=3;l[42].cv_y=4;
    l[43].cv_x=4;l[43].cv_y=4;
    l[44].cv_x=2;l[44].cv_y=3;
    l[45].cv_x=3;l[45].cv_y=3;
    l[46].cv_x=4;l[46].cv_y=3;
    l[47].cv_x=3;l[47].cv_y=2;
    l[48].cv_x=4;l[48].cv_y=2;
    l[49].cv_x=4;l[49].cv_y=1;

     //No.5 triangle
    l[50].cv_x=1;l[50].cv_y=-5;
    l[51].cv_x=2;l[51].cv_y=-5;
    l[52].cv_x=3;l[52].cv_y=-5;
    l[53].cv_x=4;l[53].cv_y=-5;
    l[54].cv_x=2;l[54].cv_y=-6;
    l[55].cv_x=3;l[55].cv_y=-6;
    l[56].cv_x=4;l[56].cv_y=-6;
    l[57].cv_x=3;l[57].cv_y=-7;
    l[58].cv_x=4;l[58].cv_y=-7;
    l[59].cv_x=4;l[59].cv_y=-8;

     //六边形上半部分
    l[60].cv_x=-4;l[60].cv_y=4;
    l[61].cv_x=-3;l[61].cv_y=4;
    l[62].cv_x=-2;l[62].cv_y=4;
    l[63].cv_x=-1;l[63].cv_y=4;
    l[64].cv_x=0;l[64].cv_y=4;
    l[65].cv_x=-4;l[65].cv_y=3;
    l[66].cv_x=-3;l[66].cv_y=3;
    l[67].cv_x=-2;l[67].cv_y=3;
    l[68].cv_x=-1;l[68].cv_y=3;
    l[69].cv_x=0;l[69].cv_y=3;
    l[70].cv_x=1;l[70].cv_y=3;
    l[71].cv_x=-4;l[71].cv_y=2;
    l[72].cv_x=-3;l[72].cv_y=2;
    l[73].cv_x=-2;l[73].cv_y=2;
    l[74].cv_x=-1;l[74].cv_y=2;
    l[75].cv_x=0;l[75].cv_y=2;
    l[76].cv_x=1;l[76].cv_y=2;
    l[77].cv_x=2;l[77].cv_y=2;
    l[78].cv_x=-4;l[78].cv_y=1;
    l[79].cv_x=-3;l[79].cv_y=1;
    l[80].cv_x=-2;l[80].cv_y=1;
    l[81].cv_x=-1;l[81].cv_y=1;
    l[82].cv_x=0;l[82].cv_y=1;
    l[83].cv_x=1;l[83].cv_y=1;
    l[84].cv_x=2;l[84].cv_y=1;
    l[85].cv_x=3;l[85].cv_y=1;
    l[86].cv_x=-4;l[86].cv_y=0;
    l[87].cv_x=-3;l[87].cv_y=0;
    l[88].cv_x=-2;l[88].cv_y=0;
    l[89].cv_x=-1;l[89].cv_y=0;
    l[90].cv_x=0;l[90].cv_y=0;
    l[91].cv_x=1;l[91].cv_y=0;
    l[92].cv_x=2;l[92].cv_y=0;
    l[93].cv_x=3;l[93].cv_y=0;
    l[94].cv_x=4;l[94].cv_y=0;

     //六边形下半部分
    l[95].cv_x=0;l[95].cv_y=-4;
    l[96].cv_x=1;l[96].cv_y=-4;
    l[97].cv_x=2;l[97].cv_y=-4;
    l[98].cv_x=3;l[98].cv_y=-4;
    l[99].cv_x=4;l[99].cv_y=-4;
    l[100].cv_x=-1;l[100].cv_y=-3;
    l[101].cv_x=0;l[101].cv_y=-3;
    l[102].cv_x=1;l[102].cv_y=-3;
    l[103].cv_x=2;l[103].cv_y=-3;
    l[104].cv_x=3;l[104].cv_y=-3;
    l[105].cv_x=4;l[105].cv_y=-3;
    l[106].cv_x=-2;l[106].cv_y=-2;
    l[107].cv_x=-1;l[107].cv_y=-2;
    l[108].cv_x=0;l[108].cv_y=-2;
    l[109].cv_x=1;l[109].cv_y=-2;
    l[110].cv_x=2;l[110].cv_y=-2;
    l[111].cv_x=3;l[111].cv_y=-2;
    l[112].cv_x=4;l[112].cv_y=-2;
    l[113].cv_x=-3;l[113].cv_y=-1;
    l[114].cv_x=-2;l[114].cv_y=-1;
    l[115].cv_x=-1;l[115].cv_y=-1;
    l[116].cv_x=0;l[116].cv_y=-1;
    l[117].cv_x=1;l[117].cv_y=-1;
    l[118].cv_x=2;l[118].cv_y=-1;
    l[119].cv_x=3;l[119].cv_y=-1;
    l[120].cv_x=4;l[120].cv_y=-1;


}
void ServerWindow::init_cvrt_st()
{
    //No.1
    s[0].cv_xs=-8;s[0].cv_xs=4;
    s[1].cv_xs=-7;s[1].cv_xs=4;
    s[2].cv_xs=-6;s[2].cv_xs=4;
    s[3].cv_xs=-5;s[3].cv_xs=4;
    s[4].cv_xs=-7;s[4].cv_xs=3;
    s[5].cv_xs=-6;s[5].cv_xs=3;
    s[6].cv_xs=-5;s[6].cv_xs=3;
    s[7].cv_xs=-6;s[7].cv_xs=2;
    s[8].cv_xs=-5;s[8].cv_xs=2;
    s[9].cv_xs=-5;s[9].cv_xs=1;

    //No.2
    s[10].cv_xs=-4;s[10].cv_ys=8;
    s[11].cv_xs=-4;s[11].cv_ys=7;
    s[12].cv_xs=-3;s[12].cv_ys=7;
    s[13].cv_xs=-4;s[13].cv_ys=6;
    s[14].cv_xs=-3;s[14].cv_ys=6;
    s[15].cv_xs=-2;s[15].cv_ys=6;
    s[16].cv_xs=-4;s[16].cv_ys=5;
    s[17].cv_xs=-3;s[17].cv_ys=5;
    s[18].cv_xs=-2;s[18].cv_ys=5;
    s[19].cv_xs=-1;s[19].cv_ys=5;

    //No.3
    s[20].cv_xs=1;s[20].cv_ys=4;
    s[21].cv_xs=2;s[21].cv_ys=4;
    s[22].cv_xs=3;s[22].cv_ys=4;
    s[23].cv_xs=4;s[23].cv_ys=4;
    s[24].cv_xs=2;s[24].cv_ys=3;
    s[25].cv_xs=3;s[25].cv_ys=3;
    s[26].cv_xs=4;s[26].cv_ys=3;
    s[27].cv_xs=3;s[27].cv_ys=2;
    s[28].cv_xs=4;s[28].cv_ys=2;
    s[29].cv_xs=4;s[29].cv_ys=1;

    //No.4
    s[30].cv_xs=5;s[30].cv_ys=-1;
    s[31].cv_xs=5;s[31].cv_ys=-2;
    s[32].cv_xs=6;s[32].cv_ys=-2;
    s[33].cv_xs=5;s[33].cv_ys=-3;
    s[34].cv_xs=6;s[34].cv_ys=-3;
    s[35].cv_xs=7;s[35].cv_ys=-3;
    s[36].cv_xs=5;s[36].cv_ys=-4;
    s[37].cv_xs=6;s[37].cv_ys=-4;
    s[38].cv_xs=7;s[38].cv_ys=-4;
    s[39].cv_xs=8;s[39].cv_ys=-4;

    //No.5
    s[40].cv_xs=1;s[40].cv_ys=-5;
    s[41].cv_xs=2;s[41].cv_ys=-6;
    s[42].cv_xs=3;s[42].cv_ys=-6;
    s[43].cv_xs=4;s[43].cv_ys=-7;
    s[44].cv_xs=2;s[44].cv_ys=-7;
    s[45].cv_xs=3;s[45].cv_ys=-7;
    s[46].cv_xs=4;s[46].cv_ys=-8;
    s[47].cv_xs=3;s[47].cv_ys=-8;
    s[48].cv_xs=4;s[48].cv_ys=-8;
    s[49].cv_xs=4;s[49].cv_ys=-8;


    //No.6
    s[50].cv_xs=-4;s[50].cv_ys=-1;
    s[51].cv_xs=-4;s[51].cv_ys=-2;
    s[52].cv_xs=-3;s[52].cv_ys=-2;
    s[53].cv_xs=-4;s[53].cv_ys=-3;
    s[54].cv_xs=-3;s[54].cv_ys=-3;
    s[55].cv_xs=-2;s[55].cv_ys=-3;
    s[56].cv_xs=-4;s[56].cv_ys=-4;
    s[57].cv_xs=-3;s[57].cv_ys=-4;
    s[58].cv_xs=-2;s[58].cv_ys=-4;
    s[59].cv_xs=-1;s[59].cv_ys=-4;
}

bool ServerWindow::getCvt(int x,int y,int &t)
{
    for(t=0;t<121;t++)
    {
        if(l[t].cv_x==x&&l[t].cv_y==y)
            return true;
    }
    return false;
}

void ServerWindow::move_Rt(int x, int y)
{
//        int mode=change_mark(mark);
        int B_loc;
//        qDebug()<<x<<y;
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
    //    for(i=0;i<60;i++)
    //    {
    //        if(s[i].x0==l[B_loc].x1&&s[i].y0==l[B_loc].y1)
    //            break ;
    //    }
    //    if(i<60)
    //    {
    //        clickid=i;
    //    }

        //三种情况的选择
        if(pattern==6) //在六个棋子的位置中搜索        //一号bug位
        {
            for(i=0;i<60;i++)
            {
                if(s[i].x0==l[B_loc].x1&&s[i].y0==l[B_loc].y1)
                    break ;
            }
            if(i<60/*&&s[i].color==mode*/)
            {
//                next=true;



    //            clickid=i;

                if(i>=0&&i<10&&(state%pattern==5))
                {
                    clickid=i;
    //                state++;
    //                state%=pattern;
                }
                if(i>=10&&i<20&&(state%pattern==0))
                {
                    clickid=i;
    //                state++;
    //                state%=pattern;
                }
                if(i>=20&&i<30&&(state%pattern==1))
                {
                    clickid=i;
    //                state++;
    //                state%=pattern;
                }
                if(i>=30&&i<40&&(state%pattern==2))
                {
                    clickid=i;
    //                state++;
    //                state%=pattern;
                }
                if(i>=40&&i<50&&(state%pattern==3))
                {
                    clickid=i;
    //                state++;
    //                state%=pattern;
                }
                if(i>=50&&i<60&&(state%pattern==4))
                {
                    clickid=i;
    //                state++;
    //                state%=pattern;
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
            if(cnt1/*&&s[i].color==mode*/)
            {
    //            clickid=i;
                if(i>=10&&i<20&&(state%pattern==0))
                {
                    clickid=i;
    //                state++;
    //                state%=pattern;
                }
                if(i>=30&&i<40&&(state%pattern==1))
                {
                    clickid=i;
    //                state++;
    //                state%=pattern;
                }
                if(i>=50&&i<60&&(state%pattern==2))
                {
                    clickid=i;
    //                state++;
    //                state%=pattern;
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
            if(cnt1/*&&s[i].color==mode*/)
            {
    //            clickid=i;
                if(i>=10&&i<20&&(state%pattern==0))
                {
                    clickid=i;
    //                state++;
    //                state%=pattern;
                }
                if(i>=40&&i<50&&(state%pattern==1))
                {
                    clickid=i;
    //                state++;
    //                state%=pattern;
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
                    /*state++;
                    state%=pattern;*/  //轮转
                    occupied[B_loc]=true;
                    getPos(s[_selected_id].x0,s[_selected_id].y0,i);
                    occupied[i]=false;
                    s[_selected_id].x0=l[B_loc].x1;
                    s[_selected_id].y0=l[B_loc].y1;
                    current_x=s[_selected_id].x0;  //此行及下面一行标记当前选中棋子，防止被wipe_dbfs()覆盖
                    current_y=s[_selected_id].y0;
                    restart_time();
                }
             }
                _selected_id=-1;

                update(); //注意这两行代码，后期可能会有错误出现

    //        qDebug()<<s[_selected_id].x0<<s[_selected_id].y0;
    //        _selected_id=-1;

    //        update();
        }

}

int ServerWindow::change_mark(char mark)
{
    if(mark=='A')
        return 1;
    else if(mark=='B')
        return 2;
    else if(mark=='C')
        return 3;
    else if(mark=='D')
        return 4;
    else if(mark=='E')
        return 5;
    else
        return 0;
}

void ServerWindow::change_char_to_int(char *p)
{

    memset(route,0,sizeof(route));

    routeLength=0;
    cnt=0;


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
//        print();
}

void ServerWindow::print()
{
    for(int i=0;i<routeLength;i++)
    {
        qDebug()<<route[i];
    }
    return ;
}

void ServerWindow::init_psr()
{
    for(int i=0;i<121;i++)
    {
        l[i].psr.clear();
    }
}

void ServerWindow::clear_psr()
{
    for(int i=0;i<121;i++)
    {
            l[i].psr.clear();
    }
}

void ServerWindow::init_psr_set()
{
    for(int i=0;i<121;i++)
    {
        l[i].psr_set=false;
    }
}

void ServerWindow::change_state()
{
    if(next)
    {
        state++;
        state%=pattern;
        next=false;
    }
}

void ServerWindow::restart_time()
{
    count_down = 15;
    time_id_clock_down = startTimer(1000);
}

void ServerWindow::wipe_player(int id)
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
    if(num-failed_num == 1)
    {
        QString list="";
        for(int i=0;i<num;i++)
        {
            int t = 0;
            for(int j=0;j<failed_num;j++)
            {
                t+=(i == fall_players[j]);
            }
            if(!t)
            {
                list = user[i];
                break;
            }
        }
        int u = failed_num;


//        for(int i = 0;i<failed_num;i++)
//        {
//            qDebug()<<user[fall_players[i]]<<"Debug()";
//        }


        for(int i=u-1;i>=0;i--)
        {
            list+=' '+user[fall_players[i]];
        }
        NetworkData data(OPCODE::END_GAME_OP,list,"");
        for(int i=0;i<num;i++)
        {
            server->send(_socket[i],data);
        }
        this->close();
    }
}
