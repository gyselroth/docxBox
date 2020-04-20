// Copyright (c) 2020 gyselroth GmbH

#include <docxbox/helper/helper_dateTime.h>

namespace helper {

// @return dateTime in ISO 8601 format, e.g. "1994-11-05T13:15:30Z"
// @see ISO-8601 https://www.w3.org/TR/NOTE-datetime
std::string DateTime::GetCurrentDateTimeInIso8601() {
  // TODO(kay): render current dateTime
  return "2020-04-20T13:15:30Z";
}

}  // namespace helper
