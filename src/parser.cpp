#include <vector>
#include "json_parser/utils.h"
#include "json_parser/parser.h"
#include "json_parser/cust_types.h"

std::vector<Token> Parser::tokenize(const std::string &stringified_json) {
  if (stringified_json.empty() || stringified_json.at(0) != '{' || stringified_json.find('\'') != std::string::npos) {
    exit(EXIT_FAILURE);
  }

  std::vector<Token> tokens;
  std::string buf;

  for (int i = 0; i < stringified_json.length(); ++i) {
    if (stringified_json.at(i) == '{') {
      tokens.push_back({constants::TokenType::OpenParenthesis, "{", "nv"});
      continue;
    }
    if (stringified_json.at(i) == '"') {
      tokens.push_back({constants::TokenType::OpenQuotes, "\"", "nv"});
      ++i;

      if (i < stringified_json.length() && (stringified_json.at(i - 2) == '{' || stringified_json.at(i - 2) == ',')) {
        while (stringified_json.at(i) != '"') {
          buf.push_back(stringified_json.at(i++));
        }

        tokens.push_back({constants::TokenType::KeyName, buf, "string"});
        buf.clear();
        tokens.push_back({constants::TokenType::CloseQuotes, "\"", "nv"});
      }
    }
    if (stringified_json.at(i) == ':') {
      tokens.push_back({constants::TokenType::Colon, ":", "nv"});
      ++i;

      if (stringified_json.at(i) == '"') {
        tokens.push_back({constants::TokenType::OpenQuotes, "\"", "nv"});
        ++i;
        while (i < stringified_json.length() && stringified_json.at(i) != '"') {
          buf.push_back(stringified_json.at(i++));
        }
        buf.push_back(stringified_json.at(i));
      } else if (stringified_json.at(i) == '{') {
        while (i < stringified_json.length() && stringified_json.at(i) != '}') {
          buf.push_back(stringified_json.at(i++));
          if (stringified_json.at(i) == '}') {
            buf.push_back(stringified_json.at(i));
          }
        }
      } else if (stringified_json.at(i) == '[') {
        while (i < stringified_json.length() && stringified_json.at(i) != ']') {
          buf.push_back(stringified_json.at(i++));
          if (stringified_json.at(i) == ']') {
            buf.push_back(stringified_json.at(i));
          }
        }
      } else if (stringified_json.at(i) == '\'') {
        exit(EXIT_FAILURE);
      } else {
        while (stringified_json.at(i) != ',' || stringified_json.at(i) != '}') {
          if (stringified_json.at(i) == ',' || stringified_json.at(i) == '}')
            break;
          buf.push_back(stringified_json.at(i++));
        }
      }

      std::string data_type;
      if (buf.find('{') != std::string::npos)
        data_type = "object";
      else if (buf.find('[') != std::string::npos)
        data_type = "array";
      else if (buf.find('"') != std::string::npos)
        data_type = "string";
      else if (buf == "false" || buf == "true")
        data_type = "boolean";
      else if (buf == "null")
        data_type = "null";
      else if (Utils::is_number(buf))
        data_type = "number";
      else
        data_type = "invalid";

      if (data_type == "string") {
        tokens.push_back({constants::TokenType::KeyValue, buf.substr(0, buf.size() - 1), data_type});
        buf.clear();
        tokens.push_back({constants::TokenType::CloseQuotes, "\"", "nv"});
      } else {
        tokens.push_back({constants::TokenType::KeyValue, buf, data_type});
        buf.clear();
      }
    }
    if (stringified_json.at(i) == ',') {
      tokens.push_back({constants::TokenType::Comma, ",", "nv"});
      continue;
    }
    if (stringified_json.at(i) == '}') {
      tokens.push_back({constants::TokenType::CloseParenthesis, "}", "nv"});
    }
  }

  return tokens;
}
