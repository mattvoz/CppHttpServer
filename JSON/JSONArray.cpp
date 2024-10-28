#include "JSON.h"
#include <sstream>
#include <iostream>

JSONArray::JSONArray()
{
    arr = std::vector<JSONElement *>();
}

JSONArray::JSONArray(int size)
{
    arr = std::vector<JSONElement *>(size);
}

JSONArray *JSONArray::parseJSONArray(std::string string)
{
    return parseJSONArray(new std::stringstream(string));
}

JSONArray *JSONArray::parseJSONArray(std::stringstream *stream)
{
    bool end = false;
    char curChar = stream->peek();

    std::cout << "parsing JSON array with string " << stream->str() << std::endl;

    int i = 0;

    if (curChar != '[')
    {
        throw new invalidJSONException("invalid JSON array does not start with [");
    }

    stream->get();

    JSONArray *arr = new JSONArray();

    while (!stream->eof() && !end)
    {
        curChar = stream->peek();
        std::cout << stream->str() << std::endl;
        printf("got char %c when parsing array int value is %d\n", curChar, curChar);
        if (curChar == ']')
        {
            stream->get();
            break;
        }
        else if (curChar == '"')
        {
            std::string str = parseJSONString(stream);
            arr->arr.push_back(new JSONElement(str));
        }
        else if (curChar == '{')
        {
            arr->arr.push_back(new JSONElement(JSONObject::parseSubObject(stream)));
        }
        else if (curChar == '[')
        {
            arr->arr.push_back(new JSONElement(parseJSONArray(stream)));
        }
        else if ((curChar > 0x29 && curChar < 0x40) || curChar == '-')
        {
            double num;
            *stream >> num;
            arr->arr.push_back(new JSONElement(num));
        }
        else if (curChar == ' ' || curChar == ',')
        {
            stream->get();
            // There is a space just continue or we have reached a value split
            continue;
        }
        else
        {
            std::string errString = (std::string("UNEXPECTED VALUE GOT IN JSON ARRAY GOT CHAR: " ) + curChar + " IN POSITION ") + std::to_string(i);
            throw invalidJSONException(errString.c_str());
        }
        i++;
    }

    printf("finished parsing JSON array");
    return arr;
}

JSONElement *JSONArray::operator[](int i)
{
    while (i > arr.size())
    {
        arr.push_back(new JSONElement());
    }
    return arr[i];
}

void JSONArray::put(int i, std::string str)
{
    while (i > arr.size())
    {
        arr.push_back(new JSONElement());
    }

    if (arr[i] != NULL)
    {
        delete arr[i];
    }

    arr[i] = new JSONElement(str);
}

void JSONArray::put(int i, double number)
{
    while (i > arr.size())
    {
        arr.push_back(new JSONElement());
    }

    if (arr[i] != NULL)
    {
        delete arr[i];
    }

    arr[i] = new JSONElement(number);
}

void JSONArray::put(int i, bool boolean)
{
    while (i > arr.size())
    {
        arr.push_back(new JSONElement());
    }

    if (arr[i] != NULL)
    {
        delete arr[i];
    }

    arr[i] = new JSONElement(boolean);
}

void JSONArray::put(int i, JSONObject *obj)
{
    if (arr.size() <= i)
    {
        throw invalidJSONException("JSON ARRAY OUT OF BOUNDS");
    }

    if (arr[i] != NULL)
    {
        delete arr[i];
    }

    arr[i] = new JSONElement(obj);
}

void JSONArray::put(int i, JSONArray *array)
{
    if (arr.size() <= i)
    {
        throw invalidJSONException("JSON ARRAY OUT OF BOUNDS");
    }

    if (arr[i] != NULL)
    {
        delete arr[i];
    }

    arr[i] = new JSONElement(array);
}

void JSONArray::deleteElement(int index)
{
    if (index >= arr.size())
    {
        throw invalidJSONException("delete index out of bounds");
    }

    delete arr[index];
}