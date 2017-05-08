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

#include "funclog.h"
#include <assert.h>
#include <stdarg.h>
#include <syslog.h>

void funclog (const char *format,... )
{	
	openlog(PROGRAM_NAME, LOG_CONS | LOG_PID, LOG_LOCAL1);
	
	if(format==NULL){
		return;
	}

	va_list args;
	memset(&args,0,sizeof(va_list));
	
	va_start(args, format);

	if(CAPCAMfunclog_CODE == CONSOLE){
		vprintf(format, args);//print information on console
	}else if(CAPCAMfunclog_CODE == SYSLOG){
		vsyslog(LOG_NOTICE, format, args);//print information on log file
	}else if(CAPCAMfunclog_CODE == ALL_RECORD){
		vsyslog(LOG_NOTICE, format, args);//print information on log file
		vprintf(format, args);//print information on console
	}

	va_end(args);
    closelog();
}
