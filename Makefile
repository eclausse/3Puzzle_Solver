#Definie le nom du projet
NOMPROJET= taquin_solver
NOMPROJETDOXY = "Projet Taquin A*"

#Compilation
CC = gcc
#Definie mes flags 
CFLAG = -Wall -Wextra -g
#Definie mes dependences
LDLIBS = -lm
#Definie la commande pour supprimmer
RM = rm -f

#Definie mes dossiers
SRCDIR = src/
BINDIR = bin/
DOCDIR = doc/
SAVEDIR = save/

#Define mon executable
PROG = $(BINDIR)$(NOMPROJET)
#Definie mes fichier sources et mes fichier objets
SRC = $(wildcard $(SRCDIR)*.c)
OBJ = $(subst $(SRCDIR), $(BINDIR), $(SRC:.c=.o))

all : $(PROG)

$(PROG) : $(OBJ)
	$(CC) $(CFLAG) $^ -o $@ $(LDLIBS)

$(BINDIR)%.o : $(SRCDIR)%.c
	$(CC) -c $^ -o $@ $(LDLIBS)



test : $(PROG) execute

execute : 
	./$(PROG)

.PHONY : clean tree save doc test

clean : 
	$(RM) $(BINDIR)*.o
	$(RM) -f Doxyfile.bak Doxyfiler

#Créer l'arboraissance 
tree : 
	mkdir -p $(SRCDIR) $(BINDIR) $(DOCDIR) $(SAVEDIR)

#Création d'une sauvegarde
save : 
	cp $(SRCDIR)* $(SAVEDIR)

restore : 
	cp $(SAVEDIR)* $(SRCDIR)

#Création de la documentation
doc :
	doxygen -g
	sed -ir 's|^PROJECT_NAME           = "My Project"|PROJECT_NAME           = $(NOMPROJETDOXY)|' Doxyfile
	sed -ir "s|^OUTPUT_DIRECTORY       =|OUTPUT_DIRECTORY       = $(DOCDIR)|" Doxyfile
	sed -ir "s|^INPUT                  =|INPUT                  = $(SRCDIR)|" Doxyfile
	doxygen Doxyfile
	$(RM) Doxyfile.bak Doxyfiler

readme : 
	echo "# Projet Taquin A*" > README

	echo "\n# Compilation" >> README
	echo "make" >> README

	echo "\n# Execution" >> README
	echo "./$(PROG)" >> README

	echo "\n# Documentation" >> README
	echo "make doc" >> README

	echo "\n# Archivage" >> README
	echo "make tar" >> README

	echo "\n# Nettoyage" >> README
	echo "make clean" >> README

	echo "\n# Sauvegarde du code" >> README
	echo "make save" >> README

	echo "\n# Restoration du code" >> README
	echo "make restore" >> README

	echo "\n# Auteur" >> README
	echo "Evan Clausse ING2 GSI Groupe 1" >> README
