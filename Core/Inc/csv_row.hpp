#pragma once

#include <cstddef>
#include <cstdio>

static char buf[512];

#define DEFINE_CSV_ROW_FIELD(type, name, fmt) type name;

#define DEFINE_CSV_ROW_HEADER_PROC(type, name, fmt)                            \
  {                                                                            \
    size_t len;                                                                \
    if (i++) {                                                                 \
      len = std::snprintf(nullptr, 0, ",%s", #name);                           \
      std::snprintf(buf + j, len + 1, ",%s", #name);                           \
    } else {                                                                   \
      len = std::snprintf(nullptr, 0, "%s", #name);                            \
      std::snprintf(buf + j, len + 1, "%s", #name);                            \
    }                                                                          \
    j += len;                                                                  \
  }

#define DEFINE_CSV_ROW_FORMAT_PROC(type, name, fmt)                            \
  {                                                                            \
    size_t len;                                                                \
    if (i++) {                                                                 \
      len = std::snprintf(nullptr, 0, "," fmt, name);                          \
      std::snprintf(buf + j, len + 1, "," fmt, name);                          \
    } else {                                                                   \
      len = std::snprintf(nullptr, 0, fmt, name);                              \
      std::snprintf(buf + j, len + 1, fmt, name);                              \
    }                                                                          \
    j += len;                                                                  \
  }

#define DEFINE_CSV_ROW(name, FIELDS)                                           \
  struct name {                                                                \
    FIELDS(DEFINE_CSV_ROW_FIELD)                                               \
    static const char *header() {                                              \
      size_t i = 0;                                                            \
      size_t j = 0;                                                            \
      FIELDS(DEFINE_CSV_ROW_HEADER_PROC)                                       \
      return buf;                                                              \
    }                                                                          \
    const char *format() const {                                               \
      size_t i = 0;                                                            \
      size_t j = 0;                                                            \
      FIELDS(DEFINE_CSV_ROW_FORMAT_PROC)                                       \
      return buf;                                                              \
    }                                                                          \
  };
