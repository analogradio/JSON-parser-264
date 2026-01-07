#ifndef __MINIUNIT_H__
#define __MINIUNIT_H__

#include <stdbool.h>
#include <string.h>
#include "clog.h"
#include <unistd.h> 
#include <stdio.h>

#define mu_start() int __mu_line_num_ff = 0

#define mu_check(condition)      \
	do{\
		if(((condition) == false) && ((__mu_line_num_ff) == false)) {\
			__mu_line_num_ff =  __LINE__;\
		}\
	}while(false)

#define mu_run(function) \
	do{\
		int muzzi = function();\
		if(muzzi == 0) {\
			logf_green("Test passed: %s\n", (#function));\
		}\
		else{ \
			logf_red("Test failed : %s at line %d\n", (#function),(muzzi));\
		}\
	}while(false)

#define mu_end() return __mu_line_num_ff

#define mu_check_strings_equal(argu1,argu2) mu_check(0==strcmp(argu1, argu2))

#endif
