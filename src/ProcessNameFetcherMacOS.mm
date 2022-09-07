#include "ProcessNameFetcher.h"
#import <ApplicationServices/ApplicationServices.h>
#import <Carbon/Carbon.h>

static char* processNameCString(CFStringRef aString) {
  if (aString == NULL) {
    return NULL;
  }
  CFIndex length = CFStringGetLength(aString);
  CFIndex maxSize =
  CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
  char* buffer = (char*)malloc(maxSize);
  if (CFStringGetCString(aString, buffer, maxSize,
                         kCFStringEncodingUTF8)) {
    return buffer;
  }
  free(buffer);
  return NULL;
}

static char* getActiveWindowName(){
    ProcessSerialNumber psn = { 0L, 0L };
    OSStatus err = GetFrontProcess(&psn);
    CFStringRef processName = NULL;
    err = CopyProcessName(&psn, &processName);
    char* cName = processNameCString(processName);
    return cName;
}

std::wstring ProcessNameFetcher::getProcessName(){
    char* nameCString = getActiveWindowName();
    std::string nameCPPString = std::string(nameCString);
    std::wstring nameCPPWstring(nameCPPString.begin(), nameCPPString.end());
    free(nameCString);
    return nameCPPWstring;
}
