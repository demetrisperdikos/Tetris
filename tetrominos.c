#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int debug = 0;

/* Function to print the tetronimo well, writes to standard output. */
void print_well(int (*final_well)[12]) {
	/* Declare our variables */
	int i,j;
	
    for (i = 0; i < 13; i++) {
        for (j = 0; j < 12; j++) {
        	if(i == 12){
        		if(j == 0){
        			printf("+");	
        		} else if(j == 11){
        			printf("+\n");
        		} else{
        			printf("-");
        		}
        	} else if(j == 0){
				printf("|");
        	} else if(j == 11){
				printf("|\n");
        	} else if (final_well[i][j] == 0) {
                printf(" ");
            } else {
                printf("#");
            }
        }
    }
}

void print_shape(int (*current_shape)[5]) {
	/* Declare our variables */
	int i,j;
	
	printf("\n");
	
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
        	printf("%d",current_shape[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void get_tetromino_piece(char identifier, int (*current_tetromino)[5]) {
	char piece_to_tetromino[6] = {'b','d','g','h','m','n'};
	int i,j,k;
	int tetrominoes[6][5][5] = {
	    {
	        {0, 1, 1, 0, 0},
	        {1, 1, 0, 0, 0},
	        {0, 1, 0, 0, 0},
	        {0, 1, 0, 0, 0},
	        {0, 0, 0, 0, 0},
	    },
	    {
	        {0, 0, 1, 0, 0},
	        {0, 0, 1, 0, 0},
	        {0, 0, 1, 0, 0},
	        {0, 1, 1, 0, 0},
	        {0, 0, 0, 0, 0},
	    },
	    {
	        {0, 0, 0, 0, 0},
	        {0, 1, 1, 0, 0},
	        {0, 1, 1, 0, 0},
	        {0, 1, 0, 0, 0},
	        {0, 0, 0, 0, 0},
	    },
	    {
	        {0, 0, 0, 0, 0},
	        {0, 0, 1, 0, 0},
	        {0, 0, 1, 0, 0},
	        {0, 1, 1, 0, 0},
	        {0, 1, 0, 0, 0},
	    },
	    {
	        {0, 0, 0, 0, 0},
	        {0, 0, 0, 1, 0},
	        {0, 0, 1, 1, 0},
	        {0, 1, 1, 0, 0},
	        {0, 0, 0, 0, 0},
	    },
	    {
	        {0, 0, 0, 0, 0},
	        {0, 0, 1, 0, 0},
	        {0, 1, 1, 1, 0},
	        {0, 0, 1, 0, 0},
	        {0, 0, 0, 0, 0},
	    }
	};
	
	for(i = 0; i < 6; i++){
		if(piece_to_tetromino[i] == tolower(identifier)){
			for(j = 0; j < 5; j++){
				for(k = 0; k < 5; k++){
					current_tetromino[j][k] = tetrominoes[i][j][k];			
				}
			}
			break;
		}
	}
	if(debug == 1){
		print_shape(current_tetromino);
	}
}

/* Function to calculate the left index, returns an integer value. */
int left_index(int (*shape)[5]) {
	/* Declare our variables */
	int i,j,left_index;
	left_index = 5;
	
	for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (shape[j][i] == 1) {
                left_index = i;
            	break;
            }
        }
        if(left_index < 5){
        	break;
        }
    }
	return left_index;
}

int right_index(int (*shape)[5]) {
	/* Declare our variables */
	int i,j,right_index,current_block_value;
	right_index = 5;
	
	for (i = 4; i > -1 ; i--) {
        for (j = 0; j < 5; j++) {
        	current_block_value = shape[j][i];
            if (current_block_value == 1){
            	right_index = i;
                break;
            }
        }
        if(right_index < 5){
        	break;
        }
    }
	return right_index;
}

/*
 * Given an input tetromino return:
 *  the row that includes the bottom of the shape
 *  the column the bottom starts at
 *  how many columns it spans
 *  
 * We will start from the bottom up.
 */

int get_bottom_row(int (*rotated_shape)[5]) {
	int bottom_row,row,column,current_block;
	
	bottom_row = 5;
	
	for(row = 4; row > -1; row--){
		for(column = 0; column < 5; column++){
			current_block = rotated_shape[row][column];
			if(debug == 1){
				printf("This is the current block value: %d\n", current_block);
			}
			if(current_block == 1){
				bottom_row = row;
				break;
			}
		}
		if(bottom_row < 5){
			break;
		}
	}
	
	return bottom_row;
}

/*
 * When rotating 90 degrees, the values of row 'i' become the values of column 'N-i' where 'N' is the length, assuming
 * this is a square.
 */
void rotate_90_degrees_clockwise(int (*current_shape)[5]) {
	int original_shape[5][5],rotated_shape[5][5],i,j;
	
	/*
	 * Copy the current_shape array into a placeholder.
	 */
	for(i = 0; i < 5; i++){
		for(j = 0; j < 5; j++){
			original_shape[i][j] = current_shape[i][j];
		}
	}
	
	/*
	 * Rotate by 90 degrees clockwise by making row i become column N-i
	 */
	for(i = 0; i < 5; i++){
		for(j = 0; j < 5; j++){
			rotated_shape[j][4-i] = original_shape[i][j];
		}
	}
	
	/*
	 * Re-assign the new values to the current_shape.
	 */
	for(i = 0; i < 5; i++){
		for(j = 0; j < 5; j++){
			current_shape[i][j] = rotated_shape[i][j];
		}
	}
}

/*
 * First we will mirror the rows where row order A, B, C, D, E becomes E, D, C, B, A
 * Then we will mirror columns where column order 1, 2, 3, 4, 5 becomes 5, 4, 3, 2, 1
 */
void rotate_180_degrees_clockwise(int (*current_shape)[5]){
	int original_shape[5][5],rotated_shape[5][5],mirrored_shape[5][5],i,j;
	
	for(i = 0; i < 5; i++){
		for(j = 0; j < 5; j++){
			original_shape[i][j] = current_shape[i][j];
		}
	}
	
	for(i = 0; i < 5; i++){
		for(j = 0; j < 5; j++){
			mirrored_shape[4-i][j] = original_shape[i][j];
		}
	}
	for(i = 0; i < 5; i++){
		for(j = 0; j < 5; j++){
			rotated_shape[i][4-j] = mirrored_shape[i][j];
		}
	}

	for(i = 0; i < 5; i++){
		for(j = 0; j < 5; j++){
			current_shape[i][j] = rotated_shape[i][j];
		}
	}
}

void rotate_shape(int turns, int (*current_shape)[5]) {
	int actual_turns = turns % 4;
	
	if(actual_turns == 1){
		rotate_90_degrees_clockwise(current_shape);
	} else if(actual_turns == 2){
		rotate_180_degrees_clockwise(current_shape);		
	} else if(actual_turns == 3){
		rotate_180_degrees_clockwise(current_shape);
		rotate_90_degrees_clockwise(current_shape);
	}
}

int get_number_of_full_rows(int (*current_well)[12]){
	int i,j,total_filled_columns;
	int number_of_full_rows = 0;
	
    for (i = 11; i > -1; i--) {
    	total_filled_columns = 0;
        for (j = 1; j < 11; j++) {
            if (current_well[i][j] == 1) {
            	total_filled_columns++;
            }
        }
        if(total_filled_columns == 10){
        	number_of_full_rows++;
        }
    }
    if(debug == 1){
    	printf("number of full rows: %d\n", number_of_full_rows);
    }
    return number_of_full_rows;
}

void clear_line(int (*current_well)[12]) {
	int original_well[13][12],updated_well[13][12];
    int i,j,number_filled_rows;
    
    number_filled_rows = get_number_of_full_rows(current_well);
    
    if(debug == 1){
    	printf("number of full rows: %d\n", number_filled_rows);
    }
    
    for(i = 0; i < 12; i++){
    	for(j = 0; j < 11; j++){
    		original_well[i][j] = current_well[i][j];
    		updated_well[i][j] = current_well[i][j];
    	}
    }
    
    if(debug == 1){
    	print_well(original_well);
    }
    
    while(number_filled_rows > 0){ 
		for(i = 10; i > -1; i--){
			for(j = 0; j < 11; j++){
				if(debug == 1){
					printf("original_well row,column: %d,%d\n", i-1,j);
					printf("updated_well row,column: %d,%d\n", i,j);
					printf("current value of original_well: %d\n",original_well[i][j]);
					printf("current value of updated_well: %d\n",updated_well[i+1][j]);
				}
				updated_well[i+1][j] = original_well[i][j];
			}
		}
		
		number_filled_rows--;
    }
    for(i = 0; i < 12; i++){
    	for(j = 0; j < 11; j++){
    		current_well[i][j] = updated_well[i][j];
    	}
    }
}

/* Function to drop a shape into the well.
 * We will take input as the shape and the column it starts at.
 * We will assume the shape has already been rotated.
 * 
 * Tasks:
 * 	1. Determine the left index of the input shape
 * 	2. Determine right index of the input shape
 * 	3. Find row and column of highest current block in well given input shape span and subset of rows/columns in the well
 */
void drop_shape(int input_column, int (*rotated_shape)[5], int (*current_well)[12]) {
	int i,j,x,y;
	int intersecting_row,bottom_row,starting_row,starting_column;
	int current_shape_left_index,current_shape_right_index,span;
	int current_well_block_value,rotated_shape_block_value;
	
	if(debug == 1){
		printf("input column: %d\n",input_column);
	}
	
	intersecting_row = -1;
	starting_column = input_column-48+1;
	
	/*
	 * Initialize where to start when updating the well
	 */
	starting_row = 11; /* Start from the bottom of the well */
	
	if(debug == 1){
		printf("starting column: %d\n",starting_column);
		
		printf("entering drop_shape\n");
	}
    
    current_shape_left_index = left_index(rotated_shape);
    current_shape_right_index = right_index(rotated_shape);
    span = current_shape_right_index - current_shape_left_index + 1;
    
	if(debug == 1){
		printf("current_shape_left_index : %d\n",current_shape_left_index);
		printf("current_shape_right_index : %d\n",current_shape_right_index);
	}
    
    /*
     * Starting from the input column and the bottom of the current well we will check the span of the
     * shape to see whether the well has a block there
     */
    bottom_row = get_bottom_row(rotated_shape);
    
	if(debug == 1){
		printf("bottom row of rotated shape with a block: %d\n",bottom_row);
		
		printf("\nChecking for intersecting rows\n");
	}
    
    y = current_shape_left_index;
    x = bottom_row;
    for(i = 0; i < 12; i++){
        y = current_shape_left_index;
        x = bottom_row;
    	for(j = starting_column; j < starting_column+span; j++){
			if(debug == 1){
				printf("current_well row,column: %d,%d\n", i,j);
				printf("rotated shape row,column: %d,%d\n", x,y);
			}
    		current_well_block_value = current_well[i][j];
    		rotated_shape_block_value = rotated_shape[x][y];
			if(debug == 1){
				printf("current value of current_well: %d\n",current_well_block_value);    		
				printf("current value of rotated shape: %d\n",rotated_shape_block_value);
			}
    		if(current_well_block_value == 1 && rotated_shape_block_value == 1){
    			intersecting_row = i;
    			break;
    		}
    		y++;
    	}
    	if(intersecting_row != -1){
    		starting_row = intersecting_row-1;
    		break;
    	}
    }
	if(debug == 1){
		printf("\nUpdating current well\n");
	}
    
    /*
     * Now we will update the current_well with the values from the dropped piece
     */
    x = bottom_row;
    for(i = starting_row; i > starting_row - bottom_row-1; i--){
    	
    	y = 0 + current_shape_left_index;
    	for(j = starting_column; j < starting_column+span; j++){
			if(debug == 1){
				printf("rotated shape row,column: %d,%d\n", x,y);
				printf("current_well row,column: %d,%d\n", i,j);
				printf("current value of rotated shape: %d\n",rotated_shape[x][y]);
				printf("current value of current_well: %d\n",current_well[i][j]);
			}
    		if(rotated_shape[x][y] == 1 && current_well[i][j] == 0 ){
    			current_well[i][j] = rotated_shape[x][y];
    		}
			if(debug == 1){
				printf("updated value of current_well: %d\n",current_well[i][j]);
			}
    		y++;
    	}
    	if(x == -1){
    		if(debug == 1){
    			printf("current value of x : %d\n",x);
    		}
    		break;
    	} else{
    		if(debug == 1){
    			printf("current value of x : %d\n",x);
    		}
        	x--;
    	}

    }
    
	if(debug == 1){
		print_well(current_well);
		printf("\nexiting drop_shape\n");
	}
	if(get_number_of_full_rows(current_well) > 0){
	    clear_line(current_well);
	}
}

int main(int argc, char *argv[]) {
	int current_well[13][12] = { 0 };
	FILE *filename_ptr;
	size_t max_line_length = 255;
	char *current_line = malloc(max_line_length);
	

	if(argc > 1){
		filename_ptr = fopen(argv[1], "r");
	} else {
		printf("No input file provided\n");
		exit(0);
	}
	
	if(debug == 1){
		print_well(current_well);
	}

    while (fgets(current_line,max_line_length,filename_ptr)) {
    	char shape,current_char;
        int column,i;
        unsigned int turns;
        char has_shape, has_column, has_turns;
        int current_line_length = strlen(current_line);
		int current_shape[5][5] = { 0 };
		has_shape = 'f';
		has_column = 'f';
		has_turns = 'f';
		
        /*
         * Use a loop to get the input data.
         */
        for(i = 0; i < current_line_length;i++){
        	current_char = current_line[i];
			if(debug == 1){
				printf("current char: %c\n",current_char);
			}
        	if(current_char != ' ' && has_shape == 'f'){
        		shape = current_char;
        		has_shape = 't';
        	} else if(current_char != ' ' && has_turns == 'f'){
        		turns = current_line[i+1];
        		has_turns = 't';
        		i++;
        	} else if(current_char != ' ' && has_column == 'f'){
        		column = current_line[i+1];
    			if(debug == 1){
					printf("column: %c\n",column);
    			}
        		has_column = 't';
        		i++;
        	}
        }
        
        /*
         * First we want to get the current shape based on the input shape value
         */
        get_tetromino_piece(shape, current_shape);
        
		if(debug == 1){
			printf("Current Tetromino Shape:\n");
			print_shape(current_shape);
		}
        
        /*
         * Then we want to rotate the current shape based on how many turns
         */
        rotate_shape(turns, current_shape);
        
		if(debug == 1){
			printf("Rotated Tetromino Shape:\n");
			print_shape(current_shape);
		}
        
        /*
         * Then we want to drop the shape
         */
		if(debug == 1){
			printf("column: %c\n",column);
		}
        drop_shape(column, current_shape, current_well);
    }
	print_well(current_well);
    return 0;
}