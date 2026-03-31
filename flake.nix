{
  description = "Mineclone C++ OpenGL project";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";

      pkgs = import nixpkgs { inherit system; };
      pkgsWin = pkgs.pkgsCross.mingwW64;
    in
    {
      packages.${system} = {

        default = pkgs.stdenv.mkDerivation {
          pname = "mineclone";
          version = "1.1.0";
          src = self;

          nativeBuildInputs = with pkgs; [ 
            cmake 
            pkg-config 
          ];

          buildInputs = with pkgs; [
            glfw
            libGL
            libX11
            libXrandr

            stb
          ];
        };

        windows = pkgsWin.stdenv.mkDerivation {
          pname = "mineclone";
          version = "1.1.0";
          src = self;

          nativeBuildInputs = [ pkgsWin.cmake ];
          buildInputs = [ pkgsWin.glfw ];

          cmakeFlags = [
            "-DCMAKE_SYSTEM_NAME=Windows"
          ];
          postInstall = ''
            for dll in \
              ${pkgsWin.glfw}/bin/glfw3.dll \
              ${pkgsWin.stdenv.cc.cc.lib}/x86_64-w64-mingw32/lib/libgcc_s_seh-1.dll \
              ${pkgsWin.stdenv.cc.cc.lib}/x86_64-w64-mingw32/lib/libstdc++-6.dll \
              ${pkgsWin.windows.mcfgthreads}/bin/libmcfgthread-2.dll
            do
              cp --no-preserve=all $dll $out/bin/
            done
          '';
        };
      };
    };
}
