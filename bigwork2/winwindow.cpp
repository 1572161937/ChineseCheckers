#include "winwindow.h"
#include "ui_winwindow.h"
#include<QLabel>
WinWindow::WinWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WinWindow)
{
    ui->setupUi(this);
    ui->label->setText("游戏胜利！");
}

WinWindow::~WinWindow()
{
    delete ui;
}

void WinWindow::on_pushButton_clicked()
{
    this->close();
}

