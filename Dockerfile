FROM ubuntu:latest
RUN mkdir -p /colnago
WORKDIR /colnago
COPY . /colnago/

RUN apt update -y
RUN apt install -y \
    cmake \
    g++ \
    make \
    curl \
    libcurl4-openssl-dev \
    libboost-all-dev \
    libasio-dev \
    catch \
    libsqlite3-dev \ 
    sqlite \
    librestbed-dev

WORKDIR /colnago
CMD rm -rf CMakeCache.txt \
    &&cmake . \
    && make -j8 \
    && cd bin \
    && ./colnago \
