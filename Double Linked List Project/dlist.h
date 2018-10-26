#ifndef DLIST_H
#define DLIST_H

#include <iterator>

template <class T>
class DList {
	struct Node {
		T data;
		Node* next;
		Node* prev;
		Node(const T& x, Node* y = NULL, Node* z = NULL) : data(x), prev(y), next(z) {}
	};

	Node* head;

	public:

	class iterator {
		Node* nd;

		public:
		// typedefs
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;

		friend class DList;		// List objects have access to private members of List::iterator.

		// constructors
		inline iterator(Node* x=NULL) : nd(x) {}	// construct from node pointer
		inline iterator(const iterator& x) : nd(x.nd) {}	// construct from another iterator

		// operator overloads
		inline iterator& operator=(const iterator& x) {
			nd = x.nd;
			return *this;
		}	// assignment

		inline iterator& operator++() {
			nd = nd->next;
			return *this;
		}	// prefix increment

		inline iterator operator++(int) {
			iterator tmp(*this);	// make copy of current iterator for return
			nd = nd->next;				// advance current iterator
			return tmp;
		}	// postfix increment

		inline T& operator*() const { return nd->data;}	// dereference

		inline bool operator==(const iterator& x) const {
			return nd == x.nd;
		}	// equal

		inline bool operator !=(const iterator& x) const {
			return nd != x.nd;
		}	// not equal
	};		// end iterator

	DList() {
	    T x; //variable holder for Node constructor
	    head = new Node(x);
        head->next = head;
        head->prev = head;

	}		// Header node is allocated along with a variable holder for the Node constructor, and head is linked to itself
	~DList() { clear(); }
	void clear() { while (!empty()) pop_front(); }

	// It's empty if head is just linking to itself
	bool empty() { return head->next == head; }

	// Link head to new first node, new first node to head, new first node to former first node, former first node to new first node.
	void push_front(const T& x) {
		Node* nd = new Node(x, head, head->next);
		head->next->prev = nd;
		head->next = nd;
	}

	// Link head to *second* node, and link second node to head. The first node is deallocated and second node is the new first node
	void pop_front() {
		if (!empty()) {
			Node* nd = head->next;
			head->next = nd->next;
			nd->next->prev = head;
			delete nd;
		}
		else return;
	}

	// Creates a new node and it is inserted before the position's node, links are fixed.
	void insert(const iterator position, const T& val) {
		Node* nd = new Node(val, position.nd->prev, position.nd);	// new node's next will be position's node
        position.nd->prev->next = nd;
        position.nd->prev = nd;
	}

	// Erase the node at the position, position's previous is linked to it's next and it's next to position's previous
	// The node being erased is deallocated. If the list is empty, don't do anything
	void erase(const iterator position) {
	    if(!empty()){
	    position.nd->next->prev = position.nd->prev;
	    position.nd->prev->next = position.nd->next;
	    delete position.nd;
	    }
	    else return;

	}

	// begin method returns an iterator to the next node of head
	// end method returns an iterator of the head node
	iterator begin() { return iterator(head->next); }
	iterator end() { return iterator(head); }
};	// end list


#endif
