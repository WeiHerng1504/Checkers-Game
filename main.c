/* Program to print and play checker games.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2021, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: Wei Herng Moo Yong 1239979
  Dated:     2/10/2021

*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/* some #define's from my sample solution ------------------------------------*/
#define BOARD_SIZE          8       // board size
#define ROWS_WITH_PIECES    3       // number of initial rows with pieces
#define CELL_EMPTY          '.'     // empty cell character
#define CELL_BPIECE         'b'     // black piece character
#define CELL_WPIECE         'w'     // white piece character
#define CELL_BTOWER         'B'     // black tower character
#define CELL_WTOWER         'W'     // white tower character
#define COST_PIECE          1       // one piece cost
#define COST_TOWER          3       // one tower cost
#define TREE_DEPTH          3       // minimax tree depth
#define COMP_ACTIONS        10      // number of computed actions

#define NUM_WPIECES         ROWS_WITH_PIECES * BOARD_SIZE / 2
#define NUM_BPIECES         ROWS_WITH_PIECES * BOARD_SIZE / 2
#define DIMENSION           2
#define DIRECTIONS          4
#define FIRST_COLUMN        'A'
#define ERR_MSG_MAXLEN      50    // maximum char count of error message
#define ERROR1              "Source cell is outside of the board."
#define ERROR2              "Target cell is outside of the board."
#define ERROR3              "Source cell is empty."
#define ERROR4              "Target cell is not empty."
#define ERROR5              "Source cell holds opponent's piece/tower."
#define ERROR6              "Illegal action."
#define TRUE                1
#define FALSE               0
#define CAPTURE             'C'     // a normal move
#define NORMAL              'N'     // a capture
#define INITIAL             2       // initial size of pointer


/* one type definition from my sample solution -------------------------------*/
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type
typedef struct node node_t;

typedef struct {
    board_t game_board;
    int board_cost;
    int turn_num;
    int bpiece_count;
    int wpiece_count;
    int btower_count;
    int wtower_count;
} game_instance_t;

typedef struct {
    char move_type;
    int src_row;
    int src_col;
    int tgt_row;
    int tgt_col;
} move_made_t;

struct node {
    game_instance_t game;
    move_made_t latest;
    int propagated_cost;
    int num_possible;
    int total_possible;
    node_t* next_level;
};

void initialise_board(game_instance_t* game);
void print_board(game_instance_t* game);
void print_line_break();
void print_turn_break();
void do_moves(game_instance_t* game, char* command);
void set_piece(game_instance_t* game, int row, int col);
int print_error(char error_msg[]);
int read_move(move_made_t* latest, char* command);
void print_stats(game_instance_t* game, move_made_t* latest, int is_command);
int is_legal(move_made_t* latest, game_instance_t* game, int is_input);
void move_piece(move_made_t* latest, game_instance_t* game);
int is_legal_move(move_made_t* latest, game_instance_t* game, int is_input,
                  char* err_msg);
void do_A_command(game_instance_t* game, char* winner);
void do_P_command(game_instance_t* game, char* winner);
void free_tree(node_t *node);
int minimax(node_t* node);
void construct_next_level(node_t* node, int depth);
void compute_possible(node_t* node, int row, int col);
int calculate_costs(node_t* node);
void append(node_t* node, move_made_t test_move);
void print_winner(char* winner);

int
main(int argc, char *argv[]) {
    // YOUR IMPLEMENTATION OF STAGES 0-2
    game_instance_t game;
    char command = '0';
    char winner = '0';

    initialise_board(&game);

    // initial board layout and info
    printf("BOARD SIZE: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
    printf("#BLACK PIECES: %d\n", game.bpiece_count);
    printf("#WHITE PIECES: %d\n", game.wpiece_count);
    print_board(&game);

    do_moves(&game, &command);

    // carry out commands
    if (command == 'A') {
        do_A_command(&game, &winner);
        print_winner(&winner);
    }
    else if (command == 'P') {
        do_P_command(&game, &winner);
        print_winner(&winner);
    }

    // algorithms are fun
    return EXIT_SUCCESS;            // exit program with the success code
}

void print_winner(char* winner) {
    if (*winner == CELL_BTOWER) {
        printf("BLACK WIN!!!");
    }
    else if (*winner == CELL_WTOWER) {
        printf("WHITE WIN!!!");
    }
}

void do_P_command(game_instance_t* game, char* winner) {
    int i;
    for (i = 0; i < COMP_ACTIONS; i++) {
        do_A_command(game, winner);

        // check if winner decided
        if (*winner == CELL_BTOWER || *winner == CELL_WTOWER) {
            break;
        }

        game->turn_num += 1;
    }
}

void do_A_command(game_instance_t* game, char* winner) {
    node_t root;
    int preferred_score;
    move_made_t optimal;

    // initialize root
    root.game = *game;
    root.total_possible = INITIAL;
    root.num_possible = 0;
    root.next_level = (node_t*)malloc(INITIAL*sizeof(node_t));
    assert(root.next_level);

    // construct decision tree
    construct_next_level(&root, 1);

    // find best score based on minimax
    preferred_score = calculate_costs(&root);

    // make move towards best score
    if (root.num_possible != 0) {
        for (int i = 0; i < root.num_possible; i++) {
            if (root.next_level[i].propagated_cost == preferred_score) {
                optimal = root.next_level[i].latest;
                break;
            }
        }

        move_piece(&optimal, game);
        print_turn_break();
        print_stats(game, &optimal, TRUE);
        print_board(game);
    }
    // winner decided in current state
    else {
        if (game->turn_num % 2 == 1) {
            *winner = CELL_WTOWER; // white winner
        }
        else {
            *winner = CELL_BTOWER;
        }
    }

    free_tree(&root);
}

// free all malloc'd memory
void free_tree(node_t *node) {
    if (node->num_possible == 0) {
        free(node->next_level);
    }
    else {
        for (int i = 0; i < node->num_possible; i++) {
            free_tree(&node->next_level[i]);
        }
        free(node->next_level);
    }
}

// returns propagated cost based on minimax rule
int minimax(node_t* node) {
    int board_score;

    // black, prefer highest
    if (node->game.turn_num % 2 == 1) {
        board_score = INT_MIN;
        for (int i = 0; i < node->num_possible; i++) {

            // higher score found, take note
            if (node->next_level[i].propagated_cost > board_score) {
                board_score = node->next_level[i].propagated_cost;
            }
        }
    }
    // white, prefer lowest
    else {
        board_score = INT_MAX;
        for (int i = 0; i < node->num_possible; i++) {

            // lower score found, take note
            if (node->next_level[i].propagated_cost < board_score) {
                board_score = node->next_level[i].propagated_cost;
            }
        }
    }

    return board_score;
}

// recursively calculates propagated costs
int calculate_costs(node_t* node) {
    // last level nodes
    if (node->num_possible == 0) {
        return node->game.board_cost;
    }

    // not last level, propagate
    for (int i = 0; i < node->num_possible; i++) {
        node->next_level[i].propagated_cost =
        calculate_costs(&node->next_level[i]);
    }

    return minimax(node);
}

// construct tree recursively
void construct_next_level(node_t* node, int depth) {
    // last level, stop
    if (depth > TREE_DEPTH) {
        return;
    }

    // construct next level
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {

            //if cell is empty, skip
            if (node->game.game_board[row][col] == CELL_EMPTY) {
                continue;
            }

            //compute all possible moves, append to node
            compute_possible(node, row, col);
        }
    }

    // no further moves possible
    if (node->num_possible == 0) {
        if (node->game.turn_num % 2 == 1) {
            node->game.board_cost = INT_MIN;
            return;
        }
        else {
            node->game.board_cost = INT_MAX;
            return;
        }
    }

    // construct next level
    if (node->num_possible > 0) {
        for (int i = 0; i < node->num_possible; i++) {
            construct_next_level(&node->next_level[i], depth + 1);
        }
    }
}

// check for all possible moves, append if exist
void compute_possible(node_t* node, int row, int col) {

    int directions[DIRECTIONS][DIMENSION] = {
        {-1, 1}, // north east
        {1, 1},  // south east
        {1, -1}, // south west
        {-1, -1} // north west
    };

    // set test move
    move_made_t test_move;
    test_move.src_col = col;
    test_move.src_row = row;

    // check if normal move possible
    for (int i = 0; i < 4; i++) {
        test_move.move_type = NORMAL;
        test_move.tgt_col = col + directions[i][1];
        test_move.tgt_row = row + directions[i][0];

        if (is_legal(&test_move, &node->game, FALSE) == TRUE) {
            append(node, test_move);
        }
        // check if capture move possible
        else {
            test_move.move_type = CAPTURE;
            test_move.tgt_col = col + 2*directions[i][1];
            test_move.tgt_row = row + 2*directions[i][0];

            if (is_legal(&test_move, &node->game, FALSE) == TRUE) {
                append(node, test_move);
            }
        }
    }
}

// append a new board to current one
void append(node_t* node, move_made_t test_move) {
    node_t new_node;

    //initialize a new node
    new_node.latest = test_move;
    new_node.game = node->game;
    new_node.num_possible = 0;
    new_node.total_possible = INITIAL;
    new_node.next_level = (node_t*)malloc(INITIAL*sizeof(node_t));

    assert(new_node.next_level);

    // make the move
    move_piece(&test_move, &new_node.game);
    new_node.game.turn_num += 1;

    // check array is big enough
    if (node->num_possible == node->total_possible) {
        node->total_possible *= 2;
        node->next_level = (node_t*)realloc(node->next_level,
                                        node->total_possible*sizeof(node_t));
        assert(node->next_level);
    }

    // append
    node->next_level[node->num_possible] = new_node;
    node->num_possible += 1;
}

void initialise_board(game_instance_t* game) {
    int row;
    int col;

    // starting state
    game->bpiece_count = NUM_BPIECES;
    game->wpiece_count = NUM_WPIECES;
    game->turn_num = 1;
    game->board_cost = 0;
    game->wtower_count = 0;
    game->btower_count = 0;

    //fill starting positions
    for (row = 0; row < ROWS_WITH_PIECES; row++) {
        for (col = 0; col < BOARD_SIZE; col++) {
            set_piece(game, row, col);
        }
    }

    //fill empty rows in middle
    for (row = ROWS_WITH_PIECES; row < BOARD_SIZE - ROWS_WITH_PIECES; row++) {
        for (col = 0; col < BOARD_SIZE; col++) {
            game->game_board[row][col] = CELL_EMPTY;
        }
    }
}

// puts the different pieces in the right place
void set_piece(game_instance_t* game, int row, int col) {
    if (row % 2 == 0) {
        game->game_board[row][col] = (col % 2 == 0) ? CELL_EMPTY : CELL_WPIECE;
        game->game_board[BOARD_SIZE - row - 1][col] = (col % 2 == 0) ?
                                                    CELL_BPIECE : CELL_EMPTY;
    }
    else {
        game->game_board[row][col] = (col % 2 == 0) ? CELL_WPIECE : CELL_EMPTY;
        game->game_board[BOARD_SIZE - row - 1][col] = (col % 2 == 0) ?
                                                    CELL_EMPTY : CELL_BPIECE;
    }
}

void print_board(game_instance_t* game) {
    int col, row;
    printf("  ");

    // column labels
    for (col = 0; col < BOARD_SIZE; col++) {
        printf("   %c", FIRST_COLUMN + col);
    }

    print_line_break();

    // rows and pieces
    for (row = 0; row < BOARD_SIZE; row++) {
        printf("%2d |", row + 1);
        for (col = 0; col < BOARD_SIZE; col++) {
            printf(" %c |", game->game_board[row][col]);
        }

        print_line_break();
    }
}

void print_line_break() {
    int i;
    printf("\n   ");
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("+---");
    }
    printf("+\n");
}

void print_turn_break() {
    int i;
    printf("===");
    for (i = 0; i < BOARD_SIZE; i++) {
        printf("====");
    }
    printf("==\n");
}

// read, check and do input
void do_moves(game_instance_t* game, char* command) {

    move_made_t latest;

    while (read_move(&latest, command) == TRUE) {

        // move not possible
        if (is_legal(&latest, game, TRUE) == FALSE) {
            exit(EXIT_FAILURE);
        }

        print_turn_break();
        move_piece(&latest, game);
        print_stats(game, &latest, FALSE);
        print_board(game);

        game->turn_num += 1;
    }
}

// header of each game state
void print_stats(game_instance_t* game, move_made_t* latest, int is_command) {
    if (is_command == TRUE) {
        printf("*** ");
    }

    if (game->turn_num % 2 == 1) {
        printf("BLACK ACTION #%d: ", game->turn_num);
    }
    else {
        printf("WHITE ACTION #%d: ", game->turn_num);
    }

    printf("%c%d-%c%d\n", latest->src_col + FIRST_COLUMN, latest->src_row + 1,
           latest->tgt_col + FIRST_COLUMN, latest->tgt_row + 1);
    printf("BOARD COST: %d\n", game->board_cost);
}

// make move
void move_piece(move_made_t* latest, game_instance_t* game) {
    char piece_captured;

    int row_change;
    int col_change;

    // moving piece
    game->game_board[latest->tgt_row][latest->tgt_col] =
                        game->game_board[latest->src_row][latest->src_col];
    game->game_board[latest->src_row][latest->src_col] = CELL_EMPTY;

    // make a capture
    if (latest->move_type == CAPTURE) {
        col_change = latest->tgt_col - latest->src_col;
        row_change = latest->tgt_row - latest->src_row;

        piece_captured =
                game->game_board[latest->src_row + row_change / 2]
                    [latest->src_col + col_change / 2];

        // what was captured?, update
        if (piece_captured == CELL_BPIECE) {
            game->bpiece_count -= 1;
        }
        else if (piece_captured == CELL_WPIECE) {
            game->wpiece_count -= 1;
        }
        else if (piece_captured == CELL_BTOWER) {
            game->btower_count -= 1;
        }
        else {
            game->wtower_count -= 1;
        }
        game->game_board[latest->src_row + row_change / 2]
                        [latest->src_col + col_change / 2] = CELL_EMPTY;
    }

    //piece promotion
    if (game->game_board[latest->tgt_row][latest->tgt_col] == CELL_BPIECE &&
        latest->tgt_row == 0) {

        game->game_board[latest->tgt_row][latest->tgt_col] = CELL_BTOWER;
        game->bpiece_count -= 1;
        game->btower_count += 1;

    }
    else if (game->game_board[latest->tgt_row][latest->tgt_col]
             == CELL_WPIECE && latest->tgt_row == BOARD_SIZE - 1) {

        game->game_board[latest->tgt_row][latest->tgt_col] = CELL_WTOWER;
        game->wpiece_count -= 1;
        game->wtower_count += 1;

    }

    // update board cost
    game->board_cost = COST_TOWER * (game->btower_count - game->wtower_count)
                    + COST_PIECE * (game->bpiece_count - game->wpiece_count);
}

// check if move is legal
int is_legal(move_made_t* latest, game_instance_t* game, int is_input) {
    char err_msg[ERR_MSG_MAXLEN];
    int legal_flag = TRUE;

    // check move description errors
    if (latest->src_row >= BOARD_SIZE || latest->src_col >= BOARD_SIZE
        || latest->src_row < 0 || latest->src_col < 0) {
        legal_flag = FALSE;
        strcpy(err_msg, ERROR1);
    }
    else if (latest->tgt_row >= BOARD_SIZE || latest->tgt_col >= BOARD_SIZE
             || latest->tgt_row < 0 || latest->tgt_col < 0) {
        legal_flag = FALSE;
        strcpy(err_msg, ERROR2);
    }
    else if (game->game_board[latest->src_row][latest->src_col] == CELL_EMPTY) {
        legal_flag = FALSE;
        strcpy(err_msg, ERROR3);
    }
    else if (game->game_board[latest->tgt_row][latest->tgt_col] != CELL_EMPTY) {
        legal_flag = FALSE;
        strcpy(err_msg, ERROR4);
    }
    else if ((game->turn_num % 2 == 0 &&
              tolower(game->game_board[latest->src_row][latest->src_col])
              != CELL_WPIECE) || (game->turn_num % 2 == 1 &&
                 tolower(game->game_board[latest->src_row][latest->src_col])
                                  != CELL_BPIECE)) {
        legal_flag = FALSE;
        strcpy(err_msg, ERROR5);
    }
    else {
        // check move errors
        legal_flag = is_legal_move(latest, game, is_input, err_msg);
    }


    if (is_input == TRUE && legal_flag == FALSE) {
        printf("ERROR: %s\n", err_msg);
    }

    return legal_flag;
}

// checks for any illegal actions
int is_legal_move(move_made_t* latest, game_instance_t* game, int is_input,
                  char* err_msg) {

    char current_piece = game->game_board[latest->src_row][latest->src_col];
    int row_change = latest->tgt_row - latest->src_row;
    int col_change = latest->tgt_col - latest->src_col;
    int too_far;            //move further than possible
    int invalid_backwards;  //move backward when not a tower
    int invalid_direction;  // not diagonal
    int invalid_capture;    // nothing to capture

    too_far = (abs(row_change) > 2);

    invalid_backwards = ((current_piece == CELL_BPIECE && row_change > 0) ||
                         (current_piece == CELL_WPIECE && row_change < 0));

    invalid_capture = (abs(row_change) == 2 &&
                           game->game_board[latest->src_row + row_change / 2]
                           [latest->src_col + col_change / 2] == CELL_EMPTY);

    invalid_direction = (row_change == 0 || col_change == 0);

    if (too_far || invalid_backwards ||invalid_capture || invalid_direction) {
        strcpy(err_msg, ERROR6);
        return FALSE;
    }
    return TRUE;
}

// reads and stores move input
int read_move(move_made_t* latest, char* command) {
    // temp values for reading
    int sr, tr;
    char sc, tc;

    // input is move
    if (scanf("%c%d-%c%d", &sc, &sr, &tc, &tr) == 4 && getchar() != EOF) {
        latest->src_col = sc - FIRST_COLUMN;
        latest->src_row = sr - 1;
        latest->tgt_col = tc - FIRST_COLUMN;
        latest->tgt_row = tr - 1;

        if (abs(sr - tr) == 2) {
            latest->move_type = CAPTURE;
        }
        else {
            latest->move_type = NORMAL;
        }
        return TRUE;
    }

    // input is command
    if (sc == 'A' || sc == 'P') {
        *command = sc;
    }

    return FALSE;
}

/* THE END -------------------------------------------------------------------*/
