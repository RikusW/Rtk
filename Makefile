#
# Install XQuartz on OSX to compile this


FLAGS = -DXLIB -Wextra -g

#all: libRtk.a
all: libRtk.so

clean:
	rm -fv *~
	rm -fv *.o
	rm -fv *.a
	rm -fv *.so
	rm -fv parse
	rm -fv Rtk_sisl.cpp

.cpp.o:
	g++ -DXLIB -Wall -g -fPIC -DPIC -c -o $@ $<
#	g++ -fpermissive -DXLIB -Wall -g -fPIC -DPIC -c -o $@ $<

#=========================================================#

parse: parse.cpp
	g++ $(FLAGS) -o parse parse.cpp

sisl.o: sisl.h sisl.cpp
Rtk.o: Rtk.h Rtk.cpp
Rtk_sisl.cpp: Rtk.h window.h RTextEdit.h RListView.h RTreeView.h RMenu.h parse
	./parse Rtk.h > Rtk_sisl.cpp
	./parse window.h "" >> Rtk_sisl.cpp
	./parse RTextEdit.h >> Rtk_sisl.cpp
	./parse RListView.h >> Rtk_sisl.cpp
	./parse RTreeView.h >> Rtk_sisl.cpp
	./parse RMenu.h >> Rtk_sisl.cpp
Rtk_sisl.o: Rtk_sisl.cpp
drawman.o: drawman.h drawman.cpp
dmRtk.o: drawman.h
dmWin4.o: drawman.h
RMenu.o: RMenu.h RMenu.cpp
window.o: window.h window.cpp
printxev.o: printxev.cpp
RTextEdit.o: RTextEdit.h RTextEdit.cpp
RTreeNode.o: RTreeNode.h RTreeNode.cpp
RTreeView.o: RTreeView.h RTreeView.cpp
RListView.o: RListView.h RListView.cpp
RCom.o: RCom.h RCom.cpp
RFile.o: RFile.h RFile.cpp
RHexFile.o: RHexFile.h RHexFile.cpp
RConfig.o: RFile.o RString.o RConfig.h RConfig.cpp
RString.o: RString.h RString.cpp

#=========================================================#

libRtk.a: sisl.o Rtk.o Rtk_sisl.o drawman.o dmRtk.o dmWin4.o RMenu.o window.o printxev.o\
	RTextEdit.o RTreeNode.o RTreeView.o RListView.o RCom.o RFile.o RHexFile.o RConfig.o RString.o
	rm -f libRtk.a
	ar rs libRtk.a sisl.o Rtk.o Rtk_sisl.o drawman.o dmRtk.o dmWin4.o RMenu.o window.o printxev.o\
		RTextEdit.o RTreeNode.o RTreeView.o RListView.o RCom.o RFile.o RHexFile.o RConfig.o RString.o

libRtk.so: sisl.o Rtk.o Rtk_sisl.o drawman.o dmRtk.o dmWin4.o RMenu.o window.o printxev.o\
	RTextEdit.o RTreeNode.o RTreeView.o RListView.o RCom.o RFile.o RHexFile.o RConfig.o RString.o
	g++ -shared -fPIC -g -o libRtk.so sisl.o Rtk.o Rtk_sisl.o drawman.o dmRtk.o dmWin4.o RMenu.o window.o printxev.o\
		RTextEdit.o RTreeNode.o RTreeView.o RListView.o RCom.o RFile.o RHexFile.o RConfig.o RString.o\
		-Wl,-soname -Wl,libRtk.so.0.1 -L/usr/X11R6/lib -lX11
# -lefence

#=========================================================#

bz:
	tar -cjvf ../Rtk.tbz ds97 *.h *.cpp Makefile RApps/*.h RApps/*.cpp RApps/Makefile
wc:
	wc *.h *.cpp

#=========================================================#

