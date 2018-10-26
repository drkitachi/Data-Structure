#ifndef VECTOR_H
#define VECTOR_H


template <class T>
class Vector
{
    T* arr;
    int sz, capacity;

    // convenience function called by push_back and insert.
    // allocate a new array with double the capacity,
    // copy arr over the the new array,
    // deallocate arr,
    // point arr to the new array,
    // update capacity
    void grow(){
        capacity *= 2;
        T *temp = new T [capacity];
        for(int i = 0; i < sz; i++)
            temp[i] = arr[i];
        delete [] arr;
        arr = temp;
    }

public:
    // allocate a new array with size 0 (nothing is in it yet)
    // and capacity 10
    // set sz, capacity, arr
    Vector(){
        sz = 0;
        capacity = 10;
        arr = new T [capacity];
    }

    // deallocate arr
    ~Vector(){delete [] arr;}

    // add element to the end of arr and sz is incremented
    void push_back(const T& elt){
        if(sz == capacity)
            grow();
        arr[sz++] = elt;
    }

    // remove the last element in arr and sz is decremented
    void pop_back(){
        if(empty()) return;
        sz--;
    }

    // add element to arr at position pos
    // throw an exception if pos is not legal
    // sz is incremented
    void insert(const T& elt, int pos){
        if(pos < 0 || pos >= sz){throw std::exception();}
        sz++;
        if(sz == capacity)
            grow();
        for(int i = sz; i > pos; i--)
            arr[i] = arr[i-1];
        arr[pos] = elt;
    }

    // erase element at position pos
    // throw an exception if pos is not legal
    // sz is decremented
    void erase(int pos){
        if(pos < 0 || pos >= sz){throw std::exception();}
        sz--;
        for(int i = pos; i < sz; i++)
            arr[i] = arr[i+1];
    }

    // return first element in vector
    T& front(){return arr[0];}

    // return last element in vector
    T& back(){return arr[sz-1];}

    // return element at position pos
    // throw an exception if pos is not legal
    T& at(int pos){
        if(pos < 0 || pos >= sz){throw std::exception();}
        return arr[pos];
    }

    // return element at position pos (will break if pos is not legal)
    T& operator[](int pos){
        if(pos < 0 || pos >= sz){throw std::exception();}
        return arr[pos];
    }

    // return size of the vector, sz
    int size(){return sz;}

    // return true if vector is empty
    bool empty(){return sz == 0;}
};

#endif
