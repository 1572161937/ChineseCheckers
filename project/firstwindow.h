#ifndef FIRSTWINDOW_H
#define FIRSTWINDOW_H

#include <QMainWindow>

namespace Ui {
class firstwindow;
}

class firstwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit firstwindow(QWidget *parent = nullptr);
    ~firstwindow();
    void paintEvent(QPaintEvent*);

private:
    Ui::firstwindow *ui;
};

#endif // FIRSTWINDOW_H
