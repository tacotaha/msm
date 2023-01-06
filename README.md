# msm

# Cross compiling for ARM (aarch64) targets

Clone the repo

```sh
git clone --recursive https://github.com/tacotaha/msm && cd msm
```

Download and extract the latest toolchain (aarch64-none-linux-gnu)

```sh
./toolchain.sh
```

Consider passing architecture specific flags (e.g. `-march=armv8-a -mtune=cortex-a73`)

```sh
make CC=arm-gnu-toolchain/bin/aarch64-none-linux-gnu-gcc all
```

This builds a static binary `msm` in the current directory. Drop it on the device and run with

```sh
OMP_NUM_THREADS=4 ./msm
```
