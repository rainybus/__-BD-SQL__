#pragma once

#include <fstream>
#include <shared_mutex>
#include <string>
using namespace std;

class Logger {
public:
    Logger();
    ~Logger();

    void write(const string& to, const string& _currUser,
        const string& content);
    string read(int line);

private:
    fstream file_;
    shared_mutex sh_;
};
