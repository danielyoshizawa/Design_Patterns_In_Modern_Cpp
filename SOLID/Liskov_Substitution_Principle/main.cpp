#include <iostream>

// Sub types should be able to be replaced by its super types
// If it looks like a Duck, quacks like a Duck, but needs batteries - You probably have the wrong abstraction

// Functions that use pointers or references to base classes must be able to use objects of derived classes
// without knowing it.

// Not the best example but the most used one
class Rectangle
{
protected:
    int width, height;
public:
    Rectangle(int w, int h) : width(w), height(h) {}
    virtual void setWidth(int w) {  width = w;  }
    virtual void setHeight(int h) { height = h; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

class Square : public Rectangle
{
public:
    Square(int s) : Rectangle(s,s) {}
    void setWidth(int w) override
    {
        Rectangle::setWidth(w);
        Rectangle::setHeight(w);
    }
    void setHeight(int h)  override
    {
        Rectangle::setWidth(h);
        Rectangle::setHeight(h);
    }
};

// Let's break LSP
// Here we show that this hierarch doesn't make sense in
// a computational solution
void Area(Rectangle &r)
{
    r.setWidth(5);
    r.setHeight(4);
    if( r.getWidth() * r.getHeight() == 20 )
    {
        std::cout << "Area function worked! " << std::endl;
    } else
    {
        std::cout << "Not the expected behavior! " << std::endl;
    }
}

int main()
{
    std::cout << "Liskov Substitution Principle" << std::endl;
    Rectangle rect(10,5);
    std::cout << "Running Area for a Rectangle" << std::endl;
    Area(rect);
    Square sq(4);
    std::cout << "Running Area for a Square" << std::endl;
    Area(sq);
    return 0;
}
