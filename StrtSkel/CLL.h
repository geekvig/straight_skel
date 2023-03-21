#pragma once

#ifndef CLL_H
#define CLL_H

#include "assert.h"
#include <iostream>
#include "CllNode.h"
#include "definitions.h"

template <typename T>
class CLL {
private:
	T* head;
	T* tail;
public:

	CLL () 
	{
		head = nullptr;
	}

	void insert_after(T* node, T* newNode);
	void insert_before(T* Node, T* newNode);
	void push_back(T* node);
	void push_front(T* node);
	void erase(T* node);

	int Size() {
		return m_size;
	}

	T* begin() const { return head;  }

public:
	size_t m_size = 0;
};

template <typename T>
void CLL<T>::insert_after(T* node, T* newNode) {
	assert(node != nullptr);
	newNode->prev = node;
	newNode->next = node->next;
	node->next->prev = newNode;
	node->next = newNode;
	if (tail == node) tail = newNode;
	m_size++;
}

template <typename T>
void CLL<T>::insert_before(T* node, T* newNode) {
	assert(node != nullptr);
	newNode->prev = node->prev;
	newNode->next = node;
	node->prev->next = newNode;
	node->prev = newNode;
	if (head == node) head = newNode;
	m_size++;
}

template <typename T>
void CLL<T>::push_back(T* node) {
	if (this->head == nullptr) {
		head = node;
		tail = node;
		node->next = node;
		node->prev = node;
		++m_size;
		return;
	}
	this->insert_after(tail, node);
}

template <typename T>
void CLL<T>::push_front(T* node) {
	if (this->head == nullptr) {
		head = node;
		tail = node;
		node->next = node;
		node->prev = node;
		++m_size;
		return;
	}
	this->insert_before(head, node);
}

template <typename T>
void CLL<T>::erase(T* node) {
	if (m_size == 1) {
		m_size = 0;
		head = nullptr;
		return;
	}
	if (head == node) head = node->next;
	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->prev = nullptr;
	node->next = nullptr;
	m_size--;
}



#endif