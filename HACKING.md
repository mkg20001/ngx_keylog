# hacking

## nixOS

```
# generate cert
nix-env -p mkcert --run "mkcert localhost"
# enter shell with nginx
nix-shell "<nixpkgs>" -A nginx
# run those in that shell
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
./configure --add-module=$(readlink -f $PWD/..) --enable-stream
make
./objs/nginx -c $PWD/../test.conf
```