// provides a cool macro to check the return values
#ifndef __CHECK_H__
#define __CHECK_H__

#include <stdio.h> // For fprintf, perror
#include <stdlib.h> // For exit
#include <string.h>
#include <errno.h>

#define CHECK(x) \
  do { \
    if (!(x)) { \
      fprintf(stderr, "shellter: %s: ", __FUNCTION__); \
	  if(errno==0) \
      errno=ECANCELED; \
    perror(#x); \
    } \
  } while (0)

#endif /* __CHECK_H__ */