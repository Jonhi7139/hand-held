#ifndef BUTTONS_H
#define BUTTONS_H

struct button
{
    int pin;
    int state_now;
    int last_state;
    const char *name;

};

void buttons_task(void *parameters);

#endif