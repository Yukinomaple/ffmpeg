#include "mainwind.h"
#include "ui_mainwind.h"
#include <QDebug>
#include <thread>
#include <functional>
#include <iostream>
#include "ffmsg.h"
MainWind::MainWind(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWind)
{
    ui->setupUi(this);
    //InitUI();
    InitSignalsAndSlots();
    //InitPlayer();
}

MainWind::~MainWind()
{
    delete ui;
}

void MainWind::InitSignalsAndSlots()
{
    connect(ui->ctrlBarWind, &CtrlBar::SigPlayOrPause, this, &MainWind::OnPlayOrPause);
}

int MainWind::message_loop(void *arg)
{
    IjkMediaPlayer* mp = (IjkMediaPlayer*)arg;
    qDebug() << "message_loop into";
    while(1){
        AVMessage msg;
        //取消息队列的消息，如果没有消息就阻塞，直到有消息被发到消息队列。
        int retval = mp->ijkmp_get_msg(&msg, 1);
        if(retval < 0){
            break;
        }
        switch(msg.what){
        case FFP_MSG_FLUSH:
            qDebug() << __FUNCTION__ << " FFP_MSG_FLUSH";
            break;
        case FFP_MSG_PREPARED:
            std::cout << __FUNCTION__ << " FFP_MSG_PREPARED" << std::endl;
            mp->ijkmp_start();
            break;
        default:
           qDebug()  << __FUNCTION__ << " default " << msg.what ;
          break;
        }
        msg_free_res(&msg);
        // 先模拟线程运行
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    qDebug() << "message_loop leave";
    return 0;
}

void MainWind::OnPlayOrPause()
{
    int ret = 0;
    if(!_mp){
        _mp = new IjkMediaPlayer();
        ret = _mp->ijkmp_create(std::bind(&MainWind::message_loop, this, std::placeholders::_1));
        if(ret < 0){
            qDebug() << "IjkMediaPlayer create failed";
            delete _mp;
            _mp = NULL;
            return;
        }
        _mp->ijkmp_set_data_source("believe.mp4");
        ret = _mp->ijkmp_prepare_async();
        if(ret < 0){
            qDebug() << "IjkMediaPlayer create failed";
            delete _mp;
            _mp = NULL;
            return;
        }
    } else {
        //TODO:已经准备好暂停或者恢复播放了
    }
}



