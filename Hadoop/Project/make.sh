#!/bin/bash

export JAVA_HOME=/usr/lib/jvm/java-7-openjdk-amd64/
export HADOOP_HOME=~/hadoop-2.7.1
export PATH=$HADOOP_HOME/bin:$PATH
export HADOOP_CLASSPATH=${JAVA_HOME}/lib/tools.jar:.

cd ..
rm -rf output/
rm -rf output2/
rm -rf output3/
hadoop com.sun.tools.javac.Main BatchTextAnalysis.java
javac Levenstein.java
jar -cvf wc.jar *.class
hadoop jar wc.jar BatchTextAnalysis input output
