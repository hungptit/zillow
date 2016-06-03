classdef MarkdownWriter < zillow.FileWriter
    properties (SetAccess = private, GetAccess = private)
        CellHeader = '------';
    end

    methods (Access = public)
        function this = MarkdownWriter(dataFile)
            this@zillow.FileWriter(dataFile);
            this.SepChar = '|';
        end
    
        function writeHeaders(this, allHeaders)
            this.writeRowPrefix;
            cellfun(@(aString) fprintf(this.FileId, '%s%s', aString, this.SepChar), allHeaders);
            this.writeNewLine;
            this.writeRowPrefix;
            arrayfun(@(idx) fprintf(this.FileId, '%s%s', this.CellHeader, this.SepChar), 1:numel(allHeaders));
            this.writeNewLine;
        end

        function writeRowPrefix(this)
            fprintf(this.FileId, '%s', this.SepChar);
        end

        function writeHyperLink(this, aLink, aName)
            fprintf(this.FileId, '[%s](%s)%s', aName, aLink, this.SepChar);
        end
    end
end
