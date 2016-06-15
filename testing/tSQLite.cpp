#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Session.h"
#include "fmt/format.h"
#include <iostream>
#include <vector>

// Cereal
#include "cereal/archives/binary.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/archives/xml.hpp"
#include "cereal/types/array.hpp"
#include "cereal/types/bitset.hpp"
#include "cereal/types/chrono.hpp"
#include "cereal/types/complex.hpp"
#include "cereal/types/deque.hpp"
#include "cereal/types/forward_list.hpp"
#include "cereal/types/queue.hpp"
#include "cereal/types/set.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/tuple.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/unordered_set.hpp"
#include "cereal/types/utility.hpp"
#include "cereal/types/valarray.hpp"
#include "cereal/types/vector.hpp"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace {
    struct Person {
        explicit Person() : name(), address(), age() {}
        explicit Person(const std::string &na, const std::string &ad,
                        const int ag)
            : name(na), address(ad), age(ag) {}

        std::string name;
        std::string address;
        int age;

        template <typename Archive> void serialize(Archive &ar) {
            ar(cereal::make_nvp("name", name),
               cereal::make_nvp("address", address),
               cereal::make_nvp("age", age));
        }
    };

    using A = std::tuple<std::string, std::string, std::string, int>;
    using B = std::tuple<std::string, std::string>;
    using C = std::tuple<std::string, int>;


    template <typename T> void print(const T &data);
    template <> void print(const A &a) {
        fmt::print("[{0}, {1}, {2}, {3}]\n", std::get<0>(a), std::get<1>(a),
                   std::get<2>(a), std::get<3>(a));
    }

    template <> void print(const B &data) {
        fmt::print("[{0}, {1}]\n", std::get<0>(data), std::get<1>(data));
    }

    template <> void print(const C &data) {
        fmt::print("[{0}, {1}]\n", std::get<0>(data), std::get<1>(data));
    }

    template <> void print(const Person &data) {
        fmt::print("[{0}, {1}, {2}]\n", data.name, data.address, data.age);
    }
}
int main(int argc, char **argv) {
    // register SQLite connector
    Poco::Data::SQLite::Connector::registerConnector();

    // create a session
    Session session("SQLite", "sample.db");

    // drop sample table, if it exists
    session << "DROP TABLE IF EXISTS Person", now;

    // (re)create table
    session << "CREATE TABLE Person (Name VARCHAR(30), Address VARCHAR, Age "
               "INTEGER(3))",
        now;

    // insert some rows
    Person person;

    // Test tuple data
    {
        using User = std::tuple<std::string, std::string, int>;
        std::vector<User> readers;
        readers.push_back(std::make_tuple("Test", "Tuple", 1));
        readers.push_back(std::make_tuple("Boo", "Tuple", 1));
        readers.push_back(std::make_tuple("Lisa Simpson", "Springfield", 1));
        readers.push_back(std::make_tuple("Boo", "Tuple", 1));

        {
            Statement insert_tuple(session);
            using TransUser = Poco::Tuple<std::string, std::string, int>;
            std::vector<TransUser> data;

            for (auto const &item : readers) {
                data.emplace_back(TransUser(
                    std::get<0>(item), std::get<1>(item), std::get<2>(item)));
            }
            insert_tuple << "INSERT INTO Person VALUES(?, ?, ?)", use(data);
            insert_tuple.execute();
        }
    }

    // a simple query
    Statement select(session);
    select << "SELECT Name, Address, Age FROM Person", into(person.name),
        into(person.address), into(person.age),
        range(0, 1); //  iterate over result set one row at a time

    while (!select.done()) {
        select.execute();
        std::cout << person.name << " " << person.address << " " << person.age
                  << std::endl;
    }

    {
        A a = std::make_tuple("1", "2", "3", 4);
        B b("1", "2");
        C c("3", 4);

        print(a);
        print(b);
        print(c);

        A x = std::tuple_cat(b, c);
        print(x);

        // std::tie(b, c) = std::make_tuple("1", "2", "3", 4);
    }

    {
        std::vector<Person> data;
        data.emplace_back(Person("Foo", "Boo", 1));
        data.emplace_back(Person("Foo", "Moo", 1));

        fmt::print("Data: \n");
        for (auto const &item : data) {
            print(item);
        }

        std::ostringstream os;
        {
            // cereal::JSONOutputArchive ar(os);
            cereal::BinaryOutputArchive ar(os);
            ar(cereal::make_nvp("data", data));
        }
        fmt::print("{}\n", os.str());

        // Deserialize the data
        {
            std::vector<Person> results;
            std::istringstream is(os.str());
            {
                cereal::BinaryInputArchive ar(is);
                ar(results);
            }

            fmt::print("Results: \n");
            for (auto const &item : results) {
                print(item);
            }
        }
    }

    return 0;
}
