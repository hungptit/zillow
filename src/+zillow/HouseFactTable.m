classdef HouseFactTable < handle   
    properties (SetAccess = private, GetAccess = public)
        DBConnection
        Cities
        States
    end
    
    methods (Access = public)
        function this = HouseFactTable(dataFile)
            if exist(dataFile, 'file')
                this.DBConnection = sqlite3.Connection(dataFile, 'ReadWrite');
            else
                this.DBConnection = sqlite3.Connection(dataFile, 'ReadWrite', 'Create');
            end
            
            % Create tables if they are not exist.
            this.createTable(zillow.Utilities.FactTableName, zillow.Utilities.FactHeaders);
            this.createTable(zillow.Utilities.InfoTableName, zillow.Utilities.InfoHeaders);
            
            tableNames = this.DBConnection.exec('select name from sqlite_master WHERE type="table"');
            if ~any(arrayfun(@(item) strcmp(item.name, zillow.Utilities.StateTableName), tableNames))
                this.createStateTable;
            end
            
            if ~any(arrayfun(@(item) strcmp(item.name, zillow.Utilities.CityTableName), tableNames))
                this.createCityTable;
            end
            
            this.updateStateData;
            this.updateCityData;
        end
        
        function crawl(this, houseInfo)            
            query = zillow.DeepSearchResults;
            updateQuery = zillow.UpdatedPropertyDetails;
            for idx = 1:numel(houseInfo)
                anAddress = houseInfo{idx};
                crawlData = query.exec(anAddress.street, anAddress.city, anAddress.state);
                if ~isempty(crawlData)
                    updateInfo = updateQuery.exec(crawlData.zpid);
                    this.write(crawlData, updateInfo);
                end
            end            
        end
        
        % Read the whole table into memory
        function results = read(this)
            results = this.DBConnection.exec(sprintf('SELECT * FROM %s', zillow.Utilities.FactTableName));
        end
        
        % Write house information to database. This methods will update the house fact
        % data if there is any change. And it also add the house information to the HouseInfoTable.
        function write(this, data, updateInfo)
            cityId = this.getCityId(data.City, data.State);
            
            % Query the data from the database
            results = this.DBConnection.exec(sprintf('SELECT * FROM %s WHERE cityid=? AND street=?', zillow.Utilities.FactTableName), ...
                                             cityId, data.Street);
            if isempty(results)                
                cmd = sprintf('INSERT INTO %s (zpid, link, street, cityid, zipcode, latitude, longitude, usecode, yearbuilt, lotsizesqft, finishedsqft, bathrooms, bedrooms, totalrooms) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)', zillow.Utilities.FactTableName);
                this.DBConnection.exec(cmd, data.zpid, data.Link, data.Street, cityId, data.ZipCode, data.Latitude, data.Longitude, ...
                                       data.UseCode, data.YearBuilt, data.LotSizeSqFt, data.FinishedSqFt, data.Bathrooms, data.Bedrooms, data.TotalRooms);
                                
            else
                % TODO: If the query data and crawl data are different then we need to update the results.                
            end
            
            % Add information to HouseInfo table
            dateNum = now;                                
            if ~isempty(updateInfo)
                infoCmd = sprintf('INSERT INTO %s (zpid, taxassessment, taxassessmentyear, lastsolddate, lastsoldprice, zestimate, homedescriptions, pageviewcountthismonth, pageviewcounttotal, querydate) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)', zillow.Utilities.InfoTableName);
                this.DBConnection.exec(infoCmd, data.zpid, data.TaxAssessment, data.TaxAssessmentYear, ...
                                       datenum2sql(data.LastSoldDate), data.LastSoldPrice, data.ZEstimate, ...
                                       updateInfo.HomeDescription, updateInfo.PageViewCountThisMonth, updateInfo.PageViewCountTotal, datenum2sql(dateNum));
            else
                infoCmd = sprintf('INSERT INTO %s (zpid, taxassessment, taxassessmentyear, lastsolddate, lastsoldprice, zestimate, querydate) VALUES (?, ?, ?, ?, ?, ?, ?)', zillow.Utilities.InfoTableName);
                this.DBConnection.exec(infoCmd, data.zpid, data.TaxAssessment, data.TaxAssessmentYear, ...
                                       datenum2sql(data.LastSoldDate), data.LastSoldPrice, data.ZEstimate, datenum2sql(dateNum));
            end
        end
    end
    
    % Utility methods
    methods (Access = public)    
        function id = getStateId(this, stateName)
            results = this.States(arrayfun(@(aState) strcmpi(aState.shortname, stateName), this.States));
            assert(~isempty(results), 'Could not find state "%s"\n', stateName);
            id = results.id;
        end

        function id = getCityId(this, cityName, stateName)
            stateId = this.getStateId(stateName);
            results = this.Cities(arrayfun(@(aCity) ((aCity.sid == stateId) && strcmpi(aCity.name, cityName)), this.Cities));
            assert(~isempty(results), 'Could not find city "%s" in state "%s"\n', cityName, stateName);
            id = results.id;
        end
        
        function createTable(this, tableName, headers)
            N = numel(headers) / 2;
            queryCmd = sprintf('CREATE TABLE IF NOT EXISTS %s (%s %s', tableName, headers{1}, headers{2});
            for colIdx = 2:N
                colName = headers{2 * colIdx - 1};
                colDataType = headers{2 * colIdx};
                queryCmd = sprintf('%s, %s %s', queryCmd, colName, colDataType);
            end
            
            queryCmd = sprintf('%s);', queryCmd);
            this.DBConnection.exec(queryCmd);
        end
        
        function createStateTable(this)
            this.DBConnection.exec('BEGIN');
            this.DBConnection.exec('CREATE TABLE IF NOT EXISTS States (ID INTEGER PRIMARY KEY AUTOINCREMENT, Name varchar(64) NOT NULL UNIQUE, ShortName char(2) NOT NULL UNIQUE)');
            this.DBConnection.exec('INSERT INTO States VALUES (?, ?, ?)', 1, 'massachusetts', 'MA');
            this.DBConnection.exec('INSERT INTO States VALUES (?, ?, ?)', 2, 'connecticut', 'CT');
            this.DBConnection.exec('COMMIT');
        end
        
        function createCityTable(this)
            this.DBConnection.exec('BEGIN');
            
            this.DBConnection.exec('CREATE TABLE  IF NOT EXISTS Cities (ID INTEGER PRIMARY KEY AUTOINCREMENT, SID INTEGER, Name varchar(128))');

            % Massachusetts
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Needham');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'West Springfield');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Springfield');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'East Longmeadow');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Longmeadow');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Chicopee');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Holyoke');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Agawam');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Wilbraham');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Westfield');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'East Otis');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Otis');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Three Rivers');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Three Rivers');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'South Hardley');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Amherst');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Russel');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Ludlow');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Ware');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'Indian Orchard');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'West Hampden');
            
            % Connecticut
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 2, 'Enfield');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 2, 'Hartford');
            
            this.DBConnection.exec('COMMIT');
        end
        
        function addCity(this, cityName, stateName)
            results = this.States(arrayfun(@(aState) strcmpi(aState.name, stateName), this.States));
            assert(~isempty(results), 'Could not find state "%s" in the database.\n', stateName);
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', results.id, cityName);
            this.updateCityData;
        end
        
        function addState(this, name, shortName)
            if ~any(arrayfun(@(aState) strcmpi(aState.name, name), this.States))
                this.DBConnection.exec('INSERT INTO States (Name, ShortName) VALUES (?, ?)', name, shortName);
                this.DBConnection.exec('select ID from States where Name=?', name);
                this.updateStateData;
            end
        end
        
        function updateCityData(this)
            this.Cities = this.DBConnection.exec('select * from Cities');
        end
        
        function updateStateData(this)
            this.States = this.DBConnection.exec('select * from States');
        end
    end
end
