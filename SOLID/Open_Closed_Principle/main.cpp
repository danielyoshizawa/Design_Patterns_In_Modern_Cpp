#include <iostream>
#include <algorithm>
#include <vector>

// Open-Closed Principle
// Should be open for extension but closed for modification
//
// I'll replicate the example from the course
// because I really liked the way they did it.

enum class Color { red, green, blue };
enum class Size  { small, medium, large };

struct Product
{
    std::string name;
    Color color;
    Size size;
};

// Filtering not following the Open-Closed Principle
struct ProductFilter
{
    typedef std::vector<Product *> Items;

    Items by_color(Items items, const Color color)
    {
        Items result {items.size()};
        auto it = std::copy_if(std::begin(items), std::end(items), std::begin(result), [&](Product * item){
            return item->color == color;
        });
        result.resize(std::distance(std::begin(result), it));
        return result;
    }

    // Boss asked for another kind of filter
    Items by_size(Items items, const Size size)
    {
        // See some duplication??
        Items result {items.size()};
        auto it = std::copy_if(std::begin(items), std::end(items), std::begin(result), [&](Product * item){
            return item->size == size;
        });
        result.resize(std::distance(std::begin(result), it));
        return result;
    }

    // And another one
    Items by_size_and_color(Items items, const Size size, const Color color)
    {
        Items result {items.size()};
        auto  it = std::copy_if(std::begin(items), std::end(items), std::begin(result), [&](Product * item){
            return item->size == size &&  item->color == color;
        });
        result.resize(std::distance(std::begin(result), it));
        return result;
    }
};

// Now using the Open-Closed Principle
// First some interfaces respecting the Single Responsibility Principle.
// The idea here is to have a template that can be extended but doesn't
// need modification
template <typename T> struct Specification
{
    virtual ~Specification() = default;
    // This function will test if the item
    // statifies the specification
    virtual bool is_satisfied(T *item) const = 0;
};

// Same for the filter, which will receive the Specification,
// so when filtering you have say by what do you want to filter
template<typename T> struct Filter
{
    typedef std::vector<T *> vec_t;
    virtual ~Filter() = default;
    virtual vec_t filter(vec_t items, Specification<T> & spec) = 0;
};

// Implementing a filter
struct FilterImpl : Filter<Product>
{
    // Yes, I'm lazy
    typedef std::vector<Product *> vec_p;
    vec_p filter(vec_p items, Specification<Product> & spec) override
    {
        vec_p result(std::distance(std::begin(items), std::end(items)));
        auto it = std::copy_if(std::begin(items), std::end(items), std::begin(result), [&](Product * item){
            return spec.is_satisfied(item);
        });
        result.resize(std::distance(std::begin(result), it));
        return result;
    }
};

// Now we need the Specifications, What do you want boss?
// Some color in your life?
struct ColorSpecification : Specification<Product>
{
    Color color;

    explicit ColorSpecification(Color color) : color(color) {}
    bool is_satisfied(Product *item) const override
    {
        return item->color == color;
    }
};

// Oh, does size really matter?
struct SizeSpecification : Specification<Product>
{
    Size size;
    explicit SizeSpecification(const Size size) : size(size) {}
    bool is_satisfied(Product *item) const override
    {
        return item->size == size;
    }
};

template<typename T>
struct AndSpecification : Specification<T>
{
    const Specification<T> & first;
    const Specification<T> & second;

    AndSpecification(const Specification<T> & first, const Specification<T> & second)
    : first(first), second(second) {}

    bool is_satisfied(T *item) const override
    {
        return first.is_satisfied(item) && second.is_satisfied(item);
    }
};

int main()
{
    std::cout << "Open-Closed Principle" << std::endl;

    Product apple{"Apple", Color::green, Size::small};
    Product tree{"Tree", Color::green, Size::large};
    Product house{"House", Color::blue, Size::large};
    // Our large and green product
    Product hulk{"Hulk", Color::green, Size::large};

    const std::vector<Product *> all {&apple, &tree, &house, &hulk};

    FilterImpl bf;
    ColorSpecification green(Color::green);
    ColorSpecification blue(Color::blue);
    auto green_things =  bf.filter(all, green);
    auto blue_things = bf.filter(all, blue);
    for(auto & item : green_things)
    {
        std::cout << item->name << " is Green, Voila" << std::endl;
    }

    for (auto & item : blue_things)
    {
        std::cout << item->name << " is Blue, Magic!!" << std::endl;
    }

    SizeSpecification large(Size::large);
    auto large_things = bf.filter(all, large);
    for(auto & item : large_things)
    {
        std::cout << item->name << " is Large, pretty cool hum?" << std::endl;
    }

    // Later on boss asked to filter by Color and Size
    // so instead of filtering twice, lets implement
    // a new specialization of Specification to do it
    AndSpecification<Product> green_and_large(green, large);
    auto large_green_things = bf.filter(all, green_and_large);
    for(auto & item : large_green_things)
    {
        std::cout << item->name << " SMASH!!!!" << std::endl;
    }

    return 0;
}
