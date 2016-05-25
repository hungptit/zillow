classdef HouseFactTable < handle
    properties (Constant, Access = public)
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
    end
    
    methods (Access = public)
        function this = HouseFactTable(dataFile)
            this.DBConnection = sqlite3.Connection(dataFile);
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
        end
        
        function crawl(this, houseInfo)            
            query = zillow.DeepSearchResults;
            this.DBConnection.exec('BEGIN');
            for idx = 1:numel(houseInfo)
                anAddress = houseInfo(idx);
                crawlData = query.exec(anAddress.Street, anAddress.City, anAddress.State);
                this.add(crawlData);
            end
            this.DBConnection.exec('COMMIT');
        end
        
        function add(this, data)
            cmd = sprintf('INSERT INTO %s VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)', this.TableName);
            results = this.DBConnection.exec('select id from HouseFactTable where zpid = ?', data.zpid);
            if isempty(results)
                this.DBConnection.exec(cmd, ...
                                       data.zpid, ...
                                       data.Bedrooms, ...                                       
                                       data.UseCode, ...
                                       data.YearBuilt, ...
                                       data.FinishedSqFt, ...
                                       data.Bathrooms, ...
                                       data.Bedrooms, ...
                                       data.TotalRooms);
            else
                
            end
        end
        
        function id = getStateId(this, shortStateName)
            results = this.DBConnection.exec('select id from States where ShortName = ?', shortStateName);
            assert(~isempty(results), 'Could not find state: %s\n', shortStateName);
            id = results.id;
        end

        function id = getCityId(this, cityName, stateId)
            cityName = lower(cityName);
            results = this.DBConnection.exec('select id from Cities where Name = ? and SID = ?', cityName, stateId);
            assert(~isempty(results), 'Could not find city: %s\n', cityName);
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
            this.DBConnection.exec('INSERT INTO States VALUES (?, ?, ?)', 1, 'Massachusetts', 'MA');
            this.DBConnection.exec('INSERT INTO States VALUES (?, ?, ?)', 2, 'Connecticut', 'CT');
            this.DBConnection.exec('COMMIT');
        end
        
        function createCityTable(this)
            this.DBConnection.exec('BEGIN');
            this.DBConnection.exec('CREATE TABLE Cities (ID INTEGER PRIMARY KEY AUTOINCREMENT, SID INTEGER, Name varchar(128))');
            this.DBConnection.exec('INSERT INTO Cities VALUES (?, ?, ?)', 1, 1, 'needham');
            this.DBConnection.exec('INSERT INTO Cities VALUES (?, ?, ?)', 2, 1, 'springfield');
            this.DBConnection.exec('INSERT INTO Cities VALUES (?, ?, ?)', 3, 2, 'enfield');
            this.DBConnection.exec('COMMIT');
        end
    end
end
