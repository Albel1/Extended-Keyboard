#include "ExtendedKeyboard.h"
#include "KeyboardListener.h"
#include "KeyboardInput.h"
#include "MacroParser.h"
#include "ProcessNameFetcher.h"

int main(){
    ExtendedKeyboard::setBufferSize(20);
    KeyboardListener::createListener
    <
        ExtendedKeyboard::onKeyPress
        <
            KeyboardInput::inputString,
            KeyboardInput::backspace,
            ProcessNameFetcher::getProcessName
        >
    >();
    return 0;
}