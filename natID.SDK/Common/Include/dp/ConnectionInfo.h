// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <dp/IDatabase.h>
#include <td/String.h>

namespace dp
{
	class ConnectionInfo
	{
	protected:
		IDatabase::ServerType _serverType;
		IDatabase::VerMSSQLServer _msSQLVersion;
		IDatabase::VerORACLE _oracleVersion;
		td::String _host;
		td::String _un;
		td::String _pwd;
		td::String _h1;
		td::String _h2;
	public:
		ConnectionInfo(IDatabase::ServerType serverType, int serverVersion, const td::String& host, const td::String& un, const td::String& pwd)
			: _serverType(serverType)
			, _host(host)
			, _un(un)
			, _pwd(pwd)
		{			
			setVersion(serverVersion);
		}

		ConnectionInfo(const td::String& h1, const td::String& h2)
			: _serverType(dp::IDatabase::ServerType::SER_NA)
			, _msSQLVersion(dp::IDatabase::VerMSSQLServer::MS_SQLNA)
			, _oracleVersion(dp::IDatabase::VerORACLE::ORA_NA)
			, _h1(h1)
			, _h2(h2)
		{
		}

		IDatabase::ServerType getServerType() const
		{
			return _serverType;
		}

		IDatabase::VerMSSQLServer getMSSQLVersion() const
		{
			return _msSQLVersion;
		}

		IDatabase::VerORACLE getOracleVersion() const
		{
			return _oracleVersion;
		}

		const td::String& getServerHost() const
		{
			return _host;
		}

		const td::String& getUserName() const
		{
			return _un;
		}

		const td::String& getPWD() const
		{
			return _pwd;
		}

		int getVersion() const
		{
			if (_serverType == IDatabase::ServerType::SER_MSSQL)
				return (int)_msSQLVersion;
			else if (_serverType == IDatabase::ServerType::SER_ORACLE)
				return (int)_oracleVersion;
			return 0;			
		}

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
