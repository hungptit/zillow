classdef Utilities < handle
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
