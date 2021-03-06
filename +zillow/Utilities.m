classdef Utilities < handle
    properties (Constant)
        FactHeaders = { ...
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
        
        InfoHeaders = {...
            'ID', 'INTEGER PRIMARY KEY AUTOINCREMENT', ...
            'zpid', 'int NOT NULL', ...
            'TaxAssessment', 'real NOT NULL', ...
            'TaxAssessmentYear', 'int NOT NULL', ...
            'LastSoldDate', 'datetime NOT NULL', ...
            'LastSoldPrice', 'real NOT NULL', ...
            'ZEstimate', 'real NOT NULL', ...
            'HomeDescriptions', 'varchar(4096)', ...
            'PageViewCountThisMonth', 'int', ...
            'PageViewCountTotal', 'int', ...
            'QueryDate', 'datetime NOT NULL'}
        
        FactTableName = 'HouseFact';
        InfoTableName = 'HouseInfo';
        StateTableName = 'States';
        CityTableName = 'Cities';

        DeepSearchResultsHeaders = {...
            'zpid', 'Link', 'Street', 'City', 'State', ...
            'ZipCode', 'Latitude', 'Longitude', 'UseCode', ...
            'YearBuilt', 'LotSizeSqFt', 'FinishedSqFt', ...
            'Bathrooms', 'Bedrooms', 'TotalRooms', ...            
            'TaxAssessment', 'TaxAssessmentYear', 'LastSoldDate', ...
            'LastSoldPrice', 'ZEstimate'}';
    end
    
    methods (Static, Access = public)        
        function write(fileName, xmlData)
            fid = fopen(fileName, 'wt');
            if (fid < 0)
                error('Could not open "%s" file to write\n', fileName);
            end
            fprintf(fid, '%s\n', xmlData);
            fclose(fid);
        end
        
        function info = extractData(results)
            if isfield(results, 'lastSoldDate')
                % Use datenum to make sure that we can save date to
                % database correctly.
                lastSoldDate = datenum(results.lastSoldDate.Text);
                lastSoldPrice = str2double(results.lastSoldPrice.Text);
            else
                lastSoldDate = 0;
                lastSoldPrice = 0;
            end

            if isfield(results, 'bathrooms')
                bathrooms = str2double(results.bathrooms.Text);
            else
                bathrooms = 0;
            end

            if isfield(results, 'bedrooms')
                bedrooms = str2double(results.bedrooms.Text);
            else
                bedrooms = 0;
            end

            
            if isfield(results, 'totalRooms')
                totalRooms = str2double(results.totalRooms.Text);
            else
                totalRooms = 0;
            end

            if isfield(results, 'taxAssessment')
                taxAssessment = str2double(results.taxAssessment.Text);
                taxAssessmentYear = str2double(results.taxAssessmentYear.Text);
            else
                taxAssessment = 0;
                taxAssessmentYear = 0;
            end

            if isfield(results, 'lotSizeSqFt')
                lotSizeSqFt = str2double(results.lotSizeSqFt.Text);
            else
                lotSizeSqFt = 0;
            end

            if isfield(results, 'finishedSqFt')
                finishedSqFt = str2double(results.finishedSqFt.Text);
            else
                finishedSqFt = 0;
            end
            
            if isfield(results, 'useCode')
                useCode = results.useCode.Text;
            else
                useCode = '';
            end

            zestimate = str2double(results.zestimate.amount.Text);
            if isnan(zestimate) 
                zestimate = 0;
                warning('Could not get zestimate for %s, %s %s', ...
                        results.address.street.Text, ...
                        results.address.city.Text, ...
                        results.address.state.Text);
            end
            
            info = struct(...
                'zpid', str2double(results.zpid.Text), ...
                'Link', results.links.homedetails.Text, ...
                'Street', results.address.street.Text, ...
                'City', results.address.city.Text, ...
                'State', results.address.state.Text, ...
                'ZipCode', results.address.zipcode.Text, ...
                'Latitude', str2double(results.address.latitude.Text), ...
                'Longitude', str2double(results.address.longitude.Text), ...
                'UseCode', useCode, ...
                'YearBuilt', str2double(results.yearBuilt.Text), ...
                'LotSizeSqFt', lotSizeSqFt, ...
                'FinishedSqFt', finishedSqFt, ...
                'Bathrooms', bathrooms, ...
                'Bedrooms', bedrooms, ...
                'TotalRooms', totalRooms, ...
                'TaxAssessment', taxAssessment, ...
                'TaxAssessmentYear', taxAssessmentYear, ...
                'LastSoldDate', lastSoldDate, ...
                'LastSoldPrice', lastSoldPrice, ...
                'ZEstimate', zestimate);
        end

        function createAllHousesView(this)
            sqlCmd = '';
        end
        
        function dispCompResults(results)
            for idx = 1:numel(results.Comps)
                item = results.Comps(idx);
                fprintf('House %d: %s, %s, %s -> zestimate: %d\n', ...
                        idx, item.Street, item.City, item.State, item.ZEstimate);
            end
        end
        
        function results = readAddressFromXLSFile(xlsFile)
            [~, ~, raw] = xlsread(xlsFile);
            [M, N] = size(raw);
            assert(N > 2, 'We assume that there are at least 3 columns which are Street, City, and State.\n');
            assert(strcmpi(raw{1, 1}, 'street'), 'The first column title must be "Street".');
            assert(strcmpi(raw{1, 2}, 'city'), 'The first column title must be "City".');
            assert(strcmpi(raw{1, 3}, 'state'), 'The first column title must be "State".');
            results = {};
            for idx = 2:M
                results{end + 1} = struct('street', raw{idx, 1}, 'city', raw{idx, 2}, 'state', raw{idx, 3}); %#ok
            end
        end
        
        function writeHouseInfo(fid, fieldNames, data, sepChar)
            N = numel(fieldNames);
            for idx = 1:N
                zillow.Utilities.writeValue(fid, data.(fieldNames{idx}), sepChar);
            end
            fprintf(fid, '\n');
        end                
        
        function writeValue(fid, val, sepChar)
            if isempty(val)
                fprintf(fid, '%s', sepChar);
            elseif ischar(val)
                fprintf(fid, '%s%s', val, sepChar);
            elseif isnumeric(val)
                fprintf(fid, '%g%s', val, sepChar);
            else
                assert(false, 'Unexpected data type: %s\n', class(val));
            end
        end
        
        % Write struct information to CSV. 
        function writeDeepCompResults(similarHouses, dataFile)
            fieldNames = fieldnames(similarHouses.Principal);
            N = numel(fieldNames);
            fid = fopen(dataFile, 'wt');
            if fid < 0
                error('Could not open file "%s" to write.\n');
            end
            
            % Write header
            for idx = 1:N
                fprintf(fid, '%s,', fieldNames{idx});
            end
            fprintf(fid, '\n');
            
            % Write principal data
            zillow.Utilities.writeHouseInfo(fid, fieldNames, similarHouses.Principal, ',');
            
            % Write deep comparison data
            for idx = 1:numel(similarHouses.Comps)
                zillow.Utilities.writeHouseInfo(fid, fieldNames, similarHouses.Comps(idx), ',');
            end
            
            % Close the output file.
            fclose(fid);
        end
    end
end
