classdef DeepComps < handle
    properties (Access = public, Constant)
        ZillowWebService = 'http://www.zillow.com/webservice/GetDeepComps.htm?';
        ZWSID = 'X1-ZWz1f8wdb88lxn_1y8f9';
    end

    methods (Access = public)
        function results = exec(this, zpid, count)
            queryCommand = sprintf('%szws-id=%s&zpid=%d&count=%d', ...
                                   this.ZillowWebService, this.ZWSID, zpid, count);            
            xmlData = webread(queryCommand);
            results = this.parse(xmlData);
        end

        function results = parse(~, xmlData)
            fileName = [tempname, '.xml'];
            zillow.Utilities.write(fileName, xmlData);
            info = zillow.xml2struct(fileName);
            delete(fileName);

            % Create the output
            data = info.Comps_colon_comps.response.properties;
                results = struct('Principal', zillow.Utilities.extractData(data.principal), ...
                                 'Comps', cellfun(@(item) ...
                                                  zillow.Utilities.extractData(item), data.comparables.comp));
                
                
        end                    
    end
end
