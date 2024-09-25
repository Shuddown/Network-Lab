#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
int main(int argc, char **argv)
{
    int len;
    int sockfd, newfd, n;
    struct sockaddr_in servaddr, cliaddr;
    char buff[1024];
    char str[1000];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        perror("cannot create socket");
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(7228);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        perror("Bind error");
    listen(sockfd, 2);
    len = sizeof(cliaddr);
    newfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
    // printf("hi");
    // Receiving the message
    n = read(newfd, buff, sizeof(buff));
    printf("\nReceived Message is \t%s", buff);
    
    close(sockfd);
    close(newfd);
    return 0;
}