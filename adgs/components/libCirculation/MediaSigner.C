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
 *  $Prod: A.C.S. libCirculation Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.3  2013/07/18 12:29:51  marpas
 *  coding best practices application in progress
 *  type mismatch fix in progress
 *
 *  Revision 5.2  2013/07/16 14:52:16  marpas
 *  coding best practices applied,
 *  performances and robustness improved
 *
 *  Revision 5.1  2013/07/15 17:21:12  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:49:01  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.4  2012/04/03 17:45:48  micmaz
 *  work in progress
 *
 *  Revision 1.3  2012/03/27 10:54:16  micmaz
 *  work in progress
 *
 *  Revision 1.2  2012/03/09 17:43:34  micmaz
 *  added media functionality
 *
 *  Revision 1.1  2012/03/08 13:20:45  micmaz
 *  added media functionality
 *
 *
 */

#include <MediaSigner.h>
#include <XMLIstringstream.h>
#include <XMLOstream.h>
#include <rsPushNSpace.h>

#include <boost/smart_ptr.hpp> // PRQA S 1013

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation)
using namespace std;
//
//SignedFile section
//
string const SignedFile::nameKey = "Name";
string const SignedFile::digestKey = "Digest";
string const SignedFile::sourceHostIdKey = "SourceHostId";
string const SignedFile::destinationHostIdKey = "DestinationHostId";
string const SignedFile::circulationHostKey = "HostId";
string const SignedFile::circulationIdKey = "CirculationId";
string const SignedFile::creationDateKey = "CreationDateTime";
string const SignedFile::lastModificationDateKey = "TranferDateTime";
string const SignedFile::mediaTag = "Media";
string const SignedFile::reportTag = "Report";
string const SignedFile::listOfFilesKey="List_of_Files";
string const SignedFile::verificationKey="Verification";
string const SignedFile::verificationDateKey="DateTime";
string const SignedFile::verificationDigestKey="Digest";
string const SignedFile::verificationStatusKey="Status";
string const SignedFile::verificationOk="Ok";
string const SignedFile::verificationNok="The digital signature is not correct";

SignedFile::SignedFile() :
        _sourceHostId(),
        _destinationHostId(),
        _circulationHost(),
        _circulationId(),
        _creationDate(),
        _lastModificationDate(),
        _name(),
        _digest(),
        _digestType(),
        _verificationDigest(),
        _verificationStatus(),
        _verificationDate()
{
    //empty
}
SignedFile::SignedFile(rsResourceSet& resultSet, string const& tagName) :
        _sourceHostId(),
        _destinationHostId(),
        _circulationHost(),
        _circulationId(),
        _creationDate(),
        _lastModificationDate(),
        _name(),
        _digest(),
        _digestType(),
        _verificationDigest(),
        _verificationStatus(),
        _verificationDate()
{
    rsPushNSpace element(resultSet, tagName);
    resultSet.getValue(SignedFile::nameKey, _name);
    resultSet.getValue(SignedFile::digestKey, _digest);
    resultSet.getValue(SignedFile::sourceHostIdKey, _sourceHostId);
    resultSet.getValue(SignedFile::destinationHostIdKey, _destinationHostId);
    resultSet.getValue(SignedFile::circulationHostKey, _circulationHost);
    resultSet.getValue(SignedFile::circulationIdKey, _circulationId);
    resultSet.getValue(SignedFile::creationDateKey, _creationDate);
    resultSet.getValue(SignedFile::lastModificationDateKey, _lastModificationDate);
    {
        rsPushNSpace digest(resultSet, SignedFile::digestKey);
        if (resultSet.hasKey("type.value")) {
            resultSet.getValue("type.value", _digestType);
        }
    }
    if (resultSet.hasKey(SignedFile::verificationKey)) {
        rsPushNSpace verificationElement(resultSet, SignedFile::verificationKey);
        resultSet.getValue(SignedFile::verificationDateKey, _verificationDate);
        if (resultSet.hasKey(SignedFile::verificationDigestKey)) {
            resultSet.getValue(SignedFile::verificationDigestKey, _verificationDigest);
        }
        resultSet.getValue(SignedFile::verificationStatusKey, _verificationStatus);
    }
}
SignedFile::SignedFile(SignedFile const& other) :
        _sourceHostId(other._sourceHostId),
        _destinationHostId(other._destinationHostId),
        _circulationHost(other._circulationHost),
        _circulationId(other._circulationId),
        _creationDate(other._creationDate),
        _lastModificationDate(other._lastModificationDate),
        _name(other._name),
        _digest(other._digest),
        _digestType(other._digestType),
        _verificationDigest(other._verificationDigest),
        _verificationStatus(other._verificationStatus),
        _verificationDate(other._verificationDate)
{
    //empty
}
SignedFile& SignedFile::operator=(SignedFile const& other) {
    if (this != &other) {
        _sourceHostId = other._sourceHostId;
        _destinationHostId = other._destinationHostId;
        _circulationHost = other._circulationHost;
        _circulationId = other._circulationId;
        _creationDate = other._creationDate;
        _lastModificationDate = other._lastModificationDate;
        _name = other._name;
        _digest = other._digest;
        _digestType = other._digestType;
        _verificationDigest=other._verificationDigest;
        _verificationStatus=other._verificationStatus;
        _verificationDate=other._verificationDate;
    }
    return *this;
}
bool SignedFile::operator==(SignedFile const& other) { // PRQA S 4020, 4211
    if (this != &other) {
        return (StringUtils::equals(_circulationHost, other._circulationHost)
                && StringUtils::equals(_sourceHostId, other._sourceHostId)
                && StringUtils::equals(_destinationHostId, other._destinationHostId)
                && StringUtils::equals(_circulationId, other._circulationId)
                && StringUtils::equalsNoCase(_creationDate, other._creationDate)
                && StringUtils::equalsNoCase(_lastModificationDate, other._lastModificationDate)
                && StringUtils::equals(_name, other._name)
                && StringUtils::equals(_digest, other._digest)
                && StringUtils::equalsNoCase(_digestType, other._digestType));
    } else {
        return true;
    }
}
string SignedFile::str(bool full) const {
    stringstream stream;
    XMLOstream* ostream = 0;
    if (full) {
        ostream = new XMLOstream(stream, reportTag);
    } else {
        ostream = new XMLOstream(stream);
    }
    boost::scoped_ptr < XMLOstream > handle(ostream);
    {
        XMLOstream::Tag media(*ostream, mediaTag, "");
        {
            XMLOstream::Tag tag(*ostream, destinationHostIdKey, _destinationHostId);
        }
        {
            XMLOstream::Tag tag(*ostream, sourceHostIdKey, _sourceHostId);
        }
        {
            XMLOstream::Tag tag(*ostream, circulationHostKey, _circulationHost);
        }
        {
            XMLOstream::Tag tag(*ostream, circulationIdKey, _circulationId);
        }
        {
            XMLOstream::Tag tag(*ostream, creationDateKey, _creationDate);
        }
        {
            XMLOstream::Tag tag(*ostream, lastModificationDateKey, _lastModificationDate);
        }
        {
            XMLOstream::Tag tag(*ostream, nameKey, _name);
        }
        {
            ostringstream attr;
            if (_digestType.size() > 0) {
                attr << "type=\"" << _digestType << "\"";
            }
            XMLOstream::Tag tag(*ostream, digestKey, _digest, attr.str());
        }
        if (!StringUtils::isBlank(_verificationDate)){
            XMLOstream::Tag tag(*ostream, verificationKey,"");
            {
                XMLOstream::Tag tag1(*ostream, verificationDateKey, _verificationDate);
            }
            if (!StringUtils::equals(_verificationStatus,SignedFile::verificationOk)){
                XMLOstream::Tag tag1(*ostream, verificationDigestKey, _verificationDigest);
            }
            {
                XMLOstream::Tag tag1(*ostream, verificationStatusKey, _verificationStatus);
            }
        }
    }
    ostream->close();
    return stream.str();
}

// PRQA S 4120 L1
std::string SignedFile::getCirculationHost() const
{
    return _circulationHost;
}

std::string SignedFile::getCirculationId() const
{
    return _circulationId;
}

std::string SignedFile::getCreationDate() const
{
    return _creationDate;
}

std::string SignedFile::getDestinationHostId() const
{
    return _destinationHostId;
}

std::string SignedFile::getDigest() const
{
    return _digest;
}

std::string SignedFile::getDigestType() const
{
    return _digestType;
}

std::string SignedFile::getLastModificationDate() const
{
    return _lastModificationDate;
}

std::string SignedFile::getName() const
{
    return _name;
}

std::string SignedFile::getSourceHostId() const
{
    return _sourceHostId;
}

std::string SignedFile::getVerificationDate() const
{
    return _verificationDate;
}

std::string SignedFile::getVerificationDigest() const
{
    return _verificationDigest;
}

std::string SignedFile::getVerificationStatus() const
{
    return _verificationStatus;
}
// PRQA L:L1

// PRQA S 4121 L2
void SignedFile::setCirculationHost(std::string const& circulationHost)
{
    _circulationHost = circulationHost;
}

void SignedFile::setCirculationId(std::string const& circulationId)
{
    _circulationId = circulationId;
}

void SignedFile::setCreationDate(std::string const& creationDate)
{
    _creationDate = creationDate;
}

void SignedFile::setDestinationHostId(std::string const& destinationHostId)
{
    _destinationHostId = destinationHostId;
}

void SignedFile::setDigest(std::string const& digest)
{
    _digest = digest;
}

void SignedFile::setDigestType(std::string const& digestType)
{
    _digestType = digestType;
}

void SignedFile::setLastModificationDate(std::string const& lastModificationDate)
{
    _lastModificationDate = lastModificationDate;
}

void SignedFile::setName(std::string const& name)
{
    _name = name;
}

void SignedFile::setSourceHostId(std::string const& sourceHostId)
{
    _sourceHostId = sourceHostId;
}

void SignedFile::setVerificationDate(std::string const& verificationDate)
{
    _verificationDate = verificationDate;
}

void SignedFile::setVerificationDigest(std::string const& verificationDigest)
{
    _verificationDigest = verificationDigest;
}

void SignedFile::setVerificationStatus(std::string const& verificationStatus)
{
    _verificationStatus = verificationStatus;
}

// PRQA L:L2

//
//MediaSerializer section
//
ostream& MediaSerializer::toStream(ostream& stream, vector<SignedFile> const& obj) {
    XMLOstream ostream(stream, SignedFile::reportTag);
    {

        ostringstream attr;
        attr << "count=\"" << obj.size() << "\"";
        XMLOstream::Tag listOfFiles(ostream, SignedFile::listOfFilesKey, "", attr.str());
        for (size_t counter = 0; counter < obj.size(); ++counter) {
            stream << obj[counter];
        }
    }
    ostream.close();
    return stream; // PRQA S 4028
}

void MediaSerializer::readStream(istream& stream, ostringstream* fileContent) {
    string line;
    while (!stream.eof()) {
        getline(stream, line);
        *fileContent << line;
    }
}

istream & MediaSerializer::fromStream(istream& stream, vector<SignedFile>* files) {
    ostringstream fileContent;
    readStream(stream, &fileContent);
    try {
        rsResourceSet resultSet;
        XMLIstringstream xis(fileContent.str());
        xis >> resultSet;
        rsPushNSpace listOfFilesTag(resultSet, SignedFile::listOfFilesKey);
        size_t arraySize = resultSet.getArraySize(SignedFile::mediaTag);
        if (0 == arraySize) {
            SignedFile file(resultSet);
            files->push_back(file);
        } else {
            for (size_t counter = 0; counter < arraySize; ++counter) {
                ostringstream tagName;
                tagName << SignedFile::mediaTag << "[" << counter << "]";
                SignedFile file(resultSet, tagName.str());
                files->push_back(file);
            }
        }
    } catch (std::exception const& ex) {
        ACS_LOG_NOTIFY_EX(ex);
        throw;
    }
    return stream; // PRQA S 4028
}
ostream& operator<<(ostream& stream, SignedFile const& obj) {
    stream << obj.str();
    return stream;// PRQA S 4028
}

exostream& operator<<(exostream& stream, SignedFile const& obj) { // PRQA S 4222
    stream << obj.str();
    return stream; // PRQA S 4028
}


_ACS_END_NESTED_NAMESPACE

