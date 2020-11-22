#include <iostream>
#include <vector>
#include <tuple>

// A. High-level modules should not depend on low-leve modules.
//    Both should depend on abstractions.
// B. Abstratcions should not depend on details.
//    Details should depend on abstractions

enum class Relationship
{
    parent,
    child,
    sibling
};

struct Person
{
    std::string name;
};

// To fix it, we can add an abstraction
struct RelationshipBrowser
{
    virtual std::vector<Person> find_all_children_of(const std::string & name)  = 0;
};

struct Relationships :public RelationshipBrowser //  low-level module
{
    std::vector<std::tuple<Person,  Relationship, Person>> relations;

    void add_parent_and_child(const Person &parent, const Person& child)
    {
        relations.push_back({parent, Relationship::parent, child});
        relations.push_back({child, Relationship::child, parent});
    }

    std::vector<Person> find_all_children_of(const std::string & name) override
    {
        std::vector<Person> result;
        // C++17 structured binding not working on g++ 6.8
        // for (auto && [first, rel, second] : relations)
        for (auto & tp : relations)
        {
            if (std::get<0>(tp).name == name && std::get<1>(tp) == Relationship::parent)
            {
                result.push_back(std::get<2>(tp));
            }
        }

        return result;
    }
};

/*struct Research  //  high-level module
{
    Research(Relationships & relationships)  // WRONG!! depends on a low level module
    {
        // Depend on the implementation details of a low level module
        auto & relations =  relationships.relations;
        for (auto&& [first,rel,second] : relations)
        {
            if (first.name ==  "John" && rel == Relationship::parent)
            {
                std::cout <<  "John has a child called " << second.name << std::endl;
            }
        }
    }
};
*/

// Fixed version
struct Research //  high-level
{
    Research(RelationshipBrowser & browser)
    {
        for (auto & child : browser.find_all_children_of("John"))
        {
            std::cout << "John has a child called "  << child.name << std::endl;
        }
    }
};

int main()
{
    std::cout << "Dependency Inversion Principle" << std::endl;

    Person parent{"John"};
    Person child1{"Chris"}, child2{"Matt"};

    Relationships relationships;
    relationships.add_parent_and_child(parent, child1);
    relationships.add_parent_and_child(parent, child2);

    Research _(relationships);

    return 0;
};
