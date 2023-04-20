#include "firstwindow.h"
#include "ui_firstwindow.h"
#include"mainscene.h"
#include<QPushButton>
#include<QDebug>
#include"secondwindow.h"
#include<QPainter>
#include<QPaintEvent>
#include<QPixmap>
#include<QFont>
firstwindow::firstwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::firstwindow)
{
    ui->setupUi(this);
    QFont font;
    font.setFamily("楷体 ");
    font.setPointSize(25);
    QPushButton *btn1 = new QPushButton;
    btn1->setParent(this);
    btn1->setText("单机模式");
//    btn1->setStyleSheet("QPushButton{border-radius:2px;background:rgb(170, 170, 255);color:red;font-size:15px;}");
    QPushButton *btn2 = new QPushButton;
    btn2->setParent(this);
    btn2->setText("联机模式");
//    btn2->setStyleSheet("QPushButton{border-radius:2px;background:rgb(170, 170, 255);color:red;font-size:15px;}");
    btn1->resize(200,80);
    btn1->move(this->width()/3-100,this->height()/3+200);
    btn2->resize(200,80);
    btn2->move(this->width()/3-100,this->height()/4+100);
    btn1->setFont(font);
    btn2->setFont(font);
    btn1->show();
    btn2->show();
    MainScene *w = new MainScene;
    secondwindow *v = new secondwindow;
    connect(btn1,&QPushButton::clicked,[=](){
        this->hide();
       w->show();
    });
    connect(btn2,&QPushButton::clicked,[=](){
        this->hide();
       v->show();
    });
}

firstwindow::~firstwindow()
{
    delete ui;
}

void firstwindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap(":/pic/picture/timo.jpg"),QRect());
}
