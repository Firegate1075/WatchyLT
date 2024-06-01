#pragma once

#include <Preferences.h>
#include <nvs.h>

class iPreferences : public Preferences {
public:
    struct PreferencesIterator {
        PreferencesIterator(const char* partition_name, const char* namespace_name);
        PreferencesIterator(const char* partition_name, const char* namespace_name, nvs_iterator_t iterator);

        PreferencesIterator& operator++(); // pre-increment
        PreferencesIterator operator++(int); // post-increment
        bool operator==(PreferencesIterator& other);
        bool operator!=(PreferencesIterator& other);
        const char*& operator*();

    private:
        nvs_iterator_t m_iter;
        nvs_entry_info_t m_entry_info;
        const char* m_currentKey;

        const char* m_namespace_name;
        const char* m_partition_name;
    };

    struct PreferencesIterable {
        PreferencesIterable(const char* partition_name, const char* namespace_name, nvs_type_t type)
            : m_namespace_name(namespace_name)
            , m_partition_name(partition_name)
            , m_type(type)
        {
        }
        PreferencesIterator begin()
        {
            nvs_iterator_t it = nvs_entry_find(m_partition_name, m_namespace_name, m_type);
            return iPreferences::PreferencesIterator(m_partition_name, m_namespace_name, it);
        }
        PreferencesIterator end()
        {
            return iPreferences::PreferencesIterator(m_partition_name, m_namespace_name);
        }

    private:
        const char* m_namespace_name;
        const char* m_partition_name;
        nvs_type_t m_type;
    };

public:
    bool begin(const char* name, bool readOnly = false, const char* partition_label = NULL); // overwrite begin to save partition name and namespace

    PreferencesIterable iterate(nvs_type_t type)
    {
        return PreferencesIterable(m_partition_name, m_namespace_name, type);
    }
    PreferencesIterable items()
    {
        return iterate(NVS_TYPE_ANY);
    }
    PreferencesIterable strings()
    {
        return iterate(NVS_TYPE_STR);
    }

private:
    const char* m_namespace_name;
    const char* m_partition_name;
};
