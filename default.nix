{ pkgs   ? import <nixpkgs> {},
  stdenv ? pkgs.stdenv,
  cc ? pkgs.gcc,
  perlPackages ? pkgs.perl534Packages,
  mkWrapper ? pkgs.makeWrapper
}:

stdenv.mkDerivation rec {
  name = "rollforward";

  src = ./.;

  buildInputs = [ cc mkWrapper ];

  buildPhase = ''
  '';

  installPhase = ''
    mkdir -p $out
    mkdir -p $out/x64
    cp src/x64/rollforward.* $out/x64
    cp src/x64/transform.pl $out/x64
  '';

  postFixup = ''
    wrapProgram $out/x64/transform.pl \
      --prefix PERL5LIB : "${with perlPackages; makePerlPath [ ]}"
  '';

  meta = {
    description = "Rollforward compiler and runtime support for posix systems.";
  };
}
