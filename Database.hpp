#ifndef Database_hpp
#define Database_hpp

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Zillow.hpp"
#include "utils/LevelDBIO.hpp"

#include "fmt/format.h"

#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Session.h"

namespace zillow {
    void writeTextFile(const std::stringstream &output, const std::string &dataFile) {
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

        // Return if either vertexes or edges are empty.
        if (vertexes.empty() || edges.empty()) {
            return;
        }

        /**
         * Create HouseInfo, SaleRecord, ZEstimate, Links, and Edges tables.
         *
         */

        fmt::print("Create tables\n");
        // (zpid, Street, ZipCode, City, State, Latitude, Longitude,
        // UseCode, YearBuilt, LotSizeSqFt, FinishedSqFt, Bathrooms,
        // Bedrooms, TotalRooms)
        session << "CREATE TABLE IF NOT EXISTS House (zpid BIGINT NOT NULL "
                   "UNIQUE, Street VARCHAR(128), ZipCode VARCHAR(10), City VARCHAR(32) "
                   "NOT NULL, State VARCHAR(3) NOT NULL, Latitude REAL, "
                   "Longitude REAL, "
                   "UseCode VARCHAR(31), YearBuilt INT, LotSizeSqFt REAL, "
                   "FinishedSqFt REAL, Bathrooms REAL, Bedrooms INT, "
                   "TotalRooms INT, TimeStamp DATETIME DEFAULT CURRENT_TIMESTAMP)",
            now;

        session << "CREATE TABLE IF NOT EXISTS Tax (zpid BIGINT NOT NULL, "
                   "TaxAssessmentYear INT, TaxAssessment REAL, TimeStamp "
                   "DATETIME DEFAULT CURRENT_TIMESTAMP)",
            now;

        session << "CREATE TABLE  IF NOT EXISTS Edges (SrcId BIGINT NOT NULL, "
                   "DstId BIGINT NOT NULL, Score REAL NOT NULL, TimeStamp "
                   "DATETIME DEFAULT CURRENT_TIMESTAMP)",
            now;

        session << "CREATE TABLE IF NOT EXISTS SaleRecord (zpid BIGINT NOT "
                   "NULL, lastSoldDate DATE, LastSoldPrice REAL, Currency "
                   "VARCHAR(5), TimeStamp DATETIME DEFAULT CURRENT_TIMESTAMP)",
            now;

        session << "CREATE TABLE IF NOT EXISTS ZEstimate (zpid BIGINT NOT "
                   "NULL, Amount REAL, Low REAL, High REAL, Currency "
                   "VARCHAR(5), LastUpdated DATE, TimeStamp DATETIME DEFAULT "
                   "CURRENT_TIMESTAMP)",
            now;

        session << "CREATE TABLE IF NOT EXISTS Links (zpid BIGINT NOT "
                   "NULL, HomeDetails VARCHAR(255), GraphAndData VARCHAR(255), "
                   "MapThisHome VARCHAR(255), Comparables VARCHAR(255))",
            now;

        // Update House table.
        {
            fmt::print("Update House table\n");
            Statement insert(session);

            // Get an std::vector of Poco::Tuple
            // (zpid, Street, ZipCode, City, State, Latitude, Longitude,
            // UseCode, YearBuilt, LotSizeSqFt, FinishedSqFt, Bathrooms,
            // Bedrooms, TotalRooms)
            using Row = Poco::Tuple<IDType, std::string, std::string, std::string, std::string,
                                    Real, Real, std::string, int, Real, Real, Real, int, int>;
            std::vector<Row> data;
            data.reserve(vertexes.size());
            for (auto const &item : vertexes) {
                data.emplace_back(
                    Row(item.zpid, item.info.HouseAddress.Street,
                        item.info.HouseAddress.ZipCode, item.info.HouseAddress.City,
                        item.info.HouseAddress.State, item.info.HouseAddress.Latitude,
                        item.info.HouseAddress.Longitude, item.info.UseCode,
                        item.info.YearBuilt, item.info.LotSizeSqFt, item.info.FinishedSqFt,
                        item.info.Bathrooms, item.info.Bedrooms, item.info.TotalRooms));
            }
            insert << "INSERT INTO House (zpid, Street, ZipCode, City, "
                      "State, Latitude, Longitude, UseCode, YearBuilt, "
                      "LotSizeSqFt, FinishedSqFt, Bathrooms, Bedrooms, "
                      "TotalRooms) VALUES (?, ?, ?, ?, ?, ?, ?, ?, "
                      "?, ?, ?, ?, ?, ?)",
                use(data);
            insert.execute();
        }

        // Update SaleRecord table
        {
            fmt::print("Create SaleRecord table\n");
            Statement insert(session);

            // Get an std::vector of Poco::Tuple
            using Row = Poco::Tuple<IDType, std::string, Real, std::string>;
            std::vector<Row> data;
            data.reserve(vertexes.size());
            for (auto const &item : vertexes) {
                data.emplace_back(Row(item.zpid, item.saleRecord.LastSoldDate,
                                      item.saleRecord.LastSoldPrice, item.saleRecord.Currency));
            }
            insert << "INSERT INTO SaleRecord (zpid, LastSoldDate, "
                      "LastSoldPrice, Currency) VALUES (?, ?, ?, ?)",
                use(data);
            insert.execute();
        }

        // Update ZEstimate table
        {
            fmt::print("Update ZEstimate table\n");
            Statement insert(session);

            // Get an std::vector of Poco::Tuple
            using Row = Poco::Tuple<IDType, Real, Real, Real, std::string, std::string>;
            std::vector<Row> data;
            data.reserve(edges.size());
            for (auto const &item : vertexes) {
                data.emplace_back(Row(item.zpid, item.zestimate.Amount, item.zestimate.Low,
                                      item.zestimate.High, item.zestimate.Currency,
                                      item.zestimate.LastUpdated));
            }
            insert << "INSERT INTO ZEstimate (zpid, Amount, Low, High, "
                      "Currency, LastUpdated) VALUES (?, ?, ?, ?, ?, ?)",
                use(data);
            insert.execute();
        }

        // Update Tax table
        {
            fmt::print("Update Tax table\n");
            using Row = Poco::Tuple<IDType, int, double>;
            std::vector<Row> data;
            data.reserve(vertexes.size());
            for (auto const &item : vertexes) {
                data.emplace_back(
                    Row(item.zpid, item.tax.TaxAssessmentYear, item.tax.TaxAssessment));
            }

            Statement insert(session);
            insert << "INSERT INTO Tax (zpid, TaxAssessmentYear, "
                      "TaxAssessment) VALUES (?, ?, ?)",
                use(data);
            insert.execute();
        }

        // Update Edges table
        {
            fmt::print("Update Edges table\n");
            using Row = Poco::Tuple<IDType, IDType, double>;
            std::vector<Row> data;
            data.reserve(edges.size());
            for (auto const &item : edges) {
                data.emplace_back(Row(item.SrcID, item.DstID, item.Score));
            }

            // Insert edge information into the Edges table
            Statement insert(session);
            insert << "INSERT INTO Edges (SrcId, DstId, Score) VALUES (?, ?, ?)", use(data);
            insert.execute();
        }

        // Update Links table
        {
            fmt::print("Create Links table\n");
            using Row = Poco::Tuple<IDType, std::string, std::string, std::string, std::string>;
            std::vector<Row> data;
            data.reserve(vertexes.size());
            for (auto const &item : vertexes) {
                data.emplace_back(Row(item.zpid, item.links.HomeDetails,
                                      item.links.GraphAndData, item.links.MapThisHome,
                                      item.links.Comparables));
            }

            Statement insert(session);
            insert << "INSERT INTO Links (zpid, HomeDetails, GraphAndData, "
                      "MapThisHome, Comparables) VALUES (?, ?, ?, ?, ?)",
                use(data);
            insert.execute();
        }

        // Create views
        {
            fmt::print("Create all views\n");
            {
                Statement views(session);
                views << "CREATE VIEW DETAILS AS SELECT House.Street, "
                         "House.City, House.State, House.YearBuilt, "
                         "House.LotSizeSqFt, House.FinishedSqFt, "
                         "House.Bathrooms, "
                         "House.Bedrooms, House.TotalRooms, "
                         "SaleRecord.LastSoldDate, SaleRecord.LastSoldPrice, "
                         "Tax.TaxAssessmentYear, Tax.TaxAssessment, "
                         " Links.HomeDetails FROM "
                         "House, Links, SaleRecord, "
                         "Tax, "
                         "ZEstimate WHERE House.zpid == Links.zpid AND "
                         "House.zpid "
                         "== SaleRecord.zpid AND "
                         "House.zpid == Tax.zpid AND House.zpid == "
                         "ZEstimate.zpid";

                views.execute();
            }

            {
                Statement views(session);
                views << "CREATE VIEW Brief AS SELECT House.Street, "
                         "House.City, House.State, House.YearBuilt, "
                         "House.LotSizeSqFt, House.FinishedSqFt, "
                         "House.Bathrooms, "
                         "House.Bedrooms, House.TotalRooms, "
                         "SaleRecord.LastSoldDate, SaleRecord.LastSoldPrice, "
                         "Tax.TaxAssessmentYear, Tax.TaxAssessment "
                         " FROM "
                         "House, Links, SaleRecord, "
                         "Tax, "
                         "ZEstimate WHERE House.zpid == Links.zpid AND "
                         "House.zpid "
                         "== SaleRecord.zpid AND "
                         "House.zpid == Tax.zpid AND House.zpid == "
                         "ZEstimate.zpid";

                views.execute();
            }
        }
    }

    // void writeToNoSQLDatabase(const std::string &dataFile,
    //                           std::vector<DeepSearchResults> &vertexes,
    //                           std::vector<EdgeData> &edges) {}
}

#endif
