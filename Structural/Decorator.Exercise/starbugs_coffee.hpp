#ifndef COFFEEHELL_HPP_
#define COFFEEHELL_HPP_

#include <iostream>
#include <string>

class Coffee
{
public:
    virtual ~Coffee() = default;

    virtual float get_total_price() const = 0;
    virtual std::string get_description() const = 0;
    virtual void prepare() = 0;
};

class CoffeeBase : public Coffee
{
    float price_;
    std::string description_;

public:
    CoffeeBase(float price, const std::string& description) : price_{price}, description_{description}
    {
    }

    float get_total_price() const override
    {
        return price_;
    }

    std::string get_description() const override
    {
        return description_;
    }
};

class Espresso : public CoffeeBase
{
public:
    Espresso(float price = 4.0, const std::string& description = "Espresso")
        : CoffeeBase{price, description}
    {
    }

    void prepare() override
    {
        std::cout << "Making a perfect espresso: 7 g, 15 bar and 24 sec.\n";
    }
};

class Cappuccino : public CoffeeBase
{
public:
    Cappuccino(float price = 6.0, const std::string& description = "Cappuccino")
        : CoffeeBase{price, description}
    {
    }

    void prepare() override
    {
        std::cout << "Making a perfect cappuccino.\n";
    }
};

class Latte : public CoffeeBase
{
public:
    Latte(float price = 8.0, const std::string& description = "Latte")
        : CoffeeBase{price, description}
    {
    }

    void prepare() override
    {
        std::cout << "Making a perfect latte.\n";
    }
};

class CoffeeDecorator : public CoffeeBase
{
    std::unique_ptr<Coffee> coffee_;
public:
    CoffeeDecorator(std::unique_ptr<Coffee> c, float price, std::string description)
        : coffee_{std::move(c)}, CoffeeBase{price, std::move(description)}
    {}

    float get_total_price() const override
    {
        return CoffeeBase::get_total_price() + coffee_->get_total_price();        
    }
    
    std::string get_description() const override
    {
        return coffee_->get_description() + " + " + CoffeeBase::get_description();
    }

    void prepare() override
    {
        coffee_->prepare();
    }
};

class Whipped : public CoffeeDecorator
{
public:
	Whipped(std::unique_ptr<Coffee> c) : CoffeeDecorator{std::move(c), 2.5f, "Whipped Cream"}
    {}

	void prepare() override
    {
        CoffeeDecorator::prepare();
        std::cout << "Adding whipped cream...\n";
    }
};

class Whisky : public CoffeeDecorator
{
public:
	Whisky(std::unique_ptr<Coffee> c) : CoffeeDecorator{std::move(c), 6.0f, "Whisky"}
    {}

	void prepare() override
    {
        CoffeeDecorator::prepare();
        std::cout << "Adding Whisky...\n";
    }
};

class ExtraEspresso : public CoffeeDecorator
{
public:
	ExtraEspresso(std::unique_ptr<Coffee> c) : CoffeeDecorator{std::move(c), 4.0f, "Extra espresso"}
    {}

	void prepare() override
    {
        CoffeeDecorator::prepare();
        std::cout << "Adding extra espresso...\n";
    }
};

// TO DO: Condiments: Whipped: 2.5$, Whisky: 6.0$, ExtraEspresso: 4.0$

// TO DO: Add CoffeeDecorator and concrete decorators for condiments 

#endif /*COFFEEHELL_HPP_*/
