/****************************************************************************
**
** Implementation of VCProject class.
**
** Copyright (C) 2002-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of qmake.
**
** This file may be used under the terms of the GNU General
** Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the files LICENSE.GPL2
** and LICENSE.GPL3 included in the packaging of this file.
** Alternatively you may (at your option) use any later version
** of the GNU General Public License if such license has been
** publicly approved by Trolltech ASA (or its successors, if any)
** and the KDE Free Qt Foundation.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/.
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** This file may be used under the terms of the Q Public License as
** defined by Trolltech ASA and appearing in the file LICENSE.QPL
** included in the packaging of this file.  Licensees holding valid Qt
** Commercial licenses may use this file in accordance with the Qt
** Commercial License Agreement provided with the Software.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not granted
** herein.
**
**********************************************************************/

#include "msvc_objectmodel.h"
#include "msvc_vcproj.h"
#include <qtextstream.h>
#include <qstringlist.h>
#include <qfileinfo.h>

extern DotNET which_dotnet_version();

// XML Tags ---------------------------------------------------------
const char* _xmlInit				= "<?xml version=\"1.0\" encoding = \"Windows-1252\"?>";
const char* _begConfiguration			= "\n\t\t<Configuration";
const char* _begConfigurations			= "\n\t<Configurations>";
const char* _begFile				= "\n\t\t\t<File";
const char* _begFileConfiguration		= "\n\t\t\t\t<FileConfiguration";
const char* _begFiles				= "\n\t<Files>";
const char* _begFilter				= "\n\t\t<Filter";
const char* _begGlobals				= "\n\t<Globals>";
const char* _begPlatform			= "\n\t\t<Platform";
const char* _begPlatforms			= "\n\t<Platforms>";
const char* _begTool3				= "\n\t\t\t<Tool";
const char* _begTool5				= "\n\t\t\t\t\t<Tool";
const char* _begVisualStudioProject		= "\n<VisualStudioProject";
const char* _endConfiguration			= "\n\t\t</Configuration>";
const char* _endConfigurations			= "\n\t</Configurations>";
const char* _endFile				= "\n\t\t\t</File>";
const char* _endFileConfiguration		= "\n\t\t\t\t</FileConfiguration>";
const char* _endFiles				= "\n\t</Files>";
const char* _endFilter				= "\n\t\t</Filter>";
const char* _endGlobals				= "\n\t</Globals>";
const char* _endPlatforms			= "\n\t</Platforms>";
const char* _endVisualStudioProject		= "\n</VisualStudioProject>";

// XML Properties ---------------------------------------------------
const char* _AddModuleNamesToAssembly 		= "\n\t\t\t\tAddModuleNamesToAssembly=\"";
const char* _AdditionalDependencies4 		= "\n\t\t\t\tAdditionalDependencies=\"";
const char* _AdditionalDependencies6 		= "\n\t\t\t\t\t\tAdditionalDependencies=\"";
const char* _AdditionalIncludeDirectories 	= "\n\t\t\t\tAdditionalIncludeDirectories=\"";
const char* _AdditionalLibraryDirectories 	= "\n\t\t\t\tAdditionalLibraryDirectories=\"";
const char* _AdditionalOptions 			= "\n\t\t\t\tAdditionalOptions=\"";
const char* _AdditionalUsingDirectories 	= "\n\t\t\t\tAdditionalUsingDirectories=\"";
const char* _AssemblerListingLocation 		= "\n\t\t\t\tAssemblerListingLocation=\"";
const char* _AssemblerOutput 			= "\n\t\t\t\tAssemblerOutput=\"";
const char* _ATLMinimizesCRunTimeLibraryUsage	= "\n\t\t\tATLMinimizesCRunTimeLibraryUsage=\"";
const char* _BaseAddress 			= "\n\t\t\t\tBaseAddress=\"";
const char* _BasicRuntimeChecks 		= "\n\t\t\t\tBasicRuntimeChecks=\"";
const char* _BrowseInformation 			= "\n\t\t\t\tBrowseInformation=\"";
const char* _BrowseInformationFile 		= "\n\t\t\t\tBrowseInformationFile=\"";
const char* _BufferSecurityCheck 		= "\n\t\t\t\tBufferSecurityCheck=\"";
const char* _BuildBrowserInformation		= "\n\t\t\tBuildBrowserInformation=\"";
const char* _CPreprocessOptions 		= "\n\t\t\t\tCPreprocessOptions=\"";
const char* _CallingConvention 			= "\n\t\t\t\tCallingConvention=\"";
const char* _CharacterSet			= "\n\t\t\tCharacterSet=\"";
const char* _CommandLine4 			= "\n\t\t\t\tCommandLine=\"";
const char* _CommandLine6 			= "\n\t\t\t\t\t\tCommandLine=\"";
const char* _CompileAs 				= "\n\t\t\t\tCompileAs=\"";
const char* _CompileAsManaged 			= "\n\t\t\t\tCompileAsManaged=\"";
const char* _CompileOnly 			= "\n\t\t\t\tCompileOnly=\"";
const char* _ConfigurationType			= "\n\t\t\tConfigurationType=\"";
const char* _Culture				= "\n\t\t\t\tCulture=\"";
const char* _DLLDataFileName 			= "\n\t\t\t\tDLLDataFileName=\"";
const char* _DebugInformationFormat 		= "\n\t\t\t\tDebugInformationFormat=\"";
const char* _DefaultCharIsUnsigned 		= "\n\t\t\t\tDefaultCharIsUnsigned=\"";
const char* _DefaultCharType 			= "\n\t\t\t\tDefaultCharType=\"";
const char* _DelayLoadDLLs	 		= "\n\t\t\t\tDelayLoadDLLs=\"";
const char* _DeleteExtensionsOnClean		= "\n\t\t\tDeleteExtensionsOnClean=\"";
const char* _Description4 			= "\n\t\t\t\tDescription=\"";
const char* _Description6 			= "\n\t\t\t\t\t\tDescription=\"";
const char* _Detect64BitPortabilityProblems 	= "\n\t\t\t\tDetect64BitPortabilityProblems=\"";
const char* _DisableLanguageExtensions 		= "\n\t\t\t\tDisableLanguageExtensions=\"";
const char* _DisableSpecificWarnings 		= "\n\t\t\t\tDisableSpecificWarnings=\"";
const char* _EnableCOMDATFolding 		= "\n\t\t\t\tEnableCOMDATFolding=\"";
const char* _EnableErrorChecks 			= "\n\t\t\t\tEnableErrorChecks=\"";
const char* _EnableFiberSafeOptimizations 	= "\n\t\t\t\tEnableFiberSafeOptimizations=\"";
const char* _EnableFunctionLevelLinking 	= "\n\t\t\t\tEnableFunctionLevelLinking=\"";
const char* _EnableIntrinsicFunctions 		= "\n\t\t\t\tEnableIntrinsicFunctions=\"";
const char* _EntryPointSymbol 			= "\n\t\t\t\tEntryPointSymbol=\"";
const char* _ErrorCheckAllocations 		= "\n\t\t\t\tErrorCheckAllocations=\"";
const char* _ErrorCheckBounds 			= "\n\t\t\t\tErrorCheckBounds=\"";
const char* _ErrorCheckEnumRange 		= "\n\t\t\t\tErrorCheckEnumRange=\"";
const char* _ErrorCheckRefPointers 		= "\n\t\t\t\tErrorCheckRefPointers=\"";
const char* _ErrorCheckStubData 		= "\n\t\t\t\tErrorCheckStubData=\"";
const char* _ExceptionHandling 			= "\n\t\t\t\tExceptionHandling=\"";
const char* _ExcludedFromBuild 			= "\n\t\t\t\tExcludedFromBuild=\"";
const char* _ExpandAttributedSource 		= "\n\t\t\t\tExpandAttributedSource=\"";
const char* _ExportNamedFunctions		= "\n\t\t\t\tExportNamedFunctions=\"";
const char* _FavorSizeOrSpeed 			= "\n\t\t\t\tFavorSizeOrSpeed=\"";
const char* _Filter				= "\n\t\t\tFilter=\"";
const char* _ForceConformanceInForLoopScope 	= "\n\t\t\t\tForceConformanceInForLoopScope=\"";
const char* _ForceSymbolReferences 		= "\n\t\t\t\tForceSymbolReferences=\"";
const char* _ForcedIncludeFiles 		= "\n\t\t\t\tForcedIncludeFiles=\"";
const char* _ForcedUsingFiles 			= "\n\t\t\t\tForcedUsingFiles=\"";
const char* _FullIncludePath 			= "\n\t\t\t\tFullIncludePath=\"";
const char* _FunctionOrder 			= "\n\t\t\t\tFunctionOrder=\"";
const char* _GenerateDebugInformation 		= "\n\t\t\t\tGenerateDebugInformation=\"";
const char* _GenerateMapFile 			= "\n\t\t\t\tGenerateMapFile=\"";
const char* _GeneratePreprocessedFile 		= "\n\t\t\t\tGeneratePreprocessedFile=\"";
const char* _GenerateStublessProxies 		= "\n\t\t\t\tGenerateStublessProxies=\"";
const char* _GenerateTypeLibrary 		= "\n\t\t\t\tGenerateTypeLibrary=\"";
const char* _GlobalOptimizations 		= "\n\t\t\t\tGlobalOptimizations=\"";
const char* _HeaderFileName 			= "\n\t\t\t\tHeaderFileName=\"";
const char* _HeapCommitSize 			= "\n\t\t\t\tHeapCommitSize=\"";
const char* _HeapReserveSize 			= "\n\t\t\t\tHeapReserveSize=\"";
const char* _IgnoreAllDefaultLibraries 		= "\n\t\t\t\tIgnoreAllDefaultLibraries=\"";
const char* _IgnoreDefaultLibraryNames 		= "\n\t\t\t\tIgnoreDefaultLibraryNames=\"";
const char* _IgnoreEmbeddedIDL 			= "\n\t\t\t\tIgnoreEmbeddedIDL=\"";
const char* _IgnoreImportLibrary 		= "\n\t\t\t\tIgnoreImportLibrary=\"";
const char* _IgnoreStandardIncludePath 		= "\n\t\t\t\tIgnoreStandardIncludePath=\"";
const char* _ImportLibrary 			= "\n\t\t\t\tImportLibrary=\"";
const char* _ImproveFloatingPointConsistency 	= "\n\t\t\t\tImproveFloatingPointConsistency=\"";
const char* _InlineFunctionExpansion 		= "\n\t\t\t\tInlineFunctionExpansion=\"";
const char* _InterfaceIdentifierFileName 	= "\n\t\t\t\tInterfaceIdentifierFileName=\"";
const char* _IntermediateDirectory		= "\n\t\t\tIntermediateDirectory=\"";
const char* _KeepComments 			= "\n\t\t\t\tKeepComments=\"";
const char* _LargeAddressAware 			= "\n\t\t\t\tLargeAddressAware=\"";
const char* _LinkDLL 				= "\n\t\t\t\tLinkDLL=\"";
const char* _LinkIncremental 			= "\n\t\t\t\tLinkIncremental=\"";
const char* _LinkTimeCodeGeneration 		= "\n\t\t\t\tLinkTimeCodeGeneration=\"";
const char* _LinkToManagedResourceFile 		= "\n\t\t\t\tLinkToManagedResourceFile=\"";
const char* _MapExports 			= "\n\t\t\t\tMapExports=\"";
const char* _MapFileName 			= "\n\t\t\t\tMapFileName=\"";
const char* _MapLines 				= "\n\t\t\t\tMapLines =\"";
const char* _MergeSections 			= "\n\t\t\t\tMergeSections=\"";
const char* _MergedIDLBaseFileName 		= "\n\t\t\t\tMergedIDLBaseFileName=\"";
const char* _MidlCommandFile 			= "\n\t\t\t\tMidlCommandFile=\"";
const char* _MinimalRebuild 			= "\n\t\t\t\tMinimalRebuild=\"";
const char* _MkTypLibCompatible			= "\n\t\t\t\tMkTypLibCompatible=\"";
const char* _ModuleDefinitionFile 		= "\n\t\t\t\tModuleDefinitionFile=\"";
const char* _Name1				= "\n\tName=\"";
const char* _Name2				= "\n\t\tName=\"";
const char* _Name3				= "\n\t\t\tName=\"";
const char* _Name4				= "\n\t\t\t\tName=\"";
const char* _Name5				= "\n\t\t\t\t\tName=\"";
const char* _ObjectFile 			= "\n\t\t\t\tObjectFile=\"";
const char* _OmitFramePointers 			= "\n\t\t\t\tOmitFramePointers=\"";
const char* _OpenMP                             = "\n\t\t\t\tOpenMP=\"";
const char* _Optimization 			= "\n\t\t\t\tOptimization =\"";
const char* _OptimizeForProcessor 		= "\n\t\t\t\tOptimizeForProcessor=\"";
const char* _OptimizeForWindows98 		= "\n\t\t\t\tOptimizeForWindows98=\"";
const char* _OptimizeForWindowsApplication 	= "\n\t\t\t\tOptimizeForWindowsApplication=\"";
const char* _OptimizeReferences 		= "\n\t\t\t\tOptimizeReferences=\"";
const char* _OutputDirectory3			= "\n\t\t\tOutputDirectory=\"";
const char* _OutputDirectory4			= "\n\t\t\t\tOutputDirectory=\"";
const char* _OutputFile 			= "\n\t\t\t\tOutputFile=\"";
const char* _Outputs4 				= "\n\t\t\t\tOutputs=\"";
const char* _Outputs6 				= "\n\t\t\t\t\t\tOutputs=\"";
const char* _ParseFiles				= "\n\t\t\tParseFiles=\"";
const char* _PrecompiledHeaderFile 		= "\n\t\t\t\tPrecompiledHeaderFile=\"";
const char* _PrecompiledHeaderThrough 		= "\n\t\t\t\tPrecompiledHeaderThrough=\"";
const char* _PreprocessorDefinitions 		= "\n\t\t\t\tPreprocessorDefinitions=\"";
const char* _PrimaryOutput			= "\n\t\t\tPrimaryOutput=\"";
const char* _ProjectGUID			= "\n\tProjectGUID=\"";
const char* _ProjectType			= "\n\tProjectType=\"Visual C++\"";
const char* _ProgramDatabase			= "\n\t\t\tProgramDatabase=\"";
const char* _ProgramDataBaseFileName 		= "\n\t\t\t\tProgramDataBaseFileName=\"";
const char* _ProgramDatabaseFile 		= "\n\t\t\t\tProgramDatabaseFile=\"";
const char* _ProxyFileName 			= "\n\t\t\t\tProxyFileName=\"";
const char* _RedirectOutputAndErrors 		= "\n\t\t\t\tRedirectOutputAndErrors=\"";
const char* _RegisterOutput 			= "\n\t\t\t\tRegisterOutput=\"";
const char* _RelativePath			= "\n\t\t\t\tRelativePath=\"";
const char* _ResourceOnlyDLL 			= "\n\t\t\t\tResourceOnlyDLL=\"";
const char* _ResourceOutputFileName 		= "\n\t\t\t\tResourceOutputFileName=\"";
const char* _RuntimeLibrary 			= "\n\t\t\t\tRuntimeLibrary=\"";
const char* _RuntimeTypeInfo 			= "\n\t\t\t\tRuntimeTypeInfo=\"";
const char* _SccProjectName			= "\n\tSccProjectName=\"";
const char* _SccLocalPath			= "\n\tSccLocalPath=\"";
const char* _SetChecksum 			= "\n\t\t\t\tSetChecksum=\"";
const char* _ShowIncludes 			= "\n\t\t\t\tShowIncludes=\"";
const char* _ShowProgress 			= "\n\t\t\t\tShowProgress=\"";
const char* _SmallerTypeCheck 			= "\n\t\t\t\tSmallerTypeCheck=\"";
const char* _StackCommitSize 			= "\n\t\t\t\tStackCommitSize=\"";
const char* _StackReserveSize 			= "\n\t\t\t\tStackReserveSize=\"";
const char* _StringPooling 			= "\n\t\t\t\tStringPooling=\"";
const char* _StripPrivateSymbols 		= "\n\t\t\t\tStripPrivateSymbols=\"";
const char* _StructMemberAlignment 		= "\n\t\t\t\tStructMemberAlignment=\"";
const char* _SubSystem 				= "\n\t\t\t\tSubSystem=\"";
const char* _SupportUnloadOfDelayLoadedDLL 	= "\n\t\t\t\tSupportUnloadOfDelayLoadedDLL=\"";
const char* _SuppressStartupBanner 		= "\n\t\t\t\tSuppressStartupBanner=\"";
const char* _SwapRunFromCD			= "\n\t\t\t\tSwapRunFromCD=\"";
const char* _SwapRunFromNet 			= "\n\t\t\t\tSwapRunFromNet=\"";
const char* _TargetEnvironment			= "\n\t\t\t\tTargetEnvironment=\"";
const char* _TargetMachine 			= "\n\t\t\t\tTargetMachine=\"";
const char* _TerminalServerAware 		= "\n\t\t\t\tTerminalServerAware=\"";
const char* _ToolName				= "\n\t\t\t\tName=\"";
const char* _ToolPath 				= "\n\t\t\t\tPath=\"";
const char* _TreatWChar_tAsBuiltInType		= "\n\t\t\t\tTreatWChar_tAsBuiltInType=\"";
const char* _TurnOffAssemblyGeneration 		= "\n\t\t\t\tTurnOffAssemblyGeneration=\"";
const char* _TypeLibraryFile 			= "\n\t\t\t\tTypeLibraryFile=\"";
const char* _TypeLibraryName			= "\n\t\t\t\tTypeLibraryName=\"";
const char* _TypeLibraryResourceID 		= "\n\t\t\t\tTypeLibraryResourceID=\"";
const char* _UndefineAllPreprocessorDefinitions = "\n\t\t\t\tUndefineAllPreprocessorDefinitions=\"";
const char* _UndefinePreprocessorDefinitions 	= "\n\t\t\t\tUndefinePreprocessorDefinitions=\"";
const char* _UseOfATL				= "\n\t\t\tUseOfATL=\"";
const char* _UseOfMfc				= "\n\t\t\tUseOfMfc=\"";
const char* _UsePrecompiledHeader 		= "\n\t\t\t\tUsePrecompiledHeader=\"";
const char* _ValidateParameters 		= "\n\t\t\t\tValidateParameters=\"";
const char* _VCCLCompilerToolName		= "\n\t\t\t\tName=\"VCCLCompilerTool\"";
const char* _VCCustomBuildTool			= "\n\t\t\t\t\t\tName=\"VCCustomBuildTool\"";
const char* _VCLinkerToolName			= "\n\t\t\t\tName=\"VCLinkerTool\"";
const char* _VCResourceCompilerToolName		= "\n\t\t\t\tName=\"VCResourceCompilerTool\"";
const char* _VCMIDLToolName			= "\n\t\t\t\tName=\"VCMIDLTool\"";
const char* _Version1 				= "\n\tVersion=\"";
const char* _Version4 				= "\n\t\t\t\tVersion=\"";
const char* _WarnAsError 			= "\n\t\t\t\tWarnAsError=\"";
const char* _WarnLevel 				= "\n\t\t\t\tWarnLevel=\"";
const char* _WarningLevel 			= "\n\t\t\t\tWarningLevel=\"";
const char* _WholeProgramOptimization		= "\n\t\t\t\tWholeProgramOptimization=\"";

// Property name and value as Pairs ---------------------------------
struct TPair {
    TPair( const char* n, const triState v ) : name(n), value(v) {};
    const char* name;
    const triState value;
};
struct EPair {
    EPair( const char* n, const int v ) : name(n), value(v) {};
    const char* name;
    const int value;
};
struct LPair {
    LPair( const char* n, const long v ) : name(n), value(v) {};
    const char* name;
    const long value;
};
struct SPair {
    SPair( const char* n, const QString& v ) : name(n), value(v) {};
    const char* name;
    const QString& value;
};
struct XPair {
    XPair( const char* n, const QStringList& v, const char* s = "," ) : name(n), value(v), sep(s) {};
    const char* name;
    const QStringList& value;
    const char* sep;
};

// void streamSPair( QTextStream &strm, const char *n, const QString &s )

// Streaming operators for property Pairs ---------------------------
QTextStream &operator<<( QTextStream &strm, const TPair &prop )
{
    switch( prop.value ) {
    case _False:
	strm << prop.name << "FALSE\"";
	break;
    case _True:
	strm << prop.name << "TRUE\"";
	break;
    case unset:
    default:
	break;
    }
    return strm;
}

/* Be sure to check that each enum is not set to
   default before streaming it out. Defaults seem
   to not be in the XML file.
*/
QTextStream &operator<<( QTextStream &strm, const EPair &prop )
{
    strm << prop.name << prop.value << "\"";
    return strm;
}

QTextStream &operator<<( QTextStream &strm, const LPair &prop )
{
    strm << prop.name << prop.value << "\"";
    return strm;
}

QTextStream &operator<<( QTextStream &strm, const SPair &prop )
{
    if ( !prop.value.isEmpty() )
	strm << prop.name << QString(prop.value).remove("\"") << "\"";
    return strm;
}

QTextStream &operator<<( QTextStream &strm, const XPair &prop )
{
    if ( !prop.value.isEmpty() ) {
        QString outText = prop.value.join(prop.sep);
        strm << prop.name << outText.replace('\"', "&quot;") << "\"";
    }
    return strm;
}

// VCCLCompilerTool -------------------------------------------------
VCCLCompilerTool::VCCLCompilerTool()
    :	AssemblerOutput( asmListingNone ),
	BasicRuntimeChecks( runtimeBasicCheckNone ),
	BrowseInformation( brInfoNone ),
	BufferSecurityCheck( _False ),
	CallingConvention( callConventionDefault ),
	CompileAs( compileAsDefault ),
	CompileAsManaged( managedDefault ),
	CompileOnly( unset ),
	DebugInformationFormat( debugDisabled ),
	DefaultCharIsUnsigned( unset ),
	Detect64BitPortabilityProblems( unset ),
	DisableLanguageExtensions( unset ),
	EnableFiberSafeOptimizations( unset ),
	EnableFunctionLevelLinking( unset ),
	EnableIntrinsicFunctions( unset ),
	ExceptionHandling( _False ),
	ExpandAttributedSource( unset ),
	FavorSizeOrSpeed( favorNone ),
	ForceConformanceInForLoopScope( unset ),
	GeneratePreprocessedFile( preprocessNo ),
	GlobalOptimizations( unset ),
	IgnoreStandardIncludePath( unset ),
	ImproveFloatingPointConsistency( unset ),
	InlineFunctionExpansion( expandDefault ),
	KeepComments( unset ),
	MinimalRebuild( unset ),
	OmitFramePointers( unset ),
	OpenMP( unset ),
	Optimization( optimizeCustom ),
	OptimizeForProcessor( procOptimizeBlended ),
	OptimizeForWindowsApplication( unset ),
	ProgramDataBaseFileName( "" ),
	RuntimeLibrary( rtSingleThreaded ),
	RuntimeTypeInfo( unset ),
	ShowIncludes( unset ),
	SmallerTypeCheck( unset ),
	StringPooling( unset ),
	StructMemberAlignment( alignNotSet ),
	SuppressStartupBanner( unset ),
	TreatWChar_tAsBuiltInType( unset ),
	TurnOffAssemblyGeneration( unset ),
	UndefineAllPreprocessorDefinitions( unset ),
	UsePrecompiledHeader( pchNone ),
	WarnAsError( unset ),
	WarningLevel( warningLevel_0 ),
	WholeProgramOptimization( unset ),
	config( 0 )
{
}

/*
 * Some values for the attribute UsePrecompiledHeader have changed from VS 2003 to VS 2005,
 * see the following chart, so we need a function that transforms those values if we are
 * using NET2005:
 *
 * Meaning                      2003    2005
 * -----------------------------------------
 * Don't use PCH                0       0
 * Create PCH (/Yc)             1       1
 * Automatically generate (/YX) 2       (seems that it was removed)
 * Use specific PCH (/Yu)       3       2
 *
 */
inline pchOption xformUsePrecompiledHeaderForNET2005(pchOption whatPch)
{
    DotNET compilerVersion = which_dotnet_version();

    if (compilerVersion == NET2005) {
        if (whatPch ==  pchGenerateAuto) whatPch = pchNone;
        if (whatPch ==  pchUseUsingSpecific) whatPch = pchGenerateAuto;
    }
    return whatPch;
}

QTextStream &operator<<( QTextStream &strm, const VCCLCompilerTool &tool )
{
    strm << _begTool3;
    strm << _VCCLCompilerToolName;
    strm << XPair( _AdditionalIncludeDirectories, tool.AdditionalIncludeDirectories );
    strm << XPair( _AdditionalOptions, tool.AdditionalOptions, " " );
    strm << XPair( _AdditionalUsingDirectories, tool.AdditionalUsingDirectories );
    strm << SPair( _AssemblerListingLocation, tool.AssemblerListingLocation );
    if ( tool.AssemblerOutput != asmListingNone )	    strm << EPair( _AssemblerOutput, tool.AssemblerOutput );
    if ( tool.BasicRuntimeChecks != runtimeBasicCheckNone ) strm << EPair( _BasicRuntimeChecks, tool.BasicRuntimeChecks );
    if ( tool.BrowseInformation != brInfoNone )		    strm << EPair( _BrowseInformation, tool.BrowseInformation );
    strm << SPair( _BrowseInformationFile, tool.BrowseInformationFile );
    strm << TPair( _BufferSecurityCheck, tool.BufferSecurityCheck );
    if ( tool.CallingConvention != callConventionDefault )  strm << EPair( _CallingConvention, tool.CallingConvention );
    if ( tool.CompileAs != compileAsDefault )		    strm << EPair( _CompileAs, tool.CompileAs );
    if ( tool.CompileAsManaged != managedDefault )	    strm << EPair( _CompileAsManaged, tool.CompileAsManaged );
    strm << TPair( _CompileOnly, tool.CompileOnly );
    if ( tool.DebugInformationFormat != debugUnknown )	    strm << EPair( _DebugInformationFormat, tool.DebugInformationFormat );
    strm << TPair( _DefaultCharIsUnsigned, tool.DefaultCharIsUnsigned );
    strm << TPair( _Detect64BitPortabilityProblems, tool.Detect64BitPortabilityProblems );
    strm << TPair( _DisableLanguageExtensions, tool.DisableLanguageExtensions );
    strm << XPair( _DisableSpecificWarnings, tool.DisableSpecificWarnings );
    strm << TPair( _EnableFiberSafeOptimizations, tool.EnableFiberSafeOptimizations );
    strm << TPair( _EnableFunctionLevelLinking, tool.EnableFunctionLevelLinking );
    strm << TPair( _EnableIntrinsicFunctions, tool.EnableIntrinsicFunctions );
    strm << TPair( _ExceptionHandling, tool.ExceptionHandling );
    strm << TPair( _ExpandAttributedSource, tool.ExpandAttributedSource );
    if ( tool.FavorSizeOrSpeed != favorNone )		    strm << EPair( _FavorSizeOrSpeed, tool.FavorSizeOrSpeed );
    strm << TPair( _ForceConformanceInForLoopScope, tool.ForceConformanceInForLoopScope );
    strm << XPair( _ForcedIncludeFiles, tool.ForcedIncludeFiles );
    strm << XPair( _ForcedUsingFiles, tool.ForcedUsingFiles );
    if ( tool.GeneratePreprocessedFile != preprocessUnknown)strm << EPair( _GeneratePreprocessedFile, tool.GeneratePreprocessedFile );
    strm << TPair( _GlobalOptimizations, tool.GlobalOptimizations );
    strm << TPair( _IgnoreStandardIncludePath, tool.IgnoreStandardIncludePath );
    strm << TPair( _ImproveFloatingPointConsistency, tool.ImproveFloatingPointConsistency );
    if ( tool.InlineFunctionExpansion != expandDefault ) strm << EPair( _InlineFunctionExpansion, tool.InlineFunctionExpansion );
    strm << TPair( _KeepComments, tool.KeepComments );
    strm << TPair( _MinimalRebuild, tool.MinimalRebuild );
    strm << SPair( _ObjectFile, tool.ObjectFile );
    strm << TPair( _OmitFramePointers, tool.OmitFramePointers );
    if ( tool.Optimization != optimizeDefault ) strm << EPair( _Optimization, tool.Optimization );
    if ( tool.OptimizeForProcessor != procOptimizeBlended ) strm << EPair( _OptimizeForProcessor, tool.OptimizeForProcessor );
    strm << TPair( _OptimizeForWindowsApplication, tool.OptimizeForWindowsApplication );
    strm << SPair( _OutputFile, tool.OutputFile );
    strm << SPair( _PrecompiledHeaderFile, tool.PrecompiledHeaderFile );
    strm << SPair( _PrecompiledHeaderThrough, tool.PrecompiledHeaderThrough );
    strm << XPair( _PreprocessorDefinitions, tool.PreprocessorDefinitions );
    if ( !tool.ProgramDataBaseFileName.isNull() ) strm << _ProgramDataBaseFileName << tool.ProgramDataBaseFileName.latin1() << "\"";
    if ( tool.RuntimeLibrary != rtUnknown ) strm << EPair( _RuntimeLibrary, tool.RuntimeLibrary );
    strm << TPair( _RuntimeTypeInfo, tool.RuntimeTypeInfo );
    strm << TPair( _OpenMP, tool.OpenMP );
    strm << TPair( _ShowIncludes, tool.ShowIncludes );
    strm << TPair( _SmallerTypeCheck, tool.SmallerTypeCheck );
    strm << TPair( _StringPooling, tool.StringPooling );
    if ( tool.StructMemberAlignment != alignNotSet )	    strm << EPair( _StructMemberAlignment, tool.StructMemberAlignment );
    strm << TPair( _SuppressStartupBanner, tool.SuppressStartupBanner );
    strm << TPair( _TreatWChar_tAsBuiltInType, tool.TreatWChar_tAsBuiltInType );
    strm << TPair( _TurnOffAssemblyGeneration, tool.TurnOffAssemblyGeneration );
    strm << TPair( _UndefineAllPreprocessorDefinitions, tool.UndefineAllPreprocessorDefinitions );
    strm << XPair( _UndefinePreprocessorDefinitions, tool.UndefinePreprocessorDefinitions );

    if ( !tool.PrecompiledHeaderFile.isEmpty() || !tool.PrecompiledHeaderThrough.isEmpty() )
        strm << EPair( _UsePrecompiledHeader, xformUsePrecompiledHeaderForNET2005(tool.UsePrecompiledHeader) );

    strm << TPair( _WarnAsError, tool.WarnAsError );
    if ( tool.WarningLevel != warningLevelUnknown ) strm << EPair( _WarningLevel, tool.WarningLevel );
    strm << TPair( _WholeProgramOptimization, tool.WholeProgramOptimization );
    strm << "/>";
return strm;
}

bool VCCLCompilerTool::parseOption( const char* option )
{
    // skip index 0 ('/' or '-')
    char first  = option[1];
    char second = option[2];
    char third  = option[3];
    char fourth = option[4];
    bool found = TRUE;

    switch ( first ) {
    case '?':
    case 'h':
	qWarning( "Generator: Option '/?', '/help': MSVC.NET projects do not support outputting help info" );
	found = FALSE;
	break;
    case '@':
	qWarning( "Generator: Option '/@': MSVC.NET projects do not support the use of a response file" );
	found = FALSE;
	break;
    case 'l':
	qWarning( "Generator: Option '/link': qmake generator does not support passing link options through the compiler tool" );
	found = FALSE;
	break;
    case 'A':
	if ( second != 'I' ) {
	    found = FALSE; break;
	}
	AdditionalUsingDirectories += option+3;
	break;
    case 'C':
	KeepComments = _True;
	break;
    case 'D':
	PreprocessorDefinitions += option+2;
	break;
    case 'E':
	if ( second == 'H' ) {
	    if ( third == 'a'
	    	|| (third == 'c' && fourth != 's')
	    	|| (third == 's' && fourth != 'c') ) {
		// ExceptionHandling must be false, or it will override
		// with an /EHsc option
		ExceptionHandling = _False;
		AdditionalOptions += option;
		break;
	    } else if ( (third == 'c' && fourth == 's')
		     || (third == 's' && fourth == 'c') ) {
		ExceptionHandling = _True;
		AdditionalOptions += option;
		break;
	    }
	    found = FALSE; break;
	}
	GeneratePreprocessedFile = preprocessYes;
	break;
    case 'F':
	if ( second <= '9' && second >= '0' ) {
	    AdditionalOptions += option;
	    break;
	} else {
	    switch ( second ) {
	    case 'A':
		if ( third == 'c' ) {
		    AssemblerOutput = asmListingAsmMachine;
		    if ( fourth == 's' )
			AssemblerOutput = asmListingAsmMachineSrc;
		} else if ( third == 's' ) {
		    AssemblerOutput = asmListingAsmSrc;
		} else {
		    AssemblerOutput = asmListingAssemblyOnly;
		}
		break;
	    case 'a':
		AssemblerListingLocation = option+3;
		break;
	    case 'I':
		ForcedIncludeFiles += option+3;
		break;
	    case 'R':
		BrowseInformation = brAllInfo;
		BrowseInformationFile = option+3;
		break;
	    case 'r':
		BrowseInformation = brNoLocalSymbols;
		BrowseInformationFile = option+3;
		break;
	    case 'U':
		ForcedUsingFiles += option+3;
		break;
	    case 'd':
		ProgramDataBaseFileName = option+3;
		break;
	    case 'e':
		OutputFile = option+3;
		break;
	    case 'm':
		AdditionalOptions += option;
		break;
	    case 'o':
		ObjectFile = option+3;
		break;
	    case 'p':
		PrecompiledHeaderFile = option+3;
		break;
	    case 'x':
		ExpandAttributedSource = _True;
		break;
	    default:
		found = FALSE; break;
	    }
	}
	break;
    case 'G':
	switch ( second ) {
	case '3':
	case '4':
	    qWarning( "Option '/G3' and '/G4' were phased out in Visual C++ 5.0" );
	    found = FALSE; break;
        case '5':
	    OptimizeForProcessor = procOptimizePentium;
	    break;
	case '6':
	case 'B':
	    OptimizeForProcessor = procOptimizePentiumProAndAbove;
	    break;
	case 'A':
	    OptimizeForWindowsApplication = _True;
	    break;
	case 'F':
	    StringPooling = _True;
	    break;
	case 'H':
	    AdditionalOptions += option;
	    break;
	case 'L':
	    WholeProgramOptimization = _True;
	    if ( third == '-' )
		WholeProgramOptimization = _False;
	    break;
	case 'R':
	    RuntimeTypeInfo = _True;
	    if ( third == '-' )
		RuntimeTypeInfo = _False;
	    break;
        case 'S':
	    BufferSecurityCheck = _True;
	    break;
	case 'T':
	    EnableFiberSafeOptimizations = _True;
	    break;
	case 'X':
	    // ExceptionHandling == true will override with
	    // an /EHsc option, which is correct with /GX
	    ExceptionHandling = _True; // Fall-through
	case 'Z':
	case 'e':
	case 'h':
	    AdditionalOptions += option;
	    break;
	case 'd':
	    CallingConvention = callConventionCDecl;
	    break;
	case 'f':
	    StringPooling = _True;
	    AdditionalOptions += option;
	    break;
	case 'm':
	    MinimalRebuild = _True;
	    if ( third == '-' )
		MinimalRebuild = _False;
	    break;
	case 'r':
	    CallingConvention = callConventionFastCall;
	    break;
	case 's':
	    // Warning: following [num] is not used,
	    // were should we put it?
	    BufferSecurityCheck = _True;
	    break;
	case 'y':
	    EnableFunctionLevelLinking = _True;
	    break;
	case 'z':
	    CallingConvention = callConventionStdCall;
	    break;
	default:
	    found = FALSE; break;
	}
	break;
    case 'H':
	AdditionalOptions += option;
	break;
    case 'I':
	AdditionalIncludeDirectories += option+2;
	break;
    case 'L':
	if ( second == 'D' ) {
	    AdditionalOptions += option;
	    break;
	}
	found = FALSE; break;
    case 'M':
	if ( second == 'D' ) {
	    RuntimeLibrary = rtMultiThreadedDLL;
	    if ( third == 'd' )
		RuntimeLibrary = rtMultiThreadedDebugDLL;
	    break;
	} else if ( second == 'L' ) {
	    RuntimeLibrary = rtSingleThreaded;
	    if ( third == 'd' )
		RuntimeLibrary = rtSingleThreadedDebug;
	    break;
	} else if ( second == 'T' ) {
	    RuntimeLibrary = rtMultiThreaded;
	    if ( third == 'd' )
		RuntimeLibrary = rtMultiThreadedDebug;
	    break;
	}
	found = FALSE; break;
    case 'O':
	switch ( second ) {
	case '1':
	    Optimization = optimizeMinSpace;
	    break;
	case '2':
	    Optimization = optimizeMaxSpeed;
	    break;
	case 'a':
	    AdditionalOptions += option;
	    break;
	case 'b':
	    if ( third == '0' )
		InlineFunctionExpansion = expandDisable;
	    else if ( third == '1' )
		InlineFunctionExpansion = expandOnlyInline;
	    else if ( third == '2' )
		InlineFunctionExpansion = expandAnySuitable;
	    else
		found = FALSE;
	    break;
	case 'd':
	    Optimization = optimizeDisabled;
	    break;
	case 'g':
	    GlobalOptimizations = _True;
	    break;
	case 'i':
	    EnableIntrinsicFunctions = _True;
	    break;
	case 'p':
	    ImproveFloatingPointConsistency = _True;
	    if ( third == '-' )
		ImproveFloatingPointConsistency = _False;
	    break;
	case 's':
	    FavorSizeOrSpeed = favorSize;
	    break;
	case 't':
	    FavorSizeOrSpeed = favorSpeed;
	    break;
	case 'w':
	    AdditionalOptions += option;
	    break;
	case 'x':
	    Optimization = optimizeFull;
	    break;
	case 'y':
	    OmitFramePointers = _True;
	    if ( third == '-' )
		OmitFramePointers = _False;
	    break;
	default:
	    found = FALSE; break;
	}
	break;
    case 'P':
	GeneratePreprocessedFile = preprocessYes;
	break;
    case 'Q':
	if ( second == 'I' ) {
	    AdditionalOptions += option;
	    break;
	}
	found = FALSE; break;
    case 'R':
	if ( second == 'T' && third == 'C' ) {
	    if ( fourth == '1' )
		BasicRuntimeChecks = runtimeBasicCheckAll;
	    else if ( fourth == 'c' )
		SmallerTypeCheck = _True;
	    else if ( fourth == 's' )
		BasicRuntimeChecks = runtimeCheckStackFrame;
	    else if ( fourth == 'u' )
		BasicRuntimeChecks = runtimeCheckUninitVariables;
	    else
		found = FALSE; break;
	}
	break;
    case 'T':
	if ( second == 'C' ) {
	    CompileAs = compileAsC;
	} else if ( second == 'P' ) {
	    CompileAs = compileAsCPlusPlus;
	} else {
	    qWarning( "Generator: Options '/Tp<filename>' and '/Tc<filename>' are not supported by qmake" );
	    found = FALSE; break;
	}
	break;
    case 'U':
	UndefinePreprocessorDefinitions += option+2;
	break;
    case 'V':
	AdditionalOptions += option;
	break;
    case 'W':
	switch ( second ) {
	case 'a':
	case '4':
	    WarningLevel = warningLevel_4;
	    break;
	case '3':
	    WarningLevel = warningLevel_3;
	    break;
	case '2':
	    WarningLevel = warningLevel_2;
	    break;
	case '1':
	    WarningLevel = warningLevel_1;
	    break;
	case '0':
	    WarningLevel = warningLevel_0;
	    break;
	case 'L':
	    AdditionalOptions += option;
	    break;
	case 'X':
	    WarnAsError = _True;
	    break;
	case 'p':
	    if ( third == '6' && fourth == '4' ) {
		Detect64BitPortabilityProblems = _True;
		break;
	    }
	    // Fallthrough
	default:
	    found = FALSE; break;
	}
	break;
    case 'X':
	IgnoreStandardIncludePath = _True;
	break;
    case 'Y':
	switch ( second ) {
	case '\0':
	case '-':
	    AdditionalOptions += option;
	    break;
	case 'X':
	    UsePrecompiledHeader = pchGenerateAuto;
	    PrecompiledHeaderFile = option+3;
	    break;
	case 'c':
	    UsePrecompiledHeader = pchCreateUsingSpecific;
	    PrecompiledHeaderFile = option+3;
	    break;
	case 'd':
	case 'l':
	    AdditionalOptions =+ option;
	    break;
	case 'u':
	    UsePrecompiledHeader = pchUseUsingSpecific;
	    PrecompiledHeaderFile = option+3;
	    break;
	default:
	    found = FALSE; break;
	}
	break;
    case 'Z':
	switch ( second ) {
	case '7':
	    DebugInformationFormat = debugOldStyleInfo;
	    break;
	case 'I':
	    DebugInformationFormat = debugEditAndContinue;
	    break;
	case 'd':
	    DebugInformationFormat = debugLineInfoOnly;
	    break;
	case 'i':
	    DebugInformationFormat = debugEnabled;
	    break;
	case 'l':
	    DebugInformationFormat = debugEditAndContinue;
	    break;
	case 'a':
	    DisableLanguageExtensions = _True;
	    break;
	case 'e':
	    DisableLanguageExtensions = _False;
	    break;
	case 'c':
	    if ( third == ':' ) {
		if ( fourth == 'f' )
		    ForceConformanceInForLoopScope = _True;
		else if ( fourth == 'w' )
		    TreatWChar_tAsBuiltInType = _True;
		else
		    found = FALSE;
	    } else {
		found = FALSE; break;
	    }
	    break;
	case 'g':
	case 'm':
	case 's':
	    AdditionalOptions += option;
	    break;
	case 'p':
	    switch ( third )
	    {
	    case '\0':
	    case '1':
		StructMemberAlignment = alignSingleByte;
		if ( fourth == '6' )
		    StructMemberAlignment = alignSixteenBytes;
		break;
	    case '2':
		StructMemberAlignment = alignTwoBytes;
		break;
	    case '4':
		StructMemberAlignment = alignFourBytes;
		break;
	    case '8':
		StructMemberAlignment = alignEightBytes;
		break;
	    default:
		found = FALSE; break;
	    }
	    break;
	default:
	    found = FALSE; break;
	}
	break;
    case 'c':
	if ( second == '\0' ) {
	    CompileOnly = _True;
	} else if ( second == 'l' ) {
	    if ( *(option+5) == 'n' ) {
		CompileAsManaged = managedAssembly;
		TurnOffAssemblyGeneration = _True;
	    } else {
		CompileAsManaged = managedAssembly;
	    }
	} else {
	    found = FALSE; break;
	}
	break;
    case 'd':
	if ( second != 'r' ) {
	    found = FALSE; break;
	}
        CompileAsManaged = managedAssembly;
	break;
    case 'n':
	if ( second == 'o' && third == 'B' && fourth == 'o' ) {
	    AdditionalOptions += "/noBool";
	    break;
	}
	if ( second == 'o' && third == 'l' && fourth == 'o' ) {
	    SuppressStartupBanner = _True;
	    break;
	}
	found = FALSE; break;
    case 'o':
        if ( second == 'p' && third == 'e' && fourth == 'n' ) {
            OpenMP = _True;
            break;
        }
	found = FALSE; break;
    case 's':
	if ( second == 'h' && third == 'o' && fourth == 'w' ) {
	    ShowIncludes = _True;
	    break;
	}
	found = FALSE; break;
    case 'u':
	UndefineAllPreprocessorDefinitions = _True;
	break;
    case 'v':
	if ( second == 'd' || second == 'm' ) {
	    AdditionalOptions += option;
	    break;
	}
	found = FALSE; break;
    case 'w':
	switch ( second ) {
	case '\0':
	    WarningLevel = warningLevel_0;
	    break;
	case 'd':
	    DisableSpecificWarnings += option+3;
	    break;
	default:
	    AdditionalOptions += option;
	}
	break;
    default:
	found = FALSE; break;
    }
    if( !found )
	warn_msg( WarnLogic, "Could not parse Compiler option: %s", option );
    return TRUE;
}

// VCLinkerTool -----------------------------------------------------
VCLinkerTool::VCLinkerTool()
    :	EnableCOMDATFolding( optFoldingDefault ),
	GenerateDebugInformation( unset ),
	GenerateMapFile( unset ),
	HeapCommitSize( -1 ),
	HeapReserveSize( -1 ),
	IgnoreAllDefaultLibraries( unset ),
	IgnoreEmbeddedIDL( unset ),
	IgnoreImportLibrary( _True ),
	LargeAddressAware( addrAwareDefault ),
	LinkDLL( unset ),
	LinkIncremental( linkIncrementalDefault ),
	LinkTimeCodeGeneration( unset ),
	MapExports( unset ),
	MapLines( unset ),
	OptimizeForWindows98( optWin98Default ),
	OptimizeReferences( optReferencesDefault ),
	RegisterOutput( unset ),
	ResourceOnlyDLL( unset ),
	SetChecksum( unset ),
	ShowProgress( linkProgressNotSet ),
	StackCommitSize( -1 ),
	StackReserveSize( -1 ),
	SubSystem( subSystemNotSet ),
	SupportUnloadOfDelayLoadedDLL( unset ),
	SuppressStartupBanner( unset ),
	SwapRunFromCD( unset ),
	SwapRunFromNet( unset ),
	TargetMachine( machineNotSet ),
	TerminalServerAware( termSvrAwareDefault ),
	TurnOffAssemblyGeneration( unset ),
	TypeLibraryResourceID( 0 )
{
}

QTextStream &operator<<( QTextStream &strm, const VCLinkerTool &tool )
{
    strm << _begTool3;
    strm << _VCLinkerToolName;
    strm << XPair( _AdditionalDependencies4, tool.AdditionalDependencies, " " );
    strm << XPair( _AdditionalLibraryDirectories, tool.AdditionalLibraryDirectories );
    strm << XPair( _AdditionalOptions, tool.AdditionalOptions, " " );
    strm << XPair( _AddModuleNamesToAssembly, tool.AddModuleNamesToAssembly );
    strm << SPair( _BaseAddress, tool.BaseAddress );
    strm << XPair( _DelayLoadDLLs, tool.DelayLoadDLLs );
    if ( tool.EnableCOMDATFolding != optFoldingDefault )    strm << EPair( _EnableCOMDATFolding, tool.EnableCOMDATFolding );
    strm << SPair( _EntryPointSymbol, tool.EntryPointSymbol );
    strm << XPair( _ForceSymbolReferences, tool.ForceSymbolReferences );
    strm << SPair( _FunctionOrder, tool.FunctionOrder );
    strm << TPair( _GenerateDebugInformation, tool.GenerateDebugInformation );
    strm << TPair( _GenerateMapFile, tool.GenerateMapFile );
    if ( tool.HeapCommitSize != -1 )			    strm << LPair( _HeapCommitSize, tool.HeapCommitSize );
    if ( tool.HeapReserveSize != -1 )			    strm << LPair( _HeapReserveSize, tool.HeapReserveSize );
    strm << TPair( _IgnoreAllDefaultLibraries, tool.IgnoreAllDefaultLibraries );
    strm << XPair( _IgnoreDefaultLibraryNames, tool.IgnoreDefaultLibraryNames );
    strm << TPair( _IgnoreEmbeddedIDL, tool.IgnoreEmbeddedIDL );
    strm << TPair( _IgnoreImportLibrary, tool.IgnoreImportLibrary );
    strm << SPair( _ImportLibrary, tool.ImportLibrary );
    if ( tool.LargeAddressAware != addrAwareDefault )	    strm << EPair( _LargeAddressAware, tool.LargeAddressAware );
    strm << TPair( _LinkDLL, tool.LinkDLL );
    if ( tool.LinkIncremental != linkIncrementalDefault )   strm << EPair( _LinkIncremental, tool.LinkIncremental );
    strm << TPair( _LinkTimeCodeGeneration, tool.LinkTimeCodeGeneration );
    strm << SPair( _LinkToManagedResourceFile, tool.LinkToManagedResourceFile );
    strm << TPair( _MapExports, tool.MapExports );
    strm << SPair( _MapFileName, tool.MapFileName );
    strm << TPair( _MapLines, tool.MapLines );
    strm << SPair( _MergedIDLBaseFileName, tool.MergedIDLBaseFileName );
    strm << SPair( _MergeSections, tool.MergeSections );
    strm << SPair( _MidlCommandFile, tool.MidlCommandFile );
    strm << SPair( _ModuleDefinitionFile, tool.ModuleDefinitionFile );
    if ( tool.OptimizeForWindows98 != optWin98Default )	    strm << EPair( _OptimizeForWindows98, tool.OptimizeForWindows98 );
    if ( tool.OptimizeReferences != optReferencesDefault )  strm << EPair( _OptimizeReferences, tool.OptimizeReferences );
    strm << SPair( _OutputFile, tool.OutputFile );
    strm << _ProgramDatabaseFile << tool.ProgramDatabaseFile << "\"";
    strm << TPair( _RegisterOutput, tool.RegisterOutput );
    strm << TPair( _ResourceOnlyDLL, tool.ResourceOnlyDLL );
    strm << TPair( _SetChecksum, tool.SetChecksum );
    if ( tool.ShowProgress != linkProgressNotSet )	    strm << EPair( _ShowProgress, tool.ShowProgress );
    if ( tool.StackCommitSize != -1 )			    strm << LPair( _StackCommitSize, tool.StackCommitSize );
    if ( tool.StackReserveSize != -1 )			    strm << LPair( _StackReserveSize, tool.StackReserveSize );
    strm << SPair( _StripPrivateSymbols, tool.StripPrivateSymbols );
    strm << EPair( _SubSystem, tool.SubSystem );
    strm << TPair( _SupportUnloadOfDelayLoadedDLL, tool.SupportUnloadOfDelayLoadedDLL );
    strm << TPair( _SuppressStartupBanner, tool.SuppressStartupBanner );
    strm << TPair( _SwapRunFromCD, tool.SwapRunFromCD );
    strm << TPair( _SwapRunFromNet, tool.SwapRunFromNet );
    if ( tool.TargetMachine != machineNotSet )		    strm << EPair( _TargetMachine, tool.TargetMachine );
    if ( tool.TerminalServerAware != termSvrAwareDefault )  strm << EPair( _TerminalServerAware, tool.TerminalServerAware );
    strm << TPair( _TurnOffAssemblyGeneration, tool.TurnOffAssemblyGeneration );
    strm << SPair( _TypeLibraryFile, tool.TypeLibraryFile );
    if ( tool.TypeLibraryResourceID != rcUseDefault ) strm << LPair( _TypeLibraryResourceID, tool.TypeLibraryResourceID );
    strm << SPair( _Version4, tool.Version );
    strm << "/>";
    return strm;
}

// Hashing routine to do fast option lookups ----
// Slightly rewritten to stop on ':' ',' and '\0'
// Original routine in qtranslator.cpp ----------
static uint elfHash( const char* name )
{
    const uchar *k;
    uint h = 0;
    uint g;

    if ( name ) {
	k = (const uchar *) name;
	while ( (*k) &&
		(*k)!= ':' &&
		(*k)!=',' &&
		(*k)!=' ' ) {
	    h = ( h << 4 ) + *k++;
	    if ( (g = (h & 0xf0000000)) != 0 )
		h ^= g >> 24;
	    h &= ~g;
	}
    }
    if ( !h )
	h = 1;
    return h;
}

//#define USE_DISPLAY_HASH
#ifdef USE_DISPLAY_HASH
static void displayHash( const char* str )
{
    printf( "case 0x%07x: // %s\n    break;\n", elfHash(str), str );
}
#endif

bool VCLinkerTool::parseOption( const char* option )
{
#ifdef USE_DISPLAY_HASH
    // Main options
    displayHash( "/ALIGN" ); displayHash( "/ALLOWBIND" ); displayHash( "/ASSEMBLYMODULE" );
    displayHash( "/ASSEMBLYRESOURCE" ); displayHash( "/BASE" ); displayHash( "/DEBUG" );
    displayHash( "/DEF" ); displayHash( "/DEFAULTLIB" ); displayHash( "/DELAY" );
    displayHash( "/DELAYLOAD" ); displayHash( "/DLL" ); displayHash( "/DRIVER" );
    displayHash( "/ENTRY" ); displayHash( "/EXETYPE" ); displayHash( "/EXPORT" );
    displayHash( "/FIXED" ); displayHash( "/FORCE" ); displayHash( "/HEAP" );
    displayHash( "/IDLOUT" ); displayHash( "/IGNOREIDL" ); displayHash( "/IMPLIB" );
    displayHash( "/INCLUDE" ); displayHash( "/INCREMENTAL" ); displayHash( "/LARGEADDRESSAWARE" );
    displayHash( "/LIBPATH" ); displayHash( "/LTCG" ); displayHash( "/MACHINE" );
    displayHash( "/MAP" ); displayHash( "/MAPINFO" ); displayHash( "/MERGE" );
    displayHash( "/MIDL" ); displayHash( "/NOASSEMBLY" ); displayHash( "/NODEFAULTLIB" );
    displayHash( "/NOENTRY" ); displayHash( "/NOLOGO" ); displayHash( "/OPT" );
    displayHash( "/ORDER" ); displayHash( "/OUT" ); displayHash( "/PDB" );
    displayHash( "/PDBSTRIPPED" ); displayHash( "/RELEASE" ); displayHash( "/SECTION" );
    displayHash( "/STACK" ); displayHash( "/STUB" ); displayHash( "/SUBSYSTEM" );
    displayHash( "/SWAPRUN" ); displayHash( "/TLBID" ); displayHash( "/TLBOUT" );
    displayHash( "/TSAWARE" ); displayHash( "/VERBOSE" ); displayHash( "/VERSION" );
    displayHash( "/VXD" ); displayHash( "/WS " );
#endif
#ifdef USE_DISPLAY_HASH
    // Sub options
    displayHash( "UNLOAD" ); displayHash( "NOBIND" ); displayHash( "no" ); displayHash( "NOSTATUS" ); displayHash( "STATUS" );
    displayHash( "AM33" ); displayHash( "ARM" ); displayHash( "CEE" ); displayHash( "IA64" ); displayHash( "X86" ); displayHash( "M32R" );
    displayHash( "MIPS" ); displayHash( "MIPS16" ); displayHash( "MIPSFPU" ); displayHash( "MIPSFPU16" ); displayHash( "MIPSR41XX" ); displayHash( "PPC" );
    displayHash( "SH3" ); displayHash( "SH4" ); displayHash( "SH5" ); displayHash( "THUMB" ); displayHash( "TRICORE" ); displayHash( "EXPORTS" );
    displayHash( "LINES" ); displayHash( "REF" ); displayHash( "NOREF" ); displayHash( "ICF" ); displayHash( "WIN98" ); displayHash( "NOWIN98" );
    displayHash( "CONSOLE" ); displayHash( "EFI_APPLICATION" ); displayHash( "EFI_BOOT_SERVICE_DRIVER" ); displayHash( "EFI_ROM" ); displayHash( "EFI_RUNTIME_DRIVER" ); displayHash( "NATIVE" );
    displayHash( "POSIX" ); displayHash( "WINDOWS" ); displayHash( "WINDOWSCE" ); displayHash( "NET" ); displayHash( "CD" ); displayHash( "NO" );
#endif
    bool found = TRUE;
    switch ( elfHash(option) ) {
    case 0x3360dbe: // /ALIGN[:number]
    case 0x1485c34: // /ALLOWBIND[:NO]
    case 0x6b21972: // /DEFAULTLIB:library
    case 0x396ea92: // /DRIVER[:UPONLY | :WDM]
    case 0xaca9d75: // /EXETYPE[:DYNAMIC | :DEV386]
    case 0x3ad5444: // /EXPORT:entryname[,@ordinal[,NONAME]][,DATA]
    case 0x33aec94: // /FIXED[:NO]
    case 0x33b4675: // /FORCE:[MULTIPLE|UNRESOLVED]
    case 0x7988f7e: // /SECTION:name,[E][R][W][S][D][K][L][P][X][,ALIGN=#]
    case 0x0348992: // /STUB:filename
    case 0x0034bc4: // /VXD
    case 0x0034c50: // /WS
	AdditionalOptions += option;
	break;
    case 0x679c075: // /ASSEMBLYMODULE:filename
	AddModuleNamesToAssembly += option+15;
	break;
    case 0x062d065: // /ASSEMBLYRESOURCE:filename
	LinkToManagedResourceFile = option+18;
	break;
    case 0x0336675: // /BASE:{address | @filename,key}
	// Do we need to do a manual lookup when '@filename,key'?
	// Seems BaseAddress only can contain the location...
	// We don't use it in Qt, so keep it simple for now
	BaseAddress = option+6;
	break;
    case 0x3389797: // /DEBUG
	GenerateDebugInformation = _True;
	break;
    case 0x0033896: // /DEF:filename
	ModuleDefinitionFile = option+5;
	break;
    case 0x338a069: // /DELAY:{UNLOAD | NOBIND}
	// MS documentation does not specify what to do with
	// this option, so we'll put it in AdditionalOptions
	AdditionalOptions += option;
	break;
    case 0x06f4bf4: // /DELAYLOAD:dllname
	DelayLoadDLLs += option+11;
	break;
    // case 0x003390c: // /DLL
    // This option is not used for vcproj files
    //	break;
    case 0x33a3979: // /ENTRY:function
	EntryPointSymbol = option+7;
	break;
    case 0x033c960: // /HEAP:reserve[,commit]
	{
	    QStringList both = QStringList::split( ",", option+6 );
	    HeapReserveSize = both[0].toLong();
	    if ( both.count() == 2 )
		HeapCommitSize = both[1].toLong();
	}
	break;
    case 0x3d91494: // /IDLOUT:[path\]filename
	MergedIDLBaseFileName = option+8;
	break;
    case 0x345a04c: // /IGNOREIDL
	IgnoreEmbeddedIDL = _True;
	break;
    case 0x3e250e2: // /IMPLIB:filename
	ImportLibrary = option+8;
	break;
    case 0xe281ab5: // /INCLUDE:symbol
	ForceSymbolReferences += option+9;
	break;
    case 0xb28103c: // /INCREMENTAL[:no]
	if ( *(option+12) == ':' &&
	     *(option+13) == 'n' )
	    LinkIncremental = linkIncrementalNo;
	else
	    LinkIncremental = linkIncrementalYes;
	break;
    case 0x26e4675: // /LARGEADDRESSAWARE[:no]
	if ( *(option+18) == ':' &&
	     *(option+19) == 'n' )
	    LargeAddressAware = addrAwareNoLarge;
	else
	    LargeAddressAware = addrAwareLarge;
	break;
    case 0x0d745c8: // /LIBPATH:dir
	AdditionalLibraryDirectories += option+9;
	break;
    case 0x0341877: // /LTCG[:NOSTATUS|:STATUS]
	config->WholeProgramOptimization = _True;
	LinkTimeCodeGeneration = _True;
	if ( *(option+5) == ':' &&
	     *(option+6) == 'S' )
	     ShowProgress = linkProgressAll;
	break;
    case 0x157cf65: // /MACHINE:{AM33|ARM|CEE|IA64|X86|M32R|MIPS|MIPS16|MIPSFPU|MIPSFPU16|MIPSR41XX|PPC|SH3|SH4|SH5|THUMB|TRICORE}
	switch ( elfHash(option+9) ) {
	    // Very limited documentation on all options but X86,
	    // so we put the others in AdditionalOptions...
	case 0x0046063: // AM33
	case 0x000466d: // ARM
	case 0x0004795: // CEE
	case 0x004d494: // IA64
	case 0x0050672: // M32R
	case 0x0051e53: // MIPS
	case 0x51e5646: // MIPS16
	case 0x1e57b05: // MIPSFPU
	case 0x57b09a6: // MIPSFPU16
	case 0x5852738: // MIPSR41XX
	case 0x0005543: // PPC
	case 0x00057b3: // SH3
	case 0x00057b4: // SH4
	case 0x00057b5: // SH5
	case 0x058da12: // THUMB
	case 0x96d8435: // TRICORE
	    AdditionalOptions += option;
	    break;
	case 0x0005bb6: // X86
	    TargetMachine = machineX86;
	    break;
	default:
	    found = FALSE;
	}
	break;
    case 0x0034160: // /MAP[:filename]
	GenerateMapFile = _True;
	MapFileName = option+5;
	break;
    case 0x164e1ef: // /MAPINFO:{EXPORTS|LINES}
	if ( *(option+9) == 'E' )
	    MapExports = _True;
	else if ( *(option+9) == 'L' )
	    MapLines = _True;
	break;
    case 0x341a6b5: // /MERGE:from=to
	MergeSections = option+7;
	break;
    case 0x0341d8c: // /MIDL:@file
	MidlCommandFile = option+7;
	break;
    case 0x84e2679: // /NOASSEMBLY
	TurnOffAssemblyGeneration = _True;
	break;
    case 0x2b21942: // /NODEFAULTLIB[:library]
	if ( *(option+13) == '\0' )
	    IgnoreAllDefaultLibraries = _True;
	else
	    IgnoreDefaultLibraryNames += option+14;
	break;
    case 0x33a3a39: // /NOENTRY
	ResourceOnlyDLL = _True;
	break;
    case 0x434138f: // /NOLOGO
	SuppressStartupBanner = _True;
	break;
    case 0x0034454: // /OPT:{REF | NOREF | ICF[=iterations] | NOICF | WIN98 | NOWIN98}
	{
	    char third = *(option+7);
	    switch ( third ) {
	    case 'F': // REF
		if ( *(option+5) == 'R' ) {
		    OptimizeReferences = optReferences;
		} else { // ICF[=iterations]
		    EnableCOMDATFolding = optFolding;
		    // [=iterations] case is not documented
		}
		break;
	    case 'R': // NOREF
		OptimizeReferences = optNoReferences;
		break;
	    case 'I': // NOICF
		EnableCOMDATFolding = optNoFolding;
		break;
	    case 'N': // WIN98
		OptimizeForWindows98 = optWin98Yes;
		break;
	    case 'W': // NOWIN98
		OptimizeForWindows98 = optWin98No;
		break;
	    default:
		found = FALSE;
	    }
	}
	break;
    case 0x34468a2: // /ORDER:@filename
	FunctionOrder = option+8;
	break;
    case 0x00344a4: // /OUT:filename
	OutputFile = option+5;
	break;
    case 0x0034482: // /PDB:filename
	ProgramDatabaseFile = option+5;
	break;
    case 0xa2ad314: // /PDBSTRIPPED:pdb_file_name
	StripPrivateSymbols = option+13;
	break;
    case 0x6a09535: // /RELEASE
	SetChecksum = _True;
	break;
    case 0x348857b: // /STACK:reserve[,commit]
	{
	    QStringList both = QStringList::split( ",", option+7 );
	    StackReserveSize = both[0].toLong();
	    if ( both.count() == 2 )
		StackCommitSize = both[1].toLong();
	}
	break;
    case 0x78dc00d: // /SUBSYSTEM:{CONSOLE|EFI_APPLICATION|EFI_BOOT_SERVICE_DRIVER|EFI_ROM|EFI_RUNTIME_DRIVER|NATIVE|POSIX|WINDOWS|WINDOWSCE}[,major[.minor]]
	{
	    // Split up in subsystem, and version number
	    QStringList both = QStringList::split( ",", option+11 );
	    switch ( elfHash(both[0].latin1()) ) {
	    case 0x8438445: // CONSOLE
		SubSystem = subSystemConsole;
		break;
	    case 0xbe29493: // WINDOWS
		SubSystem = subSystemWindows;
		break;
	    // The following are undocumented, so add them to AdditionalOptions
	    case 0x240949e: // EFI_APPLICATION
	    case 0xe617652: // EFI_BOOT_SERVICE_DRIVER
	    case 0x9af477d: // EFI_ROM
	    case 0xd34df42: // EFI_RUNTIME_DRIVER
	    case 0x5268ea5: // NATIVE
	    case 0x05547e8: // POSIX
	    case 0x2949c95: // WINDOWSCE
		AdditionalOptions += option;
		break;
	    default:
		found = FALSE;
	    }
	}
	break;
    case 0x8b654de: // /SWAPRUN:{NET | CD}
	if ( *(option+9) == 'N' )
	    SwapRunFromNet = _True;
	else if ( *(option+9) == 'C' )
	    SwapRunFromCD = _True;
	else
	    found = FALSE;
	break;
    case 0x34906d4: // /TLBID:id
	TypeLibraryResourceID = QString( option+7 ).toLong();
	break;
    case 0x4907494: // /TLBOUT:[path\]filename
	TypeLibraryFile = option+8;
	break;
    case 0x976b525: // /TSAWARE[:NO]
	if ( *(option+8) == ':' )
	    TerminalServerAware = termSvrAwareNo;
	else
	    TerminalServerAware = termSvrAwareYes;
	break;
    case 0xaa67735: // /VERBOSE[:lib]
	if ( *(option+9) == ':' ) {
	    ShowProgress = linkProgressLibs;
	    AdditionalOptions += option;
	} else {
	    ShowProgress = linkProgressAll;
	}
	break;
    case 0xaa77f7e: // /VERSION:major[.minor]
	Version = option+9;
	break;
    default:
	found = FALSE;
    }
    if( !found )
	warn_msg( WarnLogic, "Could not parse Linker options: %s", option );
    return found;
}

// VCMIDLTool -------------------------------------------------------
VCMIDLTool::VCMIDLTool()
    :	DefaultCharType( midlCharUnsigned ),
	EnableErrorChecks( midlDisableAll ),
	ErrorCheckAllocations( unset ),
	ErrorCheckBounds( unset ),
	ErrorCheckEnumRange( unset ),
	ErrorCheckRefPointers( unset ),
	ErrorCheckStubData( unset ),
	GenerateStublessProxies( unset ),
	GenerateTypeLibrary( unset ),
	IgnoreStandardIncludePath( unset ),
	MkTypLibCompatible( unset ),
	StructMemberAlignment( midlAlignNotSet ),
	SuppressStartupBanner( unset ),
	TargetEnvironment( midlTargetNotSet ),
	ValidateParameters( unset ),
	WarnAsError( unset ),
	WarningLevel( midlWarningLevel_0 )
{
}

QTextStream &operator<<( QTextStream &strm, const VCMIDLTool &tool )
{
    strm << _begTool3;
    strm << _VCMIDLToolName;
    strm << XPair( _AdditionalIncludeDirectories, tool.AdditionalIncludeDirectories );
    strm << XPair( _AdditionalOptions, tool.AdditionalOptions, " " );
    strm << XPair( _CPreprocessOptions, tool.CPreprocessOptions );
    strm << EPair( _DefaultCharType, tool.DefaultCharType );
    strm << SPair( _DLLDataFileName, tool.DLLDataFileName );
    strm << EPair( _EnableErrorChecks, tool.EnableErrorChecks );
    strm << TPair( _ErrorCheckAllocations, tool.ErrorCheckAllocations );
    strm << TPair( _ErrorCheckBounds, tool.ErrorCheckBounds );
    strm << TPair( _ErrorCheckEnumRange, tool.ErrorCheckEnumRange );
    strm << TPair( _ErrorCheckRefPointers, tool.ErrorCheckRefPointers );
    strm << TPair( _ErrorCheckStubData, tool.ErrorCheckStubData );
    strm << XPair( _FullIncludePath, tool.FullIncludePath );
    strm << TPair( _GenerateStublessProxies, tool.GenerateStublessProxies );
    strm << TPair( _GenerateTypeLibrary, tool.GenerateTypeLibrary );
    strm << SPair( _HeaderFileName, tool.HeaderFileName );
    strm << TPair( _IgnoreStandardIncludePath, tool.IgnoreStandardIncludePath );
    strm << SPair( _InterfaceIdentifierFileName, tool.InterfaceIdentifierFileName );
    strm << TPair( _MkTypLibCompatible, tool.MkTypLibCompatible );
    strm << SPair( _OutputDirectory4, tool.OutputDirectory );
    strm << XPair( _PreprocessorDefinitions, tool.PreprocessorDefinitions );
    strm << SPair( _ProxyFileName, tool.ProxyFileName );
    strm << SPair( _RedirectOutputAndErrors, tool.RedirectOutputAndErrors );
    if ( tool.StructMemberAlignment != midlAlignNotSet)	    strm << EPair( _StructMemberAlignment, tool.StructMemberAlignment );
    strm << TPair( _SuppressStartupBanner, tool.SuppressStartupBanner );
    if ( tool.TargetEnvironment != midlTargetNotSet )	    strm << EPair( _TargetEnvironment, tool.TargetEnvironment );
    strm << SPair( _TypeLibraryName, tool.TypeLibraryName );
    strm << XPair( _UndefinePreprocessorDefinitions, tool.UndefinePreprocessorDefinitions );
    strm << TPair( _ValidateParameters, tool.ValidateParameters );
    strm << TPair( _WarnAsError, tool.WarnAsError );
    strm << EPair( _WarningLevel, tool.WarningLevel );
    strm << "/>";
    return strm;
}

bool VCMIDLTool::parseOption( const char* option )
{
#ifdef USE_DISPLAY_HASH
    displayHash( "/D name[=def]" ); displayHash( "/I directory-list" ); displayHash( "/Oi" );
    displayHash( "/Oic" ); displayHash( "/Oicf" ); displayHash( "/Oif" ); displayHash( "/Os" );
    displayHash( "/U name" ); displayHash( "/WX" ); displayHash( "/W{0|1|2|3|4}" );
    displayHash( "/Zp {N}" ); displayHash( "/Zs" ); displayHash( "/acf filename" );
    displayHash( "/align {N}" ); displayHash( "/app_config" ); displayHash( "/c_ext" );
    displayHash( "/char ascii7" ); displayHash( "/char signed" ); displayHash( "/char unsigned" );
    displayHash( "/client none" ); displayHash( "/client stub" ); displayHash( "/confirm" );
    displayHash( "/cpp_cmd cmd_line" ); displayHash( "/cpp_opt options" );
    displayHash( "/cstub filename" ); displayHash( "/dlldata filename" ); displayHash( "/env win32" );
    displayHash( "/env win64" ); displayHash( "/error all" ); displayHash( "/error allocation" );
    displayHash( "/error bounds_check" ); displayHash( "/error enum" ); displayHash( "/error none" );
    displayHash( "/error ref" ); displayHash( "/error stub_data" ); displayHash( "/h filename" );
    displayHash( "/header filename" ); displayHash( "/iid filename" ); displayHash( "/lcid" );
    displayHash( "/mktyplib203" ); displayHash( "/ms_ext" ); displayHash( "/ms_union" );
    displayHash( "/msc_ver <nnnn>" ); displayHash( "/newtlb" ); displayHash( "/no_cpp" );
    displayHash( "/no_def_idir" ); displayHash( "/no_default_epv" ); displayHash( "/no_format_opt" );
    displayHash( "/no_warn" ); displayHash( "/nocpp" ); displayHash( "/nologo" ); displayHash( "/notlb" );
    displayHash( "/o filename" ); displayHash( "/oldnames" ); displayHash( "/oldtlb" );
    displayHash( "/osf" ); displayHash( "/out directory" ); displayHash( "/pack {N}" );
    displayHash( "/prefix all" ); displayHash( "/prefix client" ); displayHash( "/prefix server" );
    displayHash( "/prefix switch" ); displayHash( "/protocol all" ); displayHash( "/protocol dce" );
    displayHash( "/protocol ndr64" ); displayHash( "/proxy filename" ); displayHash( "/robust" );
    displayHash( "/rpcss" ); displayHash( "/savePP" ); displayHash( "/server none" );
    displayHash( "/server stub" ); displayHash( "/sstub filename" ); displayHash( "/syntax_check" );
    displayHash( "/target {system}" ); displayHash( "/tlb filename" ); displayHash( "/use_epv" );
    displayHash( "/win32" ); displayHash( "/win64" );
#endif
    bool found = TRUE;
    int offset = 0;
    switch( elfHash(option) ) {
    case 0x0000334: // /D name[=def]
	PreprocessorDefinitions += option+3;
	break;
    case 0x0000339: // /I directory-list
	AdditionalIncludeDirectories += option+3;
	break;
    case 0x0345f96: // /Oicf
    case 0x00345f6: // /Oif
	GenerateStublessProxies = _True;
	break;
    case 0x0000345: // /U name
	UndefinePreprocessorDefinitions += option+3;
	break;
    case 0x00034c8: // /WX
	WarnAsError = _True;
	break;
    case 0x3582fde: // /align {N}
	offset = 3;  // Fallthrough
    case 0x0003510: // /Zp {N}
	switch ( *(option+offset+4) ) {
	case '1':
	    StructMemberAlignment = ( *(option+offset+5) == '\0' ) ? midlAlignSingleByte : midlAlignSixteenBytes;
	    break;
	case '2':
	    StructMemberAlignment = midlAlignTwoBytes;
	    break;
	case '4':
	    StructMemberAlignment = midlAlignFourBytes;
	    break;
	case '8':
	    StructMemberAlignment = midlAlignEightBytes;
	    break;
	default:
	    found = FALSE;
	}
	break;
    case 0x0359e82: // /char {ascii7|signed|unsigned}
	switch( *(option+6) ) {
	case 'a':
	    DefaultCharType = midlCharAscii7;
	    break;
	case 's':
	    DefaultCharType = midlCharSigned;
	    break;
	case 'u':
	    DefaultCharType = midlCharUnsigned;
	    break;
	default:
	    found = FALSE;
	}
	break;
    case 0xa766524: // /cpp_opt options
	CPreprocessOptions += option+9;
	break;
    case 0xb32abf1: // /dlldata filename
	DLLDataFileName = option + 9;
	break;
    case 0x0035c56: // /env {win32|win64}
	TargetEnvironment = ( *(option+8) == '6' ) ? midlTargetWin64 : midlTargetWin32;
	break;
    case 0x35c9962: // /error {all|allocation|bounds_check|enum|none|ref|stub_data}
	EnableErrorChecks = midlEnableCustom;
	switch ( *(option+7) ) {
	case 'a':
	    if ( *(option+10) == '\0' )
		EnableErrorChecks = midlEnableAll;
	    else
		ErrorCheckAllocations = _True;
	    break;
	case 'b':
	    ErrorCheckBounds = _True;
	    break;
	case 'e':
	    ErrorCheckEnumRange = _True;
	    break;
	case 'n':
	    EnableErrorChecks = midlDisableAll;
	    break;
	case 'r':
	    ErrorCheckRefPointers = _True;
	    break;
	case 's':
	    ErrorCheckStubData = _True;
	    break;
	default:
	    found = FALSE;
	}
	break;
    case 0x5eb7af2: // /header filename
	offset = 5;
    case 0x0000358: // /h filename
	HeaderFileName = option + offset + 3;
	break;
    case 0x0035ff4: // /iid filename
	InterfaceIdentifierFileName = option+5;
	break;
    case 0x64b7933: // /mktyplib203
	MkTypLibCompatible = _True;
	break;
    case 0x8e0b0a2: // /no_def_idir
	IgnoreStandardIncludePath = _True;
	break;
    case 0x65635ef: // /nologo
	SuppressStartupBanner = _True;
	break;
    case 0x3656b22: // /notlb
	GenerateTypeLibrary = _True;
	break;
    case 0x000035f: // /o filename
	RedirectOutputAndErrors = option+3;
	break;
    case 0x00366c4: // /out directory
	OutputDirectory = option+5;
	break;
    case 0x36796f9: // /proxy filename
	ProxyFileName = option+7;
	break;
    case 0x6959c94: // /robust
	ValidateParameters = _True;
	break;
    case 0x6a88df4: // /target {system}
	if ( *(option+11) == '6' )
	    TargetEnvironment = midlTargetWin64;
	else
	    TargetEnvironment = midlTargetWin32;
	break;
    case 0x0036b22: // /tlb filename
	TypeLibraryName = option+5;
	break;
    case 0x36e0162: // /win32
	TargetEnvironment = midlTargetWin32;
	break;
    case 0x36e0194: // /win64
	TargetEnvironment = midlTargetWin64;
	break;
    case 0x0003459: // /Oi
    case 0x00345f3: // /Oic
    case 0x0003463: // /Os
    case 0x0003513: // /Zs
    case 0x0035796: // /acf filename
    case 0x5b1cb97: // /app_config
    case 0x3595cf4: // /c_ext
    case 0x5a2fc64: // /client {none|stub}
    case 0xa64d3dd: // /confirm
    case 0xa765b64: // /cpp_cmd cmd_line
    case 0x35aabb2: // /cstub filename
    case 0x03629f4: // /lcid
    case 0x6495cc4: // /ms_ext
    case 0x96c7a1e: // /ms_union
    case 0x4996fa2: // /msc_ver <nnnn>
    case 0x64ceb12: // /newtlb
    case 0x6555a40: // /no_cpp
    case 0xf64d6a6: // /no_default_epv
    case 0x6dd9384: // /no_format_opt
    case 0x556dbee: // /no_warn
    case 0x3655a70: // /nocpp
    case 0x2b455a3: // /oldnames
    case 0x662bb12: // /oldtlb
    case 0x0036696: // /osf
    case 0x036679b: // /pack {N}
    case 0x678bd38: // /prefix {all|client|server|switch}
    case 0x96b702c: // /protocol {all|dce|ndr64}
    case 0x3696aa3: // /rpcss
    case 0x698ca60: // /savePP
    case 0x69c9cf2: // /server {none|stub}
    case 0x36aabb2: // /sstub filename
    case 0xce9b12b: // /syntax_check
    case 0xc9b5f16: // /use_epv
	AdditionalOptions += option;
	break;
    default:
	// /W{0|1|2|3|4} case
	if ( *(option+1) == 'W' ) {
	    switch ( *(option+2) ) {
	    case '0':
		WarningLevel = midlWarningLevel_0;
		break;
	    case '1':
		WarningLevel = midlWarningLevel_1;
		break;
	    case '2':
		WarningLevel = midlWarningLevel_2;
		break;
	    case '3':
		WarningLevel = midlWarningLevel_3;
		break;
	    case '4':
		WarningLevel = midlWarningLevel_4;
		break;
	    default:
		found = FALSE;
	    }
	}
	break;
    }
    if( !found )
	warn_msg( WarnLogic, "Could not parse MIDL option: %s", option );
    return TRUE;
}

// VCLibrarianTool --------------------------------------------------
VCLibrarianTool::VCLibrarianTool()
    :	IgnoreAllDefaultLibraries( unset ),
	SuppressStartupBanner( _True )
{
}

QTextStream &operator<<( QTextStream &strm, const VCLibrarianTool &tool )
{
    strm << _begTool3;
    strm << SPair( _ToolName, QString( "VCLibrarianTool" ) );
    strm << XPair( _AdditionalDependencies4, tool.AdditionalDependencies );
    strm << XPair( _AdditionalLibraryDirectories, tool.AdditionalLibraryDirectories );
    strm << XPair( _AdditionalOptions, tool.AdditionalOptions, " " );
    strm << XPair( _ExportNamedFunctions, tool.ExportNamedFunctions );
    strm << XPair( _ForceSymbolReferences, tool.ForceSymbolReferences );
    strm << TPair( _IgnoreAllDefaultLibraries, tool.IgnoreAllDefaultLibraries );
    strm << XPair( _IgnoreDefaultLibraryNames, tool.IgnoreDefaultLibraryNames );
    strm << SPair( _ModuleDefinitionFile, tool.ModuleDefinitionFile );
    strm << SPair( _OutputFile, tool.OutputFile );
    strm << TPair( _SuppressStartupBanner, tool.SuppressStartupBanner );
    strm << "/>";
    return strm;
}

// VCCustomBuildTool ------------------------------------------------
VCCustomBuildTool::VCCustomBuildTool()
{
    ToolName = "VCCustomBuildTool";
}

QTextStream &operator<<( QTextStream &strm, const VCCustomBuildTool &tool )
{
    strm << _begTool3;
    strm << SPair( _ToolName, tool.ToolName );
    strm << XPair( _AdditionalDependencies4, tool.AdditionalDependencies, ";" );

    if (which_dotnet_version() == NET2005)
        strm << XPair( _CommandLine4, tool.CommandLine, "&#x0D;&#x0A;" );
    else
        strm << XPair( _CommandLine4, tool.CommandLine, "\n" );

    strm << SPair( _Description4, tool.Description );
    strm << XPair( _Outputs4, tool.Outputs, ";" );
    strm << SPair( _ToolPath, tool.ToolPath );
    strm << "/>";
   return strm;
}

// VCResourceCompilerTool -------------------------------------------
VCResourceCompilerTool::VCResourceCompilerTool()
    :   Culture( rcUseDefault ),
	IgnoreStandardIncludePath( unset ),
	ShowProgress( linkProgressNotSet )
{
    PreprocessorDefinitions = "NDEBUG";
}

QTextStream &operator<<( QTextStream &strm, const VCResourceCompilerTool &tool )
{
    strm << _begTool3;
    strm << _VCResourceCompilerToolName;
    strm << SPair( _ToolPath, tool.ToolPath );
    strm << XPair( _AdditionalIncludeDirectories, tool.AdditionalIncludeDirectories );
    strm << XPair( _AdditionalOptions, tool.AdditionalOptions, " " );
    if ( tool.Culture != rcUseDefault )			    strm << EPair( _Culture, tool.Culture );
    strm << XPair( _FullIncludePath, tool.FullIncludePath );
    strm << TPair( _IgnoreStandardIncludePath, tool.IgnoreStandardIncludePath );
    strm << XPair( _PreprocessorDefinitions, tool.PreprocessorDefinitions );
    strm << SPair( _ResourceOutputFileName, tool.ResourceOutputFileName );
    if ( tool.ShowProgress != linkProgressNotSet )	    strm << EPair( _ShowProgress, tool.ShowProgress );
    strm << "/>";
    return strm;
}

// VCEventTool -------------------------------------------------
QTextStream &operator<<( QTextStream &strm, const VCEventTool &tool )
{
    strm << _begTool3;
    strm << SPair( _ToolName, tool.ToolName );
    strm << SPair( _ToolPath, tool.ToolPath );
    strm << SPair( _CommandLine4, tool.CommandLine );
    strm << SPair( _Description4, tool.Description );
    strm << TPair( _ExcludedFromBuild, tool.ExcludedFromBuild );
    strm << "/>";
    return strm;
}

// VCPostBuildEventTool ---------------------------------------------
VCPostBuildEventTool::VCPostBuildEventTool()
{
    ToolName = "VCPostBuildEventTool";
}

// VCPreBuildEventTool ----------------------------------------------
VCPreBuildEventTool::VCPreBuildEventTool()
{
    ToolName = "VCPreBuildEventTool";
}

// VCPreLinkEventTool -----------------------------------------------
VCPreLinkEventTool::VCPreLinkEventTool()
{
    ToolName = "VCPreLinkEventTool";
}

// VCConfiguration --------------------------------------------------

VCConfiguration::VCConfiguration()
    :	ATLMinimizesCRunTimeLibraryUsage( unset ),
	BuildBrowserInformation( unset ),
	CharacterSet( charSetNotSet ),
	ConfigurationType( typeApplication ),
	RegisterOutput( unset ),
	UseOfATL( useATLNotSet ),
	UseOfMfc( useMfcStdWin ),
	WholeProgramOptimization( unset )
{
    compiler.config = this;
    linker.config = this;
    idl.config = this;
}

VCConfiguration::VCConfiguration(const VCConfiguration &other)
{
    *this = other;
    compiler.config = this;
    linker.config = this;
    idl.config = this;
}

QTextStream &operator<<( QTextStream &strm, const VCConfiguration &tool )
{
    strm << _begConfiguration;
    strm << SPair( _Name3, tool.Name );
    strm << SPair( _OutputDirectory3, tool.OutputDirectory );
    strm << TPair( _ATLMinimizesCRunTimeLibraryUsage, tool.ATLMinimizesCRunTimeLibraryUsage );
    strm << TPair( _BuildBrowserInformation, tool.BuildBrowserInformation );
    if ( tool.CharacterSet != charSetNotSet)	strm << EPair( _CharacterSet, tool.CharacterSet );
    strm << EPair( _ConfigurationType, tool.ConfigurationType );
    strm << SPair( _DeleteExtensionsOnClean, tool.DeleteExtensionsOnClean );
    strm << SPair( _ImportLibrary, tool.ImportLibrary );
    strm << SPair( _IntermediateDirectory, tool.IntermediateDirectory );
    strm << SPair( _PrimaryOutput, tool.PrimaryOutput );
    strm << SPair( _ProgramDatabase, tool.ProgramDatabase );
    strm << TPair( _RegisterOutput, tool.RegisterOutput );
    if ( tool.UseOfATL != useATLNotSet)		strm << EPair( _UseOfATL, tool.UseOfATL );
    strm << EPair( _UseOfMfc, tool.UseOfMfc );
    strm << TPair( _WholeProgramOptimization, tool.WholeProgramOptimization );
    strm << ">";
    strm << tool.compiler;
    strm << tool.custom;
    if ( tool.ConfigurationType == typeStaticLibrary )
	strm << tool.librarian;
    else
	strm << tool.linker;
    strm << tool.idl;
    strm << tool.postBuild;
    strm << tool.preBuild;
    strm << tool.preLink;
    strm << tool.resource;
    strm << _endConfiguration;
    return strm;
}
// VCFilter ---------------------------------------------------------
VCFilter::VCFilter()
    :   ParseFiles( unset )
{
    useCustomBuildTool = FALSE;
    useCompilerTool = FALSE;
}

void VCFilter::addMOCstage( QTextStream & /*strm*/, QString filename )
{
    QString mocOutput = Project->findMocDestination( filename );
    QString mocApp = Project->var( "QMAKE_MOC" );

    bool inputMoc = false;
    if( mocOutput.isEmpty() && filename.endsWith(".moc") ) {
        // In specialcases we DO moc .cpp files
	// when the result is an .moc file
	mocOutput = filename;
	filename = Project->findMocSource( mocOutput );
        inputMoc = true;
    }

    if (mocOutput.isEmpty())
	return;

    CustomBuildTool = VCCustomBuildTool();
    useCustomBuildTool = TRUE;
    CustomBuildTool.Description = "Moc&apos;ing " + filename + "...";
    CustomBuildTool.CommandLine += (mocApp + " "
				+ filename + " -o " + mocOutput);
    CustomBuildTool.AdditionalDependencies = mocApp;
    if (inputMoc)
        CustomBuildTool.AdditionalDependencies += filename;
    CustomBuildTool.Outputs += mocOutput;
}

void VCFilter::addUICstage( QTextStream & /*strm*/, QString str )
{
    CustomBuildTool = VCCustomBuildTool();
    useCustomBuildTool = TRUE;

    QString uicApp = Project->var("QMAKE_UIC");
    QString mocApp = Project->var( "QMAKE_MOC" );
    QString fname = str.section( '\\', -1 );
    QString mocDir = Project->var( "MOC_DIR" );
    QString uiDir = Project->var( "UI_DIR" );
    QString uiHeaders;
    QString uiSources;

    // Determining the paths for the output files.
    int slash = str.findRev( '\\' );
    QString pname = ( slash != -1 ) ? str.left( slash+1 ) : QString( ".\\" );
    if( !uiDir.isEmpty() ) {
	uiHeaders = uiDir;
	uiSources = uiDir;
    } else {
	uiHeaders = Project->var( "UI_HEADERS_DIR" );
	uiSources = Project->var( "UI_SOURCES_DIR" );
	if( uiHeaders.isEmpty() )
	    uiHeaders = pname;
	if( uiSources.isEmpty() )
	    uiSources = pname;
    }
    if( !uiHeaders.endsWith( "\\" ) )
	uiHeaders += "\\";
    if( !uiSources.endsWith( "\\" ) )
	uiSources += "\\";

    // Determine the file name.
    int dot = fname.findRev( '.' );
    if( dot != -1 )
	fname.truncate( dot );

    if ( mocDir.isEmpty() )
	mocDir = pname;

    CustomBuildTool.Description = ("Uic'ing " + str + "...\"");
    CustomBuildTool.CommandLine += // Create .h from .ui file
	uicApp + " " + str + " -o " + uiHeaders + fname + ".h";
    CustomBuildTool.CommandLine += // Create .cpp from .ui file
	uicApp + " " + str + " -i " + fname + ".h -o " + uiSources + fname + ".cpp";
    CustomBuildTool.CommandLine += // Moc the headerfile
	mocApp + " " + uiHeaders + fname + ".h -o " + mocDir + Option::h_moc_mod + fname + Option::h_moc_ext;

    CustomBuildTool.AdditionalDependencies += mocApp;
    CustomBuildTool.AdditionalDependencies += uicApp;
    CustomBuildTool.Outputs +=
	uiHeaders + fname + ".h;" + uiSources + fname + ".cpp;" + mocDir + Option::h_moc_mod + fname + Option::h_moc_ext;
}

void VCFilter::modifyPCHstage( QTextStream &/*strm*/, QString str )
{
    bool isCFile = str.endsWith(".c");
    bool isHFile = (str.endsWith(".h") && str == Project->precompH);

    if (!isCFile && !isHFile)
	return;

    CompilerTool = VCCLCompilerTool();
    useCompilerTool = TRUE;

    // Unset some default options
    CompilerTool.BufferSecurityCheck = unset;
    CompilerTool.DebugInformationFormat = debugUnknown;
    CompilerTool.ExceptionHandling = unset;
    CompilerTool.GeneratePreprocessedFile = preprocessUnknown;
    CompilerTool.Optimization = optimizeDefault;
    CompilerTool.ProgramDataBaseFileName = QString::null;
    CompilerTool.RuntimeLibrary = rtUnknown;
    CompilerTool.WarningLevel = warningLevelUnknown;

    // Setup PCH options
    CompilerTool.UsePrecompiledHeader     = (isCFile ? pchNone : pchCreateUsingSpecific);
    CompilerTool.PrecompiledHeaderThrough = "$(NOINHERIT)";
    CompilerTool.ForcedIncludeFiles       = "$(NOINHERIT)";
}

bool VCFilter::addIMGstage( QTextStream &/*strm*/, QString str )
{
    bool isCorH = FALSE;
    if (str.endsWith(".c") || str.endsWith(".rc"))
	isCorH = TRUE;
    QStringList::Iterator it;
    for(it = Option::cpp_ext.begin(); it != Option::cpp_ext.end(); ++it)
	if(str.endsWith(*it))
	    isCorH = TRUE;
    for(it = Option::h_ext.begin(); it != Option::h_ext.end(); ++it)
	if(str.endsWith(*it))
	    isCorH = TRUE;

    QString collectionName = Project->project->first("QMAKE_IMAGE_COLLECTION");
    if (str.isEmpty() || isCorH || collectionName.isEmpty())
	return FALSE;

    CustomBuildTool = VCCustomBuildTool();
    useCustomBuildTool = TRUE;

    // Some projects (like designer core) may have too many images to
    // call uic directly. Therefor we have to create a temporary
    // file, with the image list, and call uic with the -f option.
    QString tmpFileCmd = "echo ";
    QString tmpImageFilename = ".imgcol";
    QStringList& list = Project->project->variables()["IMAGES"];
    bool firstOutput = TRUE;
    it = list.begin();
    while( it!=list.end() ) {
	tmpFileCmd += (*it) + " ";
	++it;
	if (tmpFileCmd.length()>250 || it==list.end()) {
	    CustomBuildTool.CommandLine += tmpFileCmd
					  + (firstOutput?"> ":">> ")
					  + tmpImageFilename;
	    tmpFileCmd = "echo ";
	    firstOutput = FALSE;
	}
    }

    QString uicApp = Project->var("QMAKE_UIC");
    QString commandLine = uicApp + " -embed " + Project->project->first("QMAKE_ORIG_TARGET")
	+ " -f .imgcol -o " + collectionName;

    // The loop below is to avoid the resulting CommandLine buffer
    // from being a size of between 2071-#char_replaced and 2102,
    // as this triggers a buffer overflow bug in VS2003. As we only
    // the only replacement we use in this buffer is one $(QTDIR),
    // we assume this can be upto 256 characters long, making the
    // lower-bound to be 1814 characters. So, if the buffer is
    // between 1814 and 2103 bytes, the buffer is "padded" til it's
    // over 2103 bytes in size.
    int totalSize = CustomBuildTool.CommandLine.join("  ").length();
    while (totalSize > 1814 && totalSize < 2103) {
        CustomBuildTool.CommandLine += 
            "echo \"Padding\" the Custom Build Step buffer to avoid a potentional "
            "buffer overflow issue with VS2003 for buffer of 1814-2103 bytes > nul";
        totalSize = CustomBuildTool.CommandLine.join("  ").length();
    }

    CustomBuildTool.Description = ("Generate imagecollection");
    CustomBuildTool.CommandLine += commandLine;
    CustomBuildTool.AdditionalDependencies += uicApp;
    CustomBuildTool.AdditionalDependencies += list;
    CustomBuildTool.Outputs = collectionName;
    CustomBuildTool.Outputs += tmpImageFilename;
    return TRUE;
}

bool VCFilter::addLexYaccStage( QTextStream &/*strm*/, QString str )
{
    bool doLex = str.endsWith(".l");
    if (!doLex && !str.endsWith(".y"))
        return FALSE;

    QString fname = "$(InputName)";
    if (doLex) {
        fname += Option::lex_mod + Option::cpp_ext.first();
	QString lexpath = Project->var("QMAKE_LEX")
                        + Project->varGlue("QMAKE_LEXFLAGS", " ", " ", "") + " ";

        CustomBuildTool.Description = "Lex'ing $(InputFileName)";
        CustomBuildTool.CommandLine = lexpath + " $(InputFileName)";
        CustomBuildTool.CommandLine += "del " + fname;
        CustomBuildTool.CommandLine += "move lex.yy.c " + fname;
        CustomBuildTool.Outputs = fname;
    } else {
        fname +=Option::yacc_mod;
	QString yaccpath = Project->var("QMAKE_YACC")
                         + Project->varGlue("QMAKE_YACCFLAGS", " ", " ", "") + " ";

        CustomBuildTool.Description = "Yacc'ing $(InputFileName)";
        CustomBuildTool.CommandLine = yaccpath + " $(InputFileName)";
        CustomBuildTool.CommandLine += "del " + fname + Option::h_ext.first();
        CustomBuildTool.CommandLine += "move y.tab.h " + fname + Option::h_ext.first();
        CustomBuildTool.Outputs = fname + Option::h_ext.first();
        CustomBuildTool.CommandLine += "del " + fname + Option::cpp_ext.first();
        CustomBuildTool.CommandLine += "move y.tab.c " + fname + Option::cpp_ext.first();
        CustomBuildTool.Outputs += fname + Option::cpp_ext.first();
    }
    useCustomBuildTool = TRUE;
    return TRUE;
}

QTextStream &operator<<( QTextStream &strm, VCFilter &tool )
{
    if ( tool.Files.count() == 0 )
	return strm;

    strm << _begFilter;
    strm << SPair( _Name3, tool.Name );
    strm << TPair( _ParseFiles, tool.ParseFiles );
    strm << SPair( _Filter, tool.Filter );
    strm << ">";

    bool resourceBuild = FALSE;
    int currentLevels = 0;
    QStringList currentDirs;
    for ( QStringList::ConstIterator it = tool.Files.begin(); it != tool.Files.end(); ++it ) {
	if ( !tool.flat_files ) {
	    QStringList newDirs = QStringList::split('\\',(*it));
	    newDirs.pop_back(); // Skip the filename

	    int newLevels = int(newDirs.count());
	    int equalLevels = 0;
	    for (int i = 0; i<currentLevels; i++, equalLevels++ )
		if (currentDirs[i] != newDirs[i])
		    break;
	    int closeFilters = currentLevels - equalLevels;
	    int openFilters  = newLevels - equalLevels;

	    // close previous non-equal filter
	    while ( closeFilters-- )
		strm << _endFilter;

	    // open new non-equal filters
	    newLevels = 0;
	    while ( openFilters-- ) {
		strm << _begFilter;
		strm << SPair( _Name3, newDirs[equalLevels + newLevels] );
		strm << _Filter << "\">"; // Blank filter
		++newLevels;
	    }
	    currentDirs = newDirs;
	    currentLevels = int(newDirs.count());
	}

	tool.useCustomBuildTool = FALSE;
	tool.useCompilerTool = FALSE;
	// Add UIC, MOC and PCH stages to file
	if ( tool.CustomBuild == moc )
	    tool.addMOCstage( strm, *it );
	else if ( tool.CustomBuild == uic )
	    tool.addUICstage( strm, *it );
	else if ( tool.CustomBuild == lexyacc )
	    tool.addLexYaccStage( strm, *it );
	else if ( tool.CustomBuild == resource ) {
	    if (!resourceBuild)
		resourceBuild = tool.addIMGstage(strm, *it);
	}
	if (tool.Project->usePCH)
	    tool.modifyPCHstage( strm, *it );

	strm << _begFile;
	strm << SPair( _RelativePath, *it );
	strm << ">";
	// Output custom build and compiler options
	// for all configurations
	if (tool.useCustomBuildTool || tool.useCompilerTool) {
	    for ( uint i = 0; i < tool.Config->count(); i++ ) {
		strm << _begFileConfiguration;
		strm << _Name5;
		strm << (*tool.Config)[i].Name;
		strm << "\">";
		if (tool.useCustomBuildTool)
		    strm <<  tool.CustomBuildTool;
		if (tool.useCompilerTool)
		    strm <<  tool.CompilerTool;
		strm << _endFileConfiguration;
	    }
	}
	strm << _endFile;
    }
    // close remaining open filters, in non-flat mode
    while ( !tool.flat_files && currentLevels-- ) {
	strm << _endFilter;
    }
    strm << _endFilter;
    return strm;
}

// VCProject --------------------------------------------------------
VCProject::VCProject()
{
    VCConfiguration conf;
    Configuration += conf ; // Release
    //Configuration += conf ; // Debug added later, after Release init
}

QTextStream &operator<<( QTextStream &strm, const VCProject &tool )
{
    strm << _xmlInit;
    strm << _begVisualStudioProject;
    strm << _ProjectType;
    strm << SPair( _Version1, tool.Version );
    strm << SPair( _Name1, tool.Name );
    strm << SPair( _ProjectGUID, tool.ProjectGUID );
    strm << SPair( _SccProjectName, tool.SccProjectName );
    strm << SPair( _SccLocalPath, tool.SccLocalPath );
    strm << ">";
    strm << _begPlatforms;
    strm << _begPlatform;
    strm << SPair( _Name3, tool.PlatformName );
    strm << "/>";
    strm << _endPlatforms;
    strm << _begConfigurations;
    for ( uint i = 0; i < tool.Configuration.count(); i++ )
	strm << tool.Configuration[i];
    strm << _endConfigurations;
    strm << _begFiles;
    strm << (VCFilter&)tool.SourceFiles;
    strm << (VCFilter&)tool.HeaderFiles;
    strm << (VCFilter&)tool.MOCFiles;
    strm << (VCFilter&)tool.UICFiles;
    strm << (VCFilter&)tool.FormFiles;
    strm << (VCFilter&)tool.TranslationFiles;
    strm << (VCFilter&)tool.LexYaccFiles;
    strm << (VCFilter&)tool.ResourceFiles;
    strm << _endFiles;
    strm << _begGlobals;
    strm << _endGlobals;
    strm << _endVisualStudioProject;
    return strm;
}
