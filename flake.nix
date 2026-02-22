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
            inherit shellHook;
            buildInputs =
              enabledPackages
              ++ (with pkgs; [
                # Qt
                qt6.qttools
                qt6.qtbase
                qtcreator

                pre-commit
                cmake
                clazy
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
                kdePackages.syntax-highlighting
              ]);
          };
      });

      checks = forEachSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        pre-commit-check = inputs.git-hooks.lib.${system}.run {
          src = ./.;

          excludes = [
            "^po/"
            "\\.desktop"
            "Messages.sh"
          ];

          hooks = {
            trim-trailing-whitespace.enable = true;
            end-of-file-fixer.enable = true;

            check-added-large-files.enable = true;
            check-case-conflicts.enable = true;
            check-symlinks.enable = true;
            check-executables-have-shebangs.enable = true;

            check-xml.enable = true;

            check-yaml = {
              enable = true;
              args = [ "--allow-multiple-documents" ];
              excludes = [ "^\\.clang-tidy" ];
            };

            check-json.enable = true;

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
      });
    };
}
