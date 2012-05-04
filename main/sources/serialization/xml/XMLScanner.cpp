
enum STATE {
  /* content */
  STATE_CONTENT,

  /* elements */
  STATE_ELEMENT_START_OPENING,
  STATE_ELEMENT_START_NAME,
  STATE_ELEMENT_END_OPENING,
  STATE_ELEMENT_END_NAME,
  STATE_ELEMENT_END_CLOSING,
  STATE_SPECIAL_ELEMENT,

  /* attributes */
  STATE_ATTRIBUTES_START,
  STATE_ATTRIBUTES,
  STATE_ATTRIBUTE_EQUAL,
  
  /* stream end */
  STATE_STREAM_END
};

/**********************************************************************************************
 * NextToken
 *********************************************************************************************/
XMLScanner::TOKEN XMLScanner::NextToken () {
  if (state == STATE_STREAM_END)
    return TOKEN_STREAM_END;

  do {
    switch (state) {
      /* Content */
      case STATE_CONTENT:
        switch (current) {
          /* Element tag */
          case '<' :
            state = STATE_ELEMENT_TAG;
            if (! output.isEmpty ()) {
              // TODO : check for whitespace only elements ...
              return TOKEN_CHARACTER_DATA;
            }
            break;

          case '&' :
            ReadReference ();
            break;

          case ']' :
            /* checks for invalid CDATA section end */
            ReadCharacter ();
            if (current == ']') { ReadCharacter ();
              if (current == '>') { ReadCharacter ();
                throw IllegalCharacterException ();
              } else {
                output.Append (']');
                output.Append (']');
                output.Append (current);
              }
            } else {
              output.Append (']');
              output.Append (current);
            }
            break;

          default :
            output.Append (current);
            break;
        }
        break;

      /* Element Tag */
      case STATE_ELEMENT_START_OPENING:
        if (current == '!') {
          ReadCharacter ();
          state = STATE_SPECIAL_ELEMENT;

        } else if (current == '?') {
          ReadCharacter ();
          state = STATE_CONTENT;
          return ReadProcessingInstruction ();

        } else if (current == '/') {
          ReadCharacter ();
          state = STATE_ELEMENT_END_NAME;
          return TOKEN_ELEMENT_END_TAG;

        } else if (isNameFirst (current)) {
          state = STATE_ELEMENT_START_NAME;
          return TOKEN_ELEMENT_START_OPENING;      

        } else {
          throw UnexpectedCharacterException (previous);
        }
        break;
      
      /* Special elements (comments and CDATA sections etc ...) */
      /* '<!' ... */
      case STATE_SPECIAL_ELEMENT:
        if (current == '-') {
          ReadComment ();
        } else if (current == '[') {
          ReadCDATASection ();
        } else {
          throw UnsupportedFeatureException (position);
        }
        state = STATE_CONTENT;
        break;

      /* Element start name target */
      /* '<' ... */
      case STATE_ELEMENT_START_NAME:
        if (isNameFirst (current)) {
          state = STATE_ELEMENT_ATTRIBUTES_START;
          return ReadQualifiedName ();
        } else {
          throw UnexpectedCharacterException (previous);
        }
        break;

      /* Element start name target */
      /* '</' ... */
      case STATE_ELEMENT_END_NAME:
        if (isNameFirst (current)) {
          state = STATE_ELEMENT_END_TAG;
          return ReadQualifiedName ();
        } else {
          throw UnexpectedCharacterException (previous);
        }
        break;

      /* Attributes */      
      case STATE_ATTRIBUTES_START:
        if (isWhitespace (current)) {
          state = STATE_ATTRIBUTES;

        } else if (current == '/') {
          state = STATE_ELEMENT_END_CLOSING;

        } else if (current == '>') {
          state = STATE_CONTENT;
          return TOKEN_ELEMENT_START_CLOSING;

        } else {
          throw UnexpectedCharacterException (previous);
        }
        break;
      
      /* '<' QualifiedName ... */
      case STATE_ATTRIBUTES :
        if (isWhitespace (current)) {
          break;

        } else if (isNameFirst (current)) {
          state = STATE_ATTRIBUTE_EQUAL;
          return ReadQualifiedName ();

        } else {
          throw UnexpectedCharacterException (previous);          
        }
        break;

      case STATE_ELEMENT_END_CLOSING:
        if (current == '>') {
          state = STATE_CONTENT;
          return TOKEN_ELEMENT_END_CLOSING;

        } else {
          throw UnexpectedCharacterException (previous);          
        }
        break;
        

      case STATE_ATTRIBUTE_EQUAL:
        if (current == '\'' || current == '\"') {
          state = STATE_ATTRIBUTES_START;
          return ReadLiteral ();
        }
        break;

      case STATE_ELEMENT_END_TAG:
        if (current == '>') {
          state = STATE_CONTENT;
          return TOKEN_ELEMENT_END_TAG_CLOSING;
        }
        break;
    }

  } while (ReadCharacter ());
  
  /* Stream end */
  return TOKEN_STREAM_END;
}

/**********************************************************************************************
 *
 * ReadComment
 *
 * Comment = '<!--' ((Character - '-') | ('-' (Character - '-')))*  '-->' ;
 *
 *********************************************************************************************/
void XMLScanner::ReadComment () {
  if (current == '-') { ReadCharacter ();
    if (current == '-') { ReadCharacter ();      
      while (ReadCharacter ()) {
        if (current == '-') { ReadCharacter ();
          if (current == '-') { ReadCharacter ();
            if (current == '>') { ReadCharacter ();
              return ;
            } else {
              // TODO : Warning : Illegal sequence '--' inside comment
            }
          }
        }
      }
      throw UnexpectedEndException ();
    }
  }
  throw UnexpectedCharacterException ();
}

/**********************************************************************************************
 *
 * ReadCDATASection
 *
 * CDATASection = '<![CDATA[' Character *  ']]>' ;
 *
 *********************************************************************************************/
void XMLScanner::ReadCDATASection () {
  if (current == '[') { ReadCharacter ();
    if (current == 'C') { ReadCharacter ();
      if (current == 'D') { ReadCharacter ();
        if (current == 'A') { ReadCharacter ();
          if (current == 'T') { ReadCharacter ();
            if (current == 'A') { ReadCharacter ();
              if (current == '[') {
                /* CDATA section content */
                while (ReadCharacter ()) {
                  if (current == ']') { ReadCharacter ();
                    if (current == ']') { ReadCharacter ();
                      if (current == '>') { ReadCharacter ();
                        return ;
                      } else {
                        output.Append (']');
                        output.Append (']');
                        output.Append (current);
                      }
                    } else {
                      output.Append (']');
                      output.Append (current);
                    }
                  } else {
                    output.Append (current);
                  }
                }
                throw UnexpectedEndException ();
              }
            }
          }
        }
      }
    }
  }
  throw UnexpectedCharacterException ();
}

/**********************************************************************************************
 *
 * ReadProcessingInstruction
 *
 * ProcessingInstruction = '<?' QualifiedName ( Whitespace (Character)* ) ? '?>' ;
 *
 *********************************************************************************************/
XMLScanner::TOKEN XMLScanner::ReadProcessingInstruction () {
  QualfiedName targer = ReadQualifiedName ();
  if (isWhitespace (current)) {
    while (ReadCharacter ()) {
      if (current == '?') {
        ReadCharacter ();
        if (current == '>') {
          ReadCharacter ();
          return TOKEN_PROCESSING_INSTRUCTION;

        } else {
          output.Append ('?');
          output.Append (current);
          continue;
        }

      } else {
        output.Append (current);
      }
    }
    throw UnexpectedEndException ();

  } else {
    throw UnexpectedCharacterException ();
  }
}

/**********************************************************************************************
 *
 * ReadReference
 *
 * Reference = EntityReference | CharacterReference ;
 *
 * CharacterReference = "&#" [0-9]+ ";" | "&#x" [0-9a-fA-F]+ ";" ;
 *
 * EntityReference = "&lt;" | "&amp;" | "&gt;" | "&quot;" | "&apos;" ;
 *
 *********************************************************************************************/
void XMLParser::ReadReference () {
  if (current == '&') ReadCharacter ();
  else throw InvalidStateException ();
  
  if (current == '#') {
    if (current == 'x') {
      /* hexadecimal encoded unicode character. */
      while (ReadCharacter ()) {
        // TODO ...
        if (current == ';') break;
      }

    } else {
      /* decimal encoded unicode character. */
      do {
        // TODO ...
        if (current == ';') break;
      } while (ReadCharacter ());
    }

  } else {
    /* named entity */
    do {
      // TODO ...
      if (current == ';') break;
    } while (ReadCharacter ());
  }
}

/**********************************************************************************************
 *
 * ReadLiteral
 *
 * Literal = '"' ([^<&"] | Reference)* '"' | "'" ([^<&'] | Reference)* "'" ;
 *
 *********************************************************************************************/
XMLScanner::TOKEN XMLParser::ReadLiteral () {
  char quote = '\0';
  if (current == '\"' || current == '\'') {
    quote = current;
    output.Reset ();
  }
  
  while (ReadCharacter ()) {
    if (current == quote) {
      ReadCharacter ();
      break;

    } else if (current == '<') {
      throw UnexpectedCharacterException ();

    } else if (current == '&') {
      ReadReference ();

    } else {
      output.Append (current);
    }
  }
  return Literal (output.toString ());
}

/**********************************************************************************************
 *
 * Whitespaces
 *
 * Whitespaces = (#x20 | #x9 | #xD | #xA)+ ;
 *
 *********************************************************************************************/
void XMLParser::SkipWhitespaces () {
  do {
    if (current != '\x20' && current != '\x9' && current != '\xD' && current != '\xA') break;
  } while (ReadCharacter ());
}

/**********************************************************************************************
 *
 * ReadQualifiedName
 *
 * QualifiedName = Name ? ":" ? Name ;
 *
 * Name = NameStartCharacter ( NameCharacter )* ;
 *
 * NameStartCharacter = "_" | [A-Z] | [a-z] ;
 *
 * NameCharacter = NameStartCharacter | "-" | "." | [0-9] ;
 *
 *********************************************************************************************/
QualifiedName XMLParser::ReadQualifiedName () {
  static const int name_charachters [] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0    
  };
  
  bool first = true;
  bool valid = true;
  
  std::string specifier;
  std::string name;

  do {
    if (valid && current == ':') {
      if (specifier == "") {
        specifier = output.toString ();
        output.Reset ();
      } else {
        valid = false;
      }

    } else if (current == '\x20' || current == '\x9' || current ==  '\xD' || current == '\xA') {
      Position end = position;

      if (!valid) throw InvalidNameException     ();
      if (first ) throw UnexpectedTokenException ();
      
      return QualifiedName (specifier, name);

    } else if ('\0' <= current && current < '\x7f' && name_characters [(int) current] == 1) {
      if ((first && '9' < current) || !first) {
        first = false;
        output.AppendIgnoreCase (current);
      } else {
        valid = false;
      }

    } else {
      valid = false;
      continue;
    }

  } while (ReadCharacter ());

  return UnexpectedEndException ();
}

/**********************************************************************************************
 *
 * ReadCharacter
 *
 * Character = #x9 | #xA | #xD | [#x20-#x7F] ;
 *
 *********************************************************************************************/
bool XMLScanner::ReadCharacter () {
  bool invalid = false;
  current = '\0';
  while (true) {
    if (&(buffer [length - 1]) < ++position) {
      if (invalid) {        
        throw InvalidInputException ();

      } else {
        long received = input->Read (buffer, STREAM_BUFFER_SIZE);
        if (received == 0) {
          return false;
        }
        buffer [length] = '\0';
        position = buffer;
      }
    }
    
    if ((*position == '\x9' || *position == '\xA' || *position == '\xD') ||
        ('\x20' <= *position && *position < '\x7f')) {
      if (! invalid) {
        current = *position;
        return true;
      } else {
        throw InvalidInputException ();
      }

    } else {
      invalid = true;
      continue;
    }
  }
  return false;
}

