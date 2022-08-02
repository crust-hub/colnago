sudo apt update -y &&
sudo apt install git -y &&
sudo apt install cmake -y &&
sudo apt install make -y &&
sudo apt install curl -y &&
sudo rm -rf ./envtempdir &&
sudo mkdir envtempdir &&
cd envtempdir &&
sudo git clone https://github.com/Corvusoft/restbed.git &&
cd restbed &&
sudo cmake . &&
sudo make install &&
cd .. &&
sudo rm -rf ./restbed &&
sudo git clone https://github.com/nlohmann/json.git &&
cd ./json &&
cmake . &&
make install &&
cd .. &&
sudo rm -rf json &&
cd .. &&
sudo rm -rf ./envtempdir &&
echo "over"
