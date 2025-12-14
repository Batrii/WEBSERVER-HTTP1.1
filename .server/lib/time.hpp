#pragma once
#include <sys/time.h>

class time {
  public:
    static long long clock(void) throw() {
      struct timeval tv;
      gettimeofday(&tv, 0);
      return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
    }
    static long long calc(long long start, long long end) throw() {
      return end - start;
    }
};