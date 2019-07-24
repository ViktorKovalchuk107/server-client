#include <iostream>
#include <cstring>
#include <memory>
#include "names.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

typedef int SOCKET;


const unsigned PORT = 10;

int main() {
    NameParser pars;
    pars.load();


    int bufSize = 128;

    SOCKET s;

    s = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if(s == 0) {
        std::cout << "Failed to create socket!" << std::endl;
        return -1;
    }

    int off = 0;
    if(0 != setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&off, sizeof(off))) {
        std::cout << "Failed to set double-proto option!" << std::endl;
        return -1;
    }

    sockaddr_in6 addr = {};
    addr.sin6_family = AF_INET6;
    addr.sin6_port = htons(PORT);
    addr.sin6_addr = in6addr_any;

    if(0 != bind(s, (sockaddr*) &addr, sizeof(addr))) {
        std::cout << "Failed to bind socket!" << std::endl;
        return -1;
    }

    if(0 != listen(s, 5)) {
        std::cout << "Failed to start listening!" << std::endl;
        return -1;
    }

    std::cout << "Socket is listening..." << std::endl;

    SOCKET client_socket;
    addrinfo client_addr;

    client_socket=accept(s, client_addr.ai_addr, &client_addr.ai_addrlen);
    if(client_socket) {
        std::cout << "Connection accepted." << std::endl;
    }
    else {
        std::cout << "Accept returned error!" << std::endl;
        return -1;
    }

    auto buf = std::make_unique<char[]>(bufSize);
    memset(buf.get(), 0, bufSize);

    std::string data;
    while(recv(client_socket, buf.get(), bufSize - 1, 0)>0) {
        data += buf.get();
        memset(buf.get(), 0, bufSize);

        if(data[data.length() - 1] == '!') {
            break;
        }
    }

    std::cout << "Data from client" << std::endl;
    std::cout << data << std::endl;
    if (!pars.check(data))
        pars.add(data);
    std::string command = pars.names_to_string();
    int res = send(client_socket, command.c_str(), command.length(), 0);
    if (res != command.length()) {
        std::cout << "Send failed! Sent " << res << " bytes of " << command.length() << std::endl;
        return -1;
    }
    std::cout << "Send ok." << std::endl;

    shutdown(client_socket, SHUT_RDWR);
    close(client_socket);
    close(s);

    std::cout << "Sockets closed." << std::endl;

    return 0;
}