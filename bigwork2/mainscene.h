#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include"mainwindow.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainScene; }
QT_END_NAMESPACE

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();

    MainWindow *choose=NULL;
    MainWindow *choose1=NULL;
    MainWindow *choose2=NULL;

private:
    Ui::MainScene *ui;
};
#endif // MAINSCENE_H
