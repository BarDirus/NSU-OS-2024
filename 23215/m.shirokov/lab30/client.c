#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "unix_sock"
#define BUFFER_SIZE 256

int main() {
    int client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(client_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server. Enter text (type 'exit' to quit):\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        printf("> ");
        fflush(stdout);
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            break;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strcmp(buffer, "exit") == 0) {
            break;
        }
        if (write(client_fd, buffer, strlen(buffer)) == -1) {
            perror("write failed");
            break;
        }
    }

    close(client_fd);
    return 0;
}
