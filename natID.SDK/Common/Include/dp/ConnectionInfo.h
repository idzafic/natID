// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ConnectionInfo.h
 * @brief Holds database connection parameters such as server type, version, host, credentials.
 */
#pragma once
#include <dp/IDatabase.h>
#include <td/String.h>

namespace dp
{
    /// @brief Stores all information required to establish a database connection.
	class ConnectionInfo
	{
	protected:
		IDatabase::ServerType _serverType;       ///< Type of the target database server.
		IDatabase::VerMSSQLServer _msSQLVersion; ///< MS SQL Server version (relevant when server type is SER_MSSQL).
		IDatabase::VerORACLE _oracleVersion;     ///< Oracle version (relevant when server type is SER_ORACLE).
		td::String _host;                        ///< Hostname or IP address of the database server.
		td::String _un;                          ///< Database user name.
		td::String _pwd;                         ///< Password for the database user.
		td::String _h1;                          ///< Auxiliary host string (used for alternative connection modes).
		td::String _h2;                          ///< Second auxiliary host string.
	public:
        /// @brief Constructs a ConnectionInfo for a specific server type with credentials.
        /// @param serverType The type of the target database server.
        /// @param serverVersion Integer encoding of the server version.
        /// @param host Hostname or IP address of the server.
        /// @param un Database user name.
        /// @param pwd Password for the user.
		ConnectionInfo(IDatabase::ServerType serverType, int serverVersion, const td::String& host, const td::String& un, const td::String& pwd)
			: _serverType(serverType)
			, _host(host)
			, _un(un)
			, _pwd(pwd)
		{
			setVersion(serverVersion);
		}

        /// @brief Constructs a ConnectionInfo with two auxiliary host strings and no specific server type.
        /// @param h1 First auxiliary host string.
        /// @param h2 Second auxiliary host string.
		ConnectionInfo(const td::String& h1, const td::String& h2)
			: _serverType(dp::IDatabase::ServerType::SER_NA)
			, _msSQLVersion(dp::IDatabase::VerMSSQLServer::MS_SQLNA)
			, _oracleVersion(dp::IDatabase::VerORACLE::ORA_NA)
			, _h1(h1)
			, _h2(h2)
		{
		}

        /// @brief Returns the server type stored in this connection info.
        /// @return The IDatabase::ServerType value.
		IDatabase::ServerType getServerType() const
		{
			return _serverType;
		}

        /// @brief Returns the MS SQL Server version.
        /// @return The IDatabase::VerMSSQLServer enum value.
		IDatabase::VerMSSQLServer getMSSQLVersion() const
		{
			return _msSQLVersion;
		}

        /// @brief Returns the Oracle database version.
        /// @return The IDatabase::VerORACLE enum value.
		IDatabase::VerORACLE getOracleVersion() const
		{
			return _oracleVersion;
		}

        /// @brief Returns the hostname or IP address of the server.
        /// @return Constant reference to the host string.
		const td::String& getServerHost() const
		{
			return _host;
		}

        /// @brief Returns the database user name.
        /// @return Constant reference to the user name string.
		const td::String& getUserName() const
		{
			return _un;
		}

        /// @brief Returns the password for the database user.
        /// @return Constant reference to the password string.
		const td::String& getPWD() const
		{
			return _pwd;
		}

        /// @brief Returns the server version as an integer for the current server type.
        /// @return Integer version value, or 0 if the server type has no version.
		int getVersion() const
		{
			if (_serverType == IDatabase::ServerType::SER_MSSQL)
				return (int)_msSQLVersion;
			else if (_serverType == IDatabase::ServerType::SER_ORACLE)
				return (int)_oracleVersion;
			return 0;
		}

        /// @brief Sets the server version based on the current server type.
        /// @param serverVersion Integer encoding of the version to store.
		void setVersion(int serverVersion)
		{
			if (_serverType == IDatabase::ServerType::SER_MSSQL)
			{
				_msSQLVersion = (IDatabase::VerMSSQLServer) serverVersion;
				_oracleVersion = IDatabase::VerORACLE::ORA_NA;
			}
			else if (_serverType == IDatabase::ServerType::SER_ORACLE)
			{
				_msSQLVersion = IDatabase::VerMSSQLServer::MS_SQLNA;
				_oracleVersion = (IDatabase::VerORACLE) serverVersion;
			}
			else
			{
				_msSQLVersion = IDatabase::VerMSSQLServer::MS_SQLNA;
				_oracleVersion = IDatabase::VerORACLE::ORA_NA;
			}
		}
	};
}
