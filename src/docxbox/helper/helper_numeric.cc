
#include <cstdio>
#include <cstdint>
#include "helper_numeric.h"

namespace helper {

bool Numeric::IsEven(unsigned long n) {
  return n % 2 == 0;
}

} // namespace helper