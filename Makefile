#
# Install XCode and XQuartz on OSX to compile this
#

CXXFLAGS = -DXLIB -fPIC -DPIC -g -Wall -Wextra -Wno-unused-parameter -I../Rtk-base

lib-objs := sisl.o Rtk.o Rtk_sisl.o drawman.o dmRtk.o dmWin4.o RMenu.o RMenu_sisl.o window.o window_sisl.o printxev.o\
		RTextEdit.o RTextEdit_sisl.o RTreeView.o RTreeView_sisl.o RListView.o RListView_sisl.o

#all: libRtk.a
all: libRtk.so

clean:
	rm -fv *~
	rm -fv *.o
	rm -fv *.a
	rm -fv *.so
	rm -fv *.dep
	rm -fv parse
	rm -fv *_sisl.cpp

#=========================================================#
#Rules

#Keep parse output
.PRECIOUS: %_sisl.cpp

parse: parse.cpp

%_sisl.cpp: %.h parse
	./parse $< Rtk.h $< > $@

#=========================================================#
#dependencies

%.dep: %.cpp
	@echo Making $@
	@echo -n "$@ " > $@
	@$(CXX) -M $(CXXFLAGS) $< >> $@

-include $(subst .o,.dep,$(lib-objs))

#=========================================================#

libRtk.a: $(lib-objs)
	rm -f libRtk.a
	ar rs libRtk.a $^

libRtk.so: $(lib-objs)
	g++ -shared -fPIC -g -o libRtk.so $^ -Wl,-soname -Wl,libRtk.so.0.1 -L/usr/X11R6/lib -lX11 -lRtk-base
# -lefence

linkso:
	ln -s `pwd`/libRtk.so /usr/lib/libRtk.so
	ln -s `pwd`/libRtk.so /usr/lib/libRtk.so.1
	ln -s `pwd`/libRtk.so /usr/lib/libRtk.so.0.1

rmlinkso:
	rm /usr/lib/libRtk.so
	rm /usr/lib/libRtk.so.1
	rm /usr/lib/libRtk.so.0.1

#=========================================================#

bz: clean
	tar -cjvf ../Rtk.tbz *.h *.cpp Makefile
wc:
	wc *.h *.cpp

#=========================================================#

