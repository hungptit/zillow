classdef DeepSearchResults < handle
    properties (Access = private, Constant)
        ZillowWebService = 'http://www.zillow.com/webservice/GetDeepSearchResults.htm?';
        ZWSID = 'X1-ZWz1f8wdb88lxn_1y8f9';
    end
    
    properties (Constant)
        Headers = {'zpid', 'Link', 'Street', 'City', 'State', 'ZipCode', 'Latitude', 'Longitude', 'UseCode', ...
                   'YearBuilt', 'LotSizeSqFt', 'FinishedSqFt', 'Bathrooms', 'Bedrooms', 'TotalRooms', ...
                   'TaxAssessment', 'TaxAssessmentYear', 'LastSoldDate', 'LastSoldPrice', 'ZEstimate'};
    end
    
    methods (Access = public)
        function info = exec(this, anAddress, aCity, aState)
            queryStr = sprintf('&address=%s&citystatezip=%s%s+%s', ...
                               strrep(strtrim(anAddress), ' ', '+'), ...
                               strrep(strtrim(aCity), ' ', '+'), '%2C', aState);
            queryCommand = sprintf('%szws-id=%s%s', this.ZillowWebService, this.ZWSID, ...
                                   queryStr);
            
            % fprintf('%s\n', queryCommand);
            
            % Query a given address information and parse it into a struct.
            xmlData = webread(queryCommand);
            info = this.parse(xmlData);
        end               

        function item = parse(this, xmlData)
            fileName = [tempname, '.xml'];
            zillow.Utilities.write(fileName, xmlData);
            info = zillow.xml2struct(fileName);
            delete(fileName);
            
            % Update the output data            
            if (~strcmp(info.SearchResults_colon_searchresults.message.code.Text, '0'))
                warning('Cannot query the information of this house: %s %s', ...
                        info.SearchResults_colon_searchresults.request.address.Text, ...
                        info.SearchResults_colon_searchresults.request.citystatezip.Text);
                item = [];
            else
                results = info.SearchResults_colon_searchresults.response.results.result;
                if iscell(results)
                    warning('Cannot query the information of this house: %s %s', ...
                            info.SearchResults_colon_searchresults.request.address.Text, ...
                            info.SearchResults_colon_searchresults.request.citystatezip.Text);
                    item = [];                    
                else
                    item = zillow.Utilities.extractData(results);
                    
                    % Make sure that headers and actual fields are consistent.
                    assert(isequal(this.Headers', fieldnames(item)), 'Actual and expectec fields are different!');
                end
            end
        end
    end    
end
