ifndef OS # linux
LIBRARY_EXTENSION=.a
else ifeq ($(OS), Windows_NT) # windows
LIBRARY_EXTENSION=.lib
else
$(error OS not supported)
endif

ifndef OS # linux
RM=rm -f $(1)
else # windows
RM=if exist $(1) del $(1)
endif

liblcg-mini$(LIBRARY_EXTENSION): lcg_mini.o
	ar rcs $@ $^

lcg_mini.o: lcg_mini.c
	gcc -c $<

.PHONY: clean
clean:
	$(call RM,lcg_mini.o)
	$(call RM,liblcg_mini.a)
	$(call RM,liblcg_mini.lib)