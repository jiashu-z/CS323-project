#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>

/**
 * @brief Token class for lexical analysis. Tokens are immutable.
 * 
 */
class Token {
 public:
  /**
   * @brief Types of tokens.
   * 
   */
  enum Type {
    reserved,
    identifier,
    op
  };

  /**
   * @brief Names of Type. The sequence must be consistent with enum instances in Type class.
   * 
   */
  const static std::string type_names[];

 private:
  static std::string TypeToString(const Token::Type& token_type);

 private:
  const Type token_type;
  const std::string literal;

 public:
  Token(const Token::Type& token_type, const std::string& literal);

  ~Token();

  std::string ToString() const;

  Type GetType() const;

  std::string GetLiteral() const;
};

#endif
