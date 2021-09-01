//
// Created by maciej on 01.09.21.
//

#ifndef NGX_KEYLOG_NGX_SSLKEYLOG_H
#define NGX_KEYLOG_NGX_SSLKEYLOG_H

#include <openssl/ssl.h>

SSL *SSL_new(SSL_CTX *ctx);
static void open_keylog_file(const char *filename);
static void close_keylog_file(void);

#endif //NGX_KEYLOG_NGX_SSLKEYLOG_H
