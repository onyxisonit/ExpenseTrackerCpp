#include "ExpenseSheet.h"

bool ExpenseSheet::Add(std::string_view label, double value)
{
    //see if expense exists with label
    Entry e;
    e.label = label;
    e.value = value;
    
    bool canAdd = std::find(m_entries.begin(), m_entries.end(), e) == m_entries.end(); 
    return false;
}

bool ExpenseSheet::Del(std::string_view label)
{
    return false;
}

void ExpenseSheet::List(std::ostream &os)
{
}
