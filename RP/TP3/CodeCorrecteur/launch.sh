!# /bin/sh

rm s_* sock_*
./medium 0 & ./receiver totorec & ./sender toto
