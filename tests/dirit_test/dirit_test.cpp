#define BOOST_TEST_MODULE dirit_test
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <iostream>        

#include "node.h"
#include "fnv_wrapper.h"

namespace fs = boost::filesystem;

#ifndef TEST_DIR_SOURCE_PATH
#define TEST_DIR_SOURCE_PATH ../data/dirit_test/
#endif
#define str(x) #x
#define xstr(x) str(x)

bool copyDir(
    fs::path const & source,
    fs::path const & destination
)
{
    // Check whether the function call is valid
    boost::system::error_code ec;
    if(
        !fs::exists(source, ec) ||
        !fs::is_directory(source, ec)
    )
    {
        std::cerr << "Source directory " << source.string()
            << " does not exist or is not a directory." << '\n'
        ;
        return false;
    }
    if(fs::exists(destination, ec))
    {
        std::cerr << "Destination directory " << destination.string()
            << " already exists." << '\n'
        ;
        return false;
    }
    // Create the destination directory
    if(!fs::create_directory(destination, ec))
    {
        std::cerr << "Unable to create destination directory"
            << destination.string() << '\n'
        ;
        return false;
    }
    
    // Iterate through the source directory
    for(
        fs::directory_iterator file(source);
        file != fs::directory_iterator(); ++file
    )
    {
        fs::path current(file->path());
        if(fs::is_directory(current, ec))
        {
            // Found directory: Recursion
            if(
                !copyDir(
                    current,
                    destination / current.filename()
                )
            )
            {
                return false;
            }
        }
        else
        {
            // Found file: Copy
            fs::copy_file(
                current,
                destination / current.filename(),
                ec
            );
            if (ec.value() != boost::system::errc::success) {
                return false;
            }
        }
    }
    return true;
}

BOOST_AUTO_TEST_CASE(dirit) {
    // let's get a unique path in which
    // we can run our hierarchy tests
    boost::system::error_code ec;
    fs::path unique_path   = fs::unique_path();
    fs::path temp_path     = fs::temp_directory_path(ec);
    fs::path src_test_path = fs::path( xstr(TEST_DIR_SOURCE_PATH) );
    unique_path = temp_path / unique_path;

    if (ec.value() != boost::system::errc::success) {
        BOOST_FAIL("Unable to get path of temporary directory");
    }

    fs::create_directories(unique_path / fs::path("h1"), ec);
    fs::create_directories(unique_path / fs::path("h2"), ec);
    BOOST_REQUIRE_EQUAL(ec.value(), boost::system::errc::success); 

    fs::path dst1 = unique_path / fs::path("h1") / fs::path("h");
    fs::path dst2 = unique_path / fs::path("h2") / fs::path("h");

    BOOST_REQUIRE(copyDir(src_test_path, dst1));
    BOOST_REQUIRE(copyDir(src_test_path, dst2));

    BOOST_TEST_MESSAGE("Checking simple node creation");
    fs::directory_entry de(dst1);
    Node n(de);
    BOOST_REQUIRE_EQUAL(n.name(), dst1.filename()); 
    BOOST_REQUIRE_EQUAL(n.mtime(), fs::last_write_time(de));

    bool ok = false;
    Node::NodePtr node1 = Node::CreateWithPath(dst1, &ok);
    BOOST_TEST_MESSAGE("Checking proper node creation for " << dst1.string());
    BOOST_REQUIRE(ok);

    Node::NodePtr node2 = Node::CreateWithPath(dst2, &ok);
    BOOST_TEST_MESSAGE("Checking proper node creation for " << dst2.string());
    BOOST_REQUIRE(ok);
    BOOST_REQUIRE_EQUAL(node1->nsi(), node2->nsi());

    BOOST_TEST_MESSAGE("Nodes have NSI == " << node1->nsi() << " == " << node2->nsi() );
}
