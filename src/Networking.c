#include "Networking.h"

#include <stdlib.h>
#include <stdio.h> // For sprintf
//#include <ws2tcpip.h>
#include "i_system.h" // For I_GetTime
#include "i_net.h"
#include "d_net.h"
#include "g_game.h"

#define SETERROR(value) {if (error != NULL) *error = value;}
#define RETURNERROR(value, ret) {if (error != NULL) *error = value; return ret;}

struct esa
{
    short family;
    unsigned short port;
    unsigned char ip[4];
    char zero[8];
};

char network_init = 0;

extern SOCKET mysocket;

int __NET__String_Length(char* string);

int Network_Startup(int* error)
{
	//WSADATA dummy;
	int ret;

	//ret = WSAStartup(MAKEWORD(1, 1), &dummy);
	ret = 0;

	if (ret == 0) // No error.
	{
		network_init = 1;

		return 1;
	}

	SETERROR(ret);

	return 0;
}

int Network_Shutdown(int* error)
{
	int ret;

	if (! network_init)
		return 1; // Pretend we shut it down!

	//ret = WSACleanup();
	ret = 0;
	if (ret == SOCKET_ERROR)
	{
		SETERROR(WSAGetLastError())

		return 0;
	}

	network_init = 0;

	return 1; // Success!
}

sock* Create_Socket(int protocol, int blocking, int* error)
{
	SOCKET new_socket;
	sock*  my_socket;

    if (protocol == IPPROTO_TCP)
        new_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    else if (protocol == IPPROTO_UDP)
        new_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    else
        RETURNERROR(ERROR_UNSUPPORTEDPROTOCOL, NULL);

	if (new_socket == INVALID_SOCKET)
		RETURNERROR(WSAGetLastError(), NULL);

    int val = 64 * 1024, size = 4; // 64 KB buffer size.

    if (setsockopt(new_socket, SOL_SOCKET, SO_SNDBUF, (const char*) &val, size) == SOCKET_ERROR)
    {
        closesocket(new_socket);
        RETURNERROR(WSAGetLastError(), NULL);
    }

    if (setsockopt(new_socket, SOL_SOCKET, SO_RCVBUF, (const char*) &val, size) == SOCKET_ERROR)
    {
        closesocket(new_socket);
        RETURNERROR(WSAGetLastError(), NULL);
    }

	// Create our own kind of socket to hold special data... later on at least.
	my_socket = (sock*) malloc(sizeof (sock));

	if (my_socket == NULL)
		RETURNERROR(ERROR_MEMALLOCFAILURE, NULL);

	my_socket->ws_socket = new_socket;

	my_socket->protocol = protocol;

	my_socket->listening = 0;

	my_socket->udp_connected = 0;

	// Finally, set blocking.
	u_long value = (! blocking);

	ioctlsocket(my_socket->ws_socket, FIONBIO, &value);

	return my_socket;
}

int Destroy_Socket(sock* my_socket, int* error)
{
	// Close the WinSock socket attached to my_socket.
	if (closesocket(my_socket->ws_socket) == SOCKET_ERROR)
		RETURNERROR(WSAGetLastError(), 0);

	free(my_socket);

	return 1;
}

int Connect(sock* my_socket, char* ip_address, char* port, int *error)
{
    my_socket = my_socket;
    ip_address = ip_address;
    port = port;
    error = error;
    return 1; // Until we need it.
/*	struct addrinfo *ai_result, hints;
	int ret;

	// Connect can only be used on TCP sockets since UDP works too differently...
	// ...Or does it?
	//if (my_socket->protocol == IPPROTO_UDP)
	//	RETURNERROR(ERROR_CANNOTUSEPROTOCOL);

	memset(&hints, 0, sizeof (hints));

	// Attempt to find the address info.
	hints.ai_flags    = AI_PASSIVE;
	hints.ai_family   = AF_INET;
	hints.ai_socktype = (my_socket->protocol == IPPROTO_TCP ? SOCK_STREAM : SOCK_DGRAM);
	hints.ai_protocol = (my_socket->protocol == IPPROTO_TCP ? IPPROTO_TCP : IPPROTO_UDP);

	ret = getaddrinfo(ip_address, port, &hints, &ai_result);

	// Failed?
	if (ret != 0)
		RETURNERROR(ret, 0);

	// Connect the socket.
	if (my_socket->protocol == IPPROTO_TCP)
		ret = connect(my_socket->ws_socket, ai_result->ai_addr, ai_result->ai_addrlen);
	else // For UDP it's a bit different.
	{
		my_socket->udp_connected = 1;

		memcpy(my_socket->udp_addr,     ai_result->ai_addr,     ai_result->ai_addrlen);
		memcpy(&my_socket->udp_addrlen, &ai_result->ai_addrlen, 4);

		// POTENTIAL ERROR HERE:
		// ai_result->ai_addrlen might not match the full length of my_socket->udp_addr!
		// Thanks a lot, Microsoft.
	}

	freeaddrinfo(ai_result);

	if (ret == SOCKET_ERROR)
		RETURNERROR(WSAGetLastError(), 0);

	return 1;*/
}

int Bind(sock* my_socket, const char* ip_address, const char* port, int* error)
{
//	struct addrinfo *ai_result, hints;
	int ret;

	if (my_socket->protocol != IPPROTO_UDP)
		RETURNERROR(ERROR_CANNOTUSEPROTOCOL, 0);

    struct sockaddr sa;

    memset(&sa, 0, sizeof (struct sockaddr));

    sa.sa_family = AF_INET;

    ISetSockaddrPort(&sa, atoi(port));
    if (ip_address[0])
        SSetSockaddrIP(&sa, ip_address);
    else
        sa.sa_data[4] = sa.sa_data[5] = sa.sa_data[6] = sa.sa_data[7] = 0;

    ret = bind(my_socket->ws_socket, &sa, sizeof (sa));

	if (ret == SOCKET_ERROR)
		RETURNERROR(WSAGetLastError(), 0);

	return 1;
}

int Listen(sock* my_socket, char* ip_address, char* port, int max_connections, int* error)
{
    my_socket = my_socket;
    ip_address = ip_address;
    port = port;
    max_connections = max_connections;
    error = error;
    return 0; // Until we use it. I can't be bothered to port this to Winsock 1.1 without even being able to test it. =X
/*	struct addrinfo *ai_result, hints;
	int ret;

	if (my_socket->protocol != IPPROTO_TCP)
		RETURNERROR(ERROR_CANNOTUSEPROTOCOL, 0);
	if (my_socket->listening)
		RETURNERROR(ERROR_ALREADYLISTENING, 0);

	memset(&hints, 0, sizeof (hints));

	// Attempt to find the address info.
	hints.ai_flags    = AI_PASSIVE;
	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP; // This is definite - we checked it earlier.

	ret = getaddrinfo(ip_address, port, &hints, &ai_result);

	// Failed again!
	if (ret != 0)
		RETURNERROR(ret, 0);

	ret = bind(my_socket->ws_socket, ai_result->ai_addr, ai_result->ai_addrlen);

	// Quickly free the address info...
	freeaddrinfo(ai_result);

	if (ret == SOCKET_ERROR)
		RETURNERROR(WSAGetLastError(), 0);

	ret = listen(my_socket->ws_socket, max_connections);

	if (ret == SOCKET_ERROR)
		RETURNERROR(WSAGetLastError(), 0);

	// We've SUCCEEEEDED!
	my_socket->listening = 1;

	return 1;*/
}

sock* Accept(sock* my_socket, int* error)
{
	SOCKET accept_sock;
	sock* my_new_socket;

	if (my_socket->protocol == IPPROTO_UDP)
		RETURNERROR(ERROR_CANNOTUSEPROTOCOL, NULL);
	if (! my_socket->listening)
		RETURNERROR(ERROR_NOTLISTENING, NULL);

	accept_sock = accept(my_socket->ws_socket, NULL, NULL);

	if (accept_sock == INVALID_SOCKET)
		RETURNERROR(WSAGetLastError(), NULL);

	my_new_socket = (sock*) malloc(sizeof (sock));

	if (my_new_socket == NULL)
	{
		// Close the socket we just found... so close...
		closesocket(accept_sock);

		RETURNERROR(ERROR_MEMALLOCFAILURE, NULL);
	}

	my_new_socket->ws_socket = accept_sock;

	my_new_socket->protocol = IPPROTO_TCP;

	my_new_socket->listening = 0;

	my_new_socket->udp_connected = 0; // We don't actually need this, because it's TCP. But who cares? --CORRECTION 15/01/10, my_new_socket.

	return my_new_socket;
}

int Network_Send(sock* my_socket, void* buffer, int buffer_length)
{
	int ret;

	if (my_socket->protocol == IPPROTO_TCP)
		ret = send(my_socket->ws_socket, (char*)buffer, buffer_length, 0);
	else
	{
		if (! my_socket->udp_connected)
			return -1;

		ret = sendto(my_socket->ws_socket, (char*)buffer, buffer_length, 0, (struct sockaddr*) my_socket->udp_addr, my_socket->udp_addrlen);
	}

	return ret;
}

int Network_Receive(sock* my_socket, void* buffer, int buffer_length, int remove)
{
	int ret;

	if (my_socket->protocol == IPPROTO_TCP)
		ret = recv(my_socket->ws_socket, (char*)buffer, buffer_length, (remove ? 0 : MSG_PEEK));
	else
	{
		if (! my_socket->udp_connected)
			return -1;

		ret = recvfrom(my_socket->ws_socket, (char*)buffer, buffer_length, (remove ? 0 : MSG_PEEK),
			NULL, NULL);
	}

	return ret;
}

void*           debug_stored_data    [1000];
tic_t           debug_stored_times   [1000];
int             debug_stored_lengths [1000];
struct sockaddr debug_addresses      [1000];

extern sock* game_socket;

int lastrectime = 0;
int totalsentcount = 0;
int totalrecvdcount = 0;
int totalsent = 0;
int totalrecvd = 0;

extern char CS_host;

int Network_SendTo(sock* my_socket, void* buffer, int buffer_length, struct sockaddr* to)
{
	int ret = 0;
	int i;

    //if ((/*! (rand() % 2) || */b) && *(char*) buffer != 3 && *(char*) buffer != 1 && CS_host)
    //    return buffer_length;

    for (i = 0; i < 1000; i ++)
    {
        if (debug_stored_data[i] == NULL)
            break;
    }

    if (i == 1000)
        return 0;

/*    if ((rand() % 3) && ((*(char*) buffer) & 0x07) == 2)
        return buffer_length;*/

    debug_stored_data[i] = malloc(buffer_length);
    memcpy(debug_stored_data[i], buffer, buffer_length);
    debug_stored_lengths[i] = buffer_length;
    debug_stored_times[i] = I_GetTime();// + (rand() % 3) * (((*(char*) buffer) & 0x07) == 2);
    memcpy(&debug_addresses[i], to, sizeof (struct sockaddr));

    for (i = 0; i < 1000; i ++)
    {
        if (debug_stored_data[i] != NULL && (I_GetTime() >= debug_stored_times[i]))
        {
#ifndef NATIVENET
            // HACK - especially for SRB2!
            // Nonblocking UDP sockets don't seem to send any more than one message per tic. That is most odd.
            u_long value = 0;
            ioctlsocket(my_socket->ws_socket, FIONBIO, &value);

            ret = sendto(my_socket->ws_socket, (char*) debug_stored_data[i], debug_stored_lengths[i], 0, &debug_addresses[i], sizeof (struct sockaddr));

            value = 1;
            ioctlsocket(my_socket->ws_socket, FIONBIO, &value);
#else
            (void) my_socket;
            char address[25], loneaddress[25];
            int node = -1;
            int l;

            if (! I_NetMakeNode)
                I_NetOpenSocket();

            SGetSockaddrIPPort(&debug_addresses[i], address);
            SGetSockaddrIP(&debug_addresses[i], loneaddress);

            for (l = 0; l < MAXNETNODES + 1; l ++)
            {
                const char* addr = I_GetNodeAddress(l);

                if (addr == NULL)
                    continue;

                if (! strcmp(address, addr) || ! strcmp(loneaddress, addr))
                {
                    node = l;
                    break;
                }
            }

            if (node < 0)
                node = I_NetMakeNode(address);

            if (node < 0)
                continue;

            doomcom->remotenode = node;

            memcpy(&doomcom->data, debug_stored_data[i], debug_stored_lengths[i]);
            doomcom->datalength = debug_stored_lengths[i];

            I_NetSend();
            ret = buffer_length;
#endif

            free(debug_stored_data[i]);
            debug_stored_data[i] = NULL;

            if (I_GetTime() - lastrectime >= 35)
            {
                totalsent = totalsentcount;
                totalrecvd = totalrecvdcount;
                totalsentcount = 0;
                totalrecvdcount = 0;
                lastrectime = I_GetTime();
            }

            totalsentcount += ret;
        }
    }

	return ret;
}

extern void HandleStandardPacket();

UINT32 NetbufferChecksum(void);

int Network_ReceiveFrom(sock* my_socket, void* buffer, int buffer_length, int remove, struct sockaddr* from)
{
	int ret;
	int dummy = sizeof (struct sockaddr);

#ifdef NATIVENET
    if (! I_NetMakeNode || ! I_GetNodeAddress)
        I_NetOpenSocket();

    I_NetGet();

    if (doomcom->remotenode < 0)
        return 0;

	if (netbuffer->checksum == NetbufferChecksum()) // Reroute the message to GetPackets
	{
	    HandleStandardPacket();

        return 0;
	}

    const char* addr = I_GetNodeAddress(doomcom->remotenode);

    if (addr == NULL)
        return 0;

    SSetSockaddrIPPort(from, (char*) addr);
    from->sa_family = AF_INET;

    memcpy(buffer, doomcom->data, doomcom->datalength);
    buffer_length = doomcom->datalength;

    totalrecvdcount += buffer_length;

    return buffer_length;
#endif

	ret = recvfrom(my_socket->ws_socket, (char*)buffer, buffer_length, (remove ? 0 : MSG_PEEK), from, &dummy);

    if (ret > 0) totalrecvdcount += ret;

	return ret;
}

int Make_SockAddr(struct sockaddr* socket_address, const char* ip_address, const char* port, int* error)
{
    int i;
    struct hostent* he;
    char is_ip = 1;
    struct esa sa;

    sa.family = AF_INET;
    sa.port = htons(atoi(port));

    for (i = 0; ; i ++)
    {
        if ((ip_address[i] != '.' && (ip_address[i] < '0' || ip_address[i] > '9')) || i > 15)
            is_ip = 0;

        if (! ip_address[i])
            break;
    }

    if (! is_ip)
    {
        he = gethostbyname(ip_address);

        if (! he)
            RETURNERROR(1, 0);

        if (he->h_addr_list[0] == NULL)
            RETURNERROR(2, 0);

        for (i = 0; i < 4; i ++)
            sa.ip[i] = he->h_addr_list[0][i];
    }
    else
        SSetSockaddrIP((struct sockaddr*) &sa, ip_address);

    memcpy(socket_address, &sa, sizeof (struct sockaddr));

    return 1;
}

void SSetSockaddrIP(struct sockaddr* sa, const char* ip)
{
    int i;
    char temp[16];
    int last_point = 0;
    int ip_point = 0;
    struct esa* e = (struct esa*) sa;

    for (i = 0; ; i ++)
    {
        temp[i] = ip[i];

        if (ip[i] == '.' || ! ip[i])
        {
            temp[i] = 0;
            e->ip[ip_point ++] = atoi(&temp[last_point]);

            last_point = i + 1;

            if (! ip[i])
                break;
        }
    }
}

void SGetSockaddrIP(struct sockaddr* sa, char* ip)
{
    struct esa* e = (struct esa*) sa;

    sprintf(ip, "%i.%i.%i.%i", e->ip[0], e->ip[1], e->ip[2], e->ip[3]);
}

void ISetSockaddrIP(struct sockaddr* sa, unsigned int ip)
{
    struct esa* e = (struct esa*) sa;

    e->ip[0] = ip & 0xFF;
    e->ip[1] = ip >> 8 & 0xFF;
    e->ip[2] = ip >> 16 & 0xFF;
    e->ip[3] = ip >> 24 & 0xFF;
}

unsigned int IGetSockaddrIP(struct sockaddr* sa)
{
    struct esa* e = (struct esa*) sa;

    return e->ip[0] | (e->ip[1] << 8) | (e->ip[2] << 16) | (e->ip[3] << 24);
}

void ISetSockaddrPort(struct sockaddr* sa, unsigned short port)
{
    struct esa* e = (struct esa*) sa;

    e->port = htons(port);
}

unsigned short IGetSockaddrPort(struct sockaddr* sa)
{
    struct esa* e = (struct esa*) sa;

    return ntohs(e->port);
}

void SGetSockaddrIPPort(struct sockaddr* sa, char* ipport)
{
    struct esa* e = (struct esa*) sa;

    sprintf(ipport, "%i.%i.%i.%i:%i", e->ip[0], e->ip[1], e->ip[2], e->ip[3], ntohs(e->port));
}

void SSetSockaddrIPPort(struct sockaddr* sa, char* ipport)
{
    int i, colonpoint = -1;
    char temp[22];

    for (i = 0; i < 22; i ++)
    {
        temp[i] = ipport[i];

        if (ipport[i] == ':')
            colonpoint = i;

        if (! ipport[i])
            break;
    }

    if (colonpoint != -1)
        temp[colonpoint] = 0;

    SSetSockaddrIP(sa, temp);

    if (colonpoint != -1)
        ISetSockaddrPort(sa, atoi(&temp[colonpoint + 1]));
}

int _NET_String_Length(char* string)
{
	int i = 0;

	while (string[i ++]);

	return i;
}
