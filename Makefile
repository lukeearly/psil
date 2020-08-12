CPP=g++

CPPFLAGS=-I$(IDIR)

IDIR=include
ODIR=obj

LIBS=

_DEP=core/ast.hpp syntax/psil/lex.hpp
_OBJ=core/ast.o syntax/psil/lex.o syntax/psil/parse.o

DEP=$(patsubst %,$(IDIR)/%,$(_DEP))
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

psil: $(OBJ)
	$(CPP) -o $@ $^ $(CPPFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ) psil
