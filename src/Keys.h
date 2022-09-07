#pragma once
namespace Keys{
    enum Key {A, S, D, F, H, G, Z, X, C, V, B, Q, W, E, R, Y, T, O, U, I, P, L, J, K, N, M, SPACE,
             ONE, TWO, THREE, FOUR, SIX, FIVE, NINE, SEVEN, EIGHT, ZERO, RIGHT_BRACKET, LEFT_BRACKET,
             QUOTE, COLON, PIPE, COMMA, QUESTION_MARK, PERIOD, TILDE, HYPHEN, EQUALS, DECIMAL, ASTERISK, PLUS, CLEAR, DIVIDE, ENTER,
              RIGHT_ZERO, RIGHT_ONE, RIGHT_TWO, RIGHT_THREE, RIGHT_FOUR, RIGHT_FIVE, RIGHT_SIX, 
              RIGHT_SEVEN, RIGHT_EIGHT, RIGHT_NINE, RETURN, TAB, DEL, ESC, RIGHT_CMD, LEFT_CMD, LEFT_SHIFT, CAPS, 
              LEFT_OPTIONAL, LEFT_CTRL, RIGHT_SHIFT, RIGHT_OPTION, RIGHT_CTRL, FN, F17, VOLUP, VOLDOWN, MUTE, 
              F18, F19, F20, F5, F6, F7, F3, F8, F9, F11, F13, F16, F14, F10, F12, F15, HELP, HOME, PGUP, FWDDEL, 
              F4, END, F2, PGDOWN, F1, LEFT, RIGHT, DOWN, UP,UNKNOWN};
    static const wchar_t KEY_VALUES[] = {
        'a','s','d','f','h','g','z','x','c','v','b','q','w','e','r','y','t','o','u','i','p','l','j','k','n','m',' ',
        '1','2','3','4','6','5','9','7','8','0',']','[','\'',';','\\',',','/','.','`','-','=',
        'A','S','D','F','H','G','Z','X','C','V','B','Q','W','E','R','Y','T','O','U','I','P','L','J','K','N','M',' ',
        '!','@','#','$','^','%','(','&','*',')','}','{','"',':', '|', '<','?', '>','~','_','+'
    };
    /**
     * The function which converts the information known about
     * the key(s) pressed into a wide character. 
     * NOTE: The implementation is dependent on the OS and keyboard
     * language. The keyboard language is assumed to be English.
     * 
     * @param key The key that has been pressed.
     * @param shift Whether or not the shift key is being held.
     * @param caps Whether or not caps-lock is active.
     * 
     * @return The resultant character from the known key(s) pressed.
     * For example toChar(key=C,shift=true,caps=false) would return 'C'.
     * A non-textual key like F2 would return a null terminator character.
     **/ 
    wchar_t toChar(Key key, bool shift, bool caps);
};
