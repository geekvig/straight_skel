#pragma once
#ifndef CLLNODE_H
#define CLLNODE_H

// CllNode<Vertex> 

template <typename T>
struct CllNode {
	T* v;
	CllNode<T>* next = nullptr;
	CllNode<T>* prev = nullptr;;
	CllNode<T>(T* val) : v(val) {}
};

#endif

