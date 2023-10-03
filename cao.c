#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MAX_COMPLAINTS 100
#define MAX_COMPLAINT_LENGTH 200

// Structure to represent a complaint
typedef struct {
    int id;
    char description[MAX_COMPLAINT_LENGTH];
    char status[20]; // Status: New, In Progress, Resolved, etc.
} Complaint;

// Function to display a complaint
void displayComplaint(Complaint complaint) {
    printf("Complaint ID: %d\n", complaint.id);
    printf("Status: %s\n", complaint.status);
    printf("Description: %s\n", complaint.description);
}

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Complaint complaints[MAX_COMPLAINTS];
    int numComplaints = 0;

    while (1) {
        if (rank == 0) {
            printf("1. Submit a complaint\n");
            printf("2. View complaints\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");

            int choice;
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    if (numComplaints < MAX_COMPLAINTS) {
                        Complaint newComplaint;
                        newComplaint.id = numComplaints + 1;
                        printf("Enter complaint description: ");
                        getchar(); // Consume newline
                        fgets(newComplaint.description, sizeof(newComplaint.description), stdin);
                        newComplaint.description[strlen(newComplaint.description) - 1] = '\0'; // Remove trailing newline
                        strcpy(newComplaint.status, "New");
                        complaints[numComplaints] = newComplaint;
                        numComplaints++;
                        printf("Complaint submitted successfully!\n");
                    } else {
                        printf("Maximum number of complaints reached.\n");
                    }
                    break;
                case 2:
                    // Broadcast the number of complaints to all processes
                    MPI_Bcast(&numComplaints, 1, MPI_INT, 0, MPI_COMM_WORLD);

                    printf("Complaints:\n");
                    for (int i = 0; i < numComplaints; i++) {
                        displayComplaint(complaints[i]);
                        printf("\n");
                    }
                    break;
                case 3:
                    printf("Exiting the program.\n");
                    MPI_Finalize();
                    exit(0);
                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } else {
            // Worker processes (e.g., for complaint processing) can be implemented here.
            // In a real system, you would distribute and process complaints among worker processes.
        }
    }

    MPI_Finalize();
    return 0;
}