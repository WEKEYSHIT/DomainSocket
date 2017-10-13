#ifndef __IOMANAGER_H__
#define __IOMANAGER_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef void (*IO_Callback)(void*);
void loop_event(IO_Callback cbk);
void loop_init();
void loop_exit();
void add_event(int fd, void* data);
void del_event(int fd);

#ifdef __cplusplus
}
#endif

#endif

