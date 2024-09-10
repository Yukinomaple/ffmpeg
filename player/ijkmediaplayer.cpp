#include "ijkmediaplayer.h"
#include <string.h>
#include "ffmsg.h"

IjkMediaPlayer::IjkMediaPlayer()
{
    std::cout << " IjkMediaPlayer()\n ";
}

IjkMediaPlayer::~IjkMediaPlayer()
{
    std::cout << " ~IjkMediaPlayer()\n ";
}

int IjkMediaPlayer::ijkmp_create(std::function<int(void*)> msg_loop)
{
    int ret = 0;
    _ffplayer = new FFPlayer();
    if(!_ffplayer){
        std::cout << " new FFPlayer() failed" << std::endl;
        return -1;
    }
    _msg_loop = msg_loop;
    ret = _ffplayer->ffp_create();
    if(ret < 0){
        return -1;
    }
    return ret;
}

int IjkMediaPlayer::ijkmp_destory()
{
    return 0;
}

int IjkMediaPlayer::ijkmp_set_data_source(const char *url)
{
    if(!url){
        return -1;
    }
    _data_source = strdup(url);
    return 0;
}

int IjkMediaPlayer::ijkmp_prepare_async()
{
    //TODO:判断状态
    _mp_state = MP_STATE_ASYNC_PREPARING;
    //启用消息队列
    msg_queue_start(&_ffplayer->_msg_queue);
    //创建循环线程
    _msg_thread = new std::thread(&IjkMediaPlayer::ijkmp_msg_loop, this, this);
    //调用ffplayer
    int ret = _ffplayer->ffp_prepare_async_l(_data_source);
    if(ret < 0){
        _mp_state = MP_STATE_ERROR;
        return -1;
    }
    return ret;
}

int IjkMediaPlayer::ijkmp_start()
{
    ffp_notify_msg1(_ffplayer, FFP_REQ_START);
    return 0;
}

int IjkMediaPlayer:: ijkmp_get_msg(AVMessage *msg, int block)
{
    while(1){
        int continue_wait_next_msg = 0;
        int retval = msg_queue_get(&_ffplayer->_msg_queue, msg, block);
        if(retval <= 0){
            return retval;
        }
        switch (msg->what) {
        case FFP_MSG_PREPARED:
            std::cout << __FUNCTION__ << " FFP_MSG_PREPARED" << std::endl;
            break;
        case FFP_REQ_START:
            std::cout << __FUNCTION__ << " FFP_REQ_START" << std::endl;
            continue_wait_next_msg = 1;
            //ffplayer->start();
            break;
        default:
            std::cout << __FUNCTION__ << " default " << msg->what << std::endl;
            break;
        }
        if(continue_wait_next_msg){
            msg_free_res(msg);
            continue;
        }
        return retval;
    }
    return -1;
}

int IjkMediaPlayer::ijkmp_msg_loop(void *arg)
{
    _msg_loop(arg);
    return 0;
}
