// Implementation of the Socket class.


#include "Socket.h"
#include "string.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <algorithm>
#include <assert.h>


Socket::Socket() 
: m_sock ( -1 )
{
	memset ( &m_addr, 0, sizeof ( m_addr ) );
}

Socket::Socket( int sock )
: m_sock( sock )
{
	memset ( &m_addr, 0, sizeof ( m_addr ) );
}

Socket::~Socket()
{
	finish();
}

bool Socket::create()
{
	finish();
	
	m_sock = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	int on = 1;
	assert( setsockopt( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* )&on, sizeof( on ) ) == 0 );

	return is_valid();
}

bool Socket::shutdown( int how )
{
	if( is_valid() )
		return ::shutdown( m_sock, how ) == 0;
	else
		return false;
}

void Socket::close()
{
	if ( is_valid() )
	{
		::close( m_sock );
		m_sock = -1;
	}
}

void Socket::finish()
{
	shutdown();
	close();	
}

bool Socket::bind ( int port )
{
	if ( !is_valid() )
		return false;
	
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = INADDR_ANY;
	m_addr.sin_port = htons ( port );

	int bind_return = ::bind ( m_sock, ( struct sockaddr * ) &m_addr, sizeof ( m_addr ) );

	if ( bind_return == -1 )
		return false;
	
	return true;
}

bool Socket::listen( int nMaxConnections )
{
	if ( !is_valid() )
		return false;

	int listen_return = ::listen ( m_sock, nMaxConnections );

	if ( listen_return == -1 )
		return false;

	return true;
}

bool Socket::accept( counted_ptr< Socket >& ptrSocketConnected )
{
	ptrSocketConnected = new Socket();
	int addr_length = sizeof ( ptrSocketConnected->m_addr );
	ptrSocketConnected->m_sock = ::accept ( m_sock, ( sockaddr * ) &ptrSocketConnected->m_addr, 
						  					( socklen_t * ) &addr_length );

	if ( ptrSocketConnected->m_sock == -1 )
	{
		ptrSocketConnected = NULL;
		return false;
	}
	else
		return true;
}

bool Socket::send( const char* lpData, int nSize )
{
	int status = ::send( m_sock, lpData, nSize, MSG_NOSIGNAL ); //MSG_NOSIGNAL );

	return status != -1;
}

int Socket::recv( char* lpData, int nSize )
{
	return ::recv( m_sock, lpData, nSize, 0 );
}

bool Socket::recvdata( char* lpData, int nSize )
{
	int nReceived = 0, nRet;

	// ���� �� ��� ������ �������
	while( nReceived < nSize )
	{
		// ��������� ���������� ����� ������
		nRet = recv( lpData + nReceived, nSize - nReceived );
		
		// ���� ��������� ������, �������
		if( nRet <= 0 )
			return false;

		// ����������� ������� ������
		nReceived += nRet;
	}

	return true;
}

bool Socket::sendblock( const char* lpData, int nSize )
{
	// �������� ������ ������
	if( !send( ( const char* )&nSize, sizeof( nSize ) ) )
		return false;

	// �������� ���� ������
	if( !send( lpData, nSize ) )
		return false;

	return true;
}

bool Socket::recvblock( char*& lpData, int& nSize )
{
	// �������� ������ ������
	if( !recvdata( ( char* )&nSize, sizeof( nSize ) ) )
		return false;

	if( nSize <= 0 )
		return false;

	// �������� ������
	lpData = new char[ nSize ];
	
	// �������� ���� ������
	if( !recvdata( lpData, nSize ) )
	{
		delete lpData;
		return false;
	}

	return true;
}

void Socket::setblocking( bool bBlocking )
{
	if( bBlocking )
		fcntl( m_sock, F_SETFL, fcntl( m_sock, F_GETFL ) | O_NONBLOCK );
	else
		fcntl( m_sock, F_SETFL, fcntl( m_sock, F_GETFL ) & ~O_NONBLOCK );
}

bool Socket::connect ( const std::string host, int port )
{
	if ( !is_valid() ) 
		return false;

	m_addr.sin_family = AF_INET;
	m_addr.sin_port = htons ( port );

	inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

	if ( errno == EAFNOSUPPORT ) 
		return false;

	int status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

	if ( status == 0 )
		return true;
	else
		return false;
}

// ������� ���������� ����� ��������� � �������
sockaddr_in Socket::getaddr()
{
	return m_addr;	
}

bool Socket::is_valid()
{
	return m_sock != -1; 
}
