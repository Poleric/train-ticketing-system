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
}

int hash_message(const char* message, char buff[SHA256_HASH_LENGTH]) {
    unsigned char* digest = malloc(32);
    unsigned digest_size;

    digest_message((const unsigned char*)message, strlen(message), &digest, &digest_size);

    decode_digest(digest, (size_t) digest_size, buff, SHA256_HASH_LENGTH);
    free(digest);
    return EXIT_SUCCESS;
}

int compare_digest(const char* not_hashed, const char* hashed) {
    char to_hash[SHA256_HASH_LENGTH + 1];
    to_hash[SHA256_HASH_LENGTH] = 0;
    hash_message(not_hashed, to_hash);

    return strncmp(to_hash, hashed, 64);
}

