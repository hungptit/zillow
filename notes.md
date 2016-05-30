# SQL commands #

    CREATE VIEW all_houses AS SELECT HouseFact.Street, Cities.Name, States.Name, 
    HouseFact.Bathrooms, HouseFact.Bedrooms, 
    HouseInfo.LastSoldDate, HouseInfo.LastSoldPrice, HouseInfo.ZEstimate, HouseFact.Link 
    FROM HouseFact, HouseInfo, Cities, States 
    WHERE HouseFact.zpid=HouseInfo.zpid AND HouseFact.CityID=Cities.ID AND Cities.SID=States.ID;

