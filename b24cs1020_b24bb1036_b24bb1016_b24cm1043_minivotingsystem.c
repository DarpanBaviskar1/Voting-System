#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

// Declare constants for the maximum number of candidates and voters
#define MAX_CANDIDATES 100
#define MAX_VOTERS 1000

// Global variables
int n_candidates;
int n_voters;
char name[MAX_CANDIDATES + 1][50];  // Candidate names (including NOTA)
int *vote;                         // Votes per voter
int arr[MAX_CANDIDATES + 1];       // Vote counts for each candidate (including NOTA)
int voter_ids[MAX_VOTERS];         // Store voter IDs for authentication
int voter_status[MAX_VOTERS];      // Track if a voter has already voted

// Function prototypes
void who_r_u();
void admin();
void admin_panel();
void voter();
void savefile();
int authenticate_voter(int id);

void main() {
    printf("\t\t\t\t\t\t\t\t\t\t\t\tFOUR'NICS\n");
    sleep(1);
    who_r_u(); // Entry point to determine user role
}

// Determine if the user is a voter or admin
void who_r_u() {
    int count = 0;
    char opt[50];
    printf("\t\t\t\t\t\t\t\t\t\t\tAre you a voter or an admin?\n");
    sleep(1);
    scanf("%s", opt);
    if (strcmp("voter", opt) == 0) {
        printf("\n\n\t\t\t\t\t\t\t\t\t\t\t\tWelcome Voter !!\n\n");
        voter();
    } else if (strcmp("admin", opt) == 0) {
        printf("\n\n\t\t\t\t\t\t\t\t\t\t\t\tWelcome Admin !!\n\n");
        printf("Admin Panel Verification\n");
        admin(count);
    } else {
        printf("\n\t\t\t\t\t\t\t\tInvalid Response... TRY AGAIN\n");
        who_r_u();  // Recursion on invalid input
    }
}

// Admin password check
void admin(int count) {
    char password[14];
    printf("Enter the password: ");
    scanf("%s", password);
    sleep(1);

    if (strcmp(password, "FOUR'NICS") == 0) {
        printf("\n\t\t\t\t\t\t\t\tWelcome Admin !!\n");
        admin_panel();
    } else {
        printf("\n\t\t\t\t\t\t\t\t\t\t\t\tPassword Incorrect\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t\t    Try again\n\n");
        count++;
        if (count%5==0) {
            printf("\t\t\t\t\t\t\t\t\t\tToo many unsuccessful attempts. Wait for 30 seconds.\n");
            sleep(30);
        }
        admin(count);  // Return to the initial menu
    }
}

// Admin panel to input candidates and voters
void admin_panel() {
    // Input the number of candidates
    printf("Number of candidates: ");
    scanf("%d", &n_candidates);

    if (n_candidates > MAX_CANDIDATES) {
        printf("\n\t\t\t\t\t\t\t\tToo many candidates! Max allowed is %d.\n", MAX_CANDIDATES);
        return;
    }

    // Input candidate names
    for (int i = 1; i <= n_candidates; i++) {
        printf("Enter name of Candidate %d: ", i);
        scanf("%s", name[i]);
    }
    strcpy(name[0], "NOTA");  // NOTA as candidate 0

    // Input the number of voters
    printf("Number of voters: ");
    scanf("%d", &n_voters);

    if (n_voters > MAX_VOTERS) {
        printf("\n\t\t\t\t\t\t\t\t\tToo many voters! Max allowed is %d.\n", MAX_VOTERS);
        admin_panel();
    }

    printf("Enter voter IDs:\n");
    for (int i = 0; i < n_voters; i++) {
        printf("Voter %d ID: ", i + 1);
        scanf("%d", &voter_ids[i]);
        voter_status[i] = 0; // Initialize voter status as "not voted"
    }

    printf("Voters initialized successfully.\n");

    // Initialize votes and candidate vote counts
    vote = (int *)malloc(n_voters * sizeof(int));
    for (int i = 0; i < n_voters; i++) {
        *(vote + i) = 1;  // Each voter has 1 vote
    }

    printf("\n\t\t\t\t\t\t\t\t----------------- EXIT ADMIN PANEL -----------------\n\n");
    voter();  // Proceed to the voting phase
}

// Voting process
void voter() {
    printf("\n\n\t\t\t\t\t\t\t\t----------------- ENTERING VOTING PANEL-----------------\n");

    int votes_cast = 0; // Track how many votes have been cast
    while (votes_cast < n_voters) {
        int voter_id;
        printf("Enter your Voter ID: ");
        scanf("%d", &voter_id);

        // Authenticate voter
        int voter_index = authenticate_voter(voter_id);
        if (voter_index == -1) {
            printf("\n\t\t\t\t\t\t\t\tVoter not identified.\n");
            continue;
        }

        // Check if voter has already voted
        if (voter_status[voter_index] == 1) {
            printf("\n\t\t\t\t\t\t\t\tYou have already voted! No second votes allowed.\n");
            continue;
        }

        int k;
        // Display voting options
        printf("\t\t\t\t\t\t\t\tVoter %d, give your vote (1 to %d) or 0 for NOTA: \n", votes_cast + 1, n_candidates);
        scanf("%d", &k);

        // Validate the vote input
        if (k < 0 || k > n_candidates) {
            printf("\n\t\t\t\t\t\t\t\tInvalid vote! Please enter a value between 0 and %d.\n", n_candidates);
            continue;
        }

        // Increment the vote count for the selected candidate
        arr[k] += *(vote + votes_cast);
        voter_status[voter_index] = 1; // Mark voter as "voted"
        votes_cast++; // Increment the number of valid votes cast
    }

    // Display the voting results
    printf("\n\n\t\t\t\t\t\t\t\t-------------- Voting Results --------------\n\n");
    for (int i = 0; i < n_candidates + 1; i++) {
        printf("\n\t\t\t\t\t\t\t\t%s got %d votes\n", name[i], arr[i]);
    }

    int max_votes_i = 1;
    for (int i = 1; i < n_candidates + 1; i++) {
        if (arr[max_votes_i] < arr[i]) {
            max_votes_i = i;
        }
    }
    int count = 0;

    for (int i = 1; i < n_candidates + 1; i++) {
        if (arr[max_votes_i] == arr[i])
            count++;
    }

    if (count > 1) {
        printf("\nThe election is Tied!\n");
    } else {
        printf("\n%s Won the Election with %d votes.\n", name[max_votes_i], arr[max_votes_i]);
    }

    savefile();
}

// Save voting results to a file
void savefile() {
    FILE *fp = fopen("Voting_record.txt", "a");
    if (fp == NULL) {
        printf("No memory allocated");
        exit(1);
    }

    fprintf(fp, "--------------- The Voting Results for this Voting are ---------------\n\n");
    for (int i = 0; i < n_candidates + 1; i++) {
        fprintf(fp, "%d.)%s got %d votes\n", i + 1, name[i], arr[i]);
    }
    int max_votes_i = 1;
    for (int i = 1; i < n_candidates + 1; i++) {
        if (arr[max_votes_i] < arr[i]) {
            max_votes_i = i;
        }
    }
    int count = 0;

    for (int i = 1; i < n_candidates + 1; i++) {
        if (arr[max_votes_i] == arr[i])
            count++;
    }

    if (count > 1) {
        fprintf(fp, "\nThe election is Tied!\n");
    } else {
        fprintf(fp, "\n%s Won the Election with %d votes.\n", name[max_votes_i], arr[max_votes_i]);
    }
    free(vote);

    fclose(fp);
}

// Authenticate voter and return their index or -1 if not found
int authenticate_voter(int id) {
    for (int i = 0; i < n_voters; i++) {
        if (voter_ids[i] == id) {
            return i;
        }
    }
    return -1;
}
