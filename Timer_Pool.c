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

#include "Timer_Pool.h"
#include <assert.h>
#define	FREE_TIMER(obj_name) {if(obj_name){free((obj_name)); (obj_name) = NULL;}}
static TIMER_POOL *timer_pool_head = NULL;//global define
static TIMER_POOL *timer_pool_tail = NULL;//global define

struct ev_loop *loop = NULL;// global definition and let it as NULL. 

ev_timer *Search(TIMER_POOL *head, int id){
	TIMER_log_info("Enter function [%s]",__FUNC__);
	TIMER_POOL *p=NULL;
	p=head;
	TIMER_log_debug("seach timert id:%d",id);
	while(p!=NULL){
		TIMER_log_debug("id:%d",p->id);
		if(p->id==id)
			break;
		
		p=p->next;
	}

	if(p!=NULL){
		TIMER_log_info("Find node !");
		TIMER_log_debug("find id:%d",p->id);
		TIMER_log_info("Exit function [%s]",__FUNC__);
		return &(p->timer_ptr);
	}


	TIMER_log_info("no this node !!!");
	TIMER_log_info("Exit function [%s]",__FUNC__);
	return NULL;
}


void delete_node_by_id(TIMER_POOL **head,TIMER_POOL **tail, int id){
	TIMER_log_info("Enter function [%s]",__FUNC__);
	if(head==NULL||tail==NULL){
		TIMER_log_warn("input pointer is NULL");
		return;
	}
	TIMER_log_debug("Delete id:%d",id);
	TIMER_POOL *current=NULL,*previous=NULL;
	current = (*head);

	while(current !=NULL){
		if(current->id==id){
			if(current == (*head)){
				(*head) = current->next;
				FREE_TIMER(current);
				current =(*head);
			}else{
				previous->next = current->next;
				FREE_TIMER(current);
				current =previous->next;
			}
		}else{
			previous =current;
			current =current->next;
		}
		
	}

}

void clear_timer_pool_node(){
	TIMER_log_info("Enter function [%s]",__FUNC__);
	TIMER_POOL *p=NULL,*q=NULL;
	p=timer_pool_head;
	while(p!=NULL){
		q=p;
		ev_timer *target=NULL;
		target = &(p->timer_ptr);
		ev_timer_stop(loop,target);/*stop  timer*/ 
		p=p->next;
		FREE_TIMER(q);
	}
	timer_pool_head=NULL;
	TIMER_log_info("Exit function [%s]",__FUNC__);
}

STATUS_CODE delete_node(TIMER_POOL **head,TIMER_POOL **tail, int id){
	TIMER_log_info("Enter function [%s]",__FUNC__);
	if(head==NULL||tail==NULL){
		TIMER_log_warn("input pointer is NULL");
		return NOT_REG;
	}
	TIMER_POOL *current=NULL,*previous=NULL;
	current = (*head);

	while(current!=NULL){
		if(current->id==id)
			break;
		previous = current;
		current = current->next;
	}


	if(current ==NULL){
		TIMER_log_warn("Can't find  timer :%d",id);
		return NOT_REG;
	}else if(current==(*head)){
		TIMER_log_debug("Free! head, timer ID = %d",id);
		(*head) = current->next;
		/*if(current==(*tail)){
			(*tail) =  NULL;
			
		}*/
		FREE_TIMER(current);
		return GOOD;

	}else{
		TIMER_log_debug("Free! inter, timer ID = %d",id);
		previous->next = current->next;
		/*if(current==(*tail)){
			(*tail) =  previous;
		}*/
		FREE_TIMER(current);
		return GOOD;
	}

	TIMER_log_info("Exit function [%s]",__FUNC__);
	return NOT_REG;
}

STATUS_CODE add_new_node(TIMER_POOL **head, TIMER_POOL **tail, int id, int time_stmp){
	TIMER_log_info("Enter function [%s]",__FUNC__);
	ev_timer *target = Search(timer_pool_head,id);
	if(target!=NULL){
		TIMER_log_warn("The id %d has already  register",id);
		return HAVED_REG;
	}
	TIMER_POOL *p=NULL,*q=NULL;

	TIMER_POOL *new_node = calloc(1,sizeof(TIMER_POOL));

	if(new_node==NULL){
		log_warn("allocate memory failed");
		return FAIL_REG;
	}
	new_node->id = id;
	new_node->time_stmp = time_stmp;
	new_node->next = NULL;
	new_node->timer_ptr.data = (void *)new_node;
	TIMER_log_debug("Add node , timer ID = %d",new_node->id);
	p=(*head);
	if(p!=NULL){
		q=p;
		p=p->next;
	}
	new_node->next=p;
	if(p==(*head)){
		(*head)=new_node;
		(*head)->next =NULL;

	}else{
		q->next=new_node;
	}

	TIMER_log_info("Exit function [%s]",__FUNC__);
	return GOOD;
}

void print_nodes(){
	TIMER_log_info("[%s]",__FUNC__);
	TIMER_POOL *head =timer_pool_head;
	while(head != NULL){
		log_debug("ID = %d\t",head->id);
		log_debug("timer timestamp:%d\t",head->time_stmp);
		log_debug("timer remaing time:%f",ev_timer_remaining(loop,&head->timer_ptr));
		if(ev_is_active( &head->timer_ptr)){
			TIMER_log_debug("timer is active\t");
		}else{
			TIMER_log_debug("timer is deactive\t");
		}

		TIMER_log_debug("timer = %p\n",&(head->timer_ptr));

		//CAPCAMfuncTIMER_log("stmp = %d\n",head->time_stmp);
		//CAPCAMfuncTIMER_log("timer watcher = %p\n",head->timer_ptr);
		head = head->next;
	}
	TIMER_log_info("Exit function [%s]",__FUNC__);
}


STATUS_CODE timer_pool_stop_timer(uint8_t id){
	TIMER_log_info("[%s]",__FUNC__);
	ev_timer *target = Search(timer_pool_head,id);
	TIMER_log_debug("stop id:%d",id);
	if(target == NULL){
		TIMER_log_warn("Fail to stop timer, no This timer in timer poll");
		return NOT_REG;
	}else{
		ev_timer_stop(loop,target);
		return GOOD;
	}
}
STATUS_CODE timer_pool_unregister_timer(uint8_t id){
	TIMER_log_info("[%s]",__FUNC__);
	ev_timer *target = Search(timer_pool_head,id);
	TIMER_log_debug("unregister id:%d",id);
	STATUS_CODE ret =0;
	if(target == NULL){
		TIMER_log_warn("Fail to stop timer, no This timer in timer poll");
		return NOT_REG;

	}
	ev_timer_stop(loop,target);//Find proper ev_timer and stop it.
	if((ret = delete_node(&timer_pool_head,&timer_pool_tail,id)) != GOOD){
		TIMER_log_error("Fail to delete node");
		return NOT_REG;
	}	
	return GOOD;
}
STATUS_CODE timer_pool_register_timer(uint8_t id,int time_stmp,Timer_Pool_CallBack call_back){
	TIMER_log_info("[%s]",__FUNC__);
	if(call_back==NULL){
		log_error("call_back is NULL");
		return FAIL_REG;
	}

	STATUS_CODE ans = add_new_node(&timer_pool_head,&timer_pool_tail,id,time_stmp);
		
	if(ans != GOOD){
		TIMER_log_error("Fail to add a new node");
	}else{
		/*initial and setup timer*/
		ev_timer *target = Search(timer_pool_head,id);
		if(target!=NULL){
			ev_timer_init(target,call_back,time_stmp,0.);
		}
	}
	return GOOD;
}

STATUS_CODE timer_pool_register_periodic_timer( uint8_t id, int time_stmp,Timer_Pool_CallBack call_back){
	TIMER_log_info("[%s]",__FUNC__);
	if(call_back==NULL){
		log_error("call_back is NULL");
		return FAIL_REG;
	}
	STATUS_CODE ans = add_new_node(&timer_pool_head,&timer_pool_tail,id,time_stmp);
	if(ans != GOOD){
		TIMER_log_error("Fail to add a new node");
	}else{
		/*initial and setup timer*/
		ev_timer *target = Search(timer_pool_head,id);
		if(target!=NULL){
			ev_timer_init(target,call_back,time_stmp,time_stmp);
		}
		
	}
	return GOOD;
}

STATUS_CODE timer_pool_start_timer(uint8_t id){
	TIMER_log_info("[%s]",__FUNC__);
	ev_timer *target = Search(timer_pool_head,id);
	if(loop==NULL){
		TIMER_log_error("libev loop is null");
		return FAIL_REG;
	}
	if(target == NULL){
		TIMER_log_warn("Fail to start timer, no This timer in timer poll");
		return NOT_REG;
	}else{
		if(!ev_is_active(target)){
			ev_timer_start(loop,target);
			return GOOD;
		}else{
			TIMER_log_warn("Timer id:%d already active",id);
			return FAIL_START;
		}
	}
}

STATUS_CODE timer_pool_restart_timer(uint8_t id, int time_stmp){
	TIMER_log_info("[%s]",__FUNC__);
	ev_timer *target = Search(timer_pool_head,id);

	if(target == NULL){
		TIMER_log_warn("Fail to start timer, no This timer in timer poll");
		return NOT_REG;
	}else{
		ev_timer_stop(loop,target);
		target->repeat = time_stmp;
		TIMER_POOL *timer_stuct =(TIMER_POOL *) target->data;
		if(timer_stuct!=NULL){
			timer_stuct->time_stmp=time_stmp;
		}
		ev_timer_again(loop,target);
		return GOOD;
	}
}

STATUS_CODE timer_pool_stop_all_timer(void){
	TIMER_log_info("[%s]",__FUNC__);
	TIMER_POOL *head =timer_pool_head;
	while(head != NULL){
		ev_timer_stop(loop, &head->timer_ptr);
		head = head->next;
	}
}

STATUS_CODE timer_pool_unregister_all(uint8_t id){
	delete_node_by_id(&timer_pool_head,&timer_pool_tail, id);
}

STATUS_CODE timer_pool(struct ev_loop *Loop,
				int timer_id,
				Timer_Command timer_command,
				int time_stmp,
				void (*call_back)(struct ev_loop *loop, ev_timer *watcher, int revents))
{
	

	TIMER_log_info("Enter function [%s]",__FUNC__);
	assert(time_stmp>=0);
	TIMER_log_debug("The id %d",timer_id);

	switch(timer_command){
		case STOP_TIMER:{
			TIMER_log_info("STOP_TIMER");
			timer_pool_stop_timer(timer_id);
			break;
		}
		case STOP_TIMER_AND_UNREGISTER:{
			TIMER_log_info("STOP_TIMER_and_Unregister");
			timer_pool_stop_timer(timer_id);
			timer_pool_unregister_timer(timer_id);
			break;
		}
		case REGIST_TIMER:{
			TIMER_log_info("REGIST_TIMER");
			/*add timer info to timer list*/
			timer_pool_register_timer( timer_id, time_stmp,call_back);
			break;
		}
		case REGIST_TIMER_AND_START:{
			TIMER_log_info("REGIST_TIMER_AND_START");
			/*add timer info to timer list*/
			timer_pool_register_timer( timer_id, time_stmp,call_back);		
			timer_pool_start_timer(timer_id);
			break;
		}
		case START_TIMER:{
			TIMER_log_info("START_TIMER");
			timer_pool_start_timer(timer_id);
			break;
		}
		case RESTART_TIMER:{
			TIMER_log_info("RESTART_TIMER");
			timer_pool_restart_timer(timer_id, time_stmp);
			break;
		}
		case STOP_ALL_TIMER:{
			TIMER_log_info("STOP_ALL_TIMER");
			timer_pool_stop_all_timer();
			break;
		}
		case UNREGISTER_ALL:{
			TIMER_log_info("UNREGISTER_ALL");
			timer_pool_stop_all_timer();
			timer_pool_unregister_all(timer_id);
			break;
		}
		case RE_REGISTER_TIMER_AND_START:{
			TIMER_log_info("RE_REGISTER_TIMER_AND_START");
			timer_pool_stop_timer(timer_id);
			timer_pool_unregister_timer(timer_id);
			timer_pool_register_timer(timer_id,time_stmp,call_back);
			timer_pool_start_timer(timer_id);

			break;
		}
		case REGIST_TIMER_AND_PERIODIC:{
			TIMER_log_info("REGIST_TIMER_AND_PERIODIC");
			timer_pool_register_periodic_timer(timer_id,time_stmp,call_back);
			timer_pool_start_timer(timer_id);
			break;
		}
		default:{
			TIMER_log_warn("Error timer_command, timer ID = %d,timer command = %d",timer_id,timer_command);
			break;
		}
	}
	TIMER_log_info("Exit function [%s]",__FUNC__);

}
