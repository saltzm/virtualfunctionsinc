
#include <stdio.h>

class Parent {
   public:
    virtual void PrintSomething() = 0;
    virtual ~Parent() {}
};

class Child : public Parent {
   public:
    void PrintSomething() override { printf("I'm a child\n"); }
};

int main() {
    {
        Child* child = new Child;
        child->PrintSomething();
        delete child;
    }
    {
        Parent* parent = new Child;
        parent->PrintSomething();
        delete parent;
    }
}
