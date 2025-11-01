/* ev.h */

/* This work is dedicated to the public domain under CC0 1.0 Universal:
 * http://creativecommons.org/publicdomain/zero/1.0/
 * 
 * To the extent possible under law, Steven Ford has waived all copyright
 * and related or neighboring rights to this work. In other words, you can 
 * use this code for any purpose without any restrictions.
 * This work is published from: United States.
 * Project home: https://github.com/fordsfords/ev
 */

#ifndef EV_H
#define EV_H

#ifdef __cplusplus
extern "C" {
#endif

/* Portability definitions between Windows and Linux. */
#if defined(_WIN32)
  #include <windows.h>
  typedef __int8 int8_t;
  typedef __int16 int16_t;
  typedef __int32 int32_t;
  typedef __int64 int64_t;
  typedef unsigned __int8 uint8_t;
  typedef unsigned __int16 uint16_t;
  typedef unsigned __int32 uint32_t;
  typedef unsigned __int64 uint64_t;
  #define ev_atomic_inc32(ev_atomic_inc_i) InterlockedIncrement(ev_atomic_inc_i)
  #define ev_atomic_inc64(ev_atomic_inc_i) InterlockedIncrement64(ev_atomic_inc_i)
  #define ev_thread_id GetCurrentThreadId
#else
  #include <inttypes.h>
  #include <time.h>
  #include <pthread.h>
  #define ev_atomic_inc32(ev_atomic_inc_i) __sync_add_and_fetch(ev_atomic_inc_i, 1)
  #define ev_atomic_inc64(ev_atomic_inc_i) __sync_add_and_fetch(ev_atomic_inc_i, 1)
  #define ev_thread_id pthread_self
#endif


struct ev_record_s {
  uint64_t event_num;
  uint64_t data;
  uint64_t event_usec;
  uint64_t thread_id;
  uint64_t src_line;
  char *src_file;
};
typedef struct ev_record_s ev_record_t;

struct ev_s {
  uint32_t max_records;
  volatile uint32_t num_events;
  uint64_t flags;
  uint64_t create_usecs;
#if defined(_WIN32)
  uint64_t create_freq_div_1m;  /* Needed for Windows QueryPerformanceCounter(). */
#endif
  ev_record_t *records;
};
typedef struct ev_s ev_t;

/* Flag values. */
#define EV_FLAGS_PAUSE 0x0000000000000001


int ev_create(ev_t **rtn_ev, int max_records, uint64_t flags);
int ev_pause(ev_t *ev);
int ev_resume(ev_t *ev);
int ev_delete(ev_t *ev);
int ev_write(ev_t *ev, char *src_file, int src_line, uint64_t data);
uint64_t ev_get_time_usec(ev_t *ev);

#ifdef __cplusplus
}
#endif

#endif  /* EV_H */
