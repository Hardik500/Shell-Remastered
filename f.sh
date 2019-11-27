#!/bin/bash
HOST=ftp.cesca.es
USER=anonymous
PASSWORD=P@ssw0rd
ftp -inv $HOST <<EOF
user $USER $PASSWORD
pass
ls
cd ubuntu
cd release
lcd /home/cyberpunk/Desktop/project/new
get robots.txt
bye
EOF
