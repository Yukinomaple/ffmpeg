#ifndef FF_FFPLAY_H
#define FF_FFPLAY_H

#include <thread>
#include "ffmsg_queue.h"
#include <functional>

class FFPlayer
{
public:
    FFPlayer();
    int ffp_create();
    int ffp_prepare_async_l(char* file_name);
    int stream_open(const char* file_name);
    MessageQueue _msg_queue;
    char* _input_filename;
    int read_thraed();
    std::thread* _read_thread;
};
inline static void ffp_notify_msg1(FFPlayer* ffp, int what) {
    msg_queue_put_simple3(&ffp->_msg_queue, what, 0, 0);
}

inline static void ffp_notify_msg2(FFPlayer* ffp, int what, int arg1) {
    msg_queue_put_simple3(&ffp->_msg_queue, what, arg1, 0);
}

inline static void ffp_notify_msg3(FFPlayer* ffp, int what, int arg1, int arg2) {
    msg_queue_put_simple3(&ffp->_msg_queue, what, arg1, arg2);
}

inline static void ffp_notify_msg4(FFPlayer* ffp, int what, int arg1, int arg2, void *obj, int obj_len) {
    msg_queue_put_simple4(&ffp->_msg_queue, what, arg1, arg2, obj, obj_len);
}

inline static void ffp_remove_msg(FFPlayer* ffp, int what) {
    msg_queue_remove(&ffp->_msg_queue, what);
}
#endif // FF_FFPLAY_H
