#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "factory.hpp"

using namespace std;

namespace Typical
{
    class Service
    {
        shared_ptr<LoggerCreator> creator_;
        bool is_started_ = false;

    public:
        Service(shared_ptr<LoggerCreator> creator)
            : creator_(creator)
        {
        }

        Service(const Service&) = delete;
        Service& operator=(const Service&) = delete;

        void run()
        {
            unique_ptr<Logger> logger = creator_->create_logger();
            if (!is_started_)
                start();
            logger->log("Service has been started...");
            process_requests();
            logger->log("Service has processed all requests...");
        }

    protected:
        virtual void start() { }
        virtual void process_requests() { }
    };

    using LoggerFactory = std::unordered_map<std::string, shared_ptr<LoggerCreator>>;

}

using LoggerCreator = std::function<std::unique_ptr<Logger>()>;

class Service
{
    LoggerCreator creator_;
    bool is_started_ = false;

public:
    Service(LoggerCreator creator)
        : creator_(std::move(creator))
    {
    }

    Service(const Service&) = delete;
    Service& operator=(const Service&) = delete;

    void run()
    {
        unique_ptr<Logger> logger = creator_();
        if (!is_started_)
            start();
        logger->log("Service has been started...");
        process_requests();
        logger->log("Service has processed all requests...");
    }

protected:
    virtual void start() { }
    virtual void process_requests() { }
};

using LoggerFactory = std::unordered_map<std::string, LoggerCreator>;

int main()
{
    LoggerFactory logger_factory;
    logger_factory.insert(make_pair("ConsoleLogger", &std::make_unique<ConsoleLogger>));
    logger_factory.emplace("FileLogger"s, [](){ return std::make_unique<FileLogger>("data.log"); });
    logger_factory.emplace("DbLogger"s, [](){ return std::make_unique<DbLogger>("connection string"); });

    Service srv(logger_factory.at("DbLogger"));
    srv.run();
}
