# hacking

## nixOS

```
# generate cert
nix-env -p mkcert --run "mkcert localhost"
# enter shell with nginx
nix-shell "<nixpkgs>" -A nginx
# run this in that shell
unpackPhase
cd nginx-*
configureFlags="$configureFlags --add-module=$(readlink -f $PWD/..)"
configurePhase
# then for testing
buildPhase
./objs/nginx -c $PWD/../test.conf
```

## non-nixOS

dependencies:
- mkcert
- nginx source code, place in nginx-VERSION directory
- everything required to build nginx
- openssl 1.1.1+

```
mkcert localhost
cd nginx-VERSION
./configure --add-module=$(readlink -f $PWD/..) --with-stream --with-stream_ssl-module
make
./objs/nginx -c $PWD/../test.conf
```

# clion

- First do setup
- Open project in CLion: `clion ngx_keylog` (on nixOS first run `nix-shell`, then run clion from there)
- Help > Find Action > Create CMakeLists.txt
- Leave everything as-is
- CLion completion should now work (building doesn't work via clion)
