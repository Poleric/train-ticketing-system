#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef TRAINTICKETINGSYSTEM_UTILS_H
#define TRAINTICKETINGSYSTEM_UTILS_H

#define SHA256_HASH_LENGTH 64

int digest_message(const unsigned char *message, size_t message_len, unsigned char **digest, unsigned int *digest_len);
int decode_digest(const unsigned char* digest, size_t digest_size, char* buff, size_t buff_size);
int hash_message(const char* message, char buff[SHA256_HASH_LENGTH]);
int compare_hash(const char* not_hashed, const char* hashed);
int input(const char* prompt, char* buffer, size_t buffer_size);
void flush(FILE* stream);
void delete_char(char* buffer, int pos);
void add_char(char* buffer, char ch, int pos);

#endif //TRAINTICKETINGSYSTEM_UTILS_H
