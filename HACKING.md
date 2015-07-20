# HACKING

This file is here to give you basic directions for contributing to this project.
I'll try to be as clear as possible and I hope the community will help me keep this file as complete as possible to provide everyone with all the required information to **hack** this project and **have fun** with it :).


Disclaimer
----------
**I'm NOT an experienced [boost](http://boost.org) programmer**, my main experience with C++ is with [Qt](http://qt.io). I feel this project would highly benefit from **boost** rather than **Qt** so, please, if you are an experienced boost developer point me to any best practice that comes to your mind to improve the code in this project.

Project files
----------------

Here you can understand the project files hierarchy.

```bash
# BDirTree files hierarchy
├── build # this is a convenience directory for building the project
├── dirit # the directory snapshotting code
├── externals
│   └── fnv # this is an external project used to compute hashes
├── include # contains custom header files
└── tests
    ├── data # data used by the tests
    └── dirit_test # test code for the dirit component
```

Important files
---------------
**node.{h,cpp}** defines and implements the `Node` class: this file contains the definition and implementation of the main class used to generate directory snapshots. A node represents either a *file* or a *directory*. Each node has attached metadata information with it:

1. **name**: guess what? The node name :)
2. **mtime**: the last modification time for this node. This reflects the actual **mtime** on all supported file systems.
3. **size**: this reflects the node size on the file system. It can be the actual file size if it is a size, or the size of the information on disk recording all the required data to represent the directory (**TODO**: *a reference is needed here*).
4. **dmtime**: this changes meaning based on the type of node. If this node is a *leaf* **dmtime** will be the same as the **mtime**. Otherwise, this will be the most recent **mtime** among its children.
5. **nsi**: this is an identifier for the current state of the hierarchy starting with the current node. Later in this file you'll see how this is computed.


Glossary
---------

* **FNV**: this is the hash function chosen for computing a unique identifier that comes together with each node and represents the state of the hierarchy starting at the node. In particular, the **FNV-1a** variant has been chosen. I chose this function as this seemed to me a good compromise between speed and accuracy. This is a *non-cryptographic* hash function as I'm only interested in producing a unique identifier for representing the current state of the hierarchy. Further details regarding the hash function implementation can be found [here](http://www.isthe.com/chongo/tech/comp/fnv/index.html).

