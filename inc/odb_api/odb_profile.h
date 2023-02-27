#ifndef	__ODB_PROFILE_H__ 
#define	__ODB_PROFILE_H__ 
 
#include <string> 
#include <map> 
#include <set> 
 
using namespace std; 
 
namespace NProfile 
{ 
 
#define MAX_LINE_LENGTH		1000 
 
class CProfile 
{ 
public: 
	typedef map<string, string> KeyMapType; 
	typedef map<string, KeyMapType> SectionMapType; 
	typedef set<string> StringSetType;	 
 
private: 
	SectionMapType	m_mapSection; 
	string			m_strFileName; 
 
	bool GetSection(string &strSource); 
	bool GetKeyAndValue(const string &strSource, string &strKey, string &strValue); 
 
	bool Trim(string &strSource); 
 
public: 
	CProfile(); 
	~CProfile(); 
	
	CProfile(const char *pProfileName); 
	CProfile(const string &strProfileName); 
 
	void SetFileName(const char *pProfileName); 
	void SetFileName(const string &strProfileName); 
 
	int ReadFile(void); 
 
	bool IsSectionExists(const string &strSection); 
	bool IsKeyExists(const string &strSection, const string &strKey); 
	 
	string GetString(const string &strSection, const string &strKey,  
		const string &strDefalut); 
 
	int GetInt(const string &strSection, const string &strKey, int nDefault); 
 
	void GetSections(StringSetType &setSections); 
	void GetSectionKeys(StringSetType &setKeys, const string &strSection); 
	void GetSectionValues(StringSetType &setValues, const string &strSection); 
	 
	int GetKeyByValue(string& strKey, const char* pValue);     // 
	int GetKeyByValue(string& strKey, const string& strValue); // 
	 
	void PrintAllValue(void); 
	void TestFunctions(void); 
}; 

	CProfile * CProfileInstance();
	
 
}	//End of namespace NProfile 
#endif	//	__PROFILE_FILE_H__ 
