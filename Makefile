a.out: pstree.cpp
	gcc pstree.cpp -lstdc++
test:
	@./a.out
