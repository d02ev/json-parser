#ifndef CUST_TYPES_H
#define CUST_TYPES_H

#include <string>
#include "json_parser/constants.h"

struct Token {
  constants::TokenType type;
  std::string value;
  std::string data_type;
};

#endif //CUST_TYPES_H
