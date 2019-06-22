# cetcd-client
A C client for etcd.
## Install libcurl
if on Ubuntu
```
apt-get install libcur14-openssl-dev
```
if on Centos/RetHat
```
yum install libcurl-devel
```
if permission is denied, use `sudo` to install

## Compile
```
make
```
## Run
### PUT key-value
```
./put etcd-server-ip:port key value
```
for example:
```
./put 127.0.0.1:2379 foo bar
```
### GET value
```
./get etcd-server-ip:port key
```
for example:
```
./get 127.0.0.1:2379 foo
```
### DELETE key-value
```
./delete etcd-server-ip:port key
```
for example:
```
./delete 127.0.0.1:2379 foo
```
### WATCH key
```
./watch etcd-server-ip:port key
```
for example:
```
./watch 127.0.0.1:2379 foo
```
** The watch program is multi-threaded and it will block. If you want to end watch, please press CTRL+C. The program will exit gracefully. **
