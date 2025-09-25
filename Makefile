CC = gcc 
CFLAGS = #-std=c11
ADDFLAGS = -Wall -Werror -Wextra -O2
LDFLAGS = -lm -lncurses

LIB_SOURCES = src/brick_game/tetris/*.c
GUI_SOURCES = src/gui/cli/*.c
SRC_TEST = src/test_folder/*.c

LIBS = $(shell pkg-config --libs check)
GCOVFLAGS = -fprofile-arcs -ftest-coverage

VERSION = 1.0
NAME = brickgame-tetris
DIST_DIR = $(NAME)-$(VERSION)
DIST_FILES = Makefile dvi/README.md src/test_folder/ 
GCOV_DIR = gcov

.PHONY: all style install uninstall clean dvi dist run style

all: install

install: clean game/libtetris.a
	$(CC) $(ADDFLAGS) $(CFLAGS) $(GUI_SOURCES) game/libtetris.a $(LDFLAGS) -o game/tetris

uninstall: 
	rm -rf game

clean:
	rm -rf gcov tetris.info gcov.out coverage_report
	rm -rf dvi/documentation.pdf 
	rm -rf brickgame-tetris-1.0 brickgame-tetris-1.0.tar.gz 
	rm -rf test


dvi:
	pandoc dvi/README.md -o dvi/documentation.pdf \
	--pdf-engine=xelatex \
	-V mainfont="DejaVu Sans" \
	-V lang=ru \
	-V geometry:margin=2cm
	@echo "Документация создана: documentation.pdf"

dist:
	mkdir -p $(DIST_DIR)
	cp -r $(DIST_FILES) $(DIST_DIR)
	tar -czf $(NAME)-$(VERSION).tar.gz $(DIST_DIR)
	rm -rf $(DIST_DIR)
	@echo "Distribution package created: $(NAME)-$(VERSION).tar.gz"

test: game/libtetris.a
	$(CC) $(ADDFLAGS) $(LDFLAGS) $(SRC_TEST) game/libtetris.a -o test $(LIBS)
	./test

gcov_report: game/libtetris.a
	mkdir -p $(GCOV_DIR)
	$(CC) $(GCOVFLAGS) $(SRC_TEST) game/libtetris.a $(LIB_SOURCES) -o $(GCOV_DIR)/gcov.out $(LIBS)
	./$(GCOV_DIR)/gcov.out
	lcov -t "tetris" -o tetris.info -c -d . -q --rc branch_coverage=0
	lcov -r tetris.info -o tetris.info --rc branch_coverage=0
	genhtml -o coverage_report tetris.info -q --rc branch_coverage=0
	# genhtml coverage.info --output-directory coverage_report
	# open ./coverage_report/index.html

game/libtetris.a:
	mkdir -p game
	gcc $(CFLAGS) $(ADDFLAGS) -c $(LIB_SOURCES)
	ar rcs libtetris.a *.o
	mv libtetris.a game/libtetris.a
	@rm *.o

run: install
	game/tetris

style:
	clang-format -i $(LIB_SOURCES) $(GUI_SOURCES) $(SRC_TEST)
	cppcheck --enable=all --suppress=missingInclude .
	@if [[ "" != `find . \( -name "*.c" -o -name "*.h" \) -exec grep -iL "<.*@.*\..*>" {} +` ]]; then \
		echo "`find . \( -name "*.c" -o -name "*.h" \) -exec grep -iL "<.*@.*\..*>" {} +` Every file should contain license boilerplate" && exit 1; \
	fi
	@if [[ "" != `find . \( -name "*.c" -o -name "*.h" \) -exec grep -l "\b\(u\)\?int\(8\|16\|32\|64\)_t\b" {} +` ]]; then \
		echo "`find . \( -name "*.c" -o -name "*.h" \) -exec grep -il "\b\(u\)\?int\(8\|16\|32\|64\)_t\b" {} +` Verter does not support fixed-width integer types" && exit 1; \
	fi
	@find . -name "*.c" -exec clang-format -style="{BasedOnStyle: Google, InsertBraces: true}" --verbose --Werror --ferror-limit=0 -i {} +
	@clang-tidy -fix -header-filter=.*\.h `find . \( -name "*.c" \) -not -name "*test*"` \
	  --format-style="{BasedOnStyle: Google, InsertBraces: true}" -config="{CheckOptions: [ \
	  {key: readability-identifier-naming.VariableCase, value: lower_case}, \
	  {key: readability-identifier-naming.FunctionCase, value: CamelCase}, \
	  {key: readability-identifier-naming.TypedefCase, value: CamelCase}, \
	  {key: readability-identifier-naming.ConstantCase, value: CamelCase}, \
	  {key: readability-identifier-naming.EnumConstantCase, value: CamelCase}, \
	  {key: readability-identifier-naming.ConstantPrefix, value: k }, \
	  {key: readability-identifier-naming.EnumConstantPrefix, value: k}, \
	  {key: readability-identifier-naming.MacroDefinitionCase, value: UPPER_CASE}, \
	  {key: readability-identifier-naming.NamespaceCase, value: lower_case}, \
	  {key: readability-identifier-naming.ConceptCase, value: CamelCase}, \
	  {key: readability-identifier-naming.MemberCase, value: lower_case}, \
	  {key: readability-identifier-naming.ClassMemberSuffix, value: _}, \
	  {key: readability-identifier-naming.FunctionCase, value: camelBack}, \
	  {key: readability-identifier-naming.ConstantPrefix, value: '' }, \
	  {key: readability-identifier-naming.EnumConstantPrefix, value: '' }, \
	  {key: readability-identifier-naming.TypedefSuffix, value: _t}, \
	  ]}" \
	  -checks="*,-llvmlibc*,-altera*,-android*" \
	  --warnings-as-errors="*,-bugprone-easily-swappable-parameters,-readability-identifier-length" \
	  -- -I"`pwd`"