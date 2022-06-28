#include "ErrorWindow.h"
#include "ui_ErrorWindow.h"
#include<QLabel>
#include<QPushButton>
#include<roomwindow.h>
#include<QPainter>

extern RoomWindow *room;
ErrorWindow::ErrorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ErrorWindow)
{
    ui->setupUi(this);
    QFont font;
    font.setFamily("楷体 ");
    font.setPointSize(25);
    ui->label->resize(200,60);
    ui->label->setText("连接成功");
    ui->label->move(80,300);
    ui->label->setFont(font);
    ui->btn->setText("Ok");
    ui->btn->resize(150,60);
    ui->btn->move(80,400);
    connect(ui->btn,&QPushButton::clicked,[=](){
        this->hide();
        room->show();
    });
}

ErrorWindow::~ErrorWindow()
{
    delete ui;
}

void ErrorWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap(":/pic/picture/jg.jpg"),QRect());
}
