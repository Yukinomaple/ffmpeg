#include "ff_ffplay.h"
#include <iostream>
#include <string.h>
#include "ffmsg.h"

FFPlayer::FFPlayer()
{

}

int FFPlayer::ffp_create()
{
    std::cout << "ffp_creat\n";
    msg_queue_init(&_msg_queue);
    return 0;
}

int FFPlayer::ffp_prepare_async_l(char *file_name)
{
    _input_filename = strdup(file_name);
    int reval = stream_open(file_name);
    return reval;
}

int FFPlayer::stream_open(const char *file_name)
{
    // 初始化Frame帧队列
    // 初始化Packet包队列
    // 初始化时钟

    // 插件read_thread
    _read_thread = new std::thread(&FFPlayer::read_thraed, this);
    return 0;
}

int FFPlayer::read_thraed()
{
    ffp_notify_msg1(this, FFP_MSG_OPEN_INPUT);
    std::cout << "read_thread FFP_MSG_OPEN_INPUT " << this << std::endl;
    ffp_notify_msg1(this, FFP_MSG_FIND_STREAM_INFO);
    std::cout << "read_thread FFP_MSG_FIND_STREAM_INFO " << this << std::endl;
    ffp_notify_msg1(this, FFP_MSG_COMPONENT_OPEN);
    std::cout << "read_thread FFP_MSG_COMPONENT_OPEN " << this << std::endl;
    ffp_notify_msg1(this, FFP_MSG_PREPARED);
    std::cout << "read_thread FFP_MSG_PREPARED " << this << std::endl;
    while (1) {
//        std::cout << "read_thread sleep, mp:" << this << std::endl;
        // 先模拟线程运行
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
