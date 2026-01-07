#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef __CLOG_H__
#define __CLOG_H__

#ifndef NDEBUG
#define logf_ printf
#else 
#define logf_(...)
#endif

#define __mu_logf_acolor(ansi_color_code, ...) \
	do{ \
		if (isatty(STDOUT_FILENO)){ \
			printf(ansi_color_code);   \
			logf_(__VA_ARGS__);   \
			printf("\x1b[0m");    \
		}\
		else { \
			logf_(__VA_ARGS__); \
		}\
	}while(false)

#define logf_red(...) __mu_logf_acolor("\x1b[31m", __VA_ARGS__)
#define logf_green(...) __mu_logf_acolor("\x1b[32m", __VA_ARGS__)
#define logf_yellow(...) __mu_logf_acolor("\x1b[33m", __VA_ARGS__)
#define logf_blue(...) __mu_logf_acolor("\x1b[34m", __VA_ARGS__)
#define logf_magenta(...) __mu_logf_acolor("\x1b[35m", __VA_ARGS__)
#define logf_cyan(...) __mu_logf_acolor("\x1b[36m", __VA_ARGS__)

#define log_int(varn) logf_("%s == %d\n", (#varn), (varn)) 
#define log_char(varn) logf_("%s =='%c'\n", (#varn),(varn)) 
#define log_str(varn) logf_("%s == \"%s\"\n", (#varn), (varn)) 
#define log_addr(varn) logf_("%s == %p\n", (#varn),(void*)varn) 
#define log_float(varn) logf_("%s == %.016f\n", (#varn), (varn))
#define log_bool(varn) logf_("%s == %s\n",(#varn), varn?"True":"False")

#endif /* end of include guard: __CLOG_H__ */
