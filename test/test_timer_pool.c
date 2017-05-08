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

#include <stdio.h>
#include <stdlib.h>
#include "Timer_Pool.h"

static void callBack2(struct ev_loop *loop, ev_timer *watcher, int revents){
	printf("yoyoman\n");
}

static void callBack1(struct ev_loop *loop, ev_timer *watcher, int revents){
	printf("yoyohaha\n");
	//timer_pool(loop,7,STOP_TIMER_AND_UNREGISTER,5,callBack2);
	timer_pool(loop,5,STOP_ALL_TIMER,2,callBack1);
}


int main(){
	loop = ev_loop_new(EVFLAG_AUTO);

	timer_pool(loop,5,REGIST_TIMER_AND_START,2,callBack1);
	timer_pool(loop,7,REGIST_TIMER_AND_START,5,callBack2);

	while(1){
		ev_run(loop,EVRUN_NOWAIT);
	}
}