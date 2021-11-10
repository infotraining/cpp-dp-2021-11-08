#include "strategy.hpp"

int main()
{
    DataContext data{std::make_shared<UpperCaseFormatter>()};
    data.pretty_print();

    data.set_data("text SAMPLE");

    auto lcf = std::make_shared<LowerCaseFormatter>();
    data.reset_formatter(lcf);
    data.pretty_print();

    data.reset_formatter(std::make_shared<CapitalizeFormatter>());
    data.pretty_print();
}
