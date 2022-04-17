#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP

class Debugger {
    private:
    static int warningCount;
    static int errorCount;
    public:
    static void print(const char* log);
    static void warning(const char* log);
    static void error(const char* log);
};

#endif