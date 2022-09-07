#pragma once
#include <string>
#include "Keys.h"
#include <stdexcept>
#include <iostream>
#include <unordered_map>
#include "MacroParser.h"
#include "mainwindow.h"

/**
 * The class which holds the high-level logic for macros. 
 **/
class ExtendedKeyboard{
public:
    /**
     * The method which is being called on each key press detected
     * by the OS. It requires three functions passed onto the template
     * parameters; inputString, backspace, and getProcessName.
     * 
     * @tparam inputString The function which is used to send string input to the OS
     * keyboard. Takes in a string parameter.
     * @tparam backspace The function which sends a 'backspace' call to the OS
     * keyboard.
     * @tparam getProcessName The function which retrieves the name of the OS' active
     * window.
     **/
    template <void (*inputString)(const std::wstring& string),
        void (*backspace)(void),std::wstring (*getProcessName)(void)> 
    static void onKeyPress(const Keys::Key key, bool shift, bool caps){
        if (key == Keys::F3 && shift){
            gui.showExlusionOptions(getProcessName());
        }
        else if (key == Keys::F3){
            off = !off;
        }
        if (off){
            return;
        }
        if (key == Keys::DEL){
            if (backspaceLog > 0){
                backspaceLog--;
                return;
            }
            buffer[bufferIndex]=L' ';
            if (bufferIndex > 0) bufferIndex--;
            return;
        }
        addCharToBuffer(Keys::toChar(key,shift,caps));
        std::pair<std::wstring,int> mapping = trie.getMapping<getProcessName>(buffer,bufferIndex);
        if (mapping.first != L""){
            replaceText<inputString,backspace>(mapping.second,mapping.first);
        }
    }
    /**
    * Sets the size of the string buffer, which is used to store the characters
    * that have been typed. E.g. a bufferSize of 30 will store 30 of the last typed
    * characters.
    * 
    * @param bufferSize The size of the string buffer to set.
    **/
    static void setBufferSize(int bufferSize){
        std::ios_base::sync_with_stdio(false);
        std::locale utf8( std::locale(), new std::codecvt_utf8_utf16<wchar_t> );
        std::wcout.imbue(utf8);
        if (bufferSize < 1){
            throw std::runtime_error("Buffer size is " + std::to_string(bufferSize) );
        }
        ExtendedKeyboard::bufferSize = bufferSize;
        buffer.reserve(bufferSize);
        for (int i =0;i<bufferSize;i++) buffer.append(L" ");
    }
private:
    /**
    * Adds the parameter character to the buffer. If adding a character
    * to the buffer would exceed the buffer's maximum capacity, then 
    * all of the buffer's characters are shifted back by one, and the front-most
    * character is removed to make space for the new character.
    * 
    * @param c The character to be added to the buffer.
    **/
    static void addCharToBuffer(wchar_t c){
        if (c == '\0') return;
        if (bufferIndex < bufferSize){
            buffer[bufferIndex] = c;
            bufferIndex++;
        }
        else{
            for (int i =0;i<bufferIndex-1;i++){
                buffer[i]=buffer[i+1];
            }
            buffer[bufferIndex-1] = c;
        }
    }
    /**
     * Removes the last typed characters from the buffer (number specified
     * by backspaceAmount) and replaces the text with the 'replacement' 
     * parameter string. 
     * 
     * @tparam inputString The function which is used to send string input to the OS
     * keyboard. Takes in a string parameter.
     * @tparam backspace The function which sends a 'backspace' call to the OS
     * keyboard.
     * @param backspaceAmount The number of times to send a 'backspace' 
     * call to the OS to remove the macro text (e.g '\lambda').
     * @param replacement The string to replace the macro text (e.g 'λ').
     * This string is send to the OS as keyboard input.
     * 
     **/
    template <void (*inputString)(const std::wstring& string),void (*backspace)(void)>
    static void replaceText(int backspaceAmount, std::wstring& replacement){
        for (int i=0;i<backspaceAmount;i++) backspace();
        inputString(replacement);
        backspaceLog = backspaceAmount;
        for (int i =0;i<replacement.size();i++) addCharToBuffer(replacement[i]);
    }

    static int bufferSize;
    static int bufferIndex;
    static std::wstring buffer;
    static Trie trie;
    static bool off;
    static int backspaceLog;
    static std::unordered_map<std::string,std::unordered_set<std::wstring>*> excludesMap;
    static MainWindow gui;
    static MacroParser macroParser;
};
MacroParser ExtendedKeyboard::macroParser;
int ExtendedKeyboard::bufferSize = 20;
int ExtendedKeyboard::bufferIndex = 0;
std::wstring ExtendedKeyboard::buffer;
std::unordered_map<std::string,std::unordered_set<std::wstring>*> ExtendedKeyboard::excludesMap = macroParser.getExcludesMap();
Trie ExtendedKeyboard::trie = macroParser.getTrie();
bool ExtendedKeyboard::off = false;
int ExtendedKeyboard::backspaceLog = 0;
MainWindow ExtendedKeyboard::gui = MainWindow::createGUI(macroParser);
