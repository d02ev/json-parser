#include <algorithm>
#include <format>
#include <iostream>
#include <vector>
#include "json_parser/utils.h"
#include "json_parser/parser.h"

std::string enum_to_string(const TokenType type) {
  switch (type) {
    case TokenType::OpenParenthesis:
      return "OpenParen";
    case TokenType::CloseParenthesis:
      return "CloseParen";
    case TokenType::OpenQuotes:
      return "OpenQuote";
    case TokenType::CloseQuotes:
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

int main(int argc, char *argv[]) {
  const std::string file_contents = Utils::cvt_file_stream_to_string(argv[1]);
  const std::string stringified_json = Utils::stringify(file_contents);
  const std::vector<Token> tokens = Parser::tokenize(file_contents);
  // std::cout << stringified_json << std::endl;
  //
  // for (auto &[type, value, data_type]: tokenize(stringified_json)) {
  //   if (type == TokenType::KeyName || type == TokenType::KeyValue) {
  //     std::cout << std::format("TokenType: {}, Value: {}, DataType: {}", enum_to_string(type), value, data_type)
  //               << std::endl;
  //   }
  // }

  for (size_t i = 0; i < tokens.size(); ++i) {
    if ((tokens.at(i).type == TokenType::Comma && tokens.at(i + 1).type == TokenType::Colon) ||
        (tokens.at(i).type == TokenType::Comma && tokens.at(i + 1).type == TokenType::CloseParenthesis)) {
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
