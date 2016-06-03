classdef UpdatedPropertyDetails < handle
        properties (Access = public, Constant)
        ZillowWebService = 'http://www.zillow.com/webservice/GetUpdatedPropertyDetails.htm?';
        ZWSID = 'X1-ZWz1f8wdb88lxn_1y8f9';
    end

    methods (Access = public)
        function results = exec(this, zpid)
            queryCommand = sprintf('%szws-id=%s&zpid=%d', this.ZillowWebService, this.ZWSID, zpid);
            xmlData = webread(queryCommand);
            results = this.parse(xmlData);
        end

        function results = parse(this, xmlData)
            fileName = [tempname, '.xml'];
            zillow.Utilities.write(fileName, xmlData);
            info = zillow.xml2struct(fileName);
            delete(fileName);

            % Create the output
            data = info.UpdatedPropertyDetails_colon_updatedPropertyDetails;
            if isfield(data, 'response')
                results = this.extractData(data.response);
            else
                results = [];
            end
        end
    end

    methods (Static, Access = public)
        function results = extractData(item)
            if isfield(item, 'homeDescription')
                desc = item.homeDescription.Text;
            else
                desc = '';
            end
  
            if isfield(item, 'editedFacts')
                data = item.editedFacts;
                if isfield(data, 'bedrooms')
                    bedrooms = str2double(data.bedrooms.Text);
                else
                    bedrooms = 0;
                end
                
                if isfield(data, 'bedrooms')
                    bathrooms = str2double(data.bathrooms.Text);
                else
                    bathrooms = 0;
                end
                
                if isfield(data, 'numRooms')
                    numrooms = str2double(data.numRooms.Text);
                else
                    numrooms = 0;
                end
            else
                bedrooms = 0;
                bathrooms = 0;
                numrooms = 0;
            end
            
            % TODO: Read zillow specs to get the list of possible
            % fields for example take a look at below houses:
            % 1. 104 Kings Hwy, West Springfield MA
            % 2. 28 Lincoln St, Enfield, CT
            results = struct('zpid', str2double(item.zpid.Text), ...
                             'Link', item.links.homeDetails.Text, ...
                             'Street', item.address.street.Text, ...
                             'City', item.address.city.Text, ...
                             'State', item.address.state.Text, ...
                             'ZipCode', item.address.zipcode.Text, ...
                             'UseCode', item.editedFacts.useCode.Text, ...
                             'Bedrooms', bedrooms, ...
                             'Bathrooms', bathrooms, ...
                             'TotalRooms', numrooms, ...
                             'FinishedSqFt', str2double(item.editedFacts.finishedSqFt.Text), ...
                             'LotSizeSqFt', str2double(item.editedFacts.lotSizeSqFt.Text), ...
                             'YearBuilt', str2double(item.editedFacts.yearBuilt.Text), ...
                             'HomeDescription', desc, ...
                             'PageViewCountThisMonth', str2double(item.pageViewCount.currentMonth.Text), ...
                             'PageViewCountTotal', str2double(item.pageViewCount.total.Text));
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
