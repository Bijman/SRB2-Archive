#ifndef NETWORKING_H
#define NETWORKING_H

#include <winsock.h>

#define _CHAR_P(a, i) (*(unsigned char*) ((int) a + i))

/*#define MAKE_SOCKADDR(sa_pointer, a, b, c, d, e) \
{ \
int temp = (int) sa_pointer; \
unsigned char ipa = a, ipb = b, ipc = c, ipd = d; \
unsigned short port = e; \
*sa_pointer.sa_family = AF_INET; \
*(unsigned short*) (temp + 2) = (port << 8) + (port >> 8); \
*(unsigned __int32*) (temp + 4) = ipa + (ipb << 8) + (ipc << 16) + (ipd << 24); \
}*/

#define GET_PORT(sa_pointer) ((_CHAR_P(sa_pointer, 2) << 8) + _CHAR_P(sa_pointer, 3))
#define GET_PORT_ALT(sa_pointer) (*(unsigned short*) ((int) sa_pointer + 2))
#define GET_IP(sa_pointer)   (_CHAR_P(sa_pointer, 7) + (_CHAR_P(sa_pointer, 6) << 8) + (_CHAR_P(sa_pointer, 5) << 16) + \
(_CHAR_P(sa_pointer, 4) << 24))
#define GET_IP_ALT(sa_pointer) (*(unsigned __int32*) ((int) sa_pointer + 4))

#define ERROR_MEMALLOCFAILURE     1000000
#define ERROR_UNSUPPORTEDPROTOCOL 1000001
#define ERROR_CANNOTUSEPROTOCOL   1000002
#define ERROR_ALREADYLISTENING    1000003
#define ERROR_NOTLISTENING        1000004
//#define ERROR_UDPADDRESSNEEDED    1000005 - Turns out this won't be used because Network_Send and Network_Receive don't return errors.

/*
Guide on using UDP ('cause I'm probably going to forget otherwise):

First, create the socket, as usual. You may need to create one for receiving and one for sending - because a "connection reset" error can
appear otherwise - but it seems fine with just one as well. (Edit: I stand most corrected, use two instead.)
Then, bind the socket to a local address. A free unique port must be used.
Then, connect the socket to the IP (this is assuming you have one) and the port that the remote computer also used in its Connect call.
I have not yet written a function that can receive from any IP (and from thereon establish a connection).

This is all I seem to know for now. Very uneasy.
*/

typedef struct sock
{
	SOCKET ws_socket;    // WinSock socket descriptor.

	int listening;       // (Boolean) Whether or not this socket is currently listening.

	int protocol;        // Protocol used by this socket.

	int  udp_connected;
	char udp_addr[0xFF]; // Address for UDP sockets to send to and receive from.
						 // It's really a sockaddr structure- but who knows how large it'll be?
	int  udp_addrlen;    // Well, besides this guy.
} sock, vsock;

#ifdef __cplusplus
extern "C" {
#endif

int Network_Startup  (int* error);
int Network_Shutdown (int* error);

sock* Create_Socket  (int protocol, int blocking, int* error); // TCP, UDP
int   Destroy_Socket (sock* my_socket, int* error); // TCP, UDP

int Connect (sock* my_socket, char* ip_address, char* port, int *error); // TCP, UDP
int Bind    (sock* my_socket, const char* ip_address, const char* port, int *error); // UDP

int   Listen (sock* my_socket, char* ip_address, char* port, int max_connections, int* error); // TCP
sock* Accept (sock* my_socket, int* error); // TCP, UDP

int Network_Send    (sock* my_socket, void* buffer, int buffer_length);             // TCP, UDP
int Network_Receive (sock* my_socket, void* buffer, int buffer_length, int remove); // TCP, UDP

int Network_SendTo      (sock* my_socket, void* buffer, int buffer_length,             struct sockaddr* to);   // TCP, UDP
int Network_ReceiveFrom (sock* my_socket, void* buffer, int buffer_length, int remove, struct sockaddr* from); // TCP, UDP

int Make_SockAddr(struct sockaddr* socket_address, const char* ip_address, const char* port, int* error); // SRB2 special!

void SSetSockaddrIP(struct sockaddr* sa, const char* ip);
void SGetSockaddrIP(struct sockaddr* sa, char* ip);

unsigned int IGetSockaddrIP(struct sockaddr* sa);
void ISetSockaddrIP(struct sockaddr* sa, unsigned int ip);

unsigned short IGetSockaddrPort(struct sockaddr* sa);
void ISetSockaddrPort(struct sockaddr* sa, unsigned short port);

void SGetSockaddrIPPort(struct sockaddr* sa, char* ipport);
void SSetSockaddrIPPort(struct sockaddr* sa, char* ipport);

#ifdef __cplusplus
}
#endif

#endif
