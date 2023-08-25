#ifndef __PATH__TREE__
#define __PATH__TREE__

#include <string>
#include <queue>
#include <sstream>
#include "httpRequest.h"
#include "httpResponse.h"

enum httpMethods{ GETR, PUTR, POSTR, RDELETER, HEADR,  };

struct routeInfo{
	void (* func) (httpRequest * req, httpResponse * res);
	void (* middleWare[10]) (void * req, void * res);
    bool staticPath;
};

struct treeNode {
    std::string pathPiece;
    struct routeInfo * pathFunctions[10];
    treeNode * children;
    treeNode * next;
};


class PathTree {
    public:
        PathTree();
        void addPath(std::string path, routeInfo * info, httpMethods method);
        treeNode * getPath( std::string path );
    private:
        static treeNode * findChild( treeNode * parent, std::string name);
        struct treeNode * root;
};
#endif