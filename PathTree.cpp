#include "PathTree.h"

void routeSplit( std::string path, std::queue<std::string> * queue ) {
    std::stringstream stream = std::stringstream(path);
    std::string pathElement;
    while( ! stream.eof() ) {
        std::getline(stream, pathElement, '/');
        if(pathElement != ""){
            queue->push(pathElement);
        }
    }

}

PathTree::PathTree() {
    root = new treeNode();
    if( root == NULL ) {
        printf("Failed to allocate tree can not instantiate server");
        exit(1);
    }
 
    root->pathPiece="/";
}

/**
 * Search for a node by name and parent
*/
treeNode * PathTree::findChild( treeNode * parent, std::string name) {
    treeNode * current = parent->children;
    while( current != NULL ) {
        if( current->pathPiece == name ) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void PathTree::addPath( std::string path, routeInfo * routeData, httpMethods method ) {
    if( path == "/" ) {
        root->pathFunctions[method] = routeData;
    }

    std::queue<std::string> pathQueue = std::queue<std::string>();
    routeSplit( path, &pathQueue );

    struct treeNode * parentNode = root;
    
    while( ! pathQueue.empty() ) {
        std::string currentPath = pathQueue.front();
        pathQueue.pop();
        
        treeNode * currentNode;
        currentNode = findChild( parentNode, currentPath );
        if( currentNode == NULL ) {
            treeNode * newNode = new treeNode();
            if( newNode == NULL ) {
                exit(1);
            }
            treeNode * tmp = parentNode->children;
            newNode->next = tmp;
            newNode->pathPiece = currentPath;
            parentNode->children = newNode;
            currentNode = newNode;
        }
        parentNode = currentNode;
    }
    parentNode->pathFunctions[method] = routeData;
}

treeNode * PathTree::getPath( std::string path ) {
    if( path == "/") {
        return root;
    }

    std::queue<std::string> pathQueue = std::queue<std::string>();
    routeSplit( path, &pathQueue );

    struct treeNode * parentNode = root;
    
    while( ! pathQueue.empty() ) {
        std::string piece = pathQueue.front();
        pathQueue.pop();
        treeNode * current = parentNode->children;
        while( current != NULL ) {
            if( current->pathPiece[0] == '$' ) {
                //TODO
                //variable path so add to request info later
            }
            if( current->pathPiece == piece ) {
                break;
            }
            current = current->next;
        }
        if( current == NULL ) {
            return NULL;
        }
        parentNode = current;
    }
    return parentNode;
}