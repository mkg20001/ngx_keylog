# hacking

```
# generate cert
nix-env -p mkcert --run "mkcert localhost"
# enter shell with nginx
nix-shell "<nixpkgs>" -A nginx
# run those in that shell
unpackPhase
cd nginx-*
configureFlags="$configureFlags --add-module=$PWD/../"
configurePhase
# then for testing
buildPhase
./objs/nginx -c $PWD/../test.conf
```