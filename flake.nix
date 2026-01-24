# SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later

{
  description = "x86_64 dev environment for Drawy, the whiteboard app.";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";

    systems = {
      url = "github:nix-systems/default";
    };

    git-hooks = {
      url = "github:cachix/git-hooks.nix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs =
    {
      self,
      nixpkgs,
      systems,
      ...
    }@inputs:
    let
      forEachSystem = nixpkgs.lib.genAttrs (import systems);
    in
    {
      devShells = forEachSystem (system: {
        default =
          let
            pkgs = nixpkgs.legacyPackages.${system};
            inherit (self.checks.${system}.pre-commit-check) shellHook enabledPackages;
          in
          pkgs.mkShell {
            buildInputs =
              enabledPackages
              ++ (with pkgs; [
                # Qt
                qt6.qttools
                qt6.qtbase
                qtcreator

                pre-commit
                cmake
                ninja
                bear
                entr
                zstd.dev
                pkg-config
                reuse
                libclang.python
                kdePackages.extra-cmake-modules
                kdePackages.kcrash.dev
                kdePackages.kwidgetsaddons
                kdePackages.kconfig
                kdePackages.kconfigwidgets
                kdePackages.kxmlgui
              ]);
          };
      });

      checks = forEachSystem (
        system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
        in
        {
          pre-commit-check = inputs.git-hooks.lib.${system}.run {
            src = ./.;
            hooks = {
              nixfmt-rfc-style.enable = true;
              clang-format = {
                enable = true;
                package = pkgs.clang-tools;
                types_or = pkgs.lib.mkForce [
                  "c"
                  "c++"
                ];
              };
            };
          };
        }
      );

      formatter = forEachSystem (
        system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
          config = self.checks.${system}.pre-commit-check.config;
          inherit (config) package configFile;
          script = ''
            ${pkgs.lib.getExe package} run --all-files --config ${configFile}
          '';
        in
        pkgs.writeShellScriptBin "pre-commit-run" script
      );

      packages = forEachSystem (
        system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
        in
        {
          default = pkgs.qt6Packages.callPackage ./package.nix {
          };
        }
      );
    };
}
