#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "json_parser/cust_types.h"

class Parser {
  public:
    Parser() = default;
    ~Parser() = default;

    std::vector<Token> static tokenize(const std::string& stringified_json);
};

#endif //PARSER_H
