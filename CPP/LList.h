/* ==================================================
LinkedList
Shikuan Huang

Doubly Linked List
Queue (composition and inheritance implementations)
Stack (composition and inheirtance implementations)
================================================== */

#include <iostream>
using namespace std;

/* ==================================================
Doubly Linked List
================================================== */
template <typename T>
class LList {
	
template <typename S>
friend class Queue;

template <typename S>
friend class Stack;

protected:
	T value;
	int numberOfElements;
	LList<T> * head;
	LList<T> * tail;
	
	LList<T> * prev;
	LList<T> * next;
	
	// Single argument constructor used by insert
	LList(T arg)
	{
		value = arg;
		numberOfElements = 1;
		head = NULL;
		tail = NULL;
		prev = NULL;
		next = NULL;
	}
	
public:
	LList() {
		value = T();
		numberOfElements = 0;
		head = NULL;
		tail = NULL;
		prev = NULL;
		// !!! Important - used by destructor to properly deallocate
		next = head;			
	}
	
	LList(const LList<T> & arg) {
		copyHelper(arg);
	}

	LList<T> & operator=(const LList<T> & arg) {
		if (&arg == this) {
			return *this;
		}
		
		this->~LList();
	
		copyHelper(arg);
		return *this;
	}
	
	// Performs the cloning for the copy constructor and assignment operator
	void copyHelper(const LList<T> & arg) {
		numberOfElements = arg.numberOfElements;
		value = T();
		head = NULL;
		tail = NULL;
		prev = NULL;
		// !!! Important - used by destructor to properly deallocate
		next = head;				
		
		if (numberOfElements == 0) {
			return;
		}
		
		LList<T> * from = arg.head;
		LList<T> * to = head;
		// allocate head node
		to = new LList<T>;			
		// the default constructor sets "next" to "head"
		to->next = NULL;			
		
		for (int i = 0; i < numberOfElements; i++) {	
			to->value = from->value;
			to->numberOfElements = from->numberOfElements;
			
			// set head
			if (i == 0) {
				head = to;
			}
			// set tail
			else if (i == numberOfElements - 1) {
				tail = to;			
			}
			// if not the last element, then allocate another node
			if (i != numberOfElements - 1) {
				to->next = new LList<T>();
				to->next->prev = to;
				to->next->next = NULL;
			}
			
			from = from->next;
			to = to->next;
		}
	}
	
	virtual ~LList() {
		// Do not delete prev since that object has been handled already
		delete next;
		next = NULL;
	}
	
	// Output the elements in the list
	virtual void print() const{
		if (numberOfElements < 1) {
			return;
		}
		
		LList<T> * position = head;
		for (int i = 0; i < numberOfElements; i++) {
			cout << position->value << " ";
			position = position->next;
		}
	}
	
	// Insert an item at position "pos"
	virtual bool insert(T arg, int pos) {
		// Invalid insert positions
		if (pos < 0 || pos > numberOfElements) {
			return false;
		}
		
		LList<T> * newItem = new LList<T>(arg);
		
		// If the list is empty
		if (empty()) {
			head = newItem;
			tail = head;
			numberOfElements++;
			return true;	
		}
		
		// If inserting at the tail
		if (pos == numberOfElements) {
			newItem->prev = tail;
			tail->next = newItem;
			numberOfElements++;
			tail = newItem;
			return true;	
		}
		
		// Move to insert position
		LList<T> * insertPosition = head;
		for (int i = 0; i < pos; i++) {
			insertPosition = insertPosition->next;	
		}
		
		newItem->prev = insertPosition->prev;
		newItem->next = insertPosition;
		
		// NOT inserting at the head, then there is a prev
		if (pos != 0) {
			insertPosition->prev->next = newItem;
		}
	
		insertPosition->prev = newItem;		
		
		if (pos == 0) {
			head = newItem;
		}
		
		if (pos == numberOfElements) {
			tail = newItem;
		}
		
		numberOfElements++;
		return true;
	}
	
	// Insert the provided argument in the back of the list by default
	virtual bool insert(T arg) {
		insert(arg, numberOfElements);
	}
	
	// Remove the item from position "pos"; remove from the front of the list by default
	virtual bool remove(int pos = 0) {
		if (pos < 0 || pos >= numberOfElements) {
			return false;
		}
		
		LList<T> * temp = head;
		for (int i = 0; i < pos; i++) {
			temp = temp->next;
		}
		
		// Removing the head
		if (pos == 0) {
			// head is the only element
			if (numberOfElements == 1) {
				head = head->next;
				tail = NULL;
			}
			// there are more elements following head
			else {
				head->next->prev = NULL;
				head = head->next;
				temp->next = NULL;	
			}
			
			numberOfElements--;
			
			delete temp;
			return true;
		}
		
		// Removing the tail
		if (pos == numberOfElements - 1) {
			tail->prev->next = NULL;
			tail = tail->prev;
			temp->prev = NULL;
			
			numberOfElements--;
			
			delete temp;
			return true;	
		}
		
		// Removing from the middle
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		
		numberOfElements--;
		
		temp->prev = temp->next = NULL;
		delete temp;
		
		return true;
	}
	
	// Insert an item int the front of the list
	virtual bool push_front(T arg) {
		return insert(arg, 0);
	}
	
	// Insert an item in the back of the list
	virtual bool push_back(T arg) {
		return insert(arg, numberOfElements);
	}
	
	// Remove the item at the front of the list
	virtual bool pop_front() {
		return remove(0);
	}
	
	// Remove the item at the back of the list
	virtual bool pop_back() {
		return remove(numberOfElements - 1);
	}
	
	// Return the item at the front of the list
	virtual T front() const {
		return at(0);
	}
	
	// Return the item at the back of the list
	virtual T back() const {
		return at(numberOfElements - 1);
	}
	
	// Returns the element at specified index or throws the invalid index
	virtual T at(int index) const {
		if (index >= numberOfElements || index < 0) {
			throw index;
		}
			
		LList<T> * temp = head;
		for (int i = 0; i < index; i++) {
			temp = temp->next;
		}
		
		return temp->value;
	}
	
	// Determines whether the specified item is in the list
	virtual bool contains(T arg) const {
		if (numberOfElements == 0) {
			return false;
		}
			
		LList<T> * temp = head;
		while (temp != NULL && temp->value != arg) {
			temp = temp->next;
		}
			
		if (temp == NULL) {
			return false;
		}
		return true;
	}
	
	// Removes all the elements in the list
	virtual void eraseAll() {
		while (!empty()) {
			remove(numberOfElements - 1);
		}
	}
	
	// Returns the length of the list
	virtual int length() const {
		return numberOfElements;	
	}
	
	// Determines whether the list is empty
	virtual bool empty() const {
		return (numberOfElements == 0);
	}
	
	// Returns the element at index "index"
	virtual T & operator[](int index) {
		if (index >= numberOfElements || index < 0) {
			throw index;
		}
			
		LList<T> * temp = head;
		for (int i = 0; i < index; i++) {
			temp = temp->next;
		}
		
		return temp->value;
	}
	
	// Returns the element at index "index"
	virtual T operator[](int index) const {
		if (index >= numberOfElements || index < 0) {
			throw index;
		}
			
		LList<T> * temp = head;
		for (int i = 0; i < index; i++) {
			temp = temp->next;
		}
		
		return temp->value;
	}
	
	// Returns true when all the elements are the same in both lists
	bool operator==(const LList<T> & arg) const {
		if (numberOfElements != arg.numberOfElements) {
			return false;
		}
		
		for (int i = 0; i < numberOfElements; i++) {
			if ((*this)[i] != arg[i]) {
				return false;	
			}
		}
		return true;
	}
	
	// Returns true if list has fewer elements than argument
	bool operator<(const LList<T> & arg) const {
		return numberOfElements < arg.numberOfElements;
	}
	
	// Returns true if list has more elements than argument
	bool operator>(const LList<T> & arg) const {
		return numberOfElements > arg.numberOfElements;
	}
};

template <typename T>
ostream & operator<<(ostream & output, const LList<T> & arg) {
	arg.print();
	return output;	
}



/* ==================================================
Queue
================================================== */
template <typename T>
class Queue {
private:
	LList<T> core;
	
public:
	Queue(){}
	
	Queue(const Queue<T> & arg) {
		core = arg.core;	
	}
	
	Queue<T> & operator=(const Queue<T> & arg) {
		core = arg.core;	
	}
	
	~Queue(){}
	
	// Insert an item in the back of the queue
	bool enqueue(T arg) {
		return core.push_back(arg);	
	}
	
	// Remove an item from the front of the queue
	T dequeue() {
		T result = core.front();
		core.pop_front();
		return result;	
	}
	
	// Return the item in the front of the queue
	T front() const {
		return core.front();
	}
	
	// Return the item in the back of the queue
	T back() const {
		return core.back();
	}
	
	// Return the length of the queue
	int length() const {
		return core.length();	
	}
	
	// Returns the length of the queue
	int size() const {
		return core.length();
	}
	
	// Determines whether the queue is empty
	bool empty() const {
		return core.empty();	
	}
	
	// Output the items in the queue
	void print() const {
		core.print();	
	}
	
	T operator[](int index) const {
		return core[index];
	}
	
	// Returns true when all the elements are the same in both queues
	bool operator==(const Queue<T> & arg) const {
		if (core.length() != arg.size()) {
			return false;
		}
		
		for (int i = 0; i < core.length(); i++) {
			if (core[i] != arg[i]) {
				return false;	
			}
		}
		return true;
	}
	
	// Returns true if queue has fewer elements than argument
	bool operator<(const Queue<T> & arg) const {
		return core.length() < arg.size();
	}
	
	// Returns true if queue has more elements than argument
	bool operator>(const Queue<T> & arg) const {
		return core.length() > arg.size();
	}
};

template <typename T>
ostream & operator<<(ostream & output, const Queue<T> & arg) {
	arg.print();
	return output;	
}



/* ==================================================
Stack
================================================== */
template <typename T>
class Stack {
private:
	LList<T> core;
	
public:
	Stack(){}
	
	Stack(const Stack<T> & arg) {
		core = arg.core;	
	}
	
	const Stack<T> & operator=(const Stack<T> & arg) {
		core = arg.core;	
	}
	
	~Stack(){}
	
	// Insert an item at the "top" of the stack
	bool push(T arg) {
		return core.push_back(arg);	
	}
	
	// Remove the item at the "top" of the stack
	T pop() {
		T result = core.back();
		core.pop_back();
		return result;	
	}
	
	// Return the item at the "top" of the stack
	T peek() const {
		return core.back();
	}
	
	// Returns the "height" of the stack
	int length() const {
		return core.length();	
	}
	
	// Returns the "height" of the stack
	int size() const {
		return core.length();
	}
	
	// Determines whether the stack is empty
	bool empty() const {
		return core.empty();	
	}
	
	// Out the items in the stack
	void print() const {
		core.print();	
	}
	
	T operator[](int index) const {
		return core[index];
	}
	
	// Returns true when all the elements are the same in both stacks
	bool operator==(const Stack<T> & arg) const {
		if (core.length() != arg.size()) {
			return false;
		}
		
		for (int i = 0; i < core.length(); i++) {
			if (core[i] != arg[i]) {
				return false;	
			}
		}
		return true;
	}
	
	// Returns true if stack has fewer elements than argument
	bool operator<(const Stack<T> & arg) const {
		return core.length() < arg.size();
	}
	
	// Returns true if stack has more elements than argument
	bool operator>(const Stack<T> & arg) const {
		return core.length() > arg.size();
	}
};

template <typename T>
ostream & operator<<(ostream & output, const Stack<T> & arg) {
	arg.print();
	return output;	
}



/* ==================================================
QList - Queue Inheritance Implementation
================================================== */
template <typename T>
class QList : public LList<T> {
private:
	
public:
	QList() : LList<T>() {}
	
	QList(const QList & arg) : LList<T>(arg) {}

	QList<T> & operator=(const QList<T> & arg) {
		if (this == &arg) {
			return *this;
		}
		
		// convert to LList<T> * first
		*((LList<T> *)(this)) = arg;			
		return *this;
	}

	~QList(){}
	
	// Insert an item in the back of the queue
	bool enqueue(T arg) {
		return LList<T>::push_back(arg);
	}
	
	// Remove an item from the front of the queue
	T dequeue() {
		return LList<T>::pop_front();
	}
	
	// Return the item in the front of the queue
	T front() const {
		return LList<T>::front();
	}
	
	// Return the item in the back of the queue
	T back() const {
		return LList<T>::back();
	}
	
	// Return the length of the queue
	int length() const {
		return LList<T>::length();
	}
	
	// Returns the length of the queue
	int size() const {
		return LList<T>::length();
	}
	
	// Determines whether the queue is empty
	bool empty() const {
		return LList<T>::empty();
	}
	
	// Output the items in the queue
	void print() const {
		LList<T>::print();
	}
	
	T operator[](int index) const { 
		// convert to LList<T> first
		return ((LList<T>)(*this))[index]; 		
	}				
	
	// Returns true when all the elements are the same in both queues
	bool operator==(const QList<T> & arg) const {
		// convert to LList<T> first
		return ((LList<T>)(*this)) == arg;		
	}	
	
	// Returns true if queue has fewer elements than argument
	bool operator<(const QList<T> & arg) const {
		// convert to LList<T> first
		return ((LList<T>)(*this)) < arg;		
	}	
	
	// Returns true if queue has more elements than argument
	bool operator>(const QList<T> & arg) const {
		// convert to LList<T> first
		return ((LList<T>)(*this)) > arg;		
	}
};

template <typename T>
ostream & operator<<(ostream & output, const QList<T> & arg) {
	arg.print();
	return output;
}



/* ==================================================
SList - Stack Inheritance Implementation
================================================== */
template <typename T>
class SList : public LList<T> {
private:
	
public:
	SList() : LList<T>() {}
	
	SList(const SList & arg) : LList<T>(arg) {}
	
	SList<T> & operator=(const SList<T> & arg) {
		if (this == &arg) {
			return *this;
		}
		
		// convert to LList<T> * first
		*((LList<T> *)(this)) = arg;			
		return *this;
	}

	~SList(){}
	
	// Insert an item on top of the stack
	bool push(T arg){
		return LList<T>::push_back(arg);
	}
	
	// Remove an item from the top of the stack
	T pop(){
		return LList<T>::pop_back();
	}
	
	// Return the item on top of the stack
	T peek() const {
		return LList<T>::back();
	}

	// Returns the size of the stack
	int size() const {
		return LList<T>::length();
	}
	
	// Determines whether the stack is empty
	bool empty() const {
		return LList<T>::empty();
	}
	
	// Output the items in the stack
	void print() const {
		LList<T>::print();
	}
	
	T operator[](int index) const { 
		// convert to LList<T> first
		return ((LList<T>)(*this))[index]; 		
	}				
	
	// Returns true when all the elements are the same in both stacks
	bool operator==(const SList<T> & arg) const {
		// convert to LList<T> first
		return ((LList<T>)(*this)) == arg;		
	}	
	
	// Returns true if stack has fewer elements than argument
	bool operator<(const SList<T> & arg) const {
		// convert to LList<T> first
		return ((LList<T>)(*this)) < arg;		
	}	
	
	// Returns true if stack has more elements than argument
	bool operator>(const SList<T> & arg) const {
		// convert to LList<T> first
		return ((LList<T>)(*this)) > arg;		
	}
};

template <typename T>
ostream & operator<<(ostream & output, const SList<T> & arg) {
	arg.print();
	return output;
}
