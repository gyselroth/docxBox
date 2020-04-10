// Copyright (c) 2020 gyselroth GmbH

#include <cstdio>
#include <cstdint>
#include "helper_numeric.h"

namespace helper {

bool Numeric::IsEven(int n) {
  return n % 2 == 0;
}

}  // namespace helper
