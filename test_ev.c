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
#include "ev.h"

/* Options */
int o_human = 0;  /* -H */
int o_date = 0;  /* -d */
unsigned long long o_delta_start = -1;  /* -D */
int o_newline_omit = 0;  /* -n */
int o_precision = 3;  /* -p */
unsigned long long o_test_end = -1;  /* -T */


char usage_str[] = "Usage: test_ev [-h] [-d] [-D start] [-H] [-n] [-p precision] [-T end] [message ...]";
void usage(char *msg) {
  if (msg) fprintf(stderr, "\n%s\n\n", msg);
  fprintf(stderr, "%s\n", usage_str);
  exit(1);
}  /* usage */

void help() {
  printf("%s\n"
    "where:\n"
    "  -h - print help\n"
    "  -H - human-readable output for delta mode\n"
    "  -d - include date\n"
    "  -D start - delta mode; print microseconds since 'start'\n"
    "  -n - newline omit from output\n"
    "  -p precision - number of decimal digits for seconds (0-6, default=3).\n"
    "  -T end - for testing purposes\n"
    "  message - zero or more text strings to be printed.\n"
    "For details, see https://github.com/fordsfords/techo\n",
    usage_str);
  exit(0);
}  /* help */


void parse_cmdline(int argc, char **argv)
{
  int opt;

  while ((opt = getopt(argc, argv, "hHdD:np:T:")) != EOF) {
    switch (opt) {
      case 'h':
        help();
        break;
      case 'H':
        o_human = 1;
        break;
      case 'd':
        o_date = 1;
        break;
      case 'D':
        SAFE_ATOI(optarg, o_delta_start);
        break;
      case 'n':
        o_newline_omit = 1;
        break;
      case 'p':
        SAFE_ATOI(optarg, o_precision);
        if (o_precision < 0 || o_precision > 6) usage("precision must be 0-6");
        break;
      case 'T':
        SAFE_ATOI(optarg, o_test_end);
        break;
      default:
        usage(NULL);
    }  /* switch */
  }  /* while */
}  /* parse_cmdline */


int main(int argc, char **argv)
{
  parse_cmdline(argc, argv);

  return 0;
}  /* main */
