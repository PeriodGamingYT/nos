#ifndef __TIMER_H
#define __TIMER_H
void timer_delay(unsigned int);
void timer_install(unsigned int);
void timer_handler_set(void (*)());
#endif
