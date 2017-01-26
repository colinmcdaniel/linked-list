/*
 Colin McDaniel
 February 26, 2013
 
 This program gets elements of a List from the user and continues to manipulate and print the list in different ways.
 */

#include<iostream>
#include<cassert>

using namespace std;

// Class declarations
class List;
class Iterator;

class Node{
public:
    
    // Constructor
    Node(int n);
    
	// Friend functions
	friend void sort(List& l);
    
private:
    
    // Stores data of this node
    int data;
    
    // Pointer to previous object in list
    Node* previous;
    
    // Pointer to next object in list
    Node* next;
    
    // Friend classes
    friend class List;
    friend class Iterator;
};

class List{
public:
    
    // Constructor
    List();
    
    // Member functions
    void push_back(int data);
    void insert(Iterator iter, int n);
    Iterator erase(Iterator iter);
    Iterator begin();
    Iterator end();
    void reverse();
    void push_front(int data);
    void swap(List& other);
    int get_size() const;
    
private:
    
    // Points to first object in list
    Node* first;
    
    // Points to last object in list
    Node* last;
    
    // Size of list
    int size;
    
    // Friend class
    friend class Iterator;
};

class Iterator{
public:
    
    // Constructor
    Iterator();
    
    // Member functions
    int get() const;
    void next();
    void previous();
    bool equals(Iterator b) const;
    
    // Operator overloads
    Iterator operator++ (int unused);
    Iterator& operator++();
    Iterator operator-- (int unused);
    Iterator& operator--();
    bool operator==(Iterator b);
    
    // Friend functions
    friend void sort(List& l);
    friend List merge(List a, List b);
    
private:
    
    // Points to current position
    Node* position;
    
    // Points to container of nodes that Iterator is pointing to
    List* container;
    
    // Friend class
    friend class List;
};

// Non-member functions
void downsize(List& l);
int maximum(List l);
void sort(List& l);
List merge(List a, List b);

// Operator overload
ostream& operator<< (ostream& out, List& l);


int main(){
    
    // Prompt user to enter positive integers into a list
    cout << "Please input a set of nonnegative numbers for a List (Enter -1 when you are finished):\n";
    List l;
    int data = 0;
    while(data != -1){
        cin >> data;
        if(data != -1)
            l.push_back(data);
    }
    
    // Prints list to screen
    cout << "Your list is\n" << l << endl << endl;
    
    // Prompts user to insert values at a user-defined index
    Iterator pos;
    int index = 0;
    while(index != -1){
        cout << "Select an index for insertion (enter -1 when finished): ";
        cin >> index;
        
        if(index >= 0){
            
            // Increment iterator to point to the appropriate node to insert
            pos = l.begin();
            for(int i = 0; i < index; i++)
                pos++;
            
            // Prompt user to enter a value for insertion and insert into appropriate place
            cout << "Select a value for insertion: ";
            cin >> data;
            l.insert(pos,data);
        }
    }
    
    // Prints list with inserted values
    cout << endl << "The augmented List is\n" << l << endl << endl;
    
    // Prints list maximum
    cout << "The maximum of the List is: " << maximum(l) << endl;
    
    // Prints list size
    cout << "The size of the List is: " << l.get_size() << endl;
    
    // Removes list of every second value from list and prints
    cout << "When we remove every second value from this list we are left with\n";
    downsize(l);
    cout << l << endl;
    
    // Sorts list and prints
    cout << "When we sort this downsized list we obtain\n";
    sort(l);
    cout << l << endl;
    
    // Reverses list and prints
    cout << "And this sorted list in reverse order is\n";
    l.reverse();
    cout << l << endl;
    
    // Creates list to merge with l
    List listToMerge;
    listToMerge.push_back(2);
    listToMerge.push_back(3);
    listToMerge.push_back(5);
    listToMerge.push_back(7);
    listToMerge.push_back(11);
    
    // Merges list with list (2,3,5,7,11) and prints
    cout << "If we merge this list with the list (2,3,5,7,11) we obtain\n";
    List merged = merge(l,listToMerge);
    cout << merged << endl << endl;
    
    return 0;
}


/** Constructs a code with a given data value
 @param n the data to store in this node
 */
Node::Node(int n){
    data = n;
    previous = NULL;
    next = NULL;
}

/** Constructs an empty list */
List::List(){
    first = NULL;
    last = NULL;
    size = 0;
}

/** Appends an element to the list
 @param data the value to append
 */
void List::push_back(int data){
    Node* new_node = new Node(data);
    if(last == NULL){       // List is empty
        first = new_node;
        last = new_node;
    }
    else{
        new_node->previous = last;
        last->next = new_node;
        last = new_node;
    }
    size++;
    return;
}

/** Inserts an element into the list
 @param pos points to node after the insertion position
 @param n the value to appent
 */
void List::insert(Iterator iter, int n){
    if(iter.position == NULL)       // Insert at end of list
        push_back(n);
    else{
        Node* after = iter.position;
        Node* before = after->previous;
        Node* new_node = new Node(n);
        
        new_node->previous = before;
        new_node->next = after;
        
        after->previous = new_node;
        if(before == NULL)      // Insert at beginning of list
            first = new_node;
        else
            before->next = new_node;
        
        size++;
    }
    return;
}

/** Removes an element from the list
 @param pos the position to remove
 @return an iterator pointing to the element after the erased element
 */
Iterator List::erase(Iterator iter){
    
    // Cause error if invalid position
    assert(iter.position != NULL);
    
    Node* remove = iter.position;
    Node* before = remove->previous;
    Node* after = remove->next;
    
    if(remove == first)
        first = after;
    else
        before->next = after;
    if(remove == last)
        last = before;
    else
        after->previous = before;
    
    delete remove;
    Iterator r;
    r.position = after;
    r.container = this;
    size--;
    return r;
}

/** Gets the beginning position of the list
 @return an iterator pointing to the beginning of the list
 */
Iterator List::begin(){
    Iterator iter;
    iter.position = first;
    iter.container = this;
    return iter;
}

/** Gets the position after the last element of the list
 @return an iterator pointing past the end of the list
 */
Iterator List::end(){
    Iterator iter;
    iter.position = NULL;
    iter.container = this;
    return iter;
}

/** Reverses the nodes in a list */
void List::reverse(){
    int size = this->get_size();
    if(size > 1){
        
        // Iterator pos points to last element
        Iterator pos = this->end();
        pos--;
        
        // push_back in reverse order
        for(int i = 0; i < size-1; i++){
            pos--;
            this->push_back(pos.get());
        }
        
        // erase ordered elements in first half of list
        for(int i = 0; i < size-1; i++){
            pos = this->begin();
			this->erase(pos);
        }
    }
    return;
}

/** Adds a value to the beginning of a list */
void List::push_front(int data){
    Node* new_node = new Node(data);
    
    if(last == NULL){       // List is empty
        first = new_node;
        last = new_node;
    }
    else{
        new_node->next = first;
        first->previous = new_node;
        first = new_node;
    }
    size++;
    return;
}

/** Swaps the elements of this list and another
 @param other the list to be swapped with this one
 */
void List::swap(List& other){
    List temp = other;
    other = *this;
    *this = temp;
    return;
}

/** Gets the number of elements in the list */
int List::get_size() const{
    return size;
}

/*
 //Old get_size function
 int List::get_size() const{
 int counter = 0;
 Iterator iter;
 iter.position = first;
 while(iter.position != NULL){
 ++iter;
 ++counter;
 }
 
 return counter;
 }*/

/** Constructs an iterator that does not post to any list */
Iterator::Iterator(){
    position = NULL;
    container = NULL;
}

/** Looks up the value at a position
 @return the value of the node to which the iterator points
 */
int Iterator::get() const{
    
    // Cause error if invalid position
    assert(position != NULL);
    
    return position->data;
}

/** Advances the iterator to the next node */
void Iterator::next(){
    
    // Cause error if invalid position
    assert(position != NULL);
    
    position = position->next;
    return;
}

/** Moves the iterator to the previous node */
void Iterator::previous(){
    
    // Cause error if iterator is pointing to the first element
    assert(position != container->first);
    
    if(position == NULL)        // Set iterator pointing to last element if iterator at position NULL
        position = container->last;
    else
        position = position->previous;
    return;
}

/** Compares two iterators
 @param b the iterator to compare with this iterator
 @return boolean value representing if this iterator and b are equal
 */
bool Iterator::equals(Iterator b) const{
    return position == b.position;
}

/** Postfix ++ operator for Iterator class that advances iterator to next node and returns original iterator by value
 @param unused to differentiate postfix version
 @return clone Iterator
 */
Iterator Iterator::operator++ (int unused){
    Iterator clone = *this;
    this->next();
    return clone;
}

/** Prefix ++ operator for Iterator class that advances iterator to next node and returns by value
 @return modified iterator
 */
Iterator& Iterator::operator++(){
    this->next();
    return *this;
}

/** Postfix -- operator for Iterator class that advances iterator to previous node and returns original iterator by value
 @param unused to differentiate postfix version
 @return clone Iterator
 */
Iterator Iterator::operator-- (int unused){
    Iterator clone = *this;
    this->previous();
    return clone;
}

/** Prefix -- operator for Iterator class that advances iterator to previous node and returns by value
 @return modified iterator
 */
Iterator& Iterator::operator--(){
    this->previous();
    return *this;
}

/** == operator overload for Iterator class
 @param b the iterator to compare with this iterator
 @return boolean value representing if this iterator and b are equal
 */
bool Iterator::operator== (Iterator b){
    return position == b.position;
}

/** Removes every second value from a linked list
 @param l list to be modified
 */
void downsize(List& l){
    int size = l.get_size();
    Iterator pos;
    for(int i = 0; i < size; i++){
        if(i % 2 == 1){
			pos = l.begin();
			pos++;
			for(int j = 0; j < i/2; j++)
				pos++;
            l.erase(pos);
		}
    }
    return;
}

/** Computes the largest nonnegative integer that the list stores
 @param l list to be searched
 @return max integer value in list
 */
int maximum(List l){
    int size = l.get_size();
    int max = 0;
    Iterator pos = l.begin();
    for(int i = 0; i < size; i++){
        if(max < pos.get())
            max = pos.get();
        pos++;
    }
    return max;
}

/** Sorts the elements of a linked list
 @param l the list to be sorted
 */
void sort(List& l){
    
    // pos_left is the "current" iterator position
    for(Iterator pos_left = l.begin(); pos_left.position != NULL; pos_left++){
        
        // pos_right tests all subsequent positons
        Iterator pos_right = pos_left;
        pos_right++;
        for(Iterator pos_right = pos_left; pos_right.position != NULL; pos_right++){
			if(pos_left.get() > pos_right.get()){       // Swap the values that pos_left and pos_right are pointing to
                int temp = pos_left.position->data;
				pos_left.position->data = pos_right.get();
				pos_right.position->data = temp;
            }
        }
    }
}

/** Merges two Lists into one, alternating elements from each list and then appending the remaining elements from the larger list
 @param a list to merge with b
 @param b list to merge with a
 */
List merge(List a, List b){
    int larger_size;
    if(a.get_size() < b.get_size())
        larger_size = b.get_size();
    else
        larger_size = a.get_size();
    
    List l_merge;
    Iterator pos_a = a.begin();
    Iterator pos_b = b.begin();
    
    // Push back alternating elements and append the rest when the other iterator reaches end of list
    for(int i = 0; i < larger_size; i++){
        if(pos_a.position != NULL){
            l_merge.push_back(pos_a.get());
            pos_a++;
        }
        if(pos_b.position != NULL){
            l_merge.push_back(pos_b.get());
            pos_b++;
        }
    }
    return l_merge;
}

/** << operator overload for Lists, outputs in appropriate format with parentheses and commas
 @param out output stream
 @param l List to be outputted
 @return out output stream with appropriate List data
 */
ostream& operator<< (ostream& out, List& l){
    out << "(";
    
    Iterator pos = l.begin();
    int size = l.get_size();
    for(int i = 0; i < size; i++){
        out << pos.get();
        
        // If not the last element, put a comma after integer
        if(i != size-1)
            out << ",";
        
        pos++;
    }
    
    out << ")";
    
    return out;
}