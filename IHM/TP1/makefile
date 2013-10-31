CC = g++
CFLAGS="-g -Wall" 

drawtri : main.o mainframe.o dialogs.o openglcanvas.o
	$(CC) -o drawtri main.o mainframe.o dialogs.o openglcanvas.o `wx-config --libs --gl-libs`

main.o : main.cpp 
	$(CC) `wx-config --cxxflags` -c main.cpp -o main.o ${CFLAGS}

mainframe.o : mainframe.cpp
	$(CC) `wx-config --cxxflags` -c mainframe.cpp -o mainframe.o ${CFLAGS}

dialogs.o: dialogs.cpp
	$(CC) `wx-config --cxxflags` -c dialogs.cpp -o dialogs.o ${CFLAGS}

openglcanvas.o: openglcanvas.cpp
	$(CC) `wx-config --cxxflags` -c openglcanvas.cpp -o openglcanvas.o ${CFLAGS}



clean : 
	rm -f *.o drawtri 
	
