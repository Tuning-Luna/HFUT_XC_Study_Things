#ifndef _GETOPT_H_
#define _GETOPT_H_

#include <OpenMesh/Core/System/compiler.hh>

#if defined(WIN32)
#if   defined(__cplusplus)

extern "C" {

extern int opterr;
extern int optind;
extern int optopt;
extern int optreset;
extern char  *optarg;

extern int getopt(int nargc, char * const *nargv, const char *ostr);

}

#  endif
#else
#  include <getopt.h>
#endif

#endif /* _GETOPT_H_ */
