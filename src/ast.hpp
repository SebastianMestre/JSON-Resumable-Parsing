#pragma once

#include "utils.hpp"

namespace AST {

enum class Type {
	Object, Array, String, Unknown
};

struct AST {
	Type m_type;
	AST() : m_type {Type::Unknown} {}
	AST(Type type) : m_type{ type } {}
	Type type() const { return m_type; }
	virtual ~AST() = default;
};

struct Object : public AST {
	std::unordered_map<std::string, box<AST>> m_elements;
	Object () : AST(Type::Object) {}
};

struct Array : public AST {
	std::vector<box<AST>> m_elements;
	Array () : AST(Type::Array) {}
};

struct String : public AST {
	std::string m_data;
	String () : AST(Type::String) {}
};


inline void print_spaces (int d){
	if(d < 0) d = 0;
	d += d;
	for(int i = d; i--;) std::cout << ' ';
}

inline void print (AST* ast, int d = 0, bool skip_leading = false);

inline void print (String* ast, int d = 0, bool skip_leading = false) {
	if(!skip_leading)
		print_spaces(d);
	std::cout << '"' << ast->m_data << '"';
}

inline void print (Array* ast, int d = 0, bool skip_leading = false) {
	if(!skip_leading)
		print_spaces(d);
	std::cout << '[';
	std::cout << '\n';
	int n = ast->m_elements.size();
	for(int i = 0; i < n; ++i){
		print(ast->m_elements[i].get(), d+1);
		if(i != n-1) {
			std::cout << ",\n";
		}
	}
	std::cout << '\n';
	print_spaces(d);
	std::cout << ']';
}

inline void print (Object* ast, int d = 0, bool skip_leading = false) {
	print_spaces(d);
	std::cout << "{\n";
	int i = 0;
	int n = ast->m_elements.size();
	for(auto& kv : ast->m_elements){
		print_spaces(d+1);
		std::cout << '"' << kv.first << '"' << ':';
		print(kv.second.get(), d+1, true);
		if(i++ != n-1) {
			std::cout << ",\n";
		}
	}
	std::cout << '\n';
	print_spaces(d);
	std::cout << '}';
}

inline void print (AST* ast, int d, bool skip_leading) {
	switch(ast->type()){
		case Type::String: return print(static_cast<String*>(ast), d, skip_leading);
		case Type::Array: return print(static_cast<Array*>(ast), d, skip_leading);
		case Type::Object: return print(static_cast<Object*>(ast), d, skip_leading);
	}
}

} // namespace AST
