#include <iostream>
#include <map>
#include <memory>
#include <vector>

using std::cout;
using std::endl;
using std::move;

#pragma region "Class A with copy tracking"
// Class A with copy tracking
//  - _id is a static member that is incremented each time an object is created
//  - _my_id is the id of the object
//  - _parent_id is the id of the object that was copied to create this object
//  - _copy_id is the number of times this object has been copied
// only doing this to show how objects are created and destroyed, would not normally do this
class A {
    static int _id;
    int _my_id;
    int _parent_id;
    int _copy_id;
public:
    //getter functions
    int id() { return _my_id; }
    int parent_id() { return _parent_id; }
    int copy_id() { return _copy_id; }
    //constructors and destructors with tracking
    A() : _parent_id(0), _copy_id(0) { _my_id=_id++; cout << "A constructor "  << _my_id << endl; }
    //copy constructor
    A(const A& other) : _my_id(_id++), _parent_id(other._my_id), _copy_id(other._copy_id + 1) { 
        cout << "A copy constructor "  << _my_id << " parent id " << _parent_id << " copy id " << _copy_id << endl; 
    }
    //move constructor
    A(A&& other) noexcept : _my_id(other._my_id), _parent_id(other._parent_id), _copy_id(other._copy_id) {
    }
    //copy assignment
    A& operator=(A&& other) noexcept {
        if (this != &other) {
            _my_id = other._my_id;
            _parent_id = other._parent_id;
            _copy_id = other._copy_id;
        }
        return *this;
    }
    //destructor
    ~A() { 
        cout << "\033[1;31m" << "A destructor "  << _my_id << " parent id " << _parent_id << " copy id " << _copy_id << "\033[0m"<< endl; 
    }
};

//initialize the static member (using 1 so when I sum the ids I can tell if it is working)
int A::_id = 1;
#pragma endregion

#pragma region "Class B to hold objects"
//create a class to hold our objects
class B {
    std::vector<A> _store;
public:
    //MEthods to add objects to the store
    //Each method uses std::move to transfer ownership of the object to the store to avoid extra copies!
    void add(A a) { _store.emplace_back(move(a)); show();}
    void add(A* a) { _store.emplace_back(move(*a)); show();}
    void add_byref(A& a) { _store.emplace_back(move(a)); show();}
    void add(std::unique_ptr<A> a) { _store.emplace_back(move(*a)); show();}
    void show() {
        cout << "B store item:parent" << endl;
        //let's make the output green to stand out
        cout << "\033[1;32m";
        for (auto& a : _store) {
            cout << "  " << a.id() << ":" << a.parent_id() << "\t";
        }
        cout << endl;
        //reset the output color
        cout << "\033[0m";
        cout << "End of B store" << endl;
    }
    std::vector<A>& getStore() { return _store; }
};
#pragma endregion

#pragma region "creation functions"
// let's create some objects in functions, to show how they are created and destroyed
// could also do this 'inline' in main, but this is more organized and shows how to pass objects around
A* createA() {
    A* a = new A();
    cout << "create A #" << a->id() << " in createA"<< endl;
    return a;
}

A& createARef() {
    A* a = new A();
    cout << "create ARef #" << a->id() << " in createARef"<< endl;
    return *a;
}

std::unique_ptr<A> createAUnique() {
    std::unique_ptr<A> a = std::make_unique<A>();
    cout << "create AUnique #" << a->id() << " in createAUnique"<< endl;
    return a;
}   
#pragma endregion

//make a collection of objects
void make_them_all(B& b) {

    cout << "create an object 'a_ptr' ";
    A* a_ptr = createA();
    cout << "add 'a_ptr' by (pointer)value ";
    b.add(a_ptr); 

    cout << "create an object 'a_ref' "; 
    A& a_ref = createARef();
    cout << "add 'a_ref' by reference "; 
    b.add_byref(a_ref); 

    cout << "create an object 'a_unique' ";
    std::unique_ptr<A> a_unique = createAUnique();
    cout << "add 'a_unique' by unique_ptr using move to transfer ownership "; 
    b.add(move(a_unique)); 

    cout << "create an object 'a' ";
    A a;
    cout << "add 'a' by value ";
    b.add(a); 

    cout << "End of make_them_all" << endl;

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
    cout << "Sum of ids " << sum << endl;
    cout << "End of main" << endl;
 
    return 0;
}
