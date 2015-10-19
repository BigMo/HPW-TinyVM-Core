#pragma once
#include "stdafx.h"

#define TFrame StackFrame<T>	//Lazy
#define TStack Stack<T>			//Two characters saved

template <class T>
struct StackFrame
{
	T Value;
	TFrame* PreviousFrame = nullptr;

	//Initalizes a new frame, sets its value and precessing frame
	StackFrame<T>(T value, TFrame* previousFrame) : Value(value), PreviousFrame(previousFrame)
	{ }
};

template <class T>
class Stack
{
public:
	Stack();
	~Stack();
	void Push(T value);
	T Pop();
	int GetFrameCount();
	T *Dump();
private:
	void DumpFrames(T* buffer);
	int iFrameCount;
	TFrame *top;
};

template <class T>
TStack::Stack() : iFrameCount(0), top(nullptr)
{ }

template <class T>
TStack::~Stack()
{
	//Pop until there are no more frames left
	while (iFrameCount > 0)
		this->Pop();
}

//Initialize new frame and put it on top
template <class T>
void TStack::Push(T value)
{
	TFrame *frame = new TFrame(value, this->top);
	this->top = frame;
	this->iFrameCount++;
}

//Get the top-most frame and return its value
template <class T>
T TStack::Pop()
{
	//If the top-element is not null
	if (this->top != nullptr)
	{
		//Get frame and value
		TFrame *tmp = this->top;
		T ret = tmp->Value;
		//Set current top to the previous frame
		this->top = tmp->PreviousFrame;
		//Free memory and return value
		delete tmp;
		this->iFrameCount--;
		return ret;
	}

	//Return default-value
	return T();
}

template<class T>
inline int Stack<T>::GetFrameCount()
{
	return this->iFrameCount;
}

template<class T>
inline T * Stack<T>::Dump()
{
	if (this->iFrameCount == 0)
		return nullptr;

	T* buffer = new T[this->iFrameCount];
	this->DumpFrames(buffer + ((this->iFrameCount - 1) * sizeof(T)));
}

template<class T>
void Stack<T>::DumpFrames(T* buffer)
{
	T value = this->Pop();
	*buffer = value;
	if (this->top != nullptr)
		DumpFrames(buffer - sizeof(T));
	this->Push(value);
}