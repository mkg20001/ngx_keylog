# ngx_keylog

Module to log SSL Keys in the [NSS Key Log Format](https://developer.mozilla.org/en-US/docs/Mozilla/Projects/NSS/Key_Log_Format)

# Usage

```conf
server {
  keylog on;
  keylog_file /tmp/keylog-http.txt;
}

stream {
  keylog on;
  keylog_file /tmp/keylog-stream.txt;
}
```