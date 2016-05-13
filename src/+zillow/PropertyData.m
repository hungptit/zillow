classdef PropertyData < handle
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
    end
    
    methods (Static, Access = private)
        function results = parseAddress(value)
            results = value;
        end
        
        function [city, state] = parseCity(value)
            city = value;
            state = 'MA';
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
