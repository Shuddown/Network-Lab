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
    
    // create a socket to communicate with the network interface
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    strcpy(ifr.ifr_name, "eth0");
    
    // get the ip address
    if (ioctl(fd, SIOCGIFADDR, &ifr) == -1) {
        perror("ioctl");
        close(fd);
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in *ipaddr = (struct sockaddr_in *)&ifr.ifr_addr;


    // get the mac address
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) == -1) {
        perror("ioctl");
        close(fd);
        exit(EXIT_FAILURE);
    }
    unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    for(int i = 0; i < 6; i++){
        data->dest_mac[i]=mac[i];
    }
}

int main(int argc, char **argv){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    arp received_data;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    int valread = recv(new_socket, &received_data, sizeof(received_data), 0);
    if (valread < 0) {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    printf("Received struct:\n");
    printf("IP address: %s\n", inet_ntoa(received_data.source_ip));
    printf("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n", 
           received_data.source_mac[0], received_data.source_mac[1], received_data.source_mac[2], received_data.source_mac[3], received_data.source_mac[4], received_data.source_mac[5]);
    printf("Destination IP: %s\n", inet_ntoa(received_data.dest_ip));

    get_ip_mac_address(&received_data);
    valread = send(new_socket, &received_data, sizeof(received_data), 0);
    

    close(new_socket);
    close(server_fd);
    return 0;
} 
