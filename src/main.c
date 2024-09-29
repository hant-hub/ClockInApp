#include <bits/sockaddr.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>
#include <unistd.h>

#define PORT 8000



int main() {
    int fd_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_sock == -1) {
        perror("Socket Failed");
        return EXIT_FAILURE;
    }

    struct sockaddr_in Host_addr = {0};
    int addrLen = sizeof(Host_addr);
    Host_addr.sin_family = AF_INET;
    Host_addr.sin_port = htons(PORT);
    Host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(fd_sock, (struct sockaddr*)&Host_addr, addrLen)) {
        perror("Bind Failed");
        return EXIT_FAILURE;
    }

    if (listen(fd_sock, 5)) {
        perror("Listen Failed");
        return EXIT_FAILURE;
    }

    int fd;
    if ((fd = accept(fd_sock, (struct sockaddr*)&Host_addr, (socklen_t*)&addrLen)) == -1) {
        perror("Accept Failed");
        return EXIT_FAILURE;
    }

    char buffer[1024] = {0};
    ssize_t bytes_read;
    if ((bytes_read = read(fd, buffer, sizeof(buffer))) < 0) {
        perror("failed to read");
        return EXIT_FAILURE;
    }

    printf("%.*s\n", (int)bytes_read, buffer);

    static const char resp[] = 
        "HTTP/1.0 200 OK\r\n"
        "Server: webserver-c\r\n"
        "Content-type: text/html\r\n\r\n"
        "<html>hello, world</html>\r\n";
    write(fd, resp, sizeof(resp));

    close(fd);
    close(fd_sock);
    return EXIT_SUCCESS;
}
