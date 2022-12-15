{ pkgs   ? import <nixpkgs> {},
  stdenv ? pkgs.stdenv,
  cc ? pkgs.gcc,
  perlPackages ? pkgs.perl534Packages,
  mkWrapper ? pkgs.makeWrapper
}:

stdenv.mkDerivation rec {
  name = "heartbeat-linux";

  src = ./.;

  buildInputs = [ cc mkWrapper ];

  buildPhase = ''
  '';

  installPhase = ''
    mkdir -p $out
  '';

  postFixup = ''
    wrapProgram $out/rf_compiler/transform.pl \
      --prefix PERL5LIB : "${with perlPackages; makePerlPath [  ]}"
  '';

  meta = {
    description = "Library support for the heartbeat-interrupt kernel module.";
  };
}
