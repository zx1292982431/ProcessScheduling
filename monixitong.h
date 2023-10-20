#ifndef MONIXITONG_H
#define MONIXITONG_H

#include <QWidget>
#include<QDialog>
#include<QLabel>
#include<QPushButton>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include<QLineEdit>
#include<vector>
#include<string>
#include<QRadioButton>

struct PCB
{
        QString name;
        int ID;
        int arrive_time;    //到达时间
        int need_time;     //估计服务时间
        int prio;          //进程优先数，数字越大优先级越高
        int wait_time;     //等待时间
        int more_time;    //剩余执行的时间
        int state;         //进程状态 0-执行 1-就绪 2-阻塞 3-完成 4-等待
        int finishi_time; //完成服务时间
        int need_mem;       //需要内存数量
        int need_printer;   //需要打印机数量
};

class monixitong : public QDialog
{
    Q_OBJECT

public:
    monixitong(QDialog *parent = 0);
private slots:
    void init();
    void create_process();
    void refresh_wait();
    void refresh_run();
    void start();
    void refresh_ready();
    void refresh_finish();
    void alogorithm1();
    void alogorithm2();
    void refresh_begin();
    void refresh_block();
    void process_block();
    void process_active();
    void show_result();
private:
    int mem=0;
    int printer=0;
    int counter=0;
    int alogorithm=0; //1代表非抢占式，2代表抢占式。
    int time_counter=0;
    std::vector<PCB> wait;
    std::vector<PCB> ready;
    std::vector<PCB> finish;
    std::vector<PCB> begin;
    std::vector<PCB> block;
    PCB run;
    //初始化模块
    QLabel *label_time;
    QLabel *label_mem;
    QLabel *label_printer;
    QLabel *label_neicun;
    QLabel *label_dayinji;
    QPushButton *pushbutton_chushihua;
    QLineEdit *lineedit_neicun;
    QLineEdit *lineedit_dayinji;
    //创建进程模块
    QLineEdit *lineedit_process_name;
    QLineEdit *lineedit_arrive_time;
    QLineEdit *lineedit_need_time;
    QLineEdit *lineedit_prio;
    QLineEdit *lineedit_need_mem;
    QLineEdit *lineedit_need_printer;
    QPushButton *pushbutton_create_process;
    //等待队列显示模块
    QTableView *tableview_wait;
    QStandardItemModel*model_wait;
    //运行进程显示
    QTableView *tableview_run;
    QStandardItemModel *model_run;
    QPushButton *pushbutton_block;
    //算法选择模块
    QRadioButton *radiobutton_alogorithm1;
    QRadioButton *radiobutton_alogorithm2;
    QPushButton *pushbutton_start;
    //就绪队列显示模块
    QTableView *tableview_ready;
    QStandardItemModel *model_ready;
    //完成队列显示模块
    QTableView *tableview_finish;
    QStandardItemModel *model_finish;
    QPushButton *pushbutton_result;
    //进程初始情况显示模块
    QTableView *tableview_begin;
    QStandardItemModel *model_begin;
    //阻塞队列显示模块
    QTableView *tableview_block;
    QStandardItemModel *model_block;
    QPushButton *pushbutton_active;
    QLineEdit *lineedit_pid;

};
#endif // MONIXITONG_H
