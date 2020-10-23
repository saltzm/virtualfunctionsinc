

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Parent Parent;

typedef struct {
    void (*PrintSomething)(Parent*);
    void (*Destroy)(Parent**);
} ParentVTable;

struct Parent {
    ParentVTable* vtbl;
};

typedef void (*Parent_PrintSomethingFnPtr)(Parent*);
typedef void (*Parent_DestroyFnPtr)(Parent**);

typedef struct {
    ParentVTable* parent;
} GoodChild;

void Parent_PrintSomething(Parent* self) { self->vtbl->PrintSomething(self); }

void Parent_Destroy(Parent** self) { (*self)->vtbl->Destroy(self); }

GoodChild* GoodChild_Create();
void GoodChild_PrintSomething(GoodChild* self);
void GoodChild_Destroy(GoodChild** self);

static ParentVTable GoodChildVTable = {
    .PrintSomething = (Parent_PrintSomethingFnPtr)&GoodChild_PrintSomething,
    .Destroy = (Parent_DestroyFnPtr)&GoodChild_Destroy};

GoodChild* GoodChild_Create() {
    GoodChild* self = (GoodChild*)malloc(sizeof(GoodChild));
    self->parent = &GoodChildVTable;
    return self;
}

void GoodChild_PrintSomething(GoodChild* self) {
    printf("I'm a good child!\n");
}

void GoodChild_Destroy(GoodChild** self) {
    free(*self);
    self = NULL;
}

/**
 * BadChild
 */
typedef struct {
    ParentVTable* parent;
} BadChild;

BadChild* BadChild_Create();
void BadChild_PrintSomething(BadChild* self);
void BadChild_Destroy(BadChild** self);

static ParentVTable BadChildVTable = {
    .PrintSomething = (Parent_PrintSomethingFnPtr)&BadChild_PrintSomething,
    .Destroy = (Parent_DestroyFnPtr)&BadChild_Destroy};

BadChild* BadChild_Create() {
    BadChild* self = (BadChild*)malloc(sizeof(BadChild));
    self->parent = &BadChildVTable;
    return self;
}

void BadChild_PrintSomething(BadChild* self) { printf("I'm a bad child!\n"); }

void BadChild_Destroy(BadChild** self) {
    free(*self);
    self = NULL;
}

int main() {
    {
        Parent* parent = (Parent*)GoodChild_Create();
        Parent_PrintSomething(parent);
        Parent_Destroy(&parent);
    }
    {
        Parent* parent = (Parent*)BadChild_Create();
        Parent_PrintSomething(parent);
        Parent_Destroy(&parent);
    }
}
