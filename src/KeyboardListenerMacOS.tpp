/**
 * The KeyboardListener implementation file for macOS.
 **/

#include "KeyboardListener.h"
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

static const Keys::Key toKey(int keyCode){
    using namespace Keys;
    switch (keyCode) {
        case 0:   return A;
        case 1:   return S;
        case 2:   return D;
        case 3:   return F;
        case 4:   return H;
        case 5:   return G;
        case 6:   return Z;
        case 7:   return X;
        case 8:   return C;
        case 9:   return V;
        case 11:  return B;
        case 12:  return Q;
        case 13:  return W;
        case 14:  return E;
        case 15:  return R;
        case 16:  return Y;
        case 17:  return T;
        case 18:  return ONE;
        case 19:  return TWO;
        case 20:  return THREE;
        case 21:  return FOUR;
        case 22:  return SIX;
        case 23:  return FIVE;
        case 24:  return EQUALS;
        case 25:  return NINE;
        case 26:  return SEVEN;
        case 27:  return HYPHEN;
        case 28:  return EIGHT;
        case 29:  return ZERO;
        case 30:  return RIGHT_BRACKET;
        case 31:  return O;
        case 32:  return U;
        case 33:  return LEFT_BRACKET;
        case 34:  return I;
        case 35:  return P;
        case 37:  return L;
        case 38:  return J;
        case 39:  return QUOTE;
        case 40:  return K;
        case 41:  return COLON;
        case 42:  return PIPE;
        case 43:  return COMMA;
        case 44:  return QUESTION_MARK;
        case 45:  return N;
        case 46:  return M;
        case 47:  return PERIOD;
        case 50:  return TILDE;
        case 65:  return DECIMAL;
        case 67:  return ASTERISK;
        case 69:  return PLUS;
        case 71:  return CLEAR;
        case 75:  return DIVIDE;
        case 76:  return ENTER;
        case 78:  return HYPHEN;
        case 81:  return EQUALS;
        case 82:  return RIGHT_ZERO;
        case 83:  return RIGHT_ONE;
        case 84:  return RIGHT_TWO;
        case 85:  return RIGHT_THREE;
        case 86:  return RIGHT_FOUR;
        case 87:  return RIGHT_FIVE;
        case 88:  return RIGHT_SIX;
        case 89:  return RIGHT_SEVEN;
        case 91:  return RIGHT_EIGHT;
        case 92:  return RIGHT_NINE;
        case 36:  return RETURN;
        case 48:  return TAB;
        case 49:  return SPACE;
        case 51:  return DEL;
        case 53:  return ESC;
        case 54:  return RIGHT_CMD;
        case 55:  return LEFT_CMD;
        case 56:  return LEFT_SHIFT;
        case 57:  return CAPS;
        case 58:  return LEFT_OPTIONAL;
        case 59:  return LEFT_CTRL;
        case 60:  return RIGHT_SHIFT;
        case 61:  return RIGHT_OPTION;
        case 62:  return RIGHT_CTRL;
        case 63:  return FN;
        case 64:  return F17;
        case 72:  return VOLUP;
        case 73:  return VOLDOWN;
        case 74:  return MUTE;
        case 79:  return F18;
        case 80:  return F19;
        case 90:  return F20;
        case 96:  return F5;
        case 97:  return F6;
        case 98:  return F7;
        case 99:  return F3;
        case 100: return F8;
        case 101: return F9;
        case 103: return F11;
        case 105: return F13;
        case 106: return F16;
        case 107: return F14;
        case 109: return F10;
        case 111: return F12;
        case 113: return F15;
        case 114: return HELP;
        case 115: return HOME;
        case 116: return PGUP;
        case 117: return FWDDEL;
        case 118: return F4;
        case 119: return END;
        case 120: return F2;
        case 121: return PGDOWN;
        case 122: return F1;
        case 123: return LEFT;
        case 124: return RIGHT;
        case 125: return DOWN;
        case 126: return UP;
        case 160: return F3;
    }
    return UNKNOWN;
}
template <void (*onKeyPress)(const Keys::Key, bool shift, bool caps)>
static CGEventRef eventCallBack(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    static const int CAPS_LOCK = 57;
    static const int LEFT_SHIFT = 56;
    static const int RIGHT_SHIFT = 60;
    static int keyCodeCache[127];
    if (type != kCGEventKeyDown && type != kCGEventFlagsChanged) {
        return event;
    }
    CGKeyCode keyCode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    if (keyCode == LEFT_SHIFT || keyCode == RIGHT_SHIFT || keyCode == CAPS_LOCK) {
        keyCodeCache[keyCode] = keyCodeCache[keyCode] == 1 ? 0 : 1;
        return event;
    }
    bool shift = keyCodeCache[RIGHT_SHIFT] + keyCodeCache[LEFT_SHIFT] > 0;
    bool caps = keyCodeCache[CAPS_LOCK] == 1;
    onKeyPress(toKey(keyCode),shift,caps);
    return event;
}

template <void (*onKeyPress)(const Keys::Key, bool shift, bool caps)>
void KeyboardListener::createListener(){
    CGEventMask eventMask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged);
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap, kCGHeadInsertEventTap, (CGEventTapOptions) 0, 
        eventMask, eventCallBack<onKeyPress>, NULL
    );
    if (!eventTap) {
        throw std::runtime_error("ERROR: Unable to create event tap.\n");
        exit(1);
    }
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    CFRunLoopRun();
}