#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT 6000
#define BUFF_SIZE 256

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/* Reads an int from a client socket. */
int recv_int(int cli_sockfd) {
    int msg = 0;
    int n = read(cli_sockfd, &msg, sizeof(int));

    if (n < 0 || n != sizeof(int)) /* Not what we were expecting. Client likely disconnected. */
        return -1;

    return msg;
}

/* Writes a message to a client socket. */
void write_client_msg(int cli_sockfd, char * msg) {
    int n = write(cli_sockfd, msg, strlen(msg));
    if (n < 0)
        error("ERROR writing msg to client socket");
}

/* Writes an int to a client socket. */
void write_client_int(int cli_sockfd, int msg) {
    int n = write(cli_sockfd, &msg, sizeof(int));
    if (n < 0)
        error("ERROR writing int to client socket");
}

/* Writes a message to both client sockets. */
void write_clients_msg(int * cli_sockfd, char * msg) {
    write_client_msg(cli_sockfd[0], msg);
    write_client_msg(cli_sockfd[1], msg);
}

/* Writes an int to both client sockets. */
void write_clients_int(int * cli_sockfd, int msg) {
    write_client_int(cli_sockfd[0], msg);
    write_client_int(cli_sockfd[1], msg);
}

int setupListener(int portno) {
    int sockfd;
    struct sockaddr_in serv_addr;

    /* Get a socket to listen on */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening listener socket.");

    /* Zero out the memory for the server information */
    memset(&serv_addr, 0, sizeof(serv_addr));

	/* set up the server info */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Bind the server info to the listener socket. */
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR binding listener socket.");


    listen(sockfd, 2);
    /* Return the socket number. */
    return sockfd;
}

int getPlayerMove(int cli_sockfd) {
    /* Tell player to make a move. */
    write_client_msg(cli_sockfd, "TRN");

    /* Get players move. */
    return recv_int(cli_sockfd);
}

int check_move(char board[][3], int move, int player_id) {
    if ((move == 9) || (board[move/3][move%3] == ' '))   /* Move is valid. */
        return 1;
   else /* Move is invalid. */
       return 0;
}

void update_board(char board[][3], int move, int player_id) {
    board[move/3][move%3] = player_id ? 'X' : 'O';
}

void draw_board(char board[][3]) {
    printf(" %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);
}

void send_update(int * cli_sockfd, int move, int player_id) {
    /* Signal an update */
    write_clients_msg(cli_sockfd, "UPD");

    /* Send the id of the player that made the move. */
    write_clients_int(cli_sockfd, player_id);

    /* Send the move. */
    write_clients_int(cli_sockfd, move);
}

int check_board(char board[][3], int last_move) {
    int row = last_move/3;
    int col = last_move%3;

    if ( board[row][0] == board[row][1] && board[row][1] == board[row][2] ) { /* Check the row for a win. */
        return 1;
    }
    else if ( board[0][col] == board[1][col] && board[1][col] == board[2][col] ) { /* Check the column for a win. */
        return 1;
    }
    else if (!(last_move % 2)) { /* If the last move was at an even numbered position we have to check the diagonal(s) as well. */
        if ( (last_move == 0 || last_move == 4 || last_move == 8) && (board[1][1] == board[0][0] && board[1][1] == board[2][2]) ) {  /* Check backslash diagonal. */
            return 1;
        }
        if ( (last_move == 2 || last_move == 4 || last_move == 6) && (board[1][1] == board[0][2] && board[1][1] == board[2][0]) ) { /* Check frontslash diagonal. */
            return 1;
        }
    }
    /* No winner, yet. */
    return 0;
}



int main(int argc, char *argv[]) {

  int server_sockfd = setupListener(PORT);
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  while (1) {
    int player_1 = accept(server_sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    if (player_1 < 0)
      error("Player1 Accept Error");


    if (fork() == 0) {
      close(server_sockfd);
      printf("Player1 connected at port: %d\n", ntohs(address.sin_port));
      // printf("client address: %s\n", inet_ntoa(address.sin_addr));
      if (1) {
        printf("Waiting for player2...\n");
      }

      close(player_1);
      exit(0);
    } else {
        int player_2 = accept(server_sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        if (player_2 < 0)
          error("Player2 Accept Error");
        printf("Player2 connected at port: %d\n", ntohs(address.sin_port));

        close(player_2);
    }
  }

  return 0;
}
