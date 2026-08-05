#include <stdio.h>
#include <string.h>

#define FLAG "01111110"
#define ESC  "00101111" /* Binary value for forward slash '/' */

char binary[100][9];
char stuffed[300][9];
char destuffed[100][9];

void asciiToBinary(char ch, char bin[]) {
    int i;
    for(i = 7; i >= 0; i--) {
        bin[7 - i] = ((ch >> i) & 1) + '0';
    }
    bin[8] = '\0';
}

char binaryToAscii(char bin[]) {
    int i, val = 0;
    for(i = 0; i < 8; i++) {
        val = val * 2 + (bin[i] - '0');
    }
    return (char)val;
}

void run_byte_stuffing_pipeline(char *str) {
    char result[100];
    int i, j = 0, k = 0, n;

    n = strlen(str);
    printf("--------------------------------------------------\n");
    printf("Processing Input String: \"%s\"\n", str);
    printf("--------------------------------------------------\n");

    printf("ASCII to Binary conversion:\n");
    for(i = 0; i < n; i++) {
        asciiToBinary(str[i], binary[i]);
        printf("  %c -> %s\n", str[i], binary[i]);
    }

    /* --- Sender Side (Stuffing) --- */
    strcpy(stuffed[j++], FLAG);
    for(i = 0; i < n; i++) {
        if(strcmp(binary[i], FLAG) == 0 || strcmp(binary[i], ESC) == 0) {
            strcpy(stuffed[j++], ESC);
        }
        strcpy(stuffed[j++], binary[i]);
    }
    strcpy(stuffed[j++], FLAG);

    printf("\nStuffed Frame:\n  ");
    for(i = 0; i < j; i++) {
        printf("%s ", stuffed[i]);
    }
    printf("\n");

    /* --- Receiver Side (De-stuffing) --- */
    printf("\nDe-stuffed Data:\n  ");
    for(i = 1; i < j - 1; i++) {
        if(strcmp(stuffed[i], ESC) == 0) {
            i++; /* Skip escape token, extract literal next byte */
            strcpy(destuffed[k], stuffed[i]);
            printf("%s ", destuffed[k]);
            k++;
        } else {
            strcpy(destuffed[k], stuffed[i]);
            printf("%s ", destuffed[k]);
            k++;
        }
    }
    printf("\n");

    for(i = 0; i < k; i++) {
        result[i] = binaryToAscii(destuffed[i]);
    }
    result[k] = '\0';

    printf("\nRecovered String Output: \"%s\"\n\n", result);
}

int main() {
    int choice;
    char user_str[100];

    while(1) {
        printf("===================================\n");
        printf("     BYTE STUFFING SIMULATOR       \n");
        printf("===================================\n");
        printf("1. Enter Custom String Manually\n");
        printf("2. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) break;
        getchar();

        if(choice == 2) {
            printf("Exiting system program...\n");
            break;
        }

        switch(choice) {
            case 1:
                printf("\nEnter custom input text string (Use '~' for FLAG, '/' for ESC): ");
                scanf("%[^\n]", user_str);
                getchar();
                run_byte_stuffing_pipeline(user_str);
                break;

            default:
                printf("Invalid selection. Try again.\n\n");
        }
    }

    return 0;
}
