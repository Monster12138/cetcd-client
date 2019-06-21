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
./put etcd-server-addr key value
```
for example:
```
./put 127.0.0.1:2379 foo bar
```
### GET value
```
./get etcd-server-addr key
```
for example:
```
./get 127.0.0.1:2379 foo
```
