#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ev.h>
#include <time.h>
#include <sys/time.h>

#define PORT 5000
#define BUFFER_SIZE 1024

struct ev_loop *loop;

void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents) {
    char buffer[BUFFER_SIZE];
    ssize_t read_size = recv(watcher->fd, buffer, BUFFER_SIZE, 0);
    if (read_size <= 0) {
        close(watcher->fd);
        ev_io_stop(loop, watcher);
        free(watcher);
        return;
    }

    buffer[read_size] = '\0';

    time_t now = time(NULL);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *t = localtime(&now);
    char timestamp[24];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
    snprintf(timestamp + strlen(timestamp), sizeof(timestamp) - strlen(timestamp), ".%03ld", tv.tv_usec / 1000);

    printf("[%s] Получено: %s\n", timestamp, buffer);

    if (send(watcher->fd, buffer, read_size, 0) == -1) {
        perror("send");
        close(watcher->fd);
        ev_io_stop(loop, watcher);
        free(watcher);
    }
}


void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(watcher->fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
        perror("accept");
        return;
    }

    ev_io *client_watcher = (ev_io *)malloc(sizeof(ev_io));
    if (!client_watcher) {
        perror("malloc");
        close(client_fd);
        return;
    }

    ev_io_init(client_watcher, read_cb, client_fd, EV_READ);
    ev_io_start(loop, client_watcher);

    char *client_ip = inet_ntoa(client_addr.sin_addr);
    printf("Подключение: %s:%d\n", client_ip, ntohs(client_addr.sin_port));
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 10) == -1) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    loop = ev_default_loop(0);

    ev_io server_watcher;
    ev_io_init(&server_watcher, accept_cb, server_fd, EV_READ);
    ev_io_start(loop, &server_watcher);

    ev_run(loop, 0);

    close(server_fd);
    return 0;
}
