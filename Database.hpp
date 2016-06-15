#ifndef Database_hpp
#define Database_hpp

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Zillow.hpp"
#include "utils/LevelDBIO.hpp"

#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Session.h"

namespace zillow {
    void writeTextFile(const std::stringstream &output,
                       const std::string &dataFile) {
        std::ofstream outputFile;
        outputFile.open(dataFile);
        outputFile << output.str() << "\n";
    }

    void writeToSQLite(const std::string &dataFile,
                       const std::vector<DeepSearchResults> &vertexes,
                       const std::vector<EdgeData> &edges) {
        using namespace Poco::Data::Keywords;
        using Poco::Data::Session;
        using Poco::Data::Statement;

        Poco::Data::SQLite::Connector::registerConnector();
        Session session("SQLite", dataFile);

        // Create HouseInfo, SaleRecord, ZEstimate, and Edges tables.
        session << "CREATE TABLE IF NOT EXISTS House (BIGINT zpid NOT NULL "
                   "UNIQUE, street VARCHAR(128), city "
                   "VARCHAR(64) NOT NULL, state VARCHAR(3) NOT NULL)",
            now;
        session << "CREATE TABLE IF NOT EXISTS SaleRecord (BIGINT zpid NOT "
                   "NULL UNIQUE, lastSoldDate "
                   "VARCHAR(128), LastSoldPrice REAL)",
            now;

        // Now insert data to the database.
        Statement insert(session);
        insert<< "BEGIN";
        // for (auto const &item : vertexes) {
        //     session << "INSERT INTO House VALUE(?, ?, ?, ?)" << use(item.zpid)
        //             << use(std::get<STREET>(item.address))
        //             << use(std::get<CITY>(item.address))
        //             << use(std::get<STATE>(item.address));
                           
        // }
        session << "END";
        insert.execute();
    }

    // void writeToNoSQLDatabase(const std::string &dataFile,
    //                           std::vector<DeepSearchResults> &vertexes,
    //                           std::vector<EdgeData> &edges) {}
}

#endif
