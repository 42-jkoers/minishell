#ifndef JTESTER_H
# define JTESTER_H

// This can get overridden by the makefile to be the root directory
# ifndef COMPILE_PATH
#  define COMPILE_PATH __FILE__
# endif

# include "test_utils.h"
# include "pid_utils.h"
# include "catch_exceptions.h"
# include "run_program.h"
# include "path_utils.h"
# include "capture_fd.h"
# include "env_utils.h"

extern int NUM_OK;
extern int NUM_KO;
extern int NUM_WARN;
extern int NUM_SIG_OK;
extern int NUM_SIG_KO;

void JTester_init();
void JTester_flush();
void JTester_exit();

#endif