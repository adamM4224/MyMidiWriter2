#include "MyLogger.h"

MyLogger::MyLogger(const String& filename)
    : file(filename)
{
}

void MyLogger::write(const String& message) {
    file.appendText(message + "\n");
}