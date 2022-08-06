sudo apt update -y &&
sudo apt install git -y &&
sudo apt install cmake -y &&
sudo apt install make -y &&
sudo apt install curl -y &&
sudo apt install  libcurl4-openssl-dev &&
sudo apt install libboost-all-dev -y &&
sudo apt install -y libasio-dev catch &&
sudo apt install -y librestbed-dev &&
sudo apt install -y libsqlite3-dev &&
sudo apt install -y sqlite &&
sudo apt install -y python3.8 &&
sudo rm -rf ./envtempdir &&
sudo mkdir envtempdir &&
cd envtempdir &&
sudo git clone https://github.com/nlohmann/json.git &&
cd ./json &&
cmake . &&
make install &&
cd .. &&
sudo rm -rf json &&
cd .. &&
sudo rm -rf ./envtempdir &&
echo "over"
