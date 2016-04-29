BIN = LectEcrivain
all:  $(BIN:=.class)
run: 	
	java $(BIN)

%.class : %.java
	javac $*.java


clean :
	/bin/rm -f *.class

realclean : clean
	/bin/rm -f *~  pok *Helper.java *Holder.java *POA.java annu.ior iorfile.java
