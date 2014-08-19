#include "util/observer.h"

Observer::~Observer() {
    clearObservedSubjects();
}

bool Observer::isObservedSubject(const ObservableBase* pSubject) const {
    return (m_subjects.find(const_cast<ObservableBase*>(pSubject)) != m_subjects.end());
}

void Observer::clearObservedSubjects() const {
    while (!m_subjects.empty())
        removeObservedSubject(*m_subjects.begin());
}

bool Observer::addObservedSubject(const ObservableBase* pSubject) const {

    if (!pSubject)
        return false;

    std::set<const ObservableBase*>::const_iterator it = m_subjects.find(pSubject);
    if (it != m_subjects.end())
        return false;

    std::pair<std::set<const ObservableBase*>::iterator, bool> pr = m_subjects.insert(pSubject);
    if (pr.second == true) {
        pSubject->addObserver(this);
    }
    return pr.second;

}

bool Observer::removeObservedSubject(const ObservableBase* pSubject) const {

    if (!pSubject)
        return false;

    if (m_subjects.find(pSubject) != m_subjects.end()) {
        m_subjects.erase(pSubject);
        pSubject->removeObserver(this);
        return true;
    }
    else {
        return false;
    }
}


// ---------------------------------------------------------------------------

ObservableBase::~ObservableBase() {
    clearObservers();
}

bool ObservableBase::addObserver(const Observer* observer) const {

    if (!observer)
        return false;

    std::pair<std::set<const Observer*>::iterator, bool> pr = m_observers.insert(observer);
    if (pr.second)
        observer->addObservedSubject(this);

    return pr.second;
}

bool ObservableBase::removeObserver(const Observer* observer) const {

    if (!observer)
        return false;

    if (m_observers.find(observer) != m_observers.end()) {
        m_observers.erase(observer);
        observer->removeObservedSubject(this);
        return true;
    }
    else {
        return false;
    }
}

void ObservableBase::clearObservers() const {
    while (!m_observers.empty())
        removeObserver(*m_observers.begin());
}

