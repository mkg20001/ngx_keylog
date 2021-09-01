typedef struct {
    ngx_str_t keylog_location;
    ngx_flag_t keylog_enabled;
} ngx_http_keylog_main_conf_t;

static ngx_command_t ngx_http_keylog_commands = {
        { ngx_string("keylog_location")
          NGX_HTTP_MAIN_CONF|NGX_CONF_1MORE;
        }
        { ngx_string("keylog_enabled")
          NGX_HTTP_MAIN_CONF|NGX_CONF_FLAG;
        }
};

static ngx_http_module_t  ngx_http_circle_gif_module_ctx = {
        NULL,                          /* preconfiguration */
        NULL,                          /* postconfiguration */

        NULL,                          /* create main configuration */
        NULL,                          /* init main configuration */

        NULL,                          /* create server configuration */
        NULL,                          /* merge server configuration */

        ngx_http_circle_gif_create_loc_conf,  /* create location configuration */
        ngx_http_circle_gif_merge_loc_conf /* merge location configuration */
};

static void *
ngx_http_keylog_create_main_conf(ngx_conf_t *cf) {
    ngx_http_keylog_main_conf_t  *conf;
    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_keylog_main_conf_t));
    if (conf == NULL) {
        return NGX_CONF_ERROR;
    }
    conf->keylog_location = NGX_CONF_UNSET_STR;
    conf->keylog_enabled = NGX_CONF_UNSET_FLAG;
}

static char *
ngx_http_keylog_merge_main_conf(ngx_conf_t *cf, void *parent, void *child) {
    ngx_http_keylog_main_conf_t *prev = parent;
    ngx_http_keylog_main_conf_t *conf = child;

    ngx_conf_merge_str_value(conf->keylog_location, prev->keylog_location, 10);
    ngx_conf_merge_flag_value(conf->keylog_enabled, prev->keylog_enabled, 20);

    if (conf->keylog_enabled && conf->keylog_location) {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "keylog enabled but keylog_location not set");
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}

ngx_module_t  ngx_http_<module name>_module = {
        NGX_MODULE_V1,
        &ngx_http_keylog_module_ctx, /* module context */
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