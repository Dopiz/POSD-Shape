#ifndef COMPOSITESHAPE_H
#define COMPOSITESHAPE_H
#include "Shape.h"

class CompositeShape : public Shape
{
    private:
        vector<Shape *> _combo;

    public:
        CompositeShape(string name);
        string content();
        double perimeter() const;
        double area() const;
        void addShape(Shape* shape);
        void addShape(vector<Shape *> shapes);
        ~CompositeShape();
};

#endif // COMPOSITESHAPE_H
