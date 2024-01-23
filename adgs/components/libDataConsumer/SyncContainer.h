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
 *  Revision 5.5  2014/06/25 17:29:47  marpas
 *  Logger.C and Logger.h removed
 *
 *  Revision 5.4  2013/10/28 17:33:29  marpas
 *  work in progress
 *
 *  Revision 5.3  2013/07/18 12:29:08  marpas
 *  coding best practices applied
 *  type mismatching fix in progress
 *
 *  Revision 5.2  2013/07/16 14:52:48  marpas
 *  coding best practices applied
 *  robustness and performances improved
 *
 *  Revision 5.1  2013/07/15 16:48:07  marpas
 *  qa warnings fixed
 *  robusteness improved
 *
 *  Revision 5.0  2013/06/06 20:55:21  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.1  2012/02/28 17:19:36  micmaz
 *  refactory Data circulator
 *
 *  Revision 1.10  2012/02/20 15:04:50  marpas
 *  refactoring
 *
 *  Revision 1.9  2012/02/07 16:54:12  micmaz
 *  removed warnings.
 *
 *  Revision 1.8  2011/03/22 18:01:56  micmaz
 *  resolved the unterminated packages bug
 *
 *  Revision 1.7  2011/03/03 17:45:10  marpas
 *  getting rid of boost's threads
 *
 *  Revision 1.6  2010/11/30 16:45:27  micmaz
 *  renamed pc_thread in pcUtility.
 *
 *  Revision 1.5  2010/11/22 13:00:05  micmaz
 *  added the enqueue method in the synch container.
 *
 *  Revision 1.4  2010/11/11 16:14:19  micmaz
 *  corrected the cancel policy, added some comments.
 *
 *  Revision 1.3  2010/11/05 16:39:46  micmaz
 *  The syncContainer must only be a synchronized channel. other responsibilites should be in charge to the Register.
 *
 *  Revision 1.2  2010/11/04 10:14:02  micmaz
 *  the channel is now prioritized.
 *
 *  Revision 1.1  2010/11/03 16:58:23  micmaz
 *  library completely reworked in order to simply its use.
 *
 *
 *
 */

#ifndef SYNCCONTAINER_H_
#define SYNCCONTAINER_H_


#include <ConsumingRegister.h>

#include <exException.h>

#include <string>
#include <limits>

_ACS_BEGIN_NESTED_NAMESPACE(acs, pc) ;
/**
 * @brief Synchronized container.
 *
 * This container makes the std::vector container syncrhonized.
 * This class is copyable and copy-constructible.
 * The internal Mutex will never copied, just created new for every copied instance.
 */
template<typename Tp>
class SyncContainer : public Mutex { // PRQA S 2109, 2153
public:
    /**
     *@brief  Constructor of the container: it sets the queue limit to <code> limit </code>.
     */
    explicit SyncContainer(int limit = -1) :
        Mutex(),
        _container(),
        _maxSize((limit < 0) ? std::numeric_limits<short>::max() : limit) // PRQA S 3000, 3380, 3385
    {}

    /**
     *@brief  Copy constructor of the queue.
     */
    explicit SyncContainer(std::vector<Tp> const& queue, int limit = -1) :
        Mutex(),
        _container(queue), _maxSize(limit) 
    {}

    /**
     *@brief  Copy constructor of the queue.
     */
    SyncContainer(SyncContainer const& rhl) :
        Mutex(), // do not copy
        _maxSize(rhl._maxSize),
        _container(rhl._container)
    {}

    /**
     * the container is copyable.
     */
    SyncContainer& operator=(SyncContainer const& rhl) {
        if (this != &rhl) {
            _maxSize = rhl._maxSize;
            _container = rhl._container ;
        }
        return *this;
    }

    /**
     *@brief  Empty destructor (virtual because it can be overridden).
     */
    virtual ~SyncContainer() throw() {} // PRQA S 2131

    /**
     * @brief this method is called from the consumers thread (makes a pop()).
     *
     * It ensures the synchronization on the queue.
     *
     */
    Tp pop(bool force = false) {
        Lock lk(*this);
        //        dumpChannel("Tp poll()");
        if (!force && empty()) {
            ACS_THROW(exChannelException("The Consumer has been stopped, thus cannot produce new items.")); // PRQA S 3081
        }
        Tp item = _container.back();
        _container.pop_back();
        return item;
    }

    Tp& operator[](size_t counter) { // PRQA S 2140
        Lock lk(*this);
        return _container[counter];
    }

	/**
	 * @brief this method is used to verify if the item is in the internal container
	 *
	 */
	bool hasItem (Tp const &item) {
		Lock lk(*this);
		return find (_container.begin(), _container.end(), item) != _container.end();
	}


    /**
     * @brief this method is called from the producers thread (makes a push_back()).
     *
     * It ensures the synchronization on the queue.
     *
     */
    bool push(Tp const &item) {
        Lock lk(*this);
        _container.push_back(item);
        return true;
    }
    /**
     * @brief this method is called in order to enqueue an item (will popped as last).
     *
     * It ensures the synchronization on the queue.
     *
     */
    bool enqueue(Tp const &item) {
        Lock lk(*this);
        // push front
        _container.insert(_container.begin(), item);
        return true;
    }

    void clear() {
        Lock lk(*this);
        _container.clear();
    }

    /**
     * @brief Gets the channel's size.
     */
    size_t size() const {
        Lock lk(*this);
        return _container.size();
    }

    /**
     * @brief Gets the channel's size.
     */
    bool empty() const {
        Lock lk(*this);
        return _container.empty();
    }

    /**
     * Unlike the std::vector<>::erase() method, this method will return the erased item.
     *
     */
    Tp erase(size_t index) {
        Lock lk(*this);
        Tp itemToReturn = _container.at(index);
        _container.erase(_container.begin() + index);
        return itemToReturn;
    }

    /**
     * Only for debug purpose: print out the content of this container.
     */
    void dumpChannel(std::string const& message) const {
        Lock lk(*this);
        std::ostringstream os;
        os << " *********************** QUEUE INSPECTION " << size() << message << '\n' ;
        // wait for within_msec milli-seconds to make sure no new tasks coming in...
        for (unsigned int counter = 0; counter < size(); ++counter) {
            os << pthread_self();
            os << this->operator[](counter).str() << '\n' ;
        }
        os << " *********************** END QUEUE INSPECTION " << '\n'  ;
        std::cout << os.str() << '\n' ;
    }
protected:
    std::vector<Tp> _container; // PRQA S 2101 2
    size_t _maxSize;


};

_ACS_END_NESTED_NAMESPACE

#endif /* SYNCCONTAINER_H_ */
