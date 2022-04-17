#include "../../inc/util/Debugger.hpp"
#include <stdio.h>

int Debugger::warningCount = 0;
int Debugger::errorCount = 0;

void Debugger::print(const char* log) {
    printf("%s%s%s", "[debug] ", log, "\n");
}

void Debugger::warning(const char* log) {
    ++Debugger::warningCount;
    printf("%s%s%s", "[warning] ", log, "\n");
}

void Debugger::error(const char* log) {
    ++Debugger::errorCount;
    printf("%s%s%s", "[ERROR] ", log, "\n");
}
