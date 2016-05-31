% Query information of a given house
this = zillow.DeepSearchResults;
street = '1111 Ocean St';
city = 'Marshfield';
state = 'MA';
houseInfo = this.exec(street, city, state);

% Search for similar house in this area. Zillow only give maximum
% 25 houses.
query = zillow.DeepComps;
count = 25;
similarHouses = query.exec(houseInfo.zpid, count);

% Save data to CSV files
zillow.Utilities.writeDeepCompResults(similarHouses, 'data.csv');

