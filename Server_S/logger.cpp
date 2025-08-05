#include <iostream>
#include <mutex>
#include <stdexcept>

#include "logger.h"

Logger::Logger()
{
    file_.open("log.txt", ios::out | ios::in | ios::app);
    if (!file_.is_open()) {
        throw runtime_error("error: open file");
    }
}

Logger::~Logger() { file_.close(); }

void Logger::write(const string& to, const string& _currUser,
    const string& content)
{
    unique_lock<shared_mutex> l(sh_, try_to_lock);
    file_.seekp(0, ios::end);
    if (l.owns_lock()) {
        file_ << "to " << to << ". from " << _currUser
            << ". messages: " << content << std::endl;
    }
}

string Logger::read(int line)
{
    string str;
    shared_lock l(sh_);
    ifstream file_("log.txt");

    file_.clear();
    file_.seekg(0, ios::beg);
    int i;
    for (i = 0; i < line; ++i) {
        std::getline(file_, str);
    }
    return str;
}