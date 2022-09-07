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

//#include "mainwindow.h"
//#include <QApplication>
//int main(int argc, char *argv[])
//{
//    return 0;
//    MainWindow w;
//    w.showExlusionOptions();
//    int x = 0;
//    QApplication a(x, {});
//    MainWindow w;
//    w.show();
//    return a.exec();
//}
