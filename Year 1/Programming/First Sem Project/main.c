//no mater how dumb you think people are, they will still find a way to surprise you
//https://github.com/TheBanditOfRed

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>     // used for directory listing      
#include <time.h>       // used for random number generation
#include <sys/stat.h>   // used for folder creation
#include <string.h>     // used for string manipulation
#include <ctype.h>      // used for valid entry checks (isdigit) - could have done manualy but this already exists so...... 

//NOTE: every row in 2D array is a game round, allows for easy history tracking and save file management
//NOTE: any lines marked with DEBUG are either for testing or are not used in the final version of the code

typedef struct {    // Struct for offsets
    int hundreds;
    int tensFront;
    int tensBack;
    int onesFront;
    int onesBack;
} Offsets;

void printOffsetWhiteSpace(int offset) {    // Print white space for offset
    for (int i = 0; i < offset; i++) {
        printf(" ");
    }
}

void printOffsetSeperator(int offset) {    // Print seperator for offset
    for (int i = 0; i < offset; i++) {
        printf("-");
    }
}

void determineAndPrintOffsetSeperator(int saveGameHighestNumber, Offsets offsets) {     // Determine and print the separator
    if (saveGameHighestNumber > 99) {
        printOffsetSeperator((offsets.hundreds * 2) + 3);
    } else if (saveGameHighestNumber > 9 && saveGameHighestNumber < 100) {
        printOffsetSeperator(offsets.tensFront + offsets.tensBack + 2);
    } else if (saveGameHighestNumber < 10) {
        printOffsetSeperator(offsets.onesFront + offsets.onesBack + 1);
    } else {
        printf("Error ! - printing separators\n");    // debug
        exit(0);
    }
}

void determineAndPrintFrontOffsetWhitespace(int i, int saveGameHighestNumber, Offsets offsets) {        // Determine and print the front offset
    if (saveGameHighestNumber > 99) {
        if (i > 99) {
            printOffsetWhiteSpace(offsets.hundreds);
        } else if (i < 100 && i > 9) {
            printOffsetWhiteSpace(offsets.tensFront);
        } else if (i < 10) {
            printOffsetWhiteSpace(offsets.onesFront);
        } else {
            printf("Error ! - printing front offset [hundreds]\n");    // debug
            exit(0);
        }
    } else if (saveGameHighestNumber > 9 && saveGameHighestNumber < 100) {
        if (i < 100 && i > 9) {
            printOffsetWhiteSpace(offsets.tensFront);
        } else if (i < 10) {
            printOffsetWhiteSpace(offsets.onesFront);
        } else {
            printf("Error ! - returning front offset [tens]\n");    // debug
            exit(0);
        }
    } else if (saveGameHighestNumber < 10) {
        if (i < 10) {
            printOffsetWhiteSpace(offsets.onesFront);
        } else {
            printf("Error ! - returning front offset [ones]\n");    // debug
            exit(0);
        }
    } else {
        printf("Error ! - returning front offset\n");    // debug
        exit(0);
    }
}

void determineAndPrintBackOffsetWhitespace(int i, int saveGameHighestNumber, Offsets offsets) {         // Determine and print the back offset
    if (saveGameHighestNumber > 99) {
        if (i > 99) {
            printOffsetWhiteSpace(offsets.hundreds);
        } else if (i > 9 && i < 100) {
            printOffsetWhiteSpace(offsets.tensBack);
        } else if (i < 10) {
            printOffsetWhiteSpace(offsets.onesBack);
        } else {
            printf("Error ! - returning back offset [hundreds]\n");    // debug
            exit(0);
        }
    } else if (saveGameHighestNumber > 9 && saveGameHighestNumber < 100) {
        if (i > 9 && i < 100) {
            printOffsetWhiteSpace(offsets.tensBack);
        } else if (i < 10) {
            printOffsetWhiteSpace(offsets.onesBack);
        } else {
            printf("Error ! - returning back offset [tens]\n");    // debug
            exit(0);
        }
    } else if (saveGameHighestNumber < 10) {
        if (i < 10) {
            printOffsetWhiteSpace(offsets.onesBack);
        } else {
            printf("Error ! - returning back offset [ones]\n");    // debug
            exit(0);
        }
    } else {
        printf("Error ! - returning back offset\n");    // debug
        exit(0);
    }
}

char** allocateArray(int initialSize) {    // Allocate an array
    char **array = malloc(initialSize * sizeof(char *));
    if (array == NULL) {
        perror("Error allocating memory");
        exit(1);
    }
    return array;
}

void addElementToArray(char ***array, int *size, int *capacity, const char *element) {      // Add an element to an array
    // Check if the array needs to be resized
    if (*size >= *capacity) {
        *capacity *= 2; // Double the capacity
        *array = realloc(*array, *capacity * sizeof(char *));
        if (*array == NULL) {
            perror("Error reallocating memory");
            exit(1);
        }
    }

    // Add the new element
    (*array)[*size] = strdup(element); // Use strdup to duplicate the string
    (*size)++;
}

int** allocate2DArray(int initialRows, int initialCols) {    // Allocate a 2D array
    int **array = (int **)malloc(initialRows * sizeof(int *));
    if (array == NULL) {
        perror("Error allocating memory");
        exit(1);
    }
    for (int i = 0; i < initialRows; i++) {
        array[i] = (int *)malloc(initialCols * sizeof(int));
        if (array[i] == NULL) {
            printf("Error allocating memory for row %d\n", i);
            perror("Error reason");
            exit(1);
        }
    }
    return array;
}

void addElementTo2DArray(int ***array, int *rows, int *cols, int *currentRow, int *currentCol, int element) {   // Add an element to a 2D array
    if (*currentRow >= *rows) {
        *rows *= 2; // Double the number of rows
        *array = realloc(*array, *rows * sizeof(int*));
        if (*array == NULL) {
            perror("Error reallocating memory for rows");
            exit(1);
        }
        for (int i = *currentRow; i < *rows; i++) {
            (*array)[i] = malloc(*cols * sizeof(int));
            if ((*array)[i] == NULL) {
                perror("Error allocating memory for new columns");
                exit(1);
            }
        }
    }

    if (*currentCol >= *cols) {
        *cols *= 2; // Double the number of columns
        for (int i = 0; i < *rows; i++) {
            (*array)[i] = realloc((*array)[i], *cols * sizeof(int));
            if ((*array)[i] == NULL) {
                perror("Error reallocating memory for columns");
                exit(1);
            }
        }
    }

    (*array)[*currentRow][*currentCol] = element;
    (*currentCol)++;
    if (*currentCol >= *cols) {
        *currentCol = 0;
        (*currentRow)++;
    }
}

void addRowTo2DArrayWeirdCase(int ***array, int *rows, int cols, int *currentRow) {     //in some cases the main addRowTo2DArray function does not work as intended, this is a workaround
    if (*currentRow == 0) {
        printf("Error: No previous row to copy\n");
        return;
    }

    if (*currentRow >= *rows) {
        *rows += 1; // Add a new row
        *array = realloc(*array, *rows * sizeof(int*));
        if (*array == NULL) {
            perror("Error reallocating memory for rows");
            exit(1);
        }
        (*array)[*rows - 1] = malloc(cols * sizeof(int));
        if ((*array)[*rows - 1] == NULL) {
            perror("Error allocating memory for new row");
            exit(1);
        }
    } else {
        (*array)[*currentRow] = malloc(cols * sizeof(int));
        if ((*array)[*currentRow] == NULL) {
            perror("Error allocating memory for new row");
            exit(1);
        }
    }
    memcpy((*array)[*currentRow], (*array)[*currentRow - 1], cols * sizeof(int));
    (*currentRow)++;
}

int addRowTo2DArray(int ***array, int *rows, int cols) {    // Add a row to a 2D array
    // Increase the number of rows by 1
    *rows += 1;
    *array = realloc(*array, *rows * sizeof(int*));
    if (*array == NULL) {
        perror("Error reallocating memory for rows");
        exit(1);
    }

    // Allocate memory for the new row
    (*array)[*rows - 1] = malloc(cols * sizeof(int));
    if ((*array)[*rows - 1] == NULL) {
        perror("Error allocating memory for new row");
        exit(1);
    }

    // Copy data from the previous row to the new row
    if (*rows > 1) {
        memcpy((*array)[*rows - 1], (*array)[*rows - 2], cols * sizeof(int));
    }
    return *rows;
}

void printArray(char **array, int size) {       // Print the array DUBUG - might use this code might not who knows lmao
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("%s", array[i]);
    }
    printf("\n");
}

void print2DArray(int **array, int rows, int cols) {   // Print the 2D array DUBUG - might use this code might not who knows lmao
    printf("\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
}

void printCurrentGame(int **array, int currentRow, int cols, char *currentPlayer) {   // Print the last play DUBUG - might use this code might not who knows lmao
    printf("\n");
    currentRow--;

    int saveGameHighestNumber = array[0][0];    // assume the first number is the highest

    for (int j = 0; j < cols; j++) {
        if (array[currentRow][j] > saveGameHighestNumber) {
            saveGameHighestNumber = array[currentRow][j];
        }
    }

    Offsets offsets = {0, 0, 0, 0, 0}; // Initialize the struct

    if (saveGameHighestNumber > 99) {
        offsets.hundreds = 2;   
        offsets.tensFront = 3;  
        offsets.tensBack = 2;
        offsets.onesFront = 3;
        offsets.onesBack = 3;
    } else if (saveGameHighestNumber > 9 && saveGameHighestNumber < 100) { 
        offsets.tensFront = 2;  
        offsets.tensBack = 2;
        offsets.onesFront = 2;
        offsets.onesBack = 3;
    } else if (saveGameHighestNumber < 10) { 
        offsets.onesFront = 2;
        offsets.onesBack = 2;
    } else {
        printf("Error ! - saveGameHighestNumber\n");    // debug
        exit(0);
    }

    int gameCurrentTitalLenght = strlen(currentPlayer);
    int gameCurrentTitalSize = 0;
    int gameCurrentTitalCapacity = gameCurrentTitalLenght;    // Allocate memory for the string

    char **gameCurrentTitalArray = allocateArray(gameCurrentTitalCapacity);    // Allocate memory for the string
    addElementToArray(&gameCurrentTitalArray, &gameCurrentTitalSize, &gameCurrentTitalCapacity , currentPlayer);
    
    int gameCurrentTitalArraySeperatorLenght = 0;       // Calculate the length of the separator line
    if (saveGameHighestNumber > 99) {
        for (int i = 0; i < cols; i++) {
            if (i == cols - 1) {
                gameCurrentTitalArraySeperatorLenght += (offsets.hundreds * 2) + 5;    //+-------+
            } else {
                gameCurrentTitalArraySeperatorLenght += (offsets.hundreds * 2) + 4;    //+-------
            }
        }
    } else if (saveGameHighestNumber > 9 && saveGameHighestNumber < 100) {
        for (int i = 0; i < cols; i++) {
            if (i == cols - 1) {
                gameCurrentTitalArraySeperatorLenght += (offsets.tensFront + offsets.tensBack) + 4;   //+------+
            } else {
                gameCurrentTitalArraySeperatorLenght += (offsets.tensFront + offsets.tensBack) + 3;   //+------
            }
        }
    } else if (saveGameHighestNumber < 10) {
        for (int i = 0; i < cols; i++) {
            if (i == cols - 1) {
                gameCurrentTitalArraySeperatorLenght += (offsets.onesFront + offsets.onesBack) + 3;   //+-----+
            } else {
                gameCurrentTitalArraySeperatorLenght += (offsets.onesFront + offsets.onesBack) + 2;   //+-----
            }
        }
    } else {
        printf("Error ! - separator length\n");    // debug
        exit(0);
    }
    int gameCurrentTitalArrayOffsetLenght = ((gameCurrentTitalArraySeperatorLenght / 2) + 6) - (gameCurrentTitalLenght / 2);
 
    int gameCurrentTitalOffsetSize = 0;
    int gameCurrentTitalOffsetCapacity = gameCurrentTitalArrayOffsetLenght;
    char **gameCurrentTitalArrayOffset = allocateArray(gameCurrentTitalOffsetCapacity);    // Allocate memory for the string
    for (int i = 0; i < gameCurrentTitalArrayOffsetLenght; i++) {
        addElementToArray(&gameCurrentTitalArrayOffset, &gameCurrentTitalOffsetSize, &gameCurrentTitalOffsetCapacity, " ");
    }

    int gameCurrentTitalArrayAndOffsetSize = 0;
    int gameCurrentTitalArrayAndOffsetCapacity = gameCurrentTitalArrayOffsetLenght + gameCurrentTitalLenght;
    char **gameCurrentTitalArrayAndOffset = allocateArray(gameCurrentTitalArrayAndOffsetCapacity);    // Allocate memory for the string

    for (int i = 0; i < gameCurrentTitalOffsetSize; i++) {
        addElementToArray(&gameCurrentTitalArrayAndOffset, &gameCurrentTitalArrayAndOffsetSize, &gameCurrentTitalArrayAndOffsetCapacity, gameCurrentTitalArrayOffset[i]);
    }
    for (int i = 0; i < gameCurrentTitalSize; i++) {
        addElementToArray(&gameCurrentTitalArrayAndOffset, &gameCurrentTitalArrayAndOffsetSize, &gameCurrentTitalArrayAndOffsetCapacity, gameCurrentTitalArray[i]);
    }

    printArray(gameCurrentTitalArrayAndOffset, gameCurrentTitalArrayAndOffsetSize);

    free(gameCurrentTitalArray);
    free(gameCurrentTitalArrayOffset);
    free(gameCurrentTitalArrayAndOffset);
    printf("      ");
    for (int i = 0; i < cols; i++) {
        if (i == cols - 1) {
            printf("+");
            determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
            printf("+\n Fila |");
        } else {
            printf("+");
            determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
        }
    }

    for (int i = 0; i < cols; i++) {
        determineAndPrintFrontOffsetWhitespace(i, saveGameHighestNumber, offsets);
        printf("%d", i + 1);
        determineAndPrintBackOffsetWhitespace(i, saveGameHighestNumber, offsets);
        printf("|");
    }

    
    char currentCoinState[7];
    if (currentRow == 0) {
        strcpy(currentCoinState, "Inicio");
    } else {
        strcpy(currentCoinState, "Moedas");
    }
    printf("\n      |");
    determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
    for (int i = 0; i < cols; i++) {
        if (i == cols - 2) {
            printf("+");
            determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
            printf("|\n%s|", currentCoinState);
            break;
        } else {
            printf("+");
            determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
        }
    }
    int player = 0;
    
    for (int j = 0; j < cols; j++) {
        determineAndPrintFrontOffsetWhitespace(array[currentRow][j], saveGameHighestNumber, offsets);
        printf("%d", array[currentRow][j]);
        determineAndPrintBackOffsetWhitespace(array[currentRow][j], saveGameHighestNumber, offsets);
        printf("|");       
    }
        
    
    printf("\n      +");
    determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
    for (int i = 0; i < cols; i++) {
        if (i == cols - 2) {
            printf("+");
            determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
            printf("+\n");
            break;
        } else {
            printf("+");
            determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
        }
    } 
}

void printGameHistory(int **array, int rows, int cols, int gameRound, int gameMode) {  // Print the game history
    // NOTE: highest random number in the game is 255 [0-254, FF is asumed to be filler data so 255 cant be used] (so there is no need for more than 1 byte per entry)
    
    int saveGameHighestNumber = array[0][0];    // assume the first number is the highest
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (array[i][j] > saveGameHighestNumber) {
                saveGameHighestNumber = array[i][j];
            }
        }
    }

    Offsets offsets = {0, 0, 0, 0, 0}; // Initialize the struct

    if (saveGameHighestNumber > 99) {
        offsets.hundreds = 2;   
        offsets.tensFront = 3;  
        offsets.tensBack = 2;
        offsets.onesFront = 3;
        offsets.onesBack = 3;
    } else if (saveGameHighestNumber > 9 && saveGameHighestNumber < 100) { 
        offsets.tensFront = 2;  
        offsets.tensBack = 2;
        offsets.onesFront = 2;
        offsets.onesBack = 3;
    } else if (saveGameHighestNumber < 10) { 
        offsets.onesFront = 2;
        offsets.onesBack = 2;
    } else {
        printf("Error ! - saveGameHighestNumber\n");    // debug
        exit(0);
    }

    int gameHistoryTitalLenght = strlen("Historia do Jogo");
    int gameHistoryTitalSize = 0;
    int gameHistoryTitalCapacity = gameHistoryTitalLenght;    // Allocate memory for the string

    char **gameHistoryTitalArray = allocateArray(gameHistoryTitalCapacity);    // Allocate memory for the string
    addElementToArray(&gameHistoryTitalArray, &gameHistoryTitalSize, &gameHistoryTitalCapacity , "Historia do Jogo");
    
    int gameHistoryTitalArraySeperatorLenght = 0;       // Calculate the length of the separator line
    if (saveGameHighestNumber > 99) {
        for (int i = 0; i < cols; i++) {
            if (i == cols - 1) {
                gameHistoryTitalArraySeperatorLenght += (offsets.hundreds * 2) + 5;    //+-------+
            } else {
                gameHistoryTitalArraySeperatorLenght += (offsets.hundreds * 2) + 4;    //+-------
            }
        }
    } else if (saveGameHighestNumber > 9 && saveGameHighestNumber < 100) {
        for (int i = 0; i < cols; i++) {
            if (i == cols - 1) {
                gameHistoryTitalArraySeperatorLenght += (offsets.tensFront + offsets.tensBack) + 4;   //+------+
            } else {
                gameHistoryTitalArraySeperatorLenght += (offsets.tensFront + offsets.tensBack) + 3;   //+------
            }
        }
    } else if (saveGameHighestNumber < 10) {
        for (int i = 0; i < cols; i++) {
            if (i == cols - 1) {
                gameHistoryTitalArraySeperatorLenght += (offsets.onesFront + offsets.onesBack) + 3;   //+-----+
            } else {
                gameHistoryTitalArraySeperatorLenght += (offsets.onesFront + offsets.onesBack) + 2;   //+-----
            }
        }
    } else {
        printf("Error ! - separator length\n");    // debug
        exit(0);
    }

    int gameHistoryTitalArrayOffsetLenght = ((gameHistoryTitalArraySeperatorLenght / 2) + 6) - (gameHistoryTitalLenght / 2);
 
    int gameHistoryTitalOffsetSize = 0;
    int gameHistoryTitalOffsetCapacity = gameHistoryTitalArrayOffsetLenght;
    char **gameHistoryTitalArrayOffset = allocateArray(gameHistoryTitalOffsetCapacity);    // Allocate memory for the string
    for (int i = 0; i < gameHistoryTitalArrayOffsetLenght; i++) {
        addElementToArray(&gameHistoryTitalArrayOffset, &gameHistoryTitalOffsetSize, &gameHistoryTitalOffsetCapacity, " ");
    }

    int gameHistoryTitalArrayAndOffsetSize = 0;
    int gameHistoryTitalArrayAndOffsetCapacity = gameHistoryTitalArrayOffsetLenght + gameHistoryTitalLenght;
    char **gameHistoryTitalArrayAndOffset = allocateArray(gameHistoryTitalArrayAndOffsetCapacity);    // Allocate memory for the string

    for (int i = 0; i < gameHistoryTitalOffsetSize; i++) {
        addElementToArray(&gameHistoryTitalArrayAndOffset, &gameHistoryTitalArrayAndOffsetSize, &gameHistoryTitalArrayAndOffsetCapacity, gameHistoryTitalArrayOffset[i]);
    }
    for (int i = 0; i < gameHistoryTitalSize; i++) {
        addElementToArray(&gameHistoryTitalArrayAndOffset, &gameHistoryTitalArrayAndOffsetSize, &gameHistoryTitalArrayAndOffsetCapacity, gameHistoryTitalArray[i]);
    }
    printArray(gameHistoryTitalArrayAndOffset, gameHistoryTitalArrayAndOffsetSize);

    free(gameHistoryTitalArray);
    free(gameHistoryTitalArrayOffset);
    free(gameHistoryTitalArrayAndOffset);

    printf("      ");
    for (int i = 0; i < cols; i++) {
        if (i == cols - 1) {
            printf("+");
            determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
            printf("+\n Fila |");
        } else {
            printf("+");
            determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
        }
    }

    for (int i = 0; i < cols; i++) {
        determineAndPrintFrontOffsetWhitespace(i, saveGameHighestNumber, offsets);
        printf("%d", i + 1);
        determineAndPrintBackOffsetWhitespace(i, saveGameHighestNumber, offsets);
        printf("|");
    }

    printf("\n      |");
    determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
    for (int i = 0; i < cols; i++) {
        if (i == cols - 2) {
            printf("+");
            determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
            printf("|\nInicio|");
            break;
        } else {
            printf("+");
            determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
        }
    }

    int player = 0;
    for (int i = 0; i < rows; i++) {
        int row = 0;
        if (player == 1 && i > 0) {
                printf("  P1  |");
            } else if (player == 0  && i > 0) {
                if (gameMode == 1) {
                    printf("  P2  |");
                } else {
                    printf(" CPU  |");
                }
            }
        for (int j = 0; j < cols; j++) {
            determineAndPrintFrontOffsetWhitespace(array[i][j], saveGameHighestNumber, offsets);
            printf("%d", array[i][j]);
            determineAndPrintBackOffsetWhitespace(array[i][j], saveGameHighestNumber, offsets);
            printf("|");       
        }
        
        if (player == 0) {
            if (row == rows - 1) {
                printf("\n   l  ");
                for (int i = 0; i < cols; i++) {
                    if (i == cols - 1) {
                        printf("+");
                        determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
                        printf("+\n");
                    } else {
                        printf("+");
                        determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
                    }
                }
                break;
            } else if (gameRound == 1) {
                printf("\n      +");
            } else {
                printf("\n      |");
            }
            determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
            for (int i = 0; i < cols; i++) {
                if (gameRound == 1) {
                    if (i == cols - 2) {
                        printf("+");
                        determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
                        printf("+\n");
                        break;
                    } else {
                        printf("+");
                        determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
                    }
                } else if (i == cols - 2) {
                    printf("+");
                    determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
                    printf("|\n");
                    break;
                } else {
                    printf("+");
                    determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
                }
            }
            player = 1;
        } else {
            if (row == rows - 1) {
                printf("\n   l  ");
                for (int i = 0; i < cols; i++) {
                    if (i == cols - 1) {
                        printf("+");
                        determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
                        printf("+\n");
                    } else {
                        printf("+");
                        determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
                    }
                }
                break;
            } else {
            printf("\n      |");
            determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
            }
            for (int i = 0; i < cols; i++) {
                if (i == cols - 2) {
                    printf("+");
                    determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
                    printf("|\n");
                    break;
                } else {
                    printf("+");
                    determineAndPrintOffsetSeperator(saveGameHighestNumber, offsets);
                }
            }
            player = 0;
        }
        row++;
    }
}

void saveGameArray(char saveFilePathFull[], int rows, int cols, int **array, int gameMode) {   // Save the 2D array to the save file 

    FILE *file = fopen(saveFilePathFull, "r+b");    // Open the save file
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    fseek(file, 0, SEEK_SET);
    fread(&gameMode, sizeof(__int8_t), 1, file);
    fseek(file, 2, SEEK_SET);
    fwrite(&rows, sizeof(__int8_t), 1, file);
    fseek(file, 3, SEEK_SET);
    fwrite(&cols, sizeof(__int8_t), 1, file);

    fseek(file, 4, SEEK_SET);

    // Write the 2D array to the file
    for (int i = 0; i < rows; i++) {
        fwrite(array[i], sizeof(int), cols, file);
    }

    fflush(file);
    fclose(file);
}

int** loadGameArray(char saveFilePathFull[], int *rows, int *cols, int *gameMode, int *lastPlayer) {    // Load the 2D array from the save file
    FILE *file = fopen(saveFilePathFull, "r+b");    // Open the save file
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    fseek(file, 0, SEEK_SET);
    fread(gameMode, sizeof(__int8_t), 1, file);
    fseek(file, 1, SEEK_SET);
    fread(lastPlayer, sizeof(__int8_t), 1, file);
    fseek(file, 2, SEEK_SET);
    fread(rows, sizeof(__int8_t), 1, file);
    fseek(file, 3, SEEK_SET);
    fread(cols, sizeof(__int8_t), 1, file);
    fseek(file, 4, SEEK_SET);

    // Allocate memory for the 2D array
    int **array = allocate2DArray(*rows, *cols);

    // Read the 2D array from the file
    for (int i = 0; i < *rows; i++) {
        fread(array[i], sizeof(int), *cols, file);
    }

    fclose(file);
    return array;
}

void checkAndAddPairs(int x, int *numPairs4, int *numPairs2, int *numPairs1) {  // Check and add pairs
    int y = 4, z = 2, a = 1;
    int fourcheck = x % y;
    int twocheck = fourcheck % z;
    int onecheck = twocheck % a;
    int tempval = 0;

    if (fourcheck == 0) {
        *numPairs4 += x / y;
    } else {
        tempval = x - fourcheck;
        *numPairs4 += tempval / y;
        if(twocheck == 0) {
            *numPairs2 += fourcheck / z;
        } else {
            tempval = fourcheck - twocheck;
            *numPairs2 += tempval / z;
            if(onecheck == 0) {
                *numPairs1 += twocheck / a;
            } else {
                tempval = twocheck - onecheck;
                *numPairs1 += tempval / a;
            }
        }
    }
}

int checkPairsBadNim(int x, int numPairs4, int numPairs2, int numPairs1, int checkValue, int arrayIndex) {      // Check pairs and return col for bad nim
    int y = 4, z = 2, a = 1;
    int fourcheck = x % y;
    int twocheck = fourcheck % z;
    int onecheck = twocheck % a;
    int tempval = 0;
    int containsBadNim = 0;

    if (fourcheck == 0) {
        numPairs4 = x / y;
        if(numPairs4 > 0 && checkValue == 3) {
            return containsBadNim = arrayIndex;
        }
    } else {
        tempval = x - fourcheck;
        numPairs4 = tempval / y;
        if (numPairs4 > 0 && checkValue == 3) {
            return containsBadNim = arrayIndex;
        }
    }

    if(twocheck == 0) {
        numPairs2 = fourcheck / z;
        if (numPairs2 > 0 && checkValue == 2) {
            return containsBadNim = arrayIndex;
        }
    } else {
        tempval = fourcheck - twocheck;
        numPairs2 = tempval / z;
        if (numPairs2 > 0 && checkValue == 2) {
            return containsBadNim = arrayIndex;
        }
    }
    
    if(onecheck == 0) {
        numPairs1 = twocheck / a;
        if (numPairs1 > 0 && checkValue == 1) {
            return containsBadNim = arrayIndex;
        }
    } else {
        tempval = twocheck - onecheck;
        numPairs1 = tempval / a;
        if (numPairs1 > 0 && checkValue == 1) {
            return containsBadNim = arrayIndex;
        }
    }
    return containsBadNim = 1000;   // Return a value that will not be used
}

void AI(int *AIcols, int *AIcoins, int **array, int rows, int cols) {   // AI function
    int numPairs[3] = {0, 0, 0};
    int nimSum = 0;
    int numBadNimSum = 0;
    int col = 0;
    int checkPileSize = 0;

    int *x = array[rows - 1];

    for(int i = 0; i < cols; i++) {
        checkAndAddPairs(x[i], &numPairs[0], &numPairs[1], &numPairs[2]);
    }

    nimSum = (numPairs[0] + numPairs[1] + numPairs[2]) % 2;
    if (nimSum == 0) {
        *AIcols = 1000;     //Return value that will be read as random
        *AIcoins = 1000;    //Take all coins from a random pile
    } else {
        for (int i = 0; i < 3; i++) {
            if (numPairs[i] == 1) {
                numBadNimSum++;
            }
        }
        if (numBadNimSum == 3) {
            *AIcols = 1000;     //Return value that will be read as random
            *AIcoins = 1000;    //Take all coins from a random pile
        } else {
            for (int i = 0; i < 3; i++) {
                checkPileSize = numPairs[i] % 2;
                if (checkPileSize > 0){
                    for(int j = 0; j < cols; j++) {
                        col = checkPairsBadNim(x[j], 0, 0, 0, i + 1, j);
                        if (col != 1000){
                            *AIcols = col;
                            *AIcoins = i + 1;
                        }
                    }
                }
            }
        }
    }
}

void renameSave(char defaultSavePath[], char saveFileName[], char saveFilePathFull[], FILE *f, int saveOrRename, int badExit) {    // Rename the save file

    const char *invalidChars = "*?:\"<>\\/|'";

    char newSaveFileName[254];

    while (1) {
        printf("Por favor digite um nome para archivar o jogo: ");
        printf("> ");
        scanf("%s", newSaveFileName);

        // Check for invalid characters and length
        int isValid = 1;
        if (strlen(newSaveFileName) > 254) {
            isValid = 0;
            printf("O nome do arquivo é muito longo. Por favor, digite um nome com menos de 254 caracteres.\n");
        } else {
            for (int i = 0; i < strlen(newSaveFileName); i++) {
                if (strchr(invalidChars, newSaveFileName[i]) != NULL) {
                    isValid = 0;
                    break;
                }
            }
        }

        if (isValid) {
            break;
        } else {
            if (strlen(newSaveFileName) <= 254) {
                printf("O nome do arquivo contém caracteres inválidos.\n");
                printf("Caracteres inválidos: %s\n", invalidChars);
            }
        }
    }

    // Convert to camel case
    int len = strlen(newSaveFileName);
    int j = 0;
    int capitalizeNext = 0;

    for (int i = 0; i < len; i++) {
        if (newSaveFileName[i] == ' ' || newSaveFileName[i] == '_' || newSaveFileName[i] == '-') {
            capitalizeNext = 1;
        } else {
            if (capitalizeNext && newSaveFileName[i] >= 'a' && newSaveFileName[i] <= 'z') {
                newSaveFileName[j++] = newSaveFileName[i] - 'a' + 'A';
            } else if (!capitalizeNext && newSaveFileName[i] >= 'A' && newSaveFileName[i] <= 'Z') {
                newSaveFileName[j++] = newSaveFileName[i] - 'A' + 'a';
            } else {
                newSaveFileName[j++] = newSaveFileName[i];
            }
            capitalizeNext = 0;
        }
    }
    newSaveFileName[j] = '\0';

    char newSaveFilePathFull[254];
    strcpy(newSaveFilePathFull, defaultSavePath);
    strcat(newSaveFilePathFull, "/");
    strcat(newSaveFilePathFull, newSaveFileName);
    strcat(newSaveFilePathFull, ".save");

    if (rename(saveFilePathFull, newSaveFilePathFull) != 0) {
        perror("Error renaming file");
        fclose(f);
        exit(1);
    }

    int autoSaveReloadNameLenByte = 1;
    long unsigned int autoSaveReloadNameLenByteVal = strlen(newSaveFileName);
    fseek(f, autoSaveReloadNameLenByte, SEEK_SET);
    fwrite(&autoSaveReloadNameLenByteVal, sizeof(unsigned int), 1, f);
    
    // add bad exit state to initialize.ini
    int badExitByte = 0;
    unsigned char badExitByteVal = badExit;
    fseek(f, badExitByte, SEEK_SET);
    fwrite(&badExitByteVal, sizeof(unsigned char), 1, f);

    // add last save file name to initialize.ini
    int autoSaveReloadNameByte = 2;
    unsigned char autoSaveReloadNameByteVal[256];
    memset(autoSaveReloadNameByteVal, 0, sizeof(autoSaveReloadNameByteVal)); // Clear the buffer
    strcpy((char *)autoSaveReloadNameByteVal, saveFileName);
    strcpy(autoSaveReloadNameByteVal, saveFileName);
    fseek(f, autoSaveReloadNameByte, SEEK_SET);
    fwrite(&autoSaveReloadNameByteVal, sizeof(unsigned char), strlen((char*)autoSaveReloadNameByteVal), f);
    fflush(f);
    fclose(f);

    if (saveOrRename == 1) {
        printf("Jogo salvo com sucesso!\n");
    } else {
        printf("Jogo renomeado com sucesso!\n");
    }
}

void gameLoop(char defaultSavePath[], char saveFileName[], char saveFilePathFull[], FILE *f, int loadGame) {    // Game loop
    int freeMemory = 0;
    srand ( time(NULL) );
    FILE *newSaveFile = fopen(saveFilePathFull, "r+b");
    if (newSaveFile == NULL) {
        perror("Error opening file");
        exit(1);
    }

    if (loadGame == 0) {
        printf(" Por favor digite o modo que pretende:\n    1 - Humano X Humano\n    2 - Humano X Máquina\n    3 - Voltar ao Menu anterior\n");

        char gameModeUserInput;
        int gameMode;
        while (1) {
            printf("> ");
            scanf("%s", &gameModeUserInput);
            if (atoi(&gameModeUserInput) >= 1 && atoi(&gameModeUserInput) <= 3) {
                gameMode = atoi(&gameModeUserInput);
                break;
            } else {
                printf("Por favor digite um número válido!\n");
            }
            fflush(stdin);
        }


        int gameRound = 1;
        int initialRows = gameRound;
        switch (gameMode) {
            case 1:
                printf("Modo de jogo selecionado: Humano X Humano:\n");
                int gamemodeByte = 0;
                unsigned char gamemodeByteVal = 1;
                if (fseek(newSaveFile, gamemodeByte, SEEK_SET) != 0) {
                    perror("Error seeking in file");
                    fclose(newSaveFile);
                    exit(1);
                }
                if (fwrite(&gamemodeByteVal, sizeof(unsigned char), 1, newSaveFile) != 1) {
                    perror("Error writing to file");
                    fclose(newSaveFile);
                    exit(1);
                }
                fflush(newSaveFile);
                printf(" Quantas filas deseja no seu jogo?\n");

                char numColUserInput[254];
                int cols;
                while (1) {
                    printf("> ");
                    scanf("%s", numColUserInput);
                    if (isdigit(numColUserInput[0]) && atoi(numColUserInput) >= 2 && atoi(numColUserInput) <= 254) {
                        cols = atoi(numColUserInput);
                        break;
                    } else {
                        printf("Por favor digite um número válido!\n");
                    }
                }
                
                int initialCols = cols;
                int currentRow = 0;
                int currentCol = 0;
                int rows = initialRows;

                int **currentGameArray = allocate2DArray(initialRows, initialCols);

                printf("Deseja que as moedas sejam randomizadas? (Y/N)\n");

                char randomizeCoinsUserInput[254];
                while(1) {
                    printf("> ");
                    scanf("%s", randomizeCoinsUserInput);
                    if (randomizeCoinsUserInput[0] == 'Y' || randomizeCoinsUserInput[0] == 'y') {
                        for (int i = 0; i < cols; i++) {
                            addElementTo2DArray(&currentGameArray, &rows, &cols, &currentRow, &currentCol, rand() % 254);
                        }
                        break;
                    } else if (randomizeCoinsUserInput[0] == 'N' || randomizeCoinsUserInput[0] == 'n') {
                        for (int i = 0; i < cols; i++) {
                            while (1){
                                printf("Digite o valor da moeda %d: ", i + 1);
                                char coinValueUserInput;
                                int coinValue;
                                while (1) {
                                    printf("> ");
                                    scanf("%s", &coinValueUserInput);
                                    if (isdigit(coinValueUserInput)) {
                                        coinValue = atoi(&coinValueUserInput);
                                        break;
                                    } else {
                                        printf("Por favor digite um número válido!\n");
                                    }
                                }
                                if (coinValue >= 1 && coinValue <= 254) {
                                    addElementTo2DArray(&currentGameArray, &rows, &cols, &currentRow, &currentCol, coinValue);
                                    break;
                                } else {
                                    printf("Por favor digite um número entre 1 e 254!\n");
                                }
                            }
                        }
                        break;
                    } else {
                        printf("Por favor digite Y ou N!\n");
                    }
                }

                while (1) {
                    char *currentPlayer;
                    if (gameRound % 2 == 0) {
                        currentPlayer = "Jogador 2";
                        int lastPlayerByte = 1;
                        unsigned char lastPlayerByteVal = 2;
                        fseek(newSaveFile, lastPlayerByte, SEEK_SET);
                        fwrite(&lastPlayerByteVal, sizeof(unsigned char), 1, f);
                        fflush(newSaveFile);
                    } else {
                        currentPlayer = "Jogador 1";
                        int lastPlayerByte = 1;
                        unsigned char lastPlayerByteVal = 1;
                        fseek(newSaveFile, lastPlayerByte, SEEK_SET);
                        fwrite(&lastPlayerByteVal, sizeof(unsigned char), 1, f);
                        fflush(newSaveFile);
                    }
                    
                    printCurrentGame(currentGameArray, currentRow, cols, currentPlayer);

                    printf("\n");
                    printf("Digite a fila de moedas e o número de moedas a retirar (ex: 1 3) ou 'exit' para sair:\n");
                    while (1) {
                        int col;
                        int coins;
                        char userInputColsCoins[256];
                        printf("> ");
                        if (fgets(userInputColsCoins, sizeof(userInputColsCoins), stdin) == NULL) {
                            printf("Error reading input.\n");
                            continue;
                        }
                        // Remove trailing newline character if present
                        size_t len = strlen(userInputColsCoins);
                        if (len > 0 && userInputColsCoins[len - 1] == '\n') {
                            userInputColsCoins[len - 1] = '\0';
                        }

                        if (strlen(userInputColsCoins) == 0) {
                            continue;
                        }

                        if (strcmp(userInputColsCoins, "exit") == 0 || strcmp(userInputColsCoins, "Exit") == 0 || strcmp(userInputColsCoins, "EXIT") == 0) {
                            renameSave(defaultSavePath, saveFileName, saveFilePathFull, f, 1, 1);
                            freeMemory = 1;
                            printf("Deseja sair ou voltar ao menu?\n    1 - Sair\n    2 - Voltar ao Menu\n");
                            char exitOrMenu[256];
                            while (1) {
                                printf("> ");
                                scanf("%s", exitOrMenu);
                                if (strcmp(exitOrMenu, "1") == 0) {
                                    fclose(newSaveFile);
                                    exit(0);
                                } else if (strcmp(exitOrMenu, "2") == 0) {
                                    fclose(newSaveFile);
                                    return;
                                } else {
                                    printf("Por favor digite um número válido!\n");
                                }
                            }
                        } else {
                            // Parse the user input for column and coins
                            if (sscanf(userInputColsCoins, "%d %d", &col, &coins) != 2) {
                                printf("\nPor favor digite um número válido: ");
                                while (getchar() != '\n'); // Clear the input buffer
                                continue;
                            }

                            if (col >= 1 && col <= cols && coins >= 1 && coins <= 254) {
                                if (currentGameArray[gameRound - 1][col - 1] != 0) {
                                    addRowTo2DArrayWeirdCase(&currentGameArray, &rows, cols, &currentRow);
                                    if (currentGameArray[gameRound][col - 1] - coins < 0) {
                                        currentGameArray[gameRound][col - 1] = 0;
                                    } else {
                                        currentGameArray[gameRound][col - 1] -= coins;
                                    }
                                    break;
                                } else {
                                    printf("\nPor favor selecione uma fila com moedas: \n");
                                }
                            } else {
                                printf("\nPor favor digite um número válido: \n");
                            }
                        }
                    }
                    // Print the 2D array
                    gameRound++;
                    saveGameArray(saveFilePathFull, gameRound, cols, currentGameArray, gameMode);    // Save the game state
                    int sum = 0;
                    for (int i = 0; i < cols; i++) {
                        sum = sum + currentGameArray[gameRound - 1][i];
                    }
                    if (sum == 0) {
                        printf("Jogador %d perde!\n", (gameRound % 2) + 1);
                        printGameHistory(currentGameArray, rows, cols, gameRound, gameMode);
                        break;
                    }
                }

                fclose(newSaveFile);
                break;

            case 2:
                printf("Modo de jogo selecionado: Humano X Máquina\n");
                int AIcols = 0;
                int AIcoins = 0;
                gamemodeByte = 0;
                gamemodeByteVal = 2;

                if (fseek(newSaveFile, gamemodeByte, SEEK_SET) != 0) {
                    perror("Error seeking in file");
                    fclose(newSaveFile);
                    exit(1);
                }
                if (fwrite(&gamemodeByteVal, sizeof(unsigned char), 1, newSaveFile) != 1) {
                    perror("Error writing to file");
                    fclose(newSaveFile);
                    exit(1);
                }
                fflush(newSaveFile);
                printf(" Quantas filas deseja no seu jogo?\n");

                while (1) {
                    printf("> ");
                    scanf("%s", numColUserInput);
                    if (isdigit(numColUserInput[0]) && atoi(numColUserInput) >= 2 && atoi(numColUserInput) <= 254) {
                        cols = atoi(numColUserInput);
                        break;
                    } else {
                        printf("Por favor digite um número válido!\n");
                    }
                }
                
                initialCols = cols;
                currentRow = 0;
                currentCol = 0;
                rows = initialRows;

                int **currentAIGameArray = allocate2DArray(initialRows, initialCols);

                printf("Deseja que as moedas sejam randomizadas? (Y/N)\n");

                while(1) {
                    printf("> ");
                    scanf("%s", randomizeCoinsUserInput);
                    if (randomizeCoinsUserInput[0] == 'Y' || randomizeCoinsUserInput[0] == 'y') {
                        for (int i = 0; i < cols; i++) {
                            addElementTo2DArray(&currentAIGameArray, &rows, &cols, &currentRow, &currentCol, rand() % 254);
                        }
                        break;
                    } else if (randomizeCoinsUserInput[0] == 'N' || randomizeCoinsUserInput[0] == 'n') {
                        for (int i = 0; i < cols; i++) {
                            while (1){
                                printf("Digite o valor da moeda %d: ", i + 1);
                                char coinValueUserInput[254];
                                int coinValue;
                                while (1) {
                                    printf("> ");
                                    scanf("%s", coinValueUserInput);
                                    int isValid = 1;
                                    for (int j = 0; coinValueUserInput[j] != '\0'; j++) {
                                        if (!isdigit(coinValueUserInput[j])) {
                                            isValid = 0;
                                            break;
                                        }
                                    }
                                    if (isValid) {
                                        coinValue = atoi(coinValueUserInput);
                                        break;
                                    } else {
                                        printf("Por favor digite um número válido!\n");
                                    }
                                }
                                if (coinValue >= 1 && coinValue <= 254) {
                                    addElementTo2DArray(&currentAIGameArray, &rows, &cols, &currentRow, &currentCol, coinValue);
                                    break;
                                } else {
                                    printf("Por favor digite um número entre 1 e 254!\n");
                                }
                            }
                        }
                        break;
                    } else {
                        printf("Por favor digite Y ou N!\n");
                    }
                }

                while (1) {
                    char *currentPlayer;
                    
                    
                    if (gameRound % 2 == 0) {
                        currentPlayer = "Computador";
                        int lastPlayerByte = 1;
                        unsigned char lastPlayerByteVal = 3;
                        fseek(newSaveFile, lastPlayerByte, SEEK_SET);
                        fwrite(&lastPlayerByteVal, sizeof(unsigned char), 1, f);
                        fflush(newSaveFile);

                        AI(&AIcols, &AIcoins, currentAIGameArray, rows, cols);
                        if (AIcols > 999) {
                            // Pick a random non-zero column
                            int nonZeroCols[cols];
                            int nonZeroCount = 0;
                            for (int i = 0; i < cols; i++) {
                                if (currentAIGameArray[rows - 1][i] != 0) {
                                    nonZeroCols[nonZeroCount++] = i;
                                }
                            }
                            if (nonZeroCount > 0) {
                                AIcols = nonZeroCols[rand() % nonZeroCount];
                            }
                        }

                        // Ensure AI never takes 0 coins
                        if (AIcoins == 0) {
                            AIcoins = 1;
                        }

                        // Check if there is only one column with coins and if removing AIcoins would leave it empty
                        int nonZeroCount = 0;
                        int lastNonZeroCol = -1;
                        for (int i = 0; i < cols; i++) {
                            if (currentAIGameArray[rows - 1][i] != 0) {
                                nonZeroCount++;
                                lastNonZeroCol = i;
                            }
                        }
                        if (nonZeroCount == 1 && currentAIGameArray[rows - 1][lastNonZeroCol] - AIcoins <= 0) {
                            AIcoins = currentAIGameArray[rows - 1][lastNonZeroCol];
                        }

                        addRowTo2DArrayWeirdCase(&currentAIGameArray, &rows, cols, &currentRow);
                        printf("O computador selecionou a coluna %d e retirou %d moedas\n", AIcols + 1, AIcoins);
                        if (currentAIGameArray[rows - 1][AIcols] - AIcoins < 0) {
                            currentAIGameArray[rows - 1][AIcols] = 0;
                        } else {
                            currentAIGameArray[rows - 1][AIcols] -= AIcoins;
                        }

                    } else {
                        currentPlayer = "Jogador 1";
                        int lastPlayerByte = 1;
                        unsigned char lastPlayerByteVal = 1;
                        fseek(newSaveFile, lastPlayerByte, SEEK_SET);
                        fwrite(&lastPlayerByteVal, sizeof(unsigned char), 1, f);
                        fflush(newSaveFile);

                        printCurrentGame(currentAIGameArray, currentRow, cols, currentPlayer);

                        printf("\n");
                        printf("Digite a fila de moedas e o número de moedas a retirar (ex: 1 3) ou 'exit' para sair:\n");
                        while (1) {
                            int col;
                            int coins;
                            char userInputColsCoins[256];
                            printf("> ");
                            if (fgets(userInputColsCoins, sizeof(userInputColsCoins), stdin) == NULL) {
                                printf("Error reading input.\n");
                                continue;
                            }
                            // Remove trailing newline character if present
                            size_t len = strlen(userInputColsCoins);
                            if (len > 0 && userInputColsCoins[len - 1] == '\n') {
                                userInputColsCoins[len - 1] = '\0';
                            }

                            if (strlen(userInputColsCoins) == 0) {
                                continue;
                            }
                            printf("userInputColsCoins: %s\n", userInputColsCoins);

                            if (strcmp(userInputColsCoins, "exit") == 0 || strcmp(userInputColsCoins, "Exit") == 0 || strcmp(userInputColsCoins, "EXIT") == 0) {
                                renameSave(defaultSavePath, saveFileName, saveFilePathFull, f, 1, 1);
                                freeMemory = 1;
                                printf("Deseja sair ou voltar ao menu?\n    1 - Sair\n    2 - Voltar ao Menu\n");
                                char exitOrMenu[256];
                                while (1) {
                                    printf("> ");
                                    scanf("%s", exitOrMenu);
                                    if (strcmp(exitOrMenu, "1") == 0) {
                                        fclose(newSaveFile);
                                        exit(0);
                                    } else if (strcmp(exitOrMenu, "2") == 0) {
                                        fclose(newSaveFile);
                                        return;
                                    } else {
                                        printf("Por favor digite um número válido!\n");
                                    }
                                }
                            } else {
                                // Parse the user input for column and coins
                                if (sscanf(userInputColsCoins, "%d %d", &col, &coins) != 2) {
                                    printf("\nPor favor digite um número válido: ");
                                    while (getchar() != '\n'); // Clear the input buffer
                                    continue;
                                }

                                if (col >= 1 && col <= cols && coins >= 1 && coins <= 254) {
                                    if (currentAIGameArray[gameRound - 1][col - 1] != 0) {
                                        addRowTo2DArrayWeirdCase(&currentAIGameArray, &rows, cols, &currentRow);
                                        if (currentAIGameArray[gameRound][col - 1] - coins < 0) {
                                            currentAIGameArray[gameRound][col - 1] = 0;
                                        } else {
                                            currentAIGameArray[gameRound][col - 1] -= coins;
                                        }
                                        break;
                                    } else {
                                        printf("\nPor favor selecione uma fila com moedas: \n");
                                    }
                                } else {
                                    printf("\nPor favor digite um número válido: \n");
                                }
                            }
                        }
                    }

                    // Print the 2D array
                    gameRound++;
                    saveGameArray(saveFilePathFull, gameRound, cols, currentAIGameArray, gameMode);    // Save the game state
                    int sum = 0;
                    for (int i = 0; i < cols; i++) {
                        sum = sum + currentAIGameArray[gameRound - 1][i];
                    }
                    if (sum == 0) {
                        if (gameRound % 2 == 0) {
                            printf("Computador ganha!\n");
                        } else {
                            printf("Tu ganhast!\n");
                        }
                        printGameHistory(currentAIGameArray, rows, cols, gameRound, gameMode);
                        break;
                    }
                }


                fclose(newSaveFile);        //NOTE MATRIX FREEING IS BUGGED NO IDEA WHY, THEREFORE I AM RELING ON EXIT TO FREE MEMORY
                break;
            case 3:
                break;
            default:
                printf("Error ! - game mode selector\n");
                exit(1);
        }
    } else {
        int rows = 0;
        int cols = 0;
        int lastPlayer = 0;
        int gameMode = 0;
        int returnToMainMenu = 0;

        int **loadSaveArray = loadGameArray(saveFilePathFull, &rows, &cols, &gameMode, &lastPlayer);

        int checkFinishedGameSum = 0;
        for (int i = 0; i < cols; i++) {
            checkFinishedGameSum = checkFinishedGameSum + loadSaveArray[rows - 1][i];
        }
        if (checkFinishedGameSum == 0) {
            printf("O jogo carregado [ %s ] já terminou\n", saveFileName);
            printf("Gostaria de ver a história do jogo? (Y/N)\n");
            while (1) {
                char seeGameHistoryUserInput[254];
                printf("> ");
                scanf("%s", seeGameHistoryUserInput);
                if (seeGameHistoryUserInput[0] == 'Y' || seeGameHistoryUserInput[0] == 'y') {
                    printGameHistory(loadSaveArray, rows, cols, rows, gameMode);
                    printf("\nGostaria de:\n     1 - Voltar ao Menu Principal\n     2 - Sair\n");
                    while (1) {
                        char returnToMainMenuUserInput[254];
                        printf("> ");
                        scanf("%s", returnToMainMenuUserInput);
                        if (atoi(returnToMainMenuUserInput) == 1) {
                            returnToMainMenu = 1;
                            break;
                        } else if (atoi(returnToMainMenuUserInput) == 2) {
                            exit(1);
                        } else {
                            printf("Por favor digite um número válido!\n");
                        }
                    }
                    break;
                } else if (seeGameHistoryUserInput[0] == 'N' || seeGameHistoryUserInput[0] == 'n') {
                    printf("\nGostaria de:\n    1 - Voltar ao Menu Principal\n     2 - Sair\n");
                    while (1) {
                        char returnToMainMenuUserInput[254];
                        printf("> ");
                        scanf("%s", returnToMainMenuUserInput);
                        if (atoi(returnToMainMenuUserInput) == 1) {
                            returnToMainMenu = 1;
                            break;
                        } else if (atoi(returnToMainMenuUserInput) == 2) {
                            exit(1);
                        } else {
                            printf("Por favor digite um número válido!\n");
                        }
                    }
                    break;
                } else {
                    printf("Por favor digite Y ou N!\n");
                }
                break;
            }
        }

        if (returnToMainMenu == 0) {
            printGameHistory(loadSaveArray, rows, cols, rows, gameMode);

            switch(gameMode) {
                case 1:
                    while (1) {
                        char *currentPlayer;
                        int currentRow = rows - 1;
                        if (lastPlayer == 2) {
                            currentPlayer = "Jogador 2";
                        } else {
                            currentPlayer = "Jogador 1";
                        }
                        
                        printCurrentGame(loadSaveArray, rows, cols, currentPlayer);

                        printf("\n");
                        printf("Digite a fila de moedas e o número de moedas a retirar (ex: 1 3) ou 'exit' para sair: ");
                        while (1) {
                            int col;
                            int coins;
                            char loadSaveUserInputColsCoins[256];

                            scanf("%s", loadSaveUserInputColsCoins);

                            if (strcmp(loadSaveUserInputColsCoins, "exit") == 0) {
                                renameSave(defaultSavePath, saveFileName, saveFilePathFull, f, 1, 1);
                                freeMemory = 1;
                                printf("Deseja sair ou voltar ao menu?\n    1 - Sair\n    2 - Voltar ao Menu\n");
                                char exitOrMenu[256];
                                while (1) {
                                    printf("> ");
                                    scanf("%s", exitOrMenu);
                                    if (strcmp(exitOrMenu, "1") == 0) {
                                        fclose(newSaveFile);
                                        exit(0);
                                    } else if (strcmp(exitOrMenu, "2") == 0) {
                                        fclose(newSaveFile);
                                        return;
                                    } else {
                                        printf("Por favor digite um número válido!\n");
                                    }
                                }
                            } else {
                                // Parse the user input for column and coins
                                if (sscanf(loadSaveUserInputColsCoins, "%d %d", &col, &coins) != 2) {
                                    printf("\nPor favor digite um número válido: ");
                                    while (getchar() != '\n'); // Clear the input buffer
                                    continue;
                                }

                                if (col >= 1 && col <= cols && coins >= 1 && coins <= 254) {
                                    if (loadSaveArray[rows - 1][col - 1] != 0) {
                                        addRowTo2DArray(&loadSaveArray, &rows, cols);
                                        if (loadSaveArray[rows - 1][col - 1] - coins < 0) {
                                            loadSaveArray[rows - 1][col - 1] = 0;
                                        } else {
                                            loadSaveArray[rows - 1][col - 1] -= coins;
                                        }
                                        break;
                                    } else {
                                        printf("\nPor favor selecione uma fila com moedas: ");
                                    }
                                } else {
                                    printf("\nPor favor digite um número válido: ");
                                }
                            }
                        }
                        // Print the 2D array
                        rows++;
                        saveGameArray(saveFilePathFull, rows - 1, cols - 1, loadSaveArray, gameMode);    // Save the game state
                        int sum = 0;
                        for (int i = 0; i < cols; i++) {
                            sum = sum + loadSaveArray[rows - 1][i];
                        }
                        if (sum == 0) {
                            printf("Jogador %d ganha!\n", (rows % 2) + 1);
                            printGameHistory(loadSaveArray, rows, cols, rows, gameMode);
                            break;
                        }
                    }

                    fclose(newSaveFile);
                    break;

                case 2:
                    int gameRound = rows;
                    int AIcols = 0;
                    int AIcoins = 0;
                    while (1) {
                        char *currentPlayer;
                        int currentRow = rows - 1;
                        
                        if (gameRound % 2 == 0) {
                            currentPlayer = "Computador";
                            int lastPlayerByte = 1;
                            unsigned char lastPlayerByteVal = 3;
                            fseek(newSaveFile, lastPlayerByte, SEEK_SET);
                            fwrite(&lastPlayerByteVal, sizeof(unsigned char), 1, f);
                            fflush(newSaveFile);

                            AI(&AIcols, &AIcoins, loadSaveArray, rows, cols);
                            if (AIcols > 999) {
                                // Pick a random non-zero column
                                int nonZeroCols[cols];
                                int nonZeroCount = 0;
                                for (int i = 0; i < cols; i++) {
                                    if (loadSaveArray[rows - 1][i] != 0) {
                                        nonZeroCols[nonZeroCount++] = i;
                                    }
                                }
                                if (nonZeroCount > 0) {
                                    AIcols = nonZeroCols[rand() % nonZeroCount];
                                }
                            }

                            // Ensure AI never takes 0 coins
                            if (AIcoins == 0) {
                                AIcoins = 1;
                            }

                            // Check if there is only one column with coins and if removing AIcoins would leave it empty
                            int nonZeroCount = 0;
                            int lastNonZeroCol = -1;
                            for (int i = 0; i < cols; i++) {
                                if (loadSaveArray[rows - 1][i] != 0) {
                                    nonZeroCount++;
                                    lastNonZeroCol = i;
                                }
                            }
                            if (nonZeroCount == 1 && loadSaveArray[rows - 1][lastNonZeroCol] - AIcoins <= 0) {
                                AIcoins = loadSaveArray[rows - 1][lastNonZeroCol];
                            }

                            addRowTo2DArray(&loadSaveArray, &rows, cols);
                            printf("O computador selecionou a coluna %d e retirou %d moedas\n", AIcols + 1, AIcoins);
                            if (loadSaveArray[rows - 1][AIcols] - AIcoins < 0) {
                                loadSaveArray[rows - 1][AIcols] = 0;
                            } else {
                                loadSaveArray[rows - 1][AIcols] -= AIcoins;
                            }

                        } else {
                            currentPlayer = "Jogador 1";
                            int lastPlayerByte = 1;
                            unsigned char lastPlayerByteVal = 1;
                            fseek(newSaveFile, lastPlayerByte, SEEK_SET);
                            fwrite(&lastPlayerByteVal, sizeof(unsigned char), 1, f);
                            fflush(newSaveFile);
                            
                            printCurrentGame(loadSaveArray, rows, cols, currentPlayer);

                            printf("\n");
                            printf("Digite a fila de moedas e o número de moedas a retirar (ex: 1 3): ");
                            while (1) {
                                int col;
                                int coins;
                                if (scanf("%d %d", &col, &coins) != 2) {
                                    printf("\nPor favor digite um número válido: ");
                                    while (getchar() != '\n'); // Clear the input buffer
                                    continue;
                                }
                                if (col >= 1 && col <= cols && coins >= 1 && coins <= 254) {
                                    if (loadSaveArray[rows - 1][col - 1] != 0) {
                                        addRowTo2DArray(&loadSaveArray, &rows, cols);
                                        if (loadSaveArray[rows - 1][col - 1] - coins < 0) {
                                            loadSaveArray[rows - 1][col - 1] = 0;
                                        } else {
                                            loadSaveArray[rows - 1][col - 1] -= coins;
                                        }
                                        break;
                                    } else {
                                        printf("\nPor favor selecione uma fila com moedas: ");
                                    }
                                } else {
                                    printf("\nPor favor digite um número válido: ");
                                }
                            }
                        }

                        // Print the 2D array
                        gameRound++;
                        saveGameArray(saveFilePathFull, rows, cols, loadSaveArray, gameMode);    // Save the game state

                        while (1) {
                            int sum = 0;
                            for (int i = 0; i < cols; i++) {
                                sum = sum + loadSaveArray[rows - 1][i];
                            }
                            if (sum == 0) {
                                printf("Jogador %d ganho!\n", (rows % 2) + 1);
                                printGameHistory(loadSaveArray, rows, cols, rows, gameMode);

                                printf("Pretende guardar o jogo ou eliminá-lo?\n     1 - Guardar\n     2 - Apagar\n");
                                char saveOrDelete[256];
                                while (1) {
                                    printf("> ");
                                    scanf("%s", saveOrDelete);
                                    if (strcmp(saveOrDelete, "1") == 0) {
                                        renameSave(defaultSavePath, saveFileName, saveFilePathFull, f, 1, 1);
                                        freeMemory = 1;
                                        printf("Jogo guardado com sucesso!\n");
                                        break;
                                    } else if (strcmp(saveOrDelete, "2") == 0) {
                                        remove(saveFilePathFull);
                                        printf("Jogo eliminado com sucesso!\n");
                                        break;
                                    } else {
                                        printf("Por favor digite um número válido!\n");
                                    }
                                }
                                break;
                            }
                        }
                    }

                    if (freeMemory == 0){
                        free(loadSaveArray);
                        fclose(newSaveFile);
                    }
                    break;
            }
        }
    }
}

void reloadSave(char defaultSavePath[]) {   // reload save file
    struct dirent *entry;
    DIR *dp = opendir(defaultSavePath);
    if (dp == NULL) {
        perror("Error opening directory");
        return;
    }

    char **saveFiles = NULL;
    int saveFileCount = 0;
    int saveFileCapacity = 10;
    saveFiles = malloc(saveFileCapacity * sizeof(char *));
    if (saveFiles == NULL) {
        perror("Error allocating memory");
        closedir(dp);
        return;
    }

    // Read directory entries
    while ((entry = readdir(dp)) != NULL) {
        char filePath[512];
        snprintf(filePath, sizeof(filePath), "%s/%s", defaultSavePath, entry->d_name);

        struct stat fileStat;
        if (stat(filePath, &fileStat) == 0 && S_ISREG(fileStat.st_mode)) { // Check if it's a regular file
            const char *ext = strrchr(entry->d_name, '.');
            if (ext && strcmp(ext, ".save") == 0) {
                if (saveFileCount >= saveFileCapacity) {
                    saveFileCapacity *= 2;
                    saveFiles = realloc(saveFiles, saveFileCapacity * sizeof(char *));
                    if (saveFiles == NULL) {
                        perror("Error reallocating memory");
                        closedir(dp);
                        return;
                    }
                }
                saveFiles[saveFileCount] = strdup(entry->d_name);
                if (saveFiles[saveFileCount] == NULL) {
                    perror("Error duplicating string");
                    closedir(dp);
                    return;
                }
                saveFileCount++;
            }
        }
    }
    closedir(dp);

    if (saveFileCount == 0) {
        printf("Nenhum ficheiro .save encontrado no diretório.\n");
        free(saveFiles);
        return;
    }

    // Print the list of .save files
    printf("Lista de ficheiros .save:\n");
    for (int i = 0; i < saveFileCount; i++) {
        printf("%d. %s\n", i + 1, saveFiles[i]);
    }

    // Allow the user to choose a file
    int choice;
    while (1) {
        printf("Introduza o número do ficheiro que pretende abrir:");
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > saveFileCount) {
            printf("Escolha inválida. Por favor introduza um número válido.\n");
            while (getchar() != '\n'); // Clear the input buffer
        } else {
            break;
        }
    }

    // Open the chosen file
    char filePath[512];
    snprintf(filePath, sizeof(filePath), "%s/%s", defaultSavePath, saveFiles[choice - 1]);
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
    } else {
        gameLoop(defaultSavePath, saveFiles[choice - 1], filePath, file, 1);
        fclose(file);
    }

    // Free allocated memory
    for (int i = 0; i < saveFileCount; i++) {
        free(saveFiles[i]);
    }
    free(saveFiles);
}

void reloadAutoSave(char defaultSavePath[], FILE *f) {  // reload autosave file

    int nameStartByte = 1;

    // Seek to the desired position
    if (fseek(f, nameStartByte, SEEK_SET) != 0) {
        perror("Error seeking in file");
        fclose(f);
        exit(1);
    }

    // Get the size of the file
    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    fseek(f, nameStartByte, SEEK_SET);

    // Calculate the number of bytes to read
    long bytesToRead = fileSize - nameStartByte;

    // Allocate memory for the string
    char *buffer = malloc(bytesToRead + 1);
    if (buffer == NULL) {
        perror("Error allocating memory");
        fclose(f);
        exit(1);
    }

    // Read the bytes into the buffer
    size_t bytesRead = fread(buffer, 1, bytesToRead, f);
    if (bytesRead != bytesToRead) {
        perror("Error reading file");
        free(buffer);
        fclose(f);
        exit(1);
    }

    // Null-terminate the string
    buffer[bytesRead] = '\0';
    char defaultSavePathFull[254];
    strcpy(defaultSavePathFull, defaultSavePath);
    strcat(defaultSavePathFull, "/");
    strcat(defaultSavePathFull, buffer);
    strcat(defaultSavePathFull, ".save");

    gameLoop(defaultSavePath, buffer, defaultSavePathFull, f, 1);
}

int checkCurruptInitializeFile(FILE *f, char *filePath) {       // here bcs of a bug i cant figure out and ive spent too much time trying to solve it to no avail so....
    int nameStartByte = 2;
    int nameLenghtByte = 1;
    int nameLenghtVal = 0;

    fseek(f, nameLenghtByte, SEEK_SET);
    fread(&nameLenghtVal, sizeof(__int8_t), 1, f);

    if (nameLenghtVal == 255) {
        return 0;
    }

    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    fseek(f, nameStartByte, SEEK_SET);
    long bytesToRead = fileSize - nameStartByte;

    if (bytesToRead != nameLenghtVal) {        //file currupt
        printf("+-------------------------------------[ AVISO ]-------------------------------------+\n");
        printf("|                O ficheiro de arranque foi detectado como corrompido               |\n");
        printf("|                      Um novo ficheiro de arranque será criado                     |\n");
        printf("|            O último auto-save não poderá ser carregado automaticamente            |\n");
        printf("| Mas pode ser carregado manualmente a partir da pasta guardada em newSaveFile.save |\n");
        printf("\n");
        
        remove(filePath);
        fclose(f);

        f = fopen(filePath, "w+b");

        if (f == NULL) {    // Error creating file
            perror("Error creating file!\n");
            exit(1);
        }

        unsigned char fillerData[7];
        for (int i = 0; i < 7; i++) {   // Write 7 filler bytes
            fillerData[i] = 0xFF; 
        }

        fwrite(fillerData, sizeof(unsigned char), 7, f);
        fflush(f);
        fclose(f);

        printf("|        Um novo ficheiro de arranque foi criado, o jogo será agora fechado         |\n");
        printf("+-----------------------------------------------------------------------------------+\n");

        exit(1);  
    }
    return 0;
}

void newGame(char defaultSavePath[], FILE *f) {     // new game

    char saveFileName[] = "newSaveFile";

    char saveFileNameFull[256];
    strcpy(saveFileNameFull, "/");
    strcat(saveFileNameFull, saveFileName);
    strcat(saveFileNameFull, ".save");

    char saveFilePathFull[512];
    strcpy(saveFilePathFull, defaultSavePath);
    strcat(saveFilePathFull, saveFileNameFull);
    
    // Check for unnamed new save file
    FILE *newSaveFile = fopen(saveFilePathFull, "r+b");
    if (newSaveFile != NULL) {    // Existing new save file found
        unsigned char newSaveFileByte;
        int isFiller = 1; // Assume all bytes are filler
        while (fread(&newSaveFileByte, sizeof(unsigned char), 1, newSaveFile) == 1) {
            if (newSaveFileByte != 0xFF) {
                isFiller = 0; // Found a non-filler byte
                break;
            }
        }
        fclose(newSaveFile);

        if (!isFiller) {
            printf("O ficheiro guardado já existe, deseja substituí-lo? (Y/N)\n");
            char overwriteUserInput[10];
            while (1) {
                printf("> ");
                scanf("%s", overwriteUserInput);
                if (overwriteUserInput[0] == 'Y' || overwriteUserInput[0] == 'y') {
                    newSaveFile = fopen(saveFilePathFull, "w+b");
                    if (newSaveFile == NULL) {    // Error creating file
                        perror("Error creating file!  - file does not exist\n");
                        exit(1);
                    }

                    unsigned char fillerData[5];
                    for (int i = 0; i < 5; i++) {   // Write 5 filler bytes
                        fillerData[i] = 0xFF; 
                    }
                    fwrite(fillerData, sizeof(unsigned char), 5, newSaveFile);
                    fflush(newSaveFile);
                    fclose(newSaveFile);
                    printf("Ficheiro salvo antigo substituído.\n");
                    break;
                } else if (overwriteUserInput[0] == 'N' || overwriteUserInput[0] == 'n') {
                    renameSave(defaultSavePath, saveFileName, saveFilePathFull, f, 1, 0);
                    newSaveFile = fopen(saveFilePathFull, "r+b");
                    break;
                } else {
                    printf("Entrada inválida. Por favor introduza Y ou N.\n");
                }
            }
        }
    } else {    // File does not exist, create it
        newSaveFile = fopen(saveFilePathFull, "w+b");
        if (newSaveFile == NULL) {    // Error creating file
            printf("Error creating file!  - file does not exist create it\n");
            exit(1);
        }

        unsigned char fillerData[5];
        for (int i = 0; i < 5; i++) {   // Write 5 filler bytes
            fillerData[i] = 0xFF; 
        }
        fwrite(fillerData, sizeof(unsigned char), 5, newSaveFile);
        fflush(newSaveFile);
        fclose(newSaveFile);
    }

    int autoSaveReloadNameLenByte = 1;
    long unsigned int autoSaveReloadNameLenByteVal = strlen(saveFileName);
    fseek(f, autoSaveReloadNameLenByte, SEEK_SET);
    fwrite(&autoSaveReloadNameLenByteVal, sizeof(unsigned int), 1, f);
    
    // add bad exit state to initialize.ini
    int badExitByte = 0;
    unsigned char badExitByteVal = 2;
    fseek(f, badExitByte, SEEK_SET);
    fwrite(&badExitByteVal, sizeof(unsigned char), 1, f);

    // add last save file name to initialize.ini
    int autoSaveReloadNameByte = 2;
    unsigned char autoSaveReloadNameByteVal[256];
    memset(autoSaveReloadNameByteVal, 0, sizeof(autoSaveReloadNameByteVal)); // Clear the buffer
    strcpy((char *)autoSaveReloadNameByteVal, saveFileName);
    strcpy(autoSaveReloadNameByteVal, saveFileName);
    fseek(f, autoSaveReloadNameByte, SEEK_SET);
    fwrite(&autoSaveReloadNameByteVal, sizeof(unsigned char), strlen((char*)autoSaveReloadNameByteVal), f);
    fflush(f);
    fclose(f);

    gameLoop(defaultSavePath, saveFileName, saveFilePathFull, f, 0);
}

void menu(char defaultSavePath[], FILE *f, int returningToMenu) {       //menu
    char menuUserInput;
    int menuOption;
    while (1) {
        if (returningToMenu == 1) {
            printf("\n");
        }
        printf("Bem-vindo ao Jogo da Moeda!\nPor Favor digite a opção que pretende:\n    1 - Novo Jogo\n    2 - Continuar Jogo Guardado\n    3 - Ajuda\n    4 - Sair\n");
        
        while (1) {
            printf("> ");
            scanf("%s", &menuUserInput);
            if (atoi(&menuUserInput) >= 1 && atoi(&menuUserInput) <= 4) {
                menuOption = atoi(&menuUserInput);
                break;
            } else {
                printf("Por favor digite um número válido!\n");
            }
            fflush(stdin);
        }

        switch (menuOption) {
            case 1:
                newGame(defaultSavePath, f);
                break;
            case 2:
                reloadSave(defaultSavePath);
                break;
            case 3:
                printf("+----------------------------------------[ Visão Geral do Jogo ]--------------------------------------+\n");
                printf("|     O jogo é uma variação do jogo Nim, onde os jogadores se revezam a remover moedas das filas.     |\n");
                printf("|                     O objetivo é não ser o jogador que remove a última moeda.                       |\n");
                printf("+-----------------------------------------------------------------------------------------------------+\n");

                printf("| Modos de Jogo\n");
                printf("| Existem dois modos de jogo:\n");
                printf("|     1. Humano vs. Humano: Dois jogadores revezam-se.\n");
                printf("|     2. Humano vs. Computador: O jogador compete contra o computador.\n+\n");

                printf("| Iniciar o Jogo\n");
                printf("|   1. Menu Principal: Quando iniciar o jogo, verá o menu principal com as seguintes opções:\n");
                printf("|        1: Iniciar um Novo Jogo\n");
                printf("|        2: Continuar um Jogo Guardado\n");
                printf("|        3: Ajuda\n");
                printf("|        4: Sair\n|\n");

                printf("|   2. Novo Jogo: Se escolher iniciar um novo jogo, será solicitado a selecionar o modo de jogo:\n");
                printf("|        1: Humano vs. Humano\n");
                printf("|        2: Humano vs. Computador\n");
                printf("|        3: Voltar ao Menu Principal\n|\n");

                printf("|   3. Número de Filas: Será solicitado a introduzir o número de filas para o jogo (entre 2 e 254).\n|\n");

                printf("|   4. Randomizar Moedas: Será perguntado se deseja que as moedas sejam randomizadas em cada fila. Se\n|                      escolher 'Y', as moedas serão distribuídas aleatoriamente. Se escolher 'N', será solicitado a introduzir\n|                      o número de moedas para cada fila.\n+\n");

                printf("| Jogar o Jogo\n");
                printf("|   1. Turnos: Os jogadores revezam-se a remover moedas de uma fila. O jogador atual é exibido no\n|             início de cada turno.\n|\n");

                printf("|   2. Fazer uma Jogada: No seu turno, será solicitado a introduzir o número da fila e o número de\n|                     moedas a remover (por exemplo, \"1 3\" para remover 3 moedas da fila 1). Também pode digitar \"exit\" para sair do jogo.\n|\n");

                printf("|   3. Jogadas Válidas: Deve introduzir um número de fila válido e um número válido de moedas a remover.\n|                   A coluna deve ter moedas suficientes para remover o número especificado.\n|\n");

                printf("|   4. Jogadas do Computador: Se estiver a jogar contra o computador, o computador fará a sua jogada automaticamente.\n+\n");

                printf("| Terminar o Jogo\n");
                printf("|   1. Vencer: O jogo termina quando todas as moedas são removidas. O jogador que remove a última moeda vence.\n|\n");

                printf("|   2. Guardar ou Apagar: Quando o jogo termina, será solicitado a guardar ou apagar o jogo:\n");
                printf("|        1: Guardar o jogo\n");
                printf("|        2: Apagar o jogo\n+\n");

                printf("| Sair do Jogo\n");
                printf("|   1. Sair Durante o Turno: Pode digitar \"exit\" em qualquer prompt para sair do jogo.\n|                        Será perguntado se deseja guardar o jogo antes de sair.\n");
                printf("|   2. Sair do Menu Principal: Pode escolher a opção \"Sair\" do menu principal para sair do jogo.\n\n");
            case 4:
                int saveStateByte = 0;
                unsigned char saveStateByteVal = 1;
                fseek(f, saveStateByte, SEEK_SET);
                fwrite(&saveStateByteVal, sizeof(unsigned char), 1, f);
                fflush(f);
                exit(0);
                break;
            default:
                printf("Error !\n");
                exit(1);
                break;
        }
    }
}

void intialize() {      // initialize
    char defaultSavePath[256] = "saves";
    mkdir(defaultSavePath, 0777);   // create saves directory
    char filePath[256];
    strcpy(filePath, defaultSavePath);
    strcat(filePath, "/initialize.ini");
    FILE *f = fopen(filePath, "r+b");   // open file for read/write
    if (f == NULL) {    // File doesn't exist, create it
        f = fopen(filePath, "w+b");

        if (f == NULL) {    // Error creating file
            perror("Error creating file!\n");
            exit(1);
        }

        unsigned char fillerData[7];
        for (int i = 0; i < 7; i++) {   // Write 7 filler bytes
            fillerData[i] = 0xFF; 
        }
        fwrite(fillerData, sizeof(unsigned char), 7, f);
        fflush(f);
    }

    // Get file size
    fseek(f, 0L, SEEK_END);
    long int intializeSize = ftell(f); 

    if (intializeSize > 0) {    // bad exit, save reload check
        checkCurruptInitializeFile(f, filePath);
        // byte reading
        unsigned int exitStateByte = 0;
        fseek(f, exitStateByte, SEEK_SET);
        unsigned char exitStateByteVal;
        fread(&exitStateByteVal, sizeof(unsigned char), 1, f);
        if (exitStateByteVal == 1 || exitStateByteVal == 255) {    // good exit, continue to menu  DEBUG TESTING REMOVE exitStateByteVal == 2 before release push
            menu(defaultSavePath, f, 0);    // continue to menu
        } else {    // bad exit, save reload prompt
            reloadAutoSave(defaultSavePath, f);
            menu(defaultSavePath, f, 1);    // continue to menu
        }
    }
}

void main() {
    intialize();
}