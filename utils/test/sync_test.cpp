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

#include "utils/sync.h"

// Namespaces.
using namespace CppUnit;
using namespace std;

//-----------------------------------------------------------------------------

class TestSync: public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(TestSync);
    CPPUNIT_TEST(testHello);
    CPPUNIT_TEST(testSync);
    CPPUNIT_TEST(testScopedlock);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    void testHello(void);
    void testSync(void);
    void testScopedlock(void);

private:
    // Our objects
    utils::Lock *m_lock;
};

//-----------------------------------------------------------------------------

void TestSync::testHello(void) {
    CPPUNIT_ASSERT(1 == 1);
}

void TestSync::setUp(void) {
    // setup code here
    m_lock = new utils::Lock();
    return;
}

void TestSync::tearDown(void) {
    // cleanup code here
    delete m_lock;
    return;
}

void TestSync::testSync(void) {
    utils::Lock lock;
    lock.lock();
    CPPUNIT_ASSERT(!lock.trylock(500000000));
    lock.unlock();
    CPPUNIT_ASSERT(lock.trylock(500000000));
    lock.unlock();
    return;
}

void TestSync::testScopedlock(void) {
    utils::ScopedLocker scopedlock(m_lock);
    return;
}

//-----------------------------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION(TestSync);

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
    ofstream xmlFileOut("sync_test.xml");
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
