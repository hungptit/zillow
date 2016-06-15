-*- mode: compilation; default-directory: "~/projects/zillow/testing/" -*-
Compilation started at Tue Jun 14 21:58:50

make -j3
/home/hungptit/projects/zillow/testing/../..//3p/boost
-- Boost version: 1.60.0
-- Found the following Boost libraries:
--   regex
--   date_time
--   system
--   filesystem
--   thread
--   graph
--   program_options
--   iostreams
/home/hungptit/projects/zillow/testing/../../
/home/hungptit/projects/3p/boost/lib/libboost_regex.a/home/hungptit/projects/3p/boost/lib/libboost_date_time.a/home/hungptit/projects/3p/boost/lib/libboost_system.a/home/hungptit/projects/3p/boost/lib/libboost_filesystem.a/home/hungptit/projects/3p/boost/lib/libboost_thread.a/home/hungptit/projects/3p/boost/lib/libboost_graph.a/home/hungptit/projects/3p/boost/lib/libboost_program_options.a/home/hungptit/projects/3p/boost/lib/libboost_iostreams.a
-- Configuring done
-- Generating done
-- Build files have been written to: /home/hungptit/projects/zillow/testing
Scanning dependencies of target deepSearchResults
Scanning dependencies of target collect_data
Scanning dependencies of target deepComps
[  8%] Building CXX object CMakeFiles/deepSearchResults.dir/deepSearchResults.cpp.o
[ 16%] Building CXX object CMakeFiles/collect_data.dir/collect_data.cpp.o
[ 25%] Building CXX object CMakeFiles/deepComps.dir/deepComps.cpp.o
In file included from /home/hungptit/projects/zillow/testing/deepComps.cpp:4:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:16:
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:26:53: warning: unused parameter 'edges'
      [-Wunused-parameter]
                       const std::vector<EdgeData> &edges) {
                                                    ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:58:50: warning: unused parameter 'dataFile'
      [-Wunused-parameter]
    void writeToNoSQLDatabase(const std::string &dataFile,
                                                 ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:59:63: warning: unused parameter 'vertexes'
      [-Wunused-parameter]
                              std::vector<DeepSearchResults> &vertexes,
                                                              ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:60:54: warning: unused parameter 'edges'
      [-Wunused-parameter]
                              std::vector<EdgeData> &edges) {}
                                                     ^
In file included from /home/hungptit/projects/zillow/testing/deepComps.cpp:4:
/home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:135:1: error: version control conflict
      marker in file
<<<<<<< HEAD
^
/home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:174:1: error: version control conflict
      marker in file
<<<<<<< HEAD
^
/home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:195:1: error: version control conflict
      marker in file
<<<<<<< HEAD
^
In file included from /home/hungptit/projects/zillow/testing/deepSearchResults.cpp:11:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:16:
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:26:53: warning: unused parameter 'edges'
      [-Wunused-parameter]
                       const std::vector<EdgeData> &edges) {
                                                    ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:58:50: warning: unused parameter 'dataFile'
      [-Wunused-parameter]
    void writeToNoSQLDatabase(const std::string &dataFile,
                                                 ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:59:63: warning: unused parameter 'vertexes'
      [-Wunused-parameter]
                              std::vector<DeepSearchResults> &vertexes,
                                                              ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:60:54: warning: unused parameter 'edges'
      [-Wunused-parameter]
                              std::vector<EdgeData> &edges) {}
                                                     ^
In file included from /home/hungptit/projects/zillow/testing/deepSearchResults.cpp:11:
/home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:135:1: error: version control conflict
      marker in file
<<<<<<< HEAD
^
/home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:174:1: error: version control conflict
      marker in file
<<<<<<< HEAD
^
/home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:195:1: error: version control conflict
      marker in file
<<<<<<< HEAD
^
In file included from /home/hungptit/projects/zillow/testing/collect_data.cpp:11:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:16:
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:26:53: warning: unused parameter 'edges'
      [-Wunused-parameter]
                       const std::vector<EdgeData> &edges) {
                                                    ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:58:50: warning: unused parameter 'dataFile'
      [-Wunused-parameter]
    void writeToNoSQLDatabase(const std::string &dataFile,
                                                 ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:59:63: warning: unused parameter 'vertexes'
      [-Wunused-parameter]
                              std::vector<DeepSearchResults> &vertexes,
                                                              ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:60:54: warning: unused parameter 'edges'
      [-Wunused-parameter]
                              std::vector<EdgeData> &edges) {}
                                                     ^
/home/hungptit/projects/zillow/testing/deepSearchResults.cpp:84:32: warning: unused variable 'parseResults'
      [-Wunused-variable]
        pugi::xml_parse_result parseResults = doc.load(output);
                               ^
In file included from /home/hungptit/projects/zillow/testing/collect_data.cpp:11:
/home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:135:1: error: version control conflict
      marker in file
<<<<<<< HEAD
^
/home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:174:1: error: version control conflict
      marker in file
<<<<<<< HEAD
^
/home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:195:1: error: version control conflict
      marker in file
<<<<<<< HEAD
^
In file included from /home/hungptit/projects/zillow/testing/deepComps.cpp:4:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:16:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:14:
In file included from /home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Session.h:25:
In file included from /home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Statement.h:25:
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Binding.h:1393:2: error: implicit
      instantiation of undefined template 'POCO_STATIC_ASSERTION_FAILURE<false>'
        poco_static_assert (!IsConst<T>::VALUE);
        ^
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Bugcheck.h:177:34: note: expanded from macro
      'poco_static_assert'
        typedef poco_static_assert_test<sizeof(POCO_STATIC_ASSERTION_FAILURE<(bool) (B)>)> \
                                        ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:48:65: note: in instantiation of function
      template specialization 'Poco::Data::Keywords::use<const unsigned long>' requested here
            session << "INSERT INTO House VALUE(?, ?, ?, ?)" << use(item.zpid)
                                                                ^
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Bugcheck.h:152:8: note: template is declared
      here
struct POCO_STATIC_ASSERTION_FAILURE;
       ^
In file included from /home/hungptit/projects/zillow/testing/deepComps.cpp:4:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:16:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:14:
In file included from /home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Session.h:25:
In file included from /home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Statement.h:25:
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Binding.h:1393:2: error: implicit
      instantiation of undefined template 'POCO_STATIC_ASSERTION_FAILURE<false>'
        poco_static_assert (!IsConst<T>::VALUE);
        ^
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Bugcheck.h:177:34: note: expanded from macro
      'poco_static_assert'
        typedef poco_static_assert_test<sizeof(POCO_STATIC_ASSERTION_FAILURE<(bool) (B)>)> \
                                        ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:49:24: note: in instantiation of function
      template specialization 'Poco::Data::Keywords::use<const std::basic_string<char> >' requested here
                    << use(std::get<STREET>(item.address))
                       ^
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Bugcheck.h:152:8: note: template is declared
      here
struct POCO_STATIC_ASSERTION_FAILURE;
       ^
4 warnings and 5 errors generated.
CMakeFiles/deepComps.dir/build.make:62: recipe for target 'CMakeFiles/deepComps.dir/deepComps.cpp.o' failed
make[2]: *** [CMakeFiles/deepComps.dir/deepComps.cpp.o] Error 1
CMakeFiles/Makefile2:104: recipe for target 'CMakeFiles/deepComps.dir/all' failed
make[1]: *** [CMakeFiles/deepComps.dir/all] Error 2
make[1]: *** Waiting for unfinished jobs....
In file included from /home/hungptit/projects/zillow/testing/collect_data.cpp:11:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:16:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:14:
In file included from /home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Session.h:25:
In file included from /home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Statement.h:25:
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Binding.h:1393:2: error: implicit
      instantiation of undefined template 'POCO_STATIC_ASSERTION_FAILURE<false>'
        poco_static_assert (!IsConst<T>::VALUE);
        ^
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Bugcheck.h:177:34: note: expanded from macro
      'poco_static_assert'
        typedef poco_static_assert_test<sizeof(POCO_STATIC_ASSERTION_FAILURE<(bool) (B)>)> \
                                        ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:48:65: note: in instantiation of function
      template specialization 'Poco::Data::Keywords::use<const unsigned long>' requested here
            session << "INSERT INTO House VALUE(?, ?, ?, ?)" << use(item.zpid)
                                                                ^
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Bugcheck.h:152:8: note: template is declared
      here
struct POCO_STATIC_ASSERTION_FAILURE;
       ^
In file included from /home/hungptit/projects/zillow/testing/collect_data.cpp:11:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:16:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:14:
In file included from /home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Session.h:25:
In file included from /home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Statement.h:25:
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Binding.h:1393:2: error: implicit
      instantiation of undefined template 'POCO_STATIC_ASSERTION_FAILURE<false>'
        poco_static_assert (!IsConst<T>::VALUE);
        ^
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Bugcheck.h:177:34: note: expanded from macro
      'poco_static_assert'
        typedef poco_static_assert_test<sizeof(POCO_STATIC_ASSERTION_FAILURE<(bool) (B)>)> \
                                        ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:49:24: note: in instantiation of function
      template specialization 'Poco::Data::Keywords::use<const std::basic_string<char> >' requested here
                    << use(std::get<STREET>(item.address))
                       ^
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Bugcheck.h:152:8: note: template is declared
      here
struct POCO_STATIC_ASSERTION_FAILURE;
       ^
4In file included from /home/hungptit/projects/zillow/testing/deepSearchResults.cpp:11:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:16:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:14:
In file included from /home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Session.h:25:
In file included from /home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Statement.h:25:
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Binding.h:1393:2: error: implicit
      instantiation of undefined template 'POCO_STATIC_ASSERTION_FAILURE<false>'
        poco_static_assert (!IsConst<T>::VALUE);
        ^
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Bugcheck.h:177:34: note: expanded from macro
      'poco_static_assert'
        typedef poco_static_assert_test<sizeof(POCO_STATIC_ASSERTION_FAILURE<(bool) (B)>)> \
                                        ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:48:65: note: in instantiation of function
      template specialization 'Poco::Data::Keywords::use<const unsigned long>' requested here
            session << "INSERT INTO House VALUE(?, ?, ?, ?)" << use(item.zpid)
                                                                ^
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Bugcheck.h:152:8: note: template is declared
      here
struct POCO_STATIC_ASSERTION_FAILURE;
       ^
 warnings and 5 errors generated.
In file included from /home/hungptit/projects/zillow/testing/deepSearchResults.cpp:11:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/ZillowWebCrawler.hpp:16:
In file included from /home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:14:
In file included from /home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Session.h:25:
In file included from /home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Statement.h:25:
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Data/Binding.h:1393:2: error: implicit
      instantiation of undefined template 'POCO_STATIC_ASSERTION_FAILURE<false>'
        poco_static_assert (!IsConst<T>::VALUE);
        ^
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Bugcheck.h:177:34: note: expanded from macro
      'poco_static_assert'
        typedef poco_static_assert_test<sizeof(POCO_STATIC_ASSERTION_FAILURE<(bool) (B)>)> \
                                        ^
/home/hungptit/projects/zillow/testing/../../zillow/Database.hpp:49:24: note: in instantiation of function
      template specialization 'Poco::Data::Keywords::use<const std::basic_string<char> >' requested here
                    << use(std::get<STREET>(item.address))
                       ^
/home/hungptit/projects/zillow/testing/../../3p/poco/include/Poco/Bugcheck.h:152:8: note: template is declared
      here
struct POCO_STATIC_ASSERTION_FAILURE;
       ^
CMakeFiles/collect_data.dir/build.make:62: recipe for target 'CMakeFiles/collect_data.dir/collect_data.cpp.o' failed
make[2]: *** [CMakeFiles/collect_data.dir/collect_data.cpp.o] Error 1
CMakeFiles/Makefile2:67: recipe for target 'CMakeFiles/collect_data.dir/all' failed
make[1]: *** [CMakeFiles/collect_data.dir/all] Error 2
5 warnings and 5 errors generated.
CMakeFiles/deepSearchResults.dir/build.make:62: recipe for target 'CMakeFiles/deepSearchResults.dir/deepSearchResults.cpp.o' failed
make[2]: *** [CMakeFiles/deepSearchResults.dir/deepSearchResults.cpp.o] Error 1
CMakeFiles/Makefile2:141: recipe for target 'CMakeFiles/deepSearchResults.dir/all' failed
make[1]: *** [CMakeFiles/deepSearchResults.dir/all] Error 2
Makefile:138: recipe for target 'all' failed
make: *** [all] Error 2

Compilation exited abnormally with code 2 at Tue Jun 14 21:59:30
