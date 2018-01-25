// provides a cool macro to check the return values
#ifndef __CHECK_H__
#define __CHECK_H__

#include <stdio.h> // For fprintf, perror
#include <stdlib.h> // For exit
#include <errno.h>

#define CHECK(x) \
  do { \
    if (!(x)) { \
      fprintf(stderr, "\"%s\":%s:%d: ", __FILE__, __func__, __LINE__); \
	  if(errno==0) errno=ECANCELED; \
      perror(#x); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)

#endif /* __CHECK_H__ */