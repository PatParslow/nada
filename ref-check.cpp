//demonstrate pointers, references, unique_ptr
#include <iostream>
#include <map>
#include <memory>
#include <vector>

class A {
    static int _id;
    int _my_id;
public:
    int id() { return this->_my_id; }
    A() { _my_id=_id++;std::cout << "A constructor "  << _my_id << std::endl; }
    ~A() { std::cout << "A destructor "  << _my_id << std::endl; }
};
int A::_id = 0;

//create a class to hold our objects
class B {
    std::vector<A> _store;
public:
    void add(A a) { _store.push_back(a); }
    void add(A* a) { _store.push_back(*a); }
    void add_byref(A& a) { _store.push_back(a); }
    void add(std::unique_ptr<A> a) { _store.push_back(*a); }
    void show() {
        for (auto& a : _store) {
            std::cout << "B store " << a.id() << std::endl;
        }

    }
    std::vector<A>& getStore() { return _store; }
};
// we need to show how object go out of scope, so can't use references or pointers
A* createA() {
    A* a = new A();
    std::cout << "createA " << a->id() << " in createA"<< std::endl;
    return a;
}

A& createARef() {
    A* a = new A();
    std::cout << "createARef " << a->id() << " in createARef"<< std::endl;
    return *a;
}

std::unique_ptr<A> createAUnique() {
    std::unique_ptr<A> a = std::make_unique<A>();
    std::cout << "createAUnique " << a->id() << " in createAUnique"<< std::endl;
    return a;
}   

void make_them_all(B& b) {
    
    A a;
    A* a_ptr = createA();
    A& a_ref = createARef();
    std::unique_ptr<A> a_unique = createAUnique();
    //add them to store without creating new objects

    b.add(a); 
    b.add(a_ptr); 
    b.add_byref(a_ref); 
    b.add(std::move(a_unique)); 
    //show the store
    b.show();

    std::cout << "End of make_them_all" << std::endl;

}

int main() {
    //make a collection to keep things in
    B b = B();
    make_them_all(b);
    //show the store
    b.show();
    //add up the ids of all the objects
    int sum = 0;
    for (auto& a : b.getStore()){
        sum += a.id();
    }
    std::cout << "Sum of ids " << sum << std::endl;
    std::cout << "End of main" << std::endl;
 
    return 0;
}
