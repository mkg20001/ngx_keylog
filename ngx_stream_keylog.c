/*
 *  Copyright (C) 2021 Maciej Krüger
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 +  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include<ngx_config.h>
#include<ngx_core.h>
#include<ngx_stream.h>
#include<stdlib.h>
#include<stdio.h>
#include "ngx_common_keylog.h"

static ngx_command_t ngx_stream_keylog_commands[] = {
        { ngx_string("keylog_location"),
          NGX_STREAM_MAIN_CONF|NGX_CONF_1MORE,
          ngx_conf_set_str_slot,
          NGX_STREAM_MAIN_CONF_OFFSET,
          0,
          NULL },
          { ngx_string("keylog_enabled"),
            NGX_STREAM_MAIN_CONF|NGX_CONF_FLAG,
            ngx_conf_set_flag_slot,
            NGX_STREAM_MAIN_CONF_OFFSET,
            offsetof(ngx_keylog_conf_t, keylog_enabled),
            NULL
          }
};

static ngx_int_t
ngx_stream_keylog_ssl_handler(ngx_stream_session_t *s)
{
    attach_keylog("stream", s->connection);

    return NGX_OK;
}

static ngx_int_t
ngx_stream_keylog_init(ngx_conf_t *cf)
{
    ngx_stream_handler_pt        *h;
    ngx_stream_core_main_conf_t *cmcf;

    ngx_log_debug(NGX_LOG_NOTICE, cf->log, 0,
                  "init keylog stream");

    cmcf = ngx_stream_conf_get_module_main_conf(cf, ngx_stream_core_module);

    h = ngx_array_push(&cmcf->phases[NGX_STREAM_PREREAD_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }

    *h = ngx_stream_keylog_ssl_handler;

    return NGX_OK;
}


static ngx_stream_module_t  ngx_stream_keylog_ctx = {
        NULL,                          /* preconfiguration */
        ngx_stream_keylog_init,                          /* postconfiguration */

        ngx_common_keylog_create_main_conf,                          /* create main configuration */
        ngx_common_keylog_init_main_conf,                          /* init main configuration */

        NULL,                          /* create server configuration */
        NULL,                          /* merge server configuration */
};

ngx_module_t  ngx_stream_keylog = {
        NGX_MODULE_V1,
        &ngx_stream_keylog_ctx, /* module context */
        ngx_stream_keylog_commands,   /* module directives */
        NGX_STREAM_MODULE,               /* module type */
        NULL,                          /* init master */
        NULL,                          /* init module */
        NULL,                          /* init process */
        NULL,                          /* init thread */
        NULL,                          /* exit thread */
        NULL,                          /* exit process */
        NULL,                          /* exit master */
        NGX_MODULE_V1_PADDING
};