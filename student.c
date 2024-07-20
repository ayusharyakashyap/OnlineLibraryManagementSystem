#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void search_book(int sock);

/*
int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return -1;
    }

    send(sock, "user", strlen("user"), 0);

    while (1) {
        printf("Enter command (search/exit): ");
        scanf("%s", buffer);

        send(sock, buffer, strlen(buffer), 0);
        if (strcmp(buffer, "search") == 0) {
            search_book(sock);
        } else if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    close(sock);
    return 0;
}
*/

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return -1;
    }

    send(sock, "user", strlen("user"), 0);

    while (1) {
        printf("Enter command (search/exit): ");
        scanf("%s", buffer);

        send(sock, buffer, strlen(buffer), 0);
        if (strcmp(buffer, "search") == 0) {
            search_book(sock);
        } else if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    close(sock);
    return 0;
}


/*
void search_book(int sock) {
    char buffer[BUFFER_SIZE] = {0};
    printf("Enter book name to search: ");
    scanf("%s", buffer);
    send(sock, buffer, strlen(buffer), 0);
    while (read(sock, buffer, BUFFER_SIZE) > 0) {
        printf("%s", buffer);
    }
}
*/

/*
void search_book(int sock) {
    char buffer[BUFFER_SIZE] = {0};
    char search_query[BUFFER_SIZE] = {0};

    printf("Enter book name to search: ");
    scanf(" %[^\n]", search_query);

    send(sock, search_query, strlen(search_query), 0);

    // Clear buffer and read server response
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_read = read(sock, buffer, BUFFER_SIZE);
    if (bytes_read > 0) {
        printf("%s", buffer);
    } else {
        printf("No response from server\n");
    }
}
*/

/*
void search_book(int sock) {
    char buffer[BUFFER_SIZE] = {0};
    char search_query[BUFFER_SIZE] = {0};

    printf("Enter book name to search: ");
    scanf(" %[^\n]", search_query);

    send(sock, search_query, strlen(search_query), 0);

    // Clear buffer and read server response
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_read = read(sock, buffer, BUFFER_SIZE);
    if (bytes_read > 0) {
        printf("Server response: %s\n", buffer);
    } else {
        printf("No response from server\n");
    }
}
*/

void search_book(int sock) {
    char buffer[BUFFER_SIZE] = {0};
    char search_query[BUFFER_SIZE] = {0};

    printf("Enter book name to search: ");
    scanf(" %[^\n]", search_query);

    send(sock, search_query, strlen(search_query), 0);

    // Clear buffer and read server response
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_read = read(sock, buffer, BUFFER_SIZE);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the received data
        printf("Server response: %s\n", buffer);
    } else {
        printf("No response from server\n");
    }
}
