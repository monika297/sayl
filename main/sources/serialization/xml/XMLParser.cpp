
/**********************************************************************************************
 *
 * ParseElement
 *
 * Element = "<" QualifiedName { Attribute }  ( ">" Content "</" QualifiedName ">" | "/>" ) ;
 *
 *********************************************************************************************/
void Parser::ParseElement () {
  AcceptToken (TOKEN_ANGLE_LEFT);

  // TODO : ...
  Element element (getParentElement ());

  element.setPrefix (scanner.ReadPrefix ());
  element.setLocalName (scanner.ReadLocalName ());
  
  while (! CurrentToken (TOKEN_ANGLE_RIGHT) && ! CurrentToken (TOKEN_SLASH_ANGLE_RIGHT))) {
  while (! CurrentToken (TOKEN_ANGLE_RIGHT) && ! CurrentToken (TOKEN_SLASH_ANGLE_RIGHT))) {
    try {
      XMLScanner::Attribute attribute = scanner.ReadAttribute ();
      if (attribute.getLocalName () == "xmlns") {
        Element.addNamespace (attribute.getPrefix (), attribute.getValue ());
      } else {
        element.addAttribute (attribute);
      }

    } catch (...) {
      // TODO : ParseExeption ...
    }
  }
  
  try {
    element.ResolveNames ();
  } catch (...) {
    // TODO : Name resolution exception ...
  }
  
  // TODO : Event Element Start ...

  if (CurrentToken (TOKEN_SLASH_ANGLE_RIGHT)) {
    scanner.AcceptToken (TOKEN_SLASH_ANGLE_RIGHT);    
    // TODO : Event Element End ...
  
  } else {
    scanner.ReadToken (TOKEN_ANGLE_RIGHT);
    ParseContent ();
    scanner.ReadToken (TOKEN_ANGLE_LEFT_SLASH);

    if (name.Match (ParseQualifiedName ()) {
      // TODO : Event Element End ...
    } else {
      // TODO : Error unmatched tag ...
    }
    
    AcceptToken (TOKEN_ANGLE_RIGHT);
  }
}

/**********************************************************************************************
 *
 * ParseAttribute
 *
 * Attribute = QualifiedName '=' AttributeValue ;
 *
 *********************************************************************************************/ 
void Parser::ParseAttribute () {
  QualifiedName name = ParseQualifiedName ();
  AcceptToken (TOKEN_EQUAL);
  return Attribute (name, ReadLiteral ());
}

/**********************************************************************************************
 *
 * ParseContent
 *
 * Content = CharacterData ? (Element CharacterData ?)* ;
 *
 *********************************************************************************************/    
void Parser::ParseContent () {
  ParseCharacterData ();
  while (CurrentToken (TOKEN_ELEMENT_TAG_START)) {
    try {
      ParseElement ();
      ParseCharacterData ();
    } catch (...) {
      // Handle Element level and stream level exceptions ...
    }
  }
}

/**********************************************************************************************
 *
 * ParseElement
 *
 * Element = "<" QualifiedName { Attribute }  ( ">" Content "</" QualifiedName ">" | "/>" ) ;
 *
 *********************************************************************************************/
void Parser::ParseElement () {
  AcceptToken (TOKEN_ANGLE_LEFT);
  StartNamingScope ();

  Element element;  
  QualifiedName name = ParseQualifiedName ();
  
  while (! CurrentToken (TOKEN_ANGLE_RIGHT) && ! CurrentToken (TOKEN_SLASH_ANGLE_RIGHT))) {
    try {
      Attribute attribute = ParseAttribute ();
      if (attribute.getLocalName () == "xmlns") {
        addNamespace (attribute.getNamespace (), attribute.getValue ());
      } else {
        attributes.add (attribute);
      }

    } catch (...) {
      // TODO ...
    }
  }
  
  try {
    // Name resolution ...
    element.setName (ResolveName (name));
    for (/* */) {
      element.addAttribute (ResolveName (attribute.getQualifiedName ()), attribute.getValue ());
    }

  } catch (...) {
    // TODO ...
  }
  
  // TODO : Event Element Start ...

  if (CurrentToken (TOKEN_SLASH_ANGLE_RIGHT)) {
    AcceptToken (TOKEN_SLASH_ANGLE_RIGHT);    
    // TODO : Event Element End ...
  
  } else {
    AcceptToken (TOKEN_ANGLE_RIGHT);
    ParseContent ();
    AcceptToken (TOKEN_ANGLE_LEFT_SLASH);

    if (name.Match (ParseQualifiedName ()) {
      // TODO : Event Element End ...
    } else {
      // TODO : Error unmatched tag ...
    }
    
    AcceptToken (TOKEN_ANGLE_RIGHT);
  }

  EndNamingScope ();
}



