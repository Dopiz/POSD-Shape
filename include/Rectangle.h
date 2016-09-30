#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "Shape.h"
#include "Point.h"

class Rectangle : public Shape
{
    private:
        Point _upperLeftCorner;
        double _length, _width;

    public:
        Rectangle(double ulx, double uly, double length, double width);
        string content();
        double perimeter() const;
        double area() const;
        virtual ~Rectangle();
};

#endif // RECTANGLE_H

