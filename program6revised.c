#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 2

// Function to find the modular inverse of a number 'a' mod m
int modInverse(int a, int m) {
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1;  // If inverse doesn't exist
}

// Function to calculate the determinant of a 2x2 matrix
int determinant(int key[SIZE][SIZE]) {
    return (key[0][0] * key[1][1] - key[0][1] * key[1][0]);
}

// Function to find the inverse of a 2x2 matrix mod 26
void inverseKeyMatrix(int key[SIZE][SIZE], int inverseKey[SIZE][SIZE]) {
    int det = determinant(key);
    int detInverse = modInverse(det % 26, 26);  // Find modular inverse of determinant mod 26

    if (detInverse == -1) {
        printf("Inverse doesn't exist!\n");
        return;
    }

    // Calculate inverse key matrix mod 26
    inverseKey[0][0] = (key[1][1] * detInverse) % 26;
    inverseKey[1][1] = (key[0][0] * detInverse) % 26;
    inverseKey[0][1] = (-key[0][1] * detInverse) % 26;
    inverseKey[1][0] = (-key[1][0] * detInverse) % 26;

    // Make sure all elements are positive by adding 26 if any element is negative
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (inverseKey[i][j] < 0)
                inverseKey[i][j] += 26;
        }
    }
}

// Encryption function
void encrypt(char message[], int key[SIZE][SIZE]) {
    int messageVector[SIZE];
    int cipherVector[SIZE];
    int i, j, k = 0;
    int keyvector[SIZE] = {key[0][0],key[1][0],key[1][0],key[1][1]};

    // Making matrix of 2
    while (k < strlen(message)) {
        // Prepare message vector for the current block
        for (i = 0; i < SIZE; i++) {
            if (isalpha(message[k])) {
                messageVector[i] = toupper(message[k]) - 'A';  // Convert letter to number
            }
            k++;
        }

        // Encrypt the block using matrix multiplication
        for (i = 0; i < SIZE; i++) {
            cipherVector[i] = 0;
            for (j = 0; j < SIZE; j++) {
                cipherVector[i] += key[i][j] * messageVector[j] ;
            }
            cipherVector[i] += keyvector[i];
            cipherVector[i] %= 26;  // Mod 26 for each character
        }

        // Output the encrypted block
        for (i = 0; i < SIZE; i++) {
            printf("%c", cipherVector[i] + 'A');  // Convert number back to letter
        }

    }
}

// Decryption function
void decrypt(char cipher[], int key[SIZE][SIZE]) {
    int cipherVector[SIZE];
    int messageVector[SIZE];
    int inverseKey[SIZE][SIZE];
    int keyvector[SIZE] = {key[0][0],key[1][0],key[1][0],key[1][1]};
    int i, j, k = 0;

    // Find the inverse of the key matrix
    inverseKeyMatrix(key, inverseKey);

    // Making matrix of 2
    while (k < strlen(cipher)) {
        // Prepare cipher vector for the current block
        for (i = 0; i < SIZE; i++) {
            if (isalpha(cipher[k])) {
                cipherVector[i] = toupper(cipher[k]) - 'A';  // Convert letter to number
                // Subtract the keyvector value (undoing the addition during encryption)
                cipherVector[i] = (cipherVector[i] - keyvector[i] + 26) % 26;
            }
            k++;
        }

        // Decrypt the block using matrix multiplication with inverse key matrix
        for (i = 0; i < SIZE; i++) {
            messageVector[i] = 0;
            for (j = 0; j < SIZE; j++) {
                messageVector[i] += inverseKey[i][j] * cipherVector[j];
            }
            messageVector[i] %= 26;  // Mod 26 for each character

            // Adjust for negative values
            if (messageVector[i] < 0)
                messageVector[i] += 26;
        }

        // Output the decrypted block
        for (i = 0; i < SIZE; i++) {
            printf("%c", messageVector[i] + 'A');  // Convert number back to letter
        }
    }
}

int main() {
    char message[5];  // 4 characters for the 2x2 Hill cipher (with space for null terminator)
    int key[SIZE][SIZE] = {{3, 3}, {2, 5}};  // Example key matrix
    int choice;

    printf("Enter a 4-letter message: ");
    scanf("%4s", message);  // Limit input to 4 characters

    printf("Choose 1 for encryption, 2 for decryption:\n");
    scanf("%d",&choice);

    switch (choice)
    {
    case 1:
        encrypt(message,key);
        break;
    
    case 2:
        decrypt(message,key);

    default:
        break;
    }

    return 0;
}