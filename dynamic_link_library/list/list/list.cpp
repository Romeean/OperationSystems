#include "pch.h"
#include "framework.h"
#include "list.h"
#include <iostream>

using namespace std;


void NullChecking(int position) {
	if (position < 0) {
		cout << "Nahhh... Try again" << endl;
		return;
	}
	else {}

};

List::List(){
};

List::List(int element) {
	Node* currentElement = new Node{ element, nullptr };
	list.head = currentElement;
};

List::~List(){
	while (list.head != nullptr) {
		Node* currentElement = list.head;
		list.head = currentElement->nextNode;
		delete currentElement;
	}
}

void List::add(int element) {
	Node* currentElement = list.head;
	Node* newElement = new Node{ element, nullptr };
	newElement->nextNode = currentElement;
	list.head = newElement;

};

void List::del(int position) {
	NullChecking(position);
	
	if (position == 0) {
		Node* currentElement = list.head;
		list.head = list.head->nextNode;

		delete currentElement;	
		currentElement = nullptr;
		return;
	}

	if (position == size() - 1) {
		Node* currentElement = list.head;
		int index = 0;

		while (currentElement != nullptr) {

			if (index == position - 1) {
				Node* elementToDelete = currentElement->nextNode;
				currentElement->nextNode = nullptr;

				delete elementToDelete;
				elementToDelete = nullptr;
				return;
			}

			index++;
			currentElement = currentElement->nextNode;
		}
		return;
	}


	int index = 0;
	Node* currentElement = list.head;

	while (currentElement != nullptr) {
		if (index == position - 1) {
			
			Node* elementToDelete = currentElement->nextNode;
			currentElement->nextNode = elementToDelete->nextNode;
			
			delete elementToDelete;
			elementToDelete = nullptr;
			return;
		}

		currentElement = currentElement->nextNode;
		index++;
	}

};

void List::insert(int position, int element) {
	NullChecking(position);
	
	if (position == 0) {
		add(element);
		
		return;
	}


	int index = 0;
	Node* currentElement = list.head;

	while (currentElement != nullptr) {

		if (index == position - 1) {
			Node* newElement = new Node{element , nullptr};
			newElement->nextNode = currentElement->nextNode;
			currentElement->nextNode = newElement;

			return;
		}

		currentElement = currentElement->nextNode;
		index++;
	}
};

int List::size() {
	Node* currentElement = list.head;
	int index = 0;

	while (currentElement != nullptr) {
		index++;
		currentElement = currentElement->nextNode;
	}

	return index;
}