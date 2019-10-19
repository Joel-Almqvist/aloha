foo2.7: foo.cpp
	g++ foo.cpp -std=c++11 -I/usr/include/python2.7 -lpython2.7 -o foo2.7

foo3.5: foo.cpp
	g++ foo.cpp -std=c++11 -I/usr/include/python3.5 -lpython3.5m -o foo3.5

aloha: iteration.cpp node.cpp aloha.cpp
	g++ -std=c++11 -I/usr/include/python3.5 -lpython3.5m aloha.cpp node.cpp iteration.cpp -o aloha
