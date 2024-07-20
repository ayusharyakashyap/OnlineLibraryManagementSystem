#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Function prototypes
void *handle_client(void *client_socket);
void authenticate_user(int client_socket);
void admin_menu(int client_socket);
void user_menu(int client_socket);
void add_book(int client_socket);
void delete_book(int client_socket);
void modify_book(int client_socket);
void search_book(int client_socket);
void lock_file(int fd);
void unlock_file(int fd);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Set address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        pthread_t thread_id;
        int *pclient = malloc(sizeof(int));
        if (pclient == NULL) {
            perror("malloc failed");
            close(new_socket);
            continue;
        }
        *pclient = new_socket;
        if (pthread_create(&thread_id, NULL, handle_client, pclient) != 0) {
            perror("pthread_create failed");
            close(new_socket);
            free(pclient);
        }
        pthread_detach(thread_id); // Ensure the thread resources are released after completion
    }

    close(server_fd);
    return 0;
}

void *handle_client(void *client_socket) {
    int socket = *(int *)client_socket;
    free(client_socket);
    authenticate_user(socket);
    close(socket);
    pthread_exit(NULL);
}

void authenticate_user(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    read(client_socket, buffer, BUFFER_SIZE);
    if (strcmp(buffer, "admin") == 0) {
        admin_menu(client_socket);
    } else {
        user_menu(client_socket);
    }
}

void admin_menu(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    while (1) {
        read(client_socket, buffer, BUFFER_SIZE);
        if (strcmp(buffer, "add") == 0) {
            add_book(client_socket);
        } else if (strcmp(buffer, "delete") == 0) {
            delete_book(client_socket);
        } else if (strcmp(buffer, "modify") == 0) {
            modify_book(client_socket);
        } else if (strcmp(buffer, "search") == 0) {
            search_book(client_socket);
        } else if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }
}

void user_menu(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    while (1) {
        read(client_socket, buffer, BUFFER_SIZE);
        if (strcmp(buffer, "search") == 0) {
            search_book(client_socket);
        } else if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }
}

/*
void add_book(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    read(client_socket, buffer, BUFFER_SIZE);
    int fd = open("books.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) {
        perror("open failed");
        return;
    }
    lock_file(fd);
    write(fd, buffer, strlen(buffer));
    write(fd, "\n", 1);
    unlock_file(fd);
    close(fd);
}
*/

void add_book(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char response[BUFFER_SIZE] = {0};

    // Read book details from client
    int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
    if (bytes_read <= 0) {
        perror("read failed");
        snprintf(response, BUFFER_SIZE, "Failed to read book details\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }

    int fd = open("books.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) {
        perror("open failed");
        snprintf(response, BUFFER_SIZE, "Failed to open books file\n");
        send(client_socket, response, strlen(response), 0);
        return;
    }

    lock_file(fd);
    write(fd, buffer, strlen(buffer));
    write(fd, "\n", 1);
    unlock_file(fd);
    close(fd);

    snprintf(response, BUFFER_SIZE, "Book added successfully\n");
    send(client_socket, response, strlen(response), 0);
}


void delete_book(int client_socket) {
    // Simplified: just a placeholder for actual implementation
    char buffer[BUFFER_SIZE] = "Delete book not implemented\n";
    send(client_socket, buffer, strlen(buffer), 0);
}

void modify_book(int client_socket) {
    // Simplified: just a placeholder for actual implementation
    char buffer[BUFFER_SIZE] = "Modify book not implemented\n";
    send(client_socket, buffer, strlen(buffer), 0);
}

/*
void search_book(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    read(client_socket, buffer, BUFFER_SIZE);
    int fd = open("books.txt", O_RDONLY);
    if (fd < 0) {
        perror("open failed");
        return;
    }
    lock_file(fd);
    while (read(fd, buffer, BUFFER_SIZE) > 0) {
        send(client_socket, buffer, strlen(buffer), 0);
    }
    unlock_file(fd);
    close(fd);
}
*/

void search_book(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char search_query[BUFFER_SIZE] = {0};
    int fd = open("books.txt", O_RDONLY);
    if (fd < 0) {
        perror("open failed");
        snprintf(buffer, BUFFER_SIZE, "Failed to open books file\n");
        send(client_socket, buffer, strlen(buffer), 0);
        return;
    }

    lock_file(fd);

    // Read search query from client
    int bytes_read = read(client_socket, search_query, BUFFER_SIZE);
    if (bytes_read <= 0) {
        perror("Failed to read search query from client");
        snprintf(buffer, BUFFER_SIZE, "Failed to read search query\n");
        send(client_socket, buffer, strlen(buffer), 0);
        unlock_file(fd);
        close(fd);
        return;
    }
    printf("Received search query: %s\n", search_query);

    char line[BUFFER_SIZE] = {0};
    int found = 0;
    while (read(fd, line, BUFFER_SIZE) > 0) {
        if (strstr(line, search_query) != NULL) {
            send(client_socket, line, strlen(line), 0);
            found = 1;
            break;
        }
        memset(line, 0, BUFFER_SIZE); // Clear buffer for next line
    }
    if (!found) {
        snprintf(buffer, BUFFER_SIZE, "Book not found\n");
        send(client_socket, buffer, strlen(buffer), 0);
    }
    unlock_file(fd);
    close(fd);
}


void lock_file(int fd) {
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    while (fcntl(fd, F_SETLKW, &fl) == -1) {
        if (errno != EINTR) {
            perror("fcntl failed");
            exit(EXIT_FAILURE);
        }
    }
}

void unlock_file(int fd) {
    struct flock fl;
    fl.l_type = F_UNLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    if (fcntl(fd, F_SETLK, &fl) == -1) {
        perror("fcntl failed");
        exit(EXIT_FAILURE);
    }
}
