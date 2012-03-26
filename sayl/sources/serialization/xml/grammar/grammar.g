/** Document **/
Document = Prolog Element Misc * - Character * RestrictedCharacter Character *

/** Comments **/
Comment = "<!--" (( Char2 - '-') | ('-' ( Char2 - '-')))* "-->" ;

/** Processing instructions **/
ProcessingInstruction = "<?" ProcessingInstructionTarget ( WhiteSpaces( Character * - ( Character* "?>" Character *)))? "?>" ;

ProcessingInstructionTarget = Name | (('X' | 'x') ('M' | 'm') ('L' | 'l')) ;

/** Prolog **/
Prolog = XMLDeclaration Misc ;

XMLDeclaration = "<?xml" VersionInfo EncodingDeclartion ? WhiteSpaces? "?>" ;

VersionInfo = WhiteSpaces "version" Equal ("'" VersionNumber "'" | '"' VersionNumber '"') ;

Equal = WhiteSpaces ? '=' WhiteSpaces ? ;

VersionNumber = '1.0' | '1.1' ;

Misc = Comment | ProcessingInstruction | WhiteSpaces ;

/** Encoding declaration **/
EncodingDeclaration = WhiteSpaces "encoding" Equal ('"' EncodingName '"' | "'" EncodingName "'" ) ;

EncodingName = [A-Za-z] ([A-Za-z0-9._] | '-')* ;

/** Element **/
Element = EmptyElementTag | StartTag content EndTag ;

/** Start-tag **/
StartTag = "<" Name ( WhiteSpaces Attribute )* WhiteSpaces? ">" ;

Attribute = Name Equal AttributeValue ;

AttributeValue = '"' ([^<&"] | Reference)* '"'
               | "'" ([^<&'] | Reference)* "'" ;

/** End-tag **/
EndTag = "</" Name WhiteSpaces ? ">" ;

/** Content of elements **/
Content = CharacterData ? (( Element | Reference | ProcessingInstruction | Comment) CharacterData ?)* ;

/** Tags for empty elements **/
EmptyElementTag = "<" Name (WhiteSpaces Attribute)* WhiteSpaces? "/>" ;

/** Character reference **/
CharacterReference = "&#" [0-9]+ ";" | "&#x" [0-9a-fA-F]+ ";" ;

/** Entity references **/
Reference = EntityReference | CharacterReference ;

EntityReference = "&" Name ";" ;

ParameterEntityReference = "%" Name ";" ;

/** Characters **/
Character = [#x1-#xD7FF] | [#xE000-#xFFFD] | [#x10000-#x10FFFF] ;

RestrictedChararacters = [#x1-#x8] | [#xB-#xC] | [#xE-#x1F] | [#x7F-#x84] | [#x86-#x9F] ;

/** Whitespace **/
WhiteSpaces = (#x20 | #x9 | #xD | #xA)+ ;

/** Identifiers and tokens **/
NameStartCharacter = ":" | [A-Z] | "_" | [a-z] | [#xC0-#xD6] | [#xD8-#xF6] | [#xF8-#x2FF] | [#x370-#x37D] | [#x37F-#x1FFF] | [#x200C-#x200D] | [#x2070-#x218F] | [#x2C00-#x2FEF] | [#x3001-#xD7FF] | [#xF900-#xFDCF] | [#xFDF0-#xFFFD] | [#x10000-#xEFFFF] ;

NameCharacter = NameStartChar4 | "-" | "." | [0-9] | #xB7 | [#x0300-#x036F] | [#x203F-#x2040] ;

Name = NameStartCharacter ( NameCharacter )* ;

Names = Name (#x20 Name)* ;

NumericToken = ( NameCharacter )+ ;

NumericTokens = NumericToken (#x20 NumericToken)* ;

/** Character data **/
CharacterData = [^<&]* - ([^<&]* ']]>' [^<&]*) ;

