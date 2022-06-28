#include "connectwindow.h"
#include "ui_connectwindow.h"
#include "clientwindow.h"
#include "QTcpServer"
#include<QPainter>
extern QString C_host;
extern quint16 C_port;
extern ClientWindow *Board;
ConnectWindow::ConnectWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);
}

ConnectWindow::~ConnectWindow()
{
    delete ui;
}

void ConnectWindow::on_pushButton_clicked()
{
//    Board = new ClientWindow;
    C_host = ui->lineEdit_2->text();
    QString pt = ui->lineEdit->text();
    C_port = pt.toUInt();
//    Board->port = p;
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();

//    qDebug()<<C_host<<C_port;

    Board = new ClientWindow;
    this->hide();
}

void ConnectWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap(":/pic/picture/connectwindow.jpg"),QRect());
}
