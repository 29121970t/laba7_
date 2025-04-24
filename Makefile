#Libs += lib/randomLib.o lib/LabMenu.o lib/strLib.o
Libs += lib/parseLib5.o
Libs += lib/strLib.o
Libs += lib/trees.o




CFLAGS += -x c
CFLAGCS += -Wall -Wextra -Wpedantic \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs
execs=$(filter %.c, $(shell ls))
CC=gcc

all: $(patsubst %.c, %.o, $(execs))


lib/%.o: %.c
	$(CC) $(CFLAGCS) -c $^ -o $@

%.o: $(Libs) %.c
	$(CC) $(CFLAGCS) $^ -o $*.o -lm 

$(Libs): %.o : %.c
	$(CC) $(CFLAGCS) -c $^ -o $@
clean:
	-rm *.o
	-rm lib/*.o
	-rm *.h.gch
	-rm lib/*.h.gch



