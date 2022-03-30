#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"
using namespace std;

StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarArrSize(LOCAL_VARIABLE_ARRAY_SIZE) {}

void Stack::push(float temp, int type, int count) {
	if (this->size >= maxsize - 1) {
		throw StackFull(count);
	}
	arr[size] = temp;
	arr[size + 1] = float(type);
	size += 2;
}
float Stack::top(int count) {
	if (size == 0) {
		throw StackEmpty(count);
	}
	return arr[size - 2];
}

float Stack::top1(int count) {
	if (size == 0) {
		throw StackEmpty(count);
	}
	return arr[size - 1];
}

void calculatorInt(Stack* st, int count, char op) {
	int res;
	int val2 = int(st->top(count));
	int temp = st->top1(count);
	st->pop();
	int val = int(st->top(count));
	if (temp != 0) {
		throw TypeMisMatch(count);
	}
	if (st->top1(count) != 0) {
		throw TypeMisMatch(count);
	}
	st->pop();
	switch (op)
	{
	case '+':
		res = val + val2;
		break;
	case '-':
		res = val - val2;
		break;
	case '*':
		res = val * val2;
		break;
	case '/':
		if (val2 == 0) {
			throw DivideByZero(count);
		}
		res = int(int(val) / int(val2));
		break;
	case '%':
		res = int(val) % int(val2);
		break;
	case '&':
		res = val & val2;
		break;
	case '|':
		res = val | val2;
		break;
	case '=':
		if (val == val2) res = 1;
		else res = 0;
		break;
	case '!':
		if (val != val2) res = 1;
		else res = 0;
		break;
	case '>':
		if (val > val2) res = 1;
		else res = 0;
		break;
	case '<':
		if (val < val2) res = 1;
		else res = 0;
		break;
	default:
		res = -1;
		break;
	}
	st->push(float(res), 0, count);
}

void calculatorFloat(Stack* st, int count, char op) {
	float res;
	float val2 = st->top(count);
	st->pop();
	float val = st->top(count);
	st->pop();
	switch (op)
	{
	case '+':
		res = val + val2;
		break;
	case '-':
		res = val - val2;
		break;
	case '*':
		res = val * val2;
		break;
	case '/':
		res = val / val2;
		if (val2 == 0) {
			throw DivideByZero(count);
		}
		break;
	case '=':
		if (val == val2) res = 1;
		else res = 0;
		break;
	case '!':
		if (val != val2) res = 1;
		else res = 0;
		break;
	case '>':
		if (val > val2) res = 1;
		else res = 0;
		break;
	case '<':
		if (val < val2) res = 1;
		else res = 0;
		break;
	default:
		res = -1;
		break;
	}
	st->push(res, 1, count);
}

void Myclass::running(std::string filename) {
	fstream f;
	f.open(filename, ios::in);
	std::string line;
	int count = 1;
	while (!f.eof()) {
		getline(f, line);
		int pos = line.find_first_of(' ');
		string act = line.substr(0, pos);
		if (act == "iconst") {
			int val = stoi(line.substr(pos + 1, line.size() - pos));
			st->push(float(val), 0, count);
		}
		else if (act == "fconst") {
			float val = stof(line.substr(pos + 1, line.size() - pos));
			st->push(val, 1, count);
		}
		else if (act == "iload") {
			int idx = stoi(line.substr(pos + 1, line.size() - pos));
			if (freq[idx + 1] == 0) {
				throw UndefinedVariable(count);
			}
			if (this->arr[idx] != 0) {
				throw TypeMisMatch(count);
			}
			float val = this->arr[idx + 1];
			st->push(val, 0, count);
		}
		else if (act == "fload") {
			int idx = stoi(line.substr(pos + 1, line.size() - pos));
			if (freq[idx + 1] == 0) {
				throw UndefinedVariable(count);
			}
			if (this->arr[idx] != 1) {
				throw TypeMisMatch(count);
			}
			float val = this->arr[idx + 1];
			st->push(val, 1, count);
		}
		else if (act == "istore") {
			int idx = stoi(line.substr(pos + 1, line.size() - pos));
			if (idx >= this->varmax - 1) {
				throw ArrayOutOfRange(count);
			}
			if (st->top1(count) != 0) {
				throw TypeMisMatch(count);
			}
			this->arr[idx] = 0;
			this->arr[idx + 1] = st->top(count);
			st->pop();
			this->freq[idx + 1] = 1;
		}
		else if (act == "fstore") {
			int idx = stoi(line.substr(pos + 1, line.size() - pos));
			if (idx >= this->varmax - 1) {
				throw ArrayOutOfRange(count);
			}
			if (st->top1(count) != 1) {
				throw TypeMisMatch(count);
			}
			this->arr[idx] = 1;
			this->arr[idx + 1] = st->top(count);
			st->pop();
			this->freq[idx + 1] = 1;
		}
		else if (act == "iadd") {
			calculatorInt(st, count, '+');
		}
		else if (act == "fadd") {
			calculatorFloat(st, count, '+');
		}
		else if (act == "isub") {
			calculatorInt(st, count, '-');
		}
		else if (act == "fsub") {
			calculatorFloat(st, count, '-');
		}
		else if (act == "imul") {
			calculatorInt(st, count, '*');
		}
		else if (act == "idiv") {
			calculatorInt(st, count, '/');
		}
		else if (act == "irem") {
			calculatorInt(st, count, '%');
		}
		else if (act == "ineg") {
			if (st->top1(count) != 0) {
				throw TypeMisMatch(count);
			}
			float temp = st->top(count);
			st->pop();
			st->push(-temp, 0, count);
		}
		else if (act == "iand") {
			calculatorInt(st, count, '&');
		}
		else if (act == "ior") {
			calculatorInt(st, count, '|');
		}
		else if (act == "ieq") {
			calculatorInt(st, count, '=');
		}
		else if (act == "ineq") {
			calculatorInt(st, count, '!');
		}
		else if (act == "ilt") {
			calculatorInt(st, count, '<');
		}
		else if (act == "igt") {
			calculatorInt(st, count, '>');
		}
		else if (act == "ibnot") {
			if (st->top1(count) != 0) {
				throw TypeMisMatch(count);
			}
			float temp = st->top(count);
			st->pop();
			if (temp == 0) {
				st->push(1, 0, count);
			}
			else st->push(0, 0, count);
		}
		else if (act == "fmul") {
			calculatorFloat(st, count, '*');
		}
		else if (act == "fdiv") {
			calculatorFloat(st, count, '/');
		}
		else if (act == "fneg") {
			float temp = st->top(count);
			st->pop();
			st->push(-temp, 1, count);
		}
		else if (act == "feq") {
			calculatorFloat(st, count, '=');
		}
		else if (act == "fneq") {
			calculatorFloat(st, count, '!');
		}
		else if (act == "flt") {
			calculatorFloat(st, count, '<');
		}
		else if (act == "fgt") {
			calculatorFloat(st, count, '>');
		}
		else if (act == "top") {
			cout << st->top(count) << endl;
		}
		else if (act == "val") {
			int idx = stoi(line.substr(pos, line.size() - pos));
			if (idx > varmax || freq[idx + 1] == 0) {
				throw UndefinedVariable(count);
			}
			cout << this->arr[idx + 1] << endl;
		}
		else if (act == "i2f") {
			if (st->top1(count) != 0) {
				throw TypeMisMatch(count);
			}
			st->i2f();
		}
		else if (act == "f2i") {
			if (st->top1(count) != 1) {
				throw TypeMisMatch(count);
			}
			st->f2i();
		}
		else {
		}
		count++;
	}
	f.close();
}

void StackFrame::run(string filename) {
	Myclass temp(this->opStackMaxSize, this->localVarArrSize);
	temp.running(filename);
}