/*
  EasyJSONObject.h - EasyJSONBase class used internally.
  Copyright (c) 2024 Peter Kaleja.  All right reserved.

  MIT License
*/

#ifndef EASYJSONOBJECT_H
#define EASYJSONOBJECT_H

// Include Arduino.h before stdlib.h to avoid conflict with atexit()
#include <Arduino.h>
#include <map>

/*
#include "EasyJSONBase.h"
#include "EasyJSONVariant.h"

class EasyJSONObject : public EasyJSONBase
{
public:
    EasyJSONObject() {}
    ~EasyJSONObject() {}

    void set(const String& key, const EasyJSONVariant& value) {
        _data[key] = value;
    }

    EasyJSONVariant get(const String& key) const {
        auto it = _data.find(key);
        if (it != _data.end()) {
            return it->second;
        }
        return EasyJSONVariant(EasyJSONVariant::UNDEFINED);
    }

    bool contains(const String& key) const {
        return _data.find(key) != _data.end();
    }

    void remove(const String& key) {
        _data.erase(key);
    }

    size_t size() const {
        return _data.size();
    }

private:
    std::map<String, EasyJSONBase*> _data;
};

*/

#endif // EASYJSONOBJECT_H
