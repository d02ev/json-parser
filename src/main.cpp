#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

enum class TokenType {
  OpenParen,
  CloseParen,
  OpenQuote,
  CloseQuote,
  Colon,
  KeyName,
  KeyValue,
  Comma,
};

struct Token {
  TokenType type;
  std::string value;
  std::string data_type;
};

std::string enum_to_string(const TokenType type) {
  switch (type) {
    case TokenType::OpenParen:
      return "OpenParen";
    case TokenType::CloseParen:
      return "CloseParen";
    case TokenType::OpenQuote:
      return "OpenQuote";
    case TokenType::CloseQuote:
      return "CloseQuote";
    case TokenType::Colon:
      return "Colon";
    case TokenType::KeyName:
      return "KeyName";
    case TokenType::KeyValue:
      return "KeyValue";
    case TokenType::Comma:
      return "Comma";
    default:
      return "";
  }
}

// removes all the whitespaces, new line chars and tabs (except for string values)
std::string stringify(const std::string &file_content) {
  std::string json_with_whitespaces, stringified_json;
  int inside_string = 0;
  for (const char& ch : file_content) {
    if (ch != '\n' && ch != '\t') {
      json_with_whitespaces.push_back(ch);
    }
  }
  for (const char& ch : json_with_whitespaces) {
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

bool is_number(const std::string &str) { return std::ranges::all_of(str, ::isdigit); }

std::vector<Token> tokenize(const std::string &stringified_json) {
  if (stringified_json.empty() || stringified_json.at(0) != '{') {
    exit(EXIT_FAILURE);
  }

  std::vector<Token> tokens;
  std::string buf;

  for (int i = 0; i < stringified_json.length(); ++i) {
    if (stringified_json.at(i) == '{') {
      tokens.push_back({TokenType::OpenParen, "{", "nv"});
      continue;
    }
    if (stringified_json.at(i) == '"') {
      tokens.push_back({TokenType::OpenQuote, "\"", "nv"});
      ++i;

      if (i < stringified_json.length() && (stringified_json.at(i - 2) == '{' || stringified_json.at(i - 2) == ',')) {
        while (stringified_json.at(i) != '"') {
          buf.push_back(stringified_json.at(i++));
        }

        tokens.push_back({TokenType::KeyName, buf, "string"});
        buf.clear();
        tokens.push_back({TokenType::CloseQuote, "\"", "nv"});
      }
    }
    if (stringified_json.at(i) == ':') {
      tokens.push_back({TokenType::Colon, ":", "nv"});
      ++i;

      if (stringified_json.at(i) == '"') {
        tokens.push_back({TokenType::OpenQuote, "\"", "nv"});
        ++i;
        while (i < stringified_json.length() && stringified_json.at(i) != '"') {
          buf.push_back(stringified_json.at(i++));
        }
        buf.push_back(stringified_json.at(i));
      } else if (stringified_json.at(i) == '{') {
        while (i < stringified_json.length() && stringified_json.at(i) != '}') {
          buf.push_back(stringified_json.at(i++));
        }
      } else if (stringified_json.at(i) == '[') {
        while (i < stringified_json.length() && stringified_json.at(i) != ']') {
          buf.push_back(stringified_json.at(i++));
        }
      } else {
        while (stringified_json.at(i) != ',' || stringified_json.at(i) != '}') {
          if (stringified_json.at(i) == ',' || stringified_json.at(i) == '}')
            break;
          buf.push_back(stringified_json.at(i++));
        }
      }

      std::string data_type;
      if (buf.find('"') != std::string::npos)
        data_type = "string";
      else if (buf == "false" || buf == "true")
        data_type = "bool";
      else if (buf == "null")
        data_type = "null";
      else if (buf.find('{') != std::string::npos)
        data_type = "object";
      else if (buf.find('[') != std::string::npos)
        data_type = "array";
      else if (is_number(buf))
        data_type = "number";
      else
        data_type = "invalid";

      if (data_type == "string") {
        tokens.push_back({TokenType::KeyValue, buf.substr(0, buf.size() - 1), data_type});
        buf.clear();
        tokens.push_back({TokenType::CloseQuote, "\"", "nv"});
      } else {
        tokens.push_back({TokenType::KeyValue, buf, data_type});
        buf.clear();
      }
    }
    if (stringified_json.at(i) == ',') {
      tokens.push_back({TokenType::Comma, ",", "nv"});
      continue;
    }
    if (stringified_json.at(i) == '}') {
      tokens.push_back({TokenType::CloseParen, "}", "nv"});
    }
  }

  return tokens;
}

int main(int argc, char *argv[]) {
  std::string file_contents;
  {
    std::ifstream json_file(argv[1], std::ios::binary);
    std::stringstream json_stream;

    json_stream << json_file.rdbuf();
    file_contents = json_stream.str();
  }

  std::string stringified_json = stringify(file_contents);
  std::cout << stringified_json << std::endl;

  for (auto &[type, value, data_type]: tokenize(stringified_json)) {
    if (type == TokenType::KeyName || type == TokenType::KeyValue) {
      std::cout << std::format("TokenType: {}, Value: {}, DataType: {}", enum_to_string(type), value, data_type)
                << std::endl;
    }
  }

  // std::vector<Token> tokens = tokenize(file_contents);
  // for (size_t i = 0; i < tokens.size(); ++i) {
  //   if ((tokens.at(i).type == TokenType::Comma && tokens.at(i + 1).type == TokenType::Colon) ||
  //       (tokens.at(i).type == TokenType::Comma && tokens.at(i + 1).type == TokenType::CloseParen)) {
  //     return EXIT_FAILURE;
  //   }
  // }

  return EXIT_SUCCESS;
}
