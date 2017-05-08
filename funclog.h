/****************************************************************************
 * Copyright (C) 2017 by Chia-Ray Lin    				                    *
 *                                                                          *
 *                                                                          *
 *   This is a free software: you can redistribute it and/or modify it      *
 *   under the terms of the GNU Lesser General Public License as published  *
 *   by the Free Software Foundation, either version 3 of the License, or   *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Box.  If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

#ifndef _CAPCAM_FUNCTION_LOG_H_
#define _CAPCAM_FUNCTION_LOG_H_
#include <time.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <syslog.h>


void funclog (const char *format,... );

#define __FUNC__	(char *)__FUNCTION__// get function name	

/*
 * get program name
 */
extern char *__progname;
#define PROGRAM_NAME __progname 

#define PRINTFUNCTION(format, ...) funclog(format, __VA_ARGS__)

typedef enum{
	CONSOLE = 0,//0 = print on console only
	SYSLOG = 1,//1 = system log only
	ALL_RECORD = 2//2 = print on console and system log
}log_code;

#define CAPCAMfunclog_CODE CONSOLE	

#define _FILE strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__
#define NO_LOG          0x00
#define FATAL_LEVEL		0x01
#define ERROR_LEVEL     0x02
#define WARN_LEVEL		0x03
#define INFO_LEVEL      0x04
#define DEBUG_LEVEL     0x05

#ifndef LOG_LEVEL
#define LOG_LEVEL   DEBUG_LEVEL
#endif
#define NEWLINE     "\n"

#define FATAL_TAG	"FATAL"
#define ERROR_TAG   "ERROR"
#define INFO_TAG    "INFO"
#define WARN_TAG	"WARN"
#define DEBUG_TAG   "DEBUG"
#define LOG_FMT             "[%-5s] %-15s:[%d]:%s: "
#define LOG_ARGS(LOG_TAG)   LOG_TAG, _FILE,__LINE__ , __FUNCTION__

#if LOG_LEVEL >= DEBUG_LEVEL
#define log_debug(message, args...)     PRINTFUNCTION(LOG_FMT message NEWLINE, LOG_ARGS(DEBUG_TAG), ## args)
#else
#define log_debug(message, args...)
#endif

#if LOG_LEVEL >= INFO_LEVEL
#define log_info(message, args...)      PRINTFUNCTION(LOG_FMT message NEWLINE, LOG_ARGS(INFO_TAG), ## args)
#else
#define log_info(message, args...)
#endif


#if LOG_LEVEL >= WARN_LEVEL
#define log_warn(message, args...)      PRINTFUNCTION(LOG_FMT message NEWLINE, LOG_ARGS(WARN_TAG), ## args)
#else
#define log_warn(message, args...)
#endif

#if LOG_LEVEL >= ERROR_LEVEL
#define log_error(message, args...)     PRINTFUNCTION(LOG_FMT message NEWLINE, LOG_ARGS(ERROR_TAG), ## args)
#else
#define log_error(message, args...)
#endif

#if LOG_LEVEL >= FATAL_LEVEL
#define log_fatal(message, args...) 	    PRINTFUNCTION(LOG_FMT message NEWLINE, LOG_ARGS(ERROR_TAG), ## args)
#else
#define log_fatal(message, args...)
#endif


#endif
