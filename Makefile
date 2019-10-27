aloha: iteration.cpp node.cpp aloha.cpp
	g++ aloha.cpp node.cpp iteration.cpp -std=c++11 -I/usr/include/python2.7 -lpython2.7 -o aloha
