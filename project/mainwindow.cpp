#include "mainwindow.h"
#include<QPainter>
#include<QDebug>
#include<QMouseEvent>
#include<iostream>
#include<string.h>
#include<QString>
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>
#include<QTimerEvent>
#include<QTimer>
#include <QRegExp>
#include<QRegularExpression>
extern int me;

//MainWindow::MainWindow(QWidget *parent)
//    : QMainWindow(parent)
//{
//    resize(1300,1200);
//    _selected_id=-1;
//    markstone();
//    init_occupied();
//    init_Possible_Location();
//}

MainWindow::MainWindow()
{
    pattern = 0;
    state=0;
    resize(1300,1200);
    _selected_id=-1;
//    markstone();
//    init_occupied();
    init_Possible_Location();

    red_init_cvrt_Loc();
    init_psr();
    init_psr_set();


//    QPushButton *btn = new QPushButton;
//    btn->setParent(this);
//    btn->move(1400,850);
//    btn->resize(100,40);
//    QLineEdit *edit = new QLineEdit;
//    edit->setParent(this);
//    edit->resize(400,40);
//    edit->move(this->width()/2+500,this->height()/2+200);
//    QLineEdit *edit1 = new QLineEdit;
//    edit1->setParent(this);
//    edit1->resize(400,40);
//    edit1->move(this->width()/2+500,this->height()/2+150);
//    connect(btn,&QPushButton::clicked,[=](){
//        QString a=edit1->text();
//        edit1->clear();
//        char *au;
//        QByteArray ba2 = a.toLatin1();
//        au=ba2.data();
//        char ai=*au;
//        Area_num=change_mark(ai);
//        QString str = edit->text();
//        str.remove(QRegularExpression("\\s"));
//        char*  ch;
//        QByteArray ba = str.toLatin1(); // must
//        ch=ba.data();
//        change_char_to_int(ch);
//        edit->clear();
//        time_id1 = startTimer(700);
//    });



//    QPushButton * AI_openBtn = new QPushButton(this);
//    AI_openBtn->move(1300,300);
//    AI_openBtn->show();
//    QPushButton * AI_closeBtn = new QPushButton(this);
//    AI_closeBtn->move(1300,400);
//    AI_closeBtn->show();

    cd_former_route = false;
}

void MainWindow::timerEvent(QTimerEvent *ev)
{
    if(ev->timerId() == time_id1)
    {
        if(cnt<routeLength-2)
        {
            move_Rt(route[cnt],route[cnt+1]);
            move_Rt(route[cnt+2],route[cnt+3]);
            cnt+=2;
        }
        else if(cnt == routeLength-2)
        {
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
            cnt+=2;
            return ;
        }
    }
//    else
//    {
//        state++;
//        state%=pattern;
//        qDebug()<<state;
//        return ;
//    }
    return ;
}

void MainWindow::paintEvent(QPaintEvent *)
{

    QPainter painter(this);

    if(pattern == 6)
    {
    if(state == 0)
       painter.drawPixmap(rect(),QPixmap(":/pic/picture/0_chess.jpg"),QRect());
    else if(state == 1)
        painter.drawPixmap(rect(),QPixmap(":/pic/picture/1_chess.jpg"),QRect());
    else if(state == 2)
        painter.drawPixmap(rect(),QPixmap(":/pic/picture/2_chess.jpg"),QRect());
    else if(state == 3)
        painter.drawPixmap(rect(),QPixmap(":/pic/picture/3_chess.jpg"),QRect());
    else if(state == 4)
        painter.drawPixmap(rect(),QPixmap(":/pic/picture/4_chess.jpg"),QRect());
    else if(state == 5)
        painter.drawPixmap(rect(),QPixmap(":/pic/picture/5_chess.jpg"),QRect());
    }
    else if(pattern == 3)
    {
        if(state == 0)
           painter.drawPixmap(rect(),QPixmap(":/pic/picture/0_chess.jpg"),QRect());
        else if(state == 1)
            painter.drawPixmap(rect(),QPixmap(":/pic/picture/2_chess.jpg"),QRect());
        else if(state == 2)
            painter.drawPixmap(rect(),QPixmap(":/pic/picture/4_chess.jpg"),QRect());
    }
    else if(pattern == 2)
    {
        if(state == 0)
           painter.drawPixmap(rect(),QPixmap(":/pic/picture/0_chess.jpg"),QRect());
        else if(state == 1)
            painter.drawPixmap(rect(),QPixmap(":/pic/picture/3_chess.jpg"),QRect());
    }


    painter.setRenderHint(QPainter::Antialiasing);
    static const QPointF points1[3] = { //1号三角形
         QPointF(200.0, 300.0),
         QPointF(500.0, 300.0),
         QPointF(350.0, 500.0),
    };
//    painter.setBrush(QColor(255,255,0));
//    painter.drawPolygon(points1,3);
    painter.setBrush(Qt::NoBrush);

    static const QPointF points6[3] = { //6号三角形
         QPointF(200.0, 700.0),
         QPointF(500.0, 700.0),
         QPointF(350.0, 500.0),
    };
//    painter.setBrush(QColor(0,0,255));
//    painter.drawPolygon(points6,3);
    painter.setBrush(Qt::NoBrush);

    static const QPointF points2[3] = { //2号三角形
         QPointF(650.0, 100.0),
         QPointF(500.0, 300.0),
         QPointF(800.0, 300.0),
    };
//    painter.setBrush(QColor(255,0,255));
//    painter.drawPolygon(points2,3);
    painter.setBrush(Qt::NoBrush);

    static const QPointF points3[3] = { //3号三角形
         QPointF(800.0, 300.0),
         QPointF(1100.0, 300.0),
         QPointF(950.0, 500.0),
    };
//    painter.setBrush(QColor(0,255,255));
//    painter.drawPolygon(points3,3);
    painter.setBrush(Qt::NoBrush);

    static const QPointF points4[3] = { //4号三角形
         QPointF(800.0, 700.0),
         QPointF(1100.0, 700.0),
         QPointF(950.0, 500.0),
    };
//    painter.setBrush(QColor(255,0,0));
//    painter.drawPolygon(points4,3);
    painter.setBrush(Qt::NoBrush);

    static const QPointF points5[3] = { //5号三角形
         QPointF(650.0, 900.0),
         QPointF(500.0, 700.0),
         QPointF(800.0, 700.0),
    };
//    painter.setBrush(QColor(0,255,0));
//    painter.drawPolygon(points5,3);
    painter.setBrush(Qt::NoBrush);

    if(single_mode)
    {
        painter.setBrush(QColor(255,255,0));  //黄
        painter.drawPolygon(points1,3);
        painter.setBrush(QColor(0,0,255));   //蓝
        painter.drawPolygon(points6,3);
        painter.setBrush(QColor(255,0,255));  //紫
        painter.drawPolygon(points2,3);
        painter.setBrush(QColor(0,255,255));    //青
        painter.drawPolygon(points3,3);
        painter.setBrush(QColor(255,0,0));   //红
        painter.drawPolygon(points4,3);
        painter.setBrush(QColor(0,255,0));   //绿
        painter.drawPolygon(points5,3);
    }

    else
    {

        if(pattern == 2)
        {
            if(me == 0)
            {
                painter.setBrush(QColor(255,0,0));
                painter.drawPolygon(points1,3);
                painter.setBrush(QColor(0,255,255));
                painter.drawPolygon(points6,3);
                painter.setBrush(QColor(0,255,0));
                painter.drawPolygon(points2,3);
                painter.setBrush(QColor(0,0,255));
                painter.drawPolygon(points3,3);
                painter.setBrush(QColor(255,255,0));
                painter.drawPolygon(points4,3);
                painter.setBrush(QColor(255,0,255));
                painter.drawPolygon(points5,3);
            }
            else if(me == 1)
            {
                painter.setBrush(QColor(255,255,0));  //黄
                painter.drawPolygon(points1,3);
                painter.setBrush(QColor(0,0,255));   //蓝
                painter.drawPolygon(points6,3);
                painter.setBrush(QColor(255,0,255));  //紫
                painter.drawPolygon(points2,3);
                painter.setBrush(QColor(0,255,255));    //青
                painter.drawPolygon(points3,3);
                painter.setBrush(QColor(255,0,0));   //红
                painter.drawPolygon(points4,3);
                painter.setBrush(QColor(0,255,0));   //绿
                painter.drawPolygon(points5,3);
            }
        }
        if(pattern == 3)
        {
            if(me == 0)
            {
                painter.setBrush(QColor(255,0,0));
                painter.drawPolygon(points1,3);
                painter.setBrush(QColor(0,255,255));
                painter.drawPolygon(points6,3);
                painter.setBrush(QColor(0,255,0));
                painter.drawPolygon(points2,3);
                painter.setBrush(QColor(0,0,255));
                painter.drawPolygon(points3,3);
                painter.setBrush(QColor(255,255,0));
                painter.drawPolygon(points4,3);
                painter.setBrush(QColor(255,0,255));
                painter.drawPolygon(points5,3);
            }
            else if(me == 1)
            {
                painter.setBrush(QColor(0,0,255));
                painter.drawPolygon(points1,3);
                painter.setBrush(QColor(0,255,0));
                painter.drawPolygon(points6,3);
                painter.setBrush(QColor(255,255,0));
                painter.drawPolygon(points2,3);
                painter.setBrush(QColor(255,0,255));
                painter.drawPolygon(points3,3);
                painter.setBrush(QColor(0,255,255));
                painter.drawPolygon(points4,3);
                painter.setBrush(QColor(255,0,0));
                painter.drawPolygon(points5,3);

            }
            else if(me == 2)
            {
                painter.setBrush(QColor(255,0,255));
                painter.drawPolygon(points1,3);
                painter.setBrush(QColor(255,255,0));
                painter.drawPolygon(points6,3);
                painter.setBrush(QColor(0,255,255));
                painter.drawPolygon(points2,3);
                painter.setBrush(QColor(255,0,0));
                painter.drawPolygon(points3,3);
                painter.setBrush(QColor(0,255,0));
                painter.drawPolygon(points4,3);
                painter.setBrush(QColor(0,0,255));
                painter.drawPolygon(points5,3);



            }
        }
        else if(pattern == 6)
        {
            if(me == 0)
            {
                painter.setBrush(QColor(255,0,0));
                painter.drawPolygon(points1,3);
                painter.setBrush(QColor(0,255,255));
                painter.drawPolygon(points6,3);
                painter.setBrush(QColor(0,255,0));
                painter.drawPolygon(points2,3);
                painter.setBrush(QColor(0,0,255));
                painter.drawPolygon(points3,3);
                painter.setBrush(QColor(255,255,0));
                painter.drawPolygon(points4,3);
                painter.setBrush(QColor(255,0,255));
                painter.drawPolygon(points5,3);
            }
            else if(me == 1)
            {
                painter.setBrush(QColor(0,255,0));
                painter.drawPolygon(points1,3);
                painter.setBrush(QColor(255,0,0));
                painter.drawPolygon(points6,3);
                painter.setBrush(QColor(0,0,255));
                painter.drawPolygon(points2,3);
                painter.setBrush(QColor(255,255,0));
                painter.drawPolygon(points3,3);
                painter.setBrush(QColor(255,0,255));
                painter.drawPolygon(points4,3);
                painter.setBrush(QColor(0,255,255));
                painter.drawPolygon(points5,3);
            }
            else if(me == 2)
            {
                painter.setBrush(QColor(0,0,255));
                painter.drawPolygon(points1,3);
                painter.setBrush(QColor(0,255,0));
                painter.drawPolygon(points6,3);
                painter.setBrush(QColor(255,255,0));
                painter.drawPolygon(points2,3);
                painter.setBrush(QColor(255,0,255));
                painter.drawPolygon(points3,3);
                painter.setBrush(QColor(0,255,255));
                painter.drawPolygon(points4,3);
                painter.setBrush(QColor(255,0,0));
                painter.drawPolygon(points5,3);
            }
            else if(me == 3)
            {
                painter.setBrush(QColor(255,255,0));  //黄
                painter.drawPolygon(points1,3);
                painter.setBrush(QColor(0,0,255));   //蓝
                painter.drawPolygon(points6,3);
                painter.setBrush(QColor(255,0,255));  //紫
                painter.drawPolygon(points2,3);
                painter.setBrush(QColor(0,255,255));    //青
                painter.drawPolygon(points3,3);
                painter.setBrush(QColor(255,0,0));   //红
                painter.drawPolygon(points4,3);
                painter.setBrush(QColor(0,255,0));   //绿
                painter.drawPolygon(points5,3);
            }
            else if(me == 4)
            {
                painter.setBrush(QColor(255,0,255));
                painter.drawPolygon(points1,3);
                painter.setBrush(QColor(255,255,0));
                painter.drawPolygon(points6,3);
                painter.setBrush(QColor(0,255,255));
                painter.drawPolygon(points2,3);
                painter.setBrush(QColor(255,0,0));
                painter.drawPolygon(points3,3);
                painter.setBrush(QColor(0,255,0));
                painter.drawPolygon(points4,3);
                painter.setBrush(QColor(0,0,255));
                painter.drawPolygon(points5,3);
            }
            else if(me == 5)
            {
                painter.setBrush(QColor(0,255,255));
                painter.drawPolygon(points1,3);
                painter.setBrush(QColor(255,0,255));
                painter.drawPolygon(points6,3);
                painter.setBrush(QColor(255,0,0));
                painter.drawPolygon(points2,3);
                painter.setBrush(QColor(0,255,0));
                painter.drawPolygon(points3,3);
                painter.setBrush(QColor(0,0,255));
                painter.drawPolygon(points4,3);
                painter.setBrush(QColor(255,255,0));
                painter.drawPolygon(points5,3);
            }
        }
    }

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

//    if(if_overtime)   //此段代码是将超时判负的玩家棋子染成白色
//    {
//        QPainter p1(this);
//        //染色
//        p1.setBrush(Qt::white);
//        for(int i=0;i<10;i++)
//        {
//            p1.drawEllipse(QPoint(l[wipe_stones[i]].x1,l[wipe_stones[i]].y1),r,r);
//        }
//        if_overtime = false;
//    }
    if(cd_former_route)
    {
        for(int i=0;i<drawed_route_length;i=i+2)
        {
            int t;
            getCvt(drawed_route[i],drawed_route[i+1],t);
//            qDebug()<<l[t].cv_x<<" "<<l[t].cv_y<<"mainwindow paintevent";
            painter.drawPixmap(l[t].x1-r-11.5,l[t].y1-r-3,4*r,2.5*r,QPixmap(":/stone2/0.png"));
        }
        cd_former_route = false;
    }

}

void MainWindow::markstone()
{
    if(single_mode)
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
    else
    {
    if(pattern == 2)
    {
        if(me == 1)
        {
            int count=10;
            //            for(int i=1;i<=4;i++) //1号三角形 (254,324)
            //            {
            //                for(int j=1;j<=5-i;j++)
            //                {
            //                    s[count].x0=254+(i-1)*33+(j-1)*66;
            //                    s[count].y0=324+(i-1)*44;
            //                    s[count].color=5;

            //                    count++;
            //                }
            //            }

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

            //            for(int i=1;i<=4;i++) //3号三角形 (848,324)
            //            {
            //                for(int j=1;j<=5-i;j++)
            //                {
            //                    s[count].x0=848+(i-1)*33+(j-1)*66;
            //                    s[count].y0=324+(i-1)*44;
            //                    s[count].color=1;

            //                    count++;
            //                }
            //            }


            //            for(int i=1;i<=4;i++) //4号三角形 (254,324)
            //            {
            //                for(int j=1;j<=i;j++)
            //                {
            //                    s[count].x0=947-(i-1)*33+(j-1)*66;
            //                    s[count].y0=544+(i-1)*44;
            //                    s[count].color=2;

            //                    count++;
            //                }
            //            }

            count = 40;

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


            //            for(int i=1;i<=4;i++) //6号三角形 (353,544)
            //            {
            //                for(int j=1;j<=i;j++)
            //                {
            //                    s[count].x0=353-(i-1)*33+(j-1)*66;
            //                    s[count].y0=544+(i-1)*44;
            //                    s[count].color=4;

            //                    count++;
            //                }
            //            }
        }
        else if(me == 0)
        {
            int count = 10;
            //            for(int i=1;i<=4;i++) //0-1号三角形
            //            {
            //                for(int j=1;j<=i;j++)
            //                {
            //                    s[count].x0=947-(i-1)*33+(j-1)*66;
            //                    s[count].y0=544+(i-1)*44;
            //                    s[count].color=2;

            //                    count++;
            //                }
            //            }

            for(int i=1;i<=4;i++) //紫棋三角形
            {
                for(int j=1;j<=5-i;j++)
                {
                    s[count].x0=551+(i-1)*33+(j-1)*66;
                    s[count].y0=720+(i-1)*44;
                    s[count].color=3;

                    count++;
                }
            }

            //            for(int i=1;i<=4;i++) //0-3号三角形
            //            {
            //                for(int j=1;j<=i;j++)
            //                {
            //                    s[count].x0=353-(i-1)*33+(j-1)*66;
            //                    s[count].y0=544+(i-1)*44;
            //                    s[count].color=4;

            //                    count++;
            //                }
            //            }

            //            for(int i=1;i<=4;i++) //0-4号三角形
            //            {
            //                for(int j=1;j<=5-i;j++)
            //                {
            //                    s[count].x0=254+(i-1)*33+(j-1)*66;
            //                    s[count].y0=324+(i-1)*44;
            //                    s[count].color=5;

            //                    count++;
            //                }
            //            }

            count = 40;
            for(int i=1;i<=4;i++) //绿棋三角形
            {
                for(int j=1;j<=i;j++)
                {
                    s[count].x0=650-(i-1)*33+(j-1)*66;
                    s[count].y0=148+(i-1)*44;
                    s[count].color=0;

                    count++;
                }
            }

            //            for(int i=1;i<=4;i++) //0-6号三角形
            //            {
            //                for(int j=1;j<=5-i;j++)
            //                {
            //                    s[count].x0=848+(i-1)*33+(j-1)*66;
            //                    s[count].y0=324+(i-1)*44;
            //                    s[count].color=1;

            //                    count++;
            //                }
            //            }
        }

    }
    else if(pattern == 3)
    {
        if(me == 0)
        {
            int count = 10;
            for(int i=1;i<=4;i++)     //紫棋三角形 (551,720)
            {
                for(int j=1;j<=5-i;j++)
                {
                    s[count].x0=551+(i-1)*33+(j-1)*66;
                    s[count].y0=720+(i-1)*44;
                    s[count].color=3;

                    count++;
                }
            }

            count = 30;
            for(int i=1;i<=4;i++)    //红棋三角形 (254,324)
            {
                for(int j=1;j<=5-i;j++)
                {
                    s[count].x0=254+(i-1)*33+(j-1)*66;
                    s[count].y0=324+(i-1)*44;
                    s[count].color=5;

                    count++;
                }
            }

            count = 50;

            for(int i=1;i<=4;i++)     //蓝棋三角形 (848,324)
            {
                for(int j=1;j<=5-i;j++)
                {
                    s[count].x0=848+(i-1)*33+(j-1)*66;
                    s[count].y0=324+(i-1)*44;
                    s[count].color=1;

                    count++;
                }
            }
        }
        else if(me == 1)
        {
            int count = 10;
            for(int i=1;i<=4;i++)      //紫棋三角形 (848,324)  （原青棋位置）
            {
                for(int j=1;j<=5-i;j++)
                {
                    s[count].x0=848+(i-1)*33+(j-1)*66;
                    s[count].y0=324+(i-1)*44;
                    s[count].color=1;

                    count++;
                }
            }

            count = 30;

            for(int i=1;i<=4;i++)     //红棋三角形 (551,720)   （原绿棋位置）
            {
                for(int j=1;j<=5-i;j++)
                {
                    s[count].x0=551+(i-1)*33+(j-1)*66;
                    s[count].y0=720+(i-1)*44;
                    s[count].color=3;

                    count++;
                }
            }

            count = 50;

            for(int i=1;i<=4;i++) //蓝棋三角形 (254,324)    （原黄棋位置）
            {
                for(int j=1;j<=5-i;j++)
                {
                    s[count].x0=254+(i-1)*33+(j-1)*66;
                    s[count].y0=324+(i-1)*44;
                    s[count].color=5;

                    count++;
                }
            }
        }
        else if(me == 2)
        {
            int count = 10;

            for(int i=1;i<=4;i++) //蓝棋三角形 (254,324)    （原黄棋位置）
            {
                for(int j=1;j<=5-i;j++)
                {
                    s[count].x0=254+(i-1)*33+(j-1)*66;
                    s[count].y0=324+(i-1)*44;
                    s[count].color=5;

                    count++;
                }
            }

            count = 30;

            for(int i=1;i<=4;i++)      //红棋三角形 (848,324)  （原青棋位置）
            {
                for(int j=1;j<=5-i;j++)
                {
                    s[count].x0=848+(i-1)*33+(j-1)*66;
                    s[count].y0=324+(i-1)*44;
                    s[count].color=1;

                    count++;
                }
            }

            count = 50;

            for(int i=1;i<=4;i++)     //蓝棋三角形 (551,720)   （原绿棋位置）
            {
                for(int j=1;j<=5-i;j++)
                {
                    s[count].x0=551+(i-1)*33+(j-1)*66;
                    s[count].y0=720+(i-1)*44;
                    s[count].color=3;

                    count++;
                }
            }
        }
    }
    else if(pattern == 6)
    {
        if(me == 0)
        {
            int count = 0;

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

        }
        else if(me == 1)
        {
            int count = 0;

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

        }
        else if(me == 2)
        {
            int count = 0;

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
        }
        else if(me == 3)
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
        else if(me == 4)
        {
            int count = 0;

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

        }
        else if(me == 5)
        {
            int count = 0;

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
        }
    }
    }
}



QPoint MainWindow::center(int x, int y)
{
    return QPoint(x,y);
}

QPoint MainWindow::center(int id)
{
    return QPoint(s[id].x0,s[id].y0);
}

void MainWindow::drawstone(QPainter &painter,int i)
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

bool MainWindow::getPos(QPoint pt, int &B_loc)
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

bool MainWindow::getPos(int x0,int y0, int &B_loc)
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

void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{
//    change_state(); //因为输入版本的移动函数并未在移动完成后进行轮转操作，只是标记了轮转状态为可轮转，所以只要检测到可轮转，无论是在输入端还是点击端都要轮转
    QPoint pt=ev->pos();
    int B_loc;
    bool clk=getPos(pt,B_loc);
    qDebug()<<l[B_loc].cv_x<<l[B_loc].cv_y;
    if(clk==false&&_selected_id!=-1)
    {
        _selected_id=-1;
//        next=false;
        update();
        return ;
    }
    if(clk==false)
    {
//        next=false;
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
        if(i<60)
        {
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
        if(cnt1)
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
//                next=true; //这个部分的函数也要更新next，因为点击和输入会混用

                psr_mt=l[B_loc].psr;
                int i;
                state++;
                state%=pattern;
//                qDebug()<<state;
                occupied[B_loc]=true;
                getPos(s[_selected_id].x0,s[_selected_id].y0,i);
                occupied[i]=false;
                s[_selected_id].x0=l[B_loc].x1;
                s[_selected_id].y0=l[B_loc].y1;
                current_x=s[_selected_id].x0;  //此行及下面一行标记当前选中棋子，防止被wipe_dbfs()覆盖
                current_y=s[_selected_id].y0;
//                qDebug()<<psr_mt;
                psr_m.clear();
//                qDebug()<<l[B_loc].psr;
//                clear_psr();
//                qDebug()<<l[B_loc].psr<<"ACM";



            }
         }
            _selected_id=-1;

            update(); //注意这两行代码，后期可能会有错误出现

//        qDebug()<<s[_selected_id].x0<<s[_selected_id].y0;
//        _selected_id=-1;

//        update();
    }
}

void MainWindow::init_occupied() //标记哪些棋盘坐标被棋子占领
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
         if(single_mode)
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
                 occupied[i]=false;
             }
             for(int i=20;i<40;i++)
             {
                 occupied[i]=false;
             }
             for(int i=10;i<20;i++)
             {
                 occupied[i]=true;
             }
             for(int i=40;i<60;i++)
             {
                 occupied[i]=true;
             }
             for(int i=60;i<121;i++)
             {
                 occupied[i]=false;
             }
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

void MainWindow::init_Possible_Location() //标记dbfs()时可能到达的位置，为后续染色做标记
{
    for(int i=0;i<121;i++)
    {
        Possible_Location[i]=false;
    }
}

bool MainWindow::canMove(int x1,int y1,int x2,int y2) //判断移动是否合法
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

void MainWindow::dbfs(int count ,int i,QPainter &painter,QString a) //这里的i是棋盘的坐标编号
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
        p.append(s1+" ");
        QString s2= QString::number(l[i].cv_y,10);
        p.append(s2);
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

void MainWindow::wipe_dbfs(QPainter &painter) //将dbfs()染的Possibel_Location染回白色，并重置Posbible_Location数组
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
void MainWindow::green_init_cvrt_Loc()
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

void MainWindow::blue_init_cvrt_Loc()
{
    l[0].cv_x = 4;l[0].cv_y = 4;
    l[1].cv_x = 3;l[1].cv_y = 4;
    l[2].cv_x = 4;l[2].cv_y = 3;
    l[3].cv_x = 2;l[3].cv_y = 4;
    l[4].cv_x = 3;l[4].cv_y = 3;
    l[5].cv_x = 4;l[5].cv_y = 2;
    l[6].cv_x = 1;l[6].cv_y = 4;
    l[7].cv_x = 2;l[7].cv_y = 3;
    l[8].cv_x = 3;l[8].cv_y = 2;
    l[9].cv_x = 4;l[9].cv_y = 1;

    l[10].cv_x = -4;l[10].cv_y = 8;
    l[11].cv_x = -3;l[11].cv_y = 7;
    l[12].cv_x = -2;l[12].cv_y = 6;
    l[13].cv_x = -1;l[13].cv_y = 5;
    l[14].cv_x = -4;l[14].cv_y = 7;
    l[15].cv_x = -3;l[15].cv_y = 6;
    l[16].cv_x = -2;l[16].cv_y = 5;
    l[17].cv_x = -4;l[17].cv_y = 6;
    l[18].cv_x = -3;l[18].cv_y = 5;
    l[19].cv_x = -4;l[19].cv_y = 5;

    l[20].cv_x = 4;l[20].cv_y = -5;
    l[21].cv_x = 3;l[21].cv_y = -5;
    l[22].cv_x = 4;l[22].cv_y = -6;
    l[23].cv_x = 2;l[23].cv_y = -5;
    l[24].cv_x = 3;l[24].cv_y = -6;
    l[25].cv_x = 4;l[25].cv_y = -7;
    l[26].cv_x = 1;l[26].cv_y = -5;
    l[27].cv_x = 2;l[27].cv_y = -6;
    l[28].cv_x = 3;l[28].cv_y = -7;
    l[29].cv_x = 4;l[29].cv_y = -8;

    l[30].cv_x = -5;l[30].cv_y = 4;
    l[31].cv_x = -6;l[31].cv_y = 4;
    l[32].cv_x = -5;l[32].cv_y = 3;
    l[33].cv_x = -7;l[33].cv_y = 4;
    l[34].cv_x = -6;l[34].cv_y = 3;
    l[35].cv_x = -5;l[35].cv_y = 2;
    l[36].cv_x = -8;l[36].cv_y = 4;
    l[37].cv_x = -7;l[37].cv_y = 3;
    l[38].cv_x = -6;l[38].cv_y = 2;
    l[39].cv_x = -5;l[39].cv_y = 1;

    l[40].cv_x = 5;l[40].cv_y = -1;
    l[41].cv_x = 6;l[41].cv_y = -2;
    l[42].cv_x = 7;l[42].cv_y = -3;
    l[43].cv_x = 8;l[43].cv_y = -4;
    l[44].cv_x = 5;l[44].cv_y = -2;
    l[45].cv_x = 6;l[45].cv_y = -3;
    l[46].cv_x = 7;l[46].cv_y = -4;
    l[47].cv_x = 5;l[47].cv_y = -3;
    l[48].cv_x = 6;l[48].cv_y = -4;
    l[49].cv_x = 5;l[49].cv_y = -4;

    l[50].cv_x = -4;l[50].cv_y = -1;
    l[51].cv_x = -3;l[51].cv_y = -2;
    l[52].cv_x = -2;l[52].cv_y = -3;
    l[53].cv_x = -1;l[53].cv_y = -4;
    l[54].cv_x = -4;l[54].cv_y = -2;
    l[55].cv_x = -3;l[55].cv_y = -3;
    l[56].cv_x = -2;l[56].cv_y = -4;
    l[57].cv_x = -4;l[57].cv_y = -3;
    l[58].cv_x = -3;l[58].cv_y = -4;
    l[59].cv_x = -4;l[59].cv_y = -4;


    //以下为六边形，按行分开
    //上半部分
    l[60].cv_x=0;l[60].cv_y=4;
    l[61].cv_x=1;l[61].cv_y=3;
    l[62].cv_x=2;l[62].cv_y=2;
    l[63].cv_x=3;l[63].cv_y=1;
    l[64].cv_x=4;l[64].cv_y=0;

    l[65].cv_x=-1;l[65].cv_y=4;
    l[66].cv_x=0;l[66].cv_y=3;
    l[67].cv_x=1;l[67].cv_y=2;
    l[68].cv_x=2;l[68].cv_y=1;
    l[69].cv_x=3;l[69].cv_y=0;
    l[70].cv_x=4;l[70].cv_y=-1;

    l[71].cv_x=-2;l[71].cv_y=4;
    l[72].cv_x=-1;l[72].cv_y=3;
    l[73].cv_x=0;l[73].cv_y=2;
    l[74].cv_x=1;l[74].cv_y=1;
    l[75].cv_x=2;l[75].cv_y=0;
    l[76].cv_x=3;l[76].cv_y=-1;
    l[77].cv_x=4;l[77].cv_y=-2;

    l[78].cv_x=-3;l[78].cv_y=4;
    l[79].cv_x=-2;l[79].cv_y=3;
    l[80].cv_x=-1;l[80].cv_y=2;
    l[81].cv_x=0;l[81].cv_y=1;
    l[82].cv_x=1;l[82].cv_y=0;
    l[83].cv_x=2;l[83].cv_y=-1;
    l[84].cv_x=3;l[84].cv_y=-2;
    l[85].cv_x=4;l[85].cv_y=-3;

    l[86].cv_x=-4;l[86].cv_y=4;
    l[87].cv_x=-3;l[87].cv_y=3;
    l[88].cv_x=-2;l[88].cv_y=2;
    l[89].cv_x=-1;l[89].cv_y=1;
    l[90].cv_x=0;l[90].cv_y=0;
    l[91].cv_x=1;l[91].cv_y=-1;
    l[92].cv_x=2;l[92].cv_y=-2;
    l[93].cv_x=3;l[93].cv_y=-3;
    l[94].cv_x=4;l[94].cv_y=-4;

    //下半部分

    l[95].cv_x=-4;l[95].cv_y=0;
    l[96].cv_x=-3;l[96].cv_y=-1;
    l[97].cv_x=-2;l[97].cv_y=-2;
    l[98].cv_x=-1;l[98].cv_y=-3;
    l[99].cv_x=0;l[99].cv_y=-4;

    l[100].cv_x=-4;l[100].cv_y=1;
    l[101].cv_x=-3;l[101].cv_y=0;
    l[102].cv_x=-2;l[102].cv_y=-1;
    l[103].cv_x=-1;l[103].cv_y=-2;
    l[104].cv_x=0;l[104].cv_y=-3;
    l[105].cv_x=1;l[105].cv_y=-4;

    l[106].cv_x=-4;l[106].cv_y=2;
    l[107].cv_x=-3;l[107].cv_y=1;
    l[108].cv_x=-2;l[108].cv_y=0;
    l[109].cv_x=-1;l[109].cv_y=-1;
    l[110].cv_x=0;l[110].cv_y=-2;
    l[111].cv_x=1;l[111].cv_y=-3;
    l[112].cv_x=2;l[112].cv_y=-4;

    l[113].cv_x=-4;l[113].cv_y=3;
    l[114].cv_x=-3;l[114].cv_y=2;
    l[115].cv_x=-2;l[115].cv_y=1;
    l[116].cv_x=-1;l[116].cv_y=0;
    l[117].cv_x=0;l[117].cv_y=-1;
    l[118].cv_x=1;l[118].cv_y=-2;
    l[119].cv_x=2;l[119].cv_y=-3;
    l[120].cv_x=3;l[120].cv_y=-4;

}

void MainWindow::red_init_cvrt_Loc()
{
    l[0].cv_x = -8;l[0].cv_y = 4;
    l[1].cv_x = -7;l[1].cv_y = 3;
    l[2].cv_x = -7;l[2].cv_y = 4;
    l[3].cv_x = -6;l[3].cv_y = 2;
    l[4].cv_x = -6;l[4].cv_y = 3;
    l[5].cv_x = -6;l[5].cv_y = 4;
    l[6].cv_x = -5;l[6].cv_y = 1;
    l[7].cv_x = -5;l[7].cv_y = 2;
    l[8].cv_x = -5;l[8].cv_y = 3;
    l[9].cv_x = -5;l[9].cv_y = 4;

    l[10].cv_x = -4;l[10].cv_y = -4;
    l[11].cv_x = -4;l[11].cv_y = -3;
    l[12].cv_x = -4;l[12].cv_y = -2;
    l[13].cv_x = -4;l[13].cv_y = -1;
    l[14].cv_x = -3;l[14].cv_y = -4;
    l[15].cv_x = -3;l[15].cv_y = -3;
    l[16].cv_x = -3;l[16].cv_y = -2;
    l[17].cv_x = -2;l[17].cv_y = -4;
    l[18].cv_x = -2;l[18].cv_y = -3;
    l[19].cv_x = -1;l[19].cv_y = -4;

    l[20].cv_x = 1;l[20].cv_y = 4;
    l[21].cv_x = 2;l[21].cv_y = 3;
    l[22].cv_x = 2;l[22].cv_y = 4;
    l[23].cv_x = 3;l[23].cv_y = 2;
    l[24].cv_x = 3;l[24].cv_y = 3;
    l[25].cv_x = 3;l[25].cv_y = 4;
    l[26].cv_x = 4;l[26].cv_y = 1;
    l[27].cv_x = 4;l[27].cv_y = 2;
    l[28].cv_x = 4;l[28].cv_y = 3;
    l[29].cv_x = 4;l[29].cv_y = 4;

    l[30].cv_x = 1;l[30].cv_y = -5;
    l[31].cv_x = 2;l[31].cv_y = -6;
    l[32].cv_x = 2;l[32].cv_y = -5;
    l[33].cv_x = 3;l[33].cv_y = -7;
    l[34].cv_x = 3;l[34].cv_y = -6;
    l[35].cv_x = 3;l[35].cv_y = -5;
    l[36].cv_x = 4;l[36].cv_y = -8;
    l[37].cv_x = 4;l[37].cv_y = -7;
    l[38].cv_x = 4;l[38].cv_y = -6;
    l[39].cv_x = 4;l[39].cv_y = -5;

    l[40].cv_x = -4;l[40].cv_y = 5;
    l[41].cv_x = -4;l[41].cv_y = 6;
    l[42].cv_x = -4;l[42].cv_y = 7;
    l[43].cv_x = -4;l[43].cv_y = 8;
    l[44].cv_x = -3;l[44].cv_y = 5;
    l[45].cv_x = -3;l[45].cv_y = 6;
    l[46].cv_x = -3;l[46].cv_y = 7;
    l[47].cv_x = -2;l[47].cv_y = 5;
    l[48].cv_x = -2;l[48].cv_y = 6;
    l[49].cv_x = -1;l[49].cv_y = 5;

    l[50].cv_x = 5;l[50].cv_y = -4;
    l[51].cv_x = 5;l[51].cv_y = -3;
    l[52].cv_x = 5;l[52].cv_y = -2;
    l[53].cv_x = 5;l[53].cv_y = -1;
    l[54].cv_x = 6;l[54].cv_y = -4;
    l[55].cv_x = 6;l[55].cv_y = -3;
    l[56].cv_x = 6;l[56].cv_y = -2;
    l[57].cv_x = 7;l[57].cv_y = -4;
    l[58].cv_x = 7;l[58].cv_y = -3;
    l[59].cv_x = 8;l[59].cv_y = -4;


    //以下为六边形，按行分开
    //上半部分
    l[60].cv_x=-4;l[60].cv_y=0;
    l[61].cv_x=-4;l[61].cv_y=1;
    l[62].cv_x=-4;l[62].cv_y=2;
    l[63].cv_x=-4;l[63].cv_y=3;
    l[64].cv_x=-4;l[64].cv_y=4;

    l[65].cv_x=-3;l[65].cv_y=-1;
    l[66].cv_x=-3;l[66].cv_y=0;
    l[67].cv_x=-3;l[67].cv_y=1;
    l[68].cv_x=-3;l[68].cv_y=2;
    l[69].cv_x=-3;l[69].cv_y=3;
    l[70].cv_x=-3;l[70].cv_y=4;

    l[71].cv_x=-2;l[71].cv_y=-2;
    l[72].cv_x=-2;l[72].cv_y=-1;
    l[73].cv_x=-2;l[73].cv_y=0;
    l[74].cv_x=-2;l[74].cv_y=1;
    l[75].cv_x=-2;l[75].cv_y=2;
    l[76].cv_x=-2;l[76].cv_y=3;
    l[77].cv_x=-2;l[77].cv_y=4;

    l[78].cv_x=-1;l[78].cv_y=-3;
    l[79].cv_x=-1;l[79].cv_y=-2;
    l[80].cv_x=-1;l[80].cv_y=-1;
    l[81].cv_x=-1;l[81].cv_y=0;
    l[82].cv_x=-1;l[82].cv_y=1;
    l[83].cv_x=-1;l[83].cv_y=2;
    l[84].cv_x=-1;l[84].cv_y=3;
    l[85].cv_x=-1;l[85].cv_y=4;

    l[86].cv_x=0;l[86].cv_y=-4;
    l[87].cv_x=0;l[87].cv_y=-3;
    l[88].cv_x=0;l[88].cv_y=-2;
    l[89].cv_x=0;l[89].cv_y=-1;
    l[90].cv_x=0;l[90].cv_y=0;
    l[91].cv_x=0;l[91].cv_y=1;
    l[92].cv_x=0;l[92].cv_y=2;
    l[93].cv_x=0;l[93].cv_y=3;
    l[94].cv_x=0;l[94].cv_y=4;

    //下半部分

    l[95].cv_x=4;l[95].cv_y=-4;
    l[96].cv_x=4;l[96].cv_y=-3;
    l[97].cv_x=4;l[97].cv_y=-2;
    l[98].cv_x=4;l[98].cv_y=-1;
    l[99].cv_x=4;l[99].cv_y=0;

    l[100].cv_x=3;l[100].cv_y=-4;
    l[101].cv_x=3;l[101].cv_y=-3;
    l[102].cv_x=3;l[102].cv_y=-2;
    l[103].cv_x=3;l[103].cv_y=-1;
    l[104].cv_x=3;l[104].cv_y=0;
    l[105].cv_x=3;l[105].cv_y=1;

    l[106].cv_x=2;l[106].cv_y=-4;
    l[107].cv_x=2;l[107].cv_y=-3;
    l[108].cv_x=2;l[108].cv_y=-2;
    l[109].cv_x=2;l[109].cv_y=-1;
    l[110].cv_x=2;l[110].cv_y=0;
    l[111].cv_x=2;l[111].cv_y=1;
    l[112].cv_x=2;l[112].cv_y=2;

    l[113].cv_x=1;l[113].cv_y=-4;
    l[114].cv_x=1;l[114].cv_y=-3;
    l[115].cv_x=1;l[115].cv_y=-2;
    l[116].cv_x=1;l[116].cv_y=-1;
    l[117].cv_x=1;l[117].cv_y=0;
    l[118].cv_x=1;l[118].cv_y=1;
    l[119].cv_x=1;l[119].cv_y=2;
    l[120].cv_x=1;l[120].cv_y=3;
}

void MainWindow::green_init_cvrt_st()
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
    s[41].cv_xs=2;s[41].cv_ys=-5;
    s[42].cv_xs=3;s[42].cv_ys=-5;
    s[43].cv_xs=4;s[43].cv_ys=-5;
    s[44].cv_xs=2;s[44].cv_ys=-6;
    s[45].cv_xs=3;s[45].cv_ys=-6;
    s[46].cv_xs=4;s[46].cv_ys=-6;
    s[47].cv_xs=3;s[47].cv_ys=-7;
    s[48].cv_xs=4;s[48].cv_ys=-7;
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

void MainWindow::m2_purple_init_cvrt_st()
{
    s[10].cv_xs = -1;s[10].cv_ys = 5;
    s[11].cv_xs = -2;s[11].cv_ys = 5;
    s[12].cv_xs = -3;s[12].cv_ys = 5;
    s[13].cv_xs = -4;s[13].cv_ys = 5;
    s[14].cv_xs = -2;s[14].cv_ys = 6;
    s[15].cv_xs = -3;s[15].cv_ys = 6;
    s[16].cv_xs = -4;s[16].cv_ys = 6;
    s[17].cv_xs = -3;s[17].cv_ys = 7;
    s[18].cv_xs = -4;s[18].cv_ys = 7;
    s[19].cv_xs = -4;s[19].cv_ys = 8;

    s[40].cv_xs = 4;s[40].cv_ys = -8;
    s[41].cv_xs = 4;s[41].cv_ys = -7;
    s[42].cv_xs = 3;s[42].cv_ys = -7;
    s[43].cv_xs = 4;s[43].cv_ys = -6;
    s[44].cv_xs = 3;s[44].cv_ys = -6;
    s[45].cv_xs = 2;s[45].cv_ys = -6;
    s[46].cv_xs = 4;s[46].cv_ys = -5;
    s[47].cv_xs = 3;s[47].cv_ys = -5;
    s[48].cv_xs = 2;s[48].cv_ys = -5;
    s[49].cv_xs = 1;s[49].cv_ys = -5;
}

void MainWindow::m3_purple_init_cvrt_st()
{


}

void MainWindow::m3_blue_init_cvrt_st()
{

}

void MainWindow::m3_red_init_cvrt_st()
{

}

bool MainWindow::getCvt(int x,int y,int &t)
{
    for(t=0;t<121;t++)
    {
        if(l[t].cv_x==x&&l[t].cv_y==y)
            return true;
    }
    return false;
}

void MainWindow::move_Rt(int x, int y)
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
                }
             }
                _selected_id=-1;

                update(); //注意这两行代码，后期可能会有错误出现

    //        qDebug()<<s[_selected_id].x0<<s[_selected_id].y0;
    //        _selected_id=-1;

    //        update();
        }

}

int MainWindow::change_mark(char mark)
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

void MainWindow::change_char_to_int(char *p)
{
//    qDebug()<<"change_char_to_int";
    memset(route,0,sizeof(route));
//    next=true;
    routeLength=0;
    cnt=0;
    //此变量设置在change_char_to_int函数中，防止在一开始就实行轮转
//    print1();
//    if(next)
//    {
//            state++;
//            state%=pattern;
//            for(int i=0;i<failed_num;i++)
//            {
//                if(state == fall_players[i])
//                {
//                    state++;
//                    state%=pattern;
//                }
//            }

//            qDebug()<<state;
//            next=false;
//    }
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

void MainWindow::print()
{
    for(int i=0;i<routeLength;i++)
    {
        qDebug()<<route[i];
    }
    return ;
}

void MainWindow::init_psr()
{
    for(int i=0;i<121;i++)
    {
        l[i].psr.clear();
    }
}

void MainWindow::clear_psr()
{
    for(int i=0;i<121;i++)
    {
            l[i].psr.clear();
    }
}

void MainWindow::init_psr_set()
{
    for(int i=0;i<121;i++)
    {
        l[i].psr_set=false;
    }
}

void MainWindow::change_state()
{
    if(next)
    {
        qDebug()<<state<<"@__2";
        state++;
        state%=pattern;
        next=false;
    }
}
