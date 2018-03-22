
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 6000

void error(const char *msg) {
    perror(msg);
    exit(0);
}

void recv_msg(int sockfd, char * msg) {
    /* All messages are 3 bytes. */
    memset(msg, 0, 4);
    int n = read(sockfd, msg, 3);

    if (n < 0 || n != 3) /* Not what we were expecting. Server got killed or the other client disconnected. */
        error("ERROR reading message from server socket.");
}

int recv_int(int sockfd) {
    int msg = 0;
    int n = read(sockfd, &msg, sizeof(int));

    if (n < 0 || n != sizeof(int))
        error("ERROR reading int from server socket");

    return msg;
}

void write_server_int(int sockfd, int msg) {
    int n = write(sockfd, &msg, sizeof(int));
    if (n < 0)
        error("ERROR writing int to server socket");
}

int connect_to_server(char * hostname, int portno) {
    struct sockaddr_in serv_addr;
    struct hostent *server;

    /* Get a socket. */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        error("ERROR opening socket for server.");

    /* Get the address of the server. */
    server = gethostbyname(hostname);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

	/* Zero out memory for server info. */
	memset(&serv_addr, 0, sizeof(serv_addr));

	/* Set up the server info. */
    serv_addr.sin_family = AF_INET;
    memmove(server->h_addr, &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

	/* Make the connection. */
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting to server");

    return sockfd;
}

void draw_board(char board[][3]) {
    printf(" %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);
}

void take_turn(int sockfd) {
    char buffer[10];

    while (1) { /* Ask until we receive. */
        printf("Enter 0-8 to make a move, or 9 for number of active players: ");
	    fgets(buffer, 10, stdin);
	    int move = buffer[0] - '0';
        if (move <= 9 && move >= 0){
            printf("\n");
            /* Send players move to the server. */
            write_server_int(sockfd, move);
            break;
        }
        else
            printf("\nInvalid input. Try again.\n");
    }
}

void get_update(int sockfd, char board[][3]) {
    /* Get the update. */
    int player_id = recv_int(sockfd);
    int move = recv_int(sockfd);

    /* Update the game board. */
    board[move/3][move%3] = player_id ? 'X' : 'O';
}



int main(int argc, char const *argv[]) {
  int sockfd = connect_to_server("localhost", PORT)

  char msg[4];
  char board[3][3] = { {' ', ' ', ' '}, /* Game board */
                       {' ', ' ', ' '},
                       {' ', ' ', ' '} };

  while (1) {
    recv_msg(sockfd, msg);

    if (!strcmp(msg, "TRN")) {
      printf("Your move...\n");
      take_turn(sockfd);
    } else if (!strcmp(msg, "INV")) {
      printf("That position has already been played. Try again.\n");
    } else if (!strcmp(msg, "UPD")) { /* Server is sending a game board update. */
            get_update(sockfd, board);
            draw_board(board);
    } else if (!strcmp(msg, "WAT")) { /* Wait for other player to take a turn. */
            printf("Waiting for other players move...\n");
    } else if (!strcmp(msg, "WIN")) { /* Winner. */
            printf("You win!\n");
            break;
    }
    else if (!strcmp(msg, "LSE")) { /* Loser. */
        printf("You lost.\n");
        break;
    } else if (!strcmp(msg, "DRW")) { /* Game is a draw. */
        printf("Draw.\n");
        break;
    } else /* Weird... */
        error("Unknown message.");
  }


    return 0;
}
