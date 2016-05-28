#ifndef SOCKET_H_HEADER_INCLUDED_BB75362F
#define SOCKET_H_HEADER_INCLUDED_BB75362F

#include <errno.h>
#include <fcntl.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "counted_ptr.h"

#define DEFAULTMAXCONNECTIONS	5

// �����, ����������� �����.
// �������� �������� ������� ��� ������ � ��������, � ����� ������� ������� ����� ������ = ������ + ���� ������, �
// ����� ��������� ����� ������.
class Socket
{
protected:
	sockaddr_in m_addr;

public:
	int m_sock;
	// ���������� ������
	Socket();
	
	// ���������� ������
	Socket( int sock );

	// ������� �������� ������
	bool create();

	// ������������� �������� �� ������
	bool shutdown( int how = SHUT_RDWR );

	// ������� �������� ������
	void close();
	
	// ������� ���������� ������ � �������� ������ (thread-safe)
	void finish();

	// ������� �������� ������ � ����� ���������� ������
	bool bind ( int port );
	// ������� ��������� ����������� � ������
	bool listen( int nMaxConnections = DEFAULTMAXCONNECTIONS );
	// ������� �������� �����������
	bool accept( counted_ptr< Socket >& ptrSocketConnected );

	// ������� ����������� � ���������� ������
	bool connect ( std::string host, int port );

	// ������� ������� ������
	bool send( const char* lpData, int nSize );
	// ������� ��������� ������
	int recv( char* lpData, int nSize );
	// ������� ���������������� ��������� ������ ������������� �������
	bool recvdata( char* lpData, int nSize );

	// ������� ������� ����� ������
	bool sendblock( const char* lpData, int nSize );
	// ������� ��������� ����� ������
	bool recvblock( char*& lpData, int& nSize );
	
	// ������� ��������� ����� ������
	void setblocking( bool bBlocking );

	// ������� �������� ���������� ������
	bool is_valid();
	
	// ������� ���������� ����� ��������� � �������
	sockaddr_in getaddr();

	// ���������� ������
	virtual ~Socket();
};



#endif /* SOCKET_H_HEADER_INCLUDED_BB75362F */
