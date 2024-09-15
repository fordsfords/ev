/* test_ev.c */
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
#include <unistd.h>
#include <pthread.h>
#endif
#include "ev.h"

#if defined(_WIN32)
#define MY_SLEEP_MS(ev_msleep_msecs) Sleep(ev_msleep_msecs)
#else
#define MY_SLEEP_MS(ev_msleep_msecs) usleep((ev_msleep_msecs)/1000)
#endif

#define E(e_test) do { \
  if ((e_test) == -1) { \
    fprintf(stderr, "ERROR [%s:%d]: '%s' returned -1\n", __FILE__, __LINE__, #e_test); \
    exit(1); \
  } \
} while (0)

#define ASSRT(assrt_cond) do { \
  if (! (assrt_cond)) { \
    fprintf(stderr, "ERROR [%s:%d]: assert '%s' failed\n", __FILE__, __LINE__, #assrt_cond); \
    exit(1); \
  } \
} while (0)


/* Options */
int o_testnum;


char usage_str[] = "Usage: test_ev [-h] [-t testnum]";
void usage(char *msg) {
  if (msg) fprintf(stderr, "\n%s\n\n", msg);
  fprintf(stderr, "%s\n", usage_str);
  exit(1);
}  /* usage */

void help() {
  printf("%s\n"
    "where:\n"
    "  -h - print help\n"
    "  -t testnum - Specify which test to run [1].\n"
    "For details, see https://github.com/fordsfords/ev\n",
    usage_str);
  exit(0);
}  /* help */


void parse_cmdline(int argc, char **argv) {
  int i;

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0) {
      help();  exit(0);

    } else if (strcmp(argv[i], "-t") == 0) {
      if ((i + 1) < argc) {
        i++;
        o_testnum = atoi(argv[i]);
      } else { fprintf(stderr, "Error, -t requires test number\n");  exit(1); }

    } else { fprintf(stderr, "Error, unknown option '%s'\n", argv[i]);  exit(1); }
  }  /* for i */
}  /* parse_cmdline */


void test1() {
  ev_t *ev;
  ev_record_t *rec;
  int i;
  uint64_t t1, t2, t_diff;

  E(ev_create(&ev, 5, 0x4000000000000000));
  ASSRT(ev->max_records == 5);
  ASSRT(ev->num_events == 0);
  ASSRT(ev->flags == 0x4000000000000000);

  t1 = ev_get_time_usec(ev);
  MY_SLEEP_MS(2);
  t2 = ev_get_time_usec(ev);
  t_diff = t2 - t1;
  ASSRT((t_diff > (2000+500)) || (t_diff < (2000 - 500)));
  t_diff = t2 - ev->create_usecs;
  ASSRT((t_diff > (2000+500)) || (t_diff < (2000 - 500)));

  E(ev_write(ev, __FILE__, __LINE__, 126));  i = __LINE__;
  ASSRT(ev->num_events == 1);

  rec = &(ev->records[0]);
  ASSRT(rec->event_num == 0);
  ASSRT(rec->data == 126);
  ASSRT(strcmp(rec->src_file, __FILE__) == 0);
  ASSRT(rec->src_line == i);
  ASSRT(rec->thread_id == ev_thread_id());
  t_diff = rec->event_usec - t1;
  ASSRT((t_diff > (2000+500)) || (t_diff < (2000 - 500)));

  for (i = 1; i < 5; i++) {
    E(ev_write(ev, __FILE__, __LINE__, i));
  }
  ASSRT(ev->num_events == 5);
  /* Make sure first record is still intact. */
  rec = &(ev->records[0]);
  ASSRT(rec->event_num == 0);
  ASSRT(rec->data == 126);
  /* Verify newest record valid. */
  rec = &(ev->records[4]);
  ASSRT(rec->event_num == 4);
  ASSRT(rec->data == 4);
  ASSRT(strcmp(rec->src_file, __FILE__) == 0);
  ASSRT(rec->thread_id == ev_thread_id());
  t_diff = rec->event_usec - t1;
  ASSRT((t_diff > (2000+500)) || (t_diff < (2000 - 500)));

  /* Test pause. */
  E(ev_pause(ev));
  ASSRT(ev->flags == (0x4000000000000000 | EV_FLAGS_PAUSE));
  E(ev_write(ev, __FILE__, __LINE__, 127));
  /* Make sure everything is the same. */
  ASSRT(ev->num_events == 5);
  rec = &(ev->records[0]);
  ASSRT(rec->event_num == 0);
  ASSRT(rec->data == 126);
  /* Verify newest record valid. */
  rec = &(ev->records[4]);
  ASSRT(rec->event_num == 4);
  ASSRT(rec->data == 4);
  ASSRT(strcmp(rec->src_file, __FILE__) == 0);
  ASSRT(rec->thread_id == ev_thread_id());
  t_diff = rec->event_usec - t1;
  ASSRT((t_diff > (2000+500)) || (t_diff < (2000 - 500)));

  E(ev_resume(ev));
  ASSRT(ev->flags == 0x4000000000000000);

  /* Wraparound back to 0. */
  E(ev_write(ev, __FILE__, __LINE__, 128));
  ASSRT(ev->num_events == 6);

  rec = &(ev->records[0]);
  ASSRT(rec->event_num == 5);
  ASSRT(rec->data == 128);
  ASSRT(strcmp(rec->src_file, __FILE__) == 0);
  ASSRT(rec->thread_id == ev_thread_id());
  t_diff = rec->event_usec - t1;
  ASSRT((t_diff > (2000+500)) || (t_diff < (2000 - 500)));

  E(ev_delete(ev));
}  /* test1 */


int main(int argc, char **argv) {
  parse_cmdline(argc, argv);

  if (o_testnum == 0 || o_testnum == 1) {
    test1();
    printf("test1: success\n");
  }

  return 0;
}  /* main */
