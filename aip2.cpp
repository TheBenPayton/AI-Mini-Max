#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <time.h>
#include <iomanip>
#include <chrono>

using namespace std::chrono;
using namespace std;

#define WIN 1000
#define	DRAW 0
#define LOSS -1000

#define PLAYER_1 'O'
#define PLAYER_2 'X'
#define EMPTY_SPACE '-'

#define START_DEPTH 0
#define MAX_DEPTH 6
// Print game state

//Globals for user evaluation function choice (1-4)
int userAlgoChoicePlayerOne;
int userAlgoChoicePlayerTwo;
//Values that will track metrics for nodes expanded and eval metrics throughout the running of program:
int nodesExpanded_1 = 0;
int nodesExpanded_2 = 0;
int evalOneWins = 0;
int evalTwoWins = 0;
int evalThreeWins = 0;
int evalFourWins = 0;
int draws = 0;
int gamesPlayed = 0;
long percentWon;

void print_game_state(int state)
{
	if (WIN == state) { cout << "WIN" << endl; }
	else if (DRAW == state) { cout << "DRAW" << endl; }
	else if (LOSS == state) { cout << "LOSS" << endl; }
}

// All possible winning states
vector<vector<pair<int, int> > > winning_states
{
	// Every row
	{ make_pair(0, 0), make_pair(0, 1), make_pair(0, 2) },
	{ make_pair(1, 0), make_pair(1, 1), make_pair(1, 2) },
	{ make_pair(2, 0), make_pair(2, 1), make_pair(2, 2) },

	// Every column
	{ make_pair(0, 0), make_pair(1, 0), make_pair(2, 0) },
	{ make_pair(0, 1), make_pair(1, 1), make_pair(2, 1) },
	{ make_pair(0, 2), make_pair(1, 2), make_pair(2, 2) },

	// Every diagonal
	{ make_pair(0, 0), make_pair(1, 1), make_pair(2, 2) },
	{ make_pair(2, 0), make_pair(1, 1), make_pair(0, 2) }

};

// Print the current board state
void print_board(char board[3][3])
{
	cout << endl;
	cout << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << endl;
	cout << "----------" << endl;
	cout << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << endl;
	cout << "----------" << endl;
	cout << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << endl << endl;
}

// Get all available legal moves (spaces that are not occupied)
vector<pair<int, int> > get_legal_moves(char board[3][3])
{
	vector<pair<int, int> > legal_moves;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (board[i][j] != PLAYER_1 && board[i][j] != PLAYER_2)
			{
				legal_moves.push_back(make_pair(i, j));
			}
		}
	}

	return legal_moves;
}

// Check if a position is occupied
bool position_occupied(char board[3][3], pair<int, int> pos)
{
	vector<pair<int, int> > legal_moves = get_legal_moves(board);

	for (int i = 0; i < legal_moves.size(); i++)
	{
		if (pos.first == legal_moves[i].first && pos.second == legal_moves[i].second)
		{
			return false;
		}
	}

	return true;
}

// Get all board positions occupied by the given marker
vector<pair<int, int> > get_occupied_positions(char board[3][3], char marker)
{
	vector<pair<int, int> > occupied_positions;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (marker == board[i][j])
			{
				occupied_positions.push_back(make_pair(i, j));
			}
		}
	}

	return occupied_positions;
}

// Check if the board is full
bool board_is_full(char board[3][3])
{
	vector<pair<int, int> > legal_moves = get_legal_moves(board);

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
bool game_is_won(vector<pair<int, int> > occupied_positions)
{
	bool game_won;

	for (int i = 0; i < winning_states.size(); i++)
	{
		game_won = true;
		vector<pair<int, int> > curr_win_state = winning_states[i];
		for (int j = 0; j < 3; j++)
		{
			if (!(find(begin(occupied_positions),
                   end(occupied_positions),
                   curr_win_state[j]) != end(occupied_positions)))
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
	if (marker == PLAYER_2)
	{
		opponent_marker = PLAYER_1;
	}
	else
	{
		opponent_marker = PLAYER_2;
	}

	return opponent_marker;
}

// Check if someone has won or lost
int get_board_state(char board[3][3], char marker)
{

	char opponent_marker = get_opponent_marker(marker);

	vector<pair<int, int> > occupied_positions = get_occupied_positions(board, marker);

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



//Nhan function
int evaluate1(char board[3][3], char marker){
    char a[8][3]= {{board[0][0],board[0][1],board[0][2]},
                             {board[0][0],board[1][1],board[2][2]},
                             {board[0][0],board[1][0],board[2][0]},
                             {board[0][1],board[1][1],board[2][1]},
                             {board[0][2],board[1][2],board[2][2]},
                             {board[1][0],board[1][1],board[1][2]},
                             {board[2][0],board[2][1],board[2][2]},
                             {board[2][0],board[1][1],board[0][2]}};

    int count_o2 = 0,
        count_o1 = 0,
        count_x2 = 0,
        count_x1 = 0;

    for ( int i = 0; i < 8; i ++){
        char curr_b[3] = {a[i][0],a[i][1],a[i][2]};
	/*if (curr_b[0] == curr_b[1] && curr_b[1] == curr_b[2] && marker == PLAYER_1)
            return LOSS;
        if (curr_b[0] == curr_b[1] && curr_b[1] == curr_b[2] && marker == PLAYER_2)
            return WIN;*/

        if (curr_b[0] == curr_b[1] && curr_b[0] != EMPTY_SPACE && curr_b[2] == EMPTY_SPACE)
            (curr_b[0] == PLAYER_1) ? count_o2 +=1 : count_x2+=1;

        else if  (curr_b[0] == curr_b[2] && curr_b[0] != EMPTY_SPACE&& curr_b[1] == EMPTY_SPACE)
            (curr_b[0] == PLAYER_1) ? count_o2 +=1 : count_x2+=1;

        else if  (curr_b[1] == curr_b[2] && curr_b[0] != EMPTY_SPACE && curr_b[0] == EMPTY_SPACE)
            (curr_b[1] == PLAYER_1) ? count_o2 +=1 : count_x2+=1;

        if (curr_b[0] == curr_b[1] && curr_b[0]== EMPTY_SPACE && curr_b[2] != EMPTY_SPACE)
            (curr_b[2] == PLAYER_1) ? count_o1 +=1 : count_x1+=1;

        else if (curr_b[0] == curr_b[2] && curr_b[0]== EMPTY_SPACE && curr_b[1] != EMPTY_SPACE)
            (curr_b[1] == PLAYER_1) ? count_o1 +=1 : count_x1+=1;

        else if  (curr_b[1] == curr_b[2] && curr_b[1]== EMPTY_SPACE && curr_b[0] != EMPTY_SPACE)
            (curr_b[0] == PLAYER_1) ? count_o1 +=1 : count_x1+=1;

    }

    if (marker == PLAYER_2)
        return 3 * count_x2 + count_x1 - ( 3 * count_o2 + count_o1);

    return 3 * count_o2 + count_o1 - (3 * count_x2 + count_x1 );
}

//This is my evaluation function (Payton Long)
int evaluate2(char board[3][3], char marker)
{
    //These values store each evaluation of each possible win state
    //o
    int c1 = 0;
    int c2 = 0;
    //x
    int c3 = 0;
    int c4 = 0;
    //Get opponent:
    char opponent = get_opponent_marker(marker);
    // Checking rows for possible wins
    for (int row = 0; row<3; row++)
    {
        if (board[row][0]==board[row][1] && board[row][1]==board[row][2])
        {
            if (board[row][0]=='X')
               c1 = 100;
               //return 100;
        } else if (board[row][0]==board[row][1] && board[row][1]==board[row][2]) {
            if (board[row][0]=='O')
               c1 = -100;
               //return -100;
        }
    }
    // Checking columns for possible wins
    for (int colunn = 0; colunn<3; colunn++)
    {
        if (board[0][colunn]==board[1][colunn] && board[1][colunn]==board[2][colunn])
        {
            if (board[0][colunn]=='X') {
                c2 = 100;
                return 100;
            } else if (board[0][colunn]=='O')
                c2 = -100;
                return -100;
        }
    }
    // Checking diagonals for possible wins
    if (board[0][0]==board[1][1] && board[1][1]==board[2][2])
    {
        if (board[0][0]=='X') {
            c3 = 100;
            return 100;
        } else if (board[0][0]=='O')
            c3 = -100;
            return -100;
    }
    if (board[0][2]==board[1][1] && board[1][1]==board[2][0])
    {
        if (board[0][2]=='X') {
            c4 = 100;
            return 100;
        }   else if (board[0][2]=='O')
            c4 = -100;
            return -100;
    }
    //Return the values calculated by evaluation:
    if (marker == PLAYER_2)
        return 3 * c4 + c3 - ( 3 * c2 + c1);
    return 3 * c2 + c1 - (3 * c4 + c3 );
    //If none of the above or a win return 0:
    //return 0;
}

//This is Daniels:
int evaluate3(char board[3][3], char marker){
// The 8 possible winning situations
    char a[8][3]= {{board[0][0],board[0][1],board[0][2]},
                   {board[0][0],board[1][1],board[2][2]},
                   {board[0][0],board[1][0],board[2][0]},
                   {board[0][1],board[1][1],board[2][1]},
                   {board[0][2],board[1][2],board[2][2]},
                   {board[1][0],board[1][1],board[1][2]},
                   {board[2][0],board[2][1],board[2][2]},
                   {board[2][0],board[1][1],board[0][2]}};

// array to hold all possible scores
    int score = 0;

    for (int i = 0; i < 8; i++) {

        int temp_score = 0;
        char curr_b[3] = {a[i][0], a[i][1], a[i][2]};

        // Checks for 1 in a row of same type
        if (curr_b[0] != '-') {
            if (curr_b[0] == marker)
                temp_score += 1;
            else if (curr_b[0] != marker)
                temp_score += -1;
            else
                cout << "Error when checking first cell";
        }
        // Checks for 2 in a row of same type
        if (curr_b[1] != '-') {
            if (curr_b[1] == marker) {
                if (temp_score == 1)
                    temp_score = 10;
                else
                    temp_score = 0;
            }
            else if (curr_b[1] != marker)
                if (temp_score == -1)
                    temp_score = -10;
                else
                    temp_score = 0;
        }
            // Checks for 3 in a row of same type
        else if (curr_b[2] != '-') {
            if (curr_b[2] == marker) {
                if (temp_score > 0)
                    temp_score *= 10;
                else if (temp_score < 0)
                    temp_score = 0;
                else
                    temp_score = 1;
            }
            else if (curr_b[2] != marker)
                if (temp_score < 0)
                    temp_score *= 100;
                else if (temp_score > 0)
                    temp_score = 0;
                else
                    temp_score = -1;
        }
        score += temp_score;
    }
    return score;
}

//Groups Eval:
int evaluate4(char board[3][3], char marker) {
    int count_marker = 0;
    int count_opp = 0;
    char opp = get_opponent_marker(marker);
    //count row
    for (int i = 0; i < 3; i++){
        if (board[i][0] == opp || board[i][1] == opp || board[i][2] == opp){
            continue;
        }
        else if (board[i][0] == marker || board[i][1] == marker || board[i][2] == marker){
            count_marker ++;
        }
    }

    //count col
    for (int i = 0; i < 3; i++){
        if (board[0][i] == opp || board[1][i] == opp || board[2][i] == opp){
            continue;
        }
        else if (board[0][i] == marker || board[1][i] == marker || board[2][i] == marker){
            count_marker ++;
        }
    }
    //cout << "Col: " << count_marker << endl;

    if (board[0][0] == opp || board[1][1] == opp || board[2][2] == opp){
        count_marker = count_marker;
    }
    else if (board[0][0] == marker || board[1][1] == marker || board[2][2] == marker){
        count_marker++;
    }

    if (board[0][2] == opp || board[1][1] == opp || board[2][0] == opp){
        count_marker = count_marker;
    }
    else if (board[0][2] == marker || board[1][1] == marker || board[2][0] == marker){
        count_marker++;
    }


    for (int i = 0; i < 3; i++){
        if (board[i][0] == marker || board[i][1] == marker || board[i][2] == marker){
            continue;
        }
        else if (board[i][0] == opp || board[i][1] == opp || board[i][2] == opp){
            count_opp ++;
   }

    //count col
    for (int i = 0; i < 3; i++){
        if (board[0][i] == marker || board[1][i] == marker || board[2][i] == marker){
            continue;
        }
        else if (board[0][i] == opp || board[1][i] == opp || board[2][i] == opp){
            count_opp ++;
        }
    }

    if (board[0][0] == marker || board[1][1] == marker || board[2][2] == marker){
        count_opp = count_opp;
    }
    else if (board[0][0] == opp || board[1][1] == opp || board[2][2] == opp){
        count_opp++;
    }

    if (board[0][2] == marker || board[1][1] == marker || board[2][0] == marker){
        count_opp = count_opp;
    }
    else if (board[0][2] == opp || board[1][1] == opp || board[2][0] == opp){
        count_opp++;
    }

    return count_marker - count_opp;
}
}

pair<int, pair<int, int> > minimax_optimization_1(char board[3][3], char marker, int depth, int alpha, int beta){

    pair<int, int> best_move = make_pair(-1, -1);
    int best_score = (marker == PLAYER_1) ? LOSS : WIN;
    int current_score;

    if (depth >= MAX_DEPTH){
        //add swich case here for different eval func
        //best_score = evaluate1(board, marker);
        //Takes in user input to choose which algorithms to run!
        switch(userAlgoChoicePlayerOne){
            case 1:
                best_score = evaluate1(board, marker);
                break;
            case 2:
                best_score = evaluate2(board, marker);
                break;
            case 3:
                best_score = evaluate3(board, marker);
                break;
            case 4:
                best_score = evaluate4(board, marker);
                break;
            }
        return make_pair(best_score, best_move);
    }


    if (board_is_full(board) || DRAW != get_board_state(board, PLAYER_1))
	{
        //Takes in user input to choose which algorithms to run!

            switch(userAlgoChoicePlayerOne){
            case 1:
                best_score = evaluate1(board, marker);
                break;
            case 2:
                best_score = evaluate2(board, marker);
                break;
            case 3:
                best_score = evaluate3(board, marker);
                break;
            case 4:
                best_score = evaluate4(board, marker);
                break;
            }
		return make_pair(best_score, best_move);
	}

    vector<pair<int, int> > legal_moves = get_legal_moves(board);

    if (board_is_full(board))
	{
		//best_score = evaluate1(board, marker);

        switch(userAlgoChoicePlayerOne){
            case 1:
                best_score = evaluate1(board, marker);
                break;
            case 2:
                best_score = evaluate2(board, marker);
                break;
            case 3:
                best_score = evaluate3(board, marker);
                break;
            case 4:
                best_score = evaluate4(board, marker);
                break;
            }

		return make_pair(best_score, best_move);
	}

    for (int i = 0; i < legal_moves.size(); i++){
        pair<int, int> curr_move = legal_moves[i];
        board[curr_move.first][curr_move.second] = marker;
            //Player 1 move set: (Playing as alpha)
            if (marker == PLAYER_1){
                nodesExpanded_1++;
                current_score = minimax_optimization_1(board, PLAYER_2, depth + 1, alpha, beta).first;
                // need to make some change
                if (best_score < current_score){

		            best_score = current_score;
                    best_move = curr_move;

                    alpha = max(alpha, best_score);
                    board[curr_move.first][curr_move.second] = EMPTY_SPACE;
                    if (alpha <= beta) {
                        break;
                    }
                }
            }
            //Player 2 move set: (Playing as beta)
            else{
                nodesExpanded_1++;
                current_score = minimax_optimization_1(board, PLAYER_1, depth + 1, alpha, beta).first;
                // need to make some change

                if (best_score > current_score){

        		    best_score = current_score;
                    best_move = curr_move;

                    beta = min(beta, best_score);
                    board[curr_move.first][curr_move.second] = EMPTY_SPACE;
                    if (beta <= alpha){
                        break;
                    }
                }
            }
            board[curr_move.first][curr_move.second] = EMPTY_SPACE;
            //if (alpha > beta) break;
        }

        return make_pair(best_score, best_move);

}


pair<int, pair<int, int> > minimax_optimization_2(char board[3][3], char marker, int depth, int alpha, int beta){

    pair<int, int> best_move = make_pair(-1, -1);
    int best_score = (marker == PLAYER_1) ? LOSS : WIN;
    int current_score;

    if (depth >= MAX_DEPTH){
        //add swich case here for different eval func
        //best_score = evaluate1(board, marker);
        //Takes in user input to choose which algorithms to run!

        switch(userAlgoChoicePlayerTwo){
            case 1:
                best_score = evaluate1(board, marker);
                break;
            case 2:
                best_score = evaluate2(board, marker);
                break;
            case 3:
                best_score = evaluate3(board, marker);
                break;
            case 4:
                best_score = evaluate4(board, marker);
                break;
            }
        return make_pair(best_score, best_move);
    }


    if (board_is_full(board) || DRAW != get_board_state(board, PLAYER_1))
	{
        //Takes in user input to choose which algorithms to run!

       switch(userAlgoChoicePlayerTwo){
            case 1:
                best_score = evaluate1(board, marker);
                break;
            case 2:
                best_score = evaluate2(board, marker);
                break;
            case 3:
                best_score = evaluate3(board, marker);
                break;
            case 4:
                best_score = evaluate4(board, marker);
                break;
            }

		return make_pair(best_score, best_move);
	}

    vector<pair<int, int> > legal_moves = get_legal_moves(board);

    if (board_is_full(board))
	{
		//best_score = evaluate1(board, marker);

        switch(userAlgoChoicePlayerTwo){
            case 1:
                best_score = evaluate1(board, marker);
                break;
            case 2:
                best_score = evaluate2(board, marker);
                break;
            case 3:
                best_score = evaluate3(board, marker);
                break;
            case 4:
                best_score = evaluate4(board, marker);
                break;
            }
		return make_pair(best_score, best_move);
	}

    for (int i = 0; i < legal_moves.size(); i++){
        pair<int, int> curr_move = legal_moves[i];
        board[curr_move.first][curr_move.second] = marker;
            //Player 1 move set: (Playing as alpha)
            if (marker == PLAYER_1){
                nodesExpanded_2++;
                current_score = minimax_optimization_2(board, PLAYER_2, depth + 1, alpha, beta).first;
                // need to make some change
                if (best_score < current_score){

		            best_score = current_score;
                    best_move = curr_move;

                    alpha = max(alpha, best_score);
                    board[curr_move.first][curr_move.second] = EMPTY_SPACE;
                    if (alpha <= beta) {
                        break;
                    }
                }
            }
            //Player 2 move set: (Playing as beta)
            else{
                nodesExpanded_2++;
                current_score = minimax_optimization_2(board, PLAYER_1, depth + 1, alpha, beta).first;
                // need to make some change

                if (best_score > current_score){

        		    best_score = current_score;
                    best_move = curr_move;

                    beta = min(beta, best_score);
                    board[curr_move.first][curr_move.second] = EMPTY_SPACE;
                    if (beta <= alpha){
                        break;
                    }
                }
            }
            board[curr_move.first][curr_move.second] = EMPTY_SPACE;
            //if (alpha > beta) break;
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

	if (DRAW != get_board_state(board, PLAYER_1))
	{
		return true;
	}

	return false;
}


int main()
{
    //time_t start, end;
    nodesExpanded_1 = 0;
    nodesExpanded_2 = 0;
    char userPlayAgain;
    //Getting user input to choose what evaluations to use for each player:
    cout << "Please enter a whole number integer 1-4 for PLAYER 1 eval function: " << endl;
    cin >> userAlgoChoicePlayerOne;
    cout << "Please enter a whole number integer 1-4 for PLAYER 2 eval function: " << endl;
    cin >> userAlgoChoicePlayerTwo;
    //Set up empty board to be played
    char board[3][3] = {{EMPTY_SPACE,EMPTY_SPACE,EMPTY_SPACE},
                         {EMPTY_SPACE,EMPTY_SPACE,EMPTY_SPACE},
                         {EMPTY_SPACE,EMPTY_SPACE,EMPTY_SPACE}};
	print_board(board);

    //Runs this while the game is not done being played in a loop until conditional is met
    //time(&start);
    auto start = high_resolution_clock::now();
	while (!game_is_done(board))
	{
		pair<int, pair<int, int> > ai2_move = minimax_optimization_2(board, PLAYER_2, START_DEPTH, LOSS, WIN);

		board[ai2_move.second.first][ai2_move.second.second] = PLAYER_2;

        print_board(board);

        pair<int, pair<int, int> > ai1_move = minimax_optimization_1(board, PLAYER_1, START_DEPTH, WIN, LOSS);

        board[ai1_move.second.first][ai1_move.second.second] = PLAYER_1;

		print_board(board);
	}
	//time(&end);
	auto stop = high_resolution_clock::now();
    //Metric logging and program finalization:
    int player2_state = get_board_state(board, PLAYER_2);
	int player1_state = get_board_state(board, PLAYER_1);
    if (userAlgoChoicePlayerOne == 1 && player1_state == WIN || userAlgoChoicePlayerTwo == 1 && player2_state == WIN)
    {
        evalOneWins++;
        //percentWon = evalOneWins/gamesPlayed*100;

    } else if (userAlgoChoicePlayerOne == 2 && player1_state == WIN || userAlgoChoicePlayerTwo == 2 && player2_state == WIN) {
        evalTwoWins++;
        //percentWon = evalTwoWins/gamesPlayed*100;

    } else if (userAlgoChoicePlayerOne == 3 && player1_state == WIN || userAlgoChoicePlayerTwo == 3 && player2_state == WIN) {
        evalThreeWins++;
        //percentWon = evalThreeWins/gamesPlayed*100;

    } else if (userAlgoChoicePlayerOne == 4 && player1_state == WIN || userAlgoChoicePlayerTwo == 4 && player2_state == WIN) {
        evalFourWins++;
        //percentWon = evalFourWins/gamesPlayed*100;

    } else {
        draws++;
    }

    //double time_taken = double(end - start);
    auto duration = duration_cast<microseconds>(stop - start);
	cout << "********** GAME OVER **********" << endl;
    cout << "***TOTAL NODES EXPANDED player 1: " << nodesExpanded_1 << "***" << endl;
    cout << "***TOTAL NODES EXPANDED player 2: " << nodesExpanded_2 << "***" << endl;
    cout << "***TOTAL WINS WITH EVAL 1: " << evalOneWins << "***" << endl;
    cout << "***TOTAL WINS WITH EVAL 2: " << evalTwoWins << "***" << endl;
    cout << "***TOTAL WINS WITH EVAL 3: " << evalThreeWins << "***" << endl;
    cout << "***TOTAL WINS WITH EVAL 4: " << evalFourWins << "***" << endl;
    cout << "***TOTAL DRAWS: " << draws << endl;
    cout << "Total time taken to run: " << fixed << setprecision(2)<<duration.count() << " milliseconds." << endl;
	cout << "PLAYER 1: "; print_game_state(player1_state);
	cout << "Using eval # " << userAlgoChoicePlayerOne << endl;
	cout << "PLAYER 2: "; print_game_state(player2_state);
	cout << "Using eval # " << userAlgoChoicePlayerTwo << endl;
    //User selection to play again, metrics are tracked better if ran over and over again.
    cout << "Would you like to play again? Y=YES, N=NO (Tracks more metrics if selected)" << endl;
    cin >> userPlayAgain;
    if (userPlayAgain == 'Y')
    {

        gamesPlayed++;
        main();
    }
	return 0;
}
