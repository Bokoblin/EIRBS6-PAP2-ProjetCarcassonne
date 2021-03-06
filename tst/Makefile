#PROJECT : EIRBS6-PAP2-ProjetCarcassonne -- Test Makefile

#######################################################
###				CONFIGURATION
#######################################################

TST_DIR = ./
SRC_DIR = ../src
COV_DIR = ./coverage

CLI_DIR = $(SRC_DIR)/client
CLF_DIR = $(CLI_DIR)/common_clients_functions
COM_DIR = $(SRC_DIR)/common
ADT_DIR = $(COM_DIR)/ADT
SRV_DIR = $(SRC_DIR)/server

CC		= gcc
CFLAGS	= -Wall -Wextra -std=c99 -g -O0 -fPIC
LFFLAGS	= -ldl

TESTCOM_SRC = $(wildcard $(TST_DIR)/common_tests_utils.c $(COM_DIR)/utils.c)
TESTCOM_OBJ	= $(TESTCOM_SRC:%.c=%.o)

TESTS_EXEC 	= test_board test_card test_client test_deck test_meeple test_queue test_set test_stack test_zone


#######################################################
###				COMMANDS CATEGORIES
#######################################################

# Accessible commands
.PHONY: all help build test vtest ctest clean

# Helper commands
.PHONY: --prebuild --enable_coverage

#######################################################
###				DEFAULT MAKE COMMAND
#######################################################

all: test


#######################################################
###				MAKE INSTRUCTIONS / HELP
#######################################################

help:
	@echo -e Available commands:'\n' \
		'\t' make help	'\t' Show the availables commands				'\n' \
		'\t' make build	'\t' Build all tests sources					'\n' \
		'\t' make test	'\t' Execute all the tests						'\n' \
		'\t' make vtest	'\t' Execute all the tests with memory check 	'\n' \
		'\t' make ctest	'\t' Execute all the tests with a coverage		'\n' \
		'\t' make clean '\t' Clean all generated objects and binairies	'\n'


#######################################################
###				MAKE BUILD ALL TESTS
#######################################################

--prebuild:
	@echo Starting building tests...

build: --prebuild $(TESTS_EXEC)


#######################################################
###				MAKE TEST
#######################################################

test: build
ifneq ($(TESTS_EXEC),)
	@echo Starting tests...
	@for e in $(TESTS_EXEC); do \
		./$${e}; echo; \
	done
	@printf "\nTests complete.\n";
else
	@echo No test available
endif


#######################################################
###				MAKE TEST WITH VALGRIND
#######################################################

vtest: build
ifneq ($(TESTS_EXEC),)
	@echo Starting tests...
	@for e in $(TESTS_EXEC); do \
		echo ======= $${e} =======; \
		filename=$$(echo $(TST_DIR)/$${e} | cut -d_ -f2); \
		printf "TESTED FILE:\t$$filename.c\n"; \
		valgrind --log-fd=1 ./$${e} \
		| grep "TESTS SUMMARY:\|ERROR SUMMARY:\|total heap usage:" \
		| $(VALGRIND_AWK) \
	done
	@printf "\nTests complete.\n";
else
	@echo No test available
endif

#######################################################
###				MAKE TEST WITH COVERAGE
#######################################################

--enable_coverage:
	@echo Enabling coverage...
	@find .. -type f -name '*.o' -delete
	$(eval CFLAGS += --coverage)
	$(eval LFFLAGS += -lgcov)

ctest: --enable_coverage build
ifneq ($(TESTS_EXEC),)
	@mkdir -p $(COV_DIR)
	@echo Starting tests with coverage...
	@for e in $(TESTS_EXEC); do \
		filename=$$(echo $(TST_DIR)/$${e} | cut -d_ -f2); \
		echo ======= $${e} =======; \
		printf "TESTED FILE:\t$$filename.c\n"; \
		./$${e} | grep "TESTS SUMMARY:"; \
		printf "COVERAGE:\t"; \
		gcov $$(find .. -name $$filename.o) 2>/dev/null | grep "Lines" | cut -f 2 -d ':'; echo;\
		mv -f $$filename.c.gcov -t $(COV_DIR)/ 2>/dev/null; \
	done
	@find .. -type f -name '*.o' -delete
	@printf "\nTests complete.\n";
else
	@echo No test available
endif


#######################################################
###				MAKE CLEAN
#######################################################

clean:
	@echo Starting tests cleanup...
	@find .. -type f -name '*.o' -delete
	@find .. -type f -name '*.gcda' -delete
	@find .. -type f -name '*.gcno' -delete
	@find .. -type f -name '*.gcov' -delete
	@rm -rf $(COV_DIR)
	@rm -rf $(TESTS_EXEC)
	@echo Tests cleanup complete.


#######################################################
###				TEST EXECUTABLES
#######################################################

test_board:	$(TESTCOM_OBJ) $(TST_DIR)/test_board.o \
			$(SRV_DIR)/board.o $(COM_DIR)/card.o $(COM_DIR)/meeple.o $(COM_DIR)/deck.o \
			$(COM_DIR)/interface.o $(ADT_DIR)/set.o $(ADT_DIR)/stack.o $(ADT_DIR)/queue.o
	${CC} $^ -o $@ $(LFFLAGS)

test_card: 	$(TESTCOM_OBJ) $(TST_DIR)/test_card.o \
			$(COM_DIR)/card.o $(COM_DIR)/interface.o $(ADT_DIR)/stack.o
	${CC} $^ -o $@ $(LFFLAGS)

test_client:$(TESTCOM_OBJ) $(TST_DIR)/test_client.o \
			$(COM_DIR)/card.o $(COM_DIR)/interface.o $(CLF_DIR)/micro_board.o \
			$(CLF_DIR)/client.o $(ADT_DIR)/set.o
	${CC} $^ -o $@ $(LFFLAGS)

test_deck: 	$(TESTCOM_OBJ) $(TST_DIR)/test_deck.o \
			$(COM_DIR)/card.o $(COM_DIR)/deck.o $(COM_DIR)/interface.o $(ADT_DIR)/stack.o
	${CC} $^ -o $@ $(LFFLAGS)

test_meeple:$(TESTCOM_OBJ) $(TST_DIR)/test_meeple.o \
			$(COM_DIR)/meeple.o $(COM_DIR)/card.o $(COM_DIR)/interface.o $(ADT_DIR)/stack.o
	${CC} $^ -o $@ $(LFFLAGS)

test_queue:	$(TESTCOM_OBJ) $(TST_DIR)/test_queue.o $(ADT_DIR)/queue.o
	${CC} $^ -o $@ $(LFFLAGS)

test_set:	$(TESTCOM_OBJ) $(TST_DIR)/test_set.o $(ADT_DIR)/set.o
	${CC} $^ -o $@ $(LFFLAGS)

test_stack:	$(TESTCOM_OBJ) $(TST_DIR)/test_stack.o $(ADT_DIR)/stack.o
	${CC} $^ -o $@ $(LFFLAGS)

test_zone: 	$(TESTCOM_OBJ) $(TST_DIR)/test_zone.o \
			$(COM_DIR)/area_type.o $(COM_DIR)/card.o $(COM_DIR)/meeple.o $(COM_DIR)/zone.o \
			$(COM_DIR)/interface.o $(ADT_DIR)/set.o
	${CC} $^ -o $@ $(LFFLAGS)


#######################################################
###				OBJECTS FILES
#######################################################

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@


#######################################################
###				VALGRIND FORMATTING
#######################################################

VALGRIND_AWK = \
awk '{	\
	if (match( $$0, /TESTS.*/)) \
		printf "%s\n", $$0; \
	else \
		for(i=2;i<=NF;i++) { \
			if (match($$((i+1)), /allocs/) && $$i > $$((i+2))) \
				   printf "\x1B[31m%s \x1b[0m", $$i; \
			else if (match($$i, /[1-9]+$$/) && match($$((i+1)), /errors/)) \
				   printf "\x1B[31m%s \x1b[0m", $$i; \
			else if (match($$i, /[1-9]+$$/) && match($$((i+1)), /contexts/)) \
				   printf "\x1B[31m%s \x1b[0m", $$i; \
			else if (match($$i, /[0-9]+$$/)) \
				printf "\x1B[32m%s \x1b[0m", $$i; \
		   	else if (match($$i, /ERROR/)) \
				printf "\n%s ", $$i; \
			else if (match($$i, /total/)) \
				printf ""; \
			else if (match($$i, /heap/)) \
				printf "HEAP "; \
			else if (match($$i, /usage:/)) \
				printf "USAGE: \t"; \
			else if (match($$i, /frees\,/)) \
				{printf "frees", $$i; break;}\
			else if (match($$i, /contexts/)) \
				{printf "%s", $$i; break;}\
			else \
				printf "%s ", $$i; \
			} \
	}'; \
echo; echo;

