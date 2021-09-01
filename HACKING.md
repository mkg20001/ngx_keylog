# hacking

```
nix-shell "<nixpkgs>" -A nginx
unpackPhase
cd nginx-*
configureFlags="$configureFlags --add-module=$PWD/../"
configurePhase
# then for testing
buildPhase
```