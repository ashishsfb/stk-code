#ifndef CONNECT_TO_SERVER_HPP
#define CONNECT_TO_SERVER_HPP

#include "../protocol.hpp"
#include <string>

class ConnectToServer : public Protocol, public CallbackObject
{
    public:
        ConnectToServer(CallbackObject* callbackObject);
        virtual ~ConnectToServer();
        
        virtual void messageReceived(uint8_t* data);
        virtual void setup();
        virtual void update();
        
        void setSelfAddress(uint32_t ip, uint16_t port);
        void setUsername(std::string username);
        void setPassword(std::string password);
        void setHostName(std::string hostName);
        
        bool connected;
    protected:
        uint32_t m_ownPublicIp;
        uint16_t m_ownPublicPort;
        uint32_t m_serverIp;
        uint16_t m_serverPort;
        std::string m_hostName;
        std::string m_username;
        std::string m_password;
        enum STATE
        {
            NOTHING,
            ADDRESS_KNOWN_ONLINE,
            PEER_ADDRESS_RETREIVED,
            CONNECTED
        };
        STATE m_state;
        double firstTime;
};

#endif // CONNECT_TO_SERVER_HPP