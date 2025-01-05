#include <fstream>
#include <sstream>
#include <algorithm>
#include "json_parser/utils.h"

std::string Utils::cvt_file_stream_to_string(const char *file) {
  std::string file_contents;
  {
    std::ifstream json_file(file, std::ios::binary);
    std::stringstream json_stream;

    json_stream << json_file.rdbuf();
    file_contents = json_stream.str();
  }

  return file_contents;
}

std::string Utils::stringify(const std::string &str_file_content) {
  std::string json_with_whitespaces, stringified_json;
  int inside_string = 0;
  for (const char &ch: str_file_content) {
    if (ch != '\n') {
      json_with_whitespaces.push_back(ch);
    }
  }
  for (const char &ch: json_with_whitespaces) {
    if (ch == '"') {
      inside_string = !inside_string;
      stringified_json.push_back(ch);
    } else if (inside_string) {
      stringified_json.push_back(ch);
    } else if (!std::isspace(static_cast<unsigned char>(ch))) {
      stringified_json.push_back(ch);
    }
  }

  return stringified_json;
}

bool Utils::is_number(const std::string &str) {
  return std::ranges::all_of(str, ::isdigit);
}

