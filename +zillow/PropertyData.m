classdef PropertyData < handle
    properties (SetAccess = private, GetAccess = public)
        DeepSearchResults
        BadAddress = {};
    end

    properties (SetAccess = private, GetAccess = public)
        Address = {}
        City = {}
        State = {}
        OriginalData = {};
        InputDataFile
    end
    
    methods (Access = public)
        function this = PropertyData(xlsFile)
            this.import(xlsFile);
            this.InputDataFile = xlsFile;
            this.crawl;
        end
        
        function crawl(this)
            query = zillow.DeepSearchResults;                       
            N = numel(this.Address);
            for idx = 1:N
                fprintf('Address %d: %s\n', idx, this.Address{idx});
                results = query.exec(this.Address{idx}, this.City{idx}, this.State{idx}); 
                if isempty(results)
                    this.BadAddress{end+1} = struct('Address', this.Address{idx}, 'City', ...
                                                    this.City{idx}, 'State', this.State{idx});
                end
                this.DeepSearchResults{end + 1} = results;
            end

            % Display address that we cannot retrive the information.
            if ~isempty(this.BadAddress)
                fprintf('===== Cannot retrieve information of below addresses ======\n');
                for idx = 1:numel(this.BadAddress)
                    item =this.BadAddress{idx};
                    fprintf('%s, %s, %s\n', item.Address, item.City, item.State);
                end
            end
        end
    end

    methods (Access = private)
        function import(this, xlsFile)
            [~, ~, raw] = xlsread(xlsFile);
            [M, N] = size(raw);
            assert(N > 1, 'We assume that there are at least 2 columns\n');
            for idx = 2:M
                this.Address{end + 1} = this.parseAddress(raw{idx, 1});
                [this.City{end + 1}, this.State{end + 1}] = this.parseCity(raw{idx, 2});
            end
            this.OriginalData = raw;
        end
    end
    
    methods (Static, Access = private)
        function results = parseAddress(value)
            results = value;
        end
        
        function [city, state] = parseCity(value)
            pos = strfind(value, ',');
            if isempty(pos)
                city = value;
                state = 'MA';
            else
                city = value(1:(pos-1));
                state = value((pos+1):end);
            end
        end
    end    
end
