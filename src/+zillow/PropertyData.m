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

        function writeToMarkdown(this, mdFile)
            fid = fopen(mdFile, 'wt');
            % Write out the markdown table here.
            
            

            % Close the markdown file.
            fclose(fid);
        end

        function writeToCSV(this, csvFile)
            [~, N] = size(this.OriginalData);
            originalHeaders = this.OriginalData(1,:);
            zillowHeaders = zillow.DeepSearchResults.Headers;
            allHeaders = horzcat(zillow.DeepSearchResults.Headers, originalHeaders{3:end});

            % Open a CSV file  to write
            fid = fopen(csvFile, 'wt');
            
            % Write the header
            for idx = 1:numel(allHeaders)
                fprintf(fid, '%s;', allHeaders{idx});                
            end
            fprintf(fid, '\n');
            
            % Write data
            numberOfZillowHeaders = numel(zillowHeaders);
            for idx = 1:numel(this.DeepSearchResults)
                item = this.DeepSearchResults{idx};
                if isempty(item)
                    fprintf(fid, ';');  % Zillow link is empty.
                    for colId = 1:2
                        this.writeValue(fid, this.OriginalData{idx + 1, colId});
                    end
                    
                    for fieldId = 4:numberOfZillowHeaders
                        fprintf(fid, ';');
                    end
                                        
                    for colId = 3:N
                        this.writeValue(fid, this.OriginalData{idx + 1, colId});
                    end
                    
                    fprintf(fid, '\n');
                else
                    for fieldId = 1:numberOfZillowHeaders
                        this.writeValue(fid, item.(zillowHeaders{fieldId}));
                    end
                    
                    for colId = 3:N
                        this.writeValue(fid, this.OriginalData{idx + 1, colId});
                    end
                    fprintf(fid, '\n');
                end
            end
            fclose(fid);
        end

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

        function print(this, idx)
            if isempty(idx)
                idx = 1:numel(this.Address);
            end

            arrayfun(@(id) fprintf('%s; %s; %s\n', ...
                                   this.Address{id}, ...
                                   this.City{id}, ...
                                   this.State{id}), idx);
        end
    end
    
    methods (Static, Access = public)
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
        
        function results = parseOccVacant(value)
            results = value;
        end
        
        function results = parseStatus(value)
            results = value;
        end
        
        function results = parseLBCode(value)
            if isnumeric(value)
                results = num2str(value);
            else
                results = value;
            end
        end
        
        function results = parsePrice(value)
            results = value;
        end
        
        function results = parseNotes(value)
            results = value;
        end
        
        function writeValue(fid, val)
            if isempty(val)
                fprintf(fid, ';');
            elseif ischar(val)
                fprintf(fid, '%s;', val);
            elseif isnumeric(val)
                fprintf(fid, '%g;', val);
            else
                assert(false, 'Unexpected data type: %s\n', class(val));
            end
        end
    end    
end
