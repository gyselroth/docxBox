// Copyright (c) 2020 gyselroth GmbH
// Licensed under the MIT License - https://opensource.org/licenses/MIT

#ifndef DOCXBOX_HELPER_HELPER_NUMERIC_H_
#define DOCXBOX_HELPER_HELPER_NUMERIC_H_

#include <cstdio>
#include <cstdint>
#include <cmath>

namespace helper {
namespace Numeric {

bool IsEven(int number);

int GetAmountDigits(int n);

}  // namespace Numeric
}  // namespace helper

#endif  // DOCXBOX_HELPER_HELPER_NUMERIC_H_
