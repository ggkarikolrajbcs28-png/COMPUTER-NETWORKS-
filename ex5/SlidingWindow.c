#include <stdio.h>
#include <stdlib.h>

#define MAX 50

void printLine(int width) {
    for (int i = 0; i < width; i++) {
        printf("-");
    }
    printf("\n");
}

void stopAndWait(int frames, int lostFrame) {
    int Sn = 0;
    int step = 1, total = 0;

    printf("\n--- STOP-AND-WAIT PROTOCOL ---\n");
    printf("%-6s  %-8s  %-12s  %-12s\n", "Step", "Frame", "Status", "Window");
    printLine(44);

    while (Sn < frames) {
        total++;

        if (Sn == lostFrame) {
            printf("%-6d  %-8d  %-12s  [ %d ]\n", step++, Sn, "LOST", Sn);
            printf("      >> Frame %d lost. Timer started.\n", Sn);
            printf("      >> Timeout. Retransmitting frame %d.\n", Sn);
            lostFrame = -1;
        } else {
            printf("%-6d  %-8d  %-12s  [ %d ]\n", step++, Sn, "ACKED", Sn);
            printf("      >> Frame %d delivered.\n", Sn);
            printf("      >> ACK received. Window moves to %d.\n", Sn + 1);
            Sn++;
        }
    }

    printLine(44);
    printf("Frames delivered : %d\n", frames);
    printf("Total transmits  : %d\n", total);
    printLine(44);
    printf("\n");
}

void goBackN(int frames, int N, int lostFrame) {
    int Sb = 0;
    int Sm, Sn;
    int step = 1, total = 0;

    printf("\n--- GO-BACK-N PROTOCOL ---\n");
    printf("%-6s  %-8s  %-12s  %-16s\n", "Step", "Frame", "Status", "Window");
    printLine(48);

    while (Sb < frames) {
        Sm = Sb + N - 1;
        int lost_occurred = 0;
        int origSb = Sb;

        for (Sn = Sb; Sn <= Sm && Sn < frames; Sn++) {
            total++;

            if (Sn == lostFrame) {
                printf("%-6d  %-8d  %-12s  [ %d - %d ]\n",
                       step++, Sn, "LOST", origSb, (Sm < frames - 1 ? Sm : frames - 1));
                printf("      >> Frame %d lost. Later frames ignored.\n", Sn);
                lostFrame = -1;
                lost_occurred = 1;
                break;
            } else {
                printf("%-6d  %-8d  %-12s  [ %d - %d ]\n",
                       step++, Sn, "ACKED", origSb, (Sm < frames - 1 ? Sm : frames - 1));

                if (!lost_occurred) {
                    Sb++;
                }
            }
        }

        if (lost_occurred) {
            printf("      >> Timeout. Resending from frame %d.\n", Sb);
        } else if (Sb < frames) {
            printf("      >> Window shifted to [ %d - %d ].\n",
                   Sb, (Sb + N - 1 < frames - 1 ? Sb + N - 1 : frames - 1));
        }
    }

    printLine(48);
    printf("Frames delivered : %d\n", frames);
    printf("Total transmits  : %d\n", total);
    printLine(48);
    printf("\n");
}

void selectiveRepeat(int frames, int N, int lostFrame) {
    int Sb = 0;
    int Sm;
    int step = 1, total = 0;

    int *acked = (int *)calloc(frames, sizeof(int));

    printf("\n--- SELECTIVE REPEAT PROTOCOL ---\n");
    printf("%-6s  %-8s  %-12s  %-16s\n", "Step", "Frame", "Status", "Window");
    printLine(48);

    while (Sb < frames) {
        Sm = Sb + N - 1;

        for (int Sn = Sb; Sn <= Sm && Sn < frames; Sn++) {
            if (acked[Sn] == 1) continue;

            total++;

            if (Sn == lostFrame) {
                printf("%-6d  %-8d  %-12s  [ %d - %d ]\n",
                       step++, Sn, "LOST", Sb, (Sm < frames - 1 ? Sm : frames - 1));
                printf("      >> Frame %d lost. Will retransmit later.\n", Sn);
                lostFrame = -1;
            } else {
                printf("%-6d  %-8d  %-12s  [ %d - %d ]\n",
                       step++, Sn, "ACKED", Sb, (Sm < frames - 1 ? Sm : frames - 1));
                acked[Sn] = 1;
                printf("      >> Frame %d delivered and buffered.\n", Sn);
            }
        }

        int oldSb = Sb;
        while (Sb < frames && acked[Sb] == 1) {
            Sb++;
        }

        if (Sb > oldSb && Sb < frames) {
            int newSm = Sb + N - 1;
            printf("      >> Window base ACKed. Shifted to [ %d - %d ].\n",
                   Sb, (newSm < frames - 1 ? newSm : frames - 1));
        }
    }

    printLine(48);
    printf("Frames delivered : %d\n", frames);
    printf("Total transmits  : %d\n", total);
    printLine(48);
    printf("\n");

    free(acked);
}

int main() {
    int choice;
    int n, w, lost;

    do {
        printf("\n");
        printf("SLIDING WINDOW PROTOCOL SIMULATOR\n");
        printLine(34);
        printf("1. Stop-and-Wait\n");
        printf("2. Go-Back-N\n");
        printf("3. Selective Repeat\n");
        printf("4. Exit\n");
        printLine(34);
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 4) {
            printf("\nExiting. Goodbye!\n");
            break;
        }
        if (choice < 1 || choice > 4) {
            printf("\nError: Invalid choice. Try again.\n\n");
            continue;
        }

        printf("Number of frames : ");
        scanf("%d", &n);
        printf("Frame to lose    : ");
        scanf("%d", &lost);

        if (choice != 1) {
            printf("Window size (N)  : ");
            scanf("%d", &w);
        } else {
            w = 1;
        }

        if (n <= 0 || n > MAX || w <= 0 || w > n) {
            printf("\nError: Invalid config. Use 0 < N <= frames <= %d\n\n", MAX);
            continue;
        }
        if (lost >= n) {
            printf("\nError: Lost frame must be between 0 and %d.\n\n", n - 1);
            continue;
        }

        switch (choice) {
            case 1: stopAndWait(n, lost); break;
            case 2: goBackN(n, w, lost); break;
            case 3: selectiveRepeat(n, w, lost); break;
        }
    } while (choice != 4);

    return 0;
}
