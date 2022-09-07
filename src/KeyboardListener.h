#pragma once
#include "Keys.h"
#include "KeyboardInput.h"
//The implementation file for the keyboard listener.
//NOTE: This will be different depending on the OS.
#define IMPLEMENTATION_FILE "KeyboardListenerMacOS.tpp"

namespace KeyboardListener{
     /**
     * The function which creates a global listener for the OS
     * and uses the template parameter function "onKeyPress" as a
     * callback function.
     * 
     * @tparam onKeyPress The callback function for this keyboard listener.
     * This function should accept a key (representing the key being typed),
     * and the shift and caps booleans, representing whether or not shift is 
     * being held and whether caps-lock is active.
     **/
    template <void (*onKeyPress)(const Keys::Key, bool shift, bool caps)>
    void createListener();
}

#include IMPLEMENTATION_FILE
