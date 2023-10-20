#include "monixitong.h"
#include <QtGui>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<iostream>
#include<algorithm>
#include<QThread>
#include<QTimer>

int zero=0;
extern QTimer timer;
extern double timeRate;
QTimer timer;
double timeRate = 1;

monixitong::monixitong(QDialog *parent)
    : QDialog(parent)
{
    //初始化模块
    label_neicun=new QLabel("内存");
    label_dayinji=new QLabel("打印机");
    lineedit_neicun=new QLineEdit;
    lineedit_dayinji=new QLineEdit;
    pushbutton_chushihua=new QPushButton("初始化内存和打印机");
    label_neicun->setBuddy(lineedit_neicun);
    label_dayinji->setBuddy(lineedit_dayinji);
    QVBoxLayout *mainwindow_layout_left=new QVBoxLayout;
    QHBoxLayout *init_layout=new QHBoxLayout;
    connect(pushbutton_chushihua,SIGNAL(clicked()),this,SLOT(init()));
    init_layout->addWidget(label_neicun);
    init_layout->addWidget(lineedit_neicun);
    init_layout->addWidget(new QLabel("M"));
    init_layout->addWidget(label_dayinji);
    init_layout->addWidget(lineedit_dayinji);
    init_layout->addWidget(new QLabel("台"));
    init_layout->addWidget(pushbutton_chushihua);
    init_layout->setContentsMargins(1,1,1,1);
    QHBoxLayout *display_mem_printer_layout=new QHBoxLayout;
    label_time=new QLabel;
    label_time->setText("当前时间："+QString::number(time_counter));
    label_mem=new QLabel;
    label_mem->setText("内存剩余:"+QString::number(mem)+"M");
    label_printer=new QLabel;
    label_printer->setText("打印机剩余"+QString::number(printer)+"台");
    display_mem_printer_layout->addWidget(label_mem);
    display_mem_printer_layout->addWidget(label_printer);
    display_mem_printer_layout->addWidget(label_time);
    //新建进程模块
    lineedit_process_name=new QLineEdit;
    lineedit_arrive_time=new QLineEdit;
    lineedit_need_time=new QLineEdit;
    lineedit_prio=new QLineEdit;
    lineedit_need_mem=new QLineEdit;
    lineedit_need_printer=new QLineEdit;
    pushbutton_create_process=new QPushButton("创建进程");
    QHBoxLayout *create_process_layout=new QHBoxLayout;
    create_process_layout->addWidget(new QLabel("进程名"));
    create_process_layout->addWidget(lineedit_process_name);
    create_process_layout->addWidget(new QLabel("到达时间"));
    create_process_layout->addWidget(lineedit_arrive_time);
    create_process_layout->addWidget(new QLabel("预计执行时间"));
    create_process_layout->addWidget(lineedit_need_time);
    create_process_layout->addWidget(new QLabel("优先级"));
    create_process_layout->addWidget(lineedit_prio);
    create_process_layout->addWidget(new QLabel("申请内存大小"));
    create_process_layout->addWidget(lineedit_need_mem);
    create_process_layout->addWidget(new QLabel("申请打印机台数"));
    create_process_layout->addWidget(lineedit_need_printer);
    create_process_layout->addWidget(pushbutton_create_process);
    connect(pushbutton_create_process,SIGNAL(clicked()),this,SLOT(create_process()));
    //显示等待队列
    tableview_wait=new QTableView;
    model_wait=new QStandardItemModel();
    refresh_wait();
    QVBoxLayout *wait_layout=new QVBoxLayout;
    wait_layout->addWidget(new QLabel("New队列"));
    wait_layout->addWidget(tableview_wait);
    //正在运行的进程
    tableview_run=new QTableView;
    model_run=new QStandardItemModel();
    model_run->setHorizontalHeaderLabels({"进程名","PID","优先级","剩余运行时间","占用内存大小","占用打印机数量"});
    tableview_run->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    refresh_run();
    pushbutton_block=new QPushButton("阻塞");
    connect(pushbutton_block,SIGNAL(clicked()),this,SLOT(process_block()));
    QVBoxLayout *run_layout=new QVBoxLayout;
    QHBoxLayout *t1=new QHBoxLayout;
    t1->addWidget(new QLabel("正在运行的进程"));
    t1->addWidget(pushbutton_block);
    run_layout->addLayout(t1);
    run_layout->addWidget(tableview_run);
    //算法选择模块
    radiobutton_alogorithm1=new QRadioButton("非抢占式优先级算法");
    radiobutton_alogorithm2=new QRadioButton("抢占式优先级算法");
    pushbutton_start=new QPushButton("开始模拟");
    QHBoxLayout * alogorithm_select_layout=new QHBoxLayout;
    alogorithm_select_layout->addWidget(new QLabel("请选择模拟算法："));
    alogorithm_select_layout->addWidget(radiobutton_alogorithm1);
    alogorithm_select_layout->addWidget(radiobutton_alogorithm2);
    alogorithm_select_layout->addWidget(pushbutton_start);
    connect(radiobutton_alogorithm1,&QRadioButton::toggled,[=](bool ischecked){if(ischecked=true)alogorithm=1;std::cout<<"alogorithm:"<<alogorithm<<std::endl;});
    connect(radiobutton_alogorithm2,&QRadioButton::toggled,[=](bool ischecked){if(ischecked=true)alogorithm=2;std::cout<<"alogorithm:"<<alogorithm<<std::endl;});
    connect(pushbutton_start,SIGNAL(clicked()),this,SLOT(start()));
    //显示就绪队列
    tableview_ready=new QTableView;
    model_ready=new QStandardItemModel();
    refresh_ready();
    QVBoxLayout *ready_layout=new QVBoxLayout;
    ready_layout->addWidget(new QLabel("就绪队列"));
    ready_layout->addWidget(tableview_ready);
    //显示完成队列
    tableview_finish=new QTableView;
    model_finish=new QStandardItemModel();
    model_finish->setHorizontalHeaderLabels({"进程名","PID","到达时间","完成时间","周转时间","最终优先级","内存占用大小","打印机占用数量"});
    tableview_finish->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    refresh_finish();
    QVBoxLayout *finish_layout=new QVBoxLayout;
    QHBoxLayout *header_finish_layout= new QHBoxLayout;
    QLabel *l3=new QLabel("完成队列");
    header_finish_layout->addWidget(l3);
    pushbutton_result= new QPushButton("显示执行结果");
    connect(pushbutton_result,SIGNAL(clicked()),this,SLOT(show_result()));
    header_finish_layout->addWidget(pushbutton_result);
    header_finish_layout->setStretchFactor(l3,5);
    header_finish_layout->setStretchFactor(pushbutton_result,1);
    finish_layout->addWidget(tableview_finish);
    //显示初始情况（初始队列）
    tableview_begin= new QTableView;
    model_begin=new QStandardItemModel();
    refresh_begin();
    QVBoxLayout *begin_layout=new QVBoxLayout;
    begin_layout->addWidget(new QLabel("进程初始情况"));
    begin_layout->addWidget(tableview_begin);
    //显示阻塞队列
    lineedit_pid=new QLineEdit;
    pushbutton_active=new QPushButton("激活");
    connect(pushbutton_active,SIGNAL(clicked()),this,SLOT(process_active()));
    tableview_block=new QTableView;
    model_block=new QStandardItemModel();
    refresh_block();
    QHBoxLayout *header_block_layout=new QHBoxLayout;
    QLabel *l1=new QLabel("阻塞队列");
    QLabel *l2=new QLabel("输入要唤醒进程的PID");
    header_block_layout->addWidget(l1);
    header_block_layout->addWidget(l2);
    header_block_layout->addWidget(lineedit_pid);
    header_block_layout->addWidget(pushbutton_active);
    header_block_layout->setStretchFactor(l1,3);
    header_block_layout->setStretchFactor(l2,1);
    header_block_layout->setStretchFactor(lineedit_pid,1);
    header_block_layout->setStretchFactor(pushbutton_active,1);
    QVBoxLayout *block_layout=new QVBoxLayout;
    block_layout->addWidget(tableview_block);



    QHBoxLayout *mainwindow_layout=new QHBoxLayout;
    QVBoxLayout *mainwindow_layout_right =new QVBoxLayout;

    mainwindow_layout_left->addLayout(display_mem_printer_layout);
    mainwindow_layout_left->addLayout(init_layout);
    mainwindow_layout_left->addLayout(create_process_layout);
    mainwindow_layout_left->addLayout(alogorithm_select_layout);
    mainwindow_layout_left->addLayout(wait_layout);
    mainwindow_layout_left->addLayout(run_layout);
    mainwindow_layout_left->addLayout(ready_layout);

    mainwindow_layout_right->addLayout(header_block_layout);
    mainwindow_layout_right->addLayout(block_layout);
    mainwindow_layout_right->addLayout(begin_layout);
    mainwindow_layout_right->addLayout(header_finish_layout);
    mainwindow_layout_right->addLayout(finish_layout);


    mainwindow_layout->addLayout(mainwindow_layout_left);
    mainwindow_layout->addLayout(mainwindow_layout_right);
    mainwindow_layout->setStretchFactor(mainwindow_layout_left,1);
    mainwindow_layout->setStretchFactor(mainwindow_layout_right,1);
    setLayout(mainwindow_layout);

}

void monixitong::init()
{
    std::cout<<"正在初始化。。。"<<std::endl;
    int mem_=lineedit_neicun->text().toInt();
    mem=mem_;
    int printer_=lineedit_dayinji->text().toInt();
    printer=printer_;
    label_mem->setText("内存剩余:"+QString::number(mem)+"M");
    label_printer->setText("打印机剩余"+QString::number(printer)+"台");
    std::cout<<"初始化完成，内存容量："<<mem<<" 打印机数量："<<printer<<std::endl;

}

void monixitong::create_process()
{
    if(lineedit_need_mem->text().toInt()<=mem&&lineedit_need_printer->text().toInt()<=printer)
    {
        PCB n;
        n.ID=counter;
        counter++;
        n.name=lineedit_process_name->text();
        n.arrive_time=lineedit_arrive_time->text().toInt();
        n.need_time=lineedit_need_time->text().toInt();
        n.prio=lineedit_prio->text().toInt();
        n.need_mem=lineedit_need_mem->text().toInt();
        n.need_printer=lineedit_need_printer->text().toInt();
        n.state=4;
        n.wait_time=zero;
        n.more_time=n.need_time;
        wait.emplace_back(n);
        begin.emplace_back(n);
        //mem-=n.need_mem;
        //printer-=n.need_printer;
        label_mem->setText("内存剩余:"+QString::number(mem)+"M");
        label_printer->setText("打印机剩余"+QString::number(printer)+"台");
        std::cout<<"创建进程成功，PID："<<n.ID<<" 到达时间:"<<n.arrive_time<<" 预计服务时间："<<n.need_time<<" 所需内存："<<n.need_mem<<" 所需打印机："<<n.need_printer<<std::endl;
        lineedit_process_name->clear();
        lineedit_arrive_time->clear();
        lineedit_need_time->clear();
        lineedit_prio->clear();
        lineedit_need_mem->clear();
        lineedit_need_printer->clear();
        refresh_wait();
        refresh_begin();
    }
    else
    {
        std::cout<<"资源不足无法创建。。。"<<std::endl;
        QLabel *falier=new QLabel("<h2>"
                                  "<font color=red>资源不足，创建失败！</font></h2>");
        falier->show();
    }
}

void monixitong::refresh_wait()
{
    model_wait->clear();
    model_wait->setHorizontalHeaderLabels({"进程名","PID","到达时间","预计执行时间","优先级","申请内存容量","申请打印机数量"});
    //std::cout<<"刷新等待队列，size为："<<wait.size()<<std::endl;
    std::sort(wait.begin(),wait.end(),[](PCB a,PCB b){return a.arrive_time<b.arrive_time;});
    for(int i=0;i<wait.size();i++)
    {
       model_wait->setItem(i,0,new QStandardItem(wait[i].name));
       model_wait->setItem(i,1,new QStandardItem(QString::number(wait[i].ID)));
       model_wait->setItem(i,2,new QStandardItem(QString::number(wait[i].arrive_time)));
       model_wait->setItem(i,3,new QStandardItem(QString::number(wait[i].need_time)));
       model_wait->setItem(i,4,new QStandardItem(QString::number(wait[i].prio)));
       model_wait->setItem(i,5,new QStandardItem(QString::number(wait[i].need_mem)));
       model_wait->setItem(i,6,new QStandardItem(QString::number(wait[i].need_printer)));
    }
    tableview_wait->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableview_wait->setModel(model_wait);
}

void monixitong::refresh_begin()
{
    model_begin->clear();
    model_begin->setHorizontalHeaderLabels({"进程名","PID","到达时间","预计执行时间","优先级","申请内存容量","申请打印机数量"});
    for(int i=0;i<begin.size();i++)
    {
        model_begin->setItem(i,0,new QStandardItem(begin[i].name));
        model_begin->setItem(i,1,new QStandardItem(QString::number(begin[i].ID)));
        model_begin->setItem(i,2,new QStandardItem(QString::number(begin[i].arrive_time)));
        model_begin->setItem(i,3,new QStandardItem(QString::number(begin[i].need_time)));
        model_begin->setItem(i,4,new QStandardItem(QString::number(begin[i].prio)));
        model_begin->setItem(i,5,new QStandardItem(QString::number(begin[i].need_mem)));
        model_begin->setItem(i,6,new QStandardItem(QString::number(begin[i].need_printer)));
    }
    tableview_begin->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableview_begin->setModel(model_begin);
}

void monixitong::refresh_block()
{
    model_block->clear();
    model_block->setHorizontalHeaderLabels({"进程名","PID","优先级","剩余运行时间","占用内存大小","占用打印机数量"});
    for(int i=0;i<block.size();i++)
    {
        model_block->setItem(i,0,new QStandardItem(block[i].name));
        model_block->setItem(i,1,new QStandardItem(QString::number(block[i].ID)));
        model_block->setItem(i,2,new QStandardItem(QString::number(block[i].prio)));
        model_block->setItem(i,3,new QStandardItem(QString::number(block[i].more_time)));
        model_block->setItem(i,4,new QStandardItem(QString::number(block[i].need_mem)));
        model_block->setItem(i,5,new QStandardItem(QString::number(block[i].need_printer)));
    }
    tableview_block->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableview_block->setModel(model_block);
}

void monixitong::refresh_run()
{
    model_run->setItem(0,0,new QStandardItem(run.name));
    model_run->setItem(0,1,new QStandardItem(run.name==""? "":QString::number(run.ID)));
    model_run->setItem(0,2,new QStandardItem(run.name==""? "":QString::number(run.prio+run.wait_time%2)));
    model_run->setItem(0,3,new QStandardItem(run.name==""? "":QString::number(run.more_time)));
    model_run->setItem(0,4,new QStandardItem(run.name==""? "":QString::number(run.need_mem)));
    model_run->setItem(0,5,new QStandardItem(run.name==""? "":QString::number(run.need_printer)));
    tableview_run->setModel(model_run);
}

void monixitong::start()
{
    timer.start(2000*timeRate);
    if(alogorithm==1)
    {
        std::cout<<"开始执行非抢占式优先级调度算法"<<std::endl;
        connect(&timer,SIGNAL(timeout()),this,SLOT(alogorithm1()));
    }
    else
    {
        std::cout<<"开始执行抢占式优先级调度算法"<<std::endl;
        connect(&timer,SIGNAL(timeout()),this,SLOT(alogorithm2()));
    }
}

void monixitong::refresh_ready()
{
    model_ready->clear();
    model_ready->setHorizontalHeaderLabels({"进程名","PID","优先级","剩余执行时间","等待时间","占用内存大小","占用打印机数量"});
    std::sort(ready.begin(),ready.end(),[](PCB a,PCB b){return a.prio+a.wait_time/2>b.prio+b.wait_time/2;}); //暂时按照静态优先级排序了
    for(int i=0;i<ready.size();i++)
    {
        model_ready->setItem(i,0,new QStandardItem(ready[i].name));
        model_ready->setItem(i,1,new QStandardItem(QString::number(ready[i].ID)));
        model_ready->setItem(i,2,new QStandardItem(QString::number(ready[i].prio+ready[i].wait_time/2)));
        model_ready->setItem(i,3,new QStandardItem(QString::number(ready[i].more_time)));
        model_ready->setItem(i,4,new QStandardItem(QString::number(ready[i].wait_time)));
        model_ready->setItem(i,5,new QStandardItem(QString::number(ready[i].need_mem)));
        model_ready->setItem(i,6,new QStandardItem(QString::number(ready[i].need_printer)));
    }
    tableview_ready->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableview_ready->setModel(model_ready);
}

void monixitong::refresh_finish()
{
    for(int i=0;i<finish.size();i++)
    {
        model_finish->setItem(i,0,new QStandardItem(finish[i].name));
        model_finish->setItem(i,1,new QStandardItem(QString::number(finish[i].ID)));
        model_finish->setItem(i,2,new QStandardItem(QString::number(finish[i].arrive_time)));
        model_finish->setItem(i,3,new QStandardItem(QString::number(finish[i].finishi_time)));
        model_finish->setItem(i,4,new QStandardItem(QString::number(finish[i].wait_time+finish[i].need_time)));
        model_finish->setItem(i,5,new QStandardItem(QString::number(finish[i].prio+finish[i].wait_time/2)));
        model_finish->setItem(i,6,new QStandardItem(QString::number(finish[i].need_mem)));
        model_finish->setItem(i,7,new QStandardItem(QString::number(finish[i].need_printer)));
    }
    tableview_finish->setModel(model_finish);
}

void monixitong::alogorithm1()
{

    for(std::vector<PCB>::iterator i=ready.begin();i!=ready.end();i++)
    {
        i->wait_time++;
    }
    for(std::vector<PCB>::iterator i=block.begin();i!=block.end();i++)
    {
        i->wait_time++;
    }
    run.more_time--;
    refresh_ready();
    refresh_run();
    std::vector<PCB>::iterator it;
    for(it=wait.begin();it!=wait.end();it++)
    {
        if(it->arrive_time<=time_counter&&it->need_mem<=mem&&it->need_printer<=printer)
        {
            ready.push_back(*it);
            mem-=it->need_mem;
            printer-=it->need_printer;
            wait.erase(it);
            it--;
        }
    }
    refresh_ready();
    refresh_wait();
    if(run.name==""&&!ready.empty())
    {
        run=ready[0];
        ready.erase(ready.begin());
    }
    if(run.name!=""&&run.more_time==0)
    {
        run.finishi_time=time_counter;
        PCB n=run;
        finish.push_back(n);
        printer+=n.need_printer;
        mem+=n.need_mem;
        if(!ready.empty())
        {
            run=ready[0];
            ready.erase(ready.begin());
        }
        else
        {
            run.name="";
        }
    }
    refresh_ready();
    refresh_run();
    refresh_finish();
    refresh_block();

    time_counter++;
    label_mem->setText("内存剩余:"+QString::number(mem)+"M");
    label_printer->setText("打印机剩余"+QString::number(printer)+"台");
    label_time->setText("当前时间："+QString::number(time_counter));
}

void monixitong::alogorithm2()
{
    for(std::vector<PCB>::iterator i=ready.begin();i!=ready.end();i++)
    {
        i->wait_time++;
    }
    for(std::vector<PCB>::iterator i=block.begin();i!=block.end();i++)
    {
        i->wait_time++;
    }
    run.more_time--;
    refresh_ready();
    refresh_run();
    std::vector<PCB>::iterator it;
    for(it=wait.begin();it!=wait.end();it++)
    {
        if(it->arrive_time<=time_counter&&it->need_mem<=mem&&it->need_printer<=printer)
        {
            ready.push_back(*it);
            mem-=it->need_mem;
            printer-=it->need_printer;
            wait.erase(it);
            it--;
        }
    }
    refresh_ready();
    refresh_wait();
    if(run.name==""&&!ready.empty())
    {
        run=ready[0];
        ready.erase(ready.begin());
    }
    if(run.name!=""&&run.more_time==0)
    {
        run.finishi_time=time_counter;
        PCB n=run;
        finish.push_back(n);
        printer+=n.need_printer;
        mem+=n.need_mem;
        if(!ready.empty())
        {
            run=ready[0];
            ready.erase(ready.begin());
        }
        else
        {
            run.name="";
        }
    }
    if(run.prio+run.wait_time/2<ready[0].prio+ready[0].wait_time/2)
    {
        ready.push_back(run);
        run=ready[0];
        ready.erase(ready.begin());
    }
    refresh_ready();
    refresh_run();
    refresh_finish();
    refresh_block();

    time_counter++;
    label_mem->setText("内存剩余:"+QString::number(mem)+"M");
    label_printer->setText("打印机剩余"+QString::number(printer)+"台");
    label_time->setText("当前时间："+QString::number(time_counter));
}

void monixitong::process_block()
{
    block.emplace_back(run);
    run.name="";
}

void monixitong::process_active()
{
    int active_pid=lineedit_pid->text().toInt();
    std::cout<<"激活进程："<<active_pid<<std::endl;
    std::vector<PCB>::iterator it;
    for(it=block.begin();it!=block.end();it++)
    {
        if(it->ID==active_pid)
        {
            ready.emplace_back(*it);
            block.erase(it);
            break;
        }
    }
}

void monixitong::show_result()
{
    double average_deal_time=0;
    double average_deal_time_weighted=0;
    double sum=0;
    for(auto i:finish)
    {
        sum+=i.wait_time+i.need_time;
    }
    average_deal_time=sum/double(finish.size());
    sum=0;
    for(auto i:finish)
    {
        sum+=double(i.wait_time+i.need_time)/double(i.need_time);
    }
    average_deal_time_weighted=sum/double(finish.size());
    QLabel *new_label=new QLabel;
    new_label->setText("平均周转时间："+QString::number(average_deal_time)+'\n'+"平均带权周转时间："+QString::number(average_deal_time_weighted));
    new_label->show();
}
