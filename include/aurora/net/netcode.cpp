#include "netcode.hpp"

using namespace std;

void NetworkClient::ConnectToHost(ENetAddress address, u32 timeout)
{
    ENetEvent event;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    client = enet_host_connect(agent, &address, creationInfo.maxChannels, 0);
    if (client == NULL)
    {
        fprintf(stderr,
            "No available peers for initiating an ENet connection.\n");
        exit(EXIT_FAILURE);
    }
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(agent, &event, timeout) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        cout << "Connection to " << address.host << ":" << address.port << "succeeded.";

    }
    else
    {
        /* Either the seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(client);
        cout << "Connection to " << address.host << ":" << address.port << "failed.";
    }
}

void NetworkClient::DisconnectFromHost(u32 timeout)
{
    ENetEvent event;

    enet_peer_disconnect(client, 0);
    /* Allow up to 3 seconds for the disconnect to succeed
     * and drop any packets received packets.
     */
    while (enet_host_service(agent, &event, timeout) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_RECEIVE:
            enet_packet_destroy(event.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            cout << ("Disconnection succeeded.") << endl;
            return;
        }
    }
    /* We've arrived here, so the disconnect attempt didn't */
    /* succeed yet.  Force the connection down.             */
    enet_peer_reset(client);
}

void NetworkMgr::Init()
{
    if (enet_initialize() != 0)
    {
        cerr <<  "An error occurred while initializing ENet" << endl;
    }


}

NetworkHost* NetworkMgr::CreateHost(NetworkHostInfo info)
{
    NetworkHost* networkHost = new NetworkHost;

    networkHost->agent = enet_host_create(&info.address, info.maxClients, info.maxChannels, 0, 0);

    return networkHost;
}

NetworkClient* NetworkMgr::CreateClient(NetworkClientInfo info)
{
    NetworkClient* networkClient = new NetworkClient;

    networkClient->agent = enet_host_create(&info.address, 1, info.maxChannels, 0, 0);

    return networkClient;
}

void NetworkMgr::Dispatch(NetworkHost* host, u32 waitTime)
{
    ENetEvent event;

    if (enet_host_service(host->agent, &event, waitTime) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            printf("A new client connected from %x:%u.\n",
                event.peer->address.host,
                event.peer->address.port);

            cout << "New Client" << endl;

            cout << "\t" << "Address: " << event.peer->address.host << endl;
            cout << "\t" << "Port: " << event.peer->address.port << endl;

            /*
            NetworkClientData* data = new NetworkClientData;
            data->peerID = 0;
            */

            /* Store any relevant client information here. */
            event.peer->data = "Data";
            break;
        case ENET_EVENT_TYPE_RECEIVE:

            cout << "Packet Recieved" << endl;

            cout << "\t" << "Size: " << event.packet->dataLength << endl;
            cout << "\t" << "Data: " << event.packet->data << endl;
            cout << "\t" << "Client: " << event.peer->data << endl;
            cout << "\t" << "Channel: " << event.channelID << endl;

            // Do packet shit here


            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);

            break;

        case ENET_EVENT_TYPE_DISCONNECT:

            cout << event.peer->data << " disconnected." << endl;
            /* Reset the peer's client information. */
            event.peer->data = nullptr;
            break;
        }
    }
}