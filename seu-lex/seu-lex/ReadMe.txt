========================================================================
    CONSOLE APPLICATION : seu-lex Project Overview
========================================================================

AppWizard has created this seu-lex application for you.

This file contains a summary of what you will find in each of the files that
make up your seu-lex application.


seu-lex.vcxproj
    This is the main project file for VC++ projects generated using an Application Wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

seu-lex.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

seu-lex.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named seu-lex.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

seu_lex流程：
	RE预处理
	RE -> NFA
	NFA合并
	NFA -> DFA
	DFA最小化
	生成yy.lex.c

/////////////////////////////////////////////////////////////////////////////
