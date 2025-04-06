#!/usr/bin/env python3

import argparse
import os
import sys

from litex.build.tools import replace_in_file


def main():
    parser = argparse.ArgumentParser(description="litex bare metal demo app.")
    parser.add_argument(
        "--build-path", help="target's build path (ex build/board_name).", required=True
    )
    parser.add_argument("--src-path", help="src path (ex demo).", required=True)
    parser.add_argument("--with-cxx", action="store_true", help="enable cxx support.")
    parser.add_argument(
        "--mem",
        default="main_ram",
        help="memory region where code will be loaded/executed.",
    )
    args = parser.parse_args()

    os.system(
        f"chmod -R u+w {args.src_path}"
    )  # Nix specific: Allow linker script to be modified.

    # update memory region.
    replace_in_file(f"{args.src_path}/linker.ld", "main_ram", args.mem)

    # compile demo
    build_path = (
        args.build_path
        if os.path.isabs(args.build_path)
        else os.path.join("..", args.build_path)
    )
    os.system(
        f"export BUILD_DIR={build_path} && {'export WITH_CXX=1 &&' if args.with_cxx else ''} cd {args.src_path} && make V=1"
    )

    # prepare flash boot image.
    python3 = (
        sys.executable or "python3"
    )  # nix specific: reuse current python executable if available.
    os.system(
        f"{python3} -m litex.soc.software.crcfbigen {args.src_path}/demo.bin -o {args.src_path}/demo.fbi --fbi --little"
    )  # FIXME: Endianness.


if __name__ == "__main__":
    main()
