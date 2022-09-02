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
    git

CMD git clone https://github.com/Corvusoft/restbed.git
WORKDIR /colnago/restbed
CMD cmake .
CMD make instal

WORKDIR /colnago
CMD rm -rf CMakeCache.txt \
    &&cmake . \
    && make -j8 \
    && cd bin \
    && ./colnago \
