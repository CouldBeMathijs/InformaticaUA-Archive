#include "General.h"
std::string encodeBase64(uint64_t value) {
  const std::string charset =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789:/";
  if (value == 0)
    return std::string(1, charset[0]);

  std::string result;
  while (value > 0) {
    result = charset[value % 64] + result;
    value /= 64;
  }
  return result;
}