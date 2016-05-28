#ifndef _TIMER_HPP_INCLUDED
	#define _TIMER_HPP_INCLUDED

#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdexcept>
/*************************************************************************
 * timer.h created at 27.01.2005 19:17:08
 * Object interface to the QNX 6.3.0 system timer API
 * This is a part of "QNX Classes" project
 * (c) by Sergei Khandrikov 2003-2005
 *
 ************************************************************************/

#ifndef EVENT_T_DEFINED
	#define EVENT_T_DEFINED
//##ModelId=41C84024032C
typedef	struct sigevent	event_t;
#endif //EVENT_T_DEFINED

// Timer class
//##ModelId=41C8415501B6
class	TTimer
{
protected:
	// system timer id
	timer_t				timer;
	
	// timer specification
	struct	itimerspec 	spec;
	
	//timer specification to realize pause() method
	static struct	itimerspec 	pause_spec; 
	
	// timer event
	//##ModelId=41C841550245
    event_t				event;
    
    //signal set
    sigset_t 			sigset;
    
    // clock_id
    clockid_t 			clock_id;
    
    // flag of timer is started
	bool				fstarted;
	
	// timer is started flag
	//##ModelId=41C841550260
	void				create() throw( std::logic_error );
	
	// destroy current timer
	//##ModelId=41C841550261
	void				destroy();
public:
	// returns timer id
	//##ModelId=41C841550262
	timer_t			id(){return timer;};

	//Sets the event type
	//##ModelId=41C841550263
	event_t 		Event(const event_t& evnt) throw( std::logic_error );
	
	//Get the event type value
	//##ModelId=41C841550265
	const event_t *		Event(){ return &event;};
	
	const sigset_t *	Sigset(){ return &sigset;};
	
	//setting up start time   <seconds>  + <nanoseconds>
	//##ModelId=41C841550266
	void		set_start(int sec = 0, int nsec = 0);
	
	//setting up timeout        <seconds>  + <nanoseconds>
	//##ModelId=41C841550272
	void		set_timeout(int sec = 0, int nsec = 0)throw( std::logic_error );
	
	//starts ore resumes the timer
	//##ModelId=41C841550275
	void		start() throw( std::logic_error );
	
	//stops the timer
	//##ModelId=41C841550276
	void		stop() throw( std::logic_error );
	
	//pauses the timer
	//##ModelId=41C841550277
	void		pause();
	
	//	No event
	//##ModelId=41C841550280
	void		NONE() throw( std::logic_error ) ;
	

	//	Send a signal to a process. The following fields are used: 
	//	int signo			The signal to raise. 
	//						This must be in the range from 1 through NSIG - 1. 
	//##ModelId=41C841550286
	void		SIGNAL(int signo) throw( std::logic_error );
	
	//wait for signal from timer
	inline void sig_wait()
	{	sigwaitinfo(&sigset,NULL);};
	
	// Constructor
	//##ModelId=41C841550291
	TTimer( 
		// ������� ������
		int ssec, 
		// ����������� ������
		int snsec, 
		// ������� ����-����
		int sec, 
		// ����������� ����-����
		int nsec, 
		// ������� �������
		const event_t& evnt, 
		// ������������� ��������� �������
		clockid_t clk_id = CLOCK_REALTIME,
		// ���� ����������� ������ �������
		// true		- ������ �� ����� �������
		// false	- ������ ����� �������
		bool suspend = true) throw( std::logic_error );
	
	//Constructor of timer with signal
	TTimer(
	// ������� ������
	int ssec, 
	// ����������� ������
	int snsec, 
	// ������� ����-����
	int sec, 
	// ����������� ����-����
	int nsec, 
	// signal used in timer work
	int signo = SIGUSR1, 
	// ������������� ��������� �������
	clockid_t clk_id = CLOCK_REALTIME,
	// ���� ����������� ������ �������
	// true		- ������ �� ����� �������
	// false	- ������ ����� �������
	bool suspend = true) throw( std::logic_error );
	
	// Default constructor
	//##ModelId=41C8415502A1
	TTimer();
	
	// Object destructor
	//##ModelId=41C8415502A2
	virtual ~TTimer();
};


#endif
