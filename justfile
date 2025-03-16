default:
    just --list

# TODO: reduce number of stages?

# rocket simulation
simr: bsim_rocket bbm_rocket fbsim_rocket

# full reset rocket sim
simrr: clean bsim_rocket bbm_rocket fbsim_rocket

# build simulation for rocket
bsim_rocket:
    litex_sim \
    --cpu-type=rocket --cpu-variant=full \
    --no-compile-gateware \
    --with-sdram \

# build baremetal software for rocket
bbm_rocket:
    demo/build.py \
    --build-path=build/sim \
    --src-path=demo \
    # --mem=rom \

# final build sim for rocket
fbsim_rocket:
    litex_sim \
    --cpu-type=rocket --cpu-variant=full \
    --with-sdram \
    --sdram-init=demo/demo.bin

clean:
    rm -rf build
    rm -f demo/**.d
    rm -f demo/**.o
    rm -f demo/**.bin
    rm -f demo/**.fbi
    rm -f demo/**.lst
    rm -f demo/**.elf**
    rm -f demo/**.S

# helper scripts
# litex_build_py src:
#     {{ src }}/build.py \
#     --build-path=build/sim \
#     --src-path={{ src }} \
