#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function declarations
void readQuestionsFromFile(char *filename, char questions[][100], char options[][4][50], int correctAnswers[], int *numQuestions);
int play(char questions[][100], char options[][4][50], int correctAnswers[], int numQuestions, int questionsToAttempt);

int main(int argc, char *argv[]) {
    char *filename;

    // If filename is not provided, prompt the user for it
    if (argc != 2) {
        printf("Please provide a valid text file containing the quiz questions.\n");
        printf("Enter the filename: ");
        char inputFilename[100];
        scanf("%s", inputFilename);
        filename = inputFilename;
    } else {
        filename = argv[1];
    }

    srand(time(NULL)); // Seed random number generation

    int numQuestions = 0;
    char questions[10][100];
    char options[10][4][50];
    int correctAnswers[10];

    // Read questions and answers from the specified file
    readQuestionsFromFile(filename, questions, options, correctAnswers, &numQuestions);

    if (numQuestions == 0) {
        printf("No valid questions found in the file. Exiting...\n");
        return 1;
    }

    printf("There are %d questions available in the file.\n", numQuestions);

    int questionsToAttempt;
    printf("How many questions would you like to attempt? (1-%d): ", numQuestions);
    scanf("%d", &questionsToAttempt);

    if (questionsToAttempt < 1 || questionsToAttempt > numQuestions) {
        printf("Invalid number of questions to attempt. Exiting...\n");
        return 1;
    }

    // Play the quiz
    int score = play(questions, options, correctAnswers, numQuestions, questionsToAttempt);

    printf("\nYour total score is: %d\n", score);
    if (score == questionsToAttempt * 10) {
        printf("Congratulations! You've won the game!\n");
    } else {
        printf("Better luck next time!\n");
    }

    return 0;
}

void readQuestionsFromFile(char *filename, char questions[][100], char options[][4][50], int correctAnswers[], int *numQuestions) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open %s\n", filename);
        exit(1);
    }

    char line[200];
    int count = 0;
    for (; count < 10 && fgets(line, sizeof(line), file); count++) {
        // Read the question
        line[strcspn(line, "\n")] = '\0'; // Remove trailing newline
        if (strlen(line) == 0 ) {
            printf("Skipping invalid question format at question %d.\n", count + 1);
            count--; // Decrease count to retry the question
            continue;
        }
        strncpy(questions[count], line, 100);

        // Read the options
        if(fgets(line, sizeof(line), file) == NULL || strchr(line, '|') == NULL) {
            printf("Skipping invalid options format at question %d.\n", count + 1);
            count--;
            continue;
        }

        char *token = strtok(line, "|");
        int optionCount = 0;
        for (; token && optionCount < 4; optionCount++) {
            strncpy(options[count][optionCount], token, 50);
            options[count][optionCount][49] = '\0'; // Ensure null termination
            token = strtok(NULL, "|");
        }

        if (optionCount != 4) {
            printf("Skipping question %d due to insufficient options.\n", count + 1);
            count--;
            continue;
        }

        // Read the correct answer
        if (fgets(line, sizeof(line), file) == NULL || sscanf(line, "%d", &correctAnswers[count]) != 1 || correctAnswers[count] < 1 || correctAnswers[count] > 4) {
            printf("Skipping question %d due to invalid correct answer format.\n", count + 1);
            count--;
            continue;
        }
    }

    *numQuestions = count;
    fclose(file);
}

int play(char questions[][100], char options[][4][50], int correctAnswers[], int numQuestions, int questionsToAttempt) {
    int score = 0;
    int used[10] = {0}; // Array to track used indices

    for (int i = 0; i < questionsToAttempt; i++) {
        int questionIndex = rand() % numQuestions;

        // Check if questionIndex has been used
        for (int count = 0; count < i; count++) {
            if (used[count] == questionIndex) {
                questionIndex = rand() % numQuestions; // Regenerate
                count = -1; // Restart check
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
        }
    }

    return score;
}
