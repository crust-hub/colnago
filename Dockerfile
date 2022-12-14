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
    git  \
    odb \
    libmysqlclient-dev \
    mysql-client 

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
CMD cmake . && make

WORKDIR /colnago/bin
CMD ./colnago
