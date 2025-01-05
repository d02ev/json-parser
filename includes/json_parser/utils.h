#ifndef UTILS_H
#define UTILS_H

#include <string>

class Utils {
  public:
    Utils() = default;
    ~Utils() = default;

    std::string static cvt_file_stream_to_string(const char* file);
    std::string static stringify(const std::string& str_file_content);
    bool static is_number(const std::string& str);
};

#endif //UTILS_H
