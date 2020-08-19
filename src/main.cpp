#include <iostream>
#include <string>

#include "parser.hpp"

int main () {
	Parser p;
	int remembered_size = p.top_level.size();
	std::string line;
	while(std::getline(std::cin, line)){
		for(char c : line)
			p.feed(c);
		int current_size = p.top_level.size();
		while(current_size != remembered_size){
			AST::print(p.top_level[remembered_size++].get());
		std::cout << '\n';
		}
	}
}
