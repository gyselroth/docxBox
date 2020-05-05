// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#include <docxbox/helper/helper_numeric.h>

namespace helper {

bool Numeric::IsEven(int n) {
  return n % 2 == 0;
}

int Numeric::GetAmountDigits(int n) {
  return floor (log10(n)) + 1;
}

}  // namespace helper
