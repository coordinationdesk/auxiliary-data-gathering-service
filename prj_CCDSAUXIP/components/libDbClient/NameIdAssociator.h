/*

  Copyright 1995-2022, Exprivia SpA - DFDA-AS
  Via Della Bufalotta, 378 - 00139 Roma - Italy
  http://www.exprivia.com

  All Rights Reserved.

  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
  the contents of this file may not be disclosed to third parties, copied or
  duplicated in any form, in whole or in part, without the prior written
  permission of Exprivia SpA

  $Prod: A.C.S. DataBase Client Library $

*/

#ifndef _NameIdAssociator_H_
#define _NameIdAssociator_H_

#include <acs_c++config.hpp>
#include <ResourceSpaceFactory.h>
#include <dbQuery.h>
#include <dbStat.h>
#include <dbSet.h>
#include <dbConnectionPool.h>
#include <SQLString.h>
#include <File.h>
#include <StringUtils.h>
#include <Filterables.h>
#include <RegExp.h>
#include <Statistics.h>
#include <fstream>
#include <string>
#include <map>
#include <functional>
#include <array>

#include <Singleton.hpp>
#include <ThreadSafe.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * class NameIdAssociatorConfiguration
 *
 * provides a singleton implementation to allow global NameIdAssociator configurations
 *
 * list of attributes:
 * - enableDiskMap: bool enables or disables disk persistency of map files
 **/

using pattern::Singleton;

class NameIdAssociatorConfiguration : public Singleton<NameIdAssociatorConfiguration>
{
protected:
  NameIdAssociatorConfiguration() = default;

public:
  void setEnableDiskMaps(bool enable);
  bool getEnableDiskMaps() const;
  ~NameIdAssociatorConfiguration() override = default;

private:
  Mutex _mutex{};
  bool _enableDiskMaps = true;

  // friend declaration needed to use the singleton pattern
  friend NameIdAssociatorConfiguration* Singleton<NameIdAssociatorConfiguration>::instance();
};

//!
//!
//! class NameIdAssociator
//!
//! This is the base class that manage the Name<->Id association.
//! It provides the basic mechanism to load a specified table and is designed
//! to be implemented as a singleton pattern.
//! The column referred as Id is usually the primary key of the table, or at least it has the uniqueness property guaranteed
//! That column (Id) will be stored into the template T type.
//! The column referred as Name will be stored into a string.
//! The class is meant to be used to convert an Id to a Name and viceversa.
//!
//! The class saves is content in a file in order to load again it in case the DB should not respond. This behaviour guarantee that
//! applications would not fail when converting satellite ids to satellite names providing that in the past the DB was reachable and the file was saved.
//! Different DB could have different tables, so files are maintained separated according the dbkey used to address the DB (dialect+schema)
template<class T>
class NameIdAssociator
{
public:
  static constexpr long double RELOAD_TIMEOUT = 1.0 / 86400.0;  // 1 sec

  NameIdAssociator() = delete;
  NameIdAssociator(const NameIdAssociator&) = delete;
  NameIdAssociator& operator=(const NameIdAssociator&) = delete;
  NameIdAssociator(const std::string& tbl, const std::string& pk, const std::string& col, const std::string& dbkey,
                   const bool percentEnc = false) :
    _dbKey(dbkey),
    _tableName(tbl),
    _pkey(pk),
    _col(col),
    _oby(_pkey),
    _percentEnc(percentEnc)
  {
    statN(_dbKey)[_tableName][_pkey]["created #"] += 1;
    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "_dbKey: \"" << _dbKey << "\""
                                                        << " _tableName: " << _tableName
                                                        << " _pkey: " << _pkey
                                                        << " _col: " << _col);
  }

  virtual ~NameIdAssociator() noexcept = default;

  //! returns the id associated with the given name using idExact
  //!
  //! an exception of type acs::exIllegalValueException is thrown if name is not found
  //! \param name : the name whose id shall be found and returned
  //! \param caseInsensitive : if true the case will not be used to find the name
  //!
  const T& id(const std::string& name, bool caseInsensitive = true) const
  {
    return idExact(name, caseInsensitive);
  }

  //! returns the id associated with the given name using the StringUtils::sanitizeDbName method applied to both name
  //! and name entries.
  //!
  //! an exception of type acs::exIllegalValueException is thrown if name is not found
  //! \param name : the name whose id shall be found and returned
  //! \param caseInsensitive : if true the case will not be used to find the name
  //!
  const T& idSanitized(const std::string& name, bool caseInsensitive = true) const
  {
    return idInternal(name, true, caseInsensitive);
  }
  //! returns the id associated with the given name
  //! an exception of type exIllegalValueException is thrown if name is not found
  //! \param name : the name whose id shall be found and returned - not sanitized \see NameIdAssociator::id
  //! \param caseInsensitive : if true the case will not be used to find the name
  const T& idExact(const std::string& name, bool caseInsensitive = true) const
  {
    return idInternal(name, false, caseInsensitive);
  }

  //! returns the name associated with the given id
  //! an exception of type acs::exBoundsException is thrown if id is not found
  const std::string& name(const T& id) const
  {
    auto it = _values.find(id);
    // S3MARINE-656: If the requested name cannot be found, try reloading the map. Maybe there has been a hot change in
    //               the DB?
    if (it == _values.end() && DateTime() > _nextLoadAfter)
    {
      // S3MARINE-656: In order to keep the constness of this method, is needed to cast away the constness in the load
      //               call. This is not good, but it is the only solution to keep the method signature unchanged and
      //               avoid errors on other components. Using mutable for variables that are changed by the load method
      //               would allow them to be changed in other constant functions as well, which would be even worse.
      const_cast<NameIdAssociator<T>*>(this)->load();
      it = _values.find(id);
    }

    if (it == _values.end())
    {
      statN(_dbKey)[_tableName][_pkey]["name #"]["error"] += 1;
      ACS_THROW(exBoundsException() << " ID: " << id << " not found on table: " << _tableName);
    }
    return it->second;
  }

  //! \brief numbers of row loaded
  unsigned int size() const noexcept { return _values.size(); }

  //! \brief the table name
  const std::string& tableName() const noexcept { return _tableName; }

  //! \brief the column used as Id
  const std::string& pkeyName() const noexcept { return _pkey; }

  //! \brief the column used as Name
  const std::string& colName() const noexcept { return _col; }

  //! returns the name in the i-th record
  //! \param idx : the index - acs::exBoundsException will be thrown in case is out of bound
  const std::string& operator[](unsigned int idx) const
  {
    ACS_COND_THROW(idx >= size(), exBoundsException() << "Cannot address element # " << idx << " in a collection of "
                                                      << size() << " elements (table:  " << _tableName << ")");

    auto i = _values.begin();
    std::advance(i, idx);
    return i->second;
  }

  //! returns the id of the i-th record
  //! \param idx : the index - acs::exBoundsException will be thrown in case is out of bound
  const T& idByPos(unsigned int idx) const
  {
    ACS_COND_THROW(idx >= size(), exBoundsException() << "Cannot address element # " << idx << " in a collection of "
                                                      << size() << " elements (table: " << _tableName << ")");

    auto i = _values.begin();
    std::advance(i, idx);
    return i->first;
  }

  void load()
  {
    statN(_dbKey)[_tableName][_pkey]["load #"] += 1;
    ACS_COND_THROW(_tableName.empty() || _pkey.empty() || _col.empty(),
                   exIllegalValueException("NameIdAssociator::load error: invalid mandatory field (table,pkey,col)"));

    try
    {
      ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "_dbKey: \"" << _dbKey << "\"");

      // WAS: string sqlString = "SELECT " + _pkey +", " + _col + " FROM " + _tableName + " ORDER BY " + _pkey ;
      SQLString sqlString(_dbKey, SQLString::SELECT);
      sqlString.addSelectList(_pkey);
      sqlString.addSelectList(_col);
      sqlString.addTableList(_tableName);
      sqlString.addOrderByList(_oby);

      ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "sqlString: \"" << sqlString.getSQL());

      dbSet set;
      dbQuery::execForSet(set, db::ConnPools::instance()->getPool(_dbKey), sqlString,
                          "NISEL " + _tableName + "|" + _pkey);
      dbRow row(_dbKey);
      int nulls = 0;
      std::map<T, std::string> new_values;

      while (set.getNextRow(row))
      {
        bool isNull = false;

        T v;
        row.column(0, v, &isNull);
        if (isNull)
        {
          ACS_LOG_ERROR("Null value in lookup table \"" << _tableName << '.' << _pkey << " ignoring the row");
          continue;
        }

        isNull = false;
        std::string tmp;
        row.column(1, tmp, &isNull);
        if (isNull)
        {
          ++nulls;
          ACS_LOG_ERROR("Null value in lookup \"" << _tableName << '.' << _col << "\" for \"" << v << "\" in col "
                                                  << _pkey << " keep going but results could be unpredictable")
          if (nulls < 2)
          {
            new_values[v] = StringUtils::trim(tmp, StringUtils::is_not_space);
          }
          else
          {
            ACS_LOG_ERROR("Ignoring \"" << _tableName << '.' << _pkey << "[" << v << "] value")
          }
        }
        else
        {
          new_values[v] = StringUtils::trim(tmp, StringUtils::is_not_space);
        }
      }

      // here below the magic
      _values.swap(new_values);

      // Save the MAP on disk only if allowed by configuration
      if (NameIdAssociatorConfiguration::instance()->getEnableDiskMaps())
      {
        saveMap();
      }

      _nextLoadAfter = DateTime() + RELOAD_TIMEOUT;
    }
    catch (const std::exception& e)  // NOSONAR - Want to catch everything
    {
      if (NameIdAssociatorConfiguration::instance()->getEnableDiskMaps())
      {
        // Try to recover only if allowed by configuration
        ACS_LOG_NOTIFY_EX(e);
        ACS_LOG_WARNING("Trying to recover loading from disk (if any and/or applicable)");
        loadMap();
        ACS_LOG_INFO("Recover successful - please try to fix the above error ASAP!");
      }
      else
      {
        throw;
      }
    }
  }

  [[deprecated("This will not order the resultant map! To be removed")]] void orderBy(const std::string& oby)
  {
    ACS_LOG_ERROR("Be careful, this will not order the resultant map!"
                  << " dbKey = '" << _dbKey
                  << "' table Name = '" << _tableName
                  << "' primary key = '" << _pkey
                  << "' column = '" << _col << "'");
    _oby = oby;
  }

private:
  static math::StatNode& statN(const std::string& key) { return db::stat(key)["NameIdAssociator"]; }

  std::string getMapPath() const
  {
    std::ostringstream os;
    os << ResourceSpaceFactory::getConfigurationRepository() << "/"
       << _dbKey << "_"
       << StringUtils::lowercase(_tableName) << "_"
       << StringUtils::lowercase(_pkey) << "_"
       << StringUtils::lowercase(_col) << ".map";
    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "path: " << os.str());
    return os.str();
  }

  void saveMap() const noexcept
  {
    std::string path = getMapPath();
    try
    {
      File::Lock lock(path);
      File::Lock::tryLock(lock);  // will try 10 times with 1 second timeout, then File::Lock::IOException

      std::ofstream out(path.c_str());

      out.exceptions(std::ios::badbit | std::ios::failbit);
      if (_percentEnc)
      {
        for (const auto& el : _values)
        {
          out << percentEncode(el.first) << "\n"
              << internalPercentEncode(el.second) << "\n";
        }
      }
      else
      {
        for (const auto& el : _values)
        {
          out << el.first << "\n"
              << el.second << "\n";
        }
      }

      // change permissions allowing other users to override this file later.
      changePermissions(path);
    }
    catch (const std::exception& e)  // NOSONAR - Want to catch everything
    {
      // Even in case of troubles... try to change the permissions of the file
      changePermissions(path);

      // cannot rethrow, just notify
      ACS_LOG_NOTIFY_EX(exIOException(e, "Writing file: \"" + path + "\""));
    }
  }

  void changePermissions(const std::string& path) const noexcept
  {
    try
    {
      File::chmod(path, 0664);
    }
    catch (const std::exception&)  // NOSONAR - Want to catch everything
    {
      // Do nothing...  this could happen and it is not a problem
    }
  }

  //! internal method to load an Id of generic type T from a string
  //! this method is used by the loadMap method
  void loadId(T& id, const std::string& line)
  {
    std::istringstream ins1(line);
    ins1 >> id;
  }

#ifdef TEST_NAMEIDASSOCIATOR
public:
#endif
  void loadMap()
  {
    statN(_dbKey)[_tableName][_pkey]["loadMap #"] += 1;
    std::string path = getMapPath();
    ACS_COND_THROW(not File::exists(path),
                   exIOException(std::string("File \"") + path + "\" does not exists"));
    try
    {
      ACS_LOG_INFO("Loading map from file: \"" << path << "\"");
      File::Lock lock(path);
      File::Lock::tryLock(lock);  // will try 10 times with 1 second timeout, then File::Lock::IOException

      std::ifstream in(path.c_str());
      in.exceptions(std::ios::badbit);
      int nulls = 0;
      std::string lineKey;
      std::string lineVak;
      while (std::getline(in, lineKey) && std::getline(in, lineVak))
      {
        T key;
        loadId(key, _percentEnc ? percentDecode(lineKey) : lineKey);
        std::string val = _percentEnc ? percentDecode(lineVak) : lineVak;
        if (val.empty())
        {
          ACS_LOG_ERROR("Null value from map file  \"" << path << "\" corresponding to \"" << key
                                                       << "\" keep going but results could be unpredictable")
          ++nulls;
          if (nulls > 1)
          {
            ACS_LOG_ERROR("\"" << key << "\" is ignored for file " << path)
            continue;
          }
        }

        _values[key] = val;
      }
      ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "map loaded");
    }
    catch (const std::exception& e)  // NOSONAR - Want to catch everything
    {
      ACS_THROW(exIOException(e) << "Loading file: \"" << path << "\"");
    }
  }

#ifdef TEST_NAMEIDASSOCIATOR
private:
#endif
  //! returns the id associated with the given name
  //! an exception of type exIllegalValueException is thrown if name is not found
  //! \param name : the name whose id shall be found and returned
  //! \param sanitize : if true the given name and name entries will be sanitized through StringUtils::sanitizeDbName
  //! function before being compared
  //! \param caseInsensitive : if true the case will not be used to find the name
  const T& idInternal(const std::string& name, bool sanitize, bool caseInsensitive) const
  {
    // Build the compare function
    std::function<bool(const std::pair<T, std::string>&)> compare;
    if (sanitize)
    {
      // In sanitize case spaces and punctuations will be removed from the name using sanitizeDbName on both name
      // and element.second
      if (caseInsensitive)
      {
        // In caseInsensitive case, use equalsNoCase for comparison
        compare = [localName = StringUtils::sanitizeDbName(name)](const std::pair<T, std::string>& element) {
          return StringUtils::equalsNoCase(StringUtils::sanitizeDbName(element.second), localName);
        };
      }
      else
      {
        // Otherwise use standard comparison
        compare = [localName = StringUtils::sanitizeDbName(name)](const std::pair<T, std::string>& element) {
          return StringUtils::sanitizeDbName(element.second) == localName;
        };
      }
    }
    else
    {
      // In non sanitize case both name and element.second are let unchanged
      if (caseInsensitive)
      {
        // In caseInsensitive case, use equalsNoCase for comparison
        compare = [&name](const std::pair<T, std::string>& element) {
          return StringUtils::equalsNoCase(element.second, name);
        };
      }
      else
      {
        // Otherwise use standard comparison
        compare = [&name](const std::pair<T, std::string>& element) { return element.second == name; };
      }
    }

    auto it = std::find_if(_values.begin(), _values.end(), compare);
    // S3MARINE-656: If the requested name cannot be found, try reloading the map. Maybe there has been a hot change
    //               in the DB?
    if (it == _values.end() && DateTime() > _nextLoadAfter)
    {
      // S3MARINE-656: In order to keep the constness of this method, is needed to cast away the constness in the load
      //               call. This is not good, but it is the only solution to keep the method signature unchanged and
      //               avoid errors on other components. Using mutable for variables that are changed by the load method
      //               would allow them to be changed in other constant functions as well, which would be even worse.
      const_cast<NameIdAssociator<T>*>(this)->load();
      it = std::find_if(_values.begin(), _values.end(), compare);
    }

    if (it == _values.end())
    {
      statN(_dbKey)[_tableName][_pkey]["name #"]["error"] += 1;
      ACS_THROW(exIllegalValueException("String not found : \"" + name + "\"" + " in table " + _tableName));
    }

    return it->first;
  }

  inline std::string percentEncode(const T& val) const noexcept
  {
    std::ostringstream os;
    os << val;
    return internalPercentEncode(os.str());
  }

  static std::string internalPercentEncode(const std::string& val) noexcept
  {
    std::ostringstream os;
    for (const auto& c : val)
    {
      if (c == '\n')
      {
        os << "%10";
      }
      else if (c == '\r')
      {
        os << "%13";
      }
      else if (c == '%')
      {
        os << "%37";
      }
      else
      {
        os << c;
      }
    }

    return os.str();
  }

  static std::string percentDecode(const std::string& val) noexcept
  {
    std::ostringstream os;
    std::string tmp = "";
    tmp.reserve(4);
    for (const auto& c : val)
    {
      // Defensive coding: '%' should be encoded so no way to have two '%' at less than 2 char distance
      if (c == '%' && tmp.size() > 0)
      {
        os << tmp;
        tmp = c;
      }
      else if (c == '%' || tmp.size() > 0)
      {
        tmp += c;
      }
      else
      {
        os << c;
      }

      if (tmp.size() == 3)
      {
        if (tmp == "%10")
        {
          os << '\n';
        }
        else if (tmp == "%13")
        {
          os << '\r';
        }
        else if (tmp == "%37")
        {
          os << '%';
        }
        else
        {
          os << tmp;
        }
        tmp = "";
      }
    }

    // flush last chars, if any
    if (tmp.size() > 0)
    {
      os << tmp;
    }

    return os.str();
  }

  // data section
  std::string _dbKey;
  std::string _tableName;
  std::string _pkey;
  std::string _col;
  std::map<T, std::string> _values{};
  std::string _oby;
  bool _percentEnc = false;
  DateTime _nextLoadAfter = DateTime() + RELOAD_TIMEOUT;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(NameIdAssociator);
};

ACS_TEMPLATE_DEFINE_DEBUG_LEVEL(NameIdAssociator, T);

template<>
inline void NameIdAssociator<std::string>::loadId(std::string& id, const std::string& line)
{
  id = line;
}

template<>
inline std::string NameIdAssociator<std::string>::percentEncode(const std::string& val) const noexcept
{
  return internalPercentEncode(val);
}

template<class T>
inline std::ostream& operator<<(std::ostream& os, const acs::NameIdAssociator<T>& n)  // output to stream
{
  for (unsigned int i = 0; i < n.size(); ++i)
  {
    os << n.idByPos(i) << " - " << n[i] << "\n";
  }

  return os;
}

template<class T>
inline acs::exostream& operator<<(acs::exostream& os, acs::NameIdAssociator<T>& n)  // output to stream
{
  for (unsigned int i = 0; i < n.size(); ++i)
  {
    os << n.idByPos(i) << " - " << n[i] << "\n";
  }

  return os;
}

using pattern::ParamConstructibleSingletonMap;

//! This is the Singleton implementation for the NameIdAssociator.
template<const char* table, const char* idField, const char* nameField, typename IdType, bool percentEnc = false>
class NameIdAssociatorSingleton
  : public NameIdAssociator<IdType>,
    public ParamConstructibleSingletonMap<NameIdAssociatorSingleton<table, idField, nameField, IdType, percentEnc>, std::string>
{
  friend NameIdAssociatorSingleton* ParamConstructibleSingletonMap<NameIdAssociatorSingleton<table, idField, nameField, IdType, percentEnc>, std::string>::instance(const std::string&);

public:
  NameIdAssociatorSingleton& operator=(const NameIdAssociatorSingleton&) = delete;
  NameIdAssociatorSingleton(const NameIdAssociatorSingleton&) = delete;
  NameIdAssociatorSingleton() = delete;
  virtual ~NameIdAssociatorSingleton() noexcept = default;

protected:
  explicit NameIdAssociatorSingleton(const std::string& key) :
    NameIdAssociator<IdType>(table, idField, nameField, key, percentEnc),
    ParamConstructibleSingletonMap<NameIdAssociatorSingleton<table, idField, nameField, IdType, percentEnc>, std::string>()
  {
    NameIdAssociator<IdType>::load();
  }
};

_ACS_END_NAMESPACE

#endif  // _NameIdAssociator_H_
