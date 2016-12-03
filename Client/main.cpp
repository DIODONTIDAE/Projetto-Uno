#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QDebug>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <openssl/ssl.h>

int main()
{
    int count;
    int socket_l;
    int err;
    char* buff;
    struct sockaddr_in addr;
    socket_l = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4433);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    uint len = sizeof(addr);
    if (socket_l < 0)
    {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }
    qDebug() << "socket created";
    /*if (bind(socket_l, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }
    qDebug() << "socket binded";*/
    if (connect(socket_l, (struct sockaddr*)&addr, len)) {
        perror("Unable to accept");
        exit(EXIT_FAILURE);
    }
    /*SSL PART*/
    const SSL_METHOD *meth;
    SSL_CTX  *ctx;
    SSL  *myssl;

    SSL_library_init();
    qDebug() << "library inited";
    meth = SSLv23_server_method();
    ctx = SSL_CTX_new(meth);
    if (!ctx) {
        printf("Error creating the context.\n");
        exit(0);
    }
    myssl = SSL_new(ctx);
    qDebug() << "structure created";
    if(!myssl)
    {
        qDebug() << "Error creating SSL structure.\n";
        exit(0);
    }

        SSL_set_fd(myssl,socket_l);
        err = SSL_connect(myssl);
        if (!err)
        {qDebug() << "connected by ssl";}
        if (SSL_get_verify_result(myssl) != X509_V_OK)
        {
            qDebug() << "SSL Client Authentication error\n";
            SSL_free(myssl);
            SSL_CTX_free(ctx);
            exit(0);
        }
    err = SSL_read(myssl, buff, sizeof(buff));
    if (!err)
    {qDebug() << "Error read";}
    qDebug() << "Client said:", buff;
    err=SSL_shutdown(myssl);
    count = 1;

    /***********************************/
    /* Try SSL_shutdown() 5 times to   */
    /* wait for the remote application */
    /* to issue SSL_shutdown().        */
    /***********************************/

    while(err != 1) {
    err=SSL_shutdown(myssl);
    if(err != 1)
    count++;
    if (count == 5)
    break;
    }

    if(err<0)
    printf("Error in shutdown\n");
    else if(err==1)
    printf("Client exited gracefully\n");

    SSL_free(myssl);
    SSL_CTX_free(ctx);
    exit(0);
    return 0;
}
