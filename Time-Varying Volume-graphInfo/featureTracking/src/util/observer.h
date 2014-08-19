#ifndef OBSERVER_H
#define OBSERVER_H

#include <set>
#include <vector>
class ObservableBase;

/**
 * \brief Quite general implementation of the observer design pattern, takes
 * care of relationship management between observers and observed objects.
 *
 * Derive from this class in order to define a specialized Observer with
 * custom notification functions that can be registered at Observable objects
 * of matching type.
 *
 * @note It is possible to modify the observation state of const objects.
 *
 * @see Observable
 */
class Observer {

friend class ObservableBase; // for allowing ObservableBase to call addObservedSubject() and removeObservedSubject()

public:

    /**
     * Destructor which removes this Observer object from all Observable objects
     * which it is registered at. Thus, the user does not need to remove this
     * object from any Observable object, unless he wants to explicitly
     * stop observing it.
     */
    virtual ~Observer();

    /**
     * Returns whether this Observer currently observes the passed Observable
     */
    bool isObservedSubject(const ObservableBase* pSubject) const;

    /**
     * Adds the passed Observable object to those objects that are observed by
     * this observable and registers itself at the passed object.
     * An observer can only be added once.
     *
     * @param   pSubject the object that is to be observed
     * @return  true if the object has been inserted successfully, false
     *          if the null pointer are on an already registered object was passed.
     */
    bool addObservedSubject(const ObservableBase* pSubject) const;

    /**
     * Removes the passed Observable object from the list of observed objects, if
     * it has been observed, and deregisters itself at the passed object.
     *
     * @param   pSubject the observed object that is to be removed
     * @return  true if the passed object was removed, false if it
     *          was not registered.
     */
    bool removeObservedSubject(const ObservableBase* pSubject) const;

    /**
     * Clears the list of all observed objects. Like the destructor, this method
     * removes this object itself from Observable objects.
     */
    void clearObservedSubjects() const;

    /// Set of observed objects
    /// (mutable since we want to allow adding observeds to const objects)
    mutable std::set<const ObservableBase*> m_subjects;
};


// ---------------------------------------------------------------------------

/**
 * Base class for Observable classes. Do not subclass it directly!
 *
 * @see Observable
 */
class ObservableBase {

friend class Observer; // for allowing Observer to call addObserver() and removeObserver()

public:

    /// @see Observable
    virtual ~ObservableBase();

protected:

    /// @see Observable
    virtual bool addObserver(const Observer* observer) const;

    /// @see Observable
    virtual bool removeObserver(const Observer* observer) const;

     /// @see Observable
    void clearObservers() const;

    /// Set of observers.
    /// (mutable since we want to allow adding observers to const objects)
    mutable std::set<const Observer*> m_observers;

};

// ---------------------------------------------------------------------------

/**
 * \brief Base class for Observable classes, takes care of relationship management
 * between observers and observed objects. Subclass it in order to allow
 * Observers of a certain type to monitor instances of the derived class.
 *
 * @param T Type of observers that are to be allowed to register
 *      at the derived class.
 *
 * @note It is possible to modify the observation state of const objects.
 *
 * @see Observer
 */
template<class T>
class Observable : public ObservableBase {

public:

    /**
     * Besides simply destroying this object, the destructor removes this object
     * from the list of all Observer objects watching it. The user does therefore
     * not need to manually remove this object from all its observers.
     */
    virtual ~Observable() {}

    /**
     * Adds the passed Observer to the list of observing ones and registers
     * this object at the passed utils/observer.
     *
     * @note The observer must be of type T and can only be inserted once.
     *
     * @param observer Observer of type T to be inserted
     * @return true if the insertion has been successful, false otherwise
     */
    bool addObserver(const Observer* observer) const;

    /**
     * Removes the passed Observed object from the list of observers observing
     * this object and de-registers this object from the passed utils/observer.
     *
     * @param observer the observer to be removed
     * @return true if the observer was removed, false if the passed object
     *  is not registered at this observable.
     */
    bool removeObserver(const Observer* observer) const;

    /**
     * Returns whether this object is observed by the
     * passed one.
     */
    bool isObservedBy(const Observer* observer) const;

protected:

    /**
     * Returns all observers currently registered at this observable.
     */
    const std::vector<T*> getObservers() const;
};


// ---------------------------------------------------------------------------
//   Template Definitions
// ---------------------------------------------------------------------------

template<class T>
bool Observable<T>::addObserver(const Observer* observer) const {

    if (!dynamic_cast<const T*>(observer)) {
        //tgtAssert(false, "Object of template type expected!");
        //LERRORC("voreen.Observable<T>", "addObserver() : Object of type T expected");
        return false;
    }

    return ObservableBase::addObserver(observer);
}

template<class T>
bool Observable<T>::removeObserver(const Observer* observer) const {
    return ObservableBase::removeObserver(observer);
}

template<class T>
bool Observable<T>::isObservedBy(const Observer* observer) const {
    return (m_observers.find(observer) != m_observers.end());
}

template<class T>
const std::vector<T*> Observable<T>::getObservers() const {
    std::set<const Observer*>::const_iterator it;
	std::vector<T*> typedObservers;
    typedObservers.reserve(m_observers.size());
    for (it = m_observers.begin(); it != m_observers.end(); ++it) {
        // note: the only way to get elements into the m_observers vector is by
        // calling addObserver() that does a type check => static_cast is safe here
        typedObservers.push_back(static_cast<T*>(const_cast<Observer*>(*it)));
    }

    return typedObservers;
}

#endif
