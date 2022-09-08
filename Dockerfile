FROM ubuntu:latest
RUN mkdir -p /colnago
WORKDIR /colnago
COPY . /colnago/

# env prepare
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
    git \
    nodejs \
    npm

WORKDIR ~
CMD npm install -g pm2

# installing restbed
WORKDIR /restbed
CMD git clone https://github.com/Corvusoft/restbed.git
WORKDIR /restbed/restbed
CMD cmake . && \
    make && \
    make install
CMD cp -r ./distribution/include/* /usr/include/
CMD cp -r ./distribution/library/* /usr/lib/

# build project
WORKDIR /colnago
CMD rm -rf *Cache.txt \
    &&cmake . \
    && make -j8

# pm2
WORKDIR /colnago/bin
CMD pm2 start colnago
