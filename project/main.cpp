#include "mainscene.h"
#include"firstwindow.h"
#include <QApplication>
#include<QMessageBox>
#include<roomwindow.h>
#include <clientwindow.h>
#include<networksocket.h>
RoomWindow * room;
QString roomName;
QString PlayerName;
ClientWindow *Board;
NetworkSocket *socket;
quint16 C_port;
QString C_host;
int me;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    firstwindow w;
    room = new RoomWindow();

    w.show();
    return a.exec();
}
