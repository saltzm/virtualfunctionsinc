

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**************************************************************************
 * Virtual functions
 *************************************************************************/

// Let's say we want a base class called Parent, and a derived class called
// Child.

typedef struct Parent Parent;

// Everything marked "virtual" goes here, in the virtual function table, AKA
// VTable. You can think of it as an object containing the interface for the
// class.
typedef struct {
    void (*PrintSomething)(Parent*);
    void (*Destroy)(Parent**);
} ParentVTable;

// The Parent class in this case contains no data - just a pointer to the
// VTable for this particular instance.
//
// Derived classes that "override" the parent need to do two things:
// 1. Create an instance of ParentVTable where PrintSomething and Destroy
// point to implementations of the function for the derived class
// 2. Have an object of type Parent as the first member. This allows objects of
// the derived type to be cast to objects of the Parent type, since their data
// layout will be the same.
struct Parent {
    ParentVTable* vtbl;
    // Additional parent-only data members go here
};

// These are just for convenience when type-casting functions in the VTable of
// derived classes.
typedef void (*Parent_PrintSomethingFnPtr)(Parent*);
typedef void (*Parent_DestroyFnPtr)(Parent**);

// When you call a function on a pointer of type parent, it looks in its VTable
// and calls the proper function there.
void Parent_PrintSomething(Parent* self) { self->vtbl->PrintSomething(self); }

void Parent_Destroy(Parent** self) { (*self)->vtbl->Destroy(self); }

/***********************************************************************
 * Child class
 ***********************************************************************/

typedef struct {
    // Having this as the first member means that it is possible to cast Child
    // objects as Parent objects, since the data layout will be the same.
    Parent parent;
    // Additional child-only data members go here
} Child;

Child* Child_Create();
void Child_PrintSomething(Child* self);
void Child_Destroy(Child** self);

// This declares a static instance of the parent's that should be used
// for all instances of type Child. It maps functions in the ParentVTable to
// the Child-specific implementations of those functions.
static ParentVTable ChildVTable = {
    .PrintSomething = (Parent_PrintSomethingFnPtr)&Child_PrintSomething,
    .Destroy = (Parent_DestroyFnPtr)&Child_Destroy};

Child* Child_Create() {
    Child* self = (Child*)malloc(sizeof(Child));
    // This sets the VTable on this instance to point to ChildVTable,
    self->parent.vtbl = &ChildVTable;
    return self;
}

void Child_PrintSomething(Child* self) { printf("I'm a child!\n"); }

void Child_Destroy(Child** self) {
    free(*self);
    self = NULL;
}

int main() {
    {
        Child* child = Child_Create();
        Child_PrintSomething(child);
        Child_Destroy(&child);
    }
    {
        Parent* parent = (Parent*)Child_Create();
        Parent_PrintSomething(parent);
        Parent_Destroy(&parent);
    }
}
