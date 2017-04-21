/*
Yash Bhutwala
CS 315 Lab 10
11/10/2015
 */

#ifndef WRAPPERS_H_   /* Include guard */
#define WRAPPERS_H_

void handle_err(int err, int line);

#define CHECK(err)    (handle_err(err, __LINE__))

#endif
