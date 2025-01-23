#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char question[100];
    char options[4][50];
    int correctAnswer;
} Question;

int readQuestionsFromFile(const char *filename, Question questions[], int *numQuestions);
int conductQuiz(Question questions[], int numQuestions, int questionsToAttempt);

int main(int argc, char *argv[]) {
    const char *filename;
    if (argc != 2) {
        char inputFilename[100];
        printf("Enter the filename containing quiz questions: ");
        scanf("%s", inputFilename);
        filename = inputFilename;
    } else {
        filename = argv[1];
    }

    srand(time(NULL));

    Question questions[100];
    int numQuestions = 0;

    if (readQuestionsFromFile(filename, questions, &numQuestions) == 0) {
        printf("No valid questions found in the file. Exiting...\n");
        return 1;
    }

    printf("There are %d valid questions in the file.\n", numQuestions);

    int questionsToAttempt;
    printf("How many questions would you like to attempt? (1-%d): ", numQuestions);
    scanf("%d", &questionsToAttempt);

    if (questionsToAttempt < 1 || questionsToAttempt > numQuestions) {
        printf("Invalid number of questions to attempt. Exiting...\n");
        return 1;
    }

    int score = conductQuiz(questions, numQuestions, questionsToAttempt);

    printf("\nYour total score is: %d\n", score);
    if (score == questionsToAttempt * 10) {
        printf("7 karooooor!\n");
    } else {
        printf("Shama chahte hai apka dimaag kamzoor hai agli baar padke aaye!\n");
    }

    return 0;
}

// Function to read questions from the file
int readQuestionsFromFile(const char *filename, Question questions[], int *numQuestions) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file for reading.\n");
        return 0;
    }

    char line[200];
    int count = 0;

    for (count = 0; count < 100 && fgets(line, sizeof(line), file); count++) {
        line[strcspn(line, "\n")] = '\0'; // Remove  newline
        if (strlen(line) == 0) {
            printf("Skipping invalid question format at question %d.\n", count + 1);
            count--;
            continue;
        }
        strncpy(questions[count].question, line, sizeof(questions[count].question));

        if (fgets(line, sizeof(line), file) == NULL || strchr(line, '|') == NULL) {
            printf("Skipping invalid options format at question %d.\n", count + 1);
            count--;
            continue;
        }

        line[strcspn(line, "\n")] = '\0'; // Remove trailing newline
        char *token = strtok(line, "|");
        int optionCount = 0;

        for (optionCount = 0; token && optionCount < 4; optionCount++) {
            strncpy(questions[count].options[optionCount], token, sizeof(questions[count].options[optionCount]));
            token = strtok(NULL, "|");
        }

        if (optionCount != 4) {
            printf("Skipping question %d due to insufficient options.\n", count + 1);
            count--;
            continue;
        }

        if (fgets(line, sizeof(line), file) == NULL || sscanf(line, "%d", &questions[count].correctAnswer) != 1 ||
            questions[count].correctAnswer < 1 || questions[count].correctAnswer > 4) {
            printf("Skipping question %d due to invalid correct answer format.\n", count + 1);
            count--;
            continue;
        }
    }

    *numQuestions = count;
    fclose(file);
    return 1;
}

// Function to conduct the quiz
int conductQuiz(Question questions[], int numQuestions, int questionsToAttempt) {
    int score = 0;
    int used[100] = {0};

    for (int i = 0; i < questionsToAttempt; i++) {
        int questionIndex = rand() % numQuestions;
        // Check if the question has already been used
        for (int j = 0; j < i; j++) {
            if (used[j] == questionIndex) {
                questionIndex = rand() % numQuestions;
                j = -1; // Restart the check
            }
        }

        used[i] = questionIndex;

        // Display the question and options
        printf("\nQuestion %d: %s\n", i + 1, questions[questionIndex].question);
        for (int j = 0; j < 4; j++) {
            printf("%d. %s\n", j + 1, questions[questionIndex].options[j]);
        }

        int userAnswer;
        printf("Your answer (1-4): ");
        scanf("%d", &userAnswer);

        if (userAnswer == questions[questionIndex].correctAnswer) {
            printf("Correct!\n");
            score += 10;
        } else {
            printf("Incorrect! The correct answer was %d.\n", questions[questionIndex].correctAnswer);
        }
    }

    return score;
}
