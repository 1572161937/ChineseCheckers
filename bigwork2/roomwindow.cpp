#include "roomwindow.h"
#include "ui_roomwindow.h"
#include<QString>
#include<networksocket.h>
#include<networkdata.h>
#include <QtNetwork/QTcpSocket>
#include<clientwindow.h>
#include<QLabel>
#include<QPixmap>
#include<QPainter>
//定义在main.cpp中
extern ClientWindow *Board;

extern QString roomName;
extern QString PlayerName;
extern int me;
RoomWindow::RoomWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RoomWindow)
{
    ui->setupUi(this);


    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::white);
//    ui->lineEdit->move(100,500);
//    ui->lineEdit_2->move(100,550);
    ui->label_13->setPalette(pe);
    ui->label_14->setPalette(pe);

    for(int i=0;i<6;i++)
    {
        player_name[i] = new QLabel();
        player_name[i]->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        player_name[i]->setPalette(pe);
        player_name[i]->setParent(this);
        player_name[i]->resize(100,30);
        player_name[i]->move(200,50+i*65);
        player_name[i]->setText("召唤师");
        player_name[i]->setFrameShape(QFrame::Box);
        player_name[i]->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);");
        player_name[i]->show();
    }

    for(int i=0;i<6;i++)
    {
        ListState[i] = new QLabel(this);
        ListState[i]->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ListState[i]->setPalette(pe);
        ListState[i]->resize(100,30);
        ListState[i]->move(350,50+i*65);
        ListState[i]->setText("碾碎它！");
        ListState[i]->setFrameShape(QFrame::Box);
        ListState[i]->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);");
        ListState[i]->show();
    }
    Player_num = 0;
}

RoomWindow::~RoomWindow()
{
    delete ui;
}

void RoomWindow::on_pushButton_clicked()
{
    roomName=ui->lineEdit->text();
    PlayerName=ui->lineEdit_2->text();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    NetworkData data(OPCODE::JOIN_ROOM_OP,roomName,PlayerName);
    Board->socket->send(data);
}

void RoomWindow::updateInform(NetworkData data)
{
//    qDebug()<<PlayerName<<"Debug";
    int Length = data.data2.length();
    qDebug()<<data.data2;
    qDebug()<<Length;
    int i=0;
    for(i=0;i<Length;i++)
    {
        player_name[i]->setText(data.data1.section(' ',i,i));
        playerstate[i]=data.data2[i];
        playerList[i]=player_name[i]->text();
        if(playerstate[i]=='0')
           ListState[i]->setText("未到达战场");
        else
           ListState[i]->setText("全军出击！");
    }
    player_name[Length]->setText(PlayerName);
    ListState[Length]->setText("未到达战场");
    playerList[Length] = PlayerName;
    Player_num +=Length + 1;

}

void RoomWindow::single_update(NetworkData data)
{
    player_name[Player_num]->setText(data.data1);
    ListState[Player_num]->setText("未到达战场");
    playerList[Player_num] = data.data1;
    playerstate[Player_num] = '0';
    Player_num++;
}

void RoomWindow::Dele(NetworkData data)
{
    int Length = Player_num;
    int i;
    for(i=0;i<Length;i++)
    {
        if(player_name[i]->text()==data.data1)

        {
            player_name[i]->clear();
            ListState[i]->clear();
        }
    }
    for(int j=i;j<Length-1;j++)
    {
        player_name[j]->setText(player_name[j+1]->text());
        ListState[j]->setText(ListState[j+1]->text());
    }
    player_name[Length-1]->clear();
    ListState[Length-1]->clear();
}

void RoomWindow::Ready_change(NetworkData data)
{
    int len=Player_num;
    for(int i=0;i<len;i++)
    {
        if(player_name[i]->text()==data.data1)
        {
            playerstate[i]='1';
            ListState[i]->setText("全军出击!");
            break;
        }
    }
}

void RoomWindow::on_pushButton_3_clicked()
{
    for(int i=0;i<Player_num;i++)
    {
        if(PlayerName == player_name[i]->text())
           {
            ListState[i]->setText("全军出击!");
            NetworkData data(OPCODE::PLAYER_READY_OP,PlayerName,"");
            Board->socket->send(data);
        }
    }
}

void RoomWindow::Loc_me()
{
    for(int i=0;i<Player_num;i++)
    {
        if(player_name[i]->text() == PlayerName)
        {
            me = i;
            return ;
        }
    }
}

void RoomWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap(":/pic/picture/xiagu.jpg"),QRect());
}
