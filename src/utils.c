#include <openssl/evp.h>
#include <utils.h>
#include <string.h>

int digest_message(const unsigned char *message, size_t message_len, unsigned char **digest, unsigned int *digest_len) {
    // https://wiki.openssl.org/index.php/EVP_Message_Digests
    EVP_MD_CTX *mdctx;

    if((mdctx = EVP_MD_CTX_new()) == NULL)
        return EXIT_FAILURE;

    if(1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL))
        return EXIT_FAILURE;

    if(1 != EVP_DigestUpdate(mdctx, message, message_len))
        return EXIT_FAILURE;

    if((*digest = (unsigned char *)OPENSSL_malloc(EVP_MD_size(EVP_sha256()))) == NULL)
        return EXIT_FAILURE;

    if(1 != EVP_DigestFinal_ex(mdctx, *digest, digest_len))
        return EXIT_FAILURE;

    EVP_MD_CTX_free(mdctx);
    return EXIT_SUCCESS;
}

int decode_digest(const unsigned char* digest, size_t digest_size, char* buff, size_t buff_size) {
    if (buff_size < digest_size * 2) {
        fprintf(stderr, "Buffer size `%zu` is not large enough to fit the digest of length `%zu`\n", buff_size, digest_size);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < digest_size; i++) {
        snprintf(buff, 3, "%02x", digest[i]);
        buff += 2;
    }
    return EXIT_SUCCESS;
}

int hash_message(const char* message, char buff[SHA256_HASH_LENGTH]) {
    unsigned char* digest;
    unsigned digest_size;

    digest_message((const unsigned char*)message, strlen(message), &digest, &digest_size);

    decode_digest(digest, (size_t) digest_size, buff, SHA256_HASH_LENGTH);
    free(digest);
    return EXIT_SUCCESS;
}

int compare_hash(const char* not_hashed, const char* hashed) {
    char to_hash[SHA256_HASH_LENGTH + 1];
    to_hash[SHA256_HASH_LENGTH] = 0;
    hash_message(not_hashed, to_hash);

    return strncmp(to_hash, hashed, 64);
}

int input(const char* prompt, char* buffer, size_t buffer_size) {
    printf("%s", prompt);

    if (fgets(buffer, (int) buffer_size, stdin) == NULL)
        return EXIT_FAILURE;

    if (buffer[strlen(buffer) - 1] != '\n')  // input longer than buffer can hold
        flush(stdin);

    // trim \n
    buffer[strcspn(buffer, "\n")] = 0;

    if (buffer[0] == 0)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

void flush(FILE* stream) {
    int c;
    while ((c = getc(stream)) != '\n' && c != EOF);
}

void delete_char(char* buffer, int pos) {
    /* unsafe
     * helper function to remove character in middle and shift all char
     */
    buffer += pos;
    while (*(buffer++)) {
        *(buffer - 1) = *buffer;
    }
    *buffer = 0;
}

void add_char(char* buffer, char ch, int pos) {
    /* unsafe
     * helper function to add character in middle and shift all char
     */
    size_t i = strlen(buffer);
    buffer += i;
    *(buffer + 1) = 0;
    while (i-- != pos) {
        *buffer = *(buffer - 1);
        buffer--;
    }
    *buffer = ch;
}

const char* tm_mon_to_text(int tm_mon) {
    switch (tm_mon) {
        case 0: return "January";
        case 1: return "February";
        case 2: return "March";
        case 3: return "April";
        case 4: return "May";
        case 5: return "June";
        case 6: return "July";
        case 7: return "August";
        case 8: return "September";
        case 9: return "October";
        case 10: return "November";
        case 11: return "December";
        default: return NULL;
    }
}

const char* tm_wday_to_text(tm_wday_t tm_wday) {
    switch (tm_wday) {
        case 0: return "Sunday";
        case 1: return "Monday";
        case 2: return "Tuesday";
        case 3: return "Wednesday";
        case 4: return "Thursday";
        case 5: return "Friday";
        case 6: return "Saturday";
        default: return NULL;
    }
}

char* to_time(dt_time_t time, char* buffer, int n) {
    snprintf(buffer, n, "%2d:%02d", time.tm_hour, time.tm_min);
    return buffer;
}

int sum_d(const int* nums, int number_of_num) {
    int sum = 0;
    for (int i = 0; i < number_of_num; i++)
        sum += nums[i];
    return sum;
}
