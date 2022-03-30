#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>

class Stack {
public:
    float* arr;
    int size;
    int maxsize;
    Stack(int maxSize) {
        arr = new float[maxSize];
        this->size = 0;
        this->maxsize = maxSize;
    }
    Stack() {
        this->arr = NULL;
        this->size = 0;
        this->maxsize = 0;
    }
    ~Stack() {
        delete[] arr;
    }
    void push(float temp, int type, int count);
    float top(int count);
    float top1(int count);
    void i2f() {
        arr[size - 1] = 1;
    }
    void f2i() {
        arr[size - 2] = int(arr[size - 2]);
        arr[size - 1] = 0;
    }
    void pop() {
        if (size == 0) {

        }
        else {
            this->size -= 2;
        }
    }
};

class Myclass {
public:
    Stack* st;
    float* arr;
    int* freq;
    int opmax;
    int varmax;
    Myclass(int opmax, int varmax) {
        st = new Stack(opmax);
        arr = new float[varmax];
        this->opmax = opmax;
        this->varmax = varmax;
        freq = new int[varmax];
        for (int i = 0; i < varmax; i++) {
            freq[i] = 0;
        }
    }
    void running(std::string filename);
};

/*
StackFrame declaration
*/
class StackFrame {
    int opStackMaxSize; // max size of operand stack
    int localVarArrSize; // size of local variable array
public:
    /*
    Constructor of StackFrame
    */
    StackFrame();
    
    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    void run(std::string filename);
};

#endif // !__STACK_FRAME_H__