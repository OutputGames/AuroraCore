#if !defined(NETCODE_HPP)
#define NETCODE_HPP

#include "aurora/utils/utils.hpp"

#include <enet/enet.h>

struct NetworkHostInfo
{
	u32 maxClients;
	u32 maxChannels;

	ENetAddress address;
};

struct NetworkClientInfo : NetworkHostInfo
{
	
};

struct NetworkHost
{
	NetworkHostInfo creationInfo;
	ENetHost* agent;
};

struct NetworkClient
{
	NetworkHostInfo creationInfo;

	ENetHost* agent;
	ENetPeer* client;

	void ConnectToHost(ENetAddress address, u32 timeout);
	void DisconnectFromHost(u32 timeout);
};

struct NetworkClientData
{
	int peerID;
};

struct NetworkMgr
{
    static void Init();
    static NetworkHost* CreateHost(NetworkHostInfo info);
	static NetworkClient* CreateClient(NetworkClientInfo info);

	static void Dispatch(NetworkHost* host, u32 dispatchWaitTime);


};


#endif // NETCODE_HPP
