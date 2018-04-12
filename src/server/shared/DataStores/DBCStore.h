/*
 * Copyright (C) 2008-2018 TrinityCore <https://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DBCSTORE_H
#define DBCSTORE_H

#include "Common.h"
#include "DBCStorageIterator.h"
#include <vector>

 /// Interface class for common access
class TC_SHARED_API DBCStorageBase
{
    public:
        DBCStorageBase(char const* fmt);
        virtual ~DBCStorageBase();

        char const* GetFormat() const { return _fileFormat; }
        uint32 GetFieldCount() const { return _fieldCount; }

        virtual bool Load(std::string const& path) = 0;
        virtual bool LoadStringsFrom(std::string const& path) = 0;
        virtual void LoadFromDB(std::string const& path, std::string const& dbFormat, std::string const& primaryKey) = 0;

    protected:
        bool Load(std::string const& path, char**& indexTable);
        bool LoadStringsFrom(std::string const& path, char** indexTable);
        void LoadFromDB(std::string const& path, std::string const& dbFormat, std::string const& primaryKey, char**& indexTable);

        uint32 _fieldCount;
        char const* _fileFormat;
        char* _dataTable;
        char* _dataTableEx;
        std::vector<char*> _stringPool;
        uint32 _indexTableSize;
};

template <class T>
class DBCStorage : public DBCStorageBase
{
    public:
<<<<<<< HEAD
        explicit DBCStorage(char const* f)
#ifdef ELUNA
            : fmt(f), nCount(0), fieldCount(0), dataTable(NULL), maxdatacount(0), mindatacount(std::numeric_limits<uint32>::max())
#else
            : fmt(f), nCount(0), fieldCount(0), dataTable(NULL)
#endif
        {
            indexTable.asT = NULL;
        }
=======
        typedef DBCStorageIterator<T> iterator;
>>>>>>> e5bb22a388a932ca268e0fafb0edf592b1fe6990

        explicit DBCStorage(char const* fmt) : DBCStorageBase(fmt)
        {
<<<<<<< HEAD
#ifdef ELUNA
            if (id <= maxdatacount && id >= mindatacount)
            {
                typename std::unordered_map<uint32, T const*>::const_iterator it = data.find(id);
                if (it != data.end())
                    return it->second;
            }
#endif
            return (id >= nCount) ? NULL : indexTable.asT[id];
=======
            _indexTable.AsT = nullptr;
>>>>>>> e5bb22a388a932ca268e0fafb0edf592b1fe6990
        }

        ~DBCStorage()
        {
            delete[] reinterpret_cast<char*>(_indexTable.AsT);
        }

<<<<<<< HEAD
#ifdef ELUNA
        void SetEntry(uint32 id, T* t)
        {
            delete data[id];
            data[id] = t;
            maxdatacount = std::max(maxdatacount, id);
            mindatacount = std::min(mindatacount, id);
        }
#endif

#ifdef ELUNA
        uint32  GetNumRows() const { return std::max(maxdatacount + 1, nCount); }
#else
        uint32  GetNumRows() const { return nCount; }
#endif
        char const* GetFormat() const { return fmt; }
        uint32 GetFieldCount() const { return fieldCount; }

        bool Load(char const* fn, SqlDbc* sql)
        {
            DBCFileLoader dbc;
            // Check if load was sucessful, only then continue
            if (!dbc.Load(fn, fmt))
                return false;

            uint32 sqlRecordCount = 0;
            uint32 sqlHighestIndex = 0;
            Field* fields = NULL;
            QueryResult result = QueryResult(NULL);
            // Load data from sql
            if (sql)
            {
                std::string query = "SELECT * FROM " + sql->sqlTableName;
                if (sql->indexPos >= 0)
                    query +=" ORDER BY " + *sql->indexName + " DESC";
                query += ';';


                result = WorldDatabase.Query(query.c_str());
                if (result)
                {
                    sqlRecordCount = uint32(result->GetRowCount());
                    if (sql->indexPos >= 0)
                    {
                        fields = result->Fetch();
                        sqlHighestIndex = fields[sql->sqlIndexPos].GetUInt32();
                    }

                    // Check if sql index pos is valid
                    if (int32(result->GetFieldCount() - 1) < sql->sqlIndexPos)
                    {
                        TC_LOG_ERROR("server.loading", "Invalid index pos for dbc:'%s'", sql->sqlTableName.c_str());
                        return false;
                    }
                }
            }

            char* sqlDataTable = NULL;
            fieldCount = dbc.GetCols();

            dataTable = reinterpret_cast<T*>(dbc.AutoProduceData(fmt, nCount, indexTable.asChar,
                sqlRecordCount, sqlHighestIndex, sqlDataTable));
=======
        T const* LookupEntry(uint32 id) const { return (id >= _indexTableSize) ? nullptr : _indexTable.AsT[id]; }
        T const* AssertEntry(uint32 id) const { return ASSERT_NOTNULL(LookupEntry(id)); }
>>>>>>> e5bb22a388a932ca268e0fafb0edf592b1fe6990

        uint32 GetNumRows() const { return _indexTableSize; }

        bool Load(std::string const& path) override
        {
            return DBCStorageBase::Load(path, _indexTable.AsChar);
        }

        bool LoadStringsFrom(std::string const& path) override
        {
            return DBCStorageBase::LoadStringsFrom(path, _indexTable.AsChar);
        }

        void LoadFromDB(std::string const& path, std::string const& dbFormat, std::string const& primaryKey) override
        {
<<<<<<< HEAD
#ifdef ELUNA
            data.clear();
            maxdatacount = 0;
            mindatacount = std::numeric_limits<uint32>::max();
#endif

            if (!indexTable.asT)
                return;

            delete[] reinterpret_cast<char*>(indexTable.asT);
            indexTable.asT = NULL;
            delete[] reinterpret_cast<char*>(dataTable);
            dataTable = NULL;

            while (!stringPoolList.empty())
            {
                delete[] stringPoolList.front();
                stringPoolList.pop_front();
            }

            nCount = 0;
=======
            DBCStorageBase::LoadFromDB(path, dbFormat, primaryKey, _indexTable.AsChar);
>>>>>>> e5bb22a388a932ca268e0fafb0edf592b1fe6990
        }

        iterator begin() { return iterator(_indexTable.AsT, _indexTableSize); }
        iterator end() { return iterator(_indexTable.AsT, _indexTableSize, _indexTableSize); }

    private:
        union
        {
            T** AsT;
            char** AsChar;
        }
        _indexTable;

#ifdef ELUNA
        uint32 maxdatacount;
        uint32 mindatacount;
        std::unordered_map<uint32, T const*> data;
#endif

        DBCStorage(DBCStorage const& right) = delete;
        DBCStorage& operator=(DBCStorage const& right) = delete;
};

#endif
