FROM ubuntu:20.04
# avoid timezone selection:
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
    # OpenGreenery
    git \
    cmake \
    build-essential \
    qt5-default \
    libqt5charts5-dev \
    # gRPC
    build-essential autoconf libtool pkg-config

# Install gRPC
WORKDIR /tmp
RUN git clone --recurse-submodules -b v1.41.0 https://github.com/grpc/grpc
RUN cd grpc && mkdir -p cmake/build && cd cmake/build && \
    cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF ../.. && \
    cmake --build . && make install
