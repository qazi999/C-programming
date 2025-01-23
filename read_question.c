#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char question[100];
    char options[4][50];
    int correctAnswer;
} Question;

void writeQuestionsToFile( char *filename);

int main() {
    
    char filename[100];
    printf("Enter the filename to save the quiz: ");
    scanf("%s", filename);

    writeQuestionsToFile(filename);

    printf("Quiz saved to %s\n", filename);
    return 0;
}

void writeQuestionsToFile( char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file for writing.\n");
        exit(1);
    }

    int numQuestions;
    printf("Enter the number of questions you want to add: ");
    scanf("%d", &numQuestions);
    getchar(); // Consume leftover newline

    for (int i = 0; i < numQuestions; i++) {

        Question q;

        printf("\nQuestion %d:\n", i + 1);
        printf("Enter the question text: ");
        fgets(q.question, sizeof(q.question), stdin);
        q.question[strcspn(q.question, "\n")] = '\0'; // Remove newline

        for (int j = 0; j < 4; j++) {
            printf("Enter option %d: ", j + 1);
            fgets(q.options[j], sizeof(q.options[j]), stdin);
            q.options[j][strcspn(q.options[j], "\n")] = '\0'; // Remove  newline
        }

        printf("Enter the correct answer (1-4): ");
        scanf("%d", &q.correctAnswer);
        getchar(); // Consume leftover newline

        // Write question to file
        fprintf(file, "%s\n", q.question);
        for (int j = 0; j < 4; j++) {
            fprintf(file, "%s|", q.options[j]);
        }
        fprintf(file, "\n%d\n", q.correctAnswer);
    }

    fclose(file);
}
