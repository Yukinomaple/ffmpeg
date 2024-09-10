#ifndef IJKMEDIAPLAYER_H
#define IJKMEDIAPLAYER_H
#include <functional>
#include <mutex>
#include <thread>
#include <iostream>
#include "ff_ffplay.h"
#include "ffmsg_queue.h"

/*-
 * ijkmp_set_data_source()  -> MP_STATE_INITIALIZED
 *
 * ijkmp_reset              -> self
 * ijkmp_release            -> MP_STATE_END
 */
const int MP_STATE_IDLE = 0;

/*-
 * ijkmp_prepare_async()    -> MP_STATE_ASYNC_PREPARING
 *
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
const int MP_STATE_INITIALIZED = 1;

/*-
 *                   ...    -> MP_STATE_PREPARED
 *                   ...    -> MP_STATE_ERROR
 *
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
const int MP_STATE_ASYNC_PREPARING = 2;

/*-
 * ijkmp_seek_to()          -> self
 * ijkmp_start()            -> MP_STATE_STARTED
 *
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
const int MP_STATE_PREPARED = 3;

/*-
 * ijkmp_seek_to()          -> self
 * ijkmp_start()            -> self
 * ijkmp_pause()            -> MP_STATE_PAUSED
 * ijkmp_stop()             -> MP_STATE_STOPPED
 *                   ...    -> MP_STATE_COMPLETED
 *                   ...    -> MP_STATE_ERROR
 *
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
const int MP_STATE_STARTED = 4;

/*-
 * ijkmp_seek_to()          -> self
 * ijkmp_start()            -> MP_STATE_STARTED
 * ijkmp_pause()            -> self
 * ijkmp_stop()             -> MP_STATE_STOPPED
 *
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
const int MP_STATE_PAUSED = 5;

/*-
 * ijkmp_seek_to()          -> self
 * ijkmp_start()            -> MP_STATE_STARTED (from beginning)
 * ijkmp_pause()            -> self
 * ijkmp_stop()             -> MP_STATE_STOPPED
 *
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
const int MP_STATE_COMPLETED = 6;

/*-
 * ijkmp_stop()             -> self
 * ijkmp_prepare_async()    -> MP_STATE_ASYNC_PREPARING
 *
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
const int MP_STATE_STOPPED = 7;

/*-
 * ijkmp_reset              -> MP_STATE_IDLE
 * ijkmp_release            -> MP_STATE_END
 */
const int MP_STATE_ERROR = 8;

/*-
 * ijkmp_release            -> self
 */
const int MP_STATE_END = 9;

class IjkMediaPlayer
{
public:
    IjkMediaPlayer();
    ~IjkMediaPlayer();
    int ijkmp_create(std::function<int(void*)> msg_loop);
    int ijkmp_destory();
    // 设置要播放的url
    int ijkmp_set_data_source(const char* url);
    // 准备播放
    int ijkmp_prepare_async();
    // 触发播放
    int ijkmp_start();
    // 停止
    int ijkmp_stop();
    // 读取消息
    int ijkmp_get_msg(AVMessage *msg, int block);
    int ijkmp_msg_loop(void* arg);
private:
    std::mutex _mutex;
    FFPlayer* _ffplayer = NULL;
    std::function<int(void *)> _msg_loop = NULL;
    std::thread* _msg_thread;
    char* _data_source;
    int _mp_state;

};

#endif // IJKMEDIAPLAYER_H
