street = '302 Warren St';
city = 'Needham';
state = 'MA';

deepSearchQuery = zillow.DeepSearchResults;
data = deepSearchQuery.exec(street, city, state);

zpid = data.zpid;
count = 25;
deepCompsQuery = zillow.DeepComps;
comps = deepCompsQuery.exec(zpid, count);

updateQuery = zillow.UpdatedPropertyDetails;
updateInfo = updateQuery.exec(zpid);
