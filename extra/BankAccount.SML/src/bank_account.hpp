#ifndef BANK_ACCOUNT_HPP
#define BANK_ACCOUNT_HPP

#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#define BOOST_SML_DISABLE_EXCEPTIONS true

#include "boost/sml.hpp"

namespace Bank
{
    class InsufficientFunds : public std::runtime_error
    {
        const int id_;

    public:
        InsufficientFunds(const std::string& msg, int id) : std::runtime_error{ msg }, id_{ id }
        {
        }

        int id() const
        {
            return id_;
        }
    };

    namespace Classic
    {
        enum AccountState
        {
            overdraft,
            normal
        };

        class BankAccount
        {
            int id_;
            double balance_;
            AccountState state_;

        protected:
            void update_account_state()
            {
                if (balance_ < 0)
                    state_ = overdraft;
                else
                    state_ = normal;
            }

            void set_balance(double amount)
            {
                balance_ = amount;
            }

        public:
            BankAccount(int id) : id_(id), balance_(0.0), state_(normal) {}

            void withdraw(double amount)
            {
                assert(amount > 0);

                if (state_ == overdraft)
                {
                    throw InsufficientFunds{ "Insufficient funds for account #" + std::to_string(id_), id_ };
                }
                else // state_ == normal
                {
                    balance_ -= amount;

                    update_account_state();
                }
            }

            void deposit(double amount)
            {
                assert(amount > 0);

                balance_ += amount;

                update_account_state();
            }

            void pay_interest()
            {
                if (state_ == overdraft)
                    balance_ += balance_ * 0.15;

                else
                    balance_ += balance_ * 0.05;
            }

            std::string status() const
            {
                std::stringstream strm;
                strm << "BankAccount #" << id_ << "; State: ";

                if (state_ == overdraft)
                    strm << "overdraft; ";
                else
                    strm << "normal; ";

                strm << "Balance: " << std::to_string(balance_);

                return strm.str();
            }

            double balance() const
            {
                return balance_;
            }

            int id() const
            {
                return id_;
            }
        };
    }

    inline namespace SML
    {

		// events ---------------------------------------

        struct Withdraw
        {
            double amount;
        };

        struct Deposit
        {
            double amount;
        };

        struct PayInterest
        {
        };


		// data context ----------------------------------
		
        struct AccountContext
        {
            const int id;
            double balance;
        };


		// transition table ------------------------------
        struct AccountTransitions
        {
            auto operator()() const noexcept
            {
                using namespace boost::sml;

                auto has_funds = [](AccountContext& context, auto event) { return context.balance >= event.amount; };
                
                auto exceeds_debt = [](AccountContext& context, auto event) {
                    assert(context.balance < 0);
                    return event.amount >= std::fabs(context.balance);
                };

                return make_transition_table(
                    // clang-format off
				   *"normal"_s + event<Deposit> / [](AccountContext& context, auto event) { context.balance += event.amount;  } = "normal"_s,
					"normal"_s + event<Withdraw> [has_funds] / [](AccountContext& context, auto event) { context.balance -= event.amount;  } = "normal"_s,
					"normal"_s + event<Withdraw> [!has_funds] / [](AccountContext& context, auto event) { context.balance -= event.amount;  } = "overdraft"_s,
					"normal"_s + event<PayInterest> / [](AccountContext& context) { context.balance *= 1.05; } = "normal"_s,
 					"overdraft"_s + event<Deposit>[exceeds_debt] / [](AccountContext& context, auto event) { context.balance += event.amount;  } = "normal"_s,
					"overdraft"_s + event<Deposit>[!exceeds_debt] / [](AccountContext& context, auto event) { context.balance += event.amount;  } = "overdraft"_s,
					"overdraft"_s + event<Withdraw> / 
						[](AccountContext& context) { 
							auto msg = "Insufficient funds for account #" + std::to_string(context.id);
							throw InsufficientFunds{msg, context.id};  
						},
					"overdraft"_s + event<PayInterest> / [](AccountContext& context) { context.balance *= 1.15; } = "overdraft"_s
                    // clang-format on
                    );
            }
        };

        class BankAccount
        {
            AccountContext context_;
            boost::sml::sm<AccountTransitions> sm_;

        public:
            BankAccount(int id) : context_{ id, 0.0 }, sm_{ context_ }
            {
            }

            void deposit(double amount)
            {
                sm_.process_event(Deposit{ amount });
            }

            void withdraw(double amount)
            {
                sm_.process_event(Withdraw{ amount });
            }

            void pay_interest()
            {
                sm_.process_event(PayInterest{});
            }

            std::string status()
            {
                std::stringstream strm;
                strm << "BankAccount #" << context_.id << "; ";
                strm << "State: ";

                sm_.visit_current_states([&strm](auto state) { strm << state.c_str(); });

                strm << "; Balance: " << std::to_string(context_.balance);

                return strm.str();
            }

            double balance() const
            {
                return context_.balance;
            }

            int id() const
            {
                return context_.id;
            }
        };
    }
}

#endif
