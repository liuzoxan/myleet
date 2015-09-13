#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>

#include <stdlib.h>
#include "utils/sleep.h"

// Namespaces.
using namespace CppUnit;
using namespace std;

//-----------------------------------------------------------------------------

class TestSleep: public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(TestSleep);
    CPPUNIT_TEST(testHello);
    CPPUNIT_TEST(testSleep);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    void testHello(void);
    void testSleep(void);
private:
    // Our objects
};

//-----------------------------------------------------------------------------

void TestSleep::testHello(void) {
    CPPUNIT_ASSERT(1 == 1);
}

void TestSleep::setUp(void) {
    // setup code here
    return;
}

void TestSleep::tearDown(void) {
    // cleanup code here
    return;
}

void TestSleep::testSleep(void) {
    CPPUNIT_ASSERT(1==1);

    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);
    long int start_nanosec = start.tv_sec * utils::SECOND_IN_NS + start.tv_nsec;

    long int sleep_nanosec = 500000000;
    utils::ns_sleep(sleep_nanosec);
    clock_gettime(CLOCK_MONOTONIC, &stop);
    long int stop_nanosec = stop.tv_sec * utils::SECOND_IN_NS + stop.tv_nsec;
    CPPUNIT_ASSERT(stop_nanosec >= start_nanosec + sleep_nanosec);
}

//-----------------------------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION(TestSleep);

//int main(int argc, char* argv[]) {
int main() {
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener(&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener(&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write();

    // Output XML for Jenkins CPPunit plugin
    ofstream xmlFileOut("sleep_test.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    // NB: In the return code never return just 0!
    if (collectedresults.wasSuccessful()) {
        // return EXIT_SUCCESS if the tests were successful
        return EXIT_SUCCESS;
    } else {
        // return EXIT_FAILURE if the tests failed
        return EXIT_FAILURE;
    }
}
