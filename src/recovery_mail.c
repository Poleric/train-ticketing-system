#ifdef EMAIL_RECOVERY_FEATURE

#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define PAYLOAD_TEXT                                                 \
        "From: <" FROM_MAIL ">\r\n"                                  \
        "To: <%s>\r\n"                                               \
        "Subject: Train Ticketing Recovery\r\n"                      \
        "\r\n"                                                       \
        "Your recovery code is %06d.\r\n"                            \
        "\r\n"                                                       \
        "If you haven't made this request, please ignore it.\r\n"

struct upload_status {
    char content[1024];
    size_t bytes_read;
};

static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp)
{
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    const char *data;
    size_t room = size * nmemb;

    if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
        return 0;
    }

    data = upload_ctx->content + upload_ctx->bytes_read;

    if (data) {
        size_t len = strlen(data);
        if(room < len)
            len = room;
        memcpy(ptr, data, len);
        upload_ctx->bytes_read += len;

        return len;
    }

    return 0;
}

int send_recovery(const char* to_mail, int code)
{
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;
    struct upload_status upload_ctx = { 0 };

    curl = curl_easy_init();
    if(curl) {
        /* Set username and password */
        curl_easy_setopt(curl, CURLOPT_USERNAME, FROM_MAIL);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, APP_PASSWORD);

        snprintf(upload_ctx.content, 1024, PAYLOAD_TEXT, to_mail, code);

        curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");

        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_MAIL);
        recipients = curl_slist_append(recipients, to_mail);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

#ifndef DEBUG
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif

        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }

    return (int)res;
}

#endif