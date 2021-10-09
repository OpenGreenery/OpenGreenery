FROM ubuntu:20.04
# avoid timezone selection:
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
    git \
    cmake \
    build-essential \
    qt5-default \
    libqt5charts5-dev \
    curl zip unzip tar pkg-config python3 python3-setuptools

RUN git clone https://github.com/Microsoft/vcpkg.git && \
    ./vcpkg/bootstrap-vcpkg.sh && \
    /vcpkg/vcpkg install grpc

RUN git clone https://github.com/OpenGreenery/OpenGreenery.git && \
    mkdir OpenGreenery/src/device/build

WORKDIR /OpenGreenery/src/device/build


CMD cmake .. -DCMAKE_TOOLCHAIN_FILE=/usr/share/vcpkg/scripts/buildsystems/vcpkg.cmake && \
    cmake --build .
