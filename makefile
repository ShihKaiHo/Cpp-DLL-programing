.PHONY: clean
DLL: DLL.cpp
	g++ -rdynamic -o DLL DLL.cpp -ldl
clean:
	rm -f DLL *.so