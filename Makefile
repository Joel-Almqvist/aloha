foo: foo.cpp
	g++ foo.cpp -std=c++11 -I/usr/include/python3.5 -lpython3.5m

aloha: iteration.cpp node.cpp aloha.cpp
	g++ -std=c++11 -I/usr/include/python3.5 -lpython3.5m aloha.cpp node.cpp iteration.cpp -o aloha
