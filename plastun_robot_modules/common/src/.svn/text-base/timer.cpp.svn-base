#include "timer.h"


struct	itimerspec 	TTimer::pause_spec = { {0, 0}, {0, 0}};

//##ModelId=41C841550260
void TTimer::create() throw( std::logic_error )
{
	if(timer!= reinterpret_cast<timer_t>(-1)) return;
	if(timer_create( clock_id, &event, &timer) == -1)
	{
		timer = reinterpret_cast<timer_t>(-1);
		throw std::logic_error( "::create() exception" );
	}
}


//##ModelId=41C841550261
void TTimer::destroy()
{
	if (timer ==  reinterpret_cast<timer_t>(-1)) return;
	timer_delete( timer );
	timer =  reinterpret_cast<timer_t>(-1);
}


//##ModelId=41C841550266
void TTimer::set_start(int sec, int nsec)
{
	spec.it_value.tv_sec     = sec;
	spec.it_value.tv_nsec    = nsec;
}


//##ModelId=41C841550272
void TTimer::set_timeout(int sec, int nsec) throw( std::logic_error )
{
	spec.it_interval.tv_sec     = sec;
	spec.it_interval.tv_nsec    = nsec;
	if(fstarted) start();
}

//##ModelId=41C841550275
void TTimer::start() throw( std::logic_error )
{
	if(timer== reinterpret_cast<timer_t>(-1))
	    create();
	timer_settime( timer, 0, &spec, 0 );
	fstarted = true;
}

//##ModelId=41C841550276
void TTimer::stop() throw( std::logic_error )
{
	destroy();
	create();
	fstarted = false;
}

//##ModelId=41C841550277
void TTimer::pause()
{
	if(timer== reinterpret_cast<timer_t>(-1)) return;
	timer_settime( timer, 0, &pause_spec, 0 );
	fstarted = false;
}

//##ModelId=41C841550263
event_t TTimer::Event(const event_t&	evnt) throw( std::logic_error )
{
	event = evnt;
	if(timer != reinterpret_cast<timer_t> (-1))
	{
		bool was_started = fstarted;
		stop();
		if(was_started)
			start();
	}
	return event;
}

void TTimer::NONE() throw( std::logic_error )
{
	event.sigev_notify =SIGEV_NONE;
	Event(event);
}

void	TTimer::SIGNAL(int signo) throw( std::logic_error )
{
	event.sigev_notify= SIGEV_SIGNAL;
	event.sigev_signo = signo;
	sigemptyset(&sigset);
	sigaddset(&sigset,signo);
	Event(event);
}

//##ModelId=41C841550291
TTimer::TTimer( int ssec, int snsec, int sec, int nsec,
				const event_t&	evnt, clockid_t clk_id, bool suspend) throw( std::logic_error )
:	timer(reinterpret_cast<timer_t> (-1)),
	event(evnt),
	clock_id(clk_id),
	fstarted(false)
{
	set_start(ssec, snsec);
	set_timeout(sec, nsec);
	if(!suspend) start();
}

TTimer::TTimer( int ssec, int snsec, int sec, int nsec,
	int signo, clockid_t clk_id,bool suspend) throw( std::logic_error )
:	timer(reinterpret_cast<timer_t> (-1)),
	clock_id(clk_id),
	fstarted(false)
{
	SIGNAL(signo);
	set_start(ssec, snsec);
	set_timeout(sec, nsec);
	if(!suspend) start();
}


//##ModelId=41C8415502A1
TTimer::TTimer()
:	timer(reinterpret_cast<timer_t> (-1)),
	event(),
	clock_id(CLOCK_REALTIME),
	fstarted(false)
{
	set_start();
	set_timeout();
}

//##ModelId=41C8415502A2
TTimer::~TTimer()
{
	destroy();
}
