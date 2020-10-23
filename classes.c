#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**************************************************************************
 * "Classes" in C
 *************************************************************************/

// First, let's talk about how you would write a plain "class" in C.
typedef struct {
    int x;
    int y;
} Point;

// Constructor
Point* Point_Create(int x, int y) {
    Point* self = (Point*)malloc(sizeof(Point));
    self->x = x;
    self->y = y;
    return self;
}

// "Member" functions take a pointer to "self" as a parameter. This is like the
// 'this' keyword in C++.
void Point_Print(const Point* self) {
    printf("I'm a point! (%d, %d)\n", self->x, self->y);
}

// Destructor
void Point_Destroy(Point** selfPtr) {
    free(*selfPtr);
    selfPtr = NULL;
}

// How you'd use the Point class
void pointExample() {
    Point* point = Point_Create(0, 0);
    Point_Print(point);
    Point_Destroy(&point);
}

int main() { pointExample(); }
