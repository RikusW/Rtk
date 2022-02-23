#
# Install XCode and XQuartz on OSX to compile this
#

CXXFLAGS = -DXLIB -fPIC -DPIC -g -Wall -Wextra -Wno-unused-parameter -I../

lib-objs := sisl.o Rtk.o Rtk_sisl.o drawman.o dmRtk.o dmWin4.o RMenu.o RMenu_sisl.o RWindow.o RWindow_sisl.o printxev.o\
		RTextEdit.o RTextEdit_sisl.o RTreeView.o RTreeView_sisl.o RListView.o RListView_sisl.o

#all: libRtk.a
all: libRtk-gui.so

clean:
	rm -fv *~
	rm -fv *.o
	rm -fv *.a
	rm -fv *.so
	rm -fv *.dep
	rm -fv Rtk-parse
	rm -fv *_sisl.cpp

#=========================================================#
#Rules

#Keep Rtk-parse output
.PRECIOUS: %_sisl.cpp

Rtk-parse: Rtk-parse.cpp

%_sisl.cpp: %.h Rtk-parse
	./Rtk-parse $< > $@

#=========================================================#
#dependencies

%.dep: %.cpp
	@echo Making $@
	@echo -n "$@ " > $@
	@$(CXX) -M $(CXXFLAGS) $< >> $@

-include $(subst .o,.dep,$(lib-objs))

#=========================================================#

libRtk-gui.a: $(lib-objs)
	rm -f $@
	ar rs $@ $^

libRtk-gui.so: $(lib-objs)
	g++ -shared -fPIC -g -o $@ $^ -Wl,-soname -Wl,$@.0.1 -L/usr/X11R6/lib -lX11 -lRtk-base
# -lefence

linkso:
	ln -s `pwd`/Rtk-parse /usr/bin/Rtk-parse
	ln -s `pwd`/libRtk-gui.so /usr/lib/libRtk-gui.so
	ln -s `pwd`/libRtk-gui.so /usr/lib/libRtk-gui.so.1
	ln -s `pwd`/libRtk-gui.so /usr/lib/libRtk-gui.so.0.1

rmlinkso:
	rm /usr/bin/Rtk-parse
	rm /usr/lib/libRtk-gui.so
	rm /usr/lib/libRtk-gui.so.1
	rm /usr/lib/libRtk-gui.so.0.1

#=========================================================#

bz: clean
	tar -cjvf ../Rtk.tbz *.h *.cpp Makefile
wc:
	wc *.h *.cpp

#=========================================================#

