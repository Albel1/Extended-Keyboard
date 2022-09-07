#pragma once
#include <string>
namespace ProcessNameFetcher{
    /**
     * The function which retrieves the name of the active window's
     * process. For example if the window we are typing in is IntelliJ IDE
     * (i.e. IntelliJ IDE is the active window), the function should
     * return "IntelliJ IDE" when it is called for that instance.
     * 
     * @return The name of the active window's process.
     * 
     **/
    std::wstring getProcessName();
}
