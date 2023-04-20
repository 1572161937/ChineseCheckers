#include "mainscene.h"
#include "ui_mainscene.h"
#include<QPushButton>
#include"mainwindow.h"
#include<QDebug>
MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);
    this->resize(1300,1200);
    QPushButton *btn=new QPushButton;
    btn->setParent(this);
    btn->move(this->width()/2,this->height()/6);
    btn->resize(200,80);
    btn->setText("双人");
    btn->show();
    QPushButton *btn2=new QPushButton;
    btn2->setParent(this);
    btn2->move(this->width()/2,this->height()/3);
    btn2->resize(200,80);
    btn2->setText("三人");
    btn2->show();
    QPushButton *btn3=new QPushButton;
    btn3->setParent(this);
    btn3->move(this->width()/2,this->height()/2);
    btn3->resize(200,80);
    btn3->setText("六人");
    btn3->show();
    choose=new MainWindow();
    choose1=new MainWindow();
    choose2=new MainWindow();
    connect(btn,&QPushButton::clicked,[=](){
        this->hide();
        choose->pattern = 2;
        choose->single_mode = true;
        choose->markstone();
        choose->init_occupied();
        choose->update();
//        qDebug()<<choose->pattern;
        choose->show();

    });
    connect(btn2,&QPushButton::clicked,[=](){
        this->hide();
        choose1->pattern = 3;
        choose->single_mode = true;
        choose->markstone();
        choose1->init_occupied();
        choose1->update();
        choose1->show();
    });
    connect(btn3,&QPushButton::clicked,[=](){
        this->hide();
        choose2->pattern = 6;
        choose->single_mode = true;
        choose->markstone();
        choose2->init_occupied();
        choose2->update();
        choose2->show();
    });
    QPushButton *btnclose=new QPushButton;
    btnclose->setParent(this);
    btnclose->move(this->width(),this->height()/5*3);
    btnclose->resize(200,80);
    btnclose->setText("关闭");
    btnclose->show();
    connect(btnclose,&QPushButton::clicked,this,&QPushButton::close);
}

MainScene::~MainScene()
{
    delete ui;
}

