classdef Utilities < handle
    properties (Constant)
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
                lastSoldDate = results.lastSoldDate.Text;
                lastSoldPrice = str2double(results.lastSoldPrice.Text);
            else
                lastSoldDate = '';
                lastSoldPrice = 0;
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
                'Bathrooms', str2double(results.bathrooms.Text), ...
                'Bedrooms', str2double(results.bedrooms.Text), ...
                'TotalRooms', totalRooms, ...
                'TaxAssessment', taxAssessment, ...
                'TaxAssessmentYear', taxAssessmentYear, ...
                'LastSoldDate', lastSoldDate, ...
                'LastSoldPrice', lastSoldPrice, ...
                'ZEstimate', str2double(results.zestimate.amount.Text));
        end
        
        function dispCompResults(results)
            for idx = 1:numel(results.Comps)
                item = results.Comps(idx);
                fprintf('House %d: %s, %s, %s -> zestimate: %d\n', ...
                        idx, item.Street, item.City, item.State, item.ZEstimate);
            end
        end
    end
end
