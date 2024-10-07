#include <stdio.h>
#include <string.h>

long int gcd(long int a, long int b) {
    while (b != 0) {
        long int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long int mod_exp(long int base, long int exponent, long int mod) {
    long int result = 1;
    base = base % mod;
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exponent = exponent / 2;
    }
    return result;
}

long int encrypt(long int m, long int e, long int n) {
    return mod_exp(m, e, n);
}

long int decrypt(long int c, long int e, long int n, long int totient_n) {
    long int d = 1;
    while ((e * d) % totient_n != 1) {
        d++;
    }
    return mod_exp(c, d, n);
}

void main() {
    long int p, q, m, n, totient_n, e;
    long int encrypt_m, decrypt_m;

    printf("Enter prime numbers: \n");
    scanf("%ld", &p);
    scanf("%ld", &q);
    printf("Enter message: \n");
    scanf("%ld", &m);

    n = p * q;
    totient_n = (p - 1) * (q - 1);

    for (long int i = 2; i < totient_n; i++) {
        if (gcd(i, totient_n) == 1) {
            e = i;
            break;
        }
    }

    printf("Chosen e value: %ld\n", e);

    encrypt_m = encrypt(m, e, n);
    printf("Encrypted message: %ld\n", encrypt_m);

    decrypt_m = decrypt(encrypt_m, e, n, totient_n);
    printf("Decrypted message: %ld\n", decrypt_m);
}