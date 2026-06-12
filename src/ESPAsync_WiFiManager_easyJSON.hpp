// MIT License

#ifndef EASYJSONVARIANT_H
#define EASYJSONVARIANT_H

// Include Arduino.h before stdlib.h to avoid conflict with atexit()
#include <Arduino.h>

#include <map>



class EasyJSONVariant
{
public:
    enum Type
    {
        UNDEFINED,
        OBJECT,
        ARRAY,
        STRING,
        INT,
        FLOAT,
        BOOL,
        NULLTYPE
    };

    EasyJSONVariant() : _type(UNDEFINED) {}
    EasyJSONVariant(Type t) : _type(t) {}
    EasyJSONVariant(const char* s) : _type(STRING), _strValue(s) {}
    EasyJSONVariant(int i) : _type(INT)
    {
        _number.intValue = i;
    }
    EasyJSONVariant(float f) : _type(FLOAT)
    {
        _number.floatValue = f;
    }
    EasyJSONVariant(bool b) : _type(BOOL)
    {
        _number.boolValue = b;
    }

    Type getType() const { return _type; }

    const char* asString() const { return _strValue.c_str(); }
    int asInt() const { return _number.intValue; }
    float asFloat() const { return _number.floatValue; }
    bool asBool() const { return _number.boolValue; }
    String toString() const;

private:
    Type _type;
    String _strValue;
    union {
        int intValue;
        float floatValue;
        bool boolValue;
    } _number;
};


#endif // EASYJSONVARIANT_H
