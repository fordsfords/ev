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

struct ev_rec_s {
  int event_num;
  char *src_file;
  int src_line;
};
typedef struct ev_rec_s ev_rec_t;

struct ev_s {
  int max_records;
  int num_records;
  ev_rec_t *ev_recs;
};
typedef struct ev_s ev_t;

#ifdef __cplusplus
}
#endif

#endif  /* EV_H */
