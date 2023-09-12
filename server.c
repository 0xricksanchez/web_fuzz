#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_request(int client_socket, const uint8_t *data, size_t size) {
  if (size > BUFFER_SIZE - 1) {
    return;
  }
  char buffer[BUFFER_SIZE];
  memcpy(buffer, data, size);
  buffer[size] = '\0';

  char *response;
  char *header;

  if (strstr(buffer, "GET /safe") != NULL) {
    header = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n";
    response = "This is a safe endpoint.\n";
  } else if (strstr(buffer, "GET /unsafe") != NULL) {
    char *query = strchr(buffer, '?');
    if (query == NULL || query >= buffer + size) {
      header = "HTTP/1.1 400 Bad Request\nContent-Type: text/plain\n\n";
      response = "Invalid request.\n";
    } else {
      char sql[BUFFER_SIZE];
      int len =
          snprintf(sql, sizeof(sql),
                   "SELECT * FROM users WHERE username='%s' AND password='%s'",
                   query + 1, query + 1);
      if (len <= 0) {
        header =
            "HTTP/1.1 500 Internal Server Error\nContent-Type: text/plain\n\n";
        response = "Internal server error.\n";
      } else {
        header = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\n";
        response = "User authenticated successfully.\n";
      }
    }
  } else {
    header = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\n\n";
    response = "Endpoint not found.\n";
  }
  char full_response[BUFFER_SIZE];
  snprintf(full_response, sizeof(full_response), "%s%s", header, response);
  send(client_socket, full_response, strlen(full_response), 0);
}

// int main() {
//   int server_socket, client_socket;
//   struct sockaddr_in server_address, client_address;
//   socklen_t client_address_len = sizeof(client_address);
//
//   // Create a socket
//   server_socket = socket(AF_INET, SOCK_STREAM, 0);
//   if (server_socket == -1) {
//     perror("socket");
//     exit(1);
//   }
//
//   // Bind the socket to a specific address and port
//   server_address.sin_family = AF_INET;
//   server_address.sin_addr.s_addr = INADDR_ANY;
//   server_address.sin_port = htons(PORT);
//   if (bind(server_socket, (struct sockaddr *)&server_address,
//            sizeof(server_address)) == -1) {
//     perror("bind");
//     close(server_socket);
//     exit(1);
//   }
//
//   // Start listening for incoming connections
//   if (listen(server_socket, 5) == -1) {
//     perror("listen");
//     close(server_socket);
//     exit(1);
//   }
//
//   printf("Server started on port %d\n", PORT);
//
//   // Server loop
//   while (1) {
//     client_socket = accept(server_socket, (struct sockaddr *)&client_address,
//                            &client_address_len);
//     if (client_socket == -1) {
//       perror("accept");
//       continue;
//     }
//
//     handle_request(client_socket);
//     close(client_socket);
//   }
//
//   close(server_socket);
//   return 0;
// }
