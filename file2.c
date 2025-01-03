#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function declare
void readQuestionsFromFile( char *filename, char questions[][100], char options[][4][50], int correctAnswers[], int numQuestions);//Reads questions, options, and answers from a file.
int play(char questions[][100], char options[][4][50], int correctAnswers[], int numQuestions);// logic for asking questions and answers.

int main() {
    srand(time(NULL));//random number generation

    int numQuestions = 10;
    char physicsQuestions[10][100], chemistryQuestions[10][100], gkQuestions[10][100];
    char physicsOptions[10][4][50], chemistryOptions[10][4][50], gkOptions[10][4][50];
    int physicsAnswers[10], chemistryAnswers[10], gkAnswers[10];

    // Read questions and answers for each section
    readQuestionsFromFile("physics.txt", physicsQuestions, physicsOptions, physicsAnswers, numQuestions);
    readQuestionsFromFile("chemistry.txt", chemistryQuestions, chemistryOptions, chemistryAnswers, numQuestions);
    readQuestionsFromFile("gk.txt", gkQuestions, gkOptions, gkAnswers, numQuestions);

    printf("Welcome to Kus Bani Crorepati!\n");
    printf("Choose your section:\n1. Physics\n2. Chemistry\n3. General Knowledge\n");

    int section;
    printf("Enter your choice (1/2/3): ");
    scanf("%d", &section);
    //using switchcase for section selection
    int score = 0;
    switch (section) {
        case 1:
            score = play(physicsQuestions, physicsOptions, physicsAnswers, numQuestions);
            break;
        case 2:
            score = play(chemistryQuestions, chemistryOptions, chemistryAnswers, numQuestions);
            break;
        case 3:
            score = play(gkQuestions, gkOptions, gkAnswers, numQuestions);
            break;
        default:
            printf("Invalid choice! Exiting...\n");
            return 1;
    }

    printf("\nYour total score is: %d\n", score);
    if (score == 80) {
        printf("Congratulations! You've won the game!\n");
    } else {
        printf("Better luck next time!\n");
    }

    return 0;
}

void readQuestionsFromFile( char *filename, char questions[][100], char options[][4][50], int correctAnswers[], int numQuestions) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open %s\n", filename);
        exit(1);
    }

    char line[200];
    for (int i = 0; i < numQuestions; i++) {
        // Read the question
        if (fscanf(file, " %[^\n]", questions[i]) != 1) {
            printf("Error reading question %d from %s\n", i + 1, filename);
            break;
        }

        // Read the options
        if (fscanf(file, " %[^\n]", line) != 1) {
            printf("Error reading options for question %d from %s\n", i + 1, filename);
            break;
        }

        char *token = strtok(line, "|");
        for (int j = 0; j < 4; j++) {
        if (token == NULL) {
        break;  // Exit the loop if token is NULL
        }
        strncpy(options[i][j], token, 50);
        token = strtok(NULL, "|");
}


        // Read the correct answer
        if (fscanf(file, "%d", &correctAnswers[i]) != 1) {
            printf("Error reading correct answer for question %d from %s\n", i + 1, filename);
            break;
        }
    }

    fclose(file);
}

int play(char questions[][100], char options[][4][50], int correctAnswers[], int numQuestions) {
    int score = 0;
    int used[10] = {0}; // Array to track index
    int count = 0;

    for (int i = 0; i < 8; i++) {
        int questionIndex = rand() % numQuestions;

        // Check if questionIndex has been used
        for (count = 0; count < i; count++) {
            if (used[count] == questionIndex) {
                questionIndex = rand() % numQuestions; // Regenerate
                count = -1; // Rstrt check
            }
        }

        // Mark this index as used
        used[i] = questionIndex;

        // Print the question and options
        printf("\nQuestion %d: %s\n", i + 1, questions[questionIndex]);
        for (int j = 0; j < 4; j++) {
            printf("%d. %s\n", j + 1, options[questionIndex][j]);
        }

        // Get user's answer
        int userAnswer;
        printf("Your answer (1/2/3/4): ");
        scanf("%d", &userAnswer);

        if (userAnswer == correctAnswers[questionIndex]) {
            printf("Correct!\n");
            score += 10;
        } else {
            printf("Incorrect! The correct answer was %d.\n", correctAnswers[questionIndex]);
            break;
        }
    }

    return score;
}


