/*
Yash Bhutwala
CS 315 Lab 10
11/10/2015
 */

#include <stdlib.h>
#include <stdio.h>
#include "wrappers.h"

void handle_err(int err, int line) {
  if (err < 0) {
    char msg[128];
    sprintf(msg, "System Error Line %d:", line);
    perror(msg);
    exit(err);
  }
}