#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void add_book(int sock);
void delete_book(int sock);
void modify_book(int sock);
void search_book(int sock);

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

    send(sock, "admin", strlen("admin"), 0);

    while (1) {
        printf("Enter command (add/delete/modify/search/exit): ");
        scanf("%s", buffer);

        send(sock, buffer, strlen(buffer), 0);
        if (strcmp(buffer, "add") == 0) {
            add_book(sock);
        } else if (strcmp(buffer, "delete") == 0) {
            delete_book(sock);
        } else if (strcmp(buffer, "modify") == 0) {
            modify_book(sock);
        } else if (strcmp(buffer, "search") == 0) {
            search_book(sock);
        } else if (strcmp(buffer, "exit") == 0) {
            break;
        }
    }

    close(sock);
    return 0;
}

/*
void add_book(int sock) {
    char buffer[BUFFER_SIZE] = {0};
    printf("Enter book details: ");
    scanf(" %[^\n]", buffer);
    send(sock, buffer, strlen(buffer), 0);
}
*/

void add_book(int sock) {
    char buffer[BUFFER_SIZE] = {0};
    char book_details[BUFFER_SIZE] = {0};

    // Gather book details in one line
    printf("Enter book details (title, author, year): ");
    scanf(" %[^\n]", book_details);

    // Send the "add" command to the server
    snprintf(buffer, BUFFER_SIZE, "add");
    send(sock, buffer, strlen(buffer), 0);
    
    // Clear buffer and send the actual book details
    memset(buffer, 0, BUFFER_SIZE);
    snprintf(buffer, BUFFER_SIZE, "%s", book_details);
    send(sock, buffer, strlen(buffer), 0);
    
    // Read server response
    memset(buffer, 0, BUFFER_SIZE);
    if (read(sock, buffer, BUFFER_SIZE) > 0) {
        printf("Server response: %s\n", buffer);
    }
}

void delete_book(int sock) {
    char buffer[BUFFER_SIZE] = "Delete book functionality\n";
    send(sock, buffer, strlen(buffer), 0);
}

void modify_book(int sock) {
    char buffer[BUFFER_SIZE] = "Modify book functionality\n";
    send(sock, buffer, strlen(buffer), 0);
}

void search_book(int sock) {
    char buffer[BUFFER_SIZE] = {0};
    printf("Enter book name to search: ");
    scanf("%s", buffer);
    send(sock, buffer, strlen(buffer), 0);
    while (read(sock, buffer, BUFFER_SIZE) > 0) {
        printf("%s", buffer);
    }
}