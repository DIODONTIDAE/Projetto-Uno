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
    int soc;
    int err;
    char* buff;
    struct sockaddr_in addr;
    uint len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4433);
    addr.sin_addr.s_addr = inet_addr("localhost");
    soc = socket(AF_INET, SOCK_STREAM, 0);
    if (soc < 0)
    {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }
    qDebug() << "socket created";
    if (bind(soc, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }
    qDebug() << "socket binded";
    if (listen(soc, 1) < 0)
    {
        perror("Unable to listen");
        exit(EXIT_FAILURE);
    }
    qDebug() << "connecting";
    int client = accept(soc, (struct sockaddr*)&addr, &len);
    if (client < 0) {
        perror("Unable to accept");
        exit(EXIT_FAILURE);
    }
    /*SSL PART*/
    const SSL_METHOD *meth;
    SSL_CTX  *ctx;
    SSL  *myssl;

    SSL_library_init();
    meth = SSLv23_server_method();
    ctx = SSL_CTX_new(meth);
    if (!ctx) {
        printf("Error creating the context.\n");
        exit(0);
    }
    myssl = SSL_new(ctx);

    if(!myssl)
    {
        printf("Error creating SSL structure.\n");
        exit(0);
    }
        SSL_set_fd(myssl,client);
        err = SSL_connect(myssl);
        if (SSL_get_verify_result(myssl) != X509_V_OK)
        {
            printf("SSL Client Authentication error\n");
            SSL_free(myssl);
            SSL_CTX_free(ctx);
            exit(0);
        }
    err = SSL_read (myssl, buff, sizeof(buff));
    printf("Client said: %s\n",buff);
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
