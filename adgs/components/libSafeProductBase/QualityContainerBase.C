// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  $

	$Id$

	$Author$
	$Log$
	Revision 1.4  2013/02/25 16:03:15  aleber
	Reed-Solomon parameters in QualityInformation section handled

	Revision 1.3  2011/08/31 12:12:37  lucio.pulvirenti
	Copy CTOR and = operator fixed

	Revision 1.2  2011/07/16 17:05:29  aleber
	numOfPackets added in QualityContainerBase constructor

	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
	First implementation


*/

#include <QualityContainerBase.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(QualityContainerBase)


// QualityContainerBase CLASS
QualityContainerBase::QualityContainerBase(
	const string &dataObjectId,
	long int numOfPackets,
	long int numOfRSIncorrigibleElements,
	long int numOfRSCorrectedElements,
	long int numOfRSCorrectedSymbols) :

  _dataObjectId(dataObjectId),
  _numOfPackets(numOfPackets),
  _missingElements(),
  _corruptedElements(),
  _numOfRSIncorrigibleElements(numOfRSIncorrigibleElements),
  _numOfRSCorrectedElements(numOfRSCorrectedElements),
  _numOfRSCorrectedSymbols(numOfRSCorrectedSymbols)
{
    static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
}

QualityContainerBase::~QualityContainerBase() throw() {}

QualityContainerBase::QualityContainerBase(const QualityContainerBase& p):
  _dataObjectId(p._dataObjectId),
  _numOfPackets(p._numOfPackets),
  _missingElements(p._missingElements),
  _corruptedElements(p._corruptedElements),
  _numOfRSIncorrigibleElements(p._numOfRSIncorrigibleElements),
  _numOfRSCorrectedElements(p._numOfRSCorrectedElements),
  _numOfRSCorrectedSymbols(p._numOfRSCorrectedSymbols)
{
}

QualityContainerBase &QualityContainerBase::operator=(const QualityContainerBase& p)
{
 	if (this != &p) {
		_dataObjectId = p._dataObjectId;
		_numOfPackets = p._numOfPackets;
		_missingElements = p._missingElements;
		_corruptedElements = p._corruptedElements;
		_numOfRSIncorrigibleElements = p._numOfRSIncorrigibleElements;
		_numOfRSCorrectedElements = p._numOfRSCorrectedElements;
		_numOfRSCorrectedSymbols = p._numOfRSCorrectedSymbols;
 	}
 	return *this ;
}

void QualityContainerBase::addQualityMissingElements(
                   int count,
				   const string &content,
				   const string & cause,
				   int after,
				   int before,
				   int following,
				   int preceding,
 				   int over,
                   const string & startTime,
                   const string & stopTime)
{
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "EFC-17 passed arguments: ") ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t count        = " << count) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t content      = " << content) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t cause        = " << cause) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t after        = " << after) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t before       = " << before) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t following    = " << following) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t preceding    = " << preceding) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t countOver    = " << over) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t startTime    = " << startTime) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t stopTime     = " << stopTime) ;
 	ACS_CLASS_END_DEBUG

	QualityLocation loc;
	loc.setContent(content);
	loc.setAfter(after);
	loc.setBefore(before);
	loc.setFollowing(following);
	loc.setPreceding(preceding);
	loc.setStart(startTime);
	loc.setStop(stopTime);

	QualityMissingElements mE;
	mE.setQualityLocation(loc);
	mE.setCount(count);
	mE.setOver(over);
	mE.setCause(cause);

	_missingElements.push_back(mE);
}

void QualityContainerBase::addQualityCorruptedElements(
                    int count,
				    const string & content,
				    int after,
				    int before,
				    int following,
				    int preceding,
				    const string & type,
				    const string & other_type,
				    int severity,
 				    int over,
                    const string & startTime,
                    const string & stopTime)
{
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "passed arguments: ") ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t count        = " << count) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t content      = " << content) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t after        = " << after) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t before       = " << before) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t following    = " << following) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t preceding    = " << preceding) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t type         = " << type) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t otherType    = " << other_type) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t severity     = " << severity) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t countOver    = " << over) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t startTime    = " << startTime) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t stopTime     = " << stopTime) ;
 	ACS_CLASS_END_DEBUG

	QualityLocation loc;
	loc.setContent(content);
	loc.setAfter(after);
	loc.setBefore(before);
	loc.setFollowing(following);
	loc.setPreceding(preceding);
	loc.setStart(startTime);
	loc.setStop(stopTime);

	QualityCorruptedCause cc;
	cc.setType(type);
	cc.setOtherType(other_type);
	cc.setSeverity(severity);

	QualityCorruptedElements cE;
	cE.setQualityLocation(loc);
	cE.setCount(count);
	cE.setOver(over);
	cE.setCause(cc);

	_corruptedElements.push_back(cE);
}
void QualityContainerBase::setQualityMissingElements(const vector<QualityMissingElements>& mE) // PRQA S 4121
{
	_missingElements = mE;
}

void QualityContainerBase::setQualityCorruptedElements(const vector<QualityCorruptedElements>& cE) // PRQA S 4121
{
	_corruptedElements = cE;
}

void QualityContainerBase::resetQualityMissingElements()
{
	_missingElements.clear();
}

void QualityContainerBase::resetQualityCorruptedElements()
{
	_corruptedElements.clear();
}

/**
 * \brief Groups Corrupted and Missing Elements if their number exceeds a (configured) thresholdi MaxNumberOfUnits.
 * Elements are grouped if they are "closer" than a given threshold MaxUnitsGroupingDistance.
 * This method can be applied ONLY if there is no more than ONE "preceding" element as FIRST element!
 * \param maxNumOfUn = Maximum allowed number of Missing Units or Corrupted Units in the Manifest file
 * \param maxUnDist = Distance between location of two consecutive Missing/Corrupted units below which units are grouped
 * \param st = maxUnDist is incremented by "st" and the grouping algo is repeated  until iMissing/Corrupted units
 * are reduced below the threshold MaxNumberOfUnits
 * \param st = dataUnitXpath is the string (representing the data unit Xpath) to appear in the tag <location>
 * of the <MissingElements> or <CorruptedElements> tags in the manifest file
 */

void QualityContainerBase::groupQualityElements(unsigned int maxNumOfUn,
                                                unsigned int maxUnDist,
                                                unsigned int st,
                                                const string &dataUnitXpath)
{
	// unsigned int overTotal=0;
    string startTimeTotalStr="";
    string stopTimeTotalStr="";
    string corruptedType="";
    // unsigned int startTimeTotal=0;
    // unsigned int stopTimeTotal=0;

	unsigned int maxNumberOfUnits=maxNumOfUn;
	unsigned int maxUnitsDistance=maxUnDist;
	unsigned int step=st;

	size_t missSize = _missingElements.size();

	QualityContainerBase QualityContainerBaseObjOutputMiss;
	QualityContainerBaseObjOutputMiss.setQualityMissingElements(_missingElements);

	QualityContainerBase QualityContainerBaseObjTmp;

	while(missSize>maxNumberOfUnits)
	{
        QualityContainerBaseObjTmp = QualityContainerBaseObjOutputMiss;
        QualityContainerBaseObjOutputMiss.resetQualityMissingElements();

        unsigned int i=0 ;
        unsigned int sum=0;
        int beforeElem=0 ;
        int afterElem=0 ;
        int precElem=0 ;
        int second=0;
        // overTotal=0;
        startTimeTotalStr="";
        stopTimeTotalStr="";
        // startTimeTotal=0;
        // stopTimeTotal=0;
        bool isFirstElementFollowing=true ;
        bool isSecondElementFollowing=true ;
        bool isLastElementGrouped=false;

		while((i<missSize) && !isLastElementGrouped)
		{
			bool isGrouped=false;

			// 31/05/2006 location of corr and miss elements is now expressed through
            // attribute following instead of preceding;
			int first = (QualityContainerBaseObjTmp.qualityMissingElements()[i].qualityLocation().following());
			if(first==-1)
			{
                QualityMissingElements 	qualityMissingElement = QualityContainerBaseObjTmp.qualityMissingElements()[i];
                QualityMissingElements 	qualityMissingElementNext;
                // qualityMissingElementNext = ((QualityContainerBaseObjTmp.qualityMissingElements())[i+1]);
                // A.B. - June 2011 - added a check to verify if the current missing element is the last in the product.
                // If the current missing element is the last in the product, qualityMissingElementNext is the
                // current missing element, else qualityMissingElementNext is the next missing element
                if (i==missSize-1) { // PRQA S 3084 4
                    qualityMissingElementNext = QualityContainerBaseObjTmp.qualityMissingElements()[i];
                } else {
                    qualityMissingElementNext = QualityContainerBaseObjTmp.qualityMissingElements()[i+1];
                }

				//first = ((((QualityContainerBaseObjTmp.qualityMissingElements())[i]).qualityLocation()).after());
				//beforeElem = ((((QualityContainerBaseObjTmp.qualityMissingElements())[i]).qualityLocation()).before());
                first = (qualityMissingElement.qualityLocation()).after();
                beforeElem = (qualityMissingElement.qualityLocation()).before();

                // ALEBER _ EFC-17 - OVER
                // overTotal = ((QualityContainerBaseObjTmp.qualityMissingElements())[i+1]).over();
                // stopTimeTotalStr = ((QualityContainerBaseObjTmp.qualityMissingElements())[i+1]).stopTime();
                //overTotal = qualityMissingElementNext.over();
                stopTimeTotalStr = (qualityMissingElementNext.qualityLocation()).stop();


				// 31/05/2006 all missing elements with attribute preceding are intercepted here and not grouped (taken as is)
				if ( (first==-1) && (beforeElem==-1) && ( qualityMissingElement.qualityLocation().preceding() != -1 ) )
				{
                    // ALEBER _ EFC-17
                    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
	                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "EFC-17 - ADD MISSING: ") ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t count             = " << qualityMissingElement.count()) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t preceding         = " << (qualityMissingElement.qualityLocation()).preceding()) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t startTimeTotalStr = " << (qualityMissingElement.qualityLocation()).start()) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t stopTimeTotalStr  = " << (qualityMissingElement.qualityLocation()).stop()) ;
  			        ACS_CLASS_END_DEBUG
					QualityContainerBaseObjOutputMiss.addQualityMissingElements( qualityMissingElement.count(),
//                                                                             "line",
                                                                             dataUnitXpath,
                                                                             "UNKNOWN",
                                                                             -1,
                                                                             -1,
                                                                             -1,
                                                                             (qualityMissingElement.qualityLocation()).preceding(),
                                                                             qualityMissingElement.over(),
                                                                             (qualityMissingElement.qualityLocation()).start(),
                                                                             (qualityMissingElement.qualityLocation()).stop()
                                                                           );
					i++;
					maxNumberOfUnits--;
					continue;
				}

				isFirstElementFollowing = false;

			} // end if(first==-1)
			else
			{
				precElem = first;
				isFirstElementFollowing = true;
			}

			afterElem = first;

            // ALEBER _ EFC-17 OVER
            // startTimeTotal = ((QualityContainerBaseObjTmp.qualityMissingElements())[i]).startTime();
            QualityMissingElements 	qualityMissingElement = (QualityContainerBaseObjTmp.qualityMissingElements())[i];
            startTimeTotalStr = (qualityMissingElement.qualityLocation()).start();
            stopTimeTotalStr = (qualityMissingElement.qualityLocation()).stop();

			sum = qualityMissingElement.count(); // PRQA S 3000

			if(i<missSize-1) // PRQA S 3084
			{
				do
				{
                    QualityMissingElements 	qualityMissingElement1 = QualityContainerBaseObjTmp.qualityMissingElements()[i];
                    QualityMissingElements 	qualityMissingElement1Next = QualityContainerBaseObjTmp.qualityMissingElements()[i+1]; // PRQA S 3084

					// 31/05/2006 location of corr and miss elements is now expressed trough attribute following instead of preceding;
					second = (qualityMissingElement1Next.qualityLocation()).following();
					if(second==-1)
					{
						second = qualityMissingElement1Next.qualityLocation().after();
						isSecondElementFollowing = false;
					}
					else {
						isSecondElementFollowing = true;
                    }

					if(static_cast<unsigned int>(second-first)<=maxUnitsDistance) // PRQA S 3081
					{

						sum += qualityMissingElement1Next.count(); // PRQA S 3000, 3084

						// 31/05/2006 location of corr and miss elements is now expressed trough attribute following instead of preceding;
						if(isSecondElementFollowing) {
							beforeElem = second+1;
                        }
						else {
							beforeElem = qualityMissingElement1Next.qualityLocation().before();
                        }

                        // ALEBER _ EFC-17 OVER
                        // overTotal = ((QualityContainerBaseObjTmp.qualityMissingElement1s())[i+1]).over();
                        // stopTimeTotalStr = ((QualityContainerBaseObjTmp.qualityMissingElement1s())[i+1]).stopTime();
                        //overTotal = qualityMissingElement1Next.over();
                        stopTimeTotalStr = (qualityMissingElement1Next.qualityLocation()).stop();

						isGrouped=true;
						if(i==missSize-2) { // PRQA S 3084, 4400
							isLastElementGrouped=true;
                        }
					} else {
                        stopTimeTotalStr = (qualityMissingElement1.qualityLocation()).stop();
                    }

					i++;
				}
				while((static_cast<unsigned int>(second-first)<=maxUnitsDistance) && (i<missSize-1));	// PRQA S 3081, 3084
			}
			else {
				i++;
            }

			if(isGrouped) {
                // ALEBER _ EFC-17
                ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "EFC-17 - ADD MISSING: ") ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t count             = " << sum) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t afterElem         = " << afterElem) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t beforeElem        = " << beforeElem) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t startTimeTotalStr = " << startTimeTotalStr) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t stopTimeTotalStr  = " << stopTimeTotalStr) ;
  			    ACS_CLASS_END_DEBUG
		        QualityContainerBaseObjOutputMiss.addQualityMissingElements(sum, // PRQA S 3000
//                                                                        "line",
                                                                        dataUnitXpath,
                                                                        "UNKNOWN",
                                                                        afterElem,
                                                                        beforeElem,
                                                                        -1,
                                                                        -1,
//                                                                        overTotal,
                                                                         -1,
                                                                        startTimeTotalStr,
                                                                        stopTimeTotalStr
                                                                        );
			} else if(isFirstElementFollowing) {
					// 31/05/2006 location of corr and miss elements is now expressed trough attribute following instead of preceding;
                    // ALEBER _ EFC-17
                    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "EFC-17 - ADD MISSING: ") ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t count             = " << sum) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t precElem          = " << precElem) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t startTimeTotalStr = " << startTimeTotalStr) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t stopTimeTotalStr  = " << stopTimeTotalStr) ;
  			        ACS_CLASS_END_DEBUG
					QualityContainerBaseObjOutputMiss.addQualityMissingElements(sum, // PRQA S 3000
                                                                            dataUnitXpath,
//                                                                            "line",
                                                                            "UNKNOWN",
                                                                            -1,
                                                                            -1,
                                                                            precElem,
                                                                            -1,
//                                                                            overTotal,
                                                                            -1,
                                                                            startTimeTotalStr,
                                                                            stopTimeTotalStr
                                                                           );
			} else {
                    // ALEBER _ EFC-17
                    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "EFC-17 - ADD MISSING: " ) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t count             = " << sum) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t afterElem         = " << afterElem) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t beforeElem        = " << beforeElem) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t startTimeTotalStr = " << startTimeTotalStr) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t stopTimeTotalStr  = " << stopTimeTotalStr) ;
  			        ACS_CLASS_END_DEBUG
					QualityContainerBaseObjOutputMiss.addQualityMissingElements(sum, // PRQA S 3000
//                                                                            "line",
                                                                            dataUnitXpath,
                                                                            "UNKNOWN",
                                                                            afterElem,
                                                                            beforeElem,
                                                                            -1,
                                                                            -1,
//                                                                            overTotal,
                                                                            -1,
                                                                            startTimeTotalStr,
                                                                            stopTimeTotalStr
                                                                           );
			}
		} // end while(i<missSize && !isLastElementGrouped)

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QualityContainerBaseObjOutputMiss has " << QualityContainerBaseObjOutputMiss.qualityMissingElements().size() << " missing elements") ;

		missSize = (QualityContainerBaseObjOutputMiss.qualityMissingElements()).size();
		maxUnitsDistance += step; // PRQA S 3084

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "missSize is " << missSize << " and maxUnitsDistance is "<< maxUnitsDistance) ;

	} // end while(missSize>maxNumberOfUnits)

	_missingElements = QualityContainerBaseObjOutputMiss.qualityMissingElements();

	// NOW COMPUTATE THE CORRUPTED ELEMENTS GROUPING

	// reset the thresholds to the initial value also for Corrupted Units
	maxNumberOfUnits=maxNumOfUn;
	maxUnitsDistance=maxUnDist;
	step=st;

	size_t corrSize = _corruptedElements.size();

	QualityContainerBase QualityContainerBaseObjOutputCorr;
	QualityContainerBaseObjOutputCorr.setQualityCorruptedElements(_corruptedElements);
	QualityContainerBaseObjTmp.resetQualityCorruptedElements();

	while(corrSize>maxNumberOfUnits)
	{
		QualityContainerBaseObjTmp = QualityContainerBaseObjOutputCorr;
		QualityContainerBaseObjOutputCorr.resetQualityCorruptedElements();

		unsigned int i=0 ;
        unsigned int sum=0;
		int beforeElem=0 ;
        int afterElem=0;
        int precElem=0 ;
        int second=0;
		//overTotal=0;
        // startTimeTotal=0;
        // stopTimeTotal=0;
        startTimeTotalStr="";
        stopTimeTotalStr="";

		bool isFirstElementFollowing=true ;
        bool isSecondElementFollowing=true ;
        bool isLastElementGrouped=false;
		while((i<corrSize) && !isLastElementGrouped)
		{
			bool isGrouped=false;

			// 31/05/2006 location of corr and miss elements is now expressed trough attribute following instead of preceding;
			int first = QualityContainerBaseObjTmp.qualityCorruptedElements()[i].qualityLocation().following();
			if(first==-1)
			{
                QualityCorruptedElements 	qualityCorruptedElement = (QualityContainerBaseObjTmp.qualityCorruptedElements())[i];
                QualityCorruptedElements 	qualityCorruptedElementNext;
                if (i == corrSize - 1  ) { // PRQA S 3084
                    qualityCorruptedElementNext = (QualityContainerBaseObjTmp.qualityCorruptedElements())[i];
                } else {
                    qualityCorruptedElementNext = (QualityContainerBaseObjTmp.qualityCorruptedElements())[i+1]; // PRQA S 3084
                }

                first = (qualityCorruptedElement.qualityLocation()).after();
                beforeElem = (qualityCorruptedElement.qualityLocation()).before();

                // ALEBER _ EFC-17 - OVER
                // overTotal = ((QualityContainerBaseObjTmp.qualityCorruptedElements())[i+1]).over();
                // stopTimeTotalStr = ((QualityContainerBaseObjTmp.qualityCorruptedElements())[i+1]).stopTime();
                // overTotal = qualityCorruptedElementNext.over(); // PRQA S 3000
                stopTimeTotalStr = (qualityCorruptedElementNext.qualityLocation()).stop();

				// 31/05/2006 all corr elements with attribute preceding are intercepted here and not grouped (taken as is)
				if ( (first==-1) && (beforeElem==-1) && ( qualityCorruptedElement.qualityLocation().preceding() != -1 ) )
				{

                     ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "EFC-17 - ADD CORRUPTED: " ) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t count             = " << qualityCorruptedElement.count()) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t preceding         = " << (qualityCorruptedElement.qualityLocation()).preceding()) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t overTotal         = " << qualityCorruptedElement.over()) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t startTimeTotalStr = " << (qualityCorruptedElement.qualityLocation()).start()) ;
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t stopTimeTotalStr  = " << (qualityCorruptedElement.qualityLocation()).stop()) ;
 			        ACS_CLASS_END_DEBUG

                    QualityContainerBaseObjOutputCorr.addQualityCorruptedElements(qualityCorruptedElement.count(),
                                                                              dataUnitXpath,
                                                                              -1,
                                                                              -1,
                                                                              -1,
                                                                              (qualityCorruptedElement.qualityLocation()).preceding(),
                                                                              (qualityCorruptedElement.cause()).type(),
                                                                              "",
                                                                              0,
                                                                              qualityCorruptedElement.over(),
                                                                              (qualityCorruptedElement.qualityLocation()).start(),
                                                                              (qualityCorruptedElement.qualityLocation()).stop());
					maxNumberOfUnits--;
					i++;
					continue;
				}

				isFirstElementFollowing = false;
			}
			else
			{
				precElem = first;
				isFirstElementFollowing = true;
			}
			afterElem = first;

            // ALEBER _ EFC-17 OVER
            // startTimeTotal = ((QualityContainerBaseObjTmp.qualityCorruptedElements())[i]).startTime();
            QualityCorruptedElements 	qualityCorruptedElement = (QualityContainerBaseObjTmp.qualityCorruptedElements())[i];
            startTimeTotalStr = (qualityCorruptedElement.qualityLocation()).start();
            stopTimeTotalStr = (qualityCorruptedElement.qualityLocation()).stop();
            corruptedType = (qualityCorruptedElement.cause()).type();

			sum = qualityCorruptedElement.count(); // PRQA S 3000

			if(i<corrSize-1) // PRQA S 3084
			{
				do
				{
                    QualityCorruptedElements 	qualityCorruptedElement1 = (QualityContainerBaseObjTmp.qualityCorruptedElements())[i];
                    QualityCorruptedElements 	qualityCorruptedElement1Next = (QualityContainerBaseObjTmp.qualityCorruptedElements())[i+1]; // PRQA S 3084

					// 31/05/2006 location of corr and miss elements is now expressed trough attribute following instead of preceding;
					second = (qualityCorruptedElement1Next.qualityLocation()).following();
					if(second==-1)
					{
						second = (qualityCorruptedElement1Next.qualityLocation()).after();
						isSecondElementFollowing = false;
					}
					else {
						isSecondElementFollowing = true;
                    }

					if(static_cast<unsigned int>(second-first)<=maxUnitsDistance) // PRQA S 3081
					{

						sum += qualityCorruptedElement1Next.count(); // PRQA S 3000, 3084

						// 31/05/2006 location of corr and miss elements is now expressed trough attribute following instead of preceding;
						if(isSecondElementFollowing) {
							beforeElem = second + qualityCorruptedElement1Next.count() + 1;
                        }
						else {
                            beforeElem = qualityCorruptedElement1Next.qualityLocation().before();
                        }

                        // ALEBER _ EFC-17 OVER
                        // overTotal = ((QualityContainerBaseObjTmp.qualityCorruptedElement1s())[i+1]).over();
                        // stopTimeTotalStr = ((QualityContainerBaseObjTmp.qualityCorruptedElement1s())[i+1]).stopTime();
                        // overTotal = qualityCorruptedElement1Next.over();
                        stopTimeTotalStr = (qualityCorruptedElement1Next.qualityLocation()).stop();

						isGrouped=true;
						if(i==corrSize-2) { // PRQA S 3084, 4400
							isLastElementGrouped=true;
                        }
					} else {
                        stopTimeTotalStr = (qualityCorruptedElement1.qualityLocation()).stop();
                    }

					i++;
				}
				while((static_cast<unsigned int>(second-first)<=maxUnitsDistance) && (i<corrSize-1)); // PRQA S 3081, 3084
			}
			else {
				i++;
            }

			if(isGrouped) {
                // ALEBER _ EFC-17
                ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "EFC-17 - ADD CORRUPTED: ") ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t count             = " << sum) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t afterElem         = " << afterElem) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t beforeElem        = " << beforeElem) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t startTimeTotalStr = " << startTimeTotalStr) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t stopTimeTotalStr  = " << stopTimeTotalStr) ;
  			    ACS_CLASS_END_DEBUG
				QualityContainerBaseObjOutputCorr.addQualityCorruptedElements(sum, // PRQA S 3000
                                                                          dataUnitXpath,
                                                                          afterElem,
                                                                          beforeElem,
                                                                          -1,
                                                                          -1,
                                                                          corruptedType,
                                                                          "",
                                                                          0,
                                                                          -1,
                                                                          startTimeTotalStr,
                                                                          stopTimeTotalStr);
			} else if(isFirstElementFollowing) {
                ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "EFC-17 - ADD CORRUPTED: ") ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t count             = " << sum) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t precElem          = " << precElem) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t startTimeTotalStr = " << startTimeTotalStr) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t stopTimeTotalStr  = " << stopTimeTotalStr) ;
  			    ACS_CLASS_END_DEBUG
				// 31/05/2006 location of corr and miss elements is now expressed trough attribute following instead of preceding;
				QualityContainerBaseObjOutputCorr.addQualityCorruptedElements(sum, // PRQA S 3000
                                                                          dataUnitXpath,
                                                                           -1,
                                                                          -1,
                                                                          precElem,
                                                                          -1,
                                                                          corruptedType,
                                                                          "",
                                                                          0,
                                                                          -1,
                                                                          startTimeTotalStr,
                                                                          stopTimeTotalStr);
			} else {
                 ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "EFC-17 - ADD CORRUPTED: ") ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t count             = " << sum) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t afterElem         = " << afterElem) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t beforeElem        = " << beforeElem) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t startTimeTotalStr = " << startTimeTotalStr) ;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t stopTimeTotalStr  = " << stopTimeTotalStr) ;
  			    ACS_CLASS_END_DEBUG
				QualityContainerBaseObjOutputCorr.addQualityCorruptedElements(sum, // PRQA S 3000
                                                                          dataUnitXpath,
                                                                          afterElem,
                                                                          beforeElem,
                                                                          -1,
                                                                          -1,
                                                                          corruptedType,
                                                                          "",
                                                                          0,
                                                                          -1,
                                                                          startTimeTotalStr,
                                                                          stopTimeTotalStr);
			}
		}

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QualityContainerBaseObjOutputCorr has " << QualityContainerBaseObjOutputCorr.qualityCorruptedElements().size() << " corrupted elements") ;

		corrSize = QualityContainerBaseObjOutputCorr._corruptedElements.size();
		maxUnitsDistance += step; // PRQA S 3084

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "corrSize is " << corrSize << " and maxUnitsDistance is "<< maxUnitsDistance) ;

	} // end while(corrSize>maxNumberOfUnits)

	_corruptedElements = QualityContainerBaseObjOutputCorr._corruptedElements;
}


string QualityContainerBase::toStr() const
{
 	ostringstream msg;
	msg << "\tQualityContainerBase content: " << '\n'
            << "\tPRIVATE DATA MEMBERS:" <<'\n'
            << "\t\t_dataObjectId\t= " << _dataObjectId << '\n'
            << "\t\t_numOfPackets\t= " << _numOfPackets << '\n'
            << "\t\t_numOfRSIncorrigibleElements\t\t\t= " << _numOfRSIncorrigibleElements << '\n'
            << "\t\t_numOfRSCorrectedElements\t\t= " <<  _numOfRSCorrectedElements << '\n'
            << "\t\t_numOfRSCorrectedSymbols\t\t= " << _numOfRSCorrectedSymbols ;
 	return msg.str() ;

}


std::ostream &operator << (std::ostream &os, const QualityContainerBase &v)
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const QualityContainerBase &v) // PRQA S 4222
{
    return os << v.toStr() ;
}
_ACS_END_NAMESPACE
