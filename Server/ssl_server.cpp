#include "ssl_server.h"

Ssl_server::Ssl_server(int port)
{
    int sock;
    SSL_CTX *ctx;

    init_openssl();
    ctx = create_context();

    configure_context(ctx);

    sock = init_socket(port);

    /* Handle connections */
    while(1) {
        struct sockaddr_in addr;
        uint len = sizeof(addr);
        SSL *ssl;
        const char reply[] = "test\n";

        int client = accept(sock, (struct sockaddr*)&addr, &len);
        if (client < 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
        }
        else {
            SSL_write(ssl, reply, strlen(reply));
        }

        SSL_free(ssl);
        close(client);
    }

    close(sock);
    SSL_CTX_free(ctx);
    cleanup_openssl();
}
void Ssl_server::init_openssl()
{
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}
int Ssl_server::init_socket(int port)
{
    int soc;
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    soc = socket(AF_INET, SOCK_STREAM, 0);

    if (soc < 0)
    {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    if (bind(soc, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("Unable to bind");
        exit(EXIT_FAILURE);
    }

    if (listen(soc, 1) < 0)
    {
        perror("Unable to listen");
        exit(EXIT_FAILURE);
    }

    return soc;
}

void Ssl_server::cleanup_openssl()
{
    EVP_cleanup();
}

SSL_CTX* Ssl_server::create_context()
{
    const SSL_METHOD *method;
        SSL_CTX *ctx;

        method = SSLv23_server_method();

        ctx = SSL_CTX_new(method);
        if (!ctx)
        {
            perror("Unable to create SSL context");
            ERR_print_errors_fp(stderr);
            exit(EXIT_FAILURE);
        }

        return ctx;
}
void Ssl_server::configure_context(SSL_CTX *ctx)
{
//    if(!SSL_CTX_set_ecdh_auto(ctx, 1))
//    {
//        ERR_print_errors_fp(stderr);
//    }

    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, "public.pem", SSL_FILETYPE_PEM) < 0) {
        ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, "private.pem", SSL_FILETYPE_PEM) < 0 ) {
        ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
    }
}
