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
            results = this.extractData(info.UpdatedPropertyDetails_colon_updatedPropertyDetails.response);
        end
    end

    methods (Static, Access = public)
        function results = extractData(item)
            results = struct('zpid', str2double(item.zpid.Text), ...
                             'Link', item.links.homeDetails.Text, ...
                             'Street', item.address.street.Text, ...
                             'City', item.address.city.Text, ...
                             'State', item.address.state.Text, ...
                             'ZipCode', item.address.zipcode.Text, ...
                             'UseCode', item.editedFacts.useCode.Text, ...
                             'Bedrooms', str2double(item.editedFacts.bedrooms.Text), ...
                             'Bathrooms', str2double(item.editedFacts.bathrooms.Text), ...
                             'TotalRooms', str2double(item.editedFacts.numRooms.Text), ...
                             'FinishedSqFt', str2double(item.editedFacts.finishedSqFt.Text), ...
                             'LotSizeSqFt', str2double(item.editedFacts.lotSizeSqFt.Text), ...
                             'YearBuilt', str2double(item.editedFacts.yearBuilt.Text), ...
                             'HomeDescription', item.homeDescription.Text, ...
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
