#include<ngx_config.h>
#include<ngx_core.h>
#include<ngx_http.h>

typedef struct {
    ngx_str_t keylog_location;
    ngx_flag_t keylog_enabled;
} ngx_http_keylog_main_conf_t;

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

static void *
ngx_http_keylog_create_main_conf(ngx_conf_t *cf)
{
    ngx_http_keylog_main_conf_t  *conf;
    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_keylog_main_conf_t));
    if (conf == NULL) {
        return NGX_CONF_ERROR;
    }

    conf->keylog_enabled = NGX_CONF_UNSET;

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

        ngx_log_error(NGX_LOG_NOTICE, cf->log, 0,
                      "keylogging enabled, logging to %s", klcf->keylog_location);
    }

    return NGX_CONF_OK;
}

static ngx_http_module_t  ngx_http_keylog_ctx = {
        NULL,                          /* preconfiguration */
        NULL,                          /* postconfiguration */

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