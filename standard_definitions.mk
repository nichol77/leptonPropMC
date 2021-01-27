

#
#
#
ObjSuf	      = o
SrcSuf        = cxx
ExeSuf        =
OPT           = -O2 -g --debug  -Wall -fPIC
CXXOPT        = 

NOOPT         =
EXCEPTION     = 

CXX           = g++
INCLUDES      = -I $(shell $(ROOTSYS)/bin/root-config --incdir)
CXXFLAGS      = $(EXCEPTION) $(OPT) $(CXXOPT) -fPIC $(INCLUDES)
LD            = g++
LDFLAGS       = $(CXXOPT) $(EXCEPTION)
LIBS          = -lz $(ROOTLIBS) -lm -ldl 
GLIBS         = $(ROOTLIBS) $(ROOTGLIBS) -lm -ldl
#
ROOTLIBS      = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --libs) -lMinuit
ROOTGLIBS     = -L$(ROOTSYS)/lib $(shell $(ROOTSYS)/bin/root-config --glibs)

Src = $(Target).$(SrcSuf)
Obj = $(Target).$(ObjSuf)

all : $(Target)

%.$(ObjSuf) : %.$(SrcSuf)
	@echo "<**Compiling**> "$<
	@$(CXX) $(CXXFLAGS) -c $< -o  $@

%Dict.C : %.h
	@echo "Generating dictionary ..."
	@rootcint  $@ -c $(INCLUDES) $<


subclean:
	@rm -rf *.o
	@rm -rf *.d
	@rm -rf G__auto*
	@rm -rf *Dict.?
	@rm -rf *dict.?




















