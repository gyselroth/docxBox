// Copyright (c) 2020 gyselroth GmbH

#ifndef DOCXBOX_HELPER_HELPER_DATETIME_H_
#define DOCXBOX_HELPER_HELPER_DATETIME_H_

#include <string>

namespace helper {
namespace DateTime {

static const char *const kFormatIso8601 = "%Y-%m-%dT%H:%M:%SZ";

std::string GetCurrentDateTimeFormatted(const char *format);

std::string GetCurrentDateTimeInIso8601();

}  // namespace DateTime
}  // namespace helper

#endif  // DOCXBOX_HELPER_HELPER_DATETIME_H_
