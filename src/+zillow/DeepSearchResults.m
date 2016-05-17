classdef DeepSearchResults < handle
    properties (Access = private, Constant)
        ZillowWebService = 'http://www.zillow.com/webservice/GetDeepSearchResults.htm?';
        ZWSID = 'X1-ZWz1f8wdb88lxn_1y8f9';
    end
    
    properties (Constant)
        Headers = {'Link', 'Address', 'City', 'State', 'ZipCode', 'Latitude', 'Longitude', 'UseCode', ...
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
            this.write(fileName, xmlData);
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
                    if isfield(results, 'lastSoldDate')
                        lastSoldDate = results.lastSoldDate.Text;
                        lastSoldPrice = str2double(results.lastSoldPrice.Text);
                    else
                        lastSoldDate = '';
                        lastSoldPrice = -1;
                    end
                    
                    if isfield(results, 'totalRooms')
                        totalRooms = str2double(results.totalRooms.Text);
                    else
                        totalRooms = -1;
                    end

                    if isfield(results, 'taxAssessment')
                        taxAssessment = str2double(results.taxAssessment.Text);
                        taxAssessmentYear = str2double(results.taxAssessmentYear.Text);
                    else
                        taxAssessment = -1;
                        taxAssessmentYear = -1;
                    end

                    if isfield(results, 'lotSizeSqFt')
                        lotSizeSqFt = str2double(results.lotSizeSqFt.Text);
                    else
                        lotSizeSqFt = -1;
                    end

                    if isfield(results, 'finishedSqFt')
                        finishedSqFt = str2double(results.finishedSqFt.Text);
                    else
                        finishedSqFt =-1;
                    end

                    item = struct(...
                        'Link', results.links.homedetails.Text, ...
                        'Address', results.address.street.Text, ...
                        'City', results.address.city.Text, ...
                        'State', results.address.state.Text, ...
                        'ZipCode', results.address.zipcode.Text, ...
                        'Latitude', str2double(results.address.latitude.Text), ...
                        'Longitude', str2double(results.address.longitude.Text), ...
                        'UseCode', results.useCode.Text, ...
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
                    
                    % Make sure that headers and actual fields are consistent.
                    assert(isequal(this.Headers', fieldnames(item)), 'Actual and expectec fields are different!');
                end
            end
        end
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
    end
end
