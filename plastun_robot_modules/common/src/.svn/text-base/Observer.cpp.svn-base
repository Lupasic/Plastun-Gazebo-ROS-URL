#include "Observer.h"

void Observable::AddObserver( Observer* lpObj )
{
	m_Observers.insert( lpObj );
}

void Observable::RemoveObserver( Observer* lpObj )
{
	m_Observers.erase( lpObj );
}

void Observable::NotifyAllObservers( int nParam )
{
	for( ObserverContainer_t::iterator p = m_Observers.begin(); p != m_Observers.end(); p++ )
		( *p )->Update( this, nParam );
}
