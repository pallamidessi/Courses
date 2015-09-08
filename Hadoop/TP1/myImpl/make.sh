#!/bin/bash

rm -rf output/
hadoop com.sun.tools.javac.Main WordCount.java
jar -cvf wc.jar *.class
hadoop jar wc.jar WordCount input output
