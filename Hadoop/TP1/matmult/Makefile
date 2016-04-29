#
# 
# Makefile for Hadoop 2.x
#
# ------------------------------------------------------------------------------
# Take care of setting the $JAVA_HOME and $HADOOP_CLASS as examplified below
#
# On MacOSX Yosemite a convenient way of setting JAVA_HOME is 
# export JAVA_HOME=`/usr/libexec/java_home -v 1.8`


# export HADOOP_VERSION=2.4.0
# export HADOOP_HOME=/usr/local/Cellar/hadoop/${HADOOP_VERSION}   # from HomeBrew
# export PATH=$HADOOP_HOME/bin:$PATH
# export HADOOP_CLASSPATH=${JAVA_HOME}/lib/tools.jar
# ------------------------------------------------------------------------------



JAVACC=javac
JAR=jar
DIR_CLASSES=hadoop_classes
INFO="-> run with 'hadoop jar $@ WordCount input output' (make sure you have 'input' and 'output' does not exist yet)"
TARGET=matmult.jar
MAIN=MatMult

all: $(TARGET)

$(TARGET): $(MAIN).class
	mkdir -p $(DIR_CLASSES)
	$(JAR) -cvf $@ -C $(DIR_CLASSES) .
	@echo "* JAR READY *"
	@echo $(INFO)

%.class: %.java
	hadoop com.sun.tools.javac.Main -d $(DIR_CLASSES) $<

clean:
	rm -rf output/
	rm -rf res.out
	rm $(DIR_CLASSES)/*

launch:
	rm -rf res.out
	hadoop jar matmult.jar MatMult matrices.in.dat res.out

