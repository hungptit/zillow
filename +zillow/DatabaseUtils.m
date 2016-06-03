classdef SQLite < handle
    methods (Static, Access = public)
        % This table contains facts of the house at a given time slot such as
        % lot size, total square feet, number of bathrooms, number of
        % bedrooms etc.
        function createHouseTable(db, aRow, tableName)
            
        end
        
        % This table contains information about a house at a given
        % time such as page view count, description (from a listing
        % agent). This table will have an id of a house.
        function createUpdateHouseDetailTable(db, tableName)
        end
        
        % This table has a list of id, city and state id.
        function createCityTable(db, tableName)
        end
        
        % This table has a list of state, its abbreviation, and id.
        function createStateTable(db, tableName)
        end  
        
        % This table has a list of house (zpid) need to crawl. The
        % information will be erase if house information is updated.
        function createUnknownHouseInfo(db, tableName)
        end
    end
end