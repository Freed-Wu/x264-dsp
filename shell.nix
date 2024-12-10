{ pkgs ? import <nixpkgs> { } }:

with pkgs;
mkShell {
  name = "x264-dsp";
  buildInputs = [
    meson
    ninja
  ];
}
