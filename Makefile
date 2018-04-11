CC=gcc
CFLAGS=-Wall -c
LDFLAGS=-I ./include/

SRC_DIR=./src
OBJ_DIR=./obj
INC_DIR=./include
BIN_DIR=./bin
DOC_DIR=./doc
GCOV_DIR=./gcov

GCOVFLAGS=-O0 --coverage -lgcov -Wall -g

LCOV_REPORT=report.info

SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:.c=.o)
OBJ2= $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRC)))
EXEC=shellter

GEXEC=$(EXEC).cov

AR_NAME=archive_$(EXEC).tar.gz


all: $(SRC) $(EXEC)
    
%.o:%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $(OBJ_DIR)/$(notdir $@)

$(EXEC): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $(BIN_DIR)/$@ -Wall $(LDFLAGS) $(OBJ2)

$(GEXEC):
	$(CC) $(GCOVFLAGS) -o $(GCOV_DIR)/$@ -Wall $(LDFLAGS) $(SRC)

doc:
	doxygen $(DOC_DIR)/doxygen.conf

gcov: $(GEXEC)
	# generate some data for gcov by calling the generated binary with various options
	$(GCOV_DIR)/$(GEXEC) -h
	$(GCOV_DIR)/$(GEXEC) -i input -o output -v

	find ./ -maxdepth 1 -name *.gcno -exec mv {} $(GCOV_DIR) \;
	find ./ -maxdepth 1 -name *.gcda -exec mv {} $(GCOV_DIR) \;

	gcov -o $(GCOV_DIR) $(GEXEC)
	lcov -o $(GCOV_DIR)/$(LCOV_REPORT) -c -f -d $(GCOV_DIR)
	genhtml -o $(GCOV_DIR)/report $(GCOV_DIR)/$(LCOV_REPORT)

package: gcov doc all
	rm -rf $(AR_NAME)
	tar cvfz $(AR_NAME) ./*

man:
	apt-get update
	apt-get install groff
	sudo mkdir -p /usr/local/man/man1
	sudo cp ./ShellterMan /usr/local/man/man1/shellter.1
	sudo gzip /usr/local/man/man1/shellter.1
	@echo "You can now use 'man shellter'"

clean:	
	rm -rf $(OBJ2)

mrproper: clean
	rm -rf $(BIN_DIR)/*
	rm -rf $(DOC_DIR)/latex/
	rm -rf $(DOC_DIR)/html/
	rm -rf $(GCOV_DIR)/*

.PHONY: doc
