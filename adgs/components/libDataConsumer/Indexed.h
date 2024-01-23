// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2013, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S.libDataConsumer Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 5.0  2013/06/06 20:55:21  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.1  2012/02/28 17:19:35  micmaz
 *  refactory Data circulator
 *
 *  Revision 1.4  2012/02/07 16:54:12  micmaz
 *  removed warnings.
 *
 *  Revision 1.3  2011/01/21 18:05:58  micmaz
 *  producer/consumer rework.
 *
 *  Revision 1.2  2010/11/11 16:14:19  micmaz
 *  corrected the cancel policy, added some comments.
 *
 *  Revision 1.1  2010/11/03 16:58:23  micmaz
 *  library completely reworked in order to simply its use.
 *
 *
 *
 */

#ifndef Indexed_H_
#define Indexed_H_
#include <acs_c++config.hpp>


_ACS_BEGIN_NESTED_NAMESPACE(acs, pc)

/**
 * @brief indexed generic.
 *
 * A generic indexed class describe a class with a key and a value.
 * The value is the Tp _item, while the key must be of type KeyTp.
 * The indexed class is comparable using its internal key.
 * More over an indexed concrete class can be grouped using a secondary key.
 */
template<typename IndexedItemTp, typename KeyTp>
class Indexed { // PRQA S 2109
public:
    /**
     * Empty ctor.
     */
    Indexed() : _item() {}


    /**
     * Empty dtor.
     */
    virtual ~Indexed() throw() {}

    /**
     * Returns the primary key of this object.
     */
    virtual KeyTp id() const =0;

    /**
     * Returns the belonging group of this object.
     */
    virtual KeyTp groupId() const =0;

    /**
     * This object must be comparable.
     */
    static int compareTo(Indexed const& item1, Indexed const& item2) {
        if (item1 == item2) {
            return 0;
        }
        if (item1 < item2) {
            return -1;
        }
        return 1;
    }

    /**
     * This object must be comparable.
     */
    bool operator==(Indexed const& rhl) { // PRQA S 2070
        return id() == rhl.id();
    }

    /**
     * This object must be orderable.
     */
    bool operator<(Indexed const& rhl) { // PRQA S 2070
        return id() < rhl.id();
    }

    /**
     * This object must be orderable.
     */
    bool operator>(Indexed const& rhl) { // PRQA S 2070
        return id() > rhl.id();
    }

    /**
     * Setter for the _item field.
     */
    void setItem(IndexedItemTp const& item) {
        _item = item;
    }
    /**
     * Getter for the _item field.
     */
    IndexedItemTp item() {
        return _item;
    }
protected:
    /**
     * Copy ctor.
     */
    Indexed(Indexed const& rhl) : _item(rhl._item) {}
    /**
     * Copy assigner.
     */
    Indexed& operator=(Indexed const& r) {
        if (this != &r) {
            _item = r._item;
        }
        return *this;
    }

protected:
    IndexedItemTp _item; // PRQA S 2101

};


_ACS_END_NESTED_NAMESPACE


#endif /* Indexed_H_ */
