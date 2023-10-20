#include "monixitong.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    monixitong w;
    w.setWindowTitle("进程调度模拟系统");
    w.show();
    return a.exec();
}
