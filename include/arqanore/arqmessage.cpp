#include <sstream>
#include "arqmessage.h"

const std::string arqanore::ArqMessage::PREFIX = "<ARQ>";
const std::string arqanore::ArqMessage::SUFFIX = "</ARQ>";

std::vector<std::string> arqanore::ArqMessage::string_split(const std::string &s, char delim) {
    std::stringstream stream(s);
    std::string item;
    std::vector<std::string> result;

    while (getline(stream, item, delim)) {
        result.push_back(item);
    }

    return result;
}

std::string arqanore::ArqMessage::string_replace(const std::string &str, const std::string &what, const std::string &with) {
    auto src = str;
    auto pos = src.find(what);

    if (pos == std::string::npos) {
        return str;
    }

    src.replace(pos, what.length(), with);
    return src;
}

void arqanore::ArqMessage::parse(std::string input) {
    input = string_replace(input, PREFIX, "");
    input = string_replace(input, SUFFIX, "");
    input = string_replace(input, "#$#", "#");

    action = string_split(input, '#')[0];
    body = string_split(input, '#')[1];
}

std::string arqanore::ArqMessage::to_string() {
    return PREFIX + action + "#$#" + body + SUFFIX;
}
