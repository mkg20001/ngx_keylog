#include<ngx_config.h>
#include<ngx_core.h>
#include<ngx_http.h>
#include<stdlib.h>
#include<stdio.h>

#define FIRSTLINE   "# SSL key logfile generated by ngx_http_keylog.c\n"
#define FIRSTLINE_LEN (sizeof(FIRSTLINE) - 1)

typedef struct {
    ngx_str_t keylog_location;
    ngx_flag_t keylog_enabled;
    int keylog_fd;
    ngx_str_t current_keylog_location;
} ngx_http_keylog_main_conf_t;

static int keylog_file_fd = -1;

static ngx_command_t ngx_http_keylog_commands[] = {
        { ngx_string("keylog_location"),
          NGX_HTTP_MAIN_CONF|NGX_CONF_1MORE,
          ngx_conf_set_str_slot,
          NGX_HTTP_MAIN_CONF_OFFSET,
          0,
          NULL },
        { ngx_string("keylog_enabled"),
          NGX_HTTP_MAIN_CONF|NGX_CONF_FLAG,
          ngx_conf_set_flag_slot,
          NGX_HTTP_MAIN_CONF_OFFSET,
          offsetof(ngx_http_keylog_main_conf_t, keylog_enabled),
          NULL
        }
};

/* Key extraction via the new OpenSSL 1.1.1 API. */
static void keylog_callback(const SSL *ssl, const char *line)
{
    if (keylog_file_fd >= 0) {
        if (write(keylog_file_fd, line, strlen(line))) {
        }
        if (write(keylog_file_fd, "\n", 1)) {
        }
    }
}

static ngx_int_t
ngx_http_keylog_access_handler(ngx_http_request_t *r)
{
    /* ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "http hello_world handler"); */

    ngx_log_error(NGX_LOG_EMERG, r->connection->log, 0,
                  "keylog access attached");

    if (r->connection->ssl->session_ctx != NULL)
        SSL_CTX_set_keylog_callback(r->connection->ssl->session_ctx, keylog_callback);

    return NGX_OK;
}

/* SSL *SSL_new(SSL_CTX *ctx)
{
    static SSL *(*func)();
    if (!func) {
        func = ngx_dlsym(RTLD_NEXT, "SSL_new");
    }
    * Override any previous key log callback. *
    SSL_CTX_set_keylog_callback(ctx, keylog_callback);
    return func(ctx);
} */

static ngx_int_t
ngx_http_keylog_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt        *h;
    ngx_http_core_main_conf_t  *cmcf;

    ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                  "init keylog");

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_PRECONTENT_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }

    *h = ngx_http_keylog_access_handler;

    return NGX_OK;
}

static void *
ngx_http_keylog_create_main_conf(ngx_conf_t *cf)
{
    ngx_http_keylog_main_conf_t  *conf;
    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_keylog_main_conf_t));
    if (conf == NULL) {
        return NGX_CONF_ERROR;
    }

    conf->keylog_enabled = NGX_CONF_UNSET;
    conf->keylog_fd = -1;

    return conf;
}


static char *
ngx_http_keylog_init_main_conf(ngx_conf_t *cf, void *conf)
{
    ngx_http_keylog_main_conf_t *klcf;
    klcf = conf;

    if (klcf->keylog_enabled == 1) {
        if (!klcf->keylog_location.len) {
            ngx_log_error(NGX_LOG_EMERG, cf->log, 0,
                          "keylog enabled but no keylog_location set");
            return NGX_CONF_ERROR;
        }

        // setup keylogging
        if (klcf->current_keylog_location.len != klcf->keylog_location.len || !ngx_strncmp(klcf->keylog_location.data, klcf->current_keylog_location.data, klcf->keylog_location.len)) {
            if (klcf->keylog_fd >= 0) {
                close(klcf->keylog_fd);
                klcf->keylog_fd = -1;
            }

            char * kl = ngx_palloc(cf->pool, klcf->keylog_location.len + 1);
            if (kl == NULL)
                return NGX_CONF_ERROR;
            ngx_memcpy(kl, klcf->keylog_location.data, klcf->keylog_location.len);
            kl[klcf->keylog_location.len] = '\0';
            klcf->keylog_fd = open(kl, O_WRONLY | O_APPEND | O_CREAT, 0644);

            if (klcf->keylog_fd >= 0 && lseek(klcf->keylog_fd, 0, SEEK_END) == 0) {
                /* file is opened successfully and there is no data (pos == 0) */
                if (write(klcf->keylog_fd, FIRSTLINE, FIRSTLINE_LEN)) {
                    return NGX_CONF_ERROR;
                }
            }

            klcf->current_keylog_location.len = klcf->keylog_location.len;
        }

        ngx_log_error(NGX_LOG_NOTICE, cf->log, 0,
                      "keylog enabled, logging to %*s", klcf->keylog_location.len, klcf->keylog_location.data);
    } else {
        // cleanup fd and current loc
        if (klcf->keylog_fd >= 0) {
            close(klcf->keylog_fd);
            klcf->keylog_fd = -1;
        }
        if (klcf->current_keylog_location.len) {
            ngx_free(klcf->current_keylog_location.data);
            klcf->current_keylog_location.len = 0;
            klcf->current_keylog_location.data = NULL;
        }
    }

    keylog_file_fd = klcf->keylog_fd;
    /* ngx_connection_t *c = NULL;
    if (c->ssl->session_ctx != NULL)
        SSL_CTX_set_keylog_callback(c->ssl->session_ctx, keylog_callback); */


    return NGX_CONF_OK;
}

static ngx_http_module_t  ngx_http_keylog_ctx = {
        NULL,                          /* preconfiguration */
        ngx_http_keylog_init,                          /* postconfiguration */

        ngx_http_keylog_create_main_conf,                          /* create main configuration */
        ngx_http_keylog_init_main_conf,                          /* init main configuration */

        NULL,                          /* create server configuration */
        NULL,                          /* merge server configuration */

        NULL,  /* create location configuration */
        NULL /* merge location configuration */
};

ngx_module_t  ngx_http_keylog = {
        NGX_MODULE_V1,
        &ngx_http_keylog_ctx, /* module context */
        ngx_http_keylog_commands,   /* module directives */
        NGX_HTTP_MODULE,               /* module type */
        NULL,                          /* init master */
        NULL,                          /* init module */
        NULL,                          /* init process */
        NULL,                          /* init thread */
        NULL,                          /* exit thread */
        NULL,                          /* exit process */
        NULL,                          /* exit master */
        NGX_MODULE_V1_PADDING
};