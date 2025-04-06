## litex rocket chip template

### requirements
- nix or litex system dependencies
- uv or litex python modules in path

#### optional
- direnv, just command runner for convenience

### setup
- have system dependencies of litex installed or activate nix devshell
- start hacking! (refer to justfile for commands)
- if you have just installed j simr should run the demo simulation

### repo structure
- litex - contains litex_sim example projects
    - demo - basic baremetal demo from litex, modified to demonstrate fpu
    - ba - very basic bundle adjustment workflow

- qemu - contains qemu example projects
    - t1 - pure assembly baremetal helloworld demo
    - t2 - basic startup assembly and main.c example to print helloworld
    - t4 - floating point operations test in qemu
