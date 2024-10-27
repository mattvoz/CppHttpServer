#include "JSON.h"
#include <sstream>
#include <iostream>

invalidJSONException::invalidJSONException(const char *message)
{
    msg = std::string(message);
}

const char *invalidJSONException::what() const throw()
{
    return msg.c_str();
}

JSONObject::JSONObject() {}

JSONObject::JSONObject(std::string string)
{
    std::stringstream stream = std::stringstream(string);
    this->parseString(&stream);
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

    char x;
    stream->get(x);
    std::cout << x << std::endl;

    if (x != '{')
    {
        std::cout << "invalid start char" << std::endl;
        throw invalidJSONException("OBJECT DID NOT START WITH {");
    }

    while (!stream->eof())
    {
        std::cout << "parsing stream" << std::endl;
        stream->get(x);

        std::cout << "CURRENT CHAR IS " << x << '\n';

        if (x == ' ' || x == '\n')
        {
            continue;
        }

        std::cout << "END OF KEY IS " << endOfKey << '\n';
        if (endOfKey && x != ':')
        {
            throw new invalidJSONException("DID NOT FIND : AFTER KEY AT ");
        }
        else if (endOfKey)
        {
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
                stream->putback(x);
                JSONArray *arr = JSONArray::parseJSONArray(stream);
                this->put(key, arr);
            }
            else if ((x > 0x29 && x < 0x40) || x == '-')
            {
                stream->putback(x);
                double num;
                *stream >> num;
                std::cout << "num string is " << num << "\n";
                this->put(key, num);
            }
            else if (x == 'n' || x == 't' || x == 'f')
            {
                stream->putback(x);
                std::string tmpStr;
                *stream >> tmpStr;
                std::cout << "string is " << tmpStr;
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
                }
            }
            else
            {
                std::string string = std::string("UNEXPECTED VALUE STARTER GOT ");
                string += x;
                throw invalidJSONException(string.c_str());
            }

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

unsigned int JSONObject::hash(std::string key)
{
    int hashVal = 0;
    for (int i = 0; i < key.length(); i++)
    {
        hashVal += ((key[i] << 2) >> 4) * 17;
    }

    return hashVal % size;
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
    char x = stream->get();

    printf("parsing JSON array\n");

    int i = 0;

    if (x != '[')
    {
        throw new invalidJSONException("invalid JSON array does not start with [");
    }

    i++;

    JSONArray *arr = new JSONArray();

    while (!stream->eof() && !end)
    {
        x = stream->get();
        printf("got char %c when parsing array", x);
        if (x == ']')
        {
            return arr;
        }
        else if (x == '"')
        {
            std::string str = parseJSONString(stream);
            arr->arr.push_back(new JSONElement(str));
        }
        else if (x == '{')
        {
            arr->arr.push_back(new JSONElement(JSONObject::parseSubObject(stream)));
        }
        else if (x == '[')
        {
            arr->arr.push_back(new JSONElement(parseJSONArray(stream)));
        }
        else if ((x > 0x29 && x < 0x40) || x == '-')
        {
            stream->putback(x);
            double num;
            *stream >> num;
            arr->arr.push_back(new JSONElement(num));
        }
        else if (x == ' ' || x == ',')
        {
            i++;
            // There is a space just continue or we have reached a value split
            continue;
        }
        else if (x == ']')
        {
            break;
        }
        else
        {
            std::string errString = (((std::string() + "UNEXPECTED VALUE GOT IN JSON ARRAY GOT CHAR: ") + x) + "IN POSITION ") + std::to_string(i);
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

static std::string parseJSONString(std::stringstream *stream)
{
    std::string returnString = std::string();
    bool escaped = false;
    char currentChar;
    stream->get(currentChar);
    while (!stream->eof())
    {

        std::cout << currentChar << "\n";

        if (!escaped && currentChar == '"')
        {
            break;
        }

        if (!escaped && currentChar == '\\')
        {
            escaped = true;
            stream->get(currentChar);
            continue;
        }

        if (escaped)
        {
            switch (currentChar)
            {
            case 'n':
                returnString += "\n";
                break;
            case '\\':
                returnString += '\\';
                break;
            default:
                throw invalidJSONException("INVALID ESCAPED CHARACTER IN STRING");
                break;
            }
        }
        escaped = false;
        returnString += currentChar;
        stream->get(currentChar);
    }
    std::cout << "RETURNING STRING " << returnString << "\n";
    return returnString;
}