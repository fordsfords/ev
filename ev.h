/* ev.h */
/*
# This code and its documentation is Copyright 2024-2024 Steven Ford, http://geeky-boy.com
# and licensed "public domain" style under Creative Commons "CC0": http://creativecommons.org/publicdomain/zero/1.0/
# To the extent possible under law, the contributors to this project have
# waived all copyright and related or neighboring rights to this work.
# In other words, you can use this code for any purpose without any
# restrictions.  This work is published from: United States.  The project home
# is https://github.com/fordsfords/ev
*/

#ifndef EV_H
#define EV_H

#ifdef __cplusplus
extern "C" {
#endif

/* Portability definitions between Windows and Linux. */
#if defined(_WIN32)
  typedef __int8 int8_t;
  typedef __int16 int16_t;
  typedef __int32 int32_t;
  typedef __int64 int64_t;
  typedef unsigned __int8 uint8_t;
  typedef unsigned __int16 uint16_t;
  typedef unsigned __int32 uint32_t;
  typedef unsigned __int64 uint64_t;

  pthread_self GetCurrentThreadId
#else
  #include <inttypes.h>
  #include <time.h>
#endif
  

struct ev_rec_s {
  uint64_t event_num;
  uint64_t data;
  uint64_t event_usec;
  char *src_file;
  uint32_t src_line;
  uint32_t thread_id;
};
typedef struct ev_rec_s ev_rec_t;

struct ev_s {
  int max_records;
  int num_records;
  uint64_t flags;
  uint64_t create_usecs;
#if defined(_WIN32)
  uint64_t create_freq;  /* Needed for Windows QueryPerformanceCounter(). */
#endif
  ev_rec_t *records;
};
typedef struct ev_s ev_t;


int ev_create(ev_t **rtn_ev, int max_records, uint64_t flags);
int ev_delete(ev_t *rtn_ev);
int ev_write(ev_t *rtn_ev, char *src_file, int src_line, uint64_t data);
uint64_t ev_get_time_usec(ev_t *ev);

#ifdef __cplusplus
}
#endif

#endif  /* EV_H */
