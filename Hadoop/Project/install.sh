#!/bin/bash

ROOT_UID=0

if [ $UID != $ROOT_UID ]; then
	echo "Run this script as superuser!"
	exit 1
fi

apt-get install -y virtualenvwrapper
apt-get install -y python2.7
apt-get install -y python-pip

if [ ! -f node-v5.3.0-linux-x64.tar.gz ]; then
  wget https://nodejs.org/dist/v5.3.0/node-v5.3.0-linux-x64.tar.gz ~
fi

tar -C /usr/local --strip-components 1 -xzvf ~/node-v5.3.0-linux-x64.tar.gz  

if [ ! -f hadoop-2.7.1.tar.gz ]; then
  wget http://apache.crihan.fr/dist/hadoop/common/hadoop-2.7.1/hadoop-2.7.1.tar.gz ~
fi

tar -C ~ -xzvf hadoop-2.7.1.tar.gz 

pushd ~
source .bashrc
popd

source "/usr/bin/virtualenvwrapper.sh"
source /etc/bash_completion.d/virtualenvwrapper
mkvirtualenv hadoop-twitter-sentiment

pushd ~
source .bashrc
popd

workon hadoop-twitter-sentiment
pip install -r trained_classifier/requirements.txt
python trained_classifier/install.py

pushd front 
npm install -g
node_modules/sails/bin/sails.js lift
popd

