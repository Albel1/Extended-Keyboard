#pragma once
#include <string>
namespace KeyboardInput{
    /**
     * The function which is used to send string input to the OS
     * virtual keyboard.
     * 
     * @param string The string which the OS keyboard must send.
     **/
    void inputString(const std::wstring& string);
    /**
     * The function which sends a 'backspace' call to the OS
     * keyboard.
     **/
    void backspace();
}
