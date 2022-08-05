# colnago

Lightweight just-in-time web application

## quick start

1、env config  

```bash
sudo bash ./env.sh
```

2、build and run  

```bash
sudo bash ./run.sh
```

3、depoly process  

```bash
sudo bash ./depoly.sh
sudo bash ./ps.sh
```

4、search process and stop it

```bash
root@drecbb4udzdboiei-0626900:/mes/colnago# sudo bash ./ps.sh
root     3186794 3186743  0 03:43 pts/0    00:00:00 ./colnago
root     3186905 3186903  0 03:45 pts/0    00:00:00 grep colnago

root@drecbb4udzdboiei-0626900:/mes/colnago# sudo kill -9 3186794
depoly.sh: line 4: 3186794 Killed                  ./colnago

root@drecbb4udzdboiei-0626900:/mes/colnago# sudo bash ./ps.sh
root     3186991 3186989  0 03:46 pts/0    00:00:00 grep colnago
```

## dependences

[@sqlite](https://github.com/sqlite/sqlite)  
[@restbed](https://github.com/Corvusoft/restbed)  
[@json](https://github.com/nlohmann/json)  
[@libcurl](https://github.com/curl/curl)  

## web api

[https://documenter.getpostman.com/view/18416843/VUjLJmPo](https://documenter.getpostman.com/view/18416843/VUjLJmPo)  
