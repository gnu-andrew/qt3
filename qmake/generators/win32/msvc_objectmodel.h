/****************************************************************************
**
** Definition of VCProject class.
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

#ifndef __MSVC_OBJECTMODEL_H__
#define __MSVC_OBJECTMODEL_H__

#include "project.h"
#include <qstring.h>
#include <qstringlist.h>

enum DotNET {
    NETUnknown = 0,
    NET2002 = 0x70,
    NET2003 = 0x71,
    NET2005 = 0x80
};

/*
    This Object model is of course VERY simplyfied,
    and does not actually follow the original MSVC
    object model. However, it fulfilles the basic
    needs for qmake
*/

/*
    If a triState value is 'unset' then the
    corresponding property is not in the output,
    forcing the tool to utilize default values.
    False/True values will be in the output...
*/
enum customBuildCheck {
    none,
    moc,
    uic,
    lexyacc,
    resource
};
enum triState {
    unset = -1,
    _False = 0,
    _True = 1
};
enum addressAwarenessType {
    addrAwareDefault,
    addrAwareNoLarge,
    addrAwareLarge
};
enum asmListingOption {
    asmListingNone,
    asmListingAssemblyOnly,
    asmListingAsmMachineSrc,
    asmListingAsmMachine,
    asmListingAsmSrc
};
enum basicRuntimeCheckOption {
    runtimeBasicCheckNone,
    runtimeCheckStackFrame,
    runtimeCheckUninitVariables,
    runtimeBasicCheckAll
};
enum browseInfoOption {
    brInfoNone,
    brAllInfo,
    brNoLocalSymbols
};
enum callingConventionOption {
    callConventionDefault = -1,
    callConventionCDecl,
    callConventionFastCall,
    callConventionStdCall
};
enum charSet {
    charSetNotSet,
    charSetUnicode,
    charSetMBCS
};
enum compileAsManagedOptions {
    managedDefault = -1,
    managedAssembly = 2
};
enum CompileAsOptions{
    compileAsDefault,
    compileAsC,
    compileAsCPlusPlus
};
enum ConfigurationTypes {
    typeUnknown        = 0,
    typeApplication    = 1,
    typeDynamicLibrary = 2,
    typeStaticLibrary  = 4,
    typeGeneric        = 10
};
enum debugOption {
    debugUnknown = -1,
    debugDisabled,
    debugOldStyleInfo,
    debugLineInfoOnly,
    debugEnabled,
    debugEditAndContinue
};
enum eAppProtectionOption {
    eAppProtectUnchanged,
    eAppProtectLow,
    eAppProtectMedium,
    eAppProtectHigh
};
enum enumResourceLangID {
    rcUseDefault		= 0,
    rcAfrikaans			= 1078,
    rcAlbanian			= 1052,
    rcArabicAlgeria		= 5121,
    rcArabicBahrain		= 15361,
    rcArabicEgypt		= 3073,
    rcArabicIraq		= 2049,
    rcArabicJordan		= 11265,
    rcArabicKuwait		= 13313,
    rcArabicLebanon		= 12289,
    rcArabicLibya		= 4097,
    rcArabicMorocco		= 6145,
    rcArabicOman		= 8193,
    rcArabicQatar		= 16385,
    rcArabicSaudi		= 1025,
    rcArabicSyria		= 10241,
    rcArabicTunisia		= 7169,
    rcArabicUnitedArabEmirates	= 14337,
    rcArabicYemen		= 9217,
    rcBasque			= 1069,
    rcBulgarian			= 1026,
    rcByelorussian		= 1059,
    rcCatalan			= 1027,
    rcChineseHongKong		= 3076,
    rcChinesePRC		= 2052,
    rcChineseSingapore		= 4100,
    rcChineseTaiwan		= 1028,
    rcCroatian			= 1050,
    rcCzech			= 1029,
    rcDanish			= 1030,
    rcDutchBelgium		= 2067,
    rcDutchStandard		= 1043,
    rcEnglishAustralia		= 3081,
    rcEnglishBritain		= 2057,
    rcEnglishCanada		= 4105,
    RcEnglishCaribbean		= 9225,
    rcEnglishIreland		= 6153,
    rcEnglishJamaica		= 8201,
    rcEnglishNewZealand		= 5129,
    rcEnglishSouthAfrica	= 7177,
    rcEnglishUS			= 1033,
    rcEstonian			= 1061,
    rcFarsi			= 1065,
    rcFinnish			= 1035,
    rcFrenchBelgium		= 2060,
    rcFrenchCanada		= 3084,
    rcFrenchLuxembourg		= 5132,
    rcFrenchStandard		= 1036,
    rcFrenchSwitzerland		= 4108,
    rcGermanAustria		= 3079,
    rcGermanLichtenstein	= 5127,
    rcGermanLuxembourg		= 4103,
    rcGermanStandard		= 1031,
    rcGermanSwitzerland		= 2055,
    rcGreek			= 1032,
    rcHebrew			= 1037,
    rcHungarian			= 1038,
    rcIcelandic			= 1039,
    rcIndonesian		= 1057,
    rcItalianStandard		= 1040,
    rcItalianSwitzerland	= 2064,
    rcJapanese			= 1041,
    rcKorean			= 1042,
    rcKoreanJohab		= 2066,
    rcLatvian			= 1062,
    rcLithuanian		= 1063,
    rcNorwegianBokmal		= 1044,
    rcNorwegianNynorsk		= 2068,
    rcPolish			= 1045,
    rcPortugueseBrazilian	= 1046,
    rcPortugueseStandard	= 2070,
    rcRomanian			= 1048,
    rcRussian			= 1049,
    rcSerbian			= 2074,
    rcSlovak			= 1051,
    rcSpanishArgentina		= 11274,
    rcSpanishBolivia		= 16394,
    rcSpanishChile		= 13322,
    rcSpanishColombia		= 9226,
    rcSpanishCostaRica		= 5130,
    rcSpanishDominicanRepublic	= 7178,
    rcSpanishEcuador		= 12298,
    rcSpanishGuatemala		= 4106,
    rcSpanishMexico		= 2058,
    rcSpanishModern		= 3082,
    rcSpanishPanama		= 6154,
    rcSpanishParaguay		= 15370,
    rcSpanishPeru		= 10250,
    rcSpanishTraditional	= 1034,
    rcSpanishUruguay		= 14346,
    rcSpanishVenezuela		= 8202,
    rcSwedish			= 1053,
    rcThai			= 1054,
    rcTurkish			= 1055,
    rcUkrainian			= 1058,
    rcUrdu			= 1056
};
enum enumSccEvent {
    eProjectInScc,
    ePreDirtyNotification
};
enum favorSizeOrSpeedOption {
    favorNone,
    favorSpeed,
    favorSize
};
enum genProxyLanguage {
    genProxyNative,
    genProxyManaged
};
enum inlineExpansionOption {
    expandDisable,
    expandOnlyInline,
    expandAnySuitable,
    expandDefault // Not useful number, but stops the output
};
enum linkIncrementalType {
    linkIncrementalDefault,
    linkIncrementalNo,
    linkIncrementalYes
};
enum linkProgressOption {
    linkProgressNotSet,
    linkProgressAll,
    linkProgressLibs
};
enum machineTypeOption {
    machineNotSet,
    machineX86
};
enum midlCharOption {
    midlCharUnsigned,
    midlCharSigned,
    midlCharAscii7
};
enum midlErrorCheckOption {
    midlEnableCustom,
    midlDisableAll,
    midlEnableAll
};
enum midlStructMemberAlignOption {
    midlAlignNotSet,
    midlAlignSingleByte,
    midlAlignTwoBytes,
    midlAlignFourBytes,
    midlAlignEightBytes,
    midlAlignSixteenBytes
};
enum midlTargetEnvironment {
    midlTargetNotSet,
    midlTargetWin32,
    midlTargetWin64
};
enum midlWarningLevelOption {
    midlWarningLevel_0,
    midlWarningLevel_1,
    midlWarningLevel_2,
    midlWarningLevel_3,
    midlWarningLevel_4
};
enum optFoldingType {
    optFoldingDefault,
    optNoFolding,
    optFolding
};
enum optimizeOption {
    optimizeDisabled,
    optimizeMinSpace,
    optimizeMaxSpeed,
    optimizeFull,
    optimizeCustom,
    optimizeDefault // Not useful number, but stops the output
};
enum optRefType {
    optReferencesDefault,
    optNoReferences,
    optReferences
};
enum optWin98Type {
    optWin98Default,
    optWin98No,
    optWin98Yes
};
enum pchOption {
    pchNone,
    pchCreateUsingSpecific,
    pchGenerateAuto,
    pchUseUsingSpecific
};
enum preprocessOption {
    preprocessUnknown = -1,
    preprocessNo,
    preprocessYes,
    preprocessNoLineNumbers
};
enum ProcessorOptimizeOption {
    procOptimizeBlended,
    procOptimizePentium,
    procOptimizePentiumProAndAbove
};
enum RemoteDebuggerType {
    DbgLocal,
    DbgRemote,
    DbgRemoteTCPIP
};
enum runtimeLibraryOption {
    rtUnknown = -1,
    rtMultiThreaded,
    rtMultiThreadedDebug,
    rtMultiThreadedDLL,
    rtMultiThreadedDebugDLL,
    rtSingleThreaded,
    rtSingleThreadedDebug
};
enum structMemberAlignOption {
    alignNotSet,
    alignSingleByte,
    alignTwoBytes,
    alignFourBytes,
    alignEightBytes,
    alignSixteenBytes
};
enum subSystemOption {
    subSystemNotSet,
    subSystemConsole,
    subSystemWindows
};
enum termSvrAwarenessType {
    termSvrAwareDefault,
    termSvrAwareNo,
    termSvrAwareYes
};
enum toolSetType {
    toolSetUtility,
    toolSetMakefile,
    toolSetLinker,
    toolSetLibrarian,
    toolSetAll
};
enum TypeOfDebugger {
    DbgNativeOnly,
    DbgManagedOnly,
    DbgMixed,
    DbgAuto
};
enum useOfATL {
    useATLNotSet,
    useATLStatic,
    useATLDynamic
};
enum useOfMfc {
    useMfcStdWin,
    useMfcStatic,
    useMfcDynamic
};
enum warningLevelOption {
    warningLevelUnknown = -1,
    warningLevel_0,
    warningLevel_1,
    warningLevel_2,
    warningLevel_3,
    warningLevel_4
};

class VCToolBase {
protected:
    // Functions
    VCToolBase(){};
    virtual ~VCToolBase(){}
    virtual bool parseOption( const char* option ) = 0;
public:
    void parseOptions( QStringList& options ) {
	for ( QStringList::ConstIterator it=options.begin(); (it!=options.end()); it++ )
	    parseOption( (*it).latin1() );
    }
};

class VCConfiguration;
class VCProject;

class VCCLCompilerTool : public VCToolBase
{
public:
    // Functions
    VCCLCompilerTool();
    virtual ~VCCLCompilerTool(){}
    bool parseOption( const char* option );

    // Variables
    QStringList		    AdditionalIncludeDirectories;
    QStringList		    AdditionalOptions;
    QStringList		    AdditionalUsingDirectories;
    QString		    AssemblerListingLocation;
    asmListingOption	    AssemblerOutput;
    basicRuntimeCheckOption BasicRuntimeChecks;
    browseInfoOption	    BrowseInformation;
    QString		    BrowseInformationFile;
    triState		    BufferSecurityCheck;
    callingConventionOption CallingConvention;
    CompileAsOptions	    CompileAs;
    compileAsManagedOptions CompileAsManaged;
    triState		    CompileOnly;
    debugOption		    DebugInformationFormat;
    triState		    DefaultCharIsUnsigned;
    triState		    Detect64BitPortabilityProblems;
    triState		    DisableLanguageExtensions;
    QStringList		    DisableSpecificWarnings;
    triState		    EnableFiberSafeOptimizations;
    triState		    EnableFunctionLevelLinking;
    triState		    EnableIntrinsicFunctions;
    triState		    ExceptionHandling;
    triState		    ExpandAttributedSource;
    favorSizeOrSpeedOption  FavorSizeOrSpeed;
    triState		    ForceConformanceInForLoopScope;
    QStringList		    ForcedIncludeFiles;
    QStringList		    ForcedUsingFiles;
    preprocessOption	    GeneratePreprocessedFile;
    triState		    GlobalOptimizations;
    triState		    IgnoreStandardIncludePath;
    triState		    ImproveFloatingPointConsistency;
    inlineExpansionOption   InlineFunctionExpansion;
    triState		    KeepComments;
    triState		    MinimalRebuild;
    QString		    ObjectFile;
    triState		    OmitFramePointers;
    triState                OpenMP;
    optimizeOption	    Optimization;
    ProcessorOptimizeOption OptimizeForProcessor;
    triState		    OptimizeForWindowsApplication;
    QString		    OutputFile;
    QString		    PrecompiledHeaderFile;
    QString		    PrecompiledHeaderThrough;
    QStringList		    PreprocessorDefinitions;
    QString		    ProgramDataBaseFileName;
    runtimeLibraryOption    RuntimeLibrary;
    triState		    RuntimeTypeInfo;
    triState		    ShowIncludes;
    triState		    SmallerTypeCheck;
    triState		    StringPooling;
    structMemberAlignOption StructMemberAlignment;
    triState		    SuppressStartupBanner;
    triState		    TreatWChar_tAsBuiltInType;
    triState		    TurnOffAssemblyGeneration;
    triState		    UndefineAllPreprocessorDefinitions;
    QStringList		    UndefinePreprocessorDefinitions;
    pchOption		    UsePrecompiledHeader;
    triState		    WarnAsError;
    warningLevelOption	    WarningLevel;
    triState		    WholeProgramOptimization;
    VCConfiguration*	    config;
};

class VCLinkerTool : public VCToolBase
{
public:
    // Functions
    VCLinkerTool();
    virtual ~VCLinkerTool(){}
    bool parseOption( const char* option );

    // Variables
    QStringList		    AdditionalDependencies;
    QStringList		    AdditionalLibraryDirectories;
    QStringList		    AdditionalOptions;
    QStringList		    AddModuleNamesToAssembly;
    QString		    BaseAddress;
    QStringList		    DelayLoadDLLs;
    optFoldingType	    EnableCOMDATFolding;
    QString		    EntryPointSymbol;
    QStringList		    ForceSymbolReferences;
    QString		    FunctionOrder;
    triState		    GenerateDebugInformation;
    triState		    GenerateMapFile;
    long		    HeapCommitSize;
    long		    HeapReserveSize;
    triState		    IgnoreAllDefaultLibraries;
    QStringList		    IgnoreDefaultLibraryNames;
    triState		    IgnoreEmbeddedIDL;
    triState		    IgnoreImportLibrary;
    QString		    ImportLibrary;
    addressAwarenessType    LargeAddressAware;
    triState		    LinkDLL;
    linkIncrementalType     LinkIncremental;
    triState		    LinkTimeCodeGeneration;
    QString		    LinkToManagedResourceFile;
    triState		    MapExports;
    QString		    MapFileName;
    triState		    MapLines;
    QString		    MergedIDLBaseFileName;
    QString		    MergeSections;	    // Should be list?
    QString		    MidlCommandFile;
    QString		    ModuleDefinitionFile;   // Should be list?
    optWin98Type	    OptimizeForWindows98;
    optRefType		    OptimizeReferences;
    QString		    OutputFile;
    QString		    ProgramDatabaseFile;
    triState		    RegisterOutput;
    triState		    ResourceOnlyDLL;
    triState		    SetChecksum;
    linkProgressOption	    ShowProgress;
    long		    StackCommitSize;
    long		    StackReserveSize;
    QString		    StripPrivateSymbols;    // Should be list?
    subSystemOption	    SubSystem;
    triState		    SupportUnloadOfDelayLoadedDLL;
    triState		    SuppressStartupBanner;
    triState		    SwapRunFromCD;
    triState		    SwapRunFromNet;
    machineTypeOption       TargetMachine;
    termSvrAwarenessType    TerminalServerAware;
    triState		    TurnOffAssemblyGeneration;
    QString		    TypeLibraryFile;
    long		    TypeLibraryResourceID;
    QString		    Version;
    VCConfiguration*	    config;
};

class VCMIDLTool : public VCToolBase
{
public:
    // Functions
    VCMIDLTool();
    virtual ~VCMIDLTool(){}
    bool parseOption( const char* option );

    // Variables
    QStringList		    AdditionalIncludeDirectories;
    QStringList		    AdditionalOptions;
    QStringList		    CPreprocessOptions;
    midlCharOption	    DefaultCharType;
    QString		    DLLDataFileName;    // Should be list?
    midlErrorCheckOption    EnableErrorChecks;
    triState		    ErrorCheckAllocations;
    triState		    ErrorCheckBounds;
    triState		    ErrorCheckEnumRange;
    triState		    ErrorCheckRefPointers;
    triState		    ErrorCheckStubData;
    QStringList		    FullIncludePath;
    triState		    GenerateStublessProxies;
    triState		    GenerateTypeLibrary;
    QString		    HeaderFileName;
    triState		    IgnoreStandardIncludePath;
    QString		    InterfaceIdentifierFileName;
    triState		    MkTypLibCompatible;
    QString		    OutputDirectory;
    QStringList		    PreprocessorDefinitions;
    QString		    ProxyFileName;
    QString		    RedirectOutputAndErrors;
    midlStructMemberAlignOption StructMemberAlignment;
    triState		    SuppressStartupBanner;
    midlTargetEnvironment   TargetEnvironment;
    QString		    TypeLibraryName;
    QStringList		    UndefinePreprocessorDefinitions;
    triState		    ValidateParameters;
    triState		    WarnAsError;
    midlWarningLevelOption  WarningLevel;
    VCConfiguration*	    config;
};

class VCLibrarianTool : public VCToolBase
{
public:
    // Functions
    VCLibrarianTool();
    virtual ~VCLibrarianTool(){}
    bool parseOption( const char* ){ return FALSE; };

    // Variables
    QStringList		    AdditionalDependencies;
    QStringList		    AdditionalLibraryDirectories;
    QStringList		    AdditionalOptions;
    QStringList		    ExportNamedFunctions;
    QStringList		    ForceSymbolReferences;
    triState		    IgnoreAllDefaultLibraries;
    QStringList		    IgnoreDefaultLibraryNames;
    QString		    ModuleDefinitionFile;
    QString		    OutputFile;
    triState		    SuppressStartupBanner;
};

class VCCustomBuildTool : public VCToolBase
{
public:
    // Functions
    VCCustomBuildTool();
    virtual ~VCCustomBuildTool(){}
    bool parseOption( const char* ){ return FALSE; };

    // Variables
    QStringList		    AdditionalDependencies;
    QStringList		    CommandLine;
    QString		    Description;
    QStringList		    Outputs;
    QString		    ToolName;
    QString		    ToolPath;
};

class VCResourceCompilerTool : public VCToolBase
{
public:
    // Functions
    VCResourceCompilerTool();
    virtual ~VCResourceCompilerTool(){}
    bool parseOption( const char* ){ return FALSE; };

    // Variables
    QStringList		    AdditionalIncludeDirectories;
    QStringList		    AdditionalOptions;
    enumResourceLangID	    Culture;
    QStringList		    FullIncludePath;
    triState		    IgnoreStandardIncludePath;
    QStringList		    PreprocessorDefinitions;
    QString		    ResourceOutputFileName;
    linkProgressOption	    ShowProgress;
    QString		    ToolPath;
};

class VCEventTool : public VCToolBase
{
protected:
    // Functions
    VCEventTool() : ExcludedFromBuild( unset ){};
    virtual ~VCEventTool(){}
    bool parseOption( const char* ){ return FALSE; };

public:
    // Variables
    QString		    CommandLine;
    QString		    Description;
    triState		    ExcludedFromBuild;
    QString		    ToolName;
    QString		    ToolPath;
};

class VCPostBuildEventTool : public VCEventTool
{
public:
    VCPostBuildEventTool();
    ~VCPostBuildEventTool(){}
};

class VCPreBuildEventTool : public VCEventTool
{
public:
    VCPreBuildEventTool();
    ~VCPreBuildEventTool(){}
};

class VCPreLinkEventTool : public VCEventTool
{
public:
    VCPreLinkEventTool();
    ~VCPreLinkEventTool(){}
};

class VCConfiguration
{
public:
    // Functions
    VCConfiguration();
    VCConfiguration(const VCConfiguration &other);
    ~VCConfiguration() {}

    // Variables
    triState		ATLMinimizesCRunTimeLibraryUsage;
    triState		BuildBrowserInformation;
    charSet		CharacterSet;
    ConfigurationTypes	ConfigurationType;
    QString		DeleteExtensionsOnClean;
    QString		ImportLibrary;
    QString		IntermediateDirectory;
    QString		Name;
    QString		OutputDirectory;
    QString		PrimaryOutput;
    QString		ProgramDatabase;
    triState		RegisterOutput;
    useOfATL		UseOfATL;
    useOfMfc		UseOfMfc;
    triState		WholeProgramOptimization;

    // XML sub-parts
    VCCLCompilerTool	    compiler;
    VCLinkerTool	    linker;
    VCLibrarianTool	    librarian;
    VCCustomBuildTool	    custom;
    VCMIDLTool		    idl;
    VCPostBuildEventTool    postBuild;
    VCPreBuildEventTool	    preBuild;
    VCPreLinkEventTool	    preLink;
    VCResourceCompilerTool  resource;
};

class VcprojGenerator;
class VCFilter
{
public:
    // Functions
    VCFilter();
    ~VCFilter(){}
    void addMOCstage( QTextStream &strm, QString str );
    void addUICstage( QTextStream &strm, QString str );
    bool addIMGstage( QTextStream &strm, QString str );
    void modifyPCHstage( QTextStream &strm, QString str );
    bool addLexYaccStage( QTextStream &strm, QString str );

    // Variables
    QString		Name;
    QString		Filter;
    triState		ParseFiles;
    QStringList		Files;
    VcprojGenerator*	Project;
    QValueList<VCConfiguration> *Config;
    customBuildCheck	CustomBuild;
    bool		useCustomBuildTool;
    VCCustomBuildTool   CustomBuildTool;
    bool		useCompilerTool;
    VCCLCompilerTool    CompilerTool;
    bool		flat_files;
};

class VCProject
{
public:
    // Functions
    VCProject();
    ~VCProject(){}

    // Variables
    QString		Name;
    QString		Version;
    QString		ProjectGUID;
    QString		SccProjectName;
    QString		SccLocalPath;
    QString		PlatformName;

    // XML sub-parts
    QValueList<VCConfiguration> Configuration;
    VCFilter		SourceFiles;
    VCFilter		HeaderFiles;
    VCFilter		MOCFiles;
    VCFilter		UICFiles;
    VCFilter		FormFiles;
    VCFilter		TranslationFiles;
    VCFilter		LexYaccFiles;
    VCFilter		ResourceFiles;
};

QTextStream &operator<<( QTextStream &, const VCCLCompilerTool & );
QTextStream &operator<<( QTextStream &, const VCLinkerTool & );
QTextStream &operator<<( QTextStream &, const VCMIDLTool & );
QTextStream &operator<<( QTextStream &, const VCCustomBuildTool & );
QTextStream &operator<<( QTextStream &, const VCLibrarianTool & );
QTextStream &operator<<( QTextStream &, const VCResourceCompilerTool & );
QTextStream &operator<<( QTextStream &, const VCEventTool & );
QTextStream &operator<<( QTextStream &, const VCConfiguration & );
QTextStream &operator<<( QTextStream &, VCFilter & );
QTextStream &operator<<( QTextStream &, const VCProject & );

#endif //__MSVC_OBJECTMODEL_H__
