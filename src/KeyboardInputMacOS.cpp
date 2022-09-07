/**
 * The KeyboardInput implementation file for macOS.
 **/

#include "KeyboardInput.h"
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

static const int IGNORE_KEY = 91;
static const int BACKSPACE = 51;

static const UniChar* toUniChar(const std::wstring& string){
    int size = ( string.size() * sizeof(wchar_t));
    UniChar* uniCharString = (UniChar*) malloc(string.size() * sizeof(UniChar));
    for (int i=0;i<string.size();i++){
        uniCharString[i] = string[i];
    }
    return uniCharString;
}
void KeyboardInput::inputString(const std::wstring& string){
    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
    CGEventRef evt = CGEventCreateKeyboardEvent(src, (CGKeyCode) IGNORE_KEY, true);
    const UniChar* uniCharString = toUniChar(string);
    CGEventKeyboardSetUnicodeString(evt, string.size(),uniCharString);
    CGEventPost (kCGHIDEventTap, evt);
    CFRelease (evt); 
    CFRelease (src);
    free((void *)uniCharString);
    usleep(60);
}
void KeyboardInput::backspace(){
    CGEventSourceRef src = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);
    CGEventRef evt = CGEventCreateKeyboardEvent(src, (CGKeyCode) BACKSPACE, true);
    CGEventPost (kCGHIDEventTap, evt);
    CFRelease (evt); CFRelease (src);
    usleep(60);
}