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
        // clang-format off
        session << "CREATE TABLE IF NOT EXISTS House (zpid BIGINT NOT NULL UNIQUE, Street VARCHAR(128), City VARCHAR(64) NOT NULL, State VARCHAR(3) NOT NULL)",
            now;

        session << "CREATE TABLE IF NOT EXISTS Tax (BIGINT zpid NOT NULL UNIQUE, TaxAssessmentYear INT, TaxAssessment REAL)",
            now;

        session << "CREATE TABLE  IF NOT EXISTS Edges (SrcId BIGINT NOT NULL, DstId BIGINT NOT NULL, Score REAL NOT NULL)",
            now;

        // clang-format on
        // Update House table.
        {}
        // // Update SaleRecord table
        // {
        //     session
        //         << "CREATE TABLE IF NOT EXISTS SaleRecord (BIGINT zpid NOT "
        //            "NULL UNIQUE, lastSoldDate "
        //            "VARCHAR(128), LastSoldPrice REAL, Currency VARCHAR(16))",
        //         now;
        //     Statement insert(session);

        //     // Get an std::vector of Poco::Tuple
        //     using SaleRecordRow =
        //         Poco::Tuple<IDType, std::string, double, std::string>;
        //     std::vector<SaleRecordRow> data;
        //     data.reserve(vertexes.size());
        //     for (auto const &item : vertexes) {
        //         data.emplace_back(SaleRecordRow(
        //             item.zpid, item.saleRecord.LastSoldDate,
        //             item.saleRecord.LastSoldPrice,
        //             item.saleRecord.Currency));
        //     }
        //     insert << "INSERT INTO SaleRecord VALUES (?, ?, ?, ?)",
        //     use(data);
        //     insert.execute();
        // }

        // // Update ZEstimate table
        // {
        //     session
        //         << "CREATE TABLE IF NOT EXISTS ZEstimate (BIGINT zpid NOT "
        //            "NULL UNIQUE, TaxAssessmentYear INT, TaxAssessment REAL)",
        //         now;
        //     Statement insert(session);

        //     // Get an std::vector of Poco::Tuple
        //     using EdgesRow = Poco::Tuple<IDType, IDType, double>;
        //     std::vector<EdgesRow> data;
        //     data.reserve(edges.size());
        //     for (auto const &item : vertexes) {
        //         data.emplace_back(EdgesRow(item.zpid,
        //                                    item.tax.TaxAssessmentYear,
        //                                    item.tax.TaxAssessment));
        //     }
        //     insert << "INSERT INTO Tax VALUES (?, ?, ?)", use(data);
        //     insert.execute();
        // }

        // // Update Tax table
        // {
        //     Statement insert(session);

        //     // Get an std::vector of Poco::Tuple
        //     using EdgesRow = Poco::Tuple<IDType, IDType, double>;
        //     std::vector<EdgesRow> data;
        //     data.reserve(edges.size());
        //     for (auto const &item : vertexes) {
        //         data.emplace_back(EdgesRow(item.zpid,
        //                                    item.tax.TaxAssessmentYear,
        //                                    item.tax.TaxAssessment));
        //     }
        //     insert << "INSERT INTO Tax VALUES (?, ?, ?)", use(data);
        //     insert.execute();
        // }

        // Update Edges table
        {
            // Get an std::vector of Poco::Tuple
            using EdgesRow = Poco::Tuple<IDType, IDType, double>;
            std::vector<EdgesRow> data;
            data.reserve(edges.size());
            for (auto const &item : edges) {
                data.emplace_back(EdgesRow(item.SrcID, item.DstID,
                item.Score));
            }
            // Insert edge information into the Edges table

            // Statement insert(session);
            // insert << "INSERT INTO Edges VALUES (?, ?, ?)", use(data);
            // insert.execute();
        }
    }

    // void writeToNoSQLDatabase(const std::string &dataFile,
    //                           std::vector<DeepSearchResults> &vertexes,
    //                           std::vector<EdgeData> &edges) {}
}

#endif
