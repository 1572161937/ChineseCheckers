#include "secondwindow.h"
#include "ui_secondwindow.h"
#include<QPushButton>
#include"firstwindow.h"
#include"mainscene.h"
#include"clientwindow.h"
#include"serverwindow.h"
#include<QPaintEvent>
#include<QPainter>
#include<QPixmap>
#include<connectwindow.h>
extern ClientWindow *Board;
secondwindow::secondwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::secondwindow)
{
    ui->setupUi(this);
    QPushButton *btn1 = new QPushButton;
    btn1->setParent(this);
    btn1->setText("Client");
    QPushButton *btn2 = new QPushButton;
    btn2->setParent(this);
    btn2->setText("Server");
    btn1->resize(200,80);
    btn1->move(this->width()/3-100,this->height()/3+200);
    btn2->resize(200,80);
    btn2->move(this->width()/3-100,this->height()/4+100);
    btn1->show();
    btn2->show();
    ConnectWindow *c = new ConnectWindow;
    connect(btn1,&QPushButton::clicked,[=](){
        this->hide();
//        Board = new ClientWindow;
        c->show();
    });
    connect(btn2,&QPushButton::clicked,[=](){
        this->hide();
        ServerWindow *v = new ServerWindow;
    });
}

secondwindow::~secondwindow()
{
    delete ui;
}

void secondwindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap(":/pic/picture/secondwindow.jpg"),QRect());
}
