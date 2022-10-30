apt update  -y && \
apt install -y git cmake make curl libcurl4-openssl-dev libboost-all-dev libasio-dev odb catch mysql-client && \
git clone https://github.com/Corvusoft/restbed.git && \
cd restbed && \
cmake . && \
make && \
make install && \
cd distribution && \ 
cp -r ./include/* /usr/include/ && \
cp -r ./library/* /usr/lib/ && \
echo "over"
