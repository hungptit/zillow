classdef DeepComps < handle
    properties (Access = public, Constant)
        ZillowWebService = 'http://www.zillow.com/webservice/GetDeepComps.htm?';
        ZWSID = 'X1-ZWz1f8wdb88lxn_1y8f9';
    end

    methods (Access = public)
        function results = exec(this, zpid, count)
            queryCommand = sprintf('%szws-id=%s&zpid=%s&count=%d', this.ZillowWebService, this.ZWSID, zpid, count);
            xmlData = webread(queryCommand);
            results = this.parse(xmlData);
        end

        function results = parse(this, xmlData)
            fileName = [tempname, '.xml'];
            zillow.Utilities.write(fileName, xmlData);
            info = zillow.xml2struct(fileName);
            delete(fileName);

            % Create the output
            data = info.Comps_colon_comps.response.properties;
            results = struct('Prinipal', this.extractData(data.principal), ...
                             'Comps', cellfun(@(item) this.extractData(item), data.comparables.comp));
        end
    end

    methods (Static, Access = public)
        function results = extractData(item)
            results = struct('zpid', str2double(item.zpid.Text), ...
                             'Link', item.links.homedetails.Text, ...
                             'Street', item.address.street.Text, ...
                             'City', item.address.city.Text, ...
                             'State', item.address.state.Text, ...
                             'ZipCode', item.address.zipcode.Text, ...
                             'ZEstimate', str2double(item.zestimate.amount.Text));
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
