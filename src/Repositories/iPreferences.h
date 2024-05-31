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

    template <nvs_type_t type>
    struct PreferencesIterable {
        PreferencesIterable(const char* partition_name, const char* namespace_name)
            : m_namespace_name(namespace_name)
            , m_partition_name(partition_name)
        {
        }
        PreferencesIterator begin()
        {
            nvs_iterator_t it = nvs_entry_find(m_partition_name, m_namespace_name, type);
            return iPreferences::PreferencesIterator(m_partition_name, m_namespace_name, it);
        }
        PreferencesIterator end()
        {
            return iPreferences::PreferencesIterator(m_partition_name, m_namespace_name);
        }

    private:
        const char* m_namespace_name;
        const char* m_partition_name;
    };

public:
    bool begin(const char* name, bool readOnly = false, const char* partition_label = NULL); // overwrite begin to save partition name and namespace

    template <nvs_type_t type>
    PreferencesIterable<type> iterate()
    {
        return PreferencesIterable<type>(m_partition_name, m_namespace_name);
    }
    PreferencesIterable<NVS_TYPE_ANY> items()
    {
        return iterate<NVS_TYPE_ANY>();
    }
    PreferencesIterable<NVS_TYPE_STR> strings()
    {
        return iterate<NVS_TYPE_STR>();
    }

private:
    const char* m_namespace_name;
    const char* m_partition_name;
};
