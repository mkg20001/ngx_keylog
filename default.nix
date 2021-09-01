with (import <nixpkgs> {});
stdenv.mkDerivation {
  name = "sslkeylog";

  src = ./.;

  nativeBuildInputs = [
    cmake
    pkg-config
  ];

  buildInputs = [
    openssl
    nginx
  ];
}
