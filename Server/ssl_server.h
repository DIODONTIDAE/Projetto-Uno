#ifndef SSL_SERVER_H
#define SSL_SERVER_H
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

class Ssl_server
{
    int init_socket(int port);
    void init_openssl();
    void cleanup_openssl();
    SSL_CTX* create_context();
    void configure_context(SSL_CTX *ctx);
public:
    Ssl_server(int port);
};

#endif // SSL_SERVER_H
