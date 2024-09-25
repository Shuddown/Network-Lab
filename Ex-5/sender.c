#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>

#define PORT 3576



typedef struct {
    struct in_addr source_ip;
    int source_mac[6];
    struct in_addr dest_ip;
    unsigned char dest_mac[6];
    int16_t data;
}arp;

void get_ip_mac_address(arp* data) {
    int fd;
    struct ifreq ifr;
    
    // Create a socket to communicate with the network interface
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    strcpy(ifr.ifr_name, "eth0");
    
    // Get the IP address
    if (ioctl(fd, SIOCGIFADDR, &ifr) == -1) {
        perror("ioctl");
        close(fd);
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in *ipaddr = (struct sockaddr_in *)&ifr.ifr_addr;
    data->source_ip = ipaddr->sin_addr;


    // Get the MAC address
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) == -1) {
        perror("ioctl");
        close(fd);
        exit(EXIT_FAILURE);
    }
    unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    for(int i = 0; i < 6; i++){
        data->source_mac[i]=mac[i];
    }
}

int main(int argc, char **argv) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    arp my_data;

    my_data.data = 0;


    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET,argv[1], &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    inet_pton(AF_INET,argv[1], &my_data.dest_ip); 
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    send(sock, &my_data, sizeof(my_data), 0);
    printf("Struct sent\n");

    close(sock);
    return 0;
}
