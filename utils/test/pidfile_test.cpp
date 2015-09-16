// CPPUnit includes

#include <iostream>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TextTestRunner.h>

// Our includes
#include <assert.h>
#include <cstdlib>
#include <stdexcept>
#include <cstring>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "utils/pidfile.h"
#include "utils/sleep.h"

// Namespaces used
using namespace CppUnit;
using namespace std;
using namespace utils;

//-----------------------------------------------------------------------------

class TestPidFile: public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(TestPidFile);
    CPPUNIT_TEST(testSimple1);
    CPPUNIT_TEST(testSimple2);
    CPPUNIT_TEST(testSimple3);
    CPPUNIT_TEST(testSimple4);
    CPPUNIT_TEST(testSimple5);
CPPUNIT_TEST_SUITE_END();

public:
    TestPidFile()
    {
        // Not needed, really, but the compiler complains...
        m_tmpDir = NULL;
        m_PID_Path = NULL;
    }

    virtual ~TestPidFile()
    {
        // Not needed, really
        if (m_tmpDir != NULL)
        {
            string cmd("rm -rf "); // Can not have special characters, no worries
            cmd += m_tmpDir;
            free(m_tmpDir);
            int ret = system(cmd.c_str());
            assert(0 == ret);
        }
        if (m_PID_Path != NULL)
        {
            free(m_PID_Path);
        }
    }

    virtual void setUp(void)
    {
        m_tmpDir = strdup("/tmp/pid_file_test-XXXXXX");
        char* ret = mkdtemp(m_tmpDir);
        assert(ret != NULL);
        string pidPath(m_tmpDir);
        pidPath += "/my.pid";
        m_PID_Path = strdup(pidPath.c_str());
    }

    virtual void tearDown(void)
    {
        string cmd("rm -rf "); // Can not have special characters
        cmd += m_tmpDir;
        free(m_tmpDir);
        free(m_PID_Path);
        int ret = system(cmd.c_str());
        assert(0 == ret);
        m_tmpDir = NULL;
        m_PID_Path = NULL;
    }

protected:
    char* m_tmpDir;
    char* m_PID_Path;

protected:
    void testSimple1(void)
    {
        PidFile pidf(m_PID_Path);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("New PID is not unlocked!", PID_FREE, pidf.check());
    }

    void testSimple2(void)
    {
        PidFile pidf(m_PID_Path);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("New PID is not unlocked!", PID_FREE, pidf.check());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to acquire PID!", PID_OK, pidf.acquire());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("New PID is not unlocked!", PID_ACQUIRED, pidf.check());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to release PID!", PID_OK, pidf.release());
    }

    void testSimple3(void)
    {
        PidFile pidf(m_PID_Path);
        int fd = open(m_PID_Path,
                O_CREAT | O_EXCL | O_NOATIME | O_NOFOLLOW | O_RDWR,
                S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        int ret = close(fd);
        CPPUNIT_ASSERT_MESSAGE("Failed to close a file!", 0 == ret);

        CPPUNIT_ASSERT_MESSAGE("Failed to create a file!", -1 != fd);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Invalid PID not detected!", PID_INVALID, pidf.check());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Acquired invalid PID!", PID_INVALID, pidf.acquire(false));
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Invalid PID not detected!", PID_INVALID, pidf.check());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Released invalid PID!", PID_USAGE_ERROR, pidf.release());
    }

    void testSimple4(void)
    {
        PidFile pidf(m_PID_Path);

        // int fd = open(m_PID_Path,
        //         O_CREAT | O_EXCL | O_NOATIME | O_NOFOLLOW | O_RDWR,
        //         S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        // CPPUNIT_ASSERT_MESSAGE("Failed to create a file!", -1 != fd);
        // write(fd, "1\ngarbage...\n", 13);
        // int ret = close(fd);
        // CPPUNIT_ASSERT_MESSAGE("Failed to close a file!", 0 == ret);

        pid_t pid = fork();

        if (0 == pid)
        {
            // child
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to acquire PID!", PID_OK, pidf.acquire());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("New PID is not unlocked!", PID_ACQUIRED, pidf.check());
            while (true)
            {
                sleep(1024);
            }
            _exit(0);
        }
        ns_sleep(SECOND_IN_NS/16);

        CPPUNIT_ASSERT_EQUAL_MESSAGE("Acquired by someone else PID not detected!",
                PID_OTHER_ACQUIRED, pidf.check());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Acquired already acquired by someone else PID!",
                PID_OTHER_ACQUIRED, pidf.acquire(false));
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Acquired PID not detected!",
                PID_OTHER_ACQUIRED, pidf.check());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Released already acquired by someone else PID!",
                PID_USAGE_ERROR, pidf.release());

        kill(pid, SIGKILL);
        int waitres = 0;
        waitpid(pid, &waitres, 0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("waitpid failed!", 9, waitres);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Stale PID not detected!", PID_STALE, pidf.check());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to clean stale PID file!", PID_OK, pidf.removeStale());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("New PID is not unlocked!", PID_FREE, pidf.check());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to acquire PID!", PID_OK, pidf.acquire());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to release PID!", PID_OK, pidf.release());
    }

    void testSimple5(void)
    {
        PidFile pidf(m_PID_Path);
        pid_t pid = fork();
        if (0 == pid)
        {
            // child
            CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to acquire PID!", PID_OK, pidf.acquire());
            CPPUNIT_ASSERT_EQUAL_MESSAGE("New PID is not unlocked!", PID_ACQUIRED, pidf.check());
            while (true)
            {
                sleep(1024);
            }
            _exit(0);
        }
        ns_sleep(SECOND_IN_NS/16);

        CPPUNIT_ASSERT_EQUAL_MESSAGE("Acquired by someone else PID not detected!",
                PID_OTHER_ACQUIRED, pidf.check());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Acquired already acquired by someone else PID!",
                PID_OTHER_ACQUIRED, pidf.acquire(false));
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Acquired PID not detected!",
                PID_OTHER_ACQUIRED, pidf.check());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Released already acquired by someone else PID!",
                PID_USAGE_ERROR, pidf.release());

        kill(pid, SIGKILL);
        int waitres = 0;
        waitpid(pid, &waitres, 0);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("waitpid failed!", 9, waitres);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Stale PID not detected!", PID_STALE, pidf.check());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Acquire stale PID without permission!", PID_STALE, pidf.acquire(false));
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to acquire stale PID!", PID_OK, pidf.acquire(true));
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Failed to release PID!", PID_OK, pidf.release());
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Released PID is not unlocked!", PID_FREE, pidf.check());
    }
};

//-----------------------------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION(TestPidFile);

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
    ofstream xmlFileOut("pid_file_test.xml");
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
