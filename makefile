.DEFAULT_GOAL := test

FILES :=                              \
        Netflix.c++                   \
        Netflix.h                     \
        RunNetflix.c++                \
        RunNetflix.in                 \
        RunNetflix.out                \
        TestNetflix.c++               \
        TestNetflix.out               \


ifeq ($(shell uname), Darwin)                                        # Apple
    CXX          := g++
    INCLUDE      := /usr/local/include
    CXXFLAGS     := -pedantic -std=c++11 -I$(INCLUDE) -Wall -Weffc++
    LIB          := /usr/local/lib
    LDFLAGS      := -lgtest_main
    CLANG-CHECK  := clang-check
    GCOV         := gcov
    GCOVFLAGS    := -fprofile-arcs -ftest-coverage
    VALGRIND     := valgrind
    DOXYGEN      := doxygen
    CLANG-FORMAT := clang-format
else ifeq ($(CI), true)                                              # Travis CI
    CXX          := g++-5
    INCLUDE      := /usr/include
    CXXFLAGS     := -pedantic -std=c++11 -Wall -Weffc++
    LIB          := /usr/lib
    LDFLAGS      := -lgtest -lgtest_main -pthread
    CLANG-CHECK  := clang-check
    GCOV         := gcov-5
    GCOVFLAGS    := -fprofile-arcs -ftest-coverage
    VALGRIND     := valgrind
    DOXYGEN      := doxygen
    CLANG-FORMAT := clang-format
    BOOST        := -lboost_serialization
else ifeq ($(shell uname -p), unknown)                               # Docker
    CXX          := g++
    INCLUDE      := /usr/include
    CXXFLAGS     := -pedantic -std=c++11 -Wall -Weffc++
    LIB          := /usr/lib
    LDFLAGS      := -lgtest -lgtest_main -pthread
    CLANG-CHECK  := clang-check
    GCOV         := gcov
    GCOVFLAGS    := -fprofile-arcs -ftest-coverage
    VALGRIND     := valgrind
    DOXYGEN      := doxygen
    CLANG-FORMAT := clang-format-3.5
    BOOST        := -lboost_serialization
else                                                                 # UTCS
    CXX          := g++-4.8
    INCLUDE      := /usr/include
    CXXFLAGS     := -pedantic -std=c++11 -Wall -Weffc++
    LIB          := /usr/lib
    LDFLAGS      := -lgtest -lgtest_main -pthread
    CLANG-CHECK  := clang-check-3.8
    GCOV         := gcov-4.8
    GCOVFLAGS    := -fprofile-arcs -ftest-coverage
    VALGRIND     := valgrind
    DOXYGEN      := doxygen
    CLANG-FORMAT := clang-format-3.8
    BOOST        := -lboost_serialization
endif

html: Doxyfile Netflix.h Netflix.c++ RunNetflix.c++ TestNetflix.c++
	doxygen Doxyfile

GetFile:
	wget http://www.cs.utexas.edu/users/downing/netflix-cs371p/cyt276-as63439-avg-movie-ratings.txt
	wget http://www.cs.utexas.edu/users/downing/netflix-cs371p/cyt276-as63439-avg-customer-ratings.txt
	wget http://www.cs.utexas.edu/users/downing/netflix-cs371p/az6257-actual-user-ratings.txt
	wget http://www.cs.utexas.edu/users/downing/netflix/probe.txt

Netflix.log:
	git log > Netflix.log

Doxyfile:
	doxygen -g

RunNetflix: Netflix.h Netflix.c++ Netflix.c++
	$(CXX) $(CXXFLAGS) Netflix.c++ RunNetflix.c++ -o RunNetflix 
	-$(CLANG-CHECK) -extra-arg=-std=c++11          Netflix.c++     --
	-$(CLANG-CHECK) -extra-arg=-std=c++11 -analyze Netflix.c++     --
	-$(CLANG-CHECK) -extra-arg=-std=c++11          RunNetflix.c++  --
	-$(CLANG-CHECK) -extra-arg=-std=c++11 -analyze RunNetflix.c++  --

RunNetflix.tmp: RunNetflix
	#./RunNetflix < probe.txt > RunNetflix.tmp
	./RunNetflix < RunNetflix.in > RunNetflix.tmp
	#diff RunNetflix.tmp RunNetflix.out

TestNetflix: Netflix.h Netflix.c++ TestNetflix.c++
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) Netflix.c++ TestNetflix.c++ -o TestNetflix $(LDFLAGS)
	-$(CLANG-CHECK) -extra-arg=-std=c++11 -analyze TestNetflix.c++ --

TestNetflix.tmp: TestNetflix
	$(VALGRIND) ./TestNetflix                               >  TestNetflix.tmp 2>&1
	$(GCOV) -b Netflix.c++ | grep -A 5 "File 'Netflix.c++'" >> TestNetflix.tmp
	cat TestNetflix.tmp

check:
	@not_found=0;                                 \
    for i in $(FILES);                            \
    do                                            \
        if [ -e $$i ];                            \
        then                                      \
            echo "$$i found";                     \
        else                                      \
            echo "$$i NOT FOUND";                 \
            not_found=`expr "$$not_found" + "1"`; \
        fi                                        \
    done;                                         \
    if [ $$not_found -ne 0 ];                     \
    then                                          \
        echo "$$not_found failures";              \
        exit 1;                                   \
    fi;                                           \
    echo "success";

clean:
	rm -f  *.db
	rm -f  *.gcda
	rm -f  *.gcno
	rm -f  *.gcov
	rm -f  *.plist
	rm -f  Netflix.log
	rm -f  Doxyfile
	rm -f  RunNetflix
	rm -f  RunNetflix.tmp
	rm -f  TestNetflix
	rm -f  TestNetflix.tmp
	rm -rf *.dSYM
	rm -rf html
	rm -rf latex
	rm -f  cyt276-as63439-avg-movie-ratings.txt
	rm -f  cyt276-as63439-avg-customer-ratings.txt
	rm -f  az6257-actual-user-ratings.txt
	rm -f  probe.txt

config:
	git config -l

format:
	clang-format -i Netflix.c++
	clang-format -i Netflix.h
	clang-format -i RunNetflix.c++
	clang-format -i TestNetflix.c++

status:
	make clean
	@echo
	git branch
	git remote -v
	git status

test: html Netflix.log GetFile RunNetflix.tmp TestNetflix.tmp check

travis: html Netflix.log GetFile TestNetflix.tmp check

versions:
	which make
	make --version
	@echo
	which git
	git --version
	@echo
	which $(CXX)
	$(CXX) --version
	@echo
	ls -ald $(INCLUDE)/boost
	@echo
	ls -ald $(INCLUDE)/gtest
	@echo
	ls -al $(LIB)/*gtest*
	@echo
	which $(CLANG-CHECK)
	$(CLANG-CHECK) --version
	@echo
	which $(GCOV)
	$(GCOV) --version
	@echo
	which $(VALGRIND)
	$(VALGRIND) --version
	@echo
	which $(DOXYGEN)
	$(DOXYGEN) --version
	@echo
	which $(CLANG-FORMAT)
	$(CLANG-FORMAT) --version

