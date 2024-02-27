//demonstrate pointers, references, unique_ptr
#include <iostream>
#include <map>
#include <memory>
#include <vector>

class A {
    static int _id;
    int _my_id;
    int _parent_id;
    int _copy_id;
public:
    int id() { return this->_my_id; }
    int parent_id() { return this->_parent_id; }
    int copy_id() { return this->_copy_id; }
    A() : _parent_id(0), _copy_id(0) { _my_id=_id++; std::cout << "A constructor "  << _my_id << std::endl; }
    A(const A& other) : _my_id(_id++), _parent_id(other._my_id), _copy_id(other._copy_id + 1) { 
        std::cout << "A copy constructor "  << _my_id << " parent id " << _parent_id << " copy id " << _copy_id << std::endl; 
    }
    A(A&& other) noexcept : _my_id(other._my_id), _parent_id(other._parent_id), _copy_id(other._copy_id) {
    }
    A& operator=(A&& other) noexcept {
        if (this != &other) {
            _my_id = other._my_id;
            _parent_id = other._parent_id;
            _copy_id = other._copy_id;
        }
        return *this;
    }
    ~A() { 
        std::cout << "\033[1;31m" << "A destructor "  << _my_id << " parent id " << _parent_id << " copy id " << _copy_id << "\033[0m"<< std::endl; 
    }
};
int A::_id = 1;

//create a class to hold our objects
class B {
    std::vector<A> _store;
public:
    void add(A a) { _store.emplace_back(std::move(a)); show();}
    void add(A* a) { _store.emplace_back(std::move(*a)); show();}
    void add_byref(A& a) { _store.emplace_back(std::move(a)); show();}
    void add(std::unique_ptr<A> a) { _store.emplace_back(std::move(*a)); show();}
    void show() {
        std::cout << "B store:" << std::endl;
        for (auto& a : _store) {
            std::cout << "  B store " << a.id() << ":" << a.parent_id() << std::endl;
        }
        std::cout << "End of B store" << std::endl;
    }
    std::vector<A>& getStore() { return _store; }
};


// let's create some objects in functions, to show how they are created and destroyed
A* createA() {
    A* a = new A();
    std::cout << "create A #" << a->id() << " in createA"<< std::endl;
    return a;
}

A& createARef() {
    A* a = new A();
    std::cout << "create ARef #" << a->id() << " in createARef"<< std::endl;
    return *a;
}

std::unique_ptr<A> createAUnique() {
    std::unique_ptr<A> a = std::make_unique<A>();
    std::cout << "create AUnique #" << a->id() << " in createAUnique"<< std::endl;
    return a;
}   

void make_them_all(B& b) {

    std::cout << "create an object 'a_ptr' ";
    A* a_ptr = createA();
    std::cout << "add 'a_ptr' by (pointer)value ";
    b.add(a_ptr); 

    std::cout << "create an object 'a_ref' "; 
    A& a_ref = createARef();
    std::cout << "add 'a_ref' by reference "; 
    b.add_byref(a_ref); 

    std::cout << "create an object 'a_unique' ";
    std::unique_ptr<A> a_unique = createAUnique();
    std::cout << "add 'a_unique' by unique_ptr using move to transfer ownership "; 
    b.add(std::move(a_unique)); 

    std::cout << "create an object 'a' ";
    A a;
    std::cout << "add 'a' by value ";
    b.add(a); 

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
