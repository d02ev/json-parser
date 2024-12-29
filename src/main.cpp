#include <format>
#include <fstream>
#include <iostream>
#include <optional>
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
};

std::string enum_to_string(TokenType type) {
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

std::vector<Token> tokenize(const std::string &file_content) {
  if (file_content.empty() || file_content.at(0) != '{') {
    exit(EXIT_FAILURE);
  }

  std::vector<Token> tokens;
  std::string buf;

  for (int i = 0; i < file_content.length(); ++i) {
    if (file_content.at(i) == '{') {
      tokens.push_back({TokenType::OpenParen, "{"});
      continue;
    }
    if (file_content.at(i) == '"' && (file_content.at(i - 1) == '{' || std::isspace(file_content.at(i - 1)))) {
      tokens.push_back({TokenType::OpenQuote, "\""});
      ++i;
      if (file_content.at(i) != ',' || file_content.at(i) != '}') {
        while (file_content.at(i) != '"') {
          buf.push_back(file_content.at(i++));
        }

        tokens.push_back({.type = TokenType::KeyName, .value = buf});
        buf.clear();
        tokens.push_back({TokenType::CloseQuote, "\""});
        ++i;
      }
    }
    if (file_content.at(i) == ':') {
      tokens.push_back({TokenType::Colon, ":"});
      continue;
    }
    if (file_content.at(i) == '"' && (file_content.at(i - 1) == ':' || std::isspace(file_content.at(i - 1)))) {
      tokens.push_back({TokenType::OpenQuote, "{"});
      ++i;
      if (file_content.at(i) != ',' || file_content.at(i) != '}') {
        while (file_content.at(i) != '"') {
          buf.push_back(file_content.at(i++));
        }

        tokens.push_back({TokenType::KeyValue, buf});
        buf.clear();
        tokens.push_back({TokenType::CloseQuote, "}"});
        ++i;
      }
    }
    if (file_content.at(i) == ',') {
      tokens.push_back({TokenType::Comma, ","});
      continue;
    }
    if (file_content.at(i) == '}') {
      tokens.push_back({TokenType::CloseParen, "}"});
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

  // int res = is_valid_empty_json(file_contents) ? EXIT_SUCCESS : EXIT_FAILURE;
  // return res;

  std::vector<Token> tokens = tokenize(file_contents);
  for (size_t i = 0; i < tokens.size(); ++i) {
    if ((tokens.at(i).type == TokenType::Comma && tokens.at(i + 1).type == TokenType::Colon) ||
        (tokens.at(i).type == TokenType::Comma && tokens.at(i + 1).type == TokenType::CloseParen)) {
      return EXIT_FAILURE;
    }
  }


  return EXIT_SUCCESS;
}
