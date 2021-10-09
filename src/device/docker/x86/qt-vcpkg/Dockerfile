FROM ubuntu:20.04
# avoid timezone selection:
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
    git \
    cmake \
    build-essential \
    # vcpkg:
    curl zip unzip tar pkg-config python3 \
    # qt:
    bison python3-setuptools gperf libx11-dev libxi-dev libxext-dev libgl1-mesa-dev libglu1-mesa-dev \
    # something for qt
    openssh-client \
    ca-certificates \
    locales \
    sudo \
    curl \
    build-essential \
    pkg-config \
    libgl1-mesa-dev \
    libsm6 \
    libice6 \
    libxext6 \
    libxrender1 \
    libfontconfig1 \
    libdbus-1-3 \
    libboost-all-dev \
    libxi6 \
    libgconf-2-4 \
    libgtk-3-0 \
    libx11-xcb1 \
    libpq5 \
    file \
    libnss3 \
    libxtst6 \
    libasound2 \
    fuse \
    libfuse2 \
    at curl unzip tar libxt-dev gperf libxaw7-dev cifs-utils \
    build-essential g++ gfortran zip libx11-dev libxkbcommon-x11-dev libxi-dev \
    libgl1-mesa-dev libglu1-mesa-dev mesa-common-dev libxinerama-dev libxxf86vm-dev \
    libxcursor-dev yasm libnuma1 libnuma-dev python-six python3-six python-yaml \
    flex libbison-dev autoconf libudev-dev libncurses5-dev libtool libxrandr-dev \
    xutils-dev dh-autoreconf autoconf-archive libgles2-mesa-dev ruby-full \
    pkg-config meson \
    libxext-dev libxfixes-dev libxrender-dev \
    libxcb1-dev libx11-xcb-dev libxcb-glx0-dev libxcb-util0-dev \
    libxkbcommon-dev libxcb-keysyms1-dev \
    libxcb-image0-dev libxcb-shm0-dev libxcb-icccm4-dev libxcb-sync0-dev \
    libxcb-xfixes0-dev libxcb-shape0-dev libxcb-randr0-dev \
    libxcb-render-util0-dev libxcb-xinerama0-dev libxcb-xkb-dev libxcb-xinput-dev

RUN git clone https://github.com/Microsoft/vcpkg.git && \
    ./vcpkg/bootstrap-vcpkg.sh

WORKDIR /home

RUN git clone https://github.com/OpenGreenery/OpenGreenery.git && \
    mkdir OpenGreenery/src/device/build

WORKDIR /home/OpenGreenery/src/device/build

CMD git checkout origin/device/package-manager && \
    /vcpkg/vcpkg install
