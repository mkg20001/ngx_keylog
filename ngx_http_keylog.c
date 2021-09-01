#include<ngx_config.h>
#include<ngx_core.h>
#include<ngx_http.h>
#include<stdlib.h>
#include<stdio.h>
#include "ngx_common_keylog.h"

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
          offsetof(ngx_keylog_conf_t, keylog_enabled),
          NULL
        }
};

static ngx_int_t
ngx_http_keylog_access_handler(ngx_http_request_t *r)
{
    attach_keylog("http", r->connection);

    return NGX_OK;
}

static ngx_int_t
ngx_http_keylog_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt        *h;
    ngx_http_core_main_conf_t  *cmcf;

    ngx_log_debug(NGX_LOG_NOTICE, cf->log, 0,
                  "init keylog http");

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_PREACCESS_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }

    *h = ngx_http_keylog_access_handler;

    return NGX_OK;
}


static ngx_http_module_t  ngx_http_keylog_ctx = {
        NULL,                          /* preconfiguration */
        ngx_http_keylog_init,                          /* postconfiguration */

        ngx_common_keylog_create_main_conf,                          /* create main configuration */
        ngx_common_keylog_init_main_conf,                          /* init main configuration */

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