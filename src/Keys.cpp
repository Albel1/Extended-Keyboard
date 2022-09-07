#include "Keys.h"

wchar_t Keys::toChar(Keys::Key key, bool shift, bool caps){
    int capitalise = 48;
    int modifier = 0;
    if (key <= 26){
        modifier = (shift || caps) ? capitalise:0;
    }
    else if (26 < key && key <= 48){
        modifier =  shift ? capitalise:0;
    }
    else{
        return '\0';
    }
    return KEY_VALUES[key+modifier];
};