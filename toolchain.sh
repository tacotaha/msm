#!/bin/bash

VERSION=arm-gnu-toolchain-12.2.rel1-x86_64-aarch64-none-linux-gnu

wget https://developer.arm.com/-/media/Files/downloads/gnu/12.2.rel1/binrel/$VERSION.tar.xz
tar -xvf $VERSION.tar.xz
mv $VERSION arm-gnu-toolchain
