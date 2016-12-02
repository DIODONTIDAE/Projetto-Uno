#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int main()
{
    int cfd;
    struct sockaddr_in addr;
    char ch='r';
    cfd=socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family=AF_INET;
    addr.sin_addr.s_addr=inet_addr("127.0.0.1"); /* Check for server on loopback */
    addr.sin_port=htons(2323);
    if(connect(cfd, (struct sockaddr *)&addr,sizeof(addr))<0)
    {
        perror("connect error");
        return -1;
    }
    if(write(cfd, &ch, 1)<0) perror("write");
    if(read(cfd, &ch, 1)<0) perror("read");
    printf("\nReply from Server: %c\n",ch);
    close(cfd);
    return 0;
}
