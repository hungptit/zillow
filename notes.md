# Business cases #
* Have a mobil app that 
    * Allow users to query all information related to a house.
    * Help them to estimate the house price.
    * Suggest the agent name based on the historical data.
    * Create a brief report about the recent sale records and listings.
    * Give a list of suggested contractors which have been verified by local customers.
        * Should allow customers to rate their satisfaction with the work.
    * Collection of articles about home improvement that can be obtained automatically using deep learning algorithm.
    * 
# Architect #
* Use C++ at the server side.
* Use swift + java script at the client side.

# Others #
* Get mynewhome.com domain.
* Learn more about machine learning + deep learning and apply learned algorithms to the crawled data.
* Learn swift 
* Learn javascript
* Feed electrical code, plumbing code, and building code to the elasticsearch engine 
    * Expose these information in the mynewhome domain using javascript.
    * Use deep learning algorithms to improve the search results.

## Collect data automatically ##
* Need three wspids for collecting data (DeepSearchResults, DeepComps, and UpdatedPropertyDetails)
* Use one wspid for UpdatedPropertyDetails
* Use more than one wspid for DeepComps
* collect_data
** Can load the previous state -> Given the NoSQL database
    * We need to load vertex_ids.
    * Load the current queue information.
