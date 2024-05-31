#include "iPreferences.h"
#include "nvs_flash.h"

bool iPreferences::begin(const char* name, bool readOnly, const char* partition_label)
{
    m_namespace_name = name;
    if (partition_label)
        m_partition_name = partition_label;
    else
        m_partition_name = "nvs";

    if (_started) {
        return false;
    }
    _readOnly = readOnly;
    esp_err_t err = ESP_OK;
    if (partition_label != NULL) {
        err = nvs_flash_init_partition(partition_label);
        if (err) {
            return false;
        }
        err = nvs_open_from_partition(partition_label, name, readOnly ? NVS_READONLY : NVS_READWRITE, &_handle);
    } else {
        err = nvs_open(name, readOnly ? NVS_READONLY : NVS_READWRITE, &_handle);
    }
    if (err) {
        return false;
    }
    _started = true;
    return true;
}

iPreferences::PreferencesIterator::PreferencesIterator(const char* partition_name, const char* namespace_name)
    : m_partition_name(partition_name)
    , m_namespace_name(namespace_name)
    , m_iter(NULL)
{
}

iPreferences::PreferencesIterator::PreferencesIterator(const char* partition_name, const char* namespace_name, nvs_iterator_t iterator)
    : m_partition_name(partition_name)
    , m_namespace_name(namespace_name)
    , m_iter(iterator)
{
}

iPreferences::PreferencesIterator& iPreferences::PreferencesIterator::operator++()
{
    m_iter = nvs_entry_next(m_iter);
    return *this;
}

iPreferences::PreferencesIterator iPreferences::PreferencesIterator::operator++(int)
{
    iPreferences::PreferencesIterator old = *this;
    m_iter = nvs_entry_next(m_iter);
    return old;
}

bool iPreferences::PreferencesIterator::operator==(iPreferences::PreferencesIterator& other)
{
    return (m_iter == other.m_iter) && (m_namespace_name == other.m_namespace_name) && (m_partition_name == other.m_partition_name);
}

bool iPreferences::PreferencesIterator::operator!=(iPreferences::PreferencesIterator& other)
{
    return !operator==(other);
}

const char*& iPreferences::PreferencesIterator::operator*()
{
    nvs_entry_info(m_iter, &m_entry_info);
    m_currentKey = m_entry_info.key;
    return m_currentKey;
}
