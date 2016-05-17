classdef MarkdownWriter < zillow.FileWriter
    methods (Access = public)
        function this = MarkdownWriter(dataFile)
            this@zillow.FileWriter(dataFile);
            this.SepChar = '|';
        end
    
        function writeRowPrefix(this)
            fprintf(this.FileId, '%s', this.SepChar);
        end
    end
end
