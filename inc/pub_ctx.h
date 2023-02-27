
#ifndef	__PUB_CTX_H__
#define	__PUB_CTX_H__

#ifdef  _WINDOWS32
#ifdef PUB_EXPORTS
#define PUB_API __declspec(dllexport)
#else
#define PUB_API __declspec(dllimport)
#endif
#else 
#define PUB_API
#endif
#define CTX_ARG_STR		"-ctx"
#define APP_ARG_STR		"-app"


#define PORT_CONTEXT_OFFSET	1000
#define SEM_KEY_CONTEXT_OFFSET	0x10000000  //0x10000000



class PUB_API CContext
{
private:
	static short m_ArgContextNo;
	static int   m_ArgAppNo;
	
	static int ParseArgs(int argc, char** argv);
	
public:
	
	//
	// utility functions 
	// 
	
	// Validate the given context no
	static int IsValidCtxNo(short ctx_no);
	
	// Convert to linear value(from 1,2,4,8 to 0,1,2,3), would be used as offset value
	static int Convert2Linear(short ctx_no);
	
	// 
	static int ShiftPortNo(int base_port, short ctx_no = 0);
	static int ShiftSemKey(int base_key, short ctx_no = 0);
	
	//
	// set interface
	// 
	
	// This function should be called at the begining of main() function.
	// Valid context should be either parsed from command line arguments or set by default context parameter.
	// Otherwise, the function will exit.
	static int SetContextNo(int argc, char** argv, const short def_ctx_no = 0);


	
	//
	// get interface
	// 
	static int GetContextNo(short & context_no);
	static int GetAppNo(int & application_no, const int def_app_no = 0);
	
	
	//
	// get interface ( for client-side programs )
	// 
	static int GetCurCtxNo(short & context_no, const short def_ctx_no = 0);
	static int GetCtxNameByCtxNo(const short& context_no, char* context_name);
};


#endif
