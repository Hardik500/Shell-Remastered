#!/bin/bash
mkdir Testing
HOST=ftp.cesca.es
USER=anonymous
PASSWORD=P@ssw0rd
ftp -inv $HOST <<EOF
user $USER $PASSWORD
pass
ls
cd ubuntu
cd release
lcd $PWD/Testing
get robots.txt
bye
EOF
