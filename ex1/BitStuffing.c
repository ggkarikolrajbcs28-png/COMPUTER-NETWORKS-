#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define max 100
#define max_bit (max * 8)
#define max_stuff (max_bit * 2)

int data[max_bit], stuff[max_stuff], destuff[max_bit];
int sender_framed[max_stuff];
int sender_framelen = 0;

char str[max];
char output_str[max];
int flag[] = {0, 1, 1, 1, 1, 1, 1, 0};
int flaglen = 8;

void print(char *label, int arr[], int n) {
   int i;
   printf("%s ", label);
   for(i = 0; i < n; i++) {
      printf("%d", arr[i]);
      if((i + 1) % 8 == 0) {
         printf(" ");
      }
   }
   printf("\n");
}

int bit_stuffing(int input_bits[], int input_len, int output_bits[]) {
   int i, ones = 0, j = 0;
   for(i = 0; i < input_len; i++) {
      output_bits[j++] = input_bits[i];
      ones = (input_bits[i] == 1) ? ones + 1 : 0;
      if(ones == 5) {
         output_bits[j++] = 0;
         ones = 0;
      }
   }
   return j;
}

int bit_destuffing(int input_framed[], int total_len, int output_bits[]) {
   int i, ones = 0, j = 0;
   if (total_len < (flaglen * 2)) {
      printf("\nError: Frame is too short to contain valid flags!\n");
      return -1;
   }
   for(i = 0; i < flaglen; i++) {
      if(input_framed[i] != flag[i]) {
         printf("\nError: Invalid or corrupted Start Flag detected!\n");
         return -1;
      }
   }
   for(i = 0; i < flaglen; i++) {
      if(input_framed[total_len - flaglen + i] != flag[i]) {
         printf("\nError: Invalid or corrupted End Flag detected!\n");
         return -1;
      }
   }
   for(i = flaglen; i < total_len - flaglen; i++) {
      output_bits[j++] = input_framed[i];
      ones = (input_framed[i] == 1) ? ones + 1 : 0;
      if(ones == 5) {
         if(input_framed[i + 1] != 0) {
            printf("\nError: Invalid stuffing bit detected (Expected 0 after 5 consecutive 1s)!\n");
            return -1;
         }
         i++;
         ones = 0;
      }
   }
   return j;
}

void decode_binary_to_string(int input_bits[], int input_len, char target_str[]) {
   int i, b, out_idx = 0;
   for(i = 0; i < input_len; i += 8) {
      char ch = 0;
      int limit = (input_len - i < 8) ? (input_len - i) : 8;
      for(b = 0; b < limit; b++) {
         ch = (ch << 1) | input_bits[i + b];
      }
      if(limit < 8) {
         ch = ch << (8 - limit);
      }
      target_str[out_idx++] = ch;
   }
   target_str[out_idx] = '\0';
}

int main() {
   int choice;
   srand(time(NULL));
   while(1) {
      printf("\n==================================\n");
      printf("      BIT STUFFING SIMULATOR      \n");
      printf("==================================\n");
      printf("1. Sender Side \n");
      printf("2. Receiver Side\n");
      printf("3. Exit\n");
      printf("Enter your choice: ");
      if (scanf("%d", &choice) != 1) break;
      getchar();

      if(choice == 3) {
         printf("Exiting...\n");
         break;
      }

      switch(choice) {
         case 1: {
            int i, b, bit = 0;
            printf("\nEnter string to send: ");
            scanf("%[^\n]", str);
            getchar();

            int str_len = strlen(str);
            for(i = 0; i < str_len; i++) {
               char ch = str[i];
               for(b = 7; b >= 0; b--) {
                  data[bit++] = (ch >> b) & 1;
               }
            }
            print("\nData Bits:      ", data, bit);

            int stufflen = bit_stuffing(data, bit, stuff);
            print("Stuffed Bits:   ", stuff, stufflen);

            sender_framelen = 0;
            for(i = 0; i < flaglen; i++) sender_framed[sender_framelen++] = flag[i];
            for(i = 0; i < stufflen; i++) sender_framed[sender_framelen++] = stuff[i];
            for(i = 0; i < flaglen; i++) sender_framed[sender_framelen++] = flag[i];
            print("Framed Bits:    ", sender_framed, sender_framelen);
            int error_choice;
            printf("\n--- Channel Corruption Menu ---\n");
            printf("1. Transmit Frame Cleanly (No Errors)\n");
            printf("2. Inject Single Bit Error\n");
            printf("3. Inject Multiple Bit Errors\n");
            printf("Enter choice: ");
            scanf("%d", &error_choice);
            getchar();

            if (error_choice == 2) {
               int error_pos = rand() % sender_framelen;
               sender_framed[error_pos] ^= 1;
               printf("\n[ALERT] Simulated Single Bit Error! Inverted bit position %d.\n", error_pos + 1);
               print("Corrupted Frame:", sender_framed, sender_framelen);
            }
            else if (error_choice == 3) {
               int num_errors;
               printf("Enter number of bits to flip: ");
               scanf("%d", &num_errors);
               getchar();

               if(num_errors > sender_framelen) {
                  num_errors = sender_framelen;
               }

               for(i = 0; i < num_errors; i++) {
                  int error_pos = rand() % sender_framelen;
                  sender_framed[error_pos] ^= 1;
               }
               printf("\n[ALERT] Simulated %d Random Bit Errors!\n", num_errors);
               print("Corrupted Frame:", sender_framed, sender_framelen);
            } else {
               printf("\nFrame passed cleanly to transmission queue.\n");
            }
            break;
         }

         case 2: {
            int i, rx_choice;
            int active_framed[max_stuff];
            int active_framelen = 0;

            printf("\n--- Receiver Mode Selection ---\n");
            printf("1. Process Transmitted Channel Data\n");
            printf("2. Manually Paste/Enter Binary String\n");
            printf("Enter choice: ");
            scanf("%d", &rx_choice);
            getchar();

            if(rx_choice == 1) {
               if(sender_framelen == 0) {
                  printf("\nError: No active channel data found. Please run Send Side first.\n");
                  break;
               }
               active_framelen = sender_framelen;
               for(i = 0; i < sender_framelen; i++) {
                  active_framed[i] = sender_framed[i];
               }
               print("\nProcessing Channel Data: ", active_framed, active_framelen);
            }
            else if(rx_choice == 2) {
               char bit_string[max_stuff];
               printf("\nEnter custom binary frame string (e.g. 01111110...): \n");
               scanf("%s", bit_string);
               getchar();

               active_framelen = strlen(bit_string);
               for(i = 0; i < active_framelen; i++) {
                  active_framed[i] = bit_string[i] - '0';
               }
            }
            else {
               printf("Invalid selection choice\n");
               break;
            }

            int destufflen = bit_destuffing(active_framed, active_framelen, destuff);

            if(destufflen == -1) {
               printf("\n[REJECTED]: Frame discarded due to format/stuffing violations.\n");
            } else {
               print("\nDestuffed Bits:  ", destuff, destufflen);
               decode_binary_to_string(destuff, destufflen, output_str);
               printf("Decoded String:  %s\n", output_str);
            }
            break;
         }

         default:
            printf("Invalid main choice option\n");
      }
   }
   return 0;
}
