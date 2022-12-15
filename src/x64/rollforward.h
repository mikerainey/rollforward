#pragma once

#include <stdint.h>

struct rollforward_struct {
  void* from;
  void* to;
};

extern
uint64_t rollforward_table_size;
extern
uint64_t rollback_table_size;
extern
struct rollforward_struct rollforward_table[];
extern
struct rollforward_struct rollback_table[];
