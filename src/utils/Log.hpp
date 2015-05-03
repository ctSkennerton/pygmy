//=======================================================================
// Copyright 2008, Dalhousie University
// Author: Donovan Parks
//
// The contents of this file are licensed under the 
// Attribution-ShareAlike Creative Commons License:
// http://creativecommons.org/licenses/by-sa/3.0/
//=======================================================================

#ifndef _LOG_
#define _LOG_

namespace utils
{

/**
 * @brief Singleton class for writing general information, warnings, and errors 
 *				to both the application console and to file.
 */
class Log
{
public:
	/** Destructor */
	~Log()
	{
        for(QString str : m_logData)
		{
			m_log << str.c_str() << std::endl;
		}

		m_log.close();
	}

	/** Get the one and only instance of this class.. */
	static Log& Inst()
	{
		static Log log;
		return log;
	}

	/** Add text to log file. */
    void Add(const QString text)
	{
		m_logData.push_back(text);
	}

	/** Add text to log file. */
	void Write(const wchar_t* text)
	{
		m_log << text << std::endl;

		if(m_console != NULL)
		{
			m_console->AppendText(wxString(text, wxConvUTF8) + _T("\n"));
			
			wxTextPos pos = m_console->GetLastPosition();
			m_console->ShowPosition(pos);
		}
	}

	/** Add text to log file. */
    void Write(const QString text)
	{
		Write(text.c_str());
	}
  
	/** Add text to log file. */
	void Warning(const wchar_t* text)
	{
		m_log << text << std::endl;

		if(m_console != NULL)
		{
			m_console->BeginTextColour(wxColour(128, 0, 0));
			m_console->BeginBold();
			m_console->AppendText(wxString(text, wxConvUTF8) + _T("\n"));
			m_console->EndBold();
			m_console->EndTextColour();

			wxTextPos pos = m_console->GetLastPosition();
			m_console->ShowPosition(pos);
		}
		else
		{
			wxMessageBox(text, _T("Warning"), wxOK | wxICON_INFORMATION);
		}
	}

	/** Add text to log file. */
    void Warning(const QString text)
	{
		Warning(text.c_str());
	}

	/** Add text to log file. */
	void Error(const wchar_t* text)
	{
		m_log << text << std::endl;

		if(m_console != NULL)
		{
			m_console->BeginTextColour(wxColour(255, 0, 0));
			m_console->BeginBold();
			m_console->AppendText(wxString(text, wxConvUTF8) + _T("\n"));
			m_console->EndBold();
			m_console->EndTextColour();

			wxTextPos pos = m_console->GetLastPosition();
			m_console->ShowPosition(pos);
		}
		else
		{
			wxMessageBox(text, _T("Error"), wxOK | wxICON_ERROR);
		}
	}

	/** Add text to log file. */
    void Error(const QString text)
	{
		Error(text.c_str());
	}

	/** Set output console. */
	void SetConsole(wxRichTextCtrl* console) { m_console = console; }

protected:
	/** Private singleton constructor (must instantiate this class through Inst()). */
  Log()
	{
		wxString logFile = pygmy::App::Inst().GetExeDir() + _T("Log.txt");
		m_log.open(logFile.ToAscii());
		m_logData.reserve(1000);	// allocate enough space to store a thousand entries

		m_console = NULL;
	}

	/** Private copy constructor (must instantiate this class through Inst()). */
  Log(const Log&);

	/** Private assignment constructor (must instantiate this class through Inst()). */
  Log& operator= (const Log&);


private:
	/** Log stream */
	std::wofstream m_log;

	/** Vector of log data. */
    std::vector<QString> m_logData;

	/** Console to output messages to. */
	wxRichTextCtrl* m_console;
};

}

#endif
