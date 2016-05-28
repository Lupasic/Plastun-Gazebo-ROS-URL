#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <set>

// Реализация шаблона Observer
// Observer - интерфейс наблюдателя за состоянием объекта
// Observable - наблюдаемый объект
//
// НЕ thread-safe

class Observable;

class Observer {
public:
	virtual void Update( Observable* lpObj, int nParam ) = 0;
	virtual ~Observer() {}
};

class Observable {
	typedef std::set< Observer* > ObserverContainer_t;
	ObserverContainer_t m_Observers;
public:
	void AddObserver( Observer* lpObj );
	void RemoveObserver( Observer* lpObj );
	void NotifyAllObservers( int nParam = 0 );
	
	virtual ~Observable() {}
};

#endif /*OBSERVER_H_*/
