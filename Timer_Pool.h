/****************************************************************************
 * Copyright (C) 2017 by Hsien-Wen Hu and Chaing-Chiu Ti                    *
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

#ifndef Timer_Pool_h
#define Timer_Pool_h
/*Library include*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ev.h"
#include "funclog.h"

#ifndef __FUNC__	
	#define __FUNC__ (char *)__FUNCTION__// get function name
#endif

/**
 * if you want to see the log or debug in th TIMER_Pool.c 
 * #define the TIMER_POOL_LOG_ON 
 */
//#define TIMER_POOL_LOG_ON
#ifdef TIMER_POOL_LOG_ON
	#define TIMER_log_debug(message, args...)  log_debug(message, ## args) 
	#define TIMER_log_info(message, args...)   log_info(message, ## args) 
	#define TIMER_log_warn(message, args...)   log_warn(message, ## args) 
	#define TIMER_log_error(message, args...)  log_error(message, ## args) 
	#define TIMER_log_fatal(message, args...)  log_fatal(message, ## args) 
#else
	#define TIMER_log_debug(message, args...) 
	#define TIMER_log_info(message, args...)
	#define TIMER_log_warn(message, args...)
	#define TIMER_log_error(message, args...)
	#define TIMER_log_fatal(message, args...)
#endif

/*Define timer commands*/
typedef enum{
	STOP_TIMER =1, //just stop the timer. 
	STOP_TIMER_AND_UNREGISTER, //Stop the timer and canel registation of it.
	REGIST_TIMER,//regist timer
	REGIST_TIMER_AND_START,//regist timer and start it
	START_TIMER,//start timer
	RESTART_TIMER, //restart timer
	STOP_ALL_TIMER, //stop specific eap_id's all timer
	UNREGISTER_ALL, //unregister specific eap_id's all timer_pool
	RE_REGISTER_TIMER_AND_START,
	REGIST_TIMER_AND_PERIODIC
}Timer_Command;


#ifndef _STATUS_CODE_
#define _STATUS_CODE_
	/*Function status code*/
	typedef enum{
		GOOD = 0, //No error, everything is good !
		NOT_REG = -1, //The watcher haven't registered.
		FAIL_REG = -2, //The watcher fail to do somthing.
		HAVED_REG =-3, //The watcher have already registered.
		FAIL_START = -4 //Fail to start
	}STATUS_CODE;
#endif

/*Timer pool struct*/
typedef struct _timer_pool{
	uint8_t id;//non zero for eap_id
	int time_stmp;
	ev_timer timer_ptr;
	struct _timer_pool *next;
}TIMER_POOL;

extern struct ev_loop *loop;//global define
typedef void (*Timer_Pool_CallBack)(struct ev_loop *loop, ev_timer *watcher, int revents);

/*Function definition*/
STATUS_CODE timer_pool(struct ev_loop *Loop,
				int timer_id,
				Timer_Command timer_command,
				int time_stmp,
				void (*call_back)(struct ev_loop *loop, ev_timer *watcher, int revents));

#define PERIODIC_TIMER(timer_type,time_stmp,CallBack) timer_pool((loop),(0),(timer_type),(REGIST_TIMER_AND_PERIODIC),(time_stmp),(CallBack)) 
void print_nodes();
void clear_timer_pool_node();


#endif