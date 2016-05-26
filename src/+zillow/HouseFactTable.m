classdef HouseFactTable < handle
    properties (Hidden, Constant, Access = public)
        Headers = { ...
            'ID', 'INTEGER PRIMARY KEY AUTOINCREMENT', ...
            'zpid', 'int NOT NULL UNIQUE', ...
            'Link', 'varchar(512) NOT NULL UNIQUE', ...
            'Street', 'varchar(256)  NOT NULL', ...
            'CityID', 'int  NOT NULL', ...
            'ZipCode', 'int  NOT NULL', ...
            'Latitude', 'real NOT NULL' , ...
            'Longitude', 'real NOT NULL', ...
            'UseCode', 'varchar(64)', ...
            'YearBuilt', 'int NOT NULL', ...
            'LotSizeSqFt', 'int NOT NULL', ...
            'FinishedSqFt', 'int NOT NULL', ...
            'Bathrooms', 'int NOT NULL', ...
            'Bedrooms', 'int NOT NULL', ...
            'Totalrooms', 'int NOT NULL'};
        TableName = 'HouseFactTable';
        StateTable = 'States';
        CityTable = 'Cities';
    end
    
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
                % Create a new database file.
                this.DBConnection = sqlite3.Connection(dataFile);
            end
            
            tableNames = this.DBConnection.exec('select name from sqlite_master WHERE type="table"');
            if ~any(arrayfun(@(item) strcmp(item.name, this.TableName), tableNames))
                this.createTable;
            end
            
            if ~any(arrayfun(@(item) strcmp(item.name, this.StateTable), tableNames))
                this.createStateTable;
            end
        
            if ~any(arrayfun(@(item) strcmp(item.name, this.CityTable), tableNames))
                this.createCityTable;
            end
            
            this.updateStateData;
            this.updateCityData;
        end
        
        function crawl(this, houseInfo)            
            query = zillow.DeepSearchResults;
            for idx = 1:numel(houseInfo)
                anAddress = houseInfo{idx};
                crawlData = query.exec(anAddress.street, anAddress.city, anAddress.state);
                if ~isempty(crawlData)
                    this.write(crawlData);
                end
            end            
        end
        
        % Read the whole table into memory
        function results = read(this)
            results = this.DBConnection.exec(sprintf('SELECT * FROM %s', this.TableName));
        end
        
        % Write house information to database. This methods will update the house fact
        % data if there is any change. And it also add the house information to the HouseInfoTable.
        function write(this, data)
            cityId = this.getCityId(data.City, data.State);
            
            % Query the data from the database
            results = this.DBConnection.exec(sprintf('SELECT * FROM %s WHERE cityid=? AND street=?', this.TableName), ...
                                             cityId, data.Street);
            if isempty(results)                
                cmd = sprintf('INSERT INTO %s (zpid, link, street, cityid, zipcode, latitude, longitude, usecode, yearbuilt, lotsizesqft, finishedsqft, bathrooms, bedrooms, totalrooms) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)', this.TableName);
                this.DBConnection.exec(cmd, data.zpid, data.Link, data.Street, cityId, data.ZipCode, data.Latitude, data.Longitude, ...
                                       data.UseCode, data.YearBuilt, data.LotSizeSqFt, data.FinishedSqFt, data.Bathrooms, data.Bedrooms, data.TotalRooms);
            else
                % TODO: If the query data and crawl data are different then we need to update the results.                
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
        
        function createTable(this)
            N = numel(this.Headers) / 2;
            queryCmd = sprintf('CREATE TABLE %s (%s %s', this.TableName, this.Headers{1}, this.Headers{2});
            for colIdx = 2:N
                colName = this.Headers{2 * colIdx - 1};
                colDataType = this.Headers{2 * colIdx};
                queryCmd = sprintf('%s, %s %s', queryCmd, colName, colDataType);
            end
            
            queryCmd = sprintf('%s);', queryCmd);
            this.DBConnection.exec(queryCmd);
        end
        
        function createStateTable(this)
            this.DBConnection.exec('BEGIN');
            this.DBConnection.exec('CREATE TABLE States (ID INTEGER PRIMARY KEY AUTOINCREMENT, Name varchar(64) NOT NULL UNIQUE, ShortName char(2) NOT NULL UNIQUE)');
            this.DBConnection.exec('INSERT INTO States VALUES (?, ?, ?)', 1, 'massachusetts', 'MA');
            this.DBConnection.exec('INSERT INTO States VALUES (?, ?, ?)', 2, 'connecticut', 'CT');
            this.DBConnection.exec('COMMIT');
        end
        
        function createCityTable(this)
            this.DBConnection.exec('BEGIN');
            this.DBConnection.exec('CREATE TABLE Cities (ID INTEGER PRIMARY KEY AUTOINCREMENT, SID INTEGER, Name varchar(128))');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'needham');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 1, 'springfield');
            this.DBConnection.exec('INSERT INTO Cities (SID, Name) VALUES (?, ?)', 2, 'enfield');
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
