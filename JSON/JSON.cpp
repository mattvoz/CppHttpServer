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
    type = BOOLEAN;
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

void *JSONElement::value()
{
    return data;
}

JSONElement::~JSONElement()
{
    if (type == NULLTYP)
    {
        return;
    }
    delete (data);
}