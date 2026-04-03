{
  description = "Mineclone C++ OpenGL project";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    glm-src = {
      url = "github:g-truc/glm/1.0.2";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, glm-src }:
    let
      system = "x86_64-linux";

      pkgs = import nixpkgs { inherit system; };
      pkgsWin = pkgs.pkgsCross.mingwW64;
    in
    {
      # Shell for LSP config - clangd
      devShells.${system}.default = pkgs.mkShell {
        buildInputs = with pkgs; [
          cmake
          pkg-config
          clang-tools
          glfw
          libGL
          libX11
          libXrandr
          glm
          stb
        ];

        shellHook = ''
          cat > .clangd <<EOF
          CompileFlags:
            Compiler: clang++
            Add:
              - -std=c++17
              - -I$PWD/src
              - -I$PWD/lib
              - -I${pkgs.libglvnd.dev}/include
              - -I${pkgs.glfw}/include
              - -I${pkgs.glm}/include
              - -I${pkgs.stb}/include
              - -I${pkgs.stdenv.cc.cc.lib}/include
          EOF
          echo "ambiente pronto"
        '';
      };

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

            glm
            stb
          ];
        };

        windows = pkgsWin.stdenv.mkDerivation {
          pname = "mineclone";
          version = "1.1.0";
          src = self;

          preConfigure = ''
            mkdir -p include
            cp -r ${glm-src}/glm include/glm
            export CXXFLAGS="-I$PWD/include"
          '';

          nativeBuildInputs = [ pkgsWin.cmake ];
          buildInputs = with pkgsWin; [
            glfw
            stb
          ];

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
