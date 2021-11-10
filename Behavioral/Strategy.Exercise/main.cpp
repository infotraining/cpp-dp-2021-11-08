#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

struct StatResult
{
    std::string description;
    double value;

    StatResult(const std::string& desc, double val)
        : description(desc)
        , value(val)
    {
    }
};

using Data = std::vector<double>;
using Results = std::vector<StatResult>;

enum StatisticsType
{
    avg,
    min_max,
    sum
};

using FStatistics = std::function<Results(const Data& data)>;

struct IStatistics
{
    virtual Results calculate(const Data& data) = 0;
    virtual ~IStatistics() = default;
};

struct Average : IStatistics
{
    Results calculate(const Data& data) override
    {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        double avg = sum / data.size();

        return {{"Avg", avg}};
    }
};

struct Median : IStatistics
{
    Results calculate(const Data& data) override
    {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        double avg = sum / data.size();

        return {{"Median", avg}};
    }
};

struct MinMax : IStatistics
{
    Results calculate(const Data& data) override
    {
        double min = *(std::min_element(data.begin(), data.end()));
        double max = *(std::max_element(data.begin(), data.end()));

        return {{"Min", min}, {"Max", max}};
    }
};

struct Sum : IStatistics
{
    Results calculate(const Data& data) override
    {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);

        return {{"Sum", sum}};
    }
};

class StatGroup : public IStatistics
{
private:
    std::vector<std::shared_ptr<IStatistics>> _stats;

public:
    StatGroup(std::vector<std::shared_ptr<IStatistics>> stats)
        : _stats {std::move(stats)} {

        };

    Results calculate(const Data& data) override
    {
        Results r = {};
        for (const auto& stat : _stats)
        {
            auto temp_r = stat->calculate(data);
            r.insert(r.end(), temp_r.begin(), temp_r.end());
        }

        return r;
    }
};

class DataAnalyzer
{
    std::shared_ptr<IStatistics> statistics_;
    Data data_;
    Results results_;

public:
    DataAnalyzer(std::shared_ptr<IStatistics> stat)
        : statistics_ {stat}
    {
    }

    void load_data(const std::string& file_name)
    {
        data_.clear();
        results_.clear();

        std::ifstream fin(file_name.c_str());
        if (!fin)
            throw std::runtime_error("File not opened");

        double d;
        while (fin >> d)
        {
            data_.push_back(d);
        }

        std::cout << "File " << file_name << " has been loaded...\n";
    }

    void set_statistics(std::shared_ptr<IStatistics> stat)
    {
        statistics_ = stat;
    }

    void calculate()
    {
        auto stat_results = statistics_->calculate(data_);

        results_.insert(results_.end(), begin(stat_results), end(stat_results));
    }

    const Results& results() const
    {
        return results_;
    }
};

void show_results(const Results& results)
{
    for (const auto& rslt : results)
        std::cout << rslt.description << " = " << rslt.value << std::endl;
}

int main()
{
    std::shared_ptr<IStatistics> avg = std::make_shared<Average>();
    std::shared_ptr<IStatistics> min_max = std::make_shared<MinMax>();
    std::shared_ptr<IStatistics> sum = std::make_shared<Sum>();

    auto std_stats = std::make_shared<StatGroup>(std::vector{avg, min_max, sum});
    auto extra_stats = std::make_shared<StatGroup>(std::vector<std::shared_ptr<IStatistics>>{std_stats, std::make_shared<Median>()});

    DataAnalyzer da{extra_stats};
    da.load_data("data.dat");
    da.calculate();
    show_results(da.results());

    std::cout << "\n\n";

    da.load_data("new_data.dat");
    da.calculate();
    show_results(da.results());
}
