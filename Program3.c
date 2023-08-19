#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

typedef struct {
    int pid;
    char user[MAX_LINE_LENGTH];
} ProcessInfo;

int main() {
    FILE *input_file = fopen("top_output.txt", "r");
    if (input_file == NULL) {
        perror("Error opening file");
        return 1;
    }

    ProcessInfo processes[100];
    int num_processes = 0;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), input_file)) {
        int pid;
        char user[MAX_LINE_LENGTH];

        if (sscanf(line, "%d %s", &pid, user) == 2) {
            strcpy(processes[num_processes].user, user);
            processes[num_processes].pid = pid;
            num_processes++;
        }
    }

    fclose(input_file);

    printf("List of running PIDs and users:\n");
    for (int i = 0; i < num_processes; ++i) {
        printf("PID: %d, User: %s\n", processes[i].pid, processes[i].user);
    }

    return 0;
}