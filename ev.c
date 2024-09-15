/* ev.c */
/*
# This code and its documentation is Copyright 2024-2024 Steven Ford, http://geeky-boy.com
# and licensed "public domain" style under Creative Commons "CC0": http://creativecommons.org/publicdomain/zero/1.0/
# To the extent possible under law, the contributors to this project have
# waived all copyright and related or neighboring rights to this work.
# In other words, you can use this code for any purpose without any
# restrictions.  This work is published from: United States.  The project home
# is https://github.com/fordsfords/ev
*/

#include <stdio.h>
#include <string.h>
#if ! defined(_WIN32)
#include <stdlib.h>
#include <pthread.h>
#endif
#include "ev.h"


uint64_t ev_get_time_usec(ev_t *ev)
{
  uint64_t usecs;
#if defined(_WIN32)
  LARGE_INTEGER ticks;
  QueryPerformanceCounter(&ticks);
  usecs = ticks.QuadPart / ev->create_freq_div_1m;
#else
  struct timespec ticks;
  clock_gettime(CLOCK_MONOTONIC, &ticks);
  usecs = (ticks.tv_sec * 1000000LL) + (ticks.tv_nsec / 1000);
#endif
  return usecs;
}  /* ev_get_time_usec */


int ev_create(ev_t **rtn_ev, int max_records, uint64_t flags)
{
  ev_t *ev;

  ev = malloc(sizeof(ev_t));  if (ev == NULL) { return -1; }
  ev->records = malloc(sizeof(ev_record_t) * max_records);  if (ev->records == NULL) { free(ev); return -1; }
  ev->max_records = max_records;
  ev->flags = flags;
  ev->num_events = 0;
#if defined(_WIN32)
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  ev->create_freq_div_1m = freq.QuadPart / 1000000LL;
#endif
  ev->create_usecs = ev_get_time_usec(ev);

  *rtn_ev = ev;
  return 0;
}  /* ev_create */


int ev_delete(ev_t *ev) {
  free(ev->records);
  free(ev);
  return 0;
}  /* ev_delete */


int ev_pause(ev_t *ev) {
  ev->flags |= EV_FLAGS_PAUSE;
  return 0;
}  /* ev_pause */


int ev_resume(ev_t *ev) {
  ev->flags &= ~EV_FLAGS_PAUSE;
  return 0;
}  /* ev_resume */


int ev_write(ev_t *ev, char *src_file, int src_line, uint64_t data) {
  if (ev->flags & EV_FLAGS_PAUSE) {
    return 0;
  }
  uint32_t num_events = ev_atomic_inc32(&ev->num_events);
  uint32_t rec_index = (num_events - 1) % ev->max_records;
  ev_record_t *rec = &(ev->records[rec_index]);

  rec->event_num = num_events - 1;
  rec->data = data;
  rec->event_usec = ev_get_time_usec(ev);
  rec->src_file = src_file;
  rec->src_line = src_line;
  rec->thread_id = ev_thread_id();

  return 0;
}  /* ev_write */
