MAKEFLAGS += -L

CXX      = clang++ 
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 

gerp: main.o gerp.o hashTable.o FSTree.o DirNode.o stringProcessing.o
	${CXX} ${CXXFLAGS} -O2 -o gerp main.cpp gerp.o hashTable.o FSTree.o DirNode.o stringProcessing.o

main.o: main.cpp gerp.cpp
	${CXX} ${LDFLAGS} -O2 -o main.o main.cpp -c

gerp.o: gerp.cpp gerp.h hashTable.h FSTree.h DirNode.h stringProcessing.h
	${CXX} ${CXXFLAGS} -O2 -c gerp.cpp

hashTable.o: hashTable.cpp hashTable.h
	${CXX} ${CXXFLAGS} -O2 -c hashTable.cpp

stringProcessing.o: stringProcessing.cpp stringProcessing.h
	${CXX} ${CXXFLAGS} -O2 -c stringProcessing.cpp 

# unit_test: unit_test_driver.o stringProcessing.o hashTable.o gerp.o FSTree.h DirNode.h
# 	${CXX} ${LDFLAGS} -O2 $^

# treeTraversal: FSTreeTraversal.o FSTree.o DirNode.o
# 	${CXX} ${CXXFLAGS} -o treeTraversal FSTreeTraversal.o FSTree.o DirNode.o

# FSTreeTraversal.o: FSTreeTraversal.cpp FSTree.h DirNode.h 
# 	${CXX} ${LDFLAGS} -c FSTreeTraversal.cpp
