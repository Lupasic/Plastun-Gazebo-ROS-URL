/* 
 * counted_ptr - simple reference counted pointer.
 *
 * The is a non-intrusive implementation that allocates an additional
 * int and pointer for every counted object.
 * @author	Scott Meyers
 */

#ifndef COUNTED_PTR_H
#define COUNTED_PTR_H

/* For ANSI-challenged compilers, you may want to #define
 * NO_MEMBER_TEMPLATES or explicit */

 
class counted_ptr_base {
protected:

};
    
template <class X> class counted_ptr
	: public counted_ptr_base
{
public:
    typedef X element_type;

    counted_ptr( X* p = 0 ) // allocate a new counter
        : itsCounter( 0 ) 
	{
		if ( p ) 
			itsCounter = new counter( p );
	}

    ~counted_ptr()
    {
		release();
	}
    
	counted_ptr( const counted_ptr& r ) throw()
	{
		acquire( r.itsCounter );
	}

    counted_ptr& operator=( const counted_ptr& r )
    {
        if ( this != &r ) 
		{
            release();
            acquire( r.itsCounter );
        }
        return *this;
    }

#ifndef NO_MEMBER_TEMPLATES
	template <class Y> counted_ptr( counted_ptr<Y>& r ) throw()
	{
		acquire( reinterpret_cast< const counted_ptr& >( r ).itsCounter );
	}
    
	template <class Y> counted_ptr& operator=( const counted_ptr<Y>& r )
    {
        if ( ( void* )this != ( void* )&r ) 
		{
            release();
            acquire( reinterpret_cast< const counted_ptr& >( r ).itsCounter );
        }
        return *this;
    }
#endif // NO_MEMBER_TEMPLATES

    X& operator*() const throw()   
	{
		return *( X* )itsCounter->ptr;
	}
    
	X* operator->() const throw()   
	{
		return ( X* )itsCounter->ptr;
	}
    
	X* get() const throw()   
	{
		return itsCounter ? ( X* )itsCounter->ptr : 0;
	}
    
	bool unique()   const throw()
    {
		return ( itsCounter ? itsCounter->count == 1 : true );
	}
	
	bool operator!()
	{
		return get() == 0;	
	}

private:
	struct counter {
		counter( void* p = 0, unsigned c = 1 ) 
			: ptr( p ), count( c ) 
		{}
	        
		void*          ptr;
		unsigned    count;
	};

	void acquire( counter* c ) throw()
    { // increment the count
        itsCounter = c;
        if( c ) 
			++c->count;
    }

    void release()
    { // decrement the count, delete if it is 0
        if ( itsCounter ) 
		{
            if ( --itsCounter->count == 0 ) 
			{
                delete ( X* )itsCounter->ptr;
                delete itsCounter;
            }
            itsCounter = 0;
        }
    }
    
	counter* itsCounter;
};

#endif // COUNTED_PTR_H
