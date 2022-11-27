DIREXE := exec/
DIRSRC := src/
LDLIBS := -lpthread -lrt
CC := g++

all: dirs SSOOIIGLE

dirs:
	mkdir -p $(DIREXE)
	mkdir  busquedas

SSOOIIGLE:
	$(CC)  $(DIRSRC)Buscador.cpp -o $(DIREXE)Buscador -pthread -std=c++11

test:
	./$(DIREXE)Buscador 

clean:
	rm -rf $(DIREXE)
	rm -rf busquedas