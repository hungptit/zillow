classdef FileWriter < handle
    properties (SetAccess = protected, GetAccess = protected)
        FileId
        SepChar = ';'
        Headers = {'Link', 'Address', 'City', 'UseCode', ...
                   'YearBuilt', 'LotSizeSqFt', 'FinishedSqFt', 'Bathrooms', ...
                   'Bedrooms', 'TotalRooms', 'TaxAssessment', ...
                   'TaxAssessmentYear', 'LastSoldDate', 'LastSoldPrice', 'ZEstimate'}
    end
    
    methods (Access = public)
        function this = FileWriter(dataFile)
            this.FileId = fopen(dataFile, 'wt');
            if this.FileId < 0
                error('Cannot open file %s to write!', dataFile);
            end
        end
        
        function delete(this)
            if (this.FileId > 0)
                fclose(this.FileId);
            end
        end
        
        function write(this, crawlData)
            originalHeaders = crawlData.OriginalData(1,:);
            zillowHeaders = this.Headers;
            allHeaders = horzcat(this.Headers, originalHeaders{3:end});
            searchResults = crawlData.DeepSearchResults;
            originalData = crawlData.OriginalData(2:end, :); % Skip the first row.
            [~, N] = size(originalData);

            % Write the header
            this.writeHeaders(allHeaders);
                        
            % Write out the data
            numberOfZillowHeaders = numel(zillowHeaders);
            for idx = 1:numel(searchResults)
                item = searchResults{idx};
                this.writeRowPrefix;
                if isempty(item)
                    this.writeEmptyValue;                    
                    for colId = 1:2
                        this.writeValue(originalData{idx, colId});
                    end
                    
                    for fieldId = 4:numberOfZillowHeaders
                        fprintf(this.FileId, ';');
                    end
                    
                    for colId = 3:N
                        this.writeValue(originalData{idx, colId});
                    end
                    this.writeNewLine;
                else
                    for fieldId = 1:numberOfZillowHeaders
                        this.writeValue(item.(zillowHeaders{fieldId}));
                    end
                    
                    for colId = 3:N
                        this.writeValue(originalData{idx, colId});
                    end
                    this.writeNewLine;
                end
            end
        end
        
        function writeHeaders(this, allHeaders)
            this.writeRowPrefix;
            cellfun(@(aString) fprintf(this.FileId, '%s%s', aString, this.SepChar), allHeaders);
            this.writeNewLine;
        end
        
        function writeEmptyValue(this)
            fprintf(this.FileId, ';');            
        end
        
        function writeRowPrefix(this)    %#ok  % Will be defined in derived classes.            
        end
        
        function writeNewLine(this)
            fprintf(this.FileId, '\n');            
        end
        
        function writeValue(this, val)
            if isempty(val)
                fprintf(this.FileId, '%s', this.SepChar);
            elseif ischar(val)
                if ~isempty(strfind(val, 'http://'))
                    this.writeHyperLink(val, 'link');
                else
                    fprintf(this.FileId, '%s%s', val, this.SepChar);
                end
            elseif isnumeric(val)
                fprintf(this.FileId, '%g%s', val, this.SepChar);
            else
                assert(false, 'Unexpected data type: %s\n', class(val));
            end
        end

        function writeHyperLink(this, aLink, ~)
            fprintf(this.FileId, '%s%s', aLink, this.SepChar);
        end
    end
end
