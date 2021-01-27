#---------------------------------------------------
include standard_definitions.mk


CXX           = g++
LD            = g++
SOFLAGS       = -shared


LIB_OBJS          =  interactions.o InteractionInfo.o NeutrinoEvent.o Propagator.o  WorldModel.o BedmapTable.o RayTracer.o RFEvent.o SubWaveInfo.o propDict.o
HDRS      = InteractionInfo.h NeutrinoEvent.h Propagator.h WorldModel.h BedmapTable.h RayTracer.h RFEvent.h SubWaveInfo.h

LIBRARY       = lib/libPropagation.so

THIS_LIB = -Llib -lPropagation

TARGETS = propagateThroughIce doRayTracing

OBJECTS = $(TARGETS).$(ObjSuf)

all: $(TARGETS)

$(TARGETS): % : %.o $(LIBRARY)
	@echo "Linking $@..."
	@/bin/rm -f $@	
	@$(LD) $(LDFLAGS) $@.o $(LIBS) $(THIS_LIB) -o $@
	@chmod 755 $@


$(LIBRARY): $(LIB_OBJS)
	@echo "Linking $(LIBRARY) ..."
	@/bin/rm -f $(LIBRARY)	
	$(LD) $(SOFLAGS) $(LDFLAGS) $(LIB_OBJS) -o $(LIBRARY)
	@chmod 555 $(LIBRARY)



###

propDict.C: $(HDRS)
	@echo "Generating dictionary ..."
	@ rm -f *Dict* 
	rootcint propDict.C -c $(HDRS)

distclean :
	@ rm -f *dict*
	@ make clean
	@for aDir in $(SUBDIRS); do \
	( cd $$aDir ; make clean ) ; \
	done
	@ make clean
#---------------------------------------------------

clean: subclean
	@rm -rf $(TARGETS)
	@rm -rf $(LIBRARY)

















