#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int n;
    struct sockaddr_in servaddr;
    char buffer[20];
    char message[] = "My life is brilliant\n My life is brilliant\n My love is pure\n I saw an angel\n Of that I'm sure\n She smiled at me on the subway\n She was with another man\n But I won't lose no sleep on that\n 'Cause I've got a plan\n You're beautiful\n You're beautiful\n You're beautiful, it's true\n I saw your face in a crowded place\n And I don't know what to do\n 'Cause I'll never be with you\n Yes, she caught my eye\n As we walked on by\n She could see from my face that I was\n Fucking high\n And I don't think that I'll see her again\n But we shared a moment that will last 'til the end\n You're beautiful\n You're beautiful\n You're beautiful, it's true\n I saw your face in a crowded place\n And I don't know what to do\n 'Cause I'll never be with you\n La, la-la, la-la\n La, la-la, la-la\n La, la-la, la-la, la\n You're beautiful\n You're beautiful\n You're beautiful, it's true\n There must be an angel with a smile on her face\n When she thought up that I should be with you\n But it's time to face the truth\n I will never be with you";
    for(int i = 19; i < 20; i++) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("cannot create socket");
            continue;
        }

        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        snprintf(buffer, sizeof(buffer), "10.6.4.%d", i);
        servaddr.sin_addr.s_addr = inet_addr(buffer);
        servaddr.sin_port = htons(7228);

        if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            perror("connect failed");
            close(sockfd);
            continue; 
        }

        // Sending Message
        printf("Sending message to %s\n", buffer);
        n = write(sockfd, message, strlen(message));
        if (n < 0) {
            perror("write failed");
        }

        close(sockfd);
    }

    return 0;
}
