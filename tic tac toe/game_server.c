/****************************************************************************
*       Simple Tic Tac Toe Game server built using shared 
*       memory and semaphores for synchronisation of multiple
*       processes.
*
*       Usage : ./server.out <any port number>
*       
*       GROUP NO :  17
*       Roll no 15/CS/16 : Amit Sharma.
*       Roll no 15/CS/19 : Aman Kumar Sah.
*
*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define WAIT(s)  semop(s,&pop,1)
#define SIGNAL(s)  semop(s,&vop,1)
#define BUFF_SIZE 256

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/* Reads an int from a client socket. */
int recvInt(int cli_sockfd) {
    int msg = 0;
    int n = read(cli_sockfd, &msg, sizeof(int));

    if (n < 0 || n != sizeof(int)) /* Not what we were expecting. Client likely disconnected. */
        return -1;

    return msg;
}

/* Writes a message to a client socket. */
void writeClientMsg(int cli_sockfd, char * msg) {
    int n = write(cli_sockfd, msg, strlen(msg));
    if (n < 0)
        error("ERROR writing msg to client socket");
}

/* Writes an int to a client socket. */
void writeClientInt(int cli_sockfd, int msg) {
    int n = write(cli_sockfd, &msg, sizeof(int));
    if (n < 0)
        error("ERROR writing int to client socket");
}

/* Writes a message to both client sockets. */
void writeClientsMsg(int *cli_sockfd, char * msg) {
    writeClientMsg(cli_sockfd[0], msg);
    writeClientMsg(cli_sockfd[1], msg);
}

/* Writes an int to both client sockets. */
void writeClientsInt(int * cli_sockfd, int msg) {
    writeClientInt(cli_sockfd[0], msg);
    writeClientInt(cli_sockfd[1], msg);
}

int setupListener(int portno) {
    int sockfd;
    struct sockaddr_in serv_addr;
    int option = 1;
    /* Get a socket to listen on */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
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
    writeClientMsg(cli_sockfd, "TRN");

    /* Get players move. */
    return recvInt(cli_sockfd);
}

int checkMove(char board[][3], int move, int player_id) {
    if ((move == 9) || (board[move/3][move%3] == ' '))   /* Move is valid. */
        return 1;
   else /* Move is invalid. */
       return 0;
}

void updateBoard(char board[][3], int move, int player_id) {
    board[move/3][move%3] = player_id ? 'X' : 'O';
}

void drawBoard(char board[][3]) {
    printf(" %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
    printf("-----------\n");
    printf(" %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);
}

void sendBoard(int cli_sockfd, char board[][3]) {
  writeClientMsg(cli_sockfd, "UPD");
  int n = write(cli_sockfd, board, 9*sizeof(char));
  if (n < 0)
      error("ERROR writing int to client socket");

}

void sendUpdate(int cli_sockfd, int move, int player_id) {
    /* Signal an update */
    writeClientMsg(cli_sockfd, "UPD");

    /* Send the id of the player that made the move. */
    writeClientInt(cli_sockfd, player_id);

    /* Send the move. */
    writeClientInt(cli_sockfd, move);
}

int checkBoard(char board[][3], int last_move) {
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
void runGame(int cli_sockfd, int player_id, int sem[], char board[][3]) {
  struct sembuf pop = {0, 0, -1},
                vop = {0, 0, 1};

//   pop.sem_num=0;
//   vop.sem_num=0;
//   pop.sem_flg=0;
//   vop.sem_flg=0;
//   pop.sem_op=-1;
//   vop.sem_op=1;

  int game_over = 0;
  int turn_count = 0;
  int looser = 0;
  while (!game_over) {
    int valid = 0;
    int move = 0;
    WAIT(sem[player_id]);

    sendBoard(cli_sockfd, board);
    if (board[3][0] == 1)
      game_over = 2, valid = 1;
    else if(board[3][0] == 2)
      valid = 1, turn_count = 4;
    while (!valid) {

        move = getPlayerMove(cli_sockfd);
      if (move == -1)
        break;

      printf("Player %d played position %d\n", player_id+1, move);

      valid = checkMove(board, move, player_id);

      if (!valid) { /* Move was invalid. */
          printf("Move was invalid. Let's try this again...\n");
          writeClientMsg(cli_sockfd, "INV");
      }
    }
    if (move == -1) { /* Error reading from client. */
          printf("Player disconnected.\n");
          break;
    } else if(game_over == 2) {
      writeClientMsg(cli_sockfd, "LSE");
    } else if (turn_count == 4) { /* There have been nine valid moves and no winner, game is a draw. */
        printf("Draw.\n");
        writeClientMsg(cli_sockfd, "DRW");
        board[3][0] = 2;
        game_over = 1;
    } else {
      updateBoard(board, move, player_id);
      sendBoard( cli_sockfd, board);
       drawBoard(board);
       printf("checking board player %d count %d\n", player_id+1, turn_count);
       game_over = checkBoard(board, move);

        if (game_over == 1) { /* We have a winner. */
            board[3][0] = 1;
            writeClientMsg(cli_sockfd, "WIN");
            printf("Player %d won.\n", player_id+1);
        }

    }
    turn_count++;
    SIGNAL(sem[!player_id]);
}
}

void resetBoard(char board[][3]) {
  for (size_t i = 0; i < 3; i++) {
    for (size_t j = 0; j < 3; j++) {
      board[i][j] = ' ';
    }
  }
  board[3][0] = 0;
}

int main(int argc, char *argv[]) {

  if(argc < 2) {
      error("ERROR PORT required");
  }

  int server_sockfd = setupListener(strtol(argv[1], NULL, 10));
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  int sem[2];
  sem[0] = semget(IPC_PRIVATE,1,0777|IPC_CREAT);
  sem[1] = semget(IPC_PRIVATE,1,0777|IPC_CREAT);

  semctl(sem[0],0,SETVAL,0);
  semctl(sem[1],0,SETVAL,1);

  // char (*board)[3];
  int shmid = shmget(IPC_PRIVATE, 12*sizeof(char), 0777|IPC_CREAT);
  char (*board)[3] = shmat(shmid, 0, 0);




  while (1) {
    resetBoard(board);
    printf("Waiting for Player 1\n");
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

      runGame(player_1, 0, sem, board);

      printf("Player 1 Game Over!\n");
      close(player_1);
      exit(0);
    } else {
        resetBoard(board);
        int player_2 = accept(server_sockfd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        if (player_2 < 0)
          error("Player2 Accept Error");
        printf("Player2 connected at port: %d\n", ntohs(address.sin_port));
        runGame(player_2, 1, sem, board);
        printf("Player 2 Game Over!\n");
        close(player_2);
    }


  }
  shmdt(board);
  shmctl(shmid, IPC_RMID, 0);
  return 0;
}
