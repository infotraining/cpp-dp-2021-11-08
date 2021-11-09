#include "decorator.hpp"
#include <memory>

using namespace std;

void client(Component& c)
{
    c.operation();
}

int main()
{
    // Create ConcreteComponent and two Decorators
    shared_ptr<Component> c = make_shared<ConcreteComponent>();
    shared_ptr<Decorator> d1 = make_shared<ConcreteDecoratorA>(c);
    shared_ptr<Decorator> d2 = make_shared<ConcreteDecoratorB>(d1);
    client(*d2);

    cout << "\n\n";
    d2->set_component(c);
    client(*d2);

    cout << "\n\n";
    auto d3 = make_shared<ConcreteDecoratorA>(make_shared<ConcreteDecoratorB>(make_shared<ConcreteComponent>()));
    client(*d3);
}
