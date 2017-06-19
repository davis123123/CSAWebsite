#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum Visibility{REVEALED, CONCEALED, MARKED, QUESTIONED};

typedef struct TILE_STRUCT {
	int is_mines;//is that place a mine
	int num_surrounding_mines;
	enum Visibility visibility;//checks is it revealed

} Tile;

typedef struct BOARD_STRUCT {//struct
	int rows;
	int cols;
	int num_mines;
	Tile** tiles;
	int x;
	int y;
	int action;
	int mines_count;
  int* num_mines_left;
} Board;

//use these structs to store the numbers in there

Board create_board(int rows, int cols, int num_mines);
void print_board(Board board);
void destroy_board(Board board);
void plant_bombs(Board board, int seed);
void plant_nums(Board board);
bool valid_play(Board board, int play, int second_play);
void get_play(Board board);
void get_action(Board board, int play_x, int play_y);
void reveal_num(Board board,int play_x, int play_y);
void print_endBoard(Board board);
bool game_over(Board board);
void play_game(Board board);
void read_args(int argc);

Board create_board(int rows, int cols, int num_mines){//creates board
	int i,j;//variables
	Board board; //Board - type, board - variable
	board.rows = rows;//variables
	board.cols = cols;
	board.num_mines = num_mines;
//variables that'll be used

	board.tiles = (Tile**)malloc(rows*sizeof(Tile*));//rows
	i = 0;
	while( i < rows){
		board.tiles[i] = (Tile*)malloc(cols*sizeof(Tile));//cols
		i++;
	}
	i = 0;
	while(i <rows){//for loop to check the board
		int j = 0;
		while(j<cols){
			board.tiles[i][j].visibility = CONCEALED;
			j++;
		}
		i++;
	}
  board.num_mines_left = (int*)malloc(sizeof(int)*1);//creates space for the mines
  board.num_mines_left[0] = num_mines;
	return board;
}

void print_board(Board board){//print board
	int i = 0;//variable
	int j;

	while(i< board.rows){//for loop that goes through the board
		printf("%d ", board.rows - i -1);//prints space
		for (j = 0; j < board.cols; j++){//for loop that goes through the cols
      if(board.tiles[i][j].visibility == REVEALED){// if the place is revealed
        if(board.tiles[i][j].is_mines == 1){//reveals the mine
          printf("* ");
        }
        else{//prints a space
          printf("%d ",board.tiles[i][j].num_surrounding_mines);
        }
      }
			else if(board.tiles[i][j].visibility == CONCEALED){//if the point is still concealed
				printf("# ");
			}
			else if(board.tiles[i][j].visibility == MARKED){//if the point is marked
				printf("! ");
			}
			else if(board.tiles[i][j].visibility == QUESTIONED){//if the point is questioned
				printf("? ");
			}
		}
		printf("\n");
		i++;
	}

	printf(" ");
	int j  = 0;
	while( j < board.cols){
		printf(" %d", j);//prints the numbers
		j++;
	}//for loop to print the amount of cols
	printf("\n");
}
void destroy_board(Board board){//freeing up the space for the board and the mines
	int i;
	for (i=0; i < board.rows; i++){
		free(board.tiles[i]);
	}//iterating through the num of rows
	free(board.tiles);//free
  free(board.num_mines_left);//free
}

void plant_bombs(Board board, int seed){//this function randomizes the place to put the mines and then plants it
	int x, y;//x and y coordinates
	int i, j ,k;
	int num_bombs = 0;
	k = board.rows -1;//inverses it
	for (i = 0; i < board.rows; i++){//for loop to count the mines
		int j = 0;
		while( j < board.cols){
			board.tiles[i][j].is_mines = 0;
			j++;
		}
	}
	srand(seed);//random

	while(num_bombs < board.num_mines){//
		x = rand()%board.rows;//the random x
		y = rand()%board.cols;//gets random y
    if (board.tiles[k - x][y].is_mines != 0){
			board.tiles[k - x][y].is_mines = 1;//places it, 1 is just a random number, it can be any number. but you need to set the things to the left = to a number
			num_bombs++;//increment num_bombs
      }
      else{//everything else
				printf("Placing mine at %d, %d\n", x, y);//places the mines here
				if(board.rows == 1){//if theres only one row
				 board.tiles[x][y].is_mines = 1;
					num_bombs++;//increment num_bombs
	  }
  }
}



void plant_nums(Board board){//this functon checks how many bombs could be around a spot on the board
	int i = 0;
  int j = 0;
	int count = 0;//intitialize variables
  if (board.rows == 1){//if board is only 1
    if(board.tiles[i][j].is_mines != 1){
			if(board.tiles[i][j+1].is_mines == 1){//board tiles the columns is + 1
				count++;//update the count
			}
			board.tiles[i][j].num_surrounding_mines = count;//setting num mines to the count
			return;
    }
    else{
      count++;//update the count
    }
  }


	for (i = 0; i <board.rows; i++){//for loop
		count = 0;// set count to 0
		for(j = 0; j <board.cols; j++){//for loop
			count = 0;//set count to 0
			if(board.tiles[i][j].is_mines == 1){//if the tile is a mine
				continue;//just continue
			}
			else if (i == 0 && j ==0){//top left
				if(board.tiles[i+1][j].is_mines == 1){// if the row part is a mine
					count++;//update the count
				}
				if(board.tiles[i+1][j+1].is_mines ==1){//if a mine
					count++;//count increasing
				}
				if(board.tiles[i][j+1].is_mines ==1){//if is a mine
					count++;//update count
				}
				board.tiles[i][j].num_surrounding_mines = count;//seting the tiles part to the updated count
			}
			else if (j == 0 && i == board.rows-1){//bottom left
				if(board.tiles[i-1][j].is_mines == 1){//if the mine
					count++;//update count
				}
				if(board.tiles[i-1][j+1].is_mines == 1){//if is a mine
					count++;//update count
				}
				if(board.tiles[i][j+1].is_mines == 1){//if is a mine
					count++;//update count
				}
				board.tiles[i][j].num_surrounding_mines = count;//set to updated count
			}
			else if (i == 0 && j == board.cols - 1){//top right
				if(board.tiles[i][j-1].is_mines == 1){//if is a mine
					count++;//update mine
				}
				if(board.tiles[i+1][j-1].is_mines ==1){
					count++;//update mine
				}
				if(board.tiles[i+1][j].is_mines ==1){
					count++;//update mine
				}
				board.tiles[i][j].num_surrounding_mines = count;//set to most updated
			}
			else if(i == board.rows - 1 && j ==board.cols - 1){//bottom right
				if(board.tiles[i][j-1].is_mines ==1){//if a mine
					count++;//update count
				}
				if(board.tiles[i-1][j-1].is_mines == 1){//if a mine
					count++;//update count
				}
				if(board.tiles[i-1][j].is_mines == 1){//if a mine
					count++;//update count
				}
				board.tiles[i][j].num_surrounding_mines = count;//set most updated count
			}

			else if(j == 0 && (i!=0 ||i!= board.rows-1)){//left vertical
				if(board.tiles[i-1][j].is_mines == 1){//if a mine
					count++;//updat e the count
				}
				if(board.tiles[i-1][j+1].is_mines == 1){//if a mine
					count++;//count update
				}
				if(board.tiles[i][j+1].is_mines == 1){//checks around the point
					count++;//update count
				}
				if(board.tiles[i+1][j+1].is_mines == 1){//checks around the point
					count++;//update count
				}
				if(board.tiles[i+1][j].is_mines == 1){//checks around the point
					count++;//update count
				}
				board.tiles[i][j].num_surrounding_mines = count;//sets count to the variable
			}
			else if((j !=0 || j != board.cols-1) && i == 0){//top horizontal
				if(board.tiles[i][j-1].is_mines == 1){//checks around the point
					count++;//update count
				}
				if(board.tiles[i+1][j-1].is_mines == 1){//checks around the point
					count++;//update count
				}

				if(board.tiles[i+1][j].is_mines == 1){//checks around the point
					count++;//update count
				}

				if(board.tiles[i+1][j+1].is_mines == 1){//checks around the point
					count++;
				}

				if(board.tiles[i][j+1].is_mines == 1){//checks around the point
					count++;
				}
				board.tiles[i][j].num_surrounding_mines = count;//sets the count to the varaible
			}
			else if(i == board.rows - 1 && (j!=board.cols - 1 || j!=0)){//bottom horizontal
				if(board.tiles[i][j-1].is_mines == 1){//checks around the point
					count++;
				}
				if(board.tiles[i-1][j-1].is_mines == 1){//checks around the point
					count++;
				}

				if(board.tiles[i-1][j].is_mines == 1){//checks around the point
					count++;
				}

				if(board.tiles[i-1][j+1].is_mines == 1){//checks around the point
					count++;
				}

				if(board.tiles[i][j+1].is_mines == 1){//checks around the point
					count++;
				}
				board.tiles[i][j].num_surrounding_mines = count;//sets the count to that variable
			}
			else if(j == board.cols - 1 && (i != board.rows -1 || i!= 0)){//right vertical
				if(board.tiles[i-1][j].is_mines == 1){//checks around the point
					count++;
				}
				if(board.tiles[i-1][j-1].is_mines == 1){//checks around the point
					count++;
				}

				if(board.tiles[i][j-1].is_mines == 1){//checks around the point
					count++;
				}

				if(board.tiles[i+1][j-1].is_mines == 1){//checks around the point
					count++;
				}

				if(board.tiles[i+1][j].is_mines == 1){//checks around the point
					count++;
				}
				board.tiles[i][j].num_surrounding_mines = count;//sets count to board
			}
			else{//the middle points
				if(board.tiles[i-1][j].is_mines == 1){//checks around the point
					count++;
				}
				if(board.tiles[i-1][j+1].is_mines == 1){//checks around the point
					count++;
				}
				if(board.tiles[i][j+1].is_mines == 1){//checks around the point
					count++;
				}
				if(board.tiles[i+1][j+1].is_mines == 1){//checks around the point
					count++;
				}
				if(board.tiles[i+1][j].is_mines == 1){//checks around the point
					count++;
				}
				if(board.tiles[i+1][j-1].is_mines == 1){//checks around the point
					count++;
				}
				if(board.tiles[i][j-1].is_mines == 1){//checks around the point
					count++;
				}
				if(board.tiles[i-1][j-1].is_mines == 1){//checks around the point
					count++;
				}
				board.tiles[i][j].num_surrounding_mines = count;
			}
		}
	}


}


bool valid_play(Board board, int play, int second_play){//this function checks if the play is valid
	if(play > board.rows - 1){//if the play enetered bigger than the rows
		return false;
	}
	else if(play < 0){//if the play entered is less than the rows
		return false;
	}
	else if(second_play > board.cols -1){//if the play enetered bigger than the cols
		return false;
	}
	else if(second_play < 0){//if the play entered is less than the cols
		return false;
   }
  else if(board.tiles[play][second_play].visibility == REVEALED){//if its already revealed
    printf("This tile is already revealed.\n");
    return false;
	}
	else{//everything else
		return true;
	}
}

void get_play(Board board){//this function gets the play
	int play_x;
	int play_y;
  int x, y;
	//variables
	printf("Enter row a row between 0-%d and a column between 0-%d: ",board.rows - 1, board.cols - 1);
	scanf("%d %d", &x, &y);//gets input
  if(board.rows == 1){//for a single case
    play_x = x;
    play_y = y;//set the variables to play_x and play_y
  }
  else{//everything else
      play_x  = board.rows - 1 - x;
      play_y = y;
  }

	while(!(valid_play(board,play_x, play_y))){//while valid_play isnt true, do this stuff
		printf("Enter row a row between 0-%d and a column between 0-%d: ",board.rows - 1, board.cols - 1);
		scanf("%d %d", &x, &y);//user input
      if(board.rows == 1){//for a single case
        play_x = x;
        play_y = y;//these variables
      }
      else{//everything else
        play_x  = board.rows - 1 - x;
        play_y = y;
      }
	}

	get_action(board, play_x, play_y);//gets the action
}

void get_action(Board board, int play_x, int play_y){//this function gets the action
	int action, action_Q, action_M;

	if(board.tiles[play_x][play_y].visibility == QUESTIONED){//if its questioned
			printf("Enter Action\n");
			printf("0. UnQuestion\n");
			printf("1. Cancel\n");
			printf("Action: ");//print statements
			scanf("%d", &action_Q);//gets input
			if(action_Q == 1){//if the question is inputted
						get_play(board);//get the play

			}
			else if(action_Q == 0){//if its 1
							board.tiles[play_x][play_y].visibility = CONCEALED;//the tile is concealed
			}
	}
	else if(board.tiles[play_x][play_y].visibility == MARKED){//if the tile is marked
		printf("Enter Action\n");
		printf("0. UnMark\n");
		printf("1. Cancel\n");
		printf("Action: ");
		scanf("%d", &action_M);//the inputs and gets the input
		if(action_M == 0){//if its unmarked
			board.tiles[play_x][play_y].visibility = CONCEALED;
      board.num_mines_left[0]+=1;//add 1 to the amount of mines left
		}

		else if(action_M == 1){//if you canel
			get_play(board);//play the game
		}
	}
	else{//everything else
		printf("Enter Action \n");
		printf("0. Reveal\n");
		printf("1. Question\n");
		printf("2. Mark\n");
		printf("3. Cancel\n");
		printf("Action: ");
		scanf("%d", &action);//the print statements and gets the input
    if(action > 3 || action < 0){//if action isnt' valid
      get_play(board);//play game
    }
    else{//everything else

		  if (action == 0){
			  reveal_num(board,play_x, play_y);//call recursive func
		  }
		  else if (action == 1){//if its questioned
			  board.tiles[play_x][play_y].visibility = QUESTIONED;
		  }

		  else if(action == 2){//if its marked
			  board.tiles[play_x][play_y].visibility = MARKED;
        board.num_mines_left[0] -=1;//subtract
		  }
		  else if(action ==3){//if you cancel
			  get_play(board);//play the game
		  }
	  }

  }
}

void print_endBoard(Board board){
  int i = 0, j;//variables
  while(i < board.rows){
    for(j = 0; j < board.cols; j++){
      board.tiles[i][j].visibility = REVEALED;//the tile is revealed

    }
    i++;
  }
  print_board(board);//print it

}
void reveal_num(Board board,int play_x, int play_y){//the recursive call, which reveals it
	if(board.tiles[play_x][play_y].num_surrounding_mines != 0){//if its not 0
		board.tiles[play_x][play_y].visibility = REVEALED;//the tile is revealed

	}
	else if(board.tiles[play_x][play_y].is_mines == 1){//if its 1
		print_endBoard(board);//print it
    printf("You Lost :(\n");//print statement
		destroy_board(board);//frees up space
		exit(0);//exit it
	}

	else{//the recursive case

		board.tiles[play_x][play_y].visibility = REVEALED;//the tile is revealed

		if(play_x + 1 < board.rows && board.tiles[play_x + 1][play_y].visibility == CONCEALED){//checks downwards
			if(board.tiles[play_x +1][play_y].num_surrounding_mines >=0){//if its not valid
				reveal_num(board, play_x + 1, play_y);//call the function
			}
		}


		if(play_x - 1 >= 0 && board.tiles[play_x - 1][play_y].visibility == CONCEALED){//checks upwards
			if(board.tiles[play_x - 1][play_y].num_surrounding_mines >= 0){//if its not valid
				reveal_num(board, play_x - 1, play_y);//call the function
			}
		}

		if(play_y + 1 < board.cols && board.tiles[play_x][play_y + 1].visibility == CONCEALED){//checks right
      if(board.tiles[play_x][play_y +1].num_surrounding_mines >= 0){//if its not valid
        reveal_num(board,play_x, play_y + 1);//call the func
      }
    }

    if(play_y - 1 >= 0 && board.tiles[play_x][play_y - 1].visibility == CONCEALED){//checks left
      if(board.tiles[play_x][play_y - 1].num_surrounding_mines >= 0){//if its not valid
			  reveal_num(board,play_x, play_y - 1);//call the func
      }
    }

   if(play_x - 1 >=0 && play_y + 1 < board.cols && board.tiles[play_x - 1][play_y + 1].visibility == CONCEALED){//checks upright
      if(board.tiles[play_x - 1][play_y + 1].num_surrounding_mines >= 0){//if its not valid
        reveal_num(board,play_x - 1, play_y + 1);//calls the func
      }
    }

    if(play_x - 1 >=0 && play_y - 1 >= 0 && board.tiles[play_x - 1][play_y - 1].visibility == CONCEALED){//checks upleft
      if(board.tiles[play_x - 1][play_y - 1].num_surrounding_mines >= 0){//if its not valid
        reveal_num(board,play_x - 1, play_y - 1);//calls func
      }
    }

    if(play_x + 1 < board.rows && play_y - 1 >= 0 && board.tiles[play_x + 1][play_y - 1].visibility == CONCEALED){  // checks downleft
      if(board.tiles[play_x + 1][play_y - 1].num_surrounding_mines >= 0){//if its not valid
        reveal_num(board,play_x + 1, play_y - 1);//calls func
      }
    }

    if(play_x + 1 < board.rows && play_y + 1 < board.cols && board.tiles[play_x + 1][play_y + 1].visibility == CONCEALED){ //checks downright
      if(board.tiles[play_x + 1][play_y + 1].num_surrounding_mines >=0){//if its not valid
        reveal_num(board,play_x + 1, play_y + 1);//calls the func
      }
    }

	}
}

bool game_over(Board board){
  int i, j;//initialize variables
  int num_pieces_notBombs = 0;
  int count_one = 0;
  int count_two = 0;
  int count_three = 0;

  num_pieces_notBombs = (board.rows * board.cols) - board.num_mines;//num of pieces that are not bombs
  while(i < board.rows){// rows
    for(j = 0; j < board.cols; j++){//cols
      if(board.tiles[i][j].visibility == REVEALED){//if revealed
        count_one++;//update count one

        if (count_one == num_pieces_notBombs){//if count one is 1
          count_three++;//update this
        }
      }
      if (board.tiles[i][j].is_mines == 1 && board.tiles[i][j].visibility == MARKED){//if it is marked
        count_two++;//update count two

        if(count_two == board.num_mines){//if it is at the end
          count_three++;//update
         }
      }
    }
    i++;
  }

  if(count_three == 2){//if the game is over
    return true;//return true
  }
  else{
    return false;//return false
  }
}
void play_game(Board board){//play game function
  while(!game_over(board)){//while game is not over
    printf("There are %d mines left\n", board.num_mines_left[0]);//print the num of mines left
		print_board(board);//print board
		get_play(board);//play the board
	}
  print_endBoard(board);//function
  printf("You Won!!\n");//print you won if you won
	destroy_board(board);//destroy board
  exit(0);//exit
}

void read_args(int argc){//this function gets the amount of args, i didn't put in what it prints though
	if (argc > 5){//if args is less than 4
				printf("Too many arguments. Usage:\n");
    printf("./mine_sweeper.out num_rows num_cols num_mines [seed])\n");
    exit(0);

	}
	else if (argc < 4){//if args is greater than 5
    		printf("Not enough arguments. Usage:\n");
    printf("./mine_sweeper.out num_rows num_cols num_mines [seed])\n");
    exit(0);
	}
}
