#ifndef NODE_H
#define NODE_H

#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <stdint.h>
#include <map>
#include <ctime>
#include <string>

namespace fs = boost::filesystem;
class Node {
public:
    typedef boost::shared_ptr<Node> NodePtr;
    typedef boost::weak_ptr<Node>   NodeWeakPtr;

    Node();
    Node(const fs::directory_entry &entry);
    ~Node();

    static boost::shared_ptr<Node> CreateWithPath(const fs::path &, bool *ok);

    void appendChild(NodePtr &n);
    uintmax_t nsi() const;

    std::string name() const;
    uintmax_t size() const;
    uintmax_t mtime() const;

    /**
     * Updates current node and children
     * recursively
     * Fills in vectors from @params deletedPaths,
     * addedPaths, modifiedPaths with the relevant
     * data gathered during processing
     */
    //bool updateWithPath(const fs::path &path,
    //        std::vector<fs::directory_entry> &deletedPaths,
    //        std::vector<fs::directory_entry> &addedPaths,
    //        std::vector<fs::directory_entry> &modifiedPaths);

private:
    Node(const Node &);
    static NodePtr _CreateWithPath(const fs::directory_entry &, Node *parent, bool *ok);

    /*
     * Updates current NSI
     * and propagates the information
     * to the parent
     */
    void updateNSI(Node* child);

private:
    std::string _name;
    uintmax_t _size;
    uintmax_t _mtime;

    // if node represents a directory
    // _dmtime tracks the most recent
    // _mtime among its children
    // if one of the children has a
    // _mtime more recent than the
    // current node _mtime
    // otherwise it reflects the 
    // node _mtime
    uintmax_t _dmtime;

    uintmax_t _nsi;

    Node *_parent;
    std::map<std::string, NodePtr> _children;
};

#endif

