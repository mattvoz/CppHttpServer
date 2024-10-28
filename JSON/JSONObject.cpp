#include "JSON.h"
#include <sstream>
#include <iostream>
JSONObject::JSONObject() {}

JSONObject::JSONObject(std::string string)
{
    std::stringstream stream = std::stringstream(string);
    this->parseString(&stream);
}

JSONElement *JSONObject::operator[](std::string key)
{
    struct JSONHOLDER *cur = this->data[hash(key)];

    while (cur != NULL)
    {
        if (cur->key == key)
        {
            return cur->data;
        }
        cur = cur->next;
    }

    return (JSONElement *)NULL;
}

JSONElement *JSONObject::operator[](char *key)
{
    struct JSONHOLDER *cur = this->data[hash(std::string(key))];

    while (cur != NULL)
    {
        if (cur->key == key)
        {
            return cur->data;
        }
        cur = cur->next;
    }

    return (JSONElement *)NULL;
}

void JSONObject::parseString(std::stringstream *stream)
{
    char currentChar;
    bool escaped;
    bool inString;

    bool searchingForKey = true;
    bool endOfKey = false;
    bool endOfValue = false;

    std::string key;

    char x = stream->peek();
    std::cout << x << std::endl;

    if (x != '{')
    {
        std::cout << "invalid start char" << std::endl;
        throw invalidJSONException("OBJECT DID NOT START WITH {");
    }

    stream->get();

    while (!stream->eof())
    {
        std::cout << "stream position" << stream->tellg() << std::endl;
        std::cout << "remaining stream characters " << stream->str() << std::endl;
        x = stream->peek();

        std::cout << "CURRENT CHAR IS " << x << '\n';

        if (x == ' ' || x == '\n')
        {
            stream->get();
            continue;
        }

        std::cout << "END OF KEY IS " << endOfKey << '\n';
        if (endOfKey && x != ':')
        {
            throw new invalidJSONException("DID NOT FIND : AFTER KEY AT ");
        }
        else if (endOfKey)
        {
            stream->get();
            endOfKey = false;
            continue;
        }

        std::cout << "END OF VALUE IS " << endOfValue << '\n';

        if (endOfValue && x == '}')
        {
            break;
        }
        else if (endOfValue && x == ',')
        {
            stream->get();
            endOfValue = false;
            searchingForKey = true;
            continue;
        }
        else if (endOfValue)
        {
            std::cout << ", NOT FOUND";
            throw new invalidJSONException("DID NOT FIND , AFTER VALUE ");
        }

        if (searchingForKey)
        {
            if (x != '"')
            {
                throw invalidJSONException("KEY DID NOT START WITH \"");
            }
            key = parseJSONString(stream);
            std::cout << key << std::endl;
            searchingForKey = false;
            endOfKey = true;
        }
        else
        {
            if (x == '"')
            {
                std::string jsonString = parseJSONString(stream);
                this->put(key, jsonString);
            }
            else if (x == '{')
            {
                this->put(key, this->parseSubObject(stream));
            }
            else if (x == '[')
            {
                std::cout << "stream before operation " << stream->str();
                JSONArray *arr = JSONArray::parseJSONArray(stream);
                this->put(key, arr);
            }
            else if ((x >= 48 && x <= 57) || x == '-')
            {
                double num;
                *stream >> num;
                std::cout << "num string is " << num << "\n";
                this->put(key, num);
            }
            else if (x == 'n' || x == 't' || x == 'f')
            {
                std::string tmpStr;
                getline(*stream, tmpStr, ',');
                stream->putback(',');
                
                std::cout << "string is " << tmpStr << std::endl;
                if (tmpStr == "false")
                {
                    this->put(key, false);
                }
                else if (tmpStr == "true")
                {
                    this->put(key, true);
                }
                else if (tmpStr == "null")
                {
                    this->put(key);
                }else{
                    std::string errString = "INVALID CHARACTER FOUND AT POSITION " + std::to_string(stream->tellg());
                    throw invalidJSONException(errString.c_str());
                }
            }
            else
            {
                std::string string = std::string("UNEXPECTED VALUE STARTER GOT ");
                string += x;
                throw invalidJSONException(string.c_str());
            }

            stream->sync();
            endOfValue = true;
            searchingForKey = true;
        }
    }
}

// TODO
JSONObject *JSONObject::parseSubObject(std::stringstream *stream)
{
    return NULL;
}


unsigned int JSONObject::hash(std::string key){
    int hashVal = 0;
    for (int i = 0; i < key.length(); i++)
    {
        hashVal += ((key[i] << 2) >> 4) * 17;
    }

    return hashVal % size;
}

void JSONObject::put(std::string key, std::string val)
{
    unsigned int hashVal = hash(key);

    struct JSONHOLDER *cur = this->data[hashVal];
    if (cur == NULL)
    {
        struct JSONHOLDER *holder = new struct JSONHOLDER();
        holder->data = new JSONElement(val);
        holder->next = NULL;
        holder->key = key;
        this->data[hashVal] = holder;
        return;
    }

    while (cur->next != NULL)
    {
        if (cur->key == key)
        {
            if (cur->data->type != NULLTYP)
            {
                delete cur->data->data;
            }
            cur->data->data = new std::string(val);
            cur->data->type = STRING;
            return;
        }
        cur = cur->next;
    }

    if (cur->key == key)
    {
        if (cur->data->type != NULLTYP)
        {
            delete cur->data->data;
        }
        cur->data->data = new std::string(val);
        cur->data->type = STRING;
        return;
    }

    struct JSONHOLDER *holder = new struct JSONHOLDER();
    holder->data = new JSONElement(val);
    holder->key = key;
    holder->next = NULL;

    cur->next = holder;
}

void JSONObject::put(std::string key, double val)
{
    unsigned int hashVal = hash(key);

    struct JSONHOLDER *cur = this->data[hashVal];
    if (this->data[hashVal] == NULL)
    {
        struct JSONHOLDER *holder = new struct JSONHOLDER();
        holder->data = new JSONElement(val);
        holder->key = key;
        holder->next = NULL;
        this->data[hashVal] = holder;
        return;
    }

    while (cur->next != NULL)
    {
        if (cur->key == key)
        {
            if (cur->data->type != NULLTYP)
            {
                delete cur->data->data;
            }
            cur->data->data = new double(val);
            cur->data->type = BOOLEAN;
            return;
        }
        cur = cur->next;
    }

    if (cur->key == key)
    {
        if (cur->data->type != NULLTYP)
        {
            delete cur->data->data;
        }
        cur->data->data = new double(val);
        cur->data->type = BOOLEAN;
        return;
    }

    struct JSONHOLDER *holder = new struct JSONHOLDER();
    holder->data = new JSONElement(val);
    holder->key = key;
    holder->next = NULL;

    cur->next = holder;
}

void JSONObject::put(std::string key, bool boolean)
{
    unsigned int hashVal = hash(key);

    struct JSONHOLDER *cur = this->data[hashVal];
    if (this->data[hashVal] == NULL)
    {
        struct JSONHOLDER *holder = new struct JSONHOLDER();
        holder->data = new JSONElement(boolean);
        holder->key = key;
        holder->next = NULL;
        this->data[hashVal] = holder;
        return;
    }

    while (cur->next != NULL)
    {
        if (cur->key == key)
        {
            if (cur->data->type != NULLTYP)
            {
                delete cur->data->data;
            }
            cur->data->data = new bool(boolean);
            cur->data->type = NUMBER;
            return;
        }
        cur = cur->next;
    }

    if (cur->key == key)
    {
        if (cur->data->type != NULLTYP)
        {
            delete cur->data->data;
        }
        cur->data->data = new bool(boolean);
        cur->data->type = NUMBER;
        return;
    }

    struct JSONHOLDER *holder = new struct JSONHOLDER();
    holder->data = new JSONElement(boolean);
    holder->key = key;
    holder->next = NULL;

    cur->next = holder;
}

void JSONObject::put(std::string key, JSONObject *val)
{
    unsigned int hashVal = hash(key);

    struct JSONHOLDER *cur = this->data[hashVal];
    if (this->data[hashVal] == NULL)
    {
        struct JSONHOLDER *holder = new struct JSONHOLDER();
        holder->data = new JSONElement(val);
        holder->key = key;
        holder->next = NULL;
        this->data[hashVal] = holder;
        return;
    }

    while (cur->next != NULL)
    {
        if (cur->key == key)
        {
            if (cur->data->type != NULLTYP)
            {
                delete cur->data->data;
            }
            cur->data->data = val;
            cur->data->type = OBJECT;
            return;
        }
        cur = cur->next;
    }

    if (cur->key == key)
    {
        if (cur->data->type != NULLTYP)
        {
            delete cur->data->data;
        }
        cur->data->data = val;
        cur->data->type = OBJECT;
        return;
    }

    struct JSONHOLDER *holder = new struct JSONHOLDER();
    holder->data = new JSONElement(val);
    holder->key = key;
    holder->next = NULL;

    cur->next = holder;
}

void JSONObject::put(std::string key, JSONArray *val)
{
    unsigned int hashVal = hash(key);

    struct JSONHOLDER *cur = this->data[hashVal];
    if (this->data[hashVal] == NULL)
    {
        struct JSONHOLDER *holder = new struct JSONHOLDER();
        holder->data = new JSONElement(val);
        holder->key = key;
        holder->next = NULL;
        this->data[hashVal] = holder;
        return;
    }

    while (cur->next != NULL)
    {
        if (cur->key == key)
        {
            if (cur->data->type != NULLTYP)
            {
                delete cur->data->data;
            }
            cur->data->data = val;
            cur->data->type = ARRAY;
            return;
        }
        cur = cur->next;
    }

    if (cur->key == key)
    {
        if (cur->data->type != NULLTYP)
        {
            delete cur->data->data;
        }
        cur->data->data = val;
        cur->data->type = ARRAY;
        return;
    }

    struct JSONHOLDER *holder = new struct JSONHOLDER();
    holder->data = new JSONElement(val);
    holder->key = key;
    holder->next = NULL;

    cur->next = holder;
}

void JSONObject::put(std::string key)
{
    unsigned int hashVal = hash(key);

    struct JSONHOLDER *cur = this->data[hashVal];
    if (this->data[hashVal] == NULL)
    {
        struct JSONHOLDER *holder = new struct JSONHOLDER();
        holder->data = new JSONElement();
        holder->key = key;
        holder->next = NULL;
        this->data[hashVal] = holder;
        return;
    }

    while (cur->next != NULL)
    {
        cur = cur->next;
        if (cur->key == key)
        {
            cur->data->data;
            return;
        }
    }

    struct JSONHOLDER *holder = new struct JSONHOLDER();
    holder->data = new JSONElement();
    holder->key = key;
    holder->next = NULL;

    cur->next = holder;
}