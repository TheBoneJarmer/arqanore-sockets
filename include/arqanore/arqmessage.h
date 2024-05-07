#pragma once

#include <string>
#include <vector>

namespace arqanore {
    class ArqMessage {
    private:
        static const std::string PREFIX;
        static const std::string SUFFIX;

        std::vector<std::string> string_split(const std::string &s, char delim);
        std::string string_replace(const std::string &str, const std::string &what, const std::string &with);

    public:
        std::string action;
        std::string body;

        void parse(std::string input);

        std::string to_string();
    };
}