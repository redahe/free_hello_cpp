#include "system.hpp"

#include <string_view> // For std::string_view
#include <tuple>       // For std::ignore

constexpr std::string_view kMessage = "Hello World!\n";

int main() {
  std::ignore = _system::write(kMessage);
  return 0;
}
