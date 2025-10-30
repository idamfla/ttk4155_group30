FROM ubuntu:24.04

# Install packages
RUN apt-get update && apt-get install -y \
    avarice \
    avr-libc \
    avrdude \
    binutils \
    doxygen \
    gcc-arm-none-eabi \
    gcc-avr \
    gdb-arm-none-eabi \
    git \
    graphviz \
    libftdi-dev \
    libftdi1-dev \
    libhidapi-dev \
    libusb-1.0-0-dev \
    make \
    openocd \
    sudo \
    usbutils \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Create symlinks for nm and objdump because the ARM toolchain does not provide them
RUN ln -s /usr/bin/nm /usr/bin/nm-multiarch \
    && ln -s /usr/bin/objdump /usr/bin/objdump-multiarch

RUN usermod -aG sudo ubuntu \
    && echo "ubuntu ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Change to the unprivileged user
USER ubuntu
