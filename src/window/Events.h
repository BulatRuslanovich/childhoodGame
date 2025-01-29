//
// Created by getname on 27.01.2025.
//

#ifndef EVENTS_H
#define EVENTS_H

#include "Window.h"

typedef unsigned int uint;

class Events {
public:
    static bool* _keys;
    static uint* _frames;
    static uint _current;
    static float y;
    static float x;
    static float dx;
    static float dy;

    static bool _cursor_locked;
    static bool _cursor_started;

    static int init();
    static void pullEvents();

    static bool pressed(int key);
    static bool jpressed(int key);

    static bool clicked(int button);
    static bool jclicked(int button);

    static void toogleCursor();
};



#endif //EVENTS_H
