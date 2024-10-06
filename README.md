Простая реализация зеркала - неблокирующего сервера, отправляющего клиенту те же данные, что и пришли.
Реализован с помощью libev, ожидает коннекты на 5000 порт, логгирует все данные в консоль. 

Установка и сборка:
```sh
git clone https://github.com/prichinatryaski/mirror.git
cd mirror
```
Для сборки необходимы cmake и libev.

Установка зависимостей для Debian:
```sh
sudo apt update && sudo apt upgrade
sudo apt install build-essential cmake make libev-dev libev4
```
Установка зависимостей для Fedora:
```sh
sudo dnf makecache --refresh
sudo dnf install cmake make libev
```
Сборка:
```sh
mkdir build
cd build
cmake ..
make
```

Запускается сервер с помощью `./tcp_server`

Клиент (отправляет данные на 5000 порт) требует установленного python3 и запускается с помощью `python3 client.py`

Автотесты из папки tests запускаются с помощью `python3 tests/autotest.py`. Для их корректной работы необходима библиотека unittest. Её можно поставить с помощью `pip3 install unittest`


