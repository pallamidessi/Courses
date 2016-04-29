#!/bin/bash

rm -rf output/ output2/
hadoop com.sun.tools.javac.Main Degree.java
jar -cvf wc.jar *.class
hadoop jar wc.jar Degree input output
