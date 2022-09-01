FROM ubuntu:latest
RUN mkdir -p /colnago
WORKDIR /colnago
COPY . /colnago/
RUN sed -i "s@http://.*security.ubuntu.com@https://mirrors.tuna.tsinghua.edu.cn@g" /etc/apt/sources.list
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

RUN git clone https://github.com/Corvusoft/restbed.git
WORKDIR /colnago/restbed
RUN cmake .
RUN make install
RUN rm -rf /colnago/restbed

WORKDIR /colnago
CMD rm -rf CMakeCache.txt \
    &&cmake . \
    && make -j8 \
    && cd bin \
    && ./colnago \
