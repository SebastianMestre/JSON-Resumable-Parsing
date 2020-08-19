#pragma once

#include <memory>
#include <vector>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>

#include "utils.hpp"
#include "ast.hpp"

enum class StateMode {
	ObjectKey, ObjectAssign, ObjectValue, ObjectNext,
	ArrayValue, ArrayNext,
	String
};

struct State{
	StateMode m_mode;
	box<AST::AST> m_node;
	box<AST::AST> m_aux;
};

struct Parser {
	std::vector<State> m_stack;
	std::vector<box<AST::AST>> top_level;

	void do_return () {
		box<AST::AST> result = std::move(m_stack.back().m_node);

		m_stack.pop_back();

		if(m_stack.empty()) {
			top_level.push_back(std::move(result));
			return;
		}

		StateMode mode = m_stack.back().m_mode;

		if(mode == StateMode::String) {
			assert(0); // we should never recurse from this state
		} else if(mode == StateMode::ArrayNext) {
			assert(0); // we should never recurse from this state
		} else if(mode == StateMode::ArrayValue) {
			// TODO: make sure this is safe
			auto arr = static_cast<AST::Array*>(m_stack.back().m_node.get());
			arr->m_elements.push_back(std::move(result));
			m_stack.back().m_mode = StateMode::ArrayNext;
		} else if(mode == StateMode::ObjectKey){
			m_stack.back().m_aux = std::move(result);
			m_stack.back().m_mode = StateMode::ObjectAssign;
		} else if(mode == StateMode::ObjectAssign){
			assert(0); // we should never recurse from this state
		} else if(mode == StateMode::ObjectValue){
			auto obj = static_cast<AST::Object*>(m_stack.back().m_node.get());
			auto key = static_cast<AST::String*>(m_stack.back().m_aux.get());
			obj->m_elements[std::move(key->m_data)] = std::move(result);
			m_stack.back().m_aux = nullptr;
			m_stack.back().m_mode = StateMode::ObjectNext;
		} else if(mode == StateMode::ObjectNext){
			assert(0); // we should never recurse from this state
		}
	}

	void start_string(char c){
		if(c == '"'){
			m_stack.push_back(State{StateMode::String, std::make_unique<AST::String>()});
		} else {
			std::stringstream ss;
			ss << "Parser: bad initial character: " << int(c) << "\n";
			throw new std::range_error(ss.str());
		}
	}

	void start_new(char c){
		if(c == '"'){
			m_stack.push_back(State{StateMode::String, std::make_unique<AST::String>()});
		} else if (c == '[') {
			m_stack.push_back(State{StateMode::ArrayValue, std::make_unique<AST::Array>()});
		} else if (c == '{') {
			m_stack.push_back(State{StateMode::ObjectKey, std::make_unique<AST::Object>()});
		} else {
			std::stringstream ss;
			ss << "Parser: bad initial character: " << int(c) << "\n";
			throw new std::range_error(ss.str());
		}
	}

	void resume(char c){
		StateMode mode = m_stack.back().m_mode;
		if(mode == StateMode::String){
			if(c == '"'){
				do_return();
			} else {
				auto str = static_cast<AST::String*>(m_stack.back().m_node.get());
				assert(str);
				str->m_data.push_back(c);
				return;
			}
		} else if(mode == StateMode::ArrayValue){
			start_new(c);
		} else if(mode == StateMode::ArrayNext){
			if(c == ']'){
				do_return();
			} else if(c == ','){
				m_stack.back().m_mode = StateMode::ArrayValue;
			} else {
				assert(0); // TODO nice error
			}
		} else if(mode == StateMode::ObjectKey){
			start_string(c);
		} else if(mode == StateMode::ObjectAssign){
			if(c == ':'){
				m_stack.back().m_mode = StateMode::ObjectValue;
			} else {
				assert(0); // TODO nice error
			}
		} else if(mode == StateMode::ObjectValue){
			start_new(c);
		} else if(mode == StateMode::ObjectNext){
			if(c == '}') {
				do_return();
			} else if(c == ','){
				m_stack.back().m_mode = StateMode::ObjectKey;
			} else {
				assert(0); // TODO nice error
			}
		}
	}

	void feed(char c) {
		if(m_stack.empty()) start_new(c);
		else                resume(c);
	}
};

