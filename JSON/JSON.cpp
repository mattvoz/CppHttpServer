#include "JSON.h"
#include <sstream>
#include <iostream>


JSONElement::JSONElement()
{
    type = NULLTYP;
    data = NULL;
}
JSONElement::JSONElement(double num)
{
    type = NUMBER;
    data = new double(num);
}
JSONElement::JSONElement(bool boolean)
{
    type = JSON_BOOLEAN;
    data = new bool(boolean);
}
JSONElement::JSONElement(std::string string)
{
    type = STRING;
    data = new std::string(string);
}

JSONElement::JSONElement(JSONArray *array)
{
    type = ARRAY;
    data = array;
}
JSONElement::JSONElement(JSONObject *object)
{
    type = OBJECT;
    data = object;
}

void * JSONElement::value()
{
    return data;
}

JSONElement::~JSONElement()
{
    switch(type){
        case NULLTYP:
            return;
          break;
          case STRING:
            delete ((std::string * ) data);
          case NUMBER:
            delete ((double * ) data);
          break;
          case OBJECT:
            delete ((JSONObject * ) data);
          case ARRAY:
            delete ((JSONArray * ) data);
          case JSON_BOOLEAN:
            delete ((bool * ) data);
          default:
          // something went terribly wrong if the type isn't set so kill the program for now 
          //TODO throw an exception or something
          exit(-1);
            break;
    }
}

void * JSONElement::operator[](std::string key){
    switch( this->type ){
        case STRING:
            throw new invalidJSONException("String type does not have access by value");
          break;
        case NUMBER:
            throw new invalidJSONException("Number type does not have access by value");
          break;
        case OBJECT:
            return (*(JSONObject *) data)[key];
          break;
        case ARRAY:
        {
            int arrIndex = -1;

            try{
                arrIndex = std::stoi(key);
            }catch( std::exception ex ){}

            if(arrIndex < 0) {
                throw new invalidJSONException("ARRAY type does not have access by string value");
            }

            return ( * (JSONArray *) data)[arrIndex];
          break;
        }
        case JSON_BOOLEAN:
            throw new invalidJSONException("BOOLEAN type does not have access by value");
          break;
        case NULLTYP:
            throw new invalidJSONException("NULL type does not have access by value");
          break;
    }

    return NULL;
}

void * JSONElement::operator[](char * index){
    std::string key = std::string(index);
       switch( this->type ){
        case STRING:
            throw new invalidJSONException("String type does not have access by value");
          break;
        case NUMBER:
            throw new invalidJSONException("Number type does not have access by value");
          break;
        case OBJECT:
            return (*(JSONObject *) data)[key];
          break;
        case ARRAY:
        {
            int arrIndex = -1;

            try{
                arrIndex = std::stoi(key);
            }catch( std::exception ex ){}

            if(arrIndex < 0) {
                throw new invalidJSONException("ARRAY type does not have access by string value");
            }

            return ( * (JSONArray *) data)[arrIndex];
          break;
        }
        case JSON_BOOLEAN:
            throw new invalidJSONException("BOOLEAN type does not have access by value");
          break;
        case NULLTYP:
            throw new invalidJSONException("NULL type does not have access by value");
          break;
    }

    return NULL;
}

// TODO all teh following :)
bool JSONElement::operator==(JSONElement * comp){
    if( this->type != comp->type){
        return false;
    }

    return compare(comp);
}

bool JSONElement::operator==(JSONElement & comp){
        if( this->type != comp.type){
        return false;
    }

    return compare(&comp);
}

bool JSONElement::operator==(long comp){
    if( this->type != NUMBER){
      return false;
    }
    return (*(double *) data) ==  comp;
}

bool JSONElement::operator==(int comp){
    if( this->type != NUMBER){
      return false;
    }
    return (*(double *) data) == comp;
}

bool JSONElement::operator==(double comp){
    if( this->type != NUMBER){
      return false;
    }
    return (*(double *) data) == comp;
}

bool JSONElement::operator==(bool comp){
    if( this->type != JSON_BOOLEAN ){
      return false;
    }

    return (*(bool *) data) == comp;
}

bool JSONElement::operator==(char * comp){
    if( this->type != STRING ){
      return false;
    }

    return (*(std::string *) data) == std::string(comp);
}

bool JSONElement::operator==(std::string comp){
    if( this->type != STRING ){
      return false;
    }

    return (*(std::string *) data) == comp;
}

// Not sure what JSON specification is for comparing arrays marking as TODO
//TODO
bool JSONElement::operator==(JSONArray){
    return false;
}

bool JSONElement::compare( JSONElement * comp ){
    if( this->type != comp->type){
        return false;
    }
    switch( this->type ){
        case STRING:
            return (*(std::string *) data) == (*(std::string *) comp->data);
          break;
        case NUMBER:
            return (*(double *) data) == (*(double *) comp->data);
          break;
        case OBJECT:
            //just compare addresses of the object if they are the same address same object don't do deep compare at this time.
            return (JSONObject *) data == (JSONObject *) comp->data;
          break;
        case ARRAY:
            return false;
        case JSON_BOOLEAN:
            return (*(bool *) data) == (*(bool *) comp->data);
          break;
        case NULLTYP:
            // if we have a nulltyp and the type matches they must be equal in value :)
            return true;
          break;
    }

    return false;
}