/*
 * TThread.cpp
 *
 *  Created on: 01.07.2010
 *      Author: andrey
 */

#include "TThread.h"
#include <sched.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>



void	TThread::cancel_routine(void* params)
{
	((TThread*)params)->is_running = false;
	((TThread*)params)->OnCancel();
}



int	TThread::Run()
{
    return pthread_create( &thread, &attr, &start_routine, this );
 }


void	TThread::Kill(int sig)
{
	pthread_kill( thread, sig );
}


void	TThread::Cancel()
{
	if (thread != pthread_t(-1))
		pthread_cancel( thread );
	is_running = false;
}

void TThread::waitfor()
{
	if (thread!= pthread_t(-1))
		pthread_join( thread, 0 );
}


void	TThread::Suspend()
{

    if( is_running && priority() )
		old_prio = priority(0);
}


void	TThread::Resume()
{
    if( is_running && !priority() )
		old_prio = priority(old_prio);
}


int	TThread::priority(int prio)
{
	struct sched_param sparam;
	int p;

	if( is_running )
		pthread_getschedparam( thread, &p, &sparam );
	else
		pthread_attr_getschedparam(&attr, &sparam );

	int oldprio = sparam.sched_priority;
	sparam.sched_priority = prio;
	printf ("setpriority :: old = %d", oldprio);
	int res(0);
	if( is_running )
		res = pthread_setschedparam( thread, p, &sparam );
	else
		res = pthread_attr_setschedparam(&attr, &sparam );

	if (res) printf ("error = %d  %s",res, strerror(res));
	return oldprio;
}


int	TThread::priority()
{
	struct sched_param	sparam;
	int p;
	if( is_running )
		pthread_getschedparam( thread, &p, &sparam );
	else
		pthread_attr_getschedparam(&attr, &sparam );
	return sparam.sched_priority;
}


int	TThread::sched_policy(int policy)
{
	struct sched_param sparam;
	int p;

	if( is_running )
	{
		pthread_getschedparam( thread, &p, &sparam );
		pthread_setschedparam( thread, policy, &sparam );
	}
	else
	{
		pthread_attr_getschedpolicy(&attr, &p );
		pthread_attr_setschedpolicy(&attr, policy );
	}

	return p;
}


int	TThread::sched_policy()
{
	struct sched_param sparam;
	int p;

	if( is_running )
		pthread_getschedparam( thread, &p, &sparam );
	else
		pthread_attr_getschedpolicy(&attr, &p );

	return p;
}


TThread::TThread()
:	thread(-1),
	is_running(false),
	old_prio(0)
{
	pthread_attr_init( &attr );
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
}


TThread::TThread(const TThread& src)
:	thread(src.thread),
	is_running(src.is_running),
	attr(src.attr),
    old_prio(src.old_prio)
{
}


TThread::TThread(const pthread_attr_t&	th_attr)
:	thread(-1),
	is_running(false),
	attr( th_attr ),
	old_prio(0)
{
}



//##ModelId=41C84142011B
TThread::~TThread()
{
	if (is_running)  Cancel();
}

//##ModelId=41C8414200DB
void *	TThread::start_routine( void * params )
{
	void * res = 0;
	pthread_cleanup_push( cancel_routine, params );
	((TThread*)params)->is_running = true;
	res = ((TThread*)params)->Main();

	pthread_cleanup_pop( 1 );
	((TThread*)params)->is_running = false;
	return res;
}

