Dim WinScriptHost
Set WinScriptHost = CreateObject("WScript.Shell")
WinScriptHost.Run Chr(34) & "clear_traces.cmd" & Chr(34), 0
Set WinScriptHost = Nothing
