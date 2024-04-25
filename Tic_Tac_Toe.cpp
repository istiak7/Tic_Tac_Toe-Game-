#include<bits/stdc++.h>
#include <unistd.h>
#define WIN 100
#define	DRAW 0
#define LOSS -100
#define AI_MARKER 'O'
#define PLAYER_MARKER 'X'
#define EMPTY_SPACE '-'
#define NINF -10000
#define PINF  10000
using namespace std;
vector<vector<pair<int, int>>> winning_states;

// Print the current board state
void print_board(char grid[3][3])
{
    cout << endl;
    cout << grid[0][0] << " | " << grid[0][1] << " | " << grid[0][2] << endl;
    cout << "----------" << endl;
    cout << grid[1][0] << " | " << grid[1][1] << " | " << grid[1][2] << endl;
    cout << "----------" << endl;
    cout << grid[2][0] << " | " << grid[2][1] << " | " << grid[2][2] << endl;
    cout << endl;
}

// Get all available legal moves (spaces that are not occupied)
vector<pair<int, int>> get_legal_moves(char board[3][3])
{
    vector<pair<int, int>> legal_moves;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] != AI_MARKER && board[i][j] != PLAYER_MARKER)
            {
                legal_moves.push_back(make_pair(i, j));
            }
        }
    }

    return legal_moves;
}
// All possible winning states
void Win_state()
{
    winning_states =
    {
        // Every row
        { make_pair(0, 0), make_pair(0, 1), make_pair(0, 2) },
        { make_pair(1, 0), make_pair(1, 1),make_pair(1, 2) },
        { make_pair(2, 0), make_pair(2, 1), make_pair(2, 2) },

        // Every column
        { make_pair(0, 0), make_pair(1, 0), make_pair(2, 0) },
        { make_pair(0, 1), make_pair(1, 1), make_pair(2, 1) },
        { make_pair(0, 2), make_pair(1, 2), make_pair(2, 2) },

        // Every diagonal
        { make_pair(0, 0), make_pair(1, 1), make_pair(2, 2) },
        { make_pair(2, 0), make_pair(1, 1), make_pair(0, 2) }
    };
}
// Get all board positions occupied by the given marker
vector<pair<int, int>> get_occupied_positions(char board[3][3], char marker)
{
    vector<pair<int, int>> occupied_positions;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (marker == board[i][j])
            {
                occupied_positions.push_back(std::make_pair(i, j));
            }
        }
    }

    return occupied_positions;
}

// Check if the board is full
bool board_is_full(char board[3][3])
{
    vector<pair<int, int>> legal_moves = get_legal_moves(board);

    if (0 == legal_moves.size())
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Check if the game has been won
bool game_is_won(vector<pair<int, int>> occupied_positions)
{
    bool game_won;

    for (int i = 0; i < winning_states.size(); i++)
    {
        game_won = true;
        vector<pair<int, int>> curr_win_state = winning_states[i];
        for (int j = 0; j < 3; j++)
        {
            if (!(find(begin(occupied_positions), end(occupied_positions), curr_win_state[j]) != end(occupied_positions)))
            {
                game_won = false;
                break;
            }
        }
        if (game_won)
        {
            break;
        }
    }
    return game_won;
}

char get_opponent_marker(char marker)
{
    char opponent_marker;
    if (marker == PLAYER_MARKER)
    {
        opponent_marker = AI_MARKER;
    }
    else
    {
        opponent_marker = PLAYER_MARKER;
    }

    return opponent_marker;
}

// Check if someone has won or lost
int get_board_state(char board[3][3], char marker)
{
    char opponent_marker = get_opponent_marker(marker);
    vector<pair<int, int>> occupied_positions = get_occupied_positions(board, marker);

    bool is_won = game_is_won(occupied_positions);
    if (is_won)
    {
        return WIN;
    }
    occupied_positions = get_occupied_positions(board, opponent_marker);
    bool is_lost = game_is_won(occupied_positions);

    if (is_lost)
    {
        return LOSS;
    }

    bool is_full = board_is_full(board);
    if (is_full)
    {
        return DRAW;
    }
    return DRAW;
}

// Apply the minimax game optimization algorithm
pair<int,pair<int, int>> minimax_optimization(char board[3][3], char marker, int depth, int alpha, int beta)
{
    // Initialize best move
    pair<int, int> best_move = make_pair(-1, -1);
    int best_score = (marker == AI_MARKER) ? LOSS : WIN;

    // terminal state, return the best score and move
    if (board_is_full(board) || DRAW != get_board_state(board, AI_MARKER))
    {
        best_score = get_board_state(board, AI_MARKER);
        return make_pair(best_score, best_move);
    }

    vector<pair<int, int>> legal_moves = get_legal_moves(board);

    for (int i = 0; i < legal_moves.size(); i++)
    {
        pair<int, int> curr_move = legal_moves[i];
        board[curr_move.first][curr_move.second] = marker;

        // Maximizing player
        if (marker == AI_MARKER)
        {
            pair<int,pair<int,int>>PAIR =  minimax_optimization(board, PLAYER_MARKER, depth + 1, alpha, beta);
            int score = PAIR.first;

            // Get the best scoring move
            if (best_score < score)
            {
                best_score = score - depth * 10;
                best_move = curr_move;
                alpha = max(alpha, best_score);
                board[curr_move.first][curr_move.second] = EMPTY_SPACE;
                if (beta <= alpha)
                {
                    break;
                }
            }
        }
        // Minimizing opponent
        else
        {
            pair<int,pair<int,int>>PAIR = minimax_optimization(board, AI_MARKER, depth + 1, alpha, beta);
            int score = PAIR.first;

            if (best_score > score)
            {
                best_score = score + depth * 10;
                best_move = curr_move;
                beta = min(beta, best_score);
                board[curr_move.first][curr_move.second] = EMPTY_SPACE;
                if (beta <= alpha)
                {
                    break;
                }
            }
        }

        board[curr_move.first][curr_move.second] = EMPTY_SPACE; // Undo move
    }

    return make_pair(best_score, best_move);
}

// Check if the game is finished
bool game_is_done(char board[3][3])
{
    if (board_is_full(board))
    {
        return true;
    }

    if (DRAW != get_board_state(board, AI_MARKER))
    {
        return true;
    }

    return false;
}

int main()
{
    Win_state();
    char Grid[3][3];

    cout << "\n\n\tTic Tac Toe Using MinMax Algorithm\n" << endl <<"....................................................."<< endl;
    cout << "Player vs computer = 0\t Computer vs computer= 1" << endl << endl;
    cout << "please choose your MOD : ";
    int x ;
    cin >> x ;
    if(x == 0)
    {
        while (game_is_done(Grid)==false)
        {
            cout << "Enter Postion : ";
            int x ;
            cin >> x ;
            int row, col;
            if(x > 9 || x < 1)
            {
                cout << "Invalid position.\n\n";
                continue;
            }
            if(x == 1)row = 0, col = 0 ;
            else if(x ==2)row = 0, col = 1 ;
            else if(x == 3)row = 0, col = 2 ;
            else if(x==4)row = 1, col = 0 ;
            else if(x==5)row = 1, col = 1 ;
            else if(x==6)row = 1, col= 2 ;
            else if(x==7)row = 2, col = 0 ;
            else if(x==8)row = 2, col = 1 ;
            else row = 2, col = 2 ;
            cout << endl << endl;

            /// input position ki occupaid or not ?
            vector<pair<int, int>> legal_moves = get_legal_moves(Grid);
            bool flag = false;

            ///for valid
            for (int i = 0; i < legal_moves.size(); i++)
            {
                if (row == legal_moves[i].first && col == legal_moves[i].second)
                {
                    Grid[row][col] = PLAYER_MARKER;
                    flag = true;
                    break;
                }
            }
            ///for invalid
            if (flag == false)
            {
                cout << "Already played this position.Please try to another position" << endl;
                continue;
            }
            pair<int,pair<int, int>> AI = minimax_optimization(Grid, AI_MARKER, 0, NINF, PINF);

            Grid[AI.second.first][AI.second.second] = AI_MARKER;

            print_board(Grid);
        }
    }
    else if(x == 1)
    {
        while (game_is_done(Grid)==false)
        {
            pair<int,pair<int, int>>AI;
            AI = minimax_optimization(Grid, PLAYER_MARKER, 0, NINF, PINF);
            Grid[AI.second.first][AI.second.second] = PLAYER_MARKER;
            print_board(Grid);
            sleep(1);
            AI = minimax_optimization(Grid, AI_MARKER, 0, NINF, PINF);
            Grid[AI.second.first][AI.second.second] = AI_MARKER;
            print_board(Grid);
            sleep(1);
        }
    }

    cout << "GAME IS OVER "<< endl << endl;

    int player_state = get_board_state(Grid, PLAYER_MARKER);

    cout << "YOU ";
    if (WIN == player_state)
    {
        cout << "WIN" << endl;
    }
    else if (DRAW == player_state)
    {
        cout << "DRAW" << endl;
    }
    else if (LOSS == player_state)
    {
        cout << "LOSS" << endl;
    }
    return 0;
}

