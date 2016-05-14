classdef PropertyData < handle
    properties (SetAccess = private, GetAccess = public)
        DeepSearchResults
        BadAddress = {};
    end

    properties (SetAccess = private, GetAccess = public)
        Address = {}
        City = {}
        State = {}
        OccVacant = {}
        Status = {}
        LBCode = {}
        Price = [];
        Notes = {};
    end
    
    methods (Access = public)
        function this = PropertyData(xlsFile)
            this.import(xlsFile);
            this.crawl;
        end
        
        function crawl(this)
            query = zillow.DeepSearchResults;
            N = numel(this.Address);
            for idx = 1:N
                fprintf('Address %d: %s\n', idx, this.Address{idx});
                results = query.exec(this.getAddressQuery(idx)); 
                if isempty(results)
                    this.BadAddress{end+1} = struct('Address', this.Address{idx}, 'City', ...
                                                    this.City{idx}, 'State', this.State{idx});
                end
                this.DeepSearchResults{end + 1} = results;
            end

            % Display address that we cannot retrive the information.
            fprintf('===== Cannot retrieve information of below addresses ======\n');
            for idx = 1:numel(this.BadAddress)
                item =this.BadAddress{idx};
                fprintf('%s, %s, %s\n', item.Address, item.City, item.State);
            end
        end

        function writeToCSV(this, csvFile)
            N = numel(this.Address);
            fid = fopen(csvFile, 'wt');
            fieldNames = {'Link', 'Address', 'City', 'ZipCode', 'UseCode', ...
                          'TaxAssessment', 'TaxAssessmentYear', 'YearBuilt', ...
                          'LotSizeSqFt', 'FinishedSqFt', 'Bathrooms', 'Bedrooms', ...
                          'TotalRooms', 'LastSoldDate', 'LastSoldPrice', 'ZEstimate'};
            for idx = 1:numel(fieldNames)
                fprintf(fid, '%s;', fieldNames{idx});
            end
            fprintf(fid, 'OccVacant;Status;LBCode;Price;Notes');
            fprintf(fid, '\n');

            % Now write out the data
            for idx = 1:N
                fprintf('idx = %d\n', idx);
                item = this.DeepSearchResults{idx};
                if isempty(item)
                    for fieldId = 1:numel(fieldNames)
                        fprintf(fid, ';');
                    end
                else
                    fprintf(fid, '%s;%s;%s;%s;%s;', ...
                            item.Link, item.Address, item.City, item.ZipCode, item.UseCode);
                    fprintf(fid, '%d;%d;%d;', item.TaxAssessment, item.TaxAssessmentYear, item.YearBuilt);
                    fprintf(fid, '%d;%d;', item.LotSizeSqFt, item.FinishedSqFt);
                    fprintf(fid, '%g;%d;%d;', item.Bathrooms, item.Bedrooms, item.TotalRooms);
                    fprintf(fid, '%s;%d;%d', item.LastSoldDate, item.LastSoldPrice, item.ZEstimate);
                end

                fprintf(fid, ';%s;%s;%s;%g;%s', ...
                        this.OccVacant{idx}, this.Status{idx}, this.LBCode{idx}, ...
                        this.Price(idx), this.Notes{idx});
                fprintf(fid, '\n');
            end
            fclose(fid);
        end

        function import(this, xlsFile)
            [~, ~, raw] = xlsread(xlsFile);
            [M, N] = size(raw);
            assert(N == 7, 'We assume that there are only 7 columns\n');
            for idx = 2:M
                this.Address{end + 1} = this.parseAddress(raw{idx, 1});
                [this.City{end + 1}, this.State{end + 1}] = this.parseCity(raw{idx, 2});
                this.OccVacant{end + 1} = this.parseOccVacant(raw{idx, 3});
                this.Status{end + 1} = this.parseStatus(raw{idx, 4});
                this.LBCode{end + 1} = this.parseLBCode(raw{idx, 5});
                this.Price(end + 1) = this.parsePrice(raw{idx, 6});
                this.Notes{end + 1} = this.parseNotes(raw{idx, 7});
            end
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

        function cmd = getAddressQuery(this, idx)
            anAddress = this.Address{idx};
            aCity = this.City{idx};
            aState = this.State{idx};            
            cmd = sprintf('&address=%s&citystatezip=%s%s+%s', ...
                          strrep(strtrim(anAddress), ' ', '+'), ...
                          strrep(strtrim(aCity), ' ', '+'), '%2C', aState);
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
        
        function results = parseOccVacant(value)
            results = value;
        end
        
        function results = parseStatus(value)
            results = value;
        end
        
        function results = parseLBCode(value)
            results = value;
        end
        
        function results = parsePrice(value)
            results = value;
        end
        
        function results = parseNotes(value)
            results = value;
        end
    end    
end
