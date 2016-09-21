#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
int init_socet(int port)
{
        int sfd, cfd;
        char ch='k';
        struct sockaddr_in saddr;
        sfd= socket(AF_INET, SOCK_STREAM, 0);
        saddr.sin_family=AF_INET;
        saddr.sin_addr.s_addr=htonl(INADDR_ANY);
        saddr.sin_port=htons(port);
        bind(sfd, (struct sockaddr *)&saddr, sizeof(saddr));
        listen(sfd, 1);
        while(1)
        {
                printf("Server waiting\n");
                cfd=accept(sfd, (struct sockaddr *)NULL, NULL);
                if(read(cfd, &ch, 1)<0) perror("read");
                printf("%c\n",ch);
                ch++;
                printf("%c\n",ch);
                if(write(cfd, &ch, 1)<0) perror("write");
                close(cfd);
        }
}
int main()
{
    init_socet(2323);
}
