all: teststrutils teststrdatasource teststrdatasink testdsv testxml

obj:
    # The -p is to avoid error if the directory already exists
    mkdir -p obj 

bin: 
    mkdir -p bin

obj/StringUtils.o: src/StringUtils.cpp
    g++ -std=c++17 -Wall -Wextra -Iinclude -o obj/StringUtils.o -c src/StringUtils.cpp

obj/StringUtilsTest.o: testsrc/StringUtilsTest.cpp
    g++ -Iinclude -Wall -Wextra -std=c++17 -o obj/StringUtilsTest.o -c testsrc/StringUtilsTest.cpp 

obj/StringDataSource.o: src/StringDataSource.cpp
    g++ -std=c++17 -Wall -Wextra -Iinclude -o obj/StringDataSource.o -c src/StringDataSource.cpp

obj/StringDataSourceTest.o: testsrc/StringDataSourceTest.cpp
    g++ -Iinclude -Wall -Wextra -std=c++17 -o obj/StringDataSourceTest.o -c testsrc/StringDataSourceTest.cpp 

obj/StringDataSink.o: src/StringDataSink.cpp
    g++ -std=c++17 -Wall -Wextra -Iinclude -o obj/StringDataSink.o -c src/StringDataSink.cpp

obj/StringDataSinkTest.o: testsrc/StringDataSinkTest.cpp
    g++ -Iinclude -Wall -Wextra -std=c++17 -o obj/StringDataSinkTest.o -c testsrc/StringDataSinkTest.cpp 

obj/DSVReader.o: src/DSVReader.cpp
    g++ -std=c++17 -Wall -Wextra -Iinclude -o obj/DSVReader.o -c src/DSVReader.cpp

obj/DSVWriter.o: src/DSVWriter.cpp
    g++ -std=c++17 -Wall -Wextra -Iinclude -o obj/DSVWriter.o -c src/DSVWriter.cpp

obj/DSVTests.o: testsrc/DSVTests.cpp
    g++ -Iinclude -Wall -Wextra -std=c++17 -o obj/DSVTests.o -c testsrc/DSVTests.cpp 

obj/XMLReader.o: src/XMLReader.cpp
    g++ -std=c++17 -Wall -Wextra -Iinclude -o obj/XMLReader.o -c src/XMLReader.cpp

obj/XMLWriter.o: src/XMLWriter.cpp
    g++ -std=c++17 -Wall -Wextra -Iinclude -o obj/XMLWriter.o -c src/XMLWriter.cpp

obj/XMLTests.o: testsrc/XMLTests.cpp
    g++ -Iinclude -Wall -Wextra -std=c++17 -o obj/XMLTests.o -c testsrc/XMLTests.cpp 

teststrutils: obj/StringUtils.o obj/StringUtilsTest.o
    g++ -Wall -Wextra obj/StringUtils.o obj/StringUtilsTest.o -o teststrutils -lgtest -lgtest_main

teststrdatasource: obj/StringDataSource.o obj/StringDataSourceTest.o
    g++ -Wall -Wextra obj/StringDataSource.o obj/StringDataSourceTest.o -o teststrdatasource -lgtest -lgtest_main

teststrdatasink: obj/StringDataSink.o obj/StringDataSinkTest.o
    g++ -Wall -Wextra obj/StringDataSink.o obj/StringDataSinkTest.o -o teststrdatasink -lgtest -lgtest_main

testdsv: obj/DSVReader.o obj/DSVWriter.o obj/DSVTests.o
    g++ -Wall -Wextra obj/DSVReader.o obj/DSVWriter.o obj/DSVTests.o -o testdsv -lgtest -lgtest_main

testxml: obj/XMLReader.o obj/XMLWriter.o obj/XMLTests.o
    g++ -Wall -Wextra obj/XMLReader.o obj/XMLWriter.o obj/XMLTests.o -o testxml -lgtest -lgtest_main

clean:
    rm -rf obj bin

run: teststrutils teststrdatasource teststrdatasink testdsv testxml
    ./teststrutils
    ./teststrdatasource
    ./teststrdatasink
    ./testdsv
    ./testxml