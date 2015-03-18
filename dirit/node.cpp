#include "node.h"
#include "logger.h"
#include "macros.h"
#include "fnv_wrapper.h"

#include <ctime>
#include <utility>
#include <stdlib.h>

Node::Node() : 
    _size(0),
    _mtime(0),
    _dmtime(0),
    _nsi(0),
    _parent(0)
{
}

Node::Node(const fs::directory_entry &entry) :
    Node::Node()
{
    boost::system::error_code ec;
    intmax_t v = fs::file_size(entry.path(), ec);
    if (fs::is_regular_file(entry.path(), ec) && v < 0) {
        err_log() << "Cannot get entry size: " << ec.message() << " for path " << entry.path() << std::endl;
    }
    _size = v;
    v = fs::last_write_time(entry.path(), ec);
    if (v == -1) {
        err_log() << "Cannot get entry last write time: " << ec.message() << std::endl;
    }
    _mtime = v;
    _dmtime = _mtime;
    _name = entry.path().filename().string();
}

Node::Node(const Node &n) :
    _name(n._name),
    _size(n._size),
    _mtime(n._mtime),
    _dmtime(n._dmtime),
    _nsi(n._nsi),
    _parent(0)
{}

Node::~Node() {
   // shared_ptr's should handle
   // the cleanup
}

std::string Node::name() const {
    return _name;
}

uintmax_t Node::size() const {
    return _size;
}

uintmax_t Node::nsi() const {
    return _nsi;
}

uintmax_t Node::mtime() const {
    return _mtime;
}

void Node::appendChild(NodePtr &n) {
    _children[n.get()->_name] = n;
    n.get()->_parent = this;
    n.get()->updateNSI(n.get()); 
}

void Node::updateNSI(Node* child) {
    Fnv_t fnv = FNV1A_INIT;

    fnv = fnv_buf((void*)_name.data(), _name.length(), fnv);
    fnv = fnv_buf((void*)&_size, sizeof(_size), fnv);
    fnv = fnv_buf((void*)&_mtime, sizeof(_mtime), fnv);
    fnv = fnv_buf((void*)&_dmtime, sizeof(_dmtime), fnv);
    if (child) {
        fnv = fnv_buf((void*)&(child->_nsi), sizeof(_nsi), fnv);
    }
    _nsi = fnv;

    if (_parent) {
        // propagate NSI change
        // to parent
        _parent->updateNSI(this);
    }
}

Node::NodePtr Node::CreateWithPath(const fs::path &p, bool *ok) {
    fs::directory_entry dirent(p);
    return _CreateWithPath(dirent, 0, ok);
}

Node::NodePtr Node::_CreateWithPath(const fs::directory_entry &entry, Node *parent, bool *ok) {
    boost::system::error_code ec;
    uint64_t v = fs::file_size(entry.path(), ec);
    if (!fs::exists(entry.path())) {
        NOT_OK(ok);
        return NodePtr(0);
    }    

    Node *n = new Node(entry);
    NodePtr nptr(n);
    if (fs::is_regular_file(entry.path(), ec)) {
        if (parent) {
            parent->appendChild(nptr);
        }
        if (ok) {
            *ok = true;
        }
        return nptr;
    } else if (fs::is_directory(entry.path(), ec)) {
        fs::directory_iterator it(entry.path(), ec);
        if (!ec) { // success
            fs::directory_iterator end;
            uintmax_t max_mtime = n->_mtime;
            for (; it != end; ++it) {
                NodePtr child = _CreateWithPath(*it, nptr.get(), ok);
                if (!ok) {
                    return NodePtr(0);
                }
                if (child.get()->_mtime > max_mtime) {
                    max_mtime = child.get()->_mtime;
                }
                if (parent) {
                    parent->appendChild(child);
                }
            }
            n->_dmtime = max_mtime;
            n->updateNSI(0); 
            if (ok) {
                *ok = true;
            }
            return nptr;
        } else {
            NOT_OK(ok);
            err_log() << "Unable to create directory iterator " << ec.message() << std::endl;
            return NodePtr(0);
        }

    } else {
        err_log() << "Unsupported file type at path " << entry.path() << std::endl; 
        NOT_OK(ok);
        return NodePtr(0);
    }
    std::cerr << "Unexpected: unhandled file type. Aborting." << std::endl;
    abort();
    NOT_OK(ok);
    return NodePtr(0);
} 

//bool Node::updateWithPath(const fs::path &path,
//        std::vector<fs::directory_entry> &deletedPaths,
//        std::vector<fs::directory_entry> &addedPaths,
//        std::vector<fs::directory_entry> &modifiedPaths) {
//
//
//}
