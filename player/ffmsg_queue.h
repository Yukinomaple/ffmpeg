#ifndef FFMSG_QUEUE_H
#define FFMSG_QUEUE_H
#include "SDL.h"

typedef struct AVMessage{
    int what;//消息标识符
    int arg1;//传入参数1
    int arg2;//传入参数2
    void* obj;//两个参数不够的时候使用，堆上分配
    void (*free_l)(void* obj);//obj函数堆上的数据，给出该函数进行释放
    struct AVMessage* next;
}AVMessage;

typedef struct MessageQueue{
    AVMessage* first_msg, *last_msg;//头部消息，尾部消息
    int nb_message;//队列中消息的个数
    int abort_request;//请求终止消息队列
    SDL_mutex* mutex;//互斥量
    SDL_cond* cond;//条件变量
    AVMessage* recycle_msg;//消息循环用
    int recycle_count;//循环的次数
    int allloc_count;//分配的次数
}MessageQueue;

//释放msg的obj资源
void msg_free_res(AVMessage* msg);
//私有插入消息
int msg_queue_put_private(MessageQueue* q, AVMessage* msg);
//插入消息
int msg_queue_put(MessageQueue* q, AVMessage* msg);
//初始化消息
void msg_init_msg(AVMessage* msg);
//插入简单消息，只带消息类型，不带参数
void msg_queue_put_simple1(MessageQueue* q, int what);
void msg_queue_put_simple2(MessageQueue *q, int what, int arg1);
void msg_queue_put_simple3(MessageQueue *q, int what, int arg1, int arg2);
void msg_queue_put_simple4(MessageQueue *q, int what, int arg1, int arg2, void* obj, int obj_len);
//释放msg的obj资源
void msg_obj_free_l(void* obj);
//消息队列初始化
void msg_queue_init(MessageQueue* q);
//消息队列flush，清空所有消息
void msg_queue_flush(MessageQueue* q);
//消息销毁
void msg_queue_destory(MessageQueue* q);
//消息队列终止
void msg_queue_abort(MessageQueue* q);
//启用消息队列
void msg_queue_start(MessageQueue* q);


int msg_queue_get(MessageQueue* q, AVMessage* msg, int block);

void msg_queue_remove(MessageQueue* q, int what);

#endif // FFMSG_QUEUE_H
